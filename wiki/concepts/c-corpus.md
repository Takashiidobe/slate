# C corpus

<!-- toc -->
- [Layout](#layout)
- [Setup](#setup)
- [Recipes](#recipes)
- [Sweep](#sweep)
- [End to end](#end-to-end)
- [Missing dependencies](#missing-dependencies)
- [Gotchas](#gotchas)
<!-- /toc -->

`~/c-corpus` (override with `SLATE_CORPUS`) holds real-world C projects as
git checkouts. The goal: every TU that `clang -fsyntax-only` accepts goes
through `slate-parser ir --flavor=clang` on `x86_64-unknown-linux-gnu`.

## Layout

```
~/c-corpus/<project>/build-<flavor>/compile_commands.json   flavor: clang, gcc, msvc
~/c-corpus/<project>/build-<flavor>.log                     setup log
~/c-corpus/.venv                                            python for code generators
```

- Each flavor's database comes from a build with that compiler, so
  configure results (`config.h`, feature probes) match it.
- The sweep reads the selected flavor's build database and checks acceptance
  with the corresponding compiler.
- Older `compile_commands.json` files at a project's top level or in
  `build/` came from `cc` (gcc) and are ignored.

## Setup

```
python3 tools/c_corpus_setup.py [PROJECT ...] [--flavor clang|gcc|msvc ...]
```

- Recreates `build-<flavor>/` and runs the whole build, so generated
  sources and headers exist (mbedtls test suites, curl's
  `tool_hugehelp.c`, sqlite's `parse.c`). "database written, build
  incomplete" means the build failed after the database was written.
- CMake projects build out of tree with Ninja and
  `CMAKE_EXPORT_COMPILE_COMMANDS`. Autoconf-style projects build out of
  tree under `bear`. Makefile-only projects build in tree under `bear`
  after their clean target. Flavors run gcc, msvc, clang, so an in-tree
  project ends with clang's generated files.
- msvc: msvc-wine's `bin/x64` on `PATH` ([msvc-oracle](msvc-oracle.md)),
  CMake with `CMAKE_SYSTEM_NAME=Windows`, or `nmake` for sqlite (its
  database is parsed from the `nmake` log).
- A full setup takes about 30 minutes.

## Recipes

| Project                                                                             | Build                                                                                                                                   | msvc                                                 |
| ----------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------- |
| cJSON, libexpat (`expat/`), libuv, libyaml, mimalloc, pcre2, utf8proc, yyjson, zlib | CMake                                                                                                                                   | CMake                                                |
| libdeflate                                                                          | CMake, `LIBDEFLATE_BUILD_TESTS=ON`                                                                                                      | CMake                                                |
| xxHash                                                                              | CMake in `build/cmake`, `DISPATCH=ON` (adds `xxh_x86dispatch.c`)                                                                        | CMake                                                |
| lz4, zstd                                                                           | CMake in `build/cmake`                                                                                                                  | CMake                                                |
| curl                                                                                | CMake                                                                                                                                   | CMake with Schannel, no optional dependencies        |
| libpng                                                                              | CMake against the corpus zlib (`zlib/build-<flavor>`)                                                                                   | same                                                 |
| mbedtls                                                                             | CMake, testing and programs on; generators run from `.venv`                                                                             | same                                                 |
| sqlite                                                                              | `configure` + `make all testfixture`                                                                                                    | `nmake /f ..\Makefile.msc TOP=.. USE_AMALGAMATION=0` |
| musl, tinycc, cpython                                                               | `configure` out of tree                                                                                                                 | none                                                 |
| libsodium                                                                           | `autogen.sh -s` in tree, then `configure` out of tree + `make check`                                                                    | none                                                 |
| nginx                                                                               | `auto/configure --builddir=build-<flavor>/objs`                                                                                         | none                                                 |
| redis, lua, quickjs, chibicc, giflib                                                | `make CC=` in tree                                                                                                                      | none                                                 |
| oniguruma, c-ares                                                                   | CMake                                                                                                                                   | CMake, untested                                      |
| cglm                                                                                | CMake, `CGLM_USE_TEST=ON`                                                                                                               | CMake, untested                                      |
| libevent                                                                            | CMake, OpenSSL and Mbed TLS off                                                                                                         | CMake, untested                                      |
| jq                                                                                  | `autoreconf -i` in tree, then `configure --with-oniguruma=builtin --disable-docs` out of tree                                           | none                                                 |
| lmdb                                                                                | `make -C libraries/liblmdb CC=` in tree                                                                                                 | none                                                 |
| stb                                                                                 | `make -i -C tests CC=` in tree: upstream's driver TUs define each header's `*_IMPLEMENTATION`; `-i` gets past the C++ TU's link failure | none                                                 |
| postgres                                                                            | `configure` out of tree + `make world-bin` (contrib included)                                                                           | none                                                 |
| linux                                                                               | shallow clone; `make O=build-<flavor> CC= HOSTCC= defconfig`, then the full build (about 4 minutes)                                     | none                                                 |

msvc covers only projects whose upstream ships a Windows build. musl,
nginx, redis, lua, quickjs, chibicc, giflib and tinycc have no MSVC build
system to drive, so there is no msvc database for them rather than an
invented one.
CPython ships an MSBuild project (`PCbuild/`) that setup does not drive
yet.

## Sweep

```
python3 tools/c_corpus_sweep.py [PROJECT ...] [--flavor clang|gcc|msvc] [--jobs N] [--trophies]
```

- Deduplicates each database by file and drops entries whose file is
  gone (`bear` also records configure probes). Keeps `-D`, `-U`, `-I`,
  `-isystem`, `-iquote`, `-idirafter`, `-include`, `-imacros`, `-std`,
  target flags (`-march=` and valueless `-m`/`-mno-` flags, including
  `-m16`/`-m32`/`-m64`/`-mx32`), and flags that change what the C means
  (`pp_diff.SEMANTIC_FLAGS`: `-nostdinc`, `-ffreestanding`, `-fno-builtin*`,
  char signedness, `-fshort-wchar`, `-fms-anonymous-structs`,
  `-fstrict-flex-arrays=`, `-fexperimental-late-parse-attributes`).
  `-mllvm`/`-Xclang` are dropped with their value. Pins `-std=gnu17` when
  absent (slate-parser-6x05.6).
- The filtering is a stopgap for finding unhandled flags, not a policy:
  every flag a real build passes needs handling in `compiler_args`, and
  the sweep should eventually pass the full command (slate-parser-6x05.38.4).
  Never skip TUs or drop a flag to make a project pass.
- `--flavor` selects `build-<flavor>`, the Slate flavor, and the compiler
  oracle; it defaults to clang. MSVC uses `cl.exe /Zs` and the Windows x64
  target.
- Clang and GCC use `-fsyntax-only -w` against host headers. Slate uses its
  own sysroot.
- Status per TU: `ok`, `internal`, `unimplemented`, `rejected` (from the
  first detailed `×` diagnostic), `timeout` (300 s), `missing-dependency`,
  `oracle-rejects`.
- Writes `target/c-corpus-sweep.md` (per-project table, failure clusters,
  slowest TUs) and `target/c-corpus-sweep.json`.
- `--trophies` prints the README trophy-case table: projects where every
  TU is `ok`. An `oracle-rejects` disqualifies, since it means the setup
  is wrong, not slate.
- A full run takes about 3 minutes.

## End to end

```
python3 tools/corpus/e2e.py redis [--mode clang|gcc] [--setup] [--until STAGE] [--bench-runs N] [-- runtest args]
```

Translates one project's target with `slate translate-project`, links it
against the native build's libraries, benchmarks it against the native
binary, and runs the project's tests. Reports go to
`target/corpus/<project>/<mode>/` (`summary.json` plus per-stage logs).
The run stops at the first failing stage. The barrier report is always
written once translation units are known.

| Stage     | What                                                                                                                                                                                                | Report                                            |
| --------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------- |
| native    | `--setup` reruns `c_corpus_setup.py`; checks the target's link inputs exist; picks the target's TUs (`make -pn` object lists) from `build-<mode>`                                                   | `compile_commands.json`                           |
| barriers  | `slate lowering-barriers` per TU, aggregated by kind                                                                                                                                                | `barriers.{json,md}`                              |
| translate | `translate-project`; also times the native compile of the same TUs with their original flags, in parallel                                                                                           | `translate_over_compile`                          |
| check     | `cargo check` of the generated crate                                                                                                                                                                | `check.log`                                       |
| build     | `cargo build --release`, linking the native archives and libraries via `-C link-arg`                                                                                                                | `build.log`                                       |
| bench     | the project's benchmark against the native and translated binaries: one warmup each, then alternating samples, medians                                                                              | `bench.{json,md}`, `geomean_ratio`                |
| test      | the project's test command in a sandbox copy of its test tree, with the target binary swapped in; then the same command against the native binary in a second sandbox (`--no-native-test` skips it) | `test.log`, `test-native.log`, `test_over_native` |

- Translation should take about as long as the native compile, the
  translated binary should benchmark about as fast as the native one, and
  the test suite should take about as long against either binary.
  `cargo build` time is not compared.
- redis: the target is `REDIS_SERVER_OBJ` + `REDIS_VEC_SETS_OBJ`. Its
  `build-clang` database records the `src/*.c` TUs only as `-MM` entries; the
  harness keeps the first existing source per object, so
  `../modules/vector-sets/hnsw.c` wins over the missing `src/hnsw.c`. The
  benchmark is pipelined `redis-benchmark` (`-P 16 -c 50`). The test sandbox
  links `redis-server`/`redis-check-*` to the translated binary and
  `redis-cli`/`redis-benchmark` to the native ones.

## Missing dependencies

Third-party headers absent from the slate sysroot (`crypt.h`, `bzlib.h`,
`readline/readline.h`) are reported as `missing-dependency`, not as slate
failures. The sysroot holds only the C library and compiler headers and
gets no more: a project that needs a host library is given its include
directory explicitly, as a user would pass `-idirafter` to `slate-parser`
or `slate translate-project`.

- `HOST_INCLUDE_DIRS` in the sweep is that manual list, appended as
  `-idirafter` so the sysroot still wins for libc headers. redis needs
  `/usr/include` for `systemd/sd-daemon.h` (its build detects systemd on
  the host). pcre2 needs it for `bzlib.h` (`pcre2grep.c`) and
  `readline/readline.h` (`pcre2test.c`), nginx for `crypt.h`, linux for
  its host tools (`gelf.h`, `openssl/bio.h`), cpython
  for its optional modules' libraries (ssl, bz2, lzma, readline, ncurses,
  ffi).
- Where a corpus project provides the header (`zlib`), the sweep appends
  its source and build directories as `-idirafter` for every other
  project, and libpng is built against that zlib so clang and slate read
  the same header.

## Gotchas

- clang demotes a directory given as both `-I` and `-idirafter` to a
  system directory, which broke zlib's own TUs; the provider directories
  are never added to the provider itself.
- msvc: don't export `CC=cl`. mbedtls's generators compile and run probes
  with `$CC`, and a Windows executable can't run on the host. CMake gets
  `cl` through `CMAKE_C_COMPILER`. Setting `CMAKE_CXX_COMPILER` breaks
  zstd, whose flag checks test C++ when a C++ compiler is named.
- sqlite under `nmake`: `/I`, not `/K`, since Wine's `del` of a missing
  file fails; a relative `TOP` keeps command lines under `nmake`'s length
  limit.
- mbedtls's generators need `jsonschema` and `jinja2`; setup installs its
  `basic.requirements.txt` into `~/c-corpus/.venv`.
- giflib: `MAKE=true` skips the docs step (`xmlto`).
