#!/usr/bin/env python3
import os
import glob
import statistics
import argparse
from typing import List, Optional

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Try to import IPython display for notebook detection
try:
    from IPython.display import display
    from IPython import get_ipython
    IN_NOTEBOOK = get_ipython() is not None
except Exception:
    display = None
    IN_NOTEBOOK = False

# =========================
# Global constants
# =========================

RAW_FLOPS_COL = "Mean gFlops (gigaFLOP per sec.)"
SMOOTH_FLOPS_COL = "Smoothed Mean flops (gigaFLOP per sec.)"
PROBLEM_SIZE_COL = "Problem size"
VARIANT_TUNING_COL = "Variant_Tuning"
KERNEL_COL = "Kernel"
BANDWIDTH_COL = "Bandwidth (GiB per sec.)"
SMOOTH_BW_COL = "Smoothed Bandwidth (GiB per sec.)"

# =========================
# General utilities
# =========================

def ensure_dir(path: str) -> None:
    if path:
        os.makedirs(path, exist_ok=True)

def sanitize_filename(text: str) -> str:
    return "".join(c if c.isalnum() or c in "-_." else "_" for c in str(text))

def find_csv_files(root_dir: str, patterns: List[str]) -> List[str]:
    all_files: List[str] = []
    for pattern in patterns:
        search_pattern = os.path.join(root_dir, pattern)
        files = glob.glob(search_pattern, recursive=True)
        all_files.extend(files)
    all_files = sorted(set(all_files))
    return all_files

# =========================
# Header detection / reading
# =========================

def _likely_header_score(line: str) -> int:
    tokens = [
        "Kernel", "Variant", "Problem size", "Problem Size", "Mean flops",
        "GFlop", "GFLOP", "GFLOPs", "GFLOPS"
    ]
    score = 0
    for t in tokens:
        if t in line:
            score += 1
    return score

def read_single_csv(path: str) -> Optional[pd.DataFrame]:
    """Read a CSV and heuristically detect the header row."""
    try:
        with open(path, "r", encoding="utf-8") as f:
            lines = f.readlines()
    except Exception as e:
        print("Failed to read {}: {}".format(path, e))
        return None

    header_idx = None
    best_score = -1
    for i, line in enumerate(lines[:50]):
        if not line.strip():
            continue
        score = _likely_header_score(line)
        if score > best_score:
            best_score = score
            header_idx = i
            if score >= 3:
                break

    if header_idx is None:
        print("Could not find header in {}, skipping.".format(path))
        return None

    try:
        df = pd.read_csv(path, header=header_idx)
    except Exception as e:
        print("Failed to parse CSV {}: {}".format(path, e))
        return None

    df["__source_file__"] = path
    return df

# =========================
# Column normalization
# =========================

def normalize_columns(df: pd.DataFrame) -> pd.DataFrame:
    """Normalize various benchmark CSV headers to a standard schema."""
    candidates = {
        "Kernel": ["Kernel", "Kernel name", "Benchmark", "Test"],
        "Variant": ["Variant", "Implementation", "Policy", "Config", "Backend", "Suite"],
        PROBLEM_SIZE_COL: [
            "Problem size", "Problem Size", "Size", "N", "DOF", "Elements",
            "ProblemSize", "Problem-size"
        ],
        RAW_FLOPS_COL: [
            "Mean flops (gigaFLOP per sec.)",
            "Mean flops (GFlop/s)",
            "Mean Flops (GFlop/s)",
            "GFLOP/s", "GFLOPs/s", "GFLOPS", "GFlops/s", "GFlop/s", "GF/s",
            "Mean GFLOP/s", "Mean GFLOPs/s"
        ],
        BANDWIDTH_COL: [
            "Bandwidth (GiB per sec.)",
            "Bandwidth (GiB/s)",
            "Bandwidth (GB/s)",
            "Bandwidth (GiB/sec)",
            "Bandwidth (GB/sec)",
            "Mean bandwidth (GiB/s)",
            "Mean Bandwidth (GiB/s)",
            "Mean Bandwidth (GiB per sec.)"
        ],
    }

    df = df.rename(columns={c: c.strip() for c in df.columns})
    new_col_map = {}

    for standard_name, names in candidates.items():
        for c in names:
            if c in df.columns:
                new_col_map[c] = standard_name
                break

    df = df.rename(columns=new_col_map)
    return df

# =========================
# Backend / tuning classification
# =========================

def classify_backend_from_variant(variant) -> str:
    s = "" if pd.isna(variant) else str(variant).strip()
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

def classify_tuning(row: pd.Series) -> str:
    if "Tuning" in row and pd.notna(row["Tuning"]):
        return str(row["Tuning"]).strip()
    src = row.get("__source_file__", "")
    if isinstance(src, str) and src:
        return os.path.basename(src)
    return "default"

# =========================
# Build combined table
# =========================

def build_combined_table(
    root_dir: str = ".",
    glob_patterns: Optional[List[str]] = None,
    kernel_whitelist: Optional[List[str]] = None,
    verbose: bool = True,
) -> Optional[pd.DataFrame]:
    if glob_patterns is None:
        glob_patterns = ["**/*factor*kernel-run-data.csv"]

    files = find_csv_files(root_dir, glob_patterns)
    if not files:
        if verbose:
            print("No files matching patterns {} found under '{}'".format(glob_patterns, root_dir))
        return None

    if verbose:
        print("Found CSV files:")
        for f in files:
            print("  ", f)

    dfs = []
    required_cols = {KERNEL_COL, "Variant", PROBLEM_SIZE_COL, RAW_FLOPS_COL}

    for path in files:
        df = read_single_csv(path)
        if df is None:
            continue
        df = normalize_columns(df)
        missing = required_cols - set(df.columns)
        if missing:
            if verbose:
                print("[SKIP] {} missing required columns after normalization: {}".format(path, sorted(missing)))
                print("       Columns present:", list(df.columns))
            continue
        dfs.append(df)

    if not dfs:
        if verbose:
            print("No CSV files could be parsed with required columns.")
        return None

    combined_df = pd.concat(dfs, ignore_index=True)

    combined_df[KERNEL_COL] = combined_df[KERNEL_COL].astype(str).str.strip()
    combined_df["Variant"] = combined_df["Variant"].astype(str).str.strip()

    if kernel_whitelist:
        wl = [w.lower() for w in kernel_whitelist]
        kernel_series = combined_df[KERNEL_COL].fillna("").astype(str).str.lower()
        mask = kernel_series.apply(lambda k: any(w in k for w in wl))
        combined_df = combined_df[mask]
        if combined_df.empty:
            if verbose:
                print("After applying kernel_whitelist, no rows remain.")
            return None

    combined_df[PROBLEM_SIZE_COL] = pd.to_numeric(combined_df[PROBLEM_SIZE_COL], errors="coerce")
    combined_df[RAW_FLOPS_COL] = pd.to_numeric(combined_df[RAW_FLOPS_COL], errors="coerce")
    if BANDWIDTH_COL in combined_df.columns:
        combined_df[BANDWIDTH_COL] = pd.to_numeric(combined_df[BANDWIDTH_COL], errors="coerce")

    before_drop = len(combined_df)
    combined_df = combined_df.dropna(subset=[PROBLEM_SIZE_COL, RAW_FLOPS_COL])
    dropped = before_drop - len(combined_df)
    if verbose and dropped > 0:
        print("[CLEAN] Dropped {} rows with non-numeric {} or {}.".format(
            dropped, PROBLEM_SIZE_COL, RAW_FLOPS_COL))

    combined_df["Backend"] = combined_df["Variant"].apply(classify_backend_from_variant)
    combined_df["Tuning"] = combined_df.apply(classify_tuning, axis=1)

    combined_df[VARIANT_TUNING_COL] = (
        combined_df["Variant"].astype(str) + "-" + combined_df["Tuning"].astype(str)
    )

    return combined_df

# =========================
# Smoothing and saturation point utilities
# =========================

def moving_median_smooth(y, k: int = 5):
    n = len(y)
    if n == 0:
        return []
    m = (k - 1) // 2
    y_smooth = []
    for i in range(n):
        start = max(0, i - m)
        end = min(n - 1, i + m)
        window = y[start:end + 1]
        y_smooth.append(statistics.median(window))
    return y_smooth

def find_saturation_point(x, y_smooth, eps: float = 0.1, w: int = 3):
    """
    Returns the index of the x value at the first saturation point,
    or None if none is found. 
    First, the method sets y_max = max(y_smooth) and then tries to
    find the first run of y_smooth values of length >= w where
    y_smooth >= (1 - eps) * y_max. If such a run is found, the 
    function returns the index of the first y_smooth value in the run.
    Second, if the first attempt does not find such a run, it defines
    y_end = y_smooth[n - 1], where n is the length of y_smooth. Then,
    it searches for the first run of y_smooth values of length >= w
    where abs( (y_smooth[i] - y_end) / y_end ) <= eps. If such a run is
    found, the function returns the index of the first y_smooth value
    in the run.
    """

    if len(x) < w or len(y_smooth) < w:
        return None

    n = len(y_smooth)

# =========================
# First attempt to find index of saturation point
# =========================
    y_max = max(y_smooth)
    threshold = (1.0 - eps) * y_max
    run_length = 0
    run_start_idx = None

    for i in range(n):
        if y_smooth[i] >= threshold:
            if run_length == 0:
                run_start_idx = i
            run_length += 1
            if run_length >= w:
                return run_start_idx
        else:
            run_length = 0
            run_start_idx = None

# =========================
# Second attempt to find index of saturation point
# =========================

    y_end = y_smooth[n - 1]
    threshold = eps
    run_length = 0
    run_start_idx = None
   
    for i in range(n):
        if abs( (y_smooth[i] - y_end) / y_end ) <= threshold:
            if run_length == 0:
                run_start_idx = i
            run_length += 1
            if run_length >= w:
                return run_start_idx
        else:
            run_length = 0
            run_start_idx = None

    return None

# =========================
# Plotting for a single kernel (FLOPs) with FOM output as DataFrame
# =========================

def plot_kernel(
    df: pd.DataFrame,
    kernel: str,
    k: int = 5,
    eps: float = 0.1,
    w: int = 3,
    save_dir: Optional[str] = None,
):
    plt.figure(figsize=(18, 7))
    variants = df[VARIANT_TUNING_COL].unique()
    colors = plt.cm.tab10.colors

    if SMOOTH_FLOPS_COL not in df.columns:
        df[SMOOTH_FLOPS_COL] = np.nan

    for idx, variant in enumerate(variants):
        subdf = df[df[VARIANT_TUNING_COL] == variant].copy()
        subdf = subdf.sort_values(PROBLEM_SIZE_COL)
        x = subdf[PROBLEM_SIZE_COL].astype(float).values
        y = subdf[RAW_FLOPS_COL].astype(float).values
        if len(x) == 0:
            continue

        y_smooth = moving_median_smooth(list(y), k=k)

        for xi, yi in zip(x, y_smooth):
            mask = (
                (df[VARIANT_TUNING_COL] == variant)
                & (df[PROBLEM_SIZE_COL] == xi)
            )
            df.loc[mask, SMOOTH_FLOPS_COL] = yi

        plt.plot(
            x, y, "-o",
            label="{} (raw)".format(variant),
            markersize=8,
            color=colors[idx % len(colors)],
            markerfacecolor=colors[idx % len(colors)],
            markeredgewidth=0,
        )

        plt.plot(
            x, y_smooth, "--",
            label="{} (smoothed)".format(variant),
            linewidth=3,
            color=colors[idx % len(colors)],
        )

    plt.title("Kernel: {}".format(kernel), fontsize=22)
    plt.xlabel("Problem size (bytes)", fontsize=18)
    plt.ylabel("Mean flops (GFLOP per sec.)", fontsize=18)
    plt.grid(True, which="both", linestyle="--", linewidth=1)
    plt.xticks(fontsize=16)
    plt.yticks(fontsize=16)
    plt.legend(loc="upper left", bbox_to_anchor=(1.05, 1), fontsize=16, frameon=False)
    plt.tight_layout(rect=[0, 0, 0.75, 1])

    if save_dir is not None:
        ensure_dir(save_dir)
        fname = os.path.join(save_dir, "{}_flops.png".format(sanitize_filename(kernel)))
        plt.savefig(fname, dpi=200)
        print("Saved plot to {}".format(fname))
    if IN_NOTEBOOK:
        plt.show()
    else:
        plt.close()

    # FOM-style output as DataFrame
    fom_rows = []
    for variant in variants:
        subdf = df[df[VARIANT_TUNING_COL] == variant].copy()
        subdf = subdf.sort_values(PROBLEM_SIZE_COL)
        x = subdf[PROBLEM_SIZE_COL].astype(float).values
        y_smooth = subdf[SMOOTH_FLOPS_COL].astype(float).values
        sat_size = ""
        sat_flops_raw = ""
        sat_bw = ""

        sat_idx = find_saturation_point(x, y_smooth, eps, w)

        if sat_idx is not None:
            sat_size = x[sat_idx]
            mask = (subdf[PROBLEM_SIZE_COL] == sat_size)
            raw_at_sat = subdf.loc[mask, RAW_FLOPS_COL]
            if not raw_at_sat.empty and pd.notna(raw_at_sat.iloc[0]):
                sat_flops_raw = raw_at_sat.iloc[0]

            if BANDWIDTH_COL in subdf.columns:
                bw_at_sat = subdf.loc[mask, BANDWIDTH_COL]
                bw_non_nan = bw_at_sat.dropna()
                if not bw_non_nan.empty:
                    sat_bw = bw_non_nan.iloc[0]
                else:
                    sat_bw = ""
            else:
                sat_bw = ""
        else:
            print(
                "[INFO] No saturation point found for kernel '{}' "
                "variant '{}' (eps={}, w={})".format(kernel, variant, eps, w)
            )

        fom_rows.append({
            "Kernel": f"{kernel}-{variant}",
            "Sat Problem Size": sat_size if sat_size != "" else "N/A",
            "Sat GFLOP/s": sat_flops_raw if sat_flops_raw != "" else "N/A",
            "Sat B/W (GiB per sec.)": sat_bw if sat_bw != "" else "N/A",
        })

    fom_df = pd.DataFrame(fom_rows)
    if IN_NOTEBOOK and display is not None:
        display(fom_df)
    else:
        print(fom_df.to_string(index=False))

# =========================
# Plotting for a single kernel (Bandwidth)
# =========================

def plot_kernel_bandwidth(
    df: pd.DataFrame,
    kernel: str,
    k: int = 5,
    eps: float = 0.1,
    w: int = 3,
    save_dir: Optional[str] = None,
):
    if BANDWIDTH_COL not in df.columns:
        print(f"[INFO] No '{BANDWIDTH_COL}' column for kernel '{kernel}', skipping bandwidth plot.")
        return

    plt.figure(figsize=(18, 7))
    variants = df[VARIANT_TUNING_COL].unique()
    colors = plt.cm.tab10.colors

    if SMOOTH_BW_COL not in df.columns:
        df[SMOOTH_BW_COL] = np.nan

    for idx, variant in enumerate(variants):
        subdf = df[df[VARIANT_TUNING_COL] == variant].copy()
        subdf = subdf.sort_values(PROBLEM_SIZE_COL)

        x = subdf[PROBLEM_SIZE_COL].astype(float).values
        y_bw = subdf[BANDWIDTH_COL].astype(float).values

        if len(x) == 0:
            continue

        y_bw_smooth = moving_median_smooth(list(y_bw), k=k)

        for xi, yi in zip(x, y_bw_smooth):
            mask = (
                (df[VARIANT_TUNING_COL] == variant)
                & (df[PROBLEM_SIZE_COL] == xi)
            )
            df.loc[mask, SMOOTH_BW_COL] = yi

        color = colors[idx % len(colors)]

        plt.plot(
            x, y_bw, "-o",
            label=f"{variant} (raw B/W)",
            markersize=8,
            color=color,
            markerfacecolor=color,
            markeredgewidth=0,
        )

        plt.plot(
            x, y_bw_smooth, "--",
            label=f"{variant} (smoothed B/W)",
            linewidth=3,
            color=color,
        )

    plt.title(f"Kernel: {kernel} - Bandwidth", fontsize=22)
    plt.xlabel("Problem size", fontsize=18)
    plt.ylabel("Bandwidth (GiB per sec.)", fontsize=18)
    plt.grid(True, which="both", linestyle="--", linewidth=1)
    plt.xticks(fontsize=16)
    plt.yticks(fontsize=16)
    plt.legend(loc="upper left", bbox_to_anchor=(1.05, 1), fontsize=16, frameon=False)
    plt.tight_layout(rect=[0, 0, 0.75, 1])

    if save_dir is not None:
        ensure_dir(save_dir)
        fname = os.path.join(save_dir, "{}_bandwidth.png".format(sanitize_filename(kernel)))
        plt.savefig(fname, dpi=200)
        print(f"Saved bandwidth plot to {fname}")
    if IN_NOTEBOOK:
        plt.show()
    else:
        plt.close()

# =========================
# Smooth and plot all kernels (FLOPs and Bandwidth)
# =========================

def smooth_and_plot_all_kernels(
    df: pd.DataFrame,
    k: int = 5,
    eps: float = 0.1,
    w: int = 3,
    save_dir: Optional[str] = None,
) -> pd.DataFrame:
    df = df.dropna(subset=[KERNEL_COL, PROBLEM_SIZE_COL, RAW_FLOPS_COL, VARIANT_TUNING_COL]).copy()
    df[SMOOTH_FLOPS_COL] = np.nan

    if BANDWIDTH_COL in df.columns:
        df[SMOOTH_BW_COL] = np.nan

    kernels = df[KERNEL_COL].unique()
    print("Found {} kernels.".format(len(kernels)))

    for kernel in kernels:
        tmp = df[df[KERNEL_COL] == kernel].copy()

        plot_kernel(tmp, kernel, k=k, eps=eps, w=w, save_dir=save_dir)
        df.loc[df[KERNEL_COL] == kernel, SMOOTH_FLOPS_COL] = tmp[SMOOTH_FLOPS_COL]

        if BANDWIDTH_COL in df.columns:
            plot_kernel_bandwidth(tmp, kernel, k=k, eps=eps, w=w, save_dir=save_dir)
            if SMOOTH_BW_COL in tmp.columns:
                df.loc[df[KERNEL_COL] == kernel, SMOOTH_BW_COL] = tmp[SMOOTH_BW_COL]

    return df

# =========================
# Per-kernel tables (raw and smoothed)
# =========================

def save_kernel_tables(
    df: pd.DataFrame,
    outdir: str = "kernel_tables",
) -> None:
    ensure_dir(outdir)
    kernels = df[KERNEL_COL].unique()
    for kernel in kernels:
        df_kernel = df[df[KERNEL_COL] == kernel]
        raw_table = df_kernel.pivot_table(
            index=PROBLEM_SIZE_COL,
            columns=VARIANT_TUNING_COL,
            values=RAW_FLOPS_COL,
        )
        raw_table = raw_table.sort_index()
        raw_csv_path = os.path.join(outdir, "{}_flops_raw.csv".format(sanitize_filename(kernel)))
        raw_table.to_csv(raw_csv_path)
        smooth_table = df_kernel.pivot_table(
            index=PROBLEM_SIZE_COL,
            columns=VARIANT_TUNING_COL,
            values=SMOOTH_FLOPS_COL,
        )
        smooth_table = smooth_table.sort_index()
        smooth_csv_path = os.path.join(outdir, "{}_flops_smoothed.csv".format(sanitize_filename(kernel)))
        smooth_table.to_csv(smooth_csv_path)

        if BANDWIDTH_COL in df_kernel.columns and SMOOTH_BW_COL in df_kernel.columns:
            bw_raw_table = df_kernel.pivot_table(
                index=PROBLEM_SIZE_COL,
                columns=VARIANT_TUNING_COL,
                values=BANDWIDTH_COL,
            ).sort_index()
            bw_raw_csv_path = os.path.join(outdir, "{}_bandwidth_raw.csv".format(sanitize_filename(kernel)))
            bw_raw_table.to_csv(bw_raw_csv_path)

            bw_smooth_table = df_kernel.pivot_table(
                index=PROBLEM_SIZE_COL,
                columns=VARIANT_TUNING_COL,
                values=SMOOTH_BW_COL,
            ).sort_index()
            bw_smooth_csv_path = os.path.join(outdir, "{}_bandwidth_smoothed.csv".format(sanitize_filename(kernel)))
            bw_smooth_table.to_csv(bw_smooth_csv_path)

            print("Saved: {}, {}, {}, {}".format(
                raw_csv_path, smooth_csv_path, bw_raw_csv_path, bw_smooth_csv_path))
        else:
            print("Saved: {}, {}".format(raw_csv_path, smooth_csv_path))

# =========================
# Per-kernel saturation curve data (raw + smoothed in one file)
# =========================

def save_saturation_curve_data(
    df: pd.DataFrame,
    outdir: str = "saturation-curve-data",
) -> None:
    ensure_dir(outdir)
    kernels = df[KERNEL_COL].unique()
    for kernel in kernels:
        df_kernel = df[df[KERNEL_COL] == kernel].copy()
        raw_table = df_kernel.pivot_table(
            index=PROBLEM_SIZE_COL,
            columns=VARIANT_TUNING_COL,
            values=RAW_FLOPS_COL,
        )
        smooth_table = df_kernel.pivot_table(
            index=PROBLEM_SIZE_COL,
            columns=VARIANT_TUNING_COL,
            values=SMOOTH_FLOPS_COL,
        )
        raw_table = raw_table.sort_index()
        smooth_table = smooth_table.reindex(
            index=raw_table.index,
            columns=raw_table.columns,
        )
        combined = pd.DataFrame(index=raw_table.index)
        for vt in raw_table.columns:
            raw_col_name = f"{vt} (raw)"
            smooth_col_name = f"{vt} (smoothed)"
            combined[raw_col_name] = raw_table[vt]
            combined[smooth_col_name] = smooth_table[vt]

        if BANDWIDTH_COL in df_kernel.columns and SMOOTH_BW_COL in df_kernel.columns:
            bw_raw_table = df_kernel.pivot_table(
                index=PROBLEM_SIZE_COL,
                columns=VARIANT_TUNING_COL,
                values=BANDWIDTH_COL,
            ).sort_index()
            bw_smooth_table = df_kernel.pivot_table(
                index=PROBLEM_SIZE_COL,
                columns=VARIANT_TUNING_COL,
                values=SMOOTH_BW_COL,
            ).sort_index()
            bw_smooth_table = bw_smooth_table.reindex(
                index=bw_raw_table.index,
                columns=bw_raw_table.columns,
            )
            for vt in bw_raw_table.columns:
                bw_raw_col_name = f"{vt} (raw B/W)"
                bw_smooth_col_name = f"{vt} (smoothed B/W)"
                combined[bw_raw_col_name] = bw_raw_table[vt]
                combined[bw_smooth_col_name] = bw_smooth_table[vt]

        combined = combined.reset_index().rename(columns={PROBLEM_SIZE_COL: "Problem size"})
        out_path = os.path.join(outdir, f"{sanitize_filename(kernel)}.csv")
        combined.to_csv(out_path, index=False)
        print(f"Saved saturation curve data: {out_path}")

# =========================
# Per-kernel FOM tables (saturation points, with units, NO Sat Smoothed B/W)
# =========================

def save_fom_tables(
    df: pd.DataFrame,
    outdir: str = "FOM",
) -> None:
    ensure_dir(outdir)
    kernels = df[KERNEL_COL].unique()
    for kernel in kernels:
        df_kernel = df[df[KERNEL_COL] == kernel].copy()
        variant_tunings = df_kernel[VARIANT_TUNING_COL].unique()
        rows = []
        for vt in variant_tunings:
            subdf = df_kernel[df_kernel[VARIANT_TUNING_COL] == vt].copy()
            subdf = subdf.sort_values(PROBLEM_SIZE_COL)
            x = subdf[PROBLEM_SIZE_COL].astype(float).values
            y_smooth = subdf[SMOOTH_FLOPS_COL].astype(float).values
            sat_size = None
            sat_flops_raw = ""
            sat_bw = ""

            eps = 0.1
            w = 3
            sat_idx = find_saturation_point(x, y_smooth, eps, w)

            if sat_idx is not None:
                sat_size = x[sat_idx]
                mask = (subdf[PROBLEM_SIZE_COL] == sat_size)
                raw_at_sat = subdf.loc[mask, RAW_FLOPS_COL]
                if not raw_at_sat.empty and pd.notna(raw_at_sat.iloc[0]):
                    sat_flops_raw = raw_at_sat.iloc[0]
                if BANDWIDTH_COL in subdf.columns:
                    bw_at_sat = subdf.loc[mask, BANDWIDTH_COL]
                    bw_non_nan = bw_at_sat.dropna()
                    if not bw_non_nan.empty:
                        sat_bw = bw_non_nan.iloc[0]
                    else:
                        sat_bw = ""
                else:
                    print(
                        "[WARN] Bandwidth column missing for kernel '{}' variant '{}' "
                        "while computing saturation".format(kernel, vt)
                    )
                    sat_bw = ""
            else:
                print(
                    "[INFO] No saturation point found for kernel '{}' variant '{}' "
                    "when building FOM table".format(kernel, vt)
                )

            def na_if_empty(value):
                return value if value not in [None, ""] else "N/A"

            rows.append({
                "Kernel": f"{kernel}-{vt}",
                "Sat Problem Size": na_if_empty(sat_size),
                "Sat GFLOP/s": na_if_empty(sat_flops_raw),
                "Sat B/W (GiB per sec.)": na_if_empty(sat_bw),
            })
        out_path = os.path.join(outdir, f"{sanitize_filename(kernel)}.csv")
        pd.DataFrame(rows).to_csv(out_path, index=False)
        print(f"Saved FOM table: {out_path}")

# =========================
# Main pipeline: callable from notebook or CLI
# =========================

def run_pipeline(
    root_dir: str = ".",
    output_dir: str = "./output",
    glob_patterns: Optional[List[str]] = None,
    kernel_whitelist: Optional[List[str]] = None,
    smooth_window: int = 5,
    saturation_eps: float = 0.1,
    saturation_w: int = 3,
) -> Optional[pd.DataFrame]:
    if glob_patterns is None:
        glob_patterns = ["**/*factor*kernel-run-data.csv"]

    ensure_dir(output_dir)
    fig_dir = os.path.join(output_dir, "figures")
    ensure_dir(fig_dir)

    combined_df = build_combined_table(
        root_dir=root_dir,
        glob_patterns=glob_patterns,
        kernel_whitelist=kernel_whitelist,
        verbose=True,
    )
    if combined_df is None:
        print("No combined table created.")
        return None

    combined_csv_path = os.path.join(output_dir, "combined_table.csv")
    combined_df.to_csv(combined_csv_path, index=False)
    print("[SAVE] Combined table saved to {}".format(combined_csv_path))

    df_smoothed = smooth_and_plot_all_kernels(
        combined_df,
        k=smooth_window,
        eps=saturation_eps,
        w=saturation_w,
        save_dir=fig_dir,
    )

    output_variant_tuning_path = os.path.join(output_dir, "output_with_variant_tuning.csv")
    df_smoothed.to_csv(output_variant_tuning_path, index=False)
    print("[SAVE] Smoothed combined table saved to {}".format(output_variant_tuning_path))

    save_kernel_tables(df_smoothed, outdir=output_dir)
    saturation_dir = os.path.join(output_dir, "saturation-curve-data")
    save_saturation_curve_data(df_smoothed, outdir=saturation_dir)
    fom_dir = os.path.join(output_dir, "FOM")
    save_fom_tables(df_smoothed, outdir=fom_dir)

    return df_smoothed

# =========================
# CLI entry point
# =========================

def main_cli():
    parser = argparse.ArgumentParser(
        description="Combine benchmark CSVs, smooth FLOPs/Bandwidth and generate plots/tables."
    )
    parser.add_argument("--root-dir", default=".", help="Root directory to search for CSV files (default: '.')")
    parser.add_argument("--output-dir", default="./output", help="Directory to write outputs (default: './output')")
    parser.add_argument(
        "--glob-pattern",
        action="append",
        default=None,
        help="Glob pattern for CSV files (can be repeated). Default: **/*factor*kernel-run-data.csv",
    )
    parser.add_argument(
        "--kernel-whitelist",
        nargs="*",
        default=None,
        help="List of substrings to filter Kernel names by (case insensitive, default: none).",
    )
    parser.add_argument("--smooth-window", type=int, default=5, help="Moving median window size (default: '5')")
    parser.add_argument("--saturation-eps", type=float, default=0.1, help="Epsilon for saturation threshold (default: '0.1')")
    parser.add_argument("--saturation-w", type=int, default=3, help="Consecutive points needed for saturation (default: '3')")

    args = parser.parse_args()

    run_pipeline(
        root_dir=args.root_dir,
        output_dir=args.output_dir,
        glob_patterns=args.glob_pattern,
        kernel_whitelist=args.kernel_whitelist,
        smooth_window=args.smooth_window,
        saturation_eps=args.saturation_eps,
        saturation_w=args.saturation_w,
    )

# =========================
# Notebook helper (no argparse)
# =========================

def main_notebook(
    root_dir: str = ".",
    output_dir: str = "./output",
    glob_patterns: Optional[List[str]] = None,
    kernel_whitelist: Optional[List[str]] = None,
    smooth_window: int = 5,
    saturation_eps: float = 0.1,
    saturation_w: int = 3,
):
    return run_pipeline(
        root_dir=root_dir,
        output_dir=output_dir,
        glob_patterns=glob_patterns,
        kernel_whitelist=kernel_whitelist,
        smooth_window=smooth_window,
        saturation_eps=saturation_eps,
        saturation_w=saturation_w,
    )

if __name__ == "__main__":
    if not IN_NOTEBOOK:
        main_cli()
