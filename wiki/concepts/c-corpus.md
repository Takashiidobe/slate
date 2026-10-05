# C corpus

<!-- toc -->
- [Layout](#layout)
- [Setup](#setup)
- [Recipes](#recipes)
- [Sweep](#sweep)
- [End to end](#end-to-end)
  - [CMake libraries](#cmake-libraries)
  - [Make libraries](#make-libraries)
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
| cJSON, libexpat (`expat/`), libuv, libyaml, mimalloc, pcre2, yyjson, zlib           | CMake                                                                                                                                   | CMake                                                |
| utf8proc                                                                            | CMake, `UTF8PROC_ENABLE_TESTING=ON` (downloads the Unicode test data)                                                                   | CMake                                                |
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
| oniguruma                                                                           | CMake                                                                                                                                   | CMake, untested                                      |
| c-ares                                                                              | CMake, `CARES_BUILD_TESTS=ON` (needs gtest)                                                                                             | CMake, untested                                      |
| cglm                                                                                | CMake, `CGLM_USE_TEST=ON`                                                                                                               | CMake, untested                                      |
| libevent                                                                            | CMake, OpenSSL and Mbed TLS off, `EVENT__LIBRARY_TYPE=STATIC`                                                                           | CMake, untested                                      |
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
  gone (`bear` also records configure probes). gcc and clang get the full
  compile command, minus the compiler and the source file; relative paths
  resolve because both sides run in the entry's directory. The oracle also
  drops dependency output (`-M*`, `-MF`/`-MT`/`-MQ`/`-MJ`, `-Wp,-M…`), which
  would rewrite the build's `.d` files. Pins `-std=gnu17` when absent
  (slate-parser-6x05.6).
- msvc databases use `/` spellings, so they still go through
  `pp_diff.kept_args`: `-D`, `-U`, the include flags, `-include`,
  `-imacros`, `-std`, target `-m` flags and `pp_diff.SEMANTIC_FLAGS`.
  `pp_diff.py` and `tools/corpus/e2e.py` also still use `kept_args`.
- Every flag a real build passes needs handling in `compiler_args`. Never
  skip TUs or drop a flag to make a project pass. `flag_probe.py`
  ([compiler-arg-rules](compiler-arg-rules.md#adding-an-option)) checks a
  rejected flag against the oracles.
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
python3 tools/corpus/e2e.py PROJECT [--mode clang|gcc] [--setup] [--until STAGE] [--bench-runs N] [-- runtest args]
```

Translates one project's target with `slate translate-project`, links it
against the native build's libraries, benchmarks it against the native
binary, and runs the project's tests. Reports go to
`target/corpus/<project>/<mode>/` (`summary.json` plus per-stage logs).
The run stops at the first failing stage. The barrier report is always
written once translation units are known.

| Stage     | What                                                                                                                                                                                                | Report                                            |
| --------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------- |
| native    | `--setup` reruns `c_corpus_setup.py`; checks the target's link inputs exist; picks the target's TUs (make object lists) from `build-<mode>`                                                         | `compile_commands.json`                           |
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
- lua: the target is `LUA_O` + `CORE_O` + `AUX_O` + `LIB_O` (34 TUs),
  linked with `-Wl,-E -ldl` so `testes/libs/*.so` resolve the API from the
  binary. The test is `../lua all.lua` in the sandbox's `testes/` (after
  `make -C testes/libs`), with stdin a closed pipe because `files.lua`
  expects seeking stdin to fail. `test_patches` drops the
  `sh -c 'kill -s HUP $$'` row of `files.lua`: it expects `exit`, but bash
  as `/bin/sh` execs the last command and reports `signal`, failing natively
  too. The benchmark times five `lua -e` scripts (fib, table, string, sort,
  closure). 2026-10-05: `final OK`, 4.7s vs 4.5s native; benchmark geomean
  1.56x native (fib 2.8x).

### CMake libraries

`CMakeLibrary` recipes (cJSON, libyaml, zlib) cover CMake/ninja projects
whose product is a library. Their optional `benchmark` runs after the test
stage, against the relinked tools in `test-tree/`.

- native: the library's TUs are the objects `ninja -t query <library>`
  lists (symlinks such as `libcjson.so` are resolved first), plus those of
  its `components` (libevent's `_extra` and `_pthreads` archives next to
  `_core`), matched to `compile_commands.json` entries by `output`.
- translate/build: `translate-project --crate-type staticlib` (also
  `cdylib` with `translate_tests`).
- test: every CTest executable plus the recipe's `tools`, rebuilt into
  `test-tree/`. By default each is relinked from its native link command
  (`ninja -t commands`) with the library, its components and its
  `variants` (other builds of it, such as zlib's `libz.so` next to
  `libz.a`) swapped for the translated archive and Rust's native libs
  appended. A ctest case passes when it exits 0 and its stdout matches the
  native run; it runs with its `ENVIRONMENT` property, and cases run in
  parallel (`--jobs`), each native then translated. The executable is
  `argv[0]`, or `argv[2]` behind a `bash`/`sh` wrapper script (libexpat's
  `run.sh`). Cases whose executable is not in the build tree (zlib's
  `cmake` packaging and `llvm-cov` cases) are listed as skipped. Each
  `Differential` then runs a tool over every input glob with the same
  `argv[0]` for both builds and must match the native exit code and
  stdout. An `{output}` argument is a fresh directory per run whose files
  are compared too (`xmlwf -d`).
- `translate_tests` (cJSON): its unity tests `#include "../cJSON.c"`, so
  relinking would only test native code. Instead each test executable
  (its objects plus in-build archives such as `libunity.a`) is translated
  as its own binary crate, linked against the translated `cdylib` in
  place of the native `.so`, which keeps the native rule that the test's
  own definitions win.
- cJSON (2026-10-05): 1 TU, 19/19 ctest. libyaml: 8 TUs, 3/3 ctest, and
  100/100 differential runs (`run-*`/`example-*` over `examples/*.yaml`
  and `regression-inputs/*`).
- zlib (2026-10-05): 15 TUs from `libz.a`, 5/5 ctest (examples, static
  examples, `infcover`), 10 skipped, and 210/210 byte-identical
  `minigzip -c` runs (default, `-1`, `-9`, `-h`, `-r`, `-f`). The benchmark
  pipes a 64 MiB input built from the sources and `zlib.3.pdf` through
  `minigzip`, `minigzip -9`, and `minigzip -d`, and requires identical
  output: geomean 1.04x native.
- `test_env` adds environment to every ctest case (c-ares sets
  `GTEST_PRINT_TIME=0`, since gtest's per-test timings differ run to
  run). `compare_stdout=False` judges cases by exit code alone, for
  suites whose native output is not reproducible (libevent prints
  timestamps, CPU usage and pointers).
- 2026-10-05: utf8proc 1 TU, 10/10 ctest. libexpat (`translate_tests`,
  since `runtests` compiles its own copy of `lib/*.c`): 7 TUs, 1/1 ctest,
  and 36/36 `xmlwf -d` runs (default, `-n`, `-m`, `-N -p`) over
  `testdata/largefiles/*.xml` and `doc/xmlwf.xml`. c-ares: 93 TUs from
  `libcares.so`, 3/3 ctest (`arestest`'s 1219 gtest cases, live DNS
  included, plus both fuzz corpora). libevent (static build): 28 TUs from
  `_core`, `_extra` and `_pthreads`, 84/84 ctest by exit code, 25 minutes
  of test time.

### Make libraries

`MakeLibrary` recipes (giflib, lmdb) cover in-tree Make builds whose
product is a static library and a set of tools.

- native: the TUs are the members of the recipe's `archives` (`ar t`),
  matched to the `bear` database by source stem. Tool names come from a
  make variable (`UTILS`, `PROGS`), expanded with a print goal passed via
  `--eval`, because `make -pn` prints values unexpanded
  (`OBJECTS = $(SOURCES:.c=.o)`). Make `Recipe`s read their object lists
  the same way.
- test: each tool is relinked from the last line of `make -Bn <tool>`
  whose `-o` names it, with the archives swapped for the translated
  archive, into `translated-tools/`. The test command then runs twice, in
  `test-tree/` with the translated tools linked in and in
  `test-tree-native/` with the native ones, and must exit 0.
- giflib (2026-10-05): 10 TUs from `libgif.a` and `libutil.a`, 14 tools;
  `make test` in `tests/` passes 51/51, with the same TAP output as native.
- lmdb (2026-10-05): 3 TUs from `liblmdb.a`, 10 tools (`PROGS`). Upstream's
  `make test` is only `mtest` + `mdb_stat`, so the recipe's script also
  round-trips `mtest`'s database through `mdb_dump`/`mdb_load`/`mdb_copy`
  (ignoring the `mapaddr=` header, which is address-dependent) and runs
  `mtest2`-`mtest5`. A dump with `mtest5`'s dupsort subdatabase does not
  round-trip natively either, so it is not compared.

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
