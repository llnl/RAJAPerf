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

%matplotlib inline

from pathlib import Path
from typing import Optional, Union

import pandas as pd
import matplotlib.pyplot as plt


# ====================== STYLE FOR POWERPOINT ======================

def set_ppt_style():
    """
    Configure matplotlib defaults for PowerPoint ready figures.
    """
    plt.style.use("default")  # clean base

    plt.rcParams.update({
        "figure.figsize": (8, 5),        # works well on slides
        "figure.dpi": 150,               # higher DPI for clarity
        "savefig.dpi": 300,              # high quality export
        "axes.titlesize": 16,
        "axes.labelsize": 14,
        "xtick.labelsize": 12,
        "ytick.labelsize": 12,
        "legend.fontsize": 11,
        "lines.linewidth": 2.0,
        "lines.markersize": 6,
        "axes.grid": True,
        "grid.linestyle": "--",
        "grid.alpha": 0.3,
        "figure.facecolor": "white",
        "axes.facecolor": "white",
        "font.family": "sans-serif",
        "font.sans-serif": ["Arial", "DejaVu Sans", "Liberation Sans"],
    })


# ====================== NORMALIZATION HELPERS ======================

def _merge_prefixed_columns(df: pd.DataFrame, prefix: str, unified_name: str) -> None:
    cols = [c for c in df.columns if c.lower().startswith(prefix.lower())]
    if not cols:
        return

    df[cols] = df[cols].replace("", pd.NA)
    df[unified_name] = df[cols].bfill(axis=1).iloc[:, 0]

    for c in cols:
        if c != unified_name:
            df.drop(columns=c, inplace=True, errors="ignore")


def normalize_kernel_variant_tuning(df: pd.DataFrame) -> pd.DataFrame:
    df = df.copy()
    df.columns = [c.strip() for c in df.columns]

    _merge_prefixed_columns(df, "kernel", "Kernel")
    _merge_prefixed_columns(df, "variant", "Variant")
    _merge_prefixed_columns(df, "tuning", "Tuning")

    return df


# ====================== TRANSFORM CSV ======================

def transform_and_save(
    input_csv: Union[str, Path],
    output_csv: Union[str, Path],
    build_folder_col: str = "BuildFolder",
) -> pd.DataFrame:
    df = pd.read_csv(input_csv)
    print("Original columns:", df.columns.tolist())

    df = normalize_kernel_variant_tuning(df)

    # Create merged "Variant+Tuning" column
    df["Variant"] = df.get("Variant", "").fillna("").astype(str).str.strip()
    df["Tuning"]  = df.get("Tuning", "").fillna("").astype(str).str.strip()
    df["Variant+Tuning"] = (df["Variant"] + " - " + df["Tuning"]).str.strip(" -")

    # Keep columns up to and including build_folder_col, plus Variant+Tuning
    if build_folder_col not in df.columns:
        raise KeyError(f"'{build_folder_col}' column not found. Columns are: {df.columns.tolist()}")

    cols = df.columns.tolist()
    build_idx = cols.index(build_folder_col)
    keep_cols = cols[:build_idx + 1] + ["Variant+Tuning"]

    df_result = df[keep_cols].copy()
    print("Transformed columns:", df_result.columns.tolist())

    output_csv = Path(output_csv)
    df_result.to_csv(output_csv, index=False)
    print(f"Saved transformed CSV to: {output_csv.resolve()}")

    return df_result


# ====================== THROUGHPUT ======================

def add_throughput(
    df: pd.DataFrame,
    size_col: str = "Problem size",
    time_col: str = "Mean time per rep (sec.)",
) -> pd.DataFrame:
    df = df.copy()

    if size_col not in df.columns:
        raise KeyError(f"Required column '{size_col}' not found.")
    if time_col not in df.columns:
        raise KeyError(f"Required column '{time_col}' not found.")

    df[size_col] = pd.to_numeric(df[size_col], errors="coerce")
    df[time_col] = pd.to_numeric(df[time_col], errors="coerce")

    df["Throughput"] = df[size_col] / df[time_col]
    return df


# ====================== PLOTTING (POWERPOINT FRIENDLY) ======================

def plot_throughput_by_kernel_variant_tuning_ppt(
    df: pd.DataFrame,
    output_dir: Union[str, Path] = ".",
    base_title: Optional[str] = "Throughput vs Problem size",
    size_col: str = "Problem size",
    time_col: str = "Mean time per rep (sec.)",
    log_x: bool = False,
    log_y: bool = False,
    aggregate_duplicates: bool = True,
) -> None:
    set_ppt_style()
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    df = add_throughput(df, size_col=size_col, time_col=time_col)

    if "Kernel" not in df.columns:
        raise KeyError("'Kernel' column not found.")
    if "Variant+Tuning" not in df.columns:
        raise KeyError("'Variant+Tuning' column not found.")

    if aggregate_duplicates:
        group_cols = ["Kernel", "Variant+Tuning", size_col]
        agg_dict = {"Throughput": "mean"}
        if time_col in df.columns:
            agg_dict[time_col] = "mean"

        df_plot = df.groupby(group_cols, as_index=False).agg(agg_dict)
    else:
        df_plot = df.copy()

    df_plot = df_plot.dropna(subset=["Kernel", size_col, "Throughput"])
    df_plot = df_plot.sort_values(by=["Kernel", "Variant+Tuning", size_col])

    if df_plot.empty:
        print("No data to plot after filtering.")
        return

    kernels = df_plot["Kernel"].unique()
    print("Kernels found:", kernels)

    # A color palette that works well on slides
    color_cycle = plt.cm.tab10.colors

    for k_idx, kernel in enumerate(kernels):
        df_kernel = df_plot[df_plot["Kernel"] == kernel]
        if df_kernel.empty:
            continue

        fig, ax = plt.subplots()

        for s_idx, (vt_label, sub) in enumerate(df_kernel.groupby("Variant+Tuning")):
            if sub.empty:
                continue
            color = color_cycle[s_idx % len(color_cycle)]
            ax.plot(
                sub[size_col],
                sub["Throughput"],
                marker="o",
                linestyle="-",
                label=str(vt_label),
                color=color,
            )

        ax.set_xlabel(size_col)
        ax.set_ylabel("Throughput (DoFs per second)")

        # Use a figure level title, positioned higher so there is space
        title = base_title or "Throughput vs Problem size"
        fig.suptitle(
            f"{title} - Kernel: {kernel}",
            y=0.98,          # push the main title up (0.98 is near the top)
            fontsize=16,
        )

        if log_x:
            ax.set_xscale("log")
        if log_y:
            ax.set_yscale("log")

        # Legend outside plot to avoid clutter on slides
        ax.legend(
            title="Variant+Tuning",
            loc="center left",
            bbox_to_anchor=(1.02, 0.5),
            borderaxespad=0.0,
        )

        # Let tight_layout pack the axes, then keep some room for suptitle
        fig.tight_layout()
        # Reserve a bit of space at the top so suptitle is not clipped
        fig.subplots_adjust(top=0.90)

        # Save high resolution PNG per kernel
        safe_kernel = str(kernel).replace(" ", "")
        out_path = output_dir / f"ppt_plot_{safe_kernel}.png"
        fig.savefig(out_path, bbox_inches="tight")
        print(f"Saved PowerPoint ready plot: {out_path}")

        plt.show()


# ====================== RUN THE WORKFLOW ======================

INPUT_CSV  = "kernel-run-data-merged.csv"
OUTPUT_CSV = "kernel-run-data-merged.csv"   # overwrite; change if you want a separate file

try:
    # 1) Transform CSV (keep up to BuildFolder + Variant+Tuning)
    df_transformed = transform_and_save(INPUT_CSV, OUTPUT_CSV, build_folder_col="BuildFolder")

    # 2) Plot with PPT ready styling
    plot_throughput_by_kernel_variant_tuning_ppt(
        df_transformed,
        output_dir="ppt_plots",  # folder where PNGs are stored
        base_title="Kernel throughput comparison",
        size_col="Problem size",
        time_col="Mean time per rep (sec.)",
        log_x=False,
        log_y=False,
        aggregate_duplicates=True,
    )

except Exception as e:
    print("Error:", e)


#subfigures
%matplotlib inline

from pathlib import Path
from typing import Optional, Union

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


# ====================== STYLE FOR POWERPOINT ======================

def set_ppt_style():
    """
    Configure matplotlib defaults for PowerPoint ready figures.
    """
    plt.style.use("default")

    plt.rcParams.update({
        "figure.figsize": (10, 4.5),
        "figure.dpi": 150,
        "savefig.dpi": 300,
        "axes.titlesize": 16,
        "axes.labelsize": 14,
        "xtick.labelsize": 12,
        "ytick.labelsize": 12,
        "legend.fontsize": 11,
        "lines.linewidth": 2.0,
        "lines.markersize": 6,
        "axes.grid": True,
        "grid.linestyle": "--",
        "grid.alpha": 0.3,
        "figure.facecolor": "white",
        "axes.facecolor": "white",
        "font.family": "sans-serif",
        "font.sans-serif": ["Arial", "DejaVu Sans", "Liberation Sans"],
    })


# ====================== NORMALIZATION HELPERS ======================

def _merge_prefixed_columns(df: pd.DataFrame, prefix: str, unified_name: str) -> None:
    cols = [c for c in df.columns if c.lower().startswith(prefix.lower())]
    if not cols:
        return

    df[cols] = df[cols].replace("", pd.NA)
    df[unified_name] = df[cols].bfill(axis=1).iloc[:, 0]

    for c in cols:
        if c != unified_name:
            df.drop(columns=c, inplace=True, errors="ignore")


def normalize_kernel_variant_tuning(df: pd.DataFrame) -> pd.DataFrame:
    df = df.copy()
    df.columns = [c.strip() for c in df.columns]

    _merge_prefixed_columns(df, "kernel", "Kernel")
    _merge_prefixed_columns(df, "variant", "Variant")
    _merge_prefixed_columns(df, "tuning", "Tuning")

    return df


# ====================== TRANSFORM CSV ======================

def transform_and_save(
    input_csv: Union[str, Path],
    output_csv: Union[str, Path],
    build_folder_col: str = "BuildFolder",
) -> pd.DataFrame:
    df = pd.read_csv(input_csv)
    print("Original columns:", df.columns.tolist())

    df = normalize_kernel_variant_tuning(df)

    df["Variant"] = df.get("Variant", "").fillna("").astype(str).str.strip()
    df["Tuning"]  = df.get("Tuning", "").fillna("").astype(str).str.strip()
    df["Variant+Tuning"] = (df["Variant"] + " - " + df["Tuning"]).str.strip(" -")

    if build_folder_col not in df.columns:
        raise KeyError(f"'{build_folder_col}' column not found. Columns are: {df.columns.tolist()}")

    cols = df.columns.tolist()
    build_idx = cols.index(build_folder_col)
    keep_cols = cols[:build_idx + 1] + ["Variant+Tuning"]

    df_result = df[keep_cols].copy()
    print("Transformed columns:", df_result.columns.tolist())

    output_csv = Path(output_csv)
    df_result.to_csv(output_csv, index=False)
    print(f"Saved transformed CSV to: {output_csv.resolve()}")

    return df_result


# ====================== THROUGHPUT ======================

def add_throughput(
    df: pd.DataFrame,
    size_col: str = "Problem size",
    time_col: str = "Mean time per rep (sec.)",
) -> pd.DataFrame:
    df = df.copy()

    if size_col not in df.columns:
        raise KeyError(f"Required column '{size_col}' not found.")
    if time_col not in df.columns:
        raise KeyError(f"Required column '{time_col}' not found.")

    df[size_col] = pd.to_numeric(df[size_col], errors="coerce")
    df[time_col] = pd.to_numeric(df[time_col], errors="coerce")

    df["Throughput"] = df[size_col] / df[time_col]
    return df


# ====================== PLOTTING (POWERPOINT FRIENDLY) ======================

def plot_throughput_by_kernel_variant_tuning_ppt(
    df: pd.DataFrame,
    output_dir: Union[str, Path] = ".",
    base_title: Optional[str] = "Throughput vs Problem size",
    size_col: str = "Problem size",
    time_col: str = "Mean time per rep (sec.)",
    log_x: bool = False,
    log_y: bool = False,
    aggregate_duplicates: bool = True,
) -> None:
    """
    Plot throughput vs problem size.

    - First N-2 kernels: 1 row x 2 columns, two kernels per figure.
    - Last two kernels: each gets its own separate figure (1 subplot).
    - Subplots have titles "Kernel: ..."; no figure level titles.
    - Legend is outside the plot, bottom right.
    """
    set_ppt_style()
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    df = add_throughput(df, size_col=size_col, time_col=time_col)

    if "Kernel" not in df.columns:
        raise KeyError("'Kernel' column not found.")
    if "Variant+Tuning" not in df.columns:
        raise KeyError("'Variant+Tuning' column not found.")

    if aggregate_duplicates:
        group_cols = ["Kernel", "Variant+Tuning", size_col]
        agg_dict = {"Throughput": "mean"}
        if time_col in df.columns:
            agg_dict[time_col] = "mean"

        df_plot = df.groupby(group_cols, as_index=False).agg(agg_dict)
    else:
        df_plot = df.copy()

    df_plot = df_plot.dropna(subset=["Kernel", size_col, "Throughput"])
    df_plot = df_plot.sort_values(by=["Kernel", "Variant+Tuning", size_col])

    if df_plot.empty:
        print("No data to plot after filtering.")
        return

    kernels = df_plot["Kernel"].unique().tolist()
    print("Kernels found:", kernels)

    color_cycle = plt.cm.tab10.colors
    n_kernels = len(kernels)

    if n_kernels == 0:
        return

    if n_kernels > 2:
        all_but_last_two = kernels[:-2]
        last_two = kernels[-2:]
    elif n_kernels == 2:
        all_but_last_two = []
        last_two = kernels
    else:
        all_but_last_two = []
        last_two = kernels

    def plot_kernel_list_to_figure(kernel_list, figure_suffix: str):
        """
        Plot the given kernels in a single figure (1 row x up to 2 columns).
        Legend is outside bottom right.
        """
        if not kernel_list:
            return

        n_subplots = len(kernel_list)
        ncols = min(2, n_subplots)
        nrows = int(np.ceil(n_subplots / 2))

        fig, axes = plt.subplots(nrows=nrows, ncols=ncols, sharex=False, sharey=False)

        if isinstance(axes, np.ndarray):
            axes = list(np.ravel(axes))
        else:
            axes = [axes]

        used_axes = 0

        for k_idx, kernel in enumerate(kernel_list):
            ax = axes[k_idx]
            used_axes += 1

            df_kernel = df_plot[df_plot["Kernel"] == kernel]
            if df_kernel.empty:
                continue

            for s_idx, (vt_label, sub) in enumerate(df_kernel.groupby("Variant+Tuning")):
                if sub.empty:
                    continue
                color = color_cycle[s_idx % len(color_cycle)]
                ax.plot(
                    sub[size_col],
                    sub["Throughput"],
                    marker="o",
                    linestyle="-",
                    label=str(vt_label),
                    color=color,
                )

            ax.set_xlabel(size_col)
            ax.set_ylabel("Throughput (DoFs per second)")
            ax.set_title(
                f"Kernel: {kernel}",
                fontsize=14,
                pad=15,
            )

            if log_x:
                ax.set_xscale("log")
            if log_y:
                ax.set_yscale("log")

            ax.grid(True)

        for ax in axes[used_axes:]:
            fig.delaxes(ax)

        # Collect legend entries from all used axes
        all_handles = []
        all_labels = []
        for ax in axes[:used_axes]:
            h, l = ax.get_legend_handles_labels()
            all_handles.extend(h)
            all_labels.extend(l)

        seen = set()
        uniq_handles = []
        uniq_labels = []
        for h, l in zip(all_handles, all_labels):
            if l not in seen:
                seen.add(l)
                uniq_handles.append(h)
                uniq_labels.append(l)

        fig.tight_layout()

        if uniq_handles:
            fig.legend(
                uniq_handles,
                uniq_labels,
                title="Variant+Tuning",
                loc="lower right",
                bbox_to_anchor=(1.0, -0.02),  # outside bottom right
                borderaxespad=0.0,
                ncol=2,                        # adjust columns if you have many entries
            )

        # Give space below for the legend
        fig.subplots_adjust(bottom=0.25)

        safe_kernels = "_".join(str(k).replace(" ", "") for k in kernel_list)
        out_path = output_dir / f"ppt_plot_{figure_suffix}_{safe_kernels}.png"
        fig.savefig(out_path, bbox_inches="tight")
        print(f"Saved PowerPoint ready plot: {out_path}")

        plt.show()

    # First N-2 kernels in 1x2 blocks
    for i in range(0, len(all_but_last_two), 2):
        chunk = all_but_last_two[i:i + 2]
        if chunk:
            plot_kernel_list_to_figure(chunk, figure_suffix="block")

    # Last two kernels, each its own figure
    for kernel in last_two:
        plot_kernel_list_to_figure([kernel], figure_suffix="single")


# ====================== RUN THE WORKFLOW ======================

INPUT_CSV  = "kernel-run-data-merged.csv"
OUTPUT_CSV = "kernel-run-data-merged.csv"

try:
    df_transformed = transform_and_save(INPUT_CSV, OUTPUT_CSV, build_folder_col="BuildFolder")

    plot_throughput_by_kernel_variant_tuning_ppt(
        df_transformed,
        output_dir="ppt_plots",
        base_title="Kernel throughput comparison",
        size_col="Problem size",
        time_col="Mean time per rep (sec.)",
        log_x=False,
        log_y=False,
        aggregate_duplicates=True,
    )

except Exception as e:
    print("Error:", e)
