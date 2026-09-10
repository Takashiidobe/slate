#!/usr/bin/env bash
# Builds the macro-dump Clang plugin (tools/macro-dump-plugin/MacroDump.cpp)
# against the same Clang/LLVM SLATE_CLANG points at, so the resulting .so
# can be loaded with `-fplugin=` into that exact clang binary. A plugin
# built against one clang is ABI-incompatible with any other clang, even a
# different minor version -- there is no cross-version compatibility here.
#
# Two source modes, auto-detected from SLATE_CLANG:
#   source tree  SLATE_CLANG points into a CMake build dir (has
#                CMakeCache.txt) -- headers come from that checkout, and the
#                built plugin is installed to <build dir>/lib/.
#   installed    SLATE_CLANG points at a package-managed or release clang
#                with no build dir alongside it -- headers come from
#                `llvm-config --includedir` and no source checkout is
#                needed. The built plugin is left under tools/macro-dump-plugin/build/.
#
# Env vars (all optional):
#   SLATE_CLANG   path to the clang binary to build against
#                 (default: $HOME/llvm-project/build-cir/bin/clang, else
#                 whatever `clang` resolves to on PATH)
#   CXX           C++ compiler used to build the plugin
#                 (default: the clang++ next to SLATE_CLANG, else clang++ on PATH)
#   LLVM_CONFIG   llvm-config binary to query for installed-mode include dirs
#                 (default: llvm-config next to SLATE_CLANG, else on PATH)
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [[ -n "${SLATE_CLANG:-}" ]]; then
  CLANG_BIN="$SLATE_CLANG"
elif [[ -x "$HOME/llvm-project/build-cir/bin/clang" ]]; then
  CLANG_BIN="$HOME/llvm-project/build-cir/bin/clang"
else
  CLANG_BIN="$(command -v clang || true)"
fi

if [[ -z "$CLANG_BIN" || ! -x "$CLANG_BIN" ]]; then
  echo "error: clang binary not found or not executable: ${CLANG_BIN:-<unset>}" >&2
  echo "set SLATE_CLANG to the clang binary to build the plugin against" >&2
  exit 1
fi

CLANG_DIR="$(cd "$(dirname "$CLANG_BIN")" && pwd)"
BUILD_DIR="$(cd "$CLANG_DIR/.." && pwd)"
CACHE="$BUILD_DIR/CMakeCache.txt"

OUT_DIR="$SCRIPT_DIR/build"
mkdir -p "$OUT_DIR"
OUT="$OUT_DIR/MacroDump.so"

COMMON_FLAGS=(
  -D_GLIBCXX_USE_CXX11_ABI=1 -D_GNU_SOURCE
  -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS
  -fPIC -std=c++17 -fno-exceptions -fno-rtti -O2 -shared
)

if [[ -f "$CACHE" ]]; then
  # source tree mode
  CXX="${CXX:-clang++}"

  LLVM_SRC="$(sed -n 's/^CMAKE_HOME_DIRECTORY:INTERNAL=//p' "$CACHE")"
  if [[ -z "$LLVM_SRC" ]]; then
    echo "error: could not read CMAKE_HOME_DIRECTORY from $CACHE" >&2
    exit 1
  fi
  CLANG_SRC="$LLVM_SRC/../clang/include"
  INSTALLED="$BUILD_DIR/lib/SlateMacroDump.so"

  "$CXX" "${COMMON_FLAGS[@]}" \
    -I"$CLANG_SRC" \
    -I"$BUILD_DIR/tools/clang/include" \
    -I"$BUILD_DIR/include" \
    -I"$LLVM_SRC/include" \
    -o "$OUT" "$SCRIPT_DIR/MacroDump.cpp"

  install -Dm755 "$OUT" "$INSTALLED"

  echo "built $OUT (source-tree mode, against $LLVM_SRC)"
  echo "installed $INSTALLED"
  echo "run with: $CLANG_BIN -fplugin=$INSTALLED -fsyntax-only <file.c>"
else
  # installed mode: no source checkout, headers from llvm-config
  if [[ -x "$CLANG_DIR/clang++" ]]; then
    CXX="${CXX:-$CLANG_DIR/clang++}"
  else
    CXX="${CXX:-clang++}"
  fi

  if [[ -n "${LLVM_CONFIG:-}" ]]; then
    : # respect explicit override
  elif [[ -x "$CLANG_DIR/llvm-config" ]]; then
    LLVM_CONFIG="$CLANG_DIR/llvm-config"
  else
    LLVM_CONFIG="$(command -v llvm-config || true)"
  fi

  if [[ -z "$LLVM_CONFIG" || ! -x "$LLVM_CONFIG" ]]; then
    echo "error: no CMakeCache.txt at $BUILD_DIR (not a source build) and no llvm-config found" >&2
    echo "install your platform's LLVM/Clang development package (provides llvm-config" >&2
    echo "and clang/llvm headers matching $CLANG_BIN) or set LLVM_CONFIG explicitly" >&2
    exit 1
  fi

  CLANG_VERSION="$("$CLANG_BIN" --version | head -1)"
  LLVM_CONFIG_VERSION="$("$LLVM_CONFIG" --version)"
  echo "note: building against installed headers from '$LLVM_CONFIG' (llvm $LLVM_CONFIG_VERSION)" >&2
  echo "note: target clang is '$CLANG_BIN' ($CLANG_VERSION)" >&2
  echo "note: these must be the same LLVM/Clang major version and build config, or the" >&2
  echo "      plugin will fail to load with an undefined-symbol error" >&2

  INCLUDEDIR="$("$LLVM_CONFIG" --includedir)"
  INSTALLED="$OUT"

  "$CXX" "${COMMON_FLAGS[@]}" \
    -I"$INCLUDEDIR" \
    -o "$OUT" "$SCRIPT_DIR/MacroDump.cpp"

  echo "built $OUT (installed-clang mode, no source checkout used)"
  echo "run with: $CLANG_BIN -fplugin=$INSTALLED -fsyntax-only <file.c>"
  echo "or export SLATE_MACRO_DUMP_PLUGIN=$INSTALLED"
fi
