#!/usr/bin/env python3

import argparse
import os
import re
from pathlib import Path
from typing import Dict, Iterable, List, Optional

os.environ.setdefault("MPLCONFIGDIR", "/var/tmp/vargas45/matplotlib")

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
    sanitized = "".join(c if c.isalnum() or c in "-_." else "_" for c in str(text))
    sanitized = re.sub(r"_+", "_", sanitized)
    return sanitized.strip("_.") or "output"


def parse_factor(path: Path) -> float:
    match = re.search(r"(?:^|_)factor_([0-9]+(?:\.[0-9]+)?)", path.name)
    if not match:
        return np.nan
    return float(match.group(1))


def parse_run_name(path: Path) -> str:
    name = path.name
    if name.endswith("-kernel-run-data.csv"):
        name = name[: -len("-kernel-run-data.csv")]
    return name


def find_csv_files(root_dir: Path, patterns: Iterable[str]) -> List[Path]:
    files: List[Path] = []
    for pattern in patterns:
        files.extend(root_dir.glob(pattern))
    return sorted({p.resolve() for p in files if p.is_file()})


def header_score(line: str) -> int:
    tokens = ["Kernel", "Variant", "Tuning", "Problem size", "Mean time", "Mean flops", "Bandwidth"]
    return sum(1 for token in tokens if token in line)


def read_kernel_run_csv(path: Path) -> Optional[pd.DataFrame]:
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
        df = pd.read_csv(path, header=header_idx, skipinitialspace=True)
    except Exception as exc:
        print(f"[SKIP] Could not parse {path}: {exc}")
        return None

    return normalize_columns(df)


def coalesce_prefixed_columns(df: pd.DataFrame, output_name: str, prefix: str) -> pd.DataFrame:
    columns = [c for c in df.columns if c == output_name or c.lower().startswith(prefix.lower())]
    if not columns:
        return df
    df[output_name] = df[columns].replace("", np.nan).bfill(axis=1).iloc[:, 0]
    for column in columns:
        if column != output_name:
            df = df.drop(columns=column)
    return df


def normalize_columns(df: pd.DataFrame) -> pd.DataFrame:
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
    metrics = requested or [TIME_COL, FLOPS_COL, BANDWIDTH_COL]
    available = [metric for metric in metrics if metric in df.columns and df[metric].notna().any()]
    missing = [metric for metric in metrics if metric not in available]
    for metric in missing:
        print(f"[INFO] Metric unavailable or empty, skipping: {metric}")
    return available


def plot_fixed_size_bars(df: pd.DataFrame, output_dir: Path, metrics: List[str]) -> None:
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


def plot_throughput_curves(df: pd.DataFrame, output_dir: Path, metrics: List[str]) -> None:
    output_dir.mkdir(parents=True, exist_ok=True)
    sweep_df = df.copy()
    sweep_df = sweep_df[sweep_df["Factor"].notna() | sweep_df.duplicated(["Kernel", "CompilerVariantTuning"], keep=False)]
    if sweep_df.empty:
        print("[INFO] No factor-sweep or multi-size data found for throughput plots.")
        return

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
            ax.legend(title="Compiler | Variant | Tuning", bbox_to_anchor=(1.02, 1), loc="upper left", fontsize=8)
            fig.tight_layout()
            out_path = output_dir / f"{sanitize_filename(kernel)}_{sanitize_filename(metric)}_throughput.png"
            fig.savefig(out_path, dpi=200)
            plt.close(fig)


def write_summary_tables(df: pd.DataFrame, output_dir: Path, metrics: List[str]) -> None:
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
    parser.add_argument("--no-throughput-plots", action="store_true", help="Skip throughput line plots")
    parser.add_argument("--verbose", action="store_true", help="Print input discovery details")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    root_dir = Path(args.root_dir).resolve()
    output_dir = Path(args.output_dir).resolve()
    patterns = args.glob_pattern or ["**/*kernel-run-data.csv"]

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
        plot_throughput_curves(df, output_dir / "throughput-plots", metrics)

    print(f"Wrote compiler comparison outputs under: {output_dir}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
