#!/usr/bin/env bash

# Documentation of C/C++ code

set -euo pipefail

mkdir -p "doc/html"

# Generate doc
export PROJECT_NUMBER="Beta"
export PROJECT_NAME="Untitled"
export PROJECT_BRIEF=""

(cd conf/doxygen/ && doxygen)
