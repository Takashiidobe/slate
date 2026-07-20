#!/bin/bash
# Packages clang/cir-opt (built in an ubuntu:24.04 container for glibc/GLIBCXX
# compatibility with Claude Code cloud sandboxes) and alive-tv (built locally;
# already compatible) into a self-contained tarball and publishes it as a
# GitHub release asset for cloud environment setup scripts to fetch.
set -euo pipefail

REPO="Takashiidobe/slate"
TAG="toolchain"
CIR_BIN="${CIR_BIN:-$HOME/llvm-project/build-cir-linux/bin}"
ALIVE_BIN="${ALIVE_BIN:-$HOME/alive2/build/alive-tv}"
ASSET_NAME="slate-toolchain-linux-x86_64.tar.gz"

STAGE="$(mktemp -d)"
trap 'rm -rf "$STAGE"' EXIT
mkdir -p "$STAGE/bin" "$STAGE/lib/alive"

cp "$CIR_BIN/clang-23" "$CIR_BIN/cir-opt" "$STAGE/bin/"
cp "$ALIVE_BIN" "$STAGE/bin/alive-tv"
for name in clang clang++ clang-cpp clang-cl; do
  ln -s clang-23 "$STAGE/bin/$name"
done

# clang/cir-opt need only libstdc++/libm/libgcc_s/libc, all present on any
# Ubuntu 24.04 target already — bundling glibc-family libs is unsafe, since
# they carry internal ABI markers (e.g. GLIBC_ABI_DT_X86_64_PLT) that must
# match the target's own libc.so.6 exactly, not the build host's.
#
# alive-tv also needs libz3/libhiredis at exact SONAMEs (libz3.so.4.16,
# libhiredis.so.1.3.0) that Ubuntu's apt packages don't provide, so those two
# are bundled; everything else it needs is already present on the target too.
collect_libs() {
  local binary="$1" dest="$2"
  ldd "$binary" | awk '{print $3}' | grep -Ei '/libz3\.so|/libhiredis\.so' \
    | xargs -r cp -t "$dest"
}
collect_libs "$STAGE/bin/alive-tv" "$STAGE/lib/alive"

patchelf --set-rpath '$ORIGIN/../lib/alive' "$STAGE/bin/alive-tv"

{
  echo "built: $(date -u +%Y-%m-%dT%H:%M:%SZ)"
  echo "llvm-project: $(git -C "$HOME/llvm-project" rev-parse HEAD)"
  echo "alive2:       $(git -C "$HOME/alive2" rev-parse HEAD)"
} > "$STAGE/VERSION.txt"

ARCHIVE="$(mktemp -d)/$ASSET_NAME"
tar czf "$ARCHIVE" -C "$STAGE" bin lib VERSION.txt
sha256sum "$ARCHIVE" | awk '{print $1}' > "$ARCHIVE.sha256"

if gh release view "$TAG" --repo "$REPO" >/dev/null 2>&1; then
  gh release upload "$TAG" "$ARCHIVE" "$ARCHIVE.sha256" --repo "$REPO" --clobber
else
  gh release create "$TAG" "$ARCHIVE" "$ARCHIVE.sha256" \
    --repo "$REPO" \
    --title "Slate CIR toolchain (Linux x86_64)" \
    --notes "clang/cir-opt/alive-tv for cloud environment setup scripts. See VERSION.txt inside the archive for provenance."
fi

echo "Published: https://github.com/$REPO/releases/download/$TAG/$ASSET_NAME"
