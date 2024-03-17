#!/usr/bin/env bash

# Lints C/C++ code

set -euo pipefail

buildDir="obj/cppcheck"

mkdir -p "${buildDir}"

# Check
cppcheck --quiet --std=c99 --enable=warning --error-exitcode=1 --force \
	 --cppcheck-build-dir="${buildDir}" \
	 --addon=conf/cppcheck/misra.json --inline-suppr \
	 -Isrc -Iinc \
	 -i thirdparty -i tests -i apps \
	 .
