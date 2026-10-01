# C corpus

<!-- toc -->
- [Layout](#layout)
- [Setup](#setup)
- [Recipes](#recipes)
- [Sweep](#sweep)
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
- The sweep reads only `build-clang` for now. The gcc and msvc databases
  exist for later flavors.
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

| Project | Build | msvc |
| --- | --- | --- |
| cJSON, libexpat (`expat/`), libuv, libyaml, pcre2, utf8proc, yyjson, zlib | CMake | CMake |
| lz4, zstd | CMake in `build/cmake` | CMake |
| curl | CMake | CMake with Schannel, no optional dependencies |
| libpng | CMake against the corpus zlib (`zlib/build-<flavor>`) | same |
| mbedtls | CMake, testing and programs on; generators run from `.venv` | same |
| sqlite | `configure` + `make all testfixture` | `nmake /f ..\Makefile.msc TOP=.. USE_AMALGAMATION=0` |
| musl, tinycc | `configure` out of tree | none |
| nginx | `auto/configure --builddir=build-<flavor>/objs` | none |
| redis, lua, quickjs, chibicc, giflib | `make CC=` in tree | none |

msvc covers only projects whose upstream ships a Windows build. musl,
nginx, redis, lua, quickjs, chibicc, giflib and tinycc have no MSVC build
system to drive, so there is no msvc database for them rather than an
invented one.

## Sweep

```
python3 tools/c_corpus_sweep.py [PROJECT ...] [--jobs N] [--trophies]
```

- Deduplicates each database by file and drops entries whose file is
  gone (`bear` also records configure probes). Keeps `-D`, `-U`, `-I`,
  `-isystem`, `-iquote`, `-idirafter`, `-include`, `-imacros`, `-std`
  (`pp_diff.kept_args`); pins `-std=gnu17` when absent
  (slate-parser-6x05.6).
- Oracle: `clang <flags> -fsyntax-only -w` against host headers. Slate
  uses its own sysroot.
- Status per TU: `ok`, `internal`, `unimplemented`, `rejected` (from the
  first detailed `×` diagnostic), `timeout` (300 s), `missing-dependency`,
  `clang-rejects`.
- Writes `target/c-corpus-sweep.md` (per-project table, failure clusters,
  slowest TUs) and `target/c-corpus-sweep.json`.
- `--trophies` prints the README trophy-case table: projects where every
  TU is `ok`. A `clang-rejects` disqualifies, since it means the setup
  is wrong, not slate.
- A full run takes about 3 minutes.

## Missing dependencies

Third-party headers absent from the slate sysroot (`crypt.h`,
`systemd/sd-daemon.h`, `bzlib.h`, `readline/readline.h`) are reported as
`missing-dependency`, not as slate failures: slate-parser only sees its
sysroot, and adding host libraries there is out of scope. Where a corpus
project provides the header (`zlib`), the sweep appends its source and
build directories as `-idirafter` for every other project, and libpng is
built against that zlib so clang and slate read the same header.

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
