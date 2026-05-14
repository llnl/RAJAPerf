#!/usr/bin/env python3
"""Merge RAJAPerf kernel-run-data CSVs and generate compiler matrix plots.

This script is meant for compiler sweep outputs produced by
`scripts/lc-builds/run_compiler_matrix.sh`, but it also works on any directory
tree that contains RAJAPerf `*-kernel-run-data.csv` files.

The main flow is:
1. Discover matching CSV files from one or more glob patterns.
2. Find the real CSV header row even when RAJAPerf prepends text metadata.
3. Normalize column names across slightly different CSV schemas.
4. Derive compiler/build metadata from the source file path.
5. Merge everything into one dataframe and emit summary tables and plots.
"""

import argparse
import os
import re
import sys
from pathlib import Path
from typing import Dict, Iterable, List, Optional

# Matplotlib needs a writable config/cache directory in this environment.
# Using the current working directory keeps the behavior local to the run
# rather than depending on a user-specific home directory path.
mpl_config_dir = Path.cwd() / ".matplotlib"
mpl_config_dir.mkdir(parents=True, exist_ok=True)
os.environ.setdefault("MPLCONFIGDIR", str(mpl_config_dir))

import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


TIME_COL = "Mean time per rep (sec.)"
BANDWIDTH_COL = "Mean Bandwidth (GiB per sec.)"
FLOPS_COL = "Mean flops (gigaFLOP per sec.)"
PROBLEM_SIZE_COL = "Problem size"
COMPILER_HINT_TOKENS = (
    "amdclang",
    "hipcc",
    "gcc",
    "clang",
    "nvcc",
    "icpx",
    "icc",
    "cce",
    "cray",
    "oneapi",
    "dpcpp",
    "rocm",
)

METRIC_ALIASES = {
    TIME_COL: [
        TIME_COL,
        "Mean time per rep",
        "Time",
    ],
    BANDWIDTH_COL: [
        BANDWIDTH_COL,
        "Bandwidth (GiB per sec.)",
        "Bandwidth (GiB/s)",
        "Mean bandwidth (GiB/s)",
        "Mean Bandwidth (GiB/s)",
    ],
    FLOPS_COL: [
        FLOPS_COL,
        "Mean flops (GFlop/s)",
        "Mean GFLOP/s",
        "GFLOP/s",
        "GFlop/s",
    ],
}


def find_build_folder(path: Path) -> str:
    """Infer the build/compiler identifier from a CSV path.

    Preferred matches are actual `build_*` directories. The fallback cases are
    there for sweep outputs that were copied or reorganized but still keep a
    compiler-like directory name in the path.
    """
    candidates = [path.parent, *path.parents]

    for parent in candidates:
        if parent.name.startswith("build_"):
            return parent.name

    for parent in candidates:
        if parent.name.startswith("install_"):
            return parent.name

    for parent in candidates:
        lower_name = parent.name.lower()
        if lower_name.startswith("lc_") or any(token in lower_name for token in COMPILER_HINT_TOKENS):
            return parent.name

    return ""


def compiler_label(build_folder: str) -> str:
    """Turn a build folder name into a shorter plot-friendly compiler label."""
    label = build_folder
    if label.startswith("build_"):
        label = label[len("build_") :]
    if label.startswith("install_"):
        label = label[len("install_") :]
    if label.startswith("lc_toss4-"):
        label = label[len("lc_toss4-") :]
    elif label.startswith("lc_"):
        label = label[len("lc_") :]
    return label or "unknown-build"


def sanitize_filename(text: object) -> str:
    """Make plot/table filenames stable and shell-friendly."""
    sanitized = "".join(c if c.isalnum() or c in "-_." else "_" for c in str(text))
    sanitized = re.sub(r"_+", "_", sanitized)
    return sanitized.strip("_.") or "output"


def parse_factor(path: Path) -> float:
    """Extract the numeric throughput factor from a standard sweep filename.

    Expected examples:
      DIFFUSION3DPA_factor_1024-kernel-run-data.csv -> 1024.0
      VOL3D_factor_64-kernel-run-data.csv           -> 64.0

    Returns NaN when the filename is not from a factor sweep.
    """
    match = re.search(r"(?:^|_)factor_([0-9]+(?:\.[0-9]+)?)", path.name)
    if not match:
        return np.nan
    return float(match.group(1))


def parse_run_name(path: Path) -> str:
    """Drop the standard CSV suffix to keep the run/file stem for reporting."""
    name = path.name
    if name.endswith("-kernel-run-data.csv"):
        name = name[: -len("-kernel-run-data.csv")]
    return name


def find_csv_files(root_dir: Path, patterns: Iterable[str]) -> List[Path]:
    """Resolve one or more glob patterns into a unique sorted file list.

    Patterns are evaluated relative to root_dir so callers can merge multiple
    build trees at once, for example:
      build_*/compiler_sweep_runs/tier1_base_raja_hip/*kernel-run-data.csv
    """
    files: List[Path] = []
    for pattern in patterns:
        files.extend(root_dir.glob(pattern))
    return sorted({p.resolve() for p in files if p.is_file()})


def header_score(line: str) -> int:
    """Score a line by how much it looks like a kernel-run-data CSV header."""
    tokens = ["Kernel", "Variant", "Tuning", "Problem size", "Mean time", "Mean flops", "Bandwidth"]
    return sum(1 for token in tokens if token in line)


def read_kernel_run_csv(path: Path) -> Optional[pd.DataFrame]:
    """Read one RAJAPerf CSV, allowing for preamble text before the header row."""
    try:
        lines = path.read_text(encoding="utf-8").splitlines()
    except UnicodeDecodeError:
        lines = path.read_text(encoding="latin-1").splitlines()
    except OSError as exc:
        print(f"[SKIP] Could not read {path}: {exc}")
        return None

    header_idx = None
    best_score = -1
    for idx, line in enumerate(lines[:50]):
        score = header_score(line)
        if score > best_score:
            best_score = score
            header_idx = idx
        if score >= 4:
            break

    if header_idx is None or best_score <= 0:
        print(f"[SKIP] Could not identify CSV header in {path}")
        return None

    try:
        # RAJAPerf often writes one or more descriptive lines before the actual
        # CSV header, so the parser starts at the detected header row.
        df = pd.read_csv(path, header=header_idx, skipinitialspace=True)
    except Exception as exc:
        print(f"[SKIP] Could not parse {path}: {exc}")
        return None

    return normalize_columns(df)


def coalesce_prefixed_columns(df: pd.DataFrame, output_name: str, prefix: str) -> pd.DataFrame:
    """Collapse duplicate/prefixed columns that can appear after CSV parsing."""
    columns = [c for c in df.columns if c == output_name or c.lower().startswith(prefix.lower())]
    if not columns:
        return df
    df[output_name] = df[columns].replace("", np.nan).bfill(axis=1).iloc[:, 0]
    for column in columns:
        if column != output_name:
            df = df.drop(columns=column)
    return df


def normalize_columns(df: pd.DataFrame) -> pd.DataFrame:
    """Normalize known schema variations into one canonical column set."""
    df = df.copy()
    df.columns = [str(column).strip() for column in df.columns]
    df = coalesce_prefixed_columns(df, "Kernel", "Kernel")
    df = coalesce_prefixed_columns(df, "Tuning", "Tuning")

    rename_map: Dict[str, str] = {}
    for canonical, aliases in METRIC_ALIASES.items():
        for column in df.columns:
            if column.strip() in aliases:
                rename_map[column] = canonical
                break

    for column in df.columns:
        if column.strip() in {"Problem Size", "ProblemSize", "Size"}:
            rename_map[column] = PROBLEM_SIZE_COL

    return df.rename(columns=rename_map)


def collect_kernel_run_data(
    root_dir: Path,
    patterns: List[str],
    kernel_filters: Optional[List[str]] = None,
    verbose: bool = False,
) -> pd.DataFrame:
    """Load, normalize, annotate, and merge all matching kernel-run-data CSVs.

    The returned dataframe is the central analysis table used by both the CLI
    plots and the notebook. Each input file contributes its original benchmark
    rows plus derived metadata columns describing where that row came from.
    """
    files = find_csv_files(root_dir, patterns)
    if verbose:
        print(f"Found {len(files)} CSV files under {root_dir}")

    frames: List[pd.DataFrame] = []
    for path in files:
        df = read_kernel_run_csv(path)
        if df is None:
            continue

        required = {"Kernel", "Variant", "Tuning", PROBLEM_SIZE_COL}
        missing = sorted(required - set(df.columns))
        if missing:
            print(f"[SKIP] {path} missing columns: {missing}")
            continue

        # These derived columns are what let downstream plots compare compilers
        # without requiring the original directory structure at plotting time.
        build_folder = find_build_folder(path)
        df["BuildFolder"] = build_folder
        df["Compiler"] = compiler_label(build_folder)
        df["SourceFile"] = str(path)
        df["RunName"] = parse_run_name(path)
        df["Factor"] = parse_factor(path)
        frames.append(df)

    if not frames:
        return pd.DataFrame()

    combined = pd.concat(frames, ignore_index=True)
    for column in ["Kernel", "Variant", "Tuning", "BuildFolder", "Compiler"]:
        combined[column] = combined[column].astype(str).str.strip()

    for column in [PROBLEM_SIZE_COL, TIME_COL, BANDWIDTH_COL, FLOPS_COL, "Factor"]:
        if column in combined.columns:
            combined[column] = pd.to_numeric(combined[column], errors="coerce")

    if kernel_filters:
        lowered = [item.lower() for item in kernel_filters]
        kernel_text = combined["Kernel"].fillna("").astype(str).str.lower()
        combined = combined[kernel_text.apply(lambda value: any(item in value for item in lowered))]

    combined["VariantTuning"] = (
        combined["Variant"].astype(str).str.strip() + " | " + combined["Tuning"].astype(str).str.strip()
    )
    combined["CompilerVariantTuning"] = (
        combined["Compiler"].astype(str).str.strip() + " | " + combined["VariantTuning"]
    )
    return combined


def choose_available_metrics(df: pd.DataFrame, requested: Optional[List[str]]) -> List[str]:
    """Keep only metrics that are present and have at least one real value."""
    metrics = requested or [TIME_COL, FLOPS_COL, BANDWIDTH_COL]
    available = [metric for metric in metrics if metric in df.columns and df[metric].notna().any()]
    missing = [metric for metric in metrics if metric not in available]
    for metric in missing:
        print(f"[INFO] Metric unavailable or empty, skipping: {metric}")
    return available


def plot_fixed_size_bars(df: pd.DataFrame, output_dir: Path, metrics: List[str]) -> None:
    """Plot per-kernel bars comparing compilers at the merged problem sizes present.

    This view is most useful when different compilers were run on the same
    problem sizes and you want a compact side-by-side comparison for each
    variant/tuning combination.
    """
    output_dir.mkdir(parents=True, exist_ok=True)
    for metric in metrics:
        metric_df = df.dropna(subset=[metric]).copy()
        if metric_df.empty:
            continue

        grouped = (
            metric_df.groupby(["Kernel", "VariantTuning", "Compiler"], dropna=False)[metric]
            .mean()
            .reset_index()
        )

        for kernel, kernel_df in grouped.groupby("Kernel", sort=True):
            variant_tunings = sorted(kernel_df["VariantTuning"].dropna().unique())
            compilers = sorted(kernel_df["Compiler"].dropna().unique())
            if not variant_tunings or not compilers:
                continue

            x_idx = np.arange(len(variant_tunings))
            width = 0.82 / max(len(compilers), 1)
            fig, ax = plt.subplots(figsize=(14, 7))

            for idx, compiler in enumerate(compilers):
                sub = kernel_df[kernel_df["Compiler"] == compiler]
                values = []
                for vt in variant_tunings:
                    row = sub[sub["VariantTuning"] == vt]
                    values.append(row[metric].iloc[0] if not row.empty else np.nan)
                offset = (idx - len(compilers) / 2) * width + width / 2
                ax.bar(x_idx + offset, values, width=width, label=compiler)

            ax.set_title(f"{kernel} - {metric}", fontsize=15)
            ax.set_ylabel(metric)
            ax.set_xlabel("Variant | Tuning")
            ax.set_xticks(x_idx)
            ax.set_xticklabels(variant_tunings, rotation=35, ha="right")
            ax.grid(axis="y", linestyle="--", alpha=0.35)
            ax.set_axisbelow(True)
            ax.legend(title="Compiler", bbox_to_anchor=(1.02, 1), loc="upper left")
            fig.tight_layout()
            out_path = output_dir / f"{sanitize_filename(kernel)}_{sanitize_filename(metric)}_bars.png"
            fig.savefig(out_path, dpi=200)
            plt.close(fig)


def _plot_throughput_curves_pdf(sweep_df: pd.DataFrame, output_dir: Path, metrics: List[str]) -> None:
    """Matplotlib line plots (vector PDF) for metric vs problem size."""
    for metric in metrics:
        metric_df = sweep_df.dropna(subset=[PROBLEM_SIZE_COL, metric]).copy()
        if metric_df.empty:
            continue

        for kernel, kernel_df in metric_df.groupby("Kernel", sort=True):
            fig, ax = plt.subplots(figsize=(15, 7))
            plotted = False

            for label, line_df in kernel_df.groupby("CompilerVariantTuning", sort=True):
                line_df = line_df.sort_values([PROBLEM_SIZE_COL, "Factor"])
                if line_df.empty:
                    continue
                ax.plot(
                    line_df[PROBLEM_SIZE_COL],
                    line_df[metric],
                    marker="o",
                    linewidth=2,
                    markersize=5,
                    label=label,
                )
                plotted = True

            if not plotted:
                plt.close(fig)
                continue

            ax.set_title(f"{kernel} throughput - {metric}", fontsize=15)
            ax.set_xlabel("Problem size")
            ax.set_ylabel(metric)
            ax.grid(True, linestyle="--", alpha=0.35)
            ax.set_axisbelow(True)
            ax.legend(title="Compiler | Variant | Tuning", bbox_to_anchor=(1.02, 1), loc="upper left", fontsize=8)
            fig.tight_layout()
            out_path = output_dir / f"{sanitize_filename(kernel)}_{sanitize_filename(metric)}_throughput.pdf"
            fig.savefig(out_path, format="pdf", bbox_inches="tight")
            plt.close(fig)


def _plot_throughput_curves_plotly(
    sweep_df: pd.DataFrame, output_dir: Path, metrics: List[str], *, write_png: bool
) -> None:
    """Plotly throughput: standalone HTML, or PNG via ``write_image`` (same basename as PDF/HTML)."""
    try:
        import plotly.graph_objects as go
    except ImportError as exc:
        raise ImportError(
            "Throughput Plotly output requires the 'plotly' package (e.g. pip install plotly)."
        ) from exc

    if write_png:
        try:
            import kaleido  # noqa: F401
        except ImportError as exc:
            raise ImportError(
                "Throughput PNG (--confluence) requires the 'kaleido' package (e.g. pip install kaleido)."
            ) from exc

    grid_style = dict(
        showgrid=True,
        gridcolor="rgba(220, 220, 240, 0.22)",
        gridwidth=1,
        griddash="dash",
        tickfont=dict(size=15),
        title_font=dict(size=17),
    )

    for metric in metrics:
        metric_df = sweep_df.dropna(subset=[PROBLEM_SIZE_COL, metric]).copy()
        if metric_df.empty:
            continue

        for kernel, kernel_df in metric_df.groupby("Kernel", sort=True):
            traces = []

            for label, line_df in kernel_df.groupby("CompilerVariantTuning", sort=True):
                line_df = line_df.sort_values([PROBLEM_SIZE_COL, "Factor"])
                if line_df.empty:
                    continue
                traces.append(
                    go.Scatter(
                        x=line_df[PROBLEM_SIZE_COL],
                        y=line_df[metric],
                        mode="lines+markers",
                        name=str(label),
                        line=dict(width=2),
                        marker=dict(size=10),
                    )
                )

            if not traces:
                continue

            fig = go.Figure(data=traces)
            fig.update_layout(
                title=dict(text=f"{kernel} throughput - {metric}", font=dict(size=24)),
                xaxis_title="Problem size",
                yaxis_title=metric,
                legend_title_text="Compiler | Variant | Tuning",
                legend=dict(
                    font=dict(size=14),
                    title_font=dict(size=15),
                    yanchor="top",
                    y=1,
                    xanchor="left",
                    x=1.02,
                ),
                hovermode="closest",
                hoverlabel=dict(namelength=-1, font=dict(size=15)),
                template="plotly_dark",
                font=dict(size=16),
                width=1100,
                height=560,
                margin=dict(l=76, r=220, t=76, b=58),
            )
            fig.update_xaxes(**grid_style)
            fig.update_yaxes(**grid_style)

            stem = f"{sanitize_filename(kernel)}_{sanitize_filename(metric)}_throughput"
            if write_png:
                out_path = output_dir / f"{stem}.png"
                fig.write_image(str(out_path))
            else:
                out_path = output_dir / f"{stem}.html"
                fig.write_html(out_path, include_plotlyjs="cdn", full_html=True)


def plot_throughput_curves(
    df: pd.DataFrame,
    output_dir: Path,
    metrics: List[str],
    html: bool = False,
    *,
    confluence: bool = False,
) -> None:
    """Plot metric vs problem size for factor sweeps or multi-size runs.

    A row is considered sweep-like if either:
    - a numeric factor was parsed from the filename, or
    - the same kernel/compiler/variant/tuning appears at multiple sizes

    By default writes one vector PDF per kernel/metric. With ``html=True`` and
    ``confluence=False``, writes standalone Plotly HTML. With ``confluence=True``,
    writes the same Plotly figure as PNG (``*_throughput.png``, same stem as PDF/HTML)
    via ``fig.write_image``. If both ``html`` and ``confluence`` are true, PNG is used.
    """
    output_dir.mkdir(parents=True, exist_ok=True)
    sweep_df = df.copy()
    sweep_df = sweep_df[sweep_df["Factor"].notna() | sweep_df.duplicated(["Kernel", "CompilerVariantTuning"], keep=False)]
    if sweep_df.empty:
        print("[INFO] No factor-sweep or multi-size data found for throughput plots.")
        return

    if html or confluence:
        _plot_throughput_curves_plotly(sweep_df, output_dir, metrics, write_png=confluence)
    else:
        _plot_throughput_curves_pdf(sweep_df, output_dir, metrics)


def write_summary_tables(df: pd.DataFrame, output_dir: Path, metrics: List[str]) -> None:
    """Write merged and peak-performance summary tables alongside the plots.

    The peak tables keep the single best row per kernel and
    compiler/variant/tuning label for the requested metric.
    """
    output_dir.mkdir(parents=True, exist_ok=True)
    summary_columns = ["Kernel", "Compiler", "Variant", "Tuning", PROBLEM_SIZE_COL, "Factor", *metrics]
    present_columns = [column for column in summary_columns if column in df.columns]
    df[present_columns].sort_values(["Kernel", "Compiler", "Variant", "Tuning", PROBLEM_SIZE_COL]).to_csv(
        output_dir / "kernel_run_data_summary.csv",
        index=False,
    )

    if FLOPS_COL in metrics and FLOPS_COL in df.columns:
        idx = df.groupby(["Kernel", "CompilerVariantTuning"])[FLOPS_COL].idxmax()
        peak = df.loc[idx.dropna().astype(int), present_columns].sort_values(["Kernel", "Compiler", "Variant", "Tuning"])
        peak.to_csv(output_dir / "peak_flops_by_kernel.csv", index=False)

    if BANDWIDTH_COL in metrics and BANDWIDTH_COL in df.columns:
        idx = df.groupby(["Kernel", "CompilerVariantTuning"])[BANDWIDTH_COL].idxmax()
        peak = df.loc[idx.dropna().astype(int), present_columns].sort_values(["Kernel", "Compiler", "Variant", "Tuning"])
        peak.to_csv(output_dir / "peak_bandwidth_by_kernel.csv", index=False)


def parse_args() -> argparse.Namespace:
    """Define the small CLI used by ad hoc plotting and notebook preparation."""
    parser = argparse.ArgumentParser(
        description="Read RAJAPerf kernel-run-data CSVs and plot compiler comparisons."
    )
    parser.add_argument("--root-dir", default=".", help="Directory to search recursively (default: current directory)")
    parser.add_argument("--output-dir", default="compiler-comparison-output", help="Output directory")
    parser.add_argument(
        "--glob-pattern",
        action="append",
        default=None,
        help="Glob for input CSVs, repeatable (default: **/*kernel-run-data.csv)",
    )
    parser.add_argument(
        "--kernel",
        action="append",
        default=None,
        help="Kernel-name substring to include, repeatable",
    )
    parser.add_argument(
        "--metric",
        action="append",
        choices=[TIME_COL, FLOPS_COL, BANDWIDTH_COL],
        default=None,
        help="Metric to plot, repeatable (default: time, flops, bandwidth when present)",
    )
    parser.add_argument("--no-fixed-plots", action="store_true", help="Skip fixed-size bar plots")
    parser.add_argument(
        "--html",
        action="store_true",
        help="Write throughput plots as standalone Plotly HTML pages (default: vector PDF via matplotlib)",
    )
    parser.add_argument(
        "--confluence",
        action="store_true",
        help="Write throughput as Plotly PNG (fig.write_image) for Confluence; same basename as PDF/HTML; requires kaleido",
    )
    parser.add_argument(
        "--no-throughput-plots",
        action="store_true",
        help="Skip throughput plots (PDF by default; use --html or --confluence for Plotly)",
    )
    parser.add_argument("--verbose", action="store_true", help="Print input discovery details")
    return parser.parse_args()


def main() -> int:
    """CLI entrypoint: load data, write merged tables, and render requested plots."""
    args = parse_args()
    root_dir = Path(args.root_dir).resolve()
    output_dir = Path(args.output_dir).resolve()
    patterns = args.glob_pattern or ["**/*kernel-run-data.csv"]

    # Input discovery is intentionally flexible: callers can point at a single
    # build tree, the repository root, or a multi-build glob under the repo.
    df = collect_kernel_run_data(
        root_dir=root_dir,
        patterns=patterns,
        kernel_filters=args.kernel,
        verbose=args.verbose,
    )
    if df.empty:
        print("No kernel-run-data CSV rows found.")
        return 1

    output_dir.mkdir(parents=True, exist_ok=True)
    combined_path = output_dir / "kernel-run-data-merged.csv"
    df.to_csv(combined_path, index=False)
    print(f"Wrote merged data: {combined_path}")

    metrics = choose_available_metrics(df, args.metric)
    if not metrics:
        print("No requested metrics are available to plot.")
        return 1

    write_summary_tables(df, output_dir, metrics)
    if not args.no_fixed_plots:
        plot_fixed_size_bars(df, output_dir / "fixed-size-plots", metrics)
    if not args.no_throughput_plots:
        try:
            plot_throughput_curves(
                df,
                output_dir / "throughput-plots",
                metrics,
                html=args.html,
                confluence=args.confluence,
            )
        except ImportError as exc:
            print(str(exc), file=sys.stderr)
            return 1

    print(f"Wrote compiler comparison outputs under: {output_dir}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
