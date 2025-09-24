#!/usr/bin/env python3
"""
Convert Caliper .cali output to GitHub benchmark action JSON format.

Usage: python caliper_to_github_benchmark.py "pattern" output.json
       python caliper_to_github_benchmark.py input.cali output.json
       python caliper_to_github_benchmark.py "/path/to/*_*-*.cali" output.json
"""
import caliperreader as cr
import glob
import json
import sys
from pathlib import Path

METRIC = 'avg#inclusive#sum#time.duration'
REPS_METRIC = 'any#any#max#Reps'


def caliper_to_benchmark_json(input_file):
    """Convert one Caliper file to benchmark format.
    Args:
        input_file: Path to Caliper file
    Returns:
        List of benchmark dictionaries
    """

    variant = Path(input_file).stem

    reader = cr.CaliperReader()
    reader.read(input_file)

    benchmarks = []

    for record in reader.records:
        path = record['path'] if 'path' in record else 'UNKNOWN'
        total_time_s = float(record[METRIC]) if METRIC in record else 0.0
        total_time_ms = total_time_s * 1000
        reps = int(record[REPS_METRIC]) if REPS_METRIC in record else 1

        # Calculate average time per rep
        avg_time_per_rep_ms = total_time_ms / reps if reps > 0 else total_time_ms

        # Gather leaf-level kernel paths like "RAJAPerf/Group/KernelName"
        if isinstance(path, list) and len(path) >= 3:
            kernel_name = path[-1]
            benchmark = {
                "name": f"{variant}_{kernel_name}",
                "unit": "ms/rep",
                "value": avg_time_per_rep_ms
            }
            benchmarks.append(benchmark)

    return benchmarks


if __name__ == "__main__":
    if len(sys.argv) != 3:
        sys.exit("Usage: caliper_to_github_benchmark.py \"input_file_glob_pattern\" output.json")

    pattern = sys.argv[1]
    output_file = sys.argv[2]

    cali_files = glob.glob(pattern)

    if not cali_files:
        sys.exit(f"Error: No .cali files found matching pattern: {pattern}")

    print(f"Found {len(cali_files)} Caliper files to process")

    all_benchmarks = []
    for cali_file in cali_files:
        print(f"Processing {cali_file}")
        benchmarks = caliper_to_benchmark_json(cali_file)
        all_benchmarks.extend(benchmarks)

    with open(output_file, 'w') as f:
        json.dump(all_benchmarks, f, indent=2)

    print(f"Converted {len(all_benchmarks)} benchmarks from {len(cali_files)} files to {output_file}")
