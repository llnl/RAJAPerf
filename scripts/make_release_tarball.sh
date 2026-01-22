#!/usr/bin/env bash

###############################################################################
# Copyright (c) Lawrence Livermore National Security, LLC and other
# RAJA Project Developers. See top-level LICENSE and COPYRIGHT
# files for dates and other details. No copyright assignment is required
# to contribute to RAJA Performance Suite.
#
# SPDX-License-Identifier: (BSD-3-Clause)
#################################################################################

TAR_CMD=`which tar`
VERSION=`git describe --tags`

git archive --prefix=RAJAPerf-${VERSION}/ -o RAJAPerf-${VERSION}.tar HEAD 2> /dev/null

echo "Running git archive submodules..."

p=`pwd` && (echo .; git submodule foreach --recursive) | while read entering path; do
    temp="${path%\'}";
    temp="${temp#\'}";
    path=$temp;
    [ "$path" = "" ] && continue;
    (cd $path && git archive --prefix=RAJAPerf-${VERSION}/$path/ HEAD > $p/tmp.tar && ${TAR_CMD} --concatenate --file=$p/RAJAPerf-${VERSION}.tar $p/tmp.tar && rm $p/tmp.tar);
done

gzip RAJAPerf-${VERSION}.tar
