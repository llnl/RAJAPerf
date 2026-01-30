#!/usr/bin/env bash
set -euo pipefail

# Usage:
#   run_full_pipeline.sh [cpx|spx] /path/to/output_dir
#
# Example:
#   run_full_pipeline.sh cpx /p/lustre/artv3/raja_perf_runs/2026-01-29

MODE="${1:-cpx}"
OUTDIR="${2:-./raja_perf_results}"

mkdir -p "${OUTDIR}"

# 1) Run tier1 mode script (CPX or SPX)
case "${MODE}" in
  cpx|CPX)
    echo "Running CPX tier1 script..."
    ./scripts/benchmarking/run_tier1_cpx-mode.sh "${OUTDIR}"
    ;;
  spx|SPX)
    echo "Running SPX tier1 script..."
    ./scripts/benchmarking/run_tier1_spx-mode.sh "${OUTDIR}"
    ;;
  *)
    echo "ERROR: MODE must be 'cpx' or 'spx' (got: ${MODE})" >&2
    exit 1
    ;;
esac

# 2) Run study_run_kernel_tunings.py
echo "Running study_run_kernel_tunings.py..."
python3 ./scripts/benchmarking/study_run_kernel_tunings.py \
  --output-dir "${OUTDIR}"


# 4) Run study_saturation.py and create FoMs
echo "Running study_saturation.py..."
python3 ./scripts/benchmarking/study_saturation.py \
  --output-dir "${OUTDIR}"

# At this point we expect a combined_table.csv in ${OUTDIR}
COMBINED="${OUTDIR}/combined_table.csv"
if [[ ! -f "${COMBINED}" ]]; then
  echo "ERROR: combined_table.csv not found in ${COMBINED}." >&2
  echo "       Check that study_* scripts are configured to write it." >&2
  exit 1
fi

echo "Done. Final FOM files are in: ${OUTDIR}"