#!/bin/sh
here=$(cd "$(dirname "$0")" && pwd)
cpython=${SLATE_CPYTHON:-$here/cpython}
build=$cpython/build-clang
if [ ! -f "$build/pybuilddir.txt" ]; then
    echo "run.sh: no CPython build at $build; see README.md" >&2
    exit 1
fi
export PYTHONHOME="$cpython"
export PYTHONPATH="$cpython/Lib:$build/$(cat "$build/pybuilddir.txt")${PYTHONPATH:+:$PYTHONPATH}"
if [ $# -eq 0 ] || [ ! -x "$1" ] || [ -d "$1" ]; then
    (cd "$here/python" && cargo build --release -q) || exit 1
    set -- "$here/cargo-target/x86_64-unknown-linux-gnu/release/python" "$@"
fi
exec "$@"
