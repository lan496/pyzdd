#!/bin/sh -eu
BASEDIR=$(dirname "$0")

find ${BASEDIR} -name "*.so" -delete
find ${BASEDIR} -name "*.pyc" -delete
find ${BASEDIR} -name "*.pyo" -delete
find ${BASEDIR} -name "__pycache__" -delete
rm -rf "${BASEDIR}/build" "${BASEDIR}/dist" .benchmarks .eggs .mypy_cache .pytest_cache pip-wheel-metadata *.egg-info tmp _deps CMakeFiles cmake_install.cmake CMakeCache.txt
