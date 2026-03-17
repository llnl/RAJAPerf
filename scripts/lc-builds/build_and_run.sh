#!/usr/bin/env bash

###############################################################################
# Copyright (c) Lawrence Livermore National Security, LLC and other
# RAJA Project Developers. See top-level LICENSE and COPYRIGHT
# files for dates and other details. No copyright assignment is required
# to contribute to RAJA Performance Suite.
#
# SPDX-License-Identifier: (BSD-3-Clause)
###############################################################################

set -uo pipefail

usage() {
  cat <<'EOF'
Usage:
  scripts/lc-builds/build_and_run.sh <build_script.sh> <compiler_list.txt> [options] -- [raja-perf args...]

Description:
  Reads <compiler_list.txt> (one entry per line) and, for each line, invokes
  <build_script.sh> with the tokens on that line as its arguments, then builds
  and runs the suite from that build directory.

  The build script is sourced (not executed) so any 'module load ...' it does
  applies to the subsequent build+run.

List file format:
  - Blank lines and lines starting with '#' are ignored.
  - Each line is treated as arguments to the build script.
    Examples (for toss4_gcc.sh):
      12.3.0
      12.3.0 3.27.4 -DENABLE_OPENMP=On

Options:
  --kernel <name>        Add a kernel or group to run (repeatable)
  --kernel-file <path>   File with kernels/groups (one per line; '#' comments ok)
  --run-cmd <string>     Required to run; prefix command (e.g. "srun -N1 -n1 -c 64 --" or "direct")
  --no-warmup-same       Do not add --warmup-perfrun-same
  --no-skip-existing     Always reconfigure (do not reuse existing build dirs)
  -j, --jobs <N>        Parallel build jobs (default: nproc)
  --configure-only      Only run configure step (skip build+run)
  --build-only          Configure (if needed) + build (skip run)
  --keep-going          Continue after failures (default: on)
  --fail-fast           Stop at first failure
  --log-dir <path>      Log directory (default: logs/<script>-YYYYmmdd-HHMMSS)
  -h, --help            Show this help

Example:
  scripts/lc-builds/build_and_run.sh toss4_amdclang.sh compiler_list.txt --run-cmd "srun -N1 -n1 -c 64 --" --kernel INIT3 -- -i 10
  scripts/lc-builds/build_and_run.sh toss4_amdclang.sh compiler_list.txt --kernel-file kernels.txt -- --dryrun
EOF
}

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
repo_root="$(cd -- "${script_dir}/../.." && pwd -P)"

if [[ $# -lt 2 ]]; then
  usage >&2
  exit 2
fi

build_script="$1"
list_file="$2"
shift 2

jobs=""
configure_only=0
build_only=0
keep_going=1
log_dir=""
requested_kernels=()
skip_existing=1
warmup_same=1
run_cmd=""
run_args=()

while [[ $# -gt 0 ]]; do
  case "$1" in
    --kernel)
      requested_kernels+=("${2:-}")
      shift 2
      ;;
    --kernel-file)
      kernel_file="${2:-}"
      if [[ -z "${kernel_file}" || ! -f "${kernel_file}" ]]; then
        echo "Kernel file not found: ${kernel_file}" >&2
        exit 2
      fi
      while IFS= read -r raw_kline || [[ -n "${raw_kline}" ]]; do
        kline="${raw_kline#"${raw_kline%%[![:space:]]*}"}" # ltrim
        kline="${kline%"${kline##*[![:space:]]}"}"         # rtrim
        if [[ -z "${kline}" || "${kline:0:1}" == "#" ]]; then
          continue
        fi
        requested_kernels+=("${kline}")
      done < "${kernel_file}"
      shift 2
      ;;
    --no-skip-existing)
      skip_existing=0
      shift
      ;;
    --no-warmup-same)
      warmup_same=0
      shift
      ;;
    --run-cmd)
      run_cmd="${2:-}"
      shift 2
      ;;
    -j|--jobs)
      jobs="${2:-}"
      shift 2
      ;;
    --configure-only)
      configure_only=1
      shift
      ;;
    --build-only)
      build_only=1
      shift
      ;;
    --keep-going)
      keep_going=1
      shift
      ;;
    --fail-fast)
      keep_going=0
      shift
      ;;
    --log-dir)
      log_dir="${2:-}"
      shift 2
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    --)
      shift
      run_args=("$@")
      break
      ;;
    *)
      echo "Unknown option: $1" >&2
      echo >&2
      usage >&2
      exit 2
      ;;
  esac
done

# If we're going to run, require an explicit run command prefix.
if [[ "${configure_only}" -eq 0 && "${build_only}" -eq 0 && -z "${run_cmd}" ]]; then
  echo "Missing required --run-cmd (use e.g. --run-cmd \"srun -N1 -n1 -c 64 --\" or --run-cmd direct)." >&2
  exit 2
fi

# Translate requested kernels into raja-perf args (prepend so user can still override after '--').
kernel_args=()
if [[ ${#requested_kernels[@]} -gt 0 ]]; then
  kernel_args=(--kernels "${requested_kernels[@]}")
fi

# Add warmup behavior unless user explicitly controls warmup in run args.
warmup_args=()
if [[ "${warmup_same}" -eq 1 ]]; then
  has_warmup_control=0
  for arg in "${run_args[@]}"; do
    case "${arg}" in
      --warmup-kernels|-wk|--warmup-disable|--warmup-perfrun-same)
        has_warmup_control=1
        break
        ;;
    esac
  done
  if [[ "${has_warmup_control}" -eq 0 ]]; then
    warmup_args=(--warmup-perfrun-same)
  fi
fi

# Resolve build script path (allow passing e.g. 'toss4_gcc.sh').
if [[ "${build_script}" != /* && "${build_script}" != ./* && "${build_script}" != ../* ]]; then
  if [[ -f "${script_dir}/${build_script}" ]]; then
    build_script="${script_dir}/${build_script}"
  fi
fi

if [[ ! -f "${build_script}" ]]; then
  echo "Build script not found: ${build_script}" >&2
  exit 2
fi

if [[ ! -f "${list_file}" ]]; then
  echo "List file not found: ${list_file}" >&2
  exit 2
fi

if [[ -z "${jobs}" ]]; then
  if command -v nproc >/dev/null 2>&1; then
    jobs="$(nproc)"
  else
    jobs="1"
  fi
fi

if [[ -z "${log_dir}" ]]; then
  ts="$(date +%Y%m%d-%H%M%S)"
  base="$(basename -- "${build_script}")"
  base="${base%.sh}"
  log_dir="${repo_root}/logs/${base}-${ts}"
fi
mkdir -p "${log_dir}"

echo "Repo root    : ${repo_root}"
echo "Build script : ${build_script}"
echo "List file    : ${list_file}"
echo "Jobs         : ${jobs}"
echo "Log dir      : ${log_dir}"
echo "Kernels      : ${requested_kernels[*]:-(all)}"
echo "Skip existing: ${skip_existing}"
echo "Warmup same  : ${warmup_same}"
echo "Run cmd      : ${run_cmd:-"(none)"}"
echo "Run args     : ${run_args[*]:-(none)}"
echo

failures=0
total=0

line_no=0
while IFS= read -r raw_line || [[ -n "${raw_line}" ]]; do
  line_no=$((line_no + 1))

  line="${raw_line#"${raw_line%%[![:space:]]*}"}" # ltrim
  line="${line%"${line##*[![:space:]]}"}"         # rtrim

  if [[ -z "${line}" || "${line:0:1}" == "#" ]]; then
    continue
  fi

  total=$((total + 1))
  entry_id="$(printf "%03d" "${total}")"

  IFS=$' \t' read -r -a script_args <<<"${line}"
  log_prefix="${log_dir}/${entry_id}"

  echo "==> [${entry_id}] line ${line_no}: ${line}"

  if ! bash -lc '
    set -euo pipefail
    repo_root="$1"; shift
    build_script="$1"; shift
    jobs="$1"; shift
    configure_only="$1"; shift
    build_only="$1"; shift
    skip_existing="$1"; shift
    run_cmd="$1"; shift
    delim="$1"; shift

    script_args=()
    while [[ $# -gt 0 && "$1" != "$delim" ]]; do
      script_args+=("$1")
      shift
    done
    if [[ $# -gt 0 && "$1" == "$delim" ]]; then
      shift
    fi
    perf_args=("$@")

    cd "$repo_root"

    # Many existing lc-build scripts reference optional positional args (e.g. "$3")
    # without guarding for nounset. Disable it while sourcing for compatibility.
    #
    # If requested, probe for an existing configured build without deleting it.
    if [[ "$skip_existing" == "1" ]]; then
      build_preexisted=0

      cmake() { :; }
      rm() { :; }
      mkdir() {
        local arg
        for arg in "$@"; do
          [[ "$arg" == -* ]] && continue
          if [[ -d "$arg" ]]; then
            build_preexisted=1
          fi
        done
        command mkdir -p "$@"
      }

      set +u
      # shellcheck disable=SC1090
      source "$build_script" "${script_args[@]}"
      set -u

      build_dir="$PWD"
      configured_present=0
      if [[ "$build_preexisted" == "1" && -f "${build_dir}/CMakeCache.txt" && ( -f "${build_dir}/Makefile" || -f "${build_dir}/build.ninja" ) ]]; then
        configured_present=1
      fi

      cd "$repo_root"
      unset -f cmake rm mkdir

      if [[ "$configured_present" == "1" ]]; then
        cd "$build_dir"
      else
        set +u
        # shellcheck disable=SC1090
        source "$build_script" "${script_args[@]}"
        set -u
      fi
    else
      set +u
      # shellcheck disable=SC1090
      source "$build_script" "${script_args[@]}"
      set -u
    fi

    if [[ "$configure_only" == "0" ]]; then
      cmake --build . --parallel "$jobs"
      if [[ "$build_only" == "0" ]]; then
        if [[ "$run_cmd" == "direct" ]]; then
          ./bin/raja-perf.exe "${perf_args[@]}"
        else
          quoted=()
          for a in "${perf_args[@]}"; do
            printf -v q "%q" "$a"
            quoted+=("$q")
          done
          eval "${run_cmd} ./bin/raja-perf.exe ${quoted[*]}"
        fi
      fi
    fi
  ' bash "${repo_root}" "${build_script}" "${jobs}" "${configure_only}" "${build_only}" "${skip_existing}" "${run_cmd}" \
      "__RAJAPERF_MATRIX_DELIM__" "${script_args[@]}" "__RAJAPERF_MATRIX_DELIM__" "${warmup_args[@]}" "${kernel_args[@]}" "${run_args[@]}" \
      </dev/null \
      2>&1 | tee "${log_prefix}.log"; then
    echo "FAILED: line ${line_no}: ${line}" >&2
    failures=$((failures + 1))
    if [[ "${keep_going}" -eq 0 ]]; then
      break
    fi
  fi
  echo
done < "${list_file}"

echo "Done. Total entries: ${total}; failures: ${failures}"
if [[ "${failures}" -ne 0 ]]; then
  exit 1
fi
