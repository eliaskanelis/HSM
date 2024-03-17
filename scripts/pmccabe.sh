#!/usr/bin/env bash

# McCabe Cyclomatic complexity

set -euo pipefail


# Analyse all C/C++ files

find . -type d \( \
     -path './.git' -o \
     -path './conf' -o \
     -path './port/*/sys' -o \
     -path './thirdparty' \
     \) -prune \
     -o -name  '*.[c|h|cpp|hpp]' -print0 | xargs -0 pmccabe -r 10
