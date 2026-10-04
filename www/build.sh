#!/usr/bin/env bash
set -euo pipefail

root=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
slate_root=${SLATE_ROOT:-"$root/../"}
slate_data=${SLATE_DATA:-"$HOME/.local/share/slate"}
out=${1:-"$root/dist"}
sysroot_targets=(x86_64-unknown-linux-gnu i686-unknown-linux-gnu armv7-unknown-linux-gnueabihf aarch64-unknown-linux-gnu)
# msvc headers can't be redistributed, so windows targets ship without a sysroot
headerless_targets=(x86_64-pc-windows-msvc i686-pc-windows-msvc aarch64-pc-windows-msvc thumbv7a-pc-windows-msvc)
compilers=(clang gcc)

die() {
    printf 'build.sh: %s\n' "$*" >&2
    exit 1
}

latest_profile() {
    find "$slate_data/compiler-headers" -mindepth 1 -maxdepth 1 -type d -name "$1-*" -printf '%f\n' | sort -V | tail -n 1
}

for compiler in "${compilers[@]}"; do
    [[ -n $(latest_profile "$compiler") ]] || die "no $compiler headers in $slate_data/compiler-headers; run: slate sysroot install"
done
for target in "${sysroot_targets[@]}"; do
    [[ -d $slate_data/sysroots/$target ]] || die "missing sysroot $target; run: slate sysroot install $target"
done

CARGO_PROFILE_RELEASE_LTO=true \
CARGO_PROFILE_RELEASE_CODEGEN_UNITS=1 \
CARGO_PROFILE_RELEASE_OPT_LEVEL=s \
CARGO_PROFILE_RELEASE_STRIP=true \
    cargo build --release --target wasm32-wasip1 --no-default-features -p slate \
    --manifest-path "$slate_root/Cargo.toml" --target-dir "$root/target"

rm -rf "$out"
mkdir -p "$out/headers"
cp -r "$root/frontend/." "$out/"

hashed() {
    local path=$1 stem=$2 ext=$3 hash
    hash=$(sha256sum "$path" | cut -c1-12)
    mv "$path" "$out/$stem.$hash.$ext"
    printf '%s\n' "$stem.$hash.$ext"
}

pack() {
    local dir=$1 name=$2 select=${3:-.}
    (cd "$dir" && find "$select" \( -path '*/include/*' -o -name 'COPYING*' -o -name 'LICENSE*' \) \( -type f -o -type l \) -print0 \
        | LC_ALL=C sort -z \
        | tar --null -T - --dereference --format=ustar --owner=0 --group=0 --numeric-owner --mtime=@0 -cf -) \
        | gzip -9n > "$out/headers/$name.tar.gz"
    local bytes path
    bytes=$(stat -c %s "$out/headers/$name.tar.gz")
    path=$(hashed "$out/headers/$name.tar.gz" "headers/$name" tar.gz)
    printf '{ "path": "%s", "bytes": %s }\n' "$path" "$bytes"
}

join_lines() {
    local IFS=$'\n'
    printf '%s\n' "$*" | sed '$!s/$/,/'
}

worker=$(hashed "$out/slate-worker.js" slate-worker js)
grep -q "'slate-worker.js'" "$out/index.html" || die "index.html no longer references slate-worker.js"
sed -i "s|'slate-worker.js'|'$worker'|" "$out/index.html"

cp "$root/target/wasm32-wasip1/release/slate.wasm" "$out/slate.wasm"
wasm=$(hashed "$out/slate.wasm" slate wasm)

compiler_entries=()
for compiler in "${compilers[@]}"; do
    profile=$(latest_profile "$compiler")
    compiler_entries+=("    \"$compiler\": $(pack "$slate_data/compiler-headers" "$profile" "$profile")")
done

target_entries=()
for target in "${sysroot_targets[@]}"; do
    target_entries+=("    \"$target\": $(pack "$slate_data/sysroots/$target" "$target")")
done
for target in "${headerless_targets[@]}"; do
    target_entries+=("    \"$target\": null")
done

cat > "$out/manifest.json" <<EOF
{
  "wasm": "$wasm",
  "compilerHeaders": {
$(join_lines "${compiler_entries[@]}")
  },
  "targets": {
$(join_lines "${target_entries[@]}")
  }
}
EOF

du -ah "$out/$wasm" "$out/headers"/* | sort -k2
