import os
import re
from pathlib import Path

import pandas as pd


def find_build_folder(path: Path) -> str:
    """
    Given a file path, return the first parent directory whose name starts with 'build_'.
    If none is found, return an empty string.
    """
    for parent in path.parents:
        if parent.name.startswith("build_"):
            return parent.name
    return ""


def _normalize_columns(df: pd.DataFrame) -> pd.DataFrame:
    """
    Normalize and deduplicate columns, then merge any duplicate Kernel/Tuning columns
    into single columns named 'Kernel' and 'Tuning'.
    """
    # Strip leading/trailing whitespace from column names
    df = df.copy()
    df.columns = [c.strip() for c in df.columns]

    # Merge multiple Kernel columns into one
    kernel_cols = [c for c in df.columns if c.lower().startswith("kernel")]
    if kernel_cols:
        # Create a single 'Kernel' column, taking first non null / non empty value across duplicates
        df["Kernel"] = df[kernel_cols].bfill(axis=1).iloc[:, 0]
        # Drop all original kernel columns except the unified one
        for c in kernel_cols:
            if c != "Kernel":
                df.drop(columns=c, inplace=True, errors="ignore")

    # Merge multiple Tuning columns into one
    tuning_cols = [c for c in df.columns if c.lower().startswith("tuning")]
    if tuning_cols:
        df["Tuning"] = df[tuning_cols].bfill(axis=1).iloc[:, 0]
        for c in tuning_cols:
            if c != "Tuning":
                df.drop(columns=c, inplace=True, errors="ignore")

    return df


def collect_kernel_run_data(root_dir, output_csv=None, verbose=True):
    """
    Walk root_dir recursively, find *kernel-run-data.csv files, read and
    aggregate them into one DataFrame with:
      - first line of each file skipped
      - duplicate Kernel/Tuning columns merged into single 'Kernel' and 'Tuning'
      - an additional BuildFolder column.

    Parameters
    ----------
    root_dir : str or Path
        Directory to search recursively.
    output_csv : str or Path or None, optional
        If provided, the combined DataFrame is written to this CSV path.
        If None, no file is written.
    verbose : bool, optional
        If True, prints progress information.

    Returns
    -------
    pandas.DataFrame
        Combined DataFrame of all kernel-run-data.csv files found.
    """
    root_dir = Path(root_dir).resolve()
    if output_csv is not None:
        output_csv = Path(output_csv).resolve()

    all_rows = []
    pattern = re.compile(r"kernel-run-data\.csv$")

    if verbose:
        print(f"Scanning under: {root_dir}")

    for dirpath, dirnames, filenames in os.walk(root_dir):
        for filename in filenames:
            if pattern.search(filename):
                file_path = Path(dirpath) / filename
                build_folder = find_build_folder(file_path)

                if verbose:
                    print(f"Found: {file_path}")
                    if not build_folder:
                        print("  Warning: no build_ folder found in path, leaving BuildFolder empty")

                try:
                    # Skip the first line, then parse CSV header and data
                    df = pd.read_csv(
                        file_path,
                        skiprows=1,          # skip first non CSV line
                        skipinitialspace=True
                    )
                except Exception as e:
                    if verbose:
                        print(f"  Error reading {file_path}: {e}")
                    continue

                # Normalize and merge duplicate Kernel/Tuning columns
                df = _normalize_columns(df)

                # Add build folder column
                df["BuildFolder"] = build_folder
                all_rows.append(df)

    if not all_rows:
        if verbose:
            print("No kernel-run-data.csv files found. Returning empty DataFrame.")
        return pd.DataFrame()

    combined = pd.concat(all_rows, ignore_index=True)

    if output_csv is not None:
        combined.to_csv(output_csv, index=False)
        if verbose:
            print(f"Written combined CSV to: {output_csv}")

    return combined

# Search a specific directory and also save to a merged CSV
df = collect_kernel_run_data(
    ".",
    output_csv="kernel-run-data-merged.csv",
    verbose=True,
)


import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from pathlib import Path

metric_col = "Mean time per rep (sec.)"

# Check required columns exist
required_cols = ["Kernel", "Variant", "Tuning", "BuildFolder", metric_col]
missing = [c for c in required_cols if c not in df.columns]
if missing:
    raise ValueError(f"Missing required columns in df: {missing}")

df_plot = df.copy()

# 1) Create a merged Variant + Tuning label
df_plot["VariantTuning"] = (
    df_plot["Variant"].astype(str).str.strip()
    + " | "
    + df_plot["Tuning"].astype(str).str.strip()
)

# 2) Aggregate metric by Kernel, VariantTuning, BuildFolder
grouped = (
    df_plot.groupby(["Kernel", "VariantTuning", "BuildFolder"], dropna=False)[metric_col]
           .mean()
           .reset_index()
)

kernels = sorted(grouped["Kernel"].dropna().unique())
build_folders = sorted(grouped["BuildFolder"].dropna().unique())

output_dir = Path("kernel_plots_for_ppt_vertical")
output_dir.mkdir(exist_ok=True)

plt.rcParams.update({
    "figure.figsize": (14, 7),
    "axes.titlesize": 18,
    "axes.labelsize": 14,
    "xtick.labelsize": 11,
    "ytick.labelsize": 12,
    "legend.fontsize": 12,
})

for kernel in kernels:
    kernel_df = grouped[grouped["Kernel"] == kernel].copy()

    variant_tunings = sorted(kernel_df["VariantTuning"].dropna().unique())
    n_vt = len(variant_tunings)
    if n_vt == 0:
        continue

    x_idx = np.arange(n_vt)
    n_folders = len(build_folders)
    if n_folders == 0:
        continue

    total_width = 0.8
    bar_width = total_width / n_folders

    fig, ax = plt.subplots()

    for i, folder in enumerate(build_folders):
        sub = kernel_df[kernel_df["BuildFolder"] == folder]

        y_vals = []
        for vt in variant_tunings:
            row = sub[sub["VariantTuning"] == vt]
            if not row.empty:
                y_vals.append(row[metric_col].iloc[0])
            else:
                y_vals.append(np.nan)

        offset = (i - n_folders / 2) * bar_width + bar_width / 2
        ax.bar(
            x_idx + offset,
            y_vals,
            width=bar_width,
            label=folder,
        )

    ax.set_xticks(x_idx)
    ax.set_xticklabels(variant_tunings, rotation=45, ha="right")
    ax.set_ylabel(metric_col)
    ax.set_xlabel("Variant | Tuning")

    # Add some extra padding under the title
    ax.set_title(
        f"Kernel: {kernel}  -  {metric_col}",
        pad=20,  # increase this if you want more space
    )

    ax.grid(axis="y", linestyle="--", alpha=0.5)
    ax.set_axisbelow(True)

    ax.legend(title="BuildFolder", bbox_to_anchor=(1.02, 1), loc="upper left", borderaxespad=0.)

    # Increase the top margin a bit so bars and limits are not clipped by the title
    fig.tight_layout()
    plt.subplots_adjust(top=0.88)  # smaller than 1.0; reduce this number for more space

    safe_kernel_name = str(kernel)
    out_path = output_dir / f"kernel_{safe_kernel_name}.png"
    fig.savefig(out_path, dpi=200)

    plt.show()

print(f"Vertical bar plots saved in: {output_dir.resolve()}")
