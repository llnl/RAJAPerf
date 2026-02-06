#!/usr/bin/env bash

# Initialize modules for users not using bash as a default shell
if test -e /usr/share/lmod/lmod/init/bash
then
  . /usr/share/lmod/lmod/init/bash
fi

###############################################################################
# Copyright (c) Lawrence Livermore National Security, LLC and other
# RAJA Project Developers. See top-level LICENSE and COPYRIGHT
# files for dates and other details. No copyright assignment is required
# to contribute to RAJA Performance Suite.
#
# SPDX-License-Identifier: (BSD-3-Clause)
###############################################################################

set -o errexit
set -o nounset

option=${1:-""}
hostname="$(hostname)"
truehostname=${hostname//[0-9]/}
project_dir="$(pwd)"

hostconfig=${HOST_CONFIG:-""}
spec=${SPEC:-""}
module_list=${MODULE_LIST:-""}
job_unique_id=${CI_JOB_ID:-""}
use_dev_shm=${USE_DEV_SHM:-true}
spack_debug=${SPACK_DEBUG:-false}
debug_mode=${DEBUG_MODE:-false}
push_to_registry=${PUSH_TO_REGISTRY:-true}

raja_version=${UPDATE_RAJA:-""}
sys_type=${SYS_TYPE:-""}

spack_upstream_path=${SPACK_UPSTREAM_PATH:-"/usr/workspace/umdev/RAJAPerf/upstream"}
update_spack_upstream=${UPDATE_SPACK_UPSTREAM:-false}

# REGISTRY_TOKEN allows you to provide your own personal access token to the CI
# registry. Be sure to set the token with at least read access to the registry.
registry_token=${REGISTRY_TOKEN:-""}
ci_registry_user=${CI_REGISTRY_USER:-"${USER}"}
ci_registry_image=${CI_REGISTRY_IMAGE:-"czregistry.llnl.gov:5050/radiuss/rajaperf"}
ci_registry_token=${CI_JOB_TOKEN:-"${registry_token}"}

# Track script start time for elapsed time calculations
script_start_time=$(date +%s)

# Storage for section start times (supports nesting)
declare -A section_start_times

# Section stack for tracking nested sections
section_id_stack=()
section_counter=0
section_indent=""

# GitLab CI collapsible section helpers with nesting support
section_start ()
{
    local section_name="${1}"
    local section_title="${2}"
    local section_state="${3:-""}"

    local collapsed="false"
    if [[ "${section_state}" == "collapsed" ]]
    then
        local collapsed="true"
    fi

    # Generate unique section ID
    section_counter=$((section_counter + 1))
    local section_id="${section_name}_${section_counter}"

    local timestamp=$(date +%s)
    local current_time=$(date -d @${timestamp} --rfc-3339=seconds)
    local total_elapsed=$((timestamp - script_start_time))
    local total_elapsed_formatted=$(date -d @${total_elapsed} -u +%H:%M:%S)

    # Store section start time for later calculation
    section_start_times[${section_id}]=${timestamp}

    # Push section ID onto stack
    section_id_stack+=("${section_id}")

    echo "${section_indent}~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
    echo "${section_indent}~ TIME                      | TOTAL    | SECTION  "
    echo "${section_indent}~ ${current_time} | ${total_elapsed_formatted} | ${section_title}"
    echo -e "\e[0Ksection_start:${timestamp}:${section_id}[collapsed=${collapsed}]\r\e[0K${section_indent}~ ${section_title}"

    # Increase indentation for nested sections
    section_indent="${section_indent}  "
}

section_end ()
{
    # Pop section ID from stack
    if [[ ${#section_id_stack[@]} -eq 0 ]]; then
        echo "[Warning]: section_end called with empty stack"
        return 1
    fi

    # Decrease indentation before displaying
    section_indent="${section_indent%  }"

    local stack_index=$((${#section_id_stack[@]} - 1))
    local section_id="${section_id_stack[$stack_index]}"
    unset section_id_stack[$stack_index]

    local timestamp=$(date +%s)
    local current_time=$(date -d @${timestamp} --rfc-3339=seconds)
    local total_elapsed=$((timestamp - script_start_time))
    local total_elapsed_formatted=$(date -d @${total_elapsed} -u +%H:%M:%S)

    # Calculate section elapsed time
    local section_start=${section_start_times[${section_id}]:-${timestamp}}
    local section_elapsed=$((timestamp - section_start))
    local section_elapsed_formatted=$(date -d @${section_elapsed} -u +%H:%M:%S)

    echo -e "\e[0Ksection_end:${timestamp}:${section_id}\r\e[0K"
    echo "${section_indent}~ ${current_time} | ${total_elapsed_formatted} | ${section_elapsed_formatted}"
    echo "${section_indent}~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

    # Clean up stored time
    unset section_start_times[${section_id}]
}

if [[ ${debug_mode} == true ]]
then
    echo "[Information]: Debug mode:"
    echo "[Information]: - Spack debug mode."
    echo "[Information]: - Deactivated shared memory."
    echo "[Information]: - Do not push to buildcache."
    use_dev_shm=false
    spack_debug=true
    push_to_registry=false
fi

if [[ -n ${module_list} ]]
then
    echo "[Information]: Loading modules: ${module_list}"
    module load ${module_list}
fi

prefix=""

if [[ ${update_spack_upstream} == true ]]
then
    use_dev_shm=false
    echo "We don't build in shared memory when updating the spack upstream"

    prefix=${spack_upstream_path}
    mkdir -p ${prefix}
elif [[ -d /dev/shm && ${use_dev_shm} == true ]]
then
    prefix="/dev/shm/${hostname}"
    if [[ -z ${job_unique_id} ]]; then
      job_unique_id=manual_job_$(date +%s)
      while [[ -d ${prefix}-${job_unique_id} ]] ; do
          sleep 1
          job_unique_id=manual_job_$(date +%s)
      done
    fi

    prefix="${prefix}-${job_unique_id}"
else
    # We set the prefix in the parent directory so that spack dependencies are not installed inside the source tree.
    prefix="$(pwd)/../spack-and-build-root"
fi

echo "[Information]: Creating directory ${prefix}"
echo "[Information]: project_dir: ${project_dir}"

mkdir -p ${prefix}

spack_cmd="${prefix}/spack/bin/spack"
spack_env_path="${prefix}/spack_env"
uberenv_cmd="$(pwd)/tpl/RAJA/scripts/uberenv/uberenv.py --project-json=$(pwd)/.uberenv_config.json"
if [[ ${spack_debug} == true ]]
then
    spack_cmd="${spack_cmd} --debug --stacktrace"
    uberenv_cmd="${uberenv_cmd} --spack-debug"
fi

# Dependencies
if [[ "${option}" != "--build-only" && "${option}" != "--test-only" ]]
then
    section_start "dependencies" "Building Dependencies"

    if [[ -z ${spec} ]]
    then
        echo "[Error]: SPEC is undefined, aborting..."
        exit 1
    fi

    prefix_opt="--prefix=${prefix}"

    upstream_opt=""
    if [[ ${update_spack_upstream} == false && -e ${spack_upstream_path}/.spack-db ]]
    then
        upstream_opt="--upstream=${spack_upstream_path}"
    fi

    # We force Spack to put all generated files (cache and configuration of
    # all sorts) in a unique location so that there can be no collision
    # with existing or concurrent Spack.
    spack_user_cache="${prefix}/spack-user-cache"
    export SPACK_DISABLE_LOCAL_CONFIG=""
    export SPACK_USER_CACHE_PATH="${spack_user_cache}"
    mkdir -p ${spack_user_cache}

    # generate cmake cache file with uberenv and radiuss spack package
    section_start "spack_setup" "Spack setup and environment" "collapsed"
    ${uberenv_cmd} --setup-and-env-only --spec="${spec}" ${prefix_opt} ${upstream_opt}
    section_end

    if [[ -n ${ci_registry_token} ]]
    then
        section_start "registry_setup" "GitLab registry as Spack Buildcache" "collapsed"
        ${spack_cmd} -D ${spack_env_path} mirror add --unsigned --oci-username ${ci_registry_user} --oci-password ${ci_registry_token} gitlab_ci oci://${ci_registry_image}
        section_end
    fi

    section_start "spack_build" "Spack build of dependencies" "collapsed"
    ${uberenv_cmd} --skip-setup-and-env --spec="${spec}" ${prefix_opt} ${upstream_opt}
    section_end

    if [[ -n ${ci_registry_token} && ${push_to_registry} == true ]]
    then
        section_start "buildcache_push" "Push dependencies to buildcache" "collapsed"
        ${spack_cmd} -D ${spack_env_path} buildcache push --only dependencies gitlab_ci
        section_end
    fi

    mv ${project_dir}/tpl/RAJA/*.cmake ${project_dir}/.

    section_end
fi

# Find cmake cache file (hostconfig)
if [[ -z ${hostconfig} ]]
then
    # If no host config file was provided, we assume it was generated.
    # This means we are looking of a unique one in project dir.
    hostconfigs=( $( ls "${project_dir}/"*.cmake ) )
    if [[ ${#hostconfigs[@]} == 1 ]]
    then
        hostconfig_path=${hostconfigs[0]}
    elif [[ ${#hostconfigs[@]} == 0 ]]
    then
        echo "[Error]: No result for: ${project_dir}/*.cmake"
        echo "[Error]: Spack generated host-config not found."
        exit 1
    else
        echo "[Error]: More than one result for: ${project_dir}/*.cmake"
        echo "[Error]: ${hostconfigs[@]}"
        echo "[Error]: Please specify one with HOST_CONFIG variable"
        exit 1
    fi
else
    # Using provided host-config file.
    hostconfig_path="${project_dir}/${hostconfig}"
fi

hostconfig=$(basename ${hostconfig_path})
echo "[Information]: Found hostconfig ${hostconfig_path}"

# Build Directory
# When using /dev/shm, we use prefix for both spack builds and source build, unless BUILD_ROOT was defined
build_root=${BUILD_ROOT:-"${prefix}"}

build_dir="${build_root}/build_${hostconfig//.cmake/}"

cmake_exe=`grep 'CMake executable' ${hostconfig_path} | cut -d ':' -f 2 | xargs`

# Build
if [[ "${option}" != "--deps-only" && "${option}" != "--test-only" ]]
then
    echo "[Information]: Host-config  ${hostconfig_path}"
    echo "[Information]: Build Dir    ${build_dir}"
    echo "[Information]: Project Dir  ${project_dir}"

    section_start "clean" "Cleaning working directory" "collapsed"
    # Map CPU core allocations
    declare -A core_counts=(["lassen"]=40 ["poodle"]=28 ["dane"]=28 ["matrix"]=28 ["corona"]=32 ["rzansel"]=48 ["tioga"]=32 ["tuolumne"]=48)

    # If using Multi-project, set up the submodule
    if [[ -n ${raja_version} ]]
    then
      cd tpl/RAJA
      section_start "raja_submodule_update" "Updating RAJA Submodule to develop" "collapsed"
      git pull origin develop
      section_end
      section_start "raja_submodules" "Updating Submodules within RAJA" "collapsed"
      git submodule update --init --recursive
      section_end
      cd -
    fi

    # If building, then delete everything first
    # NOTE: 'cmake --build . -j core_counts' attempts to reduce individual build resources.
    #       If core_counts does not contain hostname, then will default to '-j ', which should
    #       use max cores.
    rm -rf ${build_dir} 2>/dev/null
    mkdir -p ${build_dir} && cd ${build_dir}
    section_end

    # We set the MPI tests command to allow overlapping.
    # Shared allocation: Allows build_and_test.sh to run within a sub-allocation (see CI config).
    # Use /dev/shm: Prevent MPI tests from running on a node where the build dir doesn't exist.
    cmake_options=""
    if [[ "${truehostname}" == "poodle" || "${truehostname}" == "dane" ]]
    then
        cmake_options="-DBLT_MPI_COMMAND_APPEND:STRING=--overlap"
    fi

    section_start "cmake_config" "CMake Configuration" "collapsed"
    if ! $cmake_exe \
      -C ${hostconfig_path} \
      ${cmake_options} \
      ${project_dir}
      then
        section_end
        echo "[Error]: CMake configuration failed, dumping output..."
        section_start "cmake_config_verbose" "Verbose CMake Configuration"
        $cmake_exe \
          -C ${hostconfig_path} \
          ${cmake_options} \
          ${project_dir} --debug-output --trace-expand
        section_end
        exit 1
      else
        section_end
    fi

    section_start "build" "Building RAJAPerf" "collapsed"
    if ! $cmake_exe --build . -j ${core_counts[$truehostname]}
    then
        section_end
        echo "[Error]: Compilation failed, building with verbose output..."
        section_start "build_verbose" "Verbose Rebuild" "collapsed"
        $cmake_exe --build . --verbose -j 1
        section_end
    else
        section_end
    fi
fi

# Test
if [[ "${option}" != "--build-only" ]] && grep -q -i "ENABLE_TESTS.*ON" ${hostconfig_path}
then

    if [[ ! -d ${build_dir} ]]
    then
        echo "[Error]: Build directory not found : ${build_dir}" && exit 1
    fi

    cd ${build_dir}

    section_start "tests" "Running Tests" "collapsed"
    ctest --output-on-failure -T test 2>&1 | tee tests_output.txt

    no_test_str="No tests were found!!!"
    if [[ "$(tail -n 1 tests_output.txt)" == "${no_test_str}" ]]
    then
        echo "[Error]: No tests were found" && exit 1
    fi

    tree Testing
    xsltproc -o junit.xml ${project_dir}/blt/tests/ctest-to-junit.xsl Testing/*/Test.xml
    mv junit.xml ${project_dir}/junit.xml

    if grep -q "Errors while running CTest" ./tests_output.txt
    then
        echo "[Error]: Failure(s) while running CTest" && exit 1
    fi
    section_end
fi

#timed_message "Cleaning up"
#make clean

cd ${project_dir}

echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
echo "~ Build and test completed"
echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
