# Translate zlib

Translate zlib into a Rust static library and use it from a C program. These
commands use a Linux host with Slate, Rust nightly, Git, Clang, CMake, and
Ninja.

## Configure the C project

Run these commands from a directory where you want the zlib checkout and the
generated crate.

```sh
git clone --branch v1.3.1 --depth 1 https://github.com/madler/zlib.git zlib
cmake -S zlib -B zlib/build-clang -G Ninja -DCMAKE_C_COMPILER=clang \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

CMake writes `zconf.h` and `compile_commands.json` into `zlib/build-clang`.
Slate reads the database to get the compiler, working directory, include paths,
and defines for each C file. Keep the build directory in place so Slate can read
the generated header. In projects that generate headers or sources at build
time, run those build steps before translating.

## Check headers and translate

```sh
slate sysroot doctor native clang
slate translate-project \
  --compile-commands zlib/build-clang/compile_commands.json \
  --build-target zlibstatic --crate-type staticlib zlib zlib-rs
```

Slate can use the native Clang and system headers for this host build. If the
check fails, install Clang and your host C development headers or use an
installed bundle. See [Sysroots and Headers](../reference/sysroots.md).

`--build-target zlibstatic` selects the static-library entries from the C
database. The database also contains entries for the shared library and example
programs. `--crate-type staticlib` asks Cargo to produce a C-compatible archive.
The two positional paths identify the C project root and the generated crate.

## Build and use the library

```sh
cargo +nightly build --manifest-path zlib-rs/Cargo.toml --target-dir zlib-rs/target
clang -I zlib/build-clang -I zlib zlib/test/example.c \
  zlib-rs/target/debug/libzlib_rs.a -o zlib-example
./zlib-example
```

The C example calls zlib through the original headers. Passing the generated
archive directly makes the example use the translated library. A successful run
prints lines including `uncompress(): hello, hello!` and
`inflate(): hello, hello!` and exits with status zero.

Continue with [Translate zlib for two platforms](cross-platform.md).
