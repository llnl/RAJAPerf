import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import statistics
import os
import argparse


SMOOTH_FLOPS_COL_NAME="Smoothed Mean flops (gigaFLOP per sec.)"
RAW_FLOPS_COL = 'Mean flops (gigaFLOP per sec.)'
SMOOTH_FLOPS_COL = 'Smoothed Mean flops (gigaFLOP per sec.)'
PROBLEM_SIZE_COL = 'Problem size'
VARIANT_TUNING_COL = 'Variant_Tuning'
KERNEL_COL = 'Kernel'

def moving_median_smooth(y, k=5):
    """
    Apply a moving median filter with window size k.
    Returns a list of smoothed values.
    """
    n = len(y)
    m = (k - 1) // 2
    y_smooth = []
    for i in range(n):
        start = max(0, i - m)
        end = min(n - 1, i + m)
        window = y[start:end + 1]
        y_smooth.append(statistics.median(window))
    return y_smooth

def find_saturation_point(x, y_smooth, eps=0.1, w=3):
    """
    Find the smallest x_i where y_smooth stays above (1-eps)*y_max for w consecutive points.
    Returns the corresponding x value or None if not found.
    """
    y_max = max(y_smooth)
    threshold = (1.0 - eps) * y_max
    n = len(y_smooth)
    run_length = 0
    run_start_idx = None
    for i in range(n):
        if y_smooth[i] >= threshold:
            if run_length == 0:
                run_start_idx = i
            run_length += 1
            if run_length >= w:
                return x[run_start_idx]
        else:
            run_length = 0
            run_start_idx = None
    return None

def plot_kernel(df, kernel, k=5, eps=0.1, w=3, save_dir=None):
    """
    Plot throughput curves for each variant of a kernel.
    Raw data: solid dots. Smoothed: dashed lines.
    Wide, PowerPoint-friendly plot.
    """
    plt.figure(figsize=(18, 7))  # Wider plot
    variants = df['Variant_Tuning'].unique()
    report = []
    colors = plt.cm.tab10.colors
    for idx, variant in enumerate(variants):
        subdf = df[df['Variant_Tuning'] == variant].copy()
        subdf = subdf.sort_values('Problem size')
        x = subdf['Problem size'].astype(float).values
        y = subdf['Mean flops (gigaFLOP per sec.)'].astype(float).values
        y_smooth = moving_median_smooth(list(y), k=k)
        for xi, yi in zip(x, y_smooth):
            df.loc[(df['Variant_Tuning'] == variant) & (df['Problem size'] == xi), SMOOTH_FLOPS_COL_NAME] = yi
        # Raw data points: solid dots
        plt.plot(
            x, y, '-o', 
            label=f"{variant} (raw)", 
            markersize=8, 
            color=colors[idx % len(colors)], 
            markerfacecolor=colors[idx % len(colors)], 
            markeredgewidth=0
        )
        # Smoothed curve: dashed line
        plt.plot(
            x, y_smooth, '--', 
            label=f"{variant} (smoothed)", 
            linewidth=3, 
            color=colors[idx % len(colors)]
        )
        y_max = max(y_smooth)
        sat_x = find_saturation_point(x, y_smooth, eps=eps, w=w)
        report.append({
            "Variant_Tuning": variant,
            "y_max": y_max,
            "saturation_x": sat_x
        })

    plt.title(f"Kernel: {kernel}", fontsize=22)
    plt.xlabel("Problem size", fontsize=18)
    plt.ylabel("Mean flops (gigaFLOP per sec.)", fontsize=18)
    plt.grid(True, which='both', linestyle='--', linewidth=1)
    plt.xticks(fontsize=16)
    plt.yticks(fontsize=16)
    plt.legend(loc='upper left', bbox_to_anchor=(1.05, 1), fontsize=16, frameon=False)
    plt.tight_layout(rect=[0, 0, 0.75, 1])

    if save_dir:
        import os
        os.makedirs(save_dir, exist_ok=True)
        fname = f"{save_dir}/{kernel}.png"
        plt.savefig(fname, dpi=200)
        print(f"Saved plot to {fname}")
    plt.show()

    print(f"Kernel: {kernel}")
    print(f"eps: {eps}, w: {w}")
    print("Variant_Tuning | Global max of smoothed curve (y_max) | Saturation point (x)")
    for r in report:
        sat_str = f"{r['saturation_x']:.2f}" if r['saturation_x'] is not None else "None"
        print(f"{r['Variant_Tuning']:16} | {r['y_max']:.6g} | {sat_str}")

def smooth_and_plot_all_kernels(df, k=5, eps=0.1, w=3, save_dir=None):
    """
    Load data and plot all kernels.
    """
    # Drop rows missing key columns
    df = df.dropna(subset=['Kernel', 'Problem size', 'Mean flops (gigaFLOP per sec.)', 'Variant_Tuning'])
    df[SMOOTH_FLOPS_COL_NAME] = np.nan
    kernels = df['Kernel'].unique()
    print(f"Found {len(kernels)} kernels.")
    for kernel in kernels:
        tmp = df[df['Kernel'] == kernel]
        plot_kernel(tmp, kernel, k=k, eps=eps, w=w, save_dir=save_dir)
        df.loc[df['Kernel'] == kernel, SMOOTH_FLOPS_COL_NAME] = tmp[SMOOTH_FLOPS_COL_NAME]
        
    return df
    
def save_kernel_tables(df, outdir='kernel_tables'):
    os.makedirs(outdir, exist_ok=True)
    kernels = df[KERNEL_COL].unique()
    for kernel in kernels:
        df_kernel = df[df[KERNEL_COL] == kernel]
        
        # Pivot for raw FLOP/s
        raw_table = df_kernel.pivot_table(
            index=PROBLEM_SIZE_COL,
            columns=VARIANT_TUNING_COL,
            values=RAW_FLOPS_COL
        )
        raw_table = raw_table.sort_index()
        raw_csv_path = os.path.join(outdir, f"{kernel}_raw.csv")
        raw_table.to_csv(raw_csv_path)
        
        # Pivot for smoothed FLOP/s
        smooth_table = df_kernel.pivot_table(
            index=PROBLEM_SIZE_COL,
            columns=VARIANT_TUNING_COL,
            values=SMOOTH_FLOPS_COL
        )
        smooth_table = smooth_table.sort_index()
        smooth_csv_path = os.path.join(outdir, f"{kernel}_smoothed.csv")
        smooth_table.to_csv(smooth_csv_path)
        
        print(f"Saved: {raw_csv_path}, {smooth_csv_path}")


parser = argparse.ArgumentParser()
parser.add_argument("--output-dir", required=True, help="Output directory")
args = parser.parse_args()

OUTPUT_DIR = args.output_dir
FOM_DIR = os.path.join(OUTPUT_DIR )
FIG_DIR = os.path.join(OUTPUT_DIR, "figures")
COMBINED_CSV_PATH = os.path.join(OUTPUT_DIR, "combined_table.csv")
OUTPUT_VARIANT_TUNING = os.path.join(OUTPUT_DIR, "output_with_variant_tuning.csv")

# Assume 'df' is your DataFrame loaded from the text file
# For example:
df = pd.read_csv(COMBINED_CSV_PATH, delimiter=',')  # Adjust delimiter as needed

# Create the new column
df['Variant_Tuning'] = df['Variant'] + '-' + df['Tuning']

# Example: Show the first few rows
# print(df[['Variant', 'Tuning', 'Variant_Tuning']].head())

# Calculate smoothed values, and plot them alongside raw values. 
df = smooth_and_plot_all_kernels(df, k=5, eps=0.1, w=3, save_dir=FIG_DIR)
df.to_csv(OUTPUT_VARIANT_TUNING, index=False)

save_kernel_tables(df,outdir=OUTPUT_DIR)

# Usage:
# save_kernel_tables(df, outdir='my_output_dir')
