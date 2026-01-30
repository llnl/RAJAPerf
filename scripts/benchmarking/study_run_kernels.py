import os
import glob
import numpy as np
import pandas as pd
import argparse

# ============= Configuration =============

ROOT_DIR = "."  # change if needed

# Use "factor" instead of "mref" in file patterns
GLOB_PATTERNS = [
    "**/*factor*kernel-run-data.csv",              # broad match
]

# Optional filter to only keep specific kernels by substring match (case-insensitive)
# Leave empty to include all kernels discovered.
KERNEL_WHITELIST = [
    # "MASS3DPA",
]

# Derivative reporting configuration
DERIV_USE_RELATIVE = True
DERIV_EPS_REL = 0.03          # relative threshold on |dy/dx| normalized by (y_range/x_range)
DERIV_EPS_ABS = 1e-4          # absolute threshold on |dy/dx|, only used if DERIV_USE_RELATIVE=False
DERIV_MIN_CONSEC = 3          # minimum consecutive points below threshold to consider a plateau run
DERIV_SMOOTH_WINDOW = 3       # moving average window for smoothing y before derivative
DERIV_MIN_FRAC_OF_MAX_Y = 0.9 # only search after reaching this fraction of max(y)
DERIV_REPORT_MAX_POINTS = 8   # limit how many points to print per series
DERIV_REPORT_ABS = True       # print |dy/dx| if True, else print signed dy/dx

FIG_DPI = 300
SHOW_PLOTS = True  # show interactive windows while also saving PNGs

# Use a non-interactive backend only when not showing plots
if not SHOW_PLOTS:
    import matplotlib
    matplotlib.use("Agg")

import matplotlib.pyplot as plt


# ============= Helper functions =============

def ensure_dir(path):
    os.makedirs(path, exist_ok=True)

def sanitize_filename(text):
    return "".join(c if c.isalnum() or c in "-_." else "_" for c in str(text))

def find_csv_files(root_dir, patterns):
    """Recursively find CSV files matching any of the given patterns."""
    all_files = []
    for pattern in patterns:
        search_pattern = os.path.join(root_dir, pattern)
        files = glob.glob(search_pattern, recursive=True)
        all_files.extend(files)
    all_files = sorted(set(all_files))  # Remove duplicates and sort
    return all_files

def _likely_header_score(line):
    """
    Score a potential header line based on presence of common column tokens.
    Higher is more likely to be the header.
    """
    tokens = [
        "Kernel",
        "Variant",
        "Problem size",
        "Problem Size",
        "Mean flops",
        "GFlop",
        "GFLOP",
        "GFLOPs",
        "GFLOPS",
    ]
    score = 0
    for t in tokens:
        if t in line:
            score += 1
    return score

def read_single_csv(path):
    """
    Read one CSV, trying to detect the header row by locating a line
    that contains key column names. Returns a DataFrame or None.
    """
    try:
        with open(path, "r", encoding="utf-8") as f:
            lines = f.readlines()
    except Exception as e:
        print(f"Failed to read {path}: {e}")
        return None

    header_idx = None
    best_score = -1
    for i, line in enumerate(lines[:50]):  # only inspect the first 50 lines
        score = _likely_header_score(line)
        if score > best_score:
            best_score = score
            header_idx = i

    if header_idx is None:
        print(f"Could not find header in {path}, skipping.")
        return None

    try:
        df = pd.read_csv(path, header=header_idx)
    except Exception as e:
        print(f"Failed to parse CSV {path}: {e}")
        return None

    df["__source_file__"] = path
    return df

def normalize_columns(df):
    """
    Normalize common column names to a standard set if possible.
    """
    candidates = {
        # Standard name : possible variants
        "Kernel": ["Kernel", "Kernel name", "Benchmark", "Test"],
        "Variant": ["Variant", "Implementation", "Policy", "Config", "Backend", "Suite"],
        "Problem size": [
            "Problem size", "Problem Size", "Size", "N", "DOF", "Elements",
            "ProblemSize", "Problem-size"
        ],
        "Mean flops (gigaFLOP per sec.)": [
            "Mean flops (gigaFLOP per sec.)",
            "Mean flops (GFlop/s)",
            "Mean Flops (GFlop/s)",
            "GFLOP/s", "GFLOPs/s", "GFLOPS", "GFlops/s", "GFlop/s", "GF/s",
            "Mean GFLOP/s", "Mean GFLOPs/s"
        ],
    }

    new_col_map = {}
    # strip whitespace from existing columns first
    df = df.rename(columns={c: c.strip() for c in df.columns})

    for standard_name, names in candidates.items():
        for c in names:
            if c in df.columns:
                new_col_map[c] = standard_name
                break  # first match wins

    df = df.rename(columns=new_col_map)
    return df

def _moving_average(y, window):
    if window is None or window <= 1 or len(y) < 3:
        return y
    window = max(2, int(window))
    kernel = np.ones(window, dtype=float) / float(window)
    return np.convolve(y, kernel, mode="same")

def _find_first_run(mask, min_len):
    """Return the start index and run length of the first run of True with length >= min_len."""
    run = 0
    for i, v in enumerate(mask):
        if v:
            run += 1
            if run >= min_len:
                start = i - run + 1
                j = i + 1
                while j < len(mask) and mask[j]:
                    j += 1
                return start, j - start
        else:
            run = 0
    return None, 0

def classify_backend_from_variant(variant):
    """
    Heuristic classification of backend based on the Variant string.
    Captures common cases even if names do not end with specific suffixes.
    """
    s = str(variant).strip()
    low = s.lower()
    if "hip" in low:
        return "HIP"
    if "cuda" in low:
        return "CUDA"
    if "openmp" in low or low.endswith("_omp") or " omp" in low or low.startswith("omp"):
        return "OpenMP"
    if "seq" in low or "serial" in low or "baseline" in low or "sequential" in low:
        return "Seq"
    return "Unknown"

def report_near_zero_derivative_points(
    x,
    y,
    backend_label,
    kernel,
    variant,
    use_relative=DERIV_USE_RELATIVE,
    eps_rel=DERIV_EPS_REL,
    eps_abs=DERIV_EPS_ABS,
    min_consecutive=DERIV_MIN_CONSEC,
    smooth_window=DERIV_SMOOTH_WINDOW,
    min_frac_of_max_y=DERIV_MIN_FRAC_OF_MAX_Y,
    max_points=DERIV_REPORT_MAX_POINTS,
    report_abs=DERIV_REPORT_ABS,
):
    """
    Prints lines "Problem size=<x>, dy/dx=<val>" for points with small enough derivative.
    Uses either a relative threshold or an absolute slope threshold.
    Focuses on the near-peak region to avoid early flat areas.
    """
    x = np.asarray(x, dtype=float)
    y = np.asarray(y, dtype=float)

    # Aggregate duplicate x values by averaging y
    if len(x) != len(np.unique(x)):
        tmp = pd.DataFrame({"x": x, "y": y}).groupby("x", as_index=False)["y"].mean()
        x = tmp["x"].values
        y = tmp["y"].values

    # Sort by x
    order = np.argsort(x)
    x = x[order]
    y = y[order]

    if len(x) < max(3, min_consecutive):
        print(
            f"[DERIV] Backend={backend_label}, Kernel={kernel}, Variant={variant}: not enough points for derivative analysis"
        )
        return

    # Optional smoothing
    y_sm = _moving_average(y, smooth_window)

    x_range = float(x.max() - x.min())
    if x_range == 0.0:
        print(
            f"[DERIV] Backend={backend_label}, Kernel={kernel}, Variant={variant}: zero x-range, cannot compute derivative"
        )
        return

    deriv = np.gradient(y_sm, x)  # dy/dx, same length as x

    # Restrict to near-peak region if requested
    search_mask = np.ones_like(deriv, dtype=bool)
    y_range = float(y_sm.max() - y_sm.min())
    if min_frac_of_max_y is not None and 0.0 < min_frac_of_max_y < 1.0 and y_range > 0:
        thresh_y = y_sm.max() * float(min_frac_of_max_y)
        search_mask = y_sm >= thresh_y

    if use_relative:
        # Normalize slope by typical scale y_range/x_range for a dimensionless measure
        norm_factor = (y_range / x_range) if y_range > 0 else 1.0
        deriv_norm = np.abs(deriv) / norm_factor
        near_zero_mask = (deriv_norm <= float(eps_rel)) & search_mask
    else:
        near_zero_mask = (np.abs(deriv) <= float(eps_abs)) & search_mask

    # Prefer the first sustained run of small derivatives
    start_idx, run_len = _find_first_run(near_zero_mask, int(min_consecutive))

    if start_idx is not None:
        run_indices = np.arange(start_idx, start_idx + run_len)
        # Downsample to at most max_points for readability
        if len(run_indices) > max_points:
            picks_rel = np.linspace(0, len(run_indices) - 1, num=max_points)
            pick = run_indices[np.round(picks_rel).astype(int)]
        else:
            pick = run_indices
        print(
            f"[DERIV] Backend={backend_label}, Kernel={kernel}, Variant={variant}: sustained near-zero derivative region found, points={run_len}"
        )
    else:
        # Fallback: choose up to max_points with smallest slope in the search region
        candidates = np.where(search_mask)[0]
        if candidates.size == 0:
            print(
                f"[DERIV] Backend={backend_label}, Kernel={kernel}, Variant={variant}: no valid search region for derivative analysis"
            )
            return

        if use_relative:
            norm_factor = (y_range / x_range) if y_range > 0 else 1.0
            deriv_norm = np.abs(deriv) / (norm_factor if norm_factor > 0 else 1.0)
            order_c = np.argsort(deriv_norm[candidates])
        else:
            order_c = np.argsort(np.abs(deriv[candidates]))
        pick = candidates[order_c[:max_points]]
        print(
            f"[DERIV] Backend={backend_label}, Kernel={kernel}, Variant={variant}: no sustained plateau, showing {len(pick)} smallest-slope points"
        )

    # Ensure sorted by x before printing
    pick = np.array(sorted(pick.tolist()))

    # Print lines in the requested format
    for idx in pick:
        dy = deriv[idx]
        dy_out = abs(dy) if report_abs else dy
        print(f"  Problem size={x[idx]:.6g}, dy/dx={dy_out:.6g}")

def plot_backend(df_backend, backend_label, fig_dir, show_plots, fig_dpi):
    """
    For a given backend:
      - One figure per Kernel with solid line and markers per Variant.
      - Prints derivative-based small-slope points per Variant.
      - Saves each figure as PNG.
      - Shows figures interactively if requested.
    """
    if df_backend.empty:
        print(f"\nNo data for backend {backend_label}.")
        return

    kernels = sorted(df_backend["Kernel"].dropna().unique())
    variants = sorted(df_backend["Variant"].dropna().unique())

    # Use a larger palette in case many variants exist
    cmap = plt.cm.tab20
    colors = [cmap(i % cmap.N) for i in range(max(1, len(variants)))]
    color_map = {v: colors[i % len(colors)] for i, v in enumerate(variants)}

    for kernel in kernels:
        df_k = df_backend[df_backend["Kernel"] == kernel]
        if df_k.empty:
            continue

        fig = plt.figure(figsize=(10, 6))

        for variant, g in df_k.groupby("Variant"):
            g_sorted = g.sort_values("Problem size")
            x = g_sorted["Problem size"].values
            y = g_sorted["Mean flops (gigaFLOP per sec.)"].values

            color = color_map.get(variant, "black")

            # Actual data curve: solid line with markers
            plt.plot(
                x,
                y,
                marker="o",
                linestyle="-",
                color=color,
                label=f"{variant}",
            )

            # Derivative-based report of small-slope points
            report_near_zero_derivative_points(
                x,
                y,
                backend_label,
                kernel,
                variant,
                use_relative=DERIV_USE_RELATIVE,
                eps_rel=DERIV_EPS_REL,
                eps_abs=DERIV_EPS_ABS,
                min_consecutive=DERIV_MIN_CONSEC,
                smooth_window=DERIV_SMOOTH_WINDOW,
                min_frac_of_max_y=DERIV_MIN_FRAC_OF_MAX_Y,
                max_points=DERIV_REPORT_MAX_POINTS,
                report_abs=DERIV_REPORT_ABS,
            )

        plt.xlabel("Problem size")
        plt.ylabel("Mean flops (gigaFLOP per sec.)")
        plt.title(f"{backend_label} backend, Kernel: {kernel}")
        plt.grid(True)
        plt.tight_layout()
        plt.legend(fontsize="small", bbox_to_anchor=(1.05, 1), loc="upper left")

        # Save figure as PNG
        kernel_safe = sanitize_filename(kernel)
        fname = f"{backend_label}_Kernel-{kernel_safe}.png"
        fig_path = os.path.join(fig_dir, fname)
        plt.savefig(fig_path, dpi=fig_dpi, bbox_inches="tight")
        print(f"[SAVE] Figure saved to: {fig_path}")

        if show_plots:
            plt.show()
        else:
            plt.close(fig)

# ============= Main logic =============

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--output-dir", required=True, help="Output directory")
    args = parser.parse_args()
    
    # Output and plotting configuration
    OUTPUT_DIR = args.output_dir
    FIG_DIR = os.path.join(OUTPUT_DIR, "figures")
    COMBINED_CSV_PATH = os.path.join(OUTPUT_DIR, "combined_table.csv")
    
    ensure_dir(OUTPUT_DIR)
    ensure_dir(FIG_DIR)

    files = find_csv_files(ROOT_DIR, GLOB_PATTERNS)
    if not files:
        print(f"No files matching patterns {GLOB_PATTERNS} found under '{ROOT_DIR}'")
        return

    print("Found CSV files:")
    for f in files:
        print("  ", f)

    dfs = []
    for path in files:
        df = read_single_csv(path)
        if df is None:
            continue
        df = normalize_columns(df)

        # Verify required columns exist post-normalization, else report and skip
        required_any = {"Kernel", "Variant", "Problem size", "Mean flops (gigaFLOP per sec.)"}
        if not required_any.issubset(set(df.columns)):
            print(f"[SKIP] {path} missing required columns after normalization.")
            print("       Columns present:", list(df.columns))
            continue

        dfs.append(df)

    if not dfs:
        print("No CSV files could be parsed with required columns.")
        return

    combined_df = pd.concat(dfs, ignore_index=True)

    # Basic cleaning
    combined_df["Kernel"] = combined_df["Kernel"].astype(str).str.strip()
    combined_df["Variant"] = combined_df["Variant"].astype(str).str.strip()

    # Optional kernel filter
    if KERNEL_WHITELIST:
        wl = [w.lower() for w in KERNEL_WHITELIST]
        combined_df = combined_df[
            combined_df["Kernel"].str.lower().apply(lambda k: any(w in k for w in wl))
        ]
        if combined_df.empty:
            print("After applying KERNEL_WHITELIST, no rows remain.")
            return

    # Convert numeric columns
    combined_df["Problem size"] = pd.to_numeric(combined_df["Problem size"], errors="coerce")
    combined_df["Mean flops (gigaFLOP per sec.)"] = pd.to_numeric(
        combined_df["Mean flops (gigaFLOP per sec.)"], errors="coerce"
    )

    # Drop rows without x or y
    before_drop = len(combined_df)
    combined_df = combined_df.dropna(subset=["Problem size", "Mean flops (gigaFLOP per sec.)"])
    dropped = before_drop - len(combined_df)
    if dropped > 0:
        print(f"[CLEAN] Dropped {dropped} rows with non-numeric Problem size or Mean flops.")

    # Save concatenated table to CSV
    ensure_dir(os.path.dirname(COMBINED_CSV_PATH))
    combined_df.to_csv(COMBINED_CSV_PATH, index=False)
    print(f"[SAVE] Combined table saved to: {COMBINED_CSV_PATH}")

    # Backend classification
    combined_df["Backend"] = combined_df["Variant"].apply(classify_backend_from_variant)

    # Quick summary to help verify MASS3DPA is present
    print("\nKernels discovered:")
    print(sorted(combined_df["Kernel"].unique()))

    print("\nCounts by Kernel and Backend:")
    summary = (
        combined_df.groupby(["Kernel", "Backend"])
        .size()
        .reset_index(name="rows")
        .sort_values(["Kernel", "Backend"])
    )
    for _, row in summary.iterrows():
        print(f"  Kernel={row['Kernel']}, Backend={row['Backend']}: rows={row['rows']}")

    # Plot aggregated "All" view so kernels appear even if backend classification is Unknown
    print("\n[Plot] Generating 'All' plots per kernel...")
    plot_backend(combined_df, "All", FIG_DIR, SHOW_PLOTS, FIG_DPI)

    # Plot per requested backends, only if data exists
    for b in ["CUDA", "HIP", "Seq", "OpenMP"]:
        df_b = combined_df[combined_df["Backend"] == b]
        if df_b.empty:
            print(f"[Plot] Skipping backend {b}, no rows.")
            continue
        print(f"\n[Plot] Generating '{b}' plots per kernel...")
        plot_backend(df_b, b, FIG_DIR, SHOW_PLOTS, FIG_DPI)

if __name__ == "__main__":
    main()
