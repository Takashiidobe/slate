#!/usr/bin/env bash
# Builds the macro-dump Clang plugin (tools/macro-dump-plugin/MacroDump.cpp)
# against the same Clang/LLVM tree SLATE_CLANG points at, so the resulting
# .so can be loaded with `-fplugin=` into that exact clang binary.
#
# Env vars (all optional):
#   SLATE_CLANG   path to the clang binary to build against
#                 (default: $HOME/llvm-project/build-cir/bin/clang)
#   CXX           C++ compiler used to build the plugin (default: clang++)
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CLANG_BIN="${SLATE_CLANG:-$HOME/llvm-project/build-cir/bin/clang}"
CXX="${CXX:-clang++}"

if [[ ! -x "$CLANG_BIN" ]]; then
  echo "error: clang binary not found or not executable: $CLANG_BIN" >&2
  echo "set SLATE_CLANG to the clang binary to build the plugin against" >&2
  exit 1
fi

BUILD_DIR="$(cd "$(dirname "$CLANG_BIN")/.." && pwd)"
CACHE="$BUILD_DIR/CMakeCache.txt"
if [[ ! -f "$CACHE" ]]; then
  echo "error: no CMakeCache.txt at $BUILD_DIR -- is SLATE_CLANG pointing into a CMake build dir?" >&2
  exit 1
fi

LLVM_SRC="$(sed -n 's/^CMAKE_HOME_DIRECTORY:INTERNAL=//p' "$CACHE")"
if [[ -z "$LLVM_SRC" ]]; then
  echo "error: could not read CMAKE_HOME_DIRECTORY from $CACHE" >&2
  exit 1
fi
CLANG_SRC="$LLVM_SRC/../clang/include"

OUT_DIR="$SCRIPT_DIR/build"
mkdir -p "$OUT_DIR"
OUT="$OUT_DIR/MacroDump.so"
INSTALLED="$BUILD_DIR/lib/SlateMacroDump.so"

"$CXX" \
  -D_GLIBCXX_USE_CXX11_ABI=1 -D_GNU_SOURCE \
  -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS \
  -I"$CLANG_SRC" \
  -I"$BUILD_DIR/tools/clang/include" \
  -I"$BUILD_DIR/include" \
  -I"$LLVM_SRC/include" \
  -fPIC -std=c++17 -fno-exceptions -fno-rtti -O2 -shared \
  -o "$OUT" "$SCRIPT_DIR/MacroDump.cpp"

install -Dm755 "$OUT" "$INSTALLED"

echo "built $OUT"
echo "installed $INSTALLED"
echo "run with: $CLANG_BIN -fplugin=$INSTALLED -fsyntax-only <file.c>"
