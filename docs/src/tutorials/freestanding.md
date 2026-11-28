# Translate a freestanding library

Build a `no_std` Rust archive from a C function that sums bytes. This example
uses the host target so you can focus on the freestanding build settings. You
need Slate, Rust nightly, Clang, CMake, and Ninja.

## Create the C project

```sh
mkdir byte-sum
cat > byte-sum/byte_sum.c <<'EOF'
#include <stddef.h>

unsigned long sum_bytes(const unsigned char *bytes, size_t len) {
    unsigned long sum = 0;
    for (size_t i = 0; i < len; ++i)
        sum += bytes[i];
    return sum;
}
EOF

cat > byte-sum/CMakeLists.txt <<'EOF'
cmake_minimum_required(VERSION 3.15)
project(byte_sum C)
add_library(byte_sum STATIC byte_sum.c)
target_compile_options(byte_sum PRIVATE -ffreestanding -nostdlibinc)
EOF
```

`-ffreestanding` tells Slate that the C code runs without a hosted runtime. Keep
that flag in every translation unit's compile command to generate a `#![no_std]`
crate. `-nostdlibinc` leaves Clang's resource headers available while excluding
the standard C library include directories. The example needs Clang's `stddef.h`
for `size_t`.

## Check the compiler headers and translate

```sh
slate sysroot doctor native clang
cmake -S byte-sum -B byte-sum/build -G Ninja -DCMAKE_C_COMPILER=clang \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
slate translate-project --compile-commands byte-sum/build/compile_commands.json \
  --build-target byte_sum --crate-type staticlib byte-sum byte-sum-rs
cargo +nightly build --manifest-path byte-sum-rs/Cargo.toml \
  --target-dir byte-sum-rs/target
```

The native Clang resource headers supply `stddef.h`. If you use installed
headers, get the Clang bundle with
`slate sysroot install compiler-headers clang`. For a real firmware build,
retain the original target options and include paths so Slate sees the correct
ABI and project or SDK headers. See
[Sysroots and Headers](../reference/sysroots.md).

The archive is `byte-sum-rs/target/debug/libbyte_sum_rs.a`. Use it in the
original target's link step. When integrating it into firmware or a kernel, keep
the original startup code, linker script, and any runtime functions the
translated library calls.
