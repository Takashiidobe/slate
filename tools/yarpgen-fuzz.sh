#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

export SLATE_CLANG="${SLATE_CLANG:-$HOME/llvm-project/build-cir/bin/clang}"
export YARPGEN_BIN="${YARPGEN_BIN:-$HOME/yarpgen/build/yarpgen}"
export YARPGEN_FUZZ_ARGS="$*"

cargo nextest run --release --manifest-path "$ROOT_DIR/Cargo.toml" --test yarpgen -- --ignored --nocapture
