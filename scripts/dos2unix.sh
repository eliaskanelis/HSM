#!/usr/bin/env bash

# Dos2unix

set -euo pipefail

find . -type d \( \
     -path './.git' -o \
     -path './thirdparty' \
     \) -prune -o -print0 | xargs -0 dos2unix -s -q
