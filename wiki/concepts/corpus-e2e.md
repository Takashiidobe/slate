# Adding corpus tests

<!-- toc -->
- [What e2e.py does](#what-e2epy-does)
- [The comparison rule](#the-comparison-rule)
- [Flags](#flags)
- [Picking a recipe type](#picking-a-recipe-type)
  - [Adding a project with an existing
    type](#adding-a-project-with-an-existing-type)
  - [Adding a recipe type](#adding-a-recipe-type)
- [Linux kernel](#linux-kernel)
<!-- /toc -->

How to add a project, or more tests for an existing one, to
`tools/corpus/e2e.py`. [C corpus](c-corpus.md) describes setup and the existing
per-project recipes and results. Read this page first, then the recipe section
there that matches the kind of project you are adding.

## What e2e.py does

```bash
python3 tools/corpus/e2e.py PROJECT [--mode clang|gcc|clang-aarch64] [--setup] [--until STAGE] [-- TEST_ARGS]
```

A project is a key of `RECIPES` in `e2e.py`. Its sources are
`$SLATE_CORPUS/<project>` (default `~/c-corpus/<project>`), and its native build
and compile database are `build-<mode>/compile_commands.json` there, written by
`crates/slate-parser/tools/c_corpus_setup.py` (`--setup` reruns it).

Stages run in order and stop at the first failure or at `--until`:

| Stage     | Does                                                                                         |
| --------- | -------------------------------------------------------------------------------------------- |
| native    | checks the native build's outputs exist, picks the TUs, writes `<out>/compile_commands.json` |
| barriers  | `slate lowering-barriers` per TU, `barriers.md`                                              |
| translate | `slate translate-project` into one crate, and times the native compile of the same TUs       |
| build     | `cargo build`, linking native libraries                                                      |
| link      | linux only: swap translated objects into vmlinux (`kernel_link.py`)                          |
| boot      | linux only: boot native and translated bzImages under QEMU, diff dmesg (`kernel_boot.py`)    |
| kselftest | linux only: run userspace selftests on both kernels, diff TAP results (`kernel_selftest.py`) |
| bench     | the recipe's `benchmark`, native vs translated                                               |
| test      | run the tests against the translated build and the native build and compare                  |

Output goes to `target/corpus/<project>/<mode>/`, unless `CANONICAL_OUT` maps
the project to a fixed directory (linux clang:
`~/slate-translated/slate-linux`, cpython clang:
`~/slate-translated/slate-python`). `summary.json` holds every stage's status and
details; each stage writes its own log or report next to it.

## The comparison rule

A test passes when the translated build behaves like the native build of the
same sources with the same flags. Every recipe type runs its test both ways
(`--no-native-test` skips the native run only where the recipe allows it):

- Same argv, same inputs, same working-directory layout. Tools are run through a
  sandbox (`test_sandbox`) or a relinked copy so `argv[0]` matches.
- Compare exit status, and stdout unless it is not reproducible natively.
- Normalize, don't skip. Strip what varies run to run natively (timestamps,
  timings, addresses, thread interleaving) and compare the rest. Use the
  existing knobs before writing code: `test_env` (c-ares sets
  `GTEST_PRINT_TIME=0`), `compare_stdout=False` (exit code only), a `suite`
  callback that compares chosen lines (`oniguruma_suite` compares
  `RESULT SUCC/FAIL`).
- A test that fails natively is not a slate failure. Patch it out with
  `test_patches` (lua's `files.lua`) and say why in c-corpus.md.

## Flags

Every flag in the harness's compile commands needs a slate policy
([Compiler flags](compiler-flags.md),
[Compiler argument rules](compiler-arg-rules.md)). Never drop a flag or a TU to
get a project through. After the translate stage, grep `translate.log` for
`unsupported compiler flag`: each hit is a missing policy and gets a bead under
`slate-3amj` before the project is counted as passing. If a flag changes macros
or `__has_feature` (for example `-fsanitize` sets `__SANITIZE_ADDRESS__`), slate
has to reproduce that. Otherwise slate parses it and ignores it.

## Picking a recipe type

| Type              | Use for                                                          | TUs come from                                                     | Test                                                                                                               |
| ----------------- | ---------------------------------------------------------------- | ----------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------ |
| `Recipe`          | one executable built by make (redis, lua, jq, nginx)             | make variables named in `objects`, expanded with `make_variables` | `test` command in a sandbox with the binary linked in (`links`), then against native                               |
| `CMakeLibrary`    | CMake/Ninja library (zlib, curl, zstd)                           | `ninja -t query <library>` plus `components`                      | ctest cases and `tools` relinked against the translated archive; `differential` runs; `suite`                      |
| `MakeLibrary`     | in-tree make building `.a` archives plus tools (giflib)          | `ar t` of `archives`                                              | tools relinked from `make -Bn <tool>` with the archive swapped (`relink_make_tools`); `test` run in both sandboxes |
| `TranslationOnly` | no runnable comparison yet, or a custom stage (linux, numpy)     | every C unit in the database (`required_args`, `object_prefixes`) | `extension`/`smoke` hooks; `shard_units` and `relink_vmlinux` give the kernel pipeline                             |
| `MsvcLibrary`     | Windows library under msvc-wine (`tools/corpus/msvc_library.py`) | recipe source globs                                               | drivers in `tools/corpus/msvc/` linked against native and translated `.lib`, run under wine                        |

Each dataclass at the top of `e2e.py` (`MsvcLibrary`: `msvc_library.py`) lists
its fields with defaults; copy the closest existing entry in `RECIPES` and
change it.

### Adding a project with an existing type

1. Add the project to `c_corpus_setup.py`'s `RECIPES` so `--setup` builds
   `build-<mode>/` and writes the compile database (CMake exports it; make
   builds run under `bear`).
2. Add the `e2e.py` recipe.
3. Run `e2e.py PROJECT --until barriers`, then the full run. If something fails,
   file beads per failure cluster, not one per TU.
4. Add a bullet to the matching section of [C corpus](c-corpus.md): TU count,
   what the test covers, anything patched or not compared and why, and the dated
   result.
5. `llog new` with `-k corpus -v <project>`.

### Adding a recipe type

Add a new dataclass only when no type above can select the TUs or run the test.
The parts of `main()` to extend:

- the unit-selection branch (`library` / `translation_only` / `every_unit` /
  make): pick TUs and set `missing` for absent native outputs;
- crate type for `translate-project` (`staticlib`, `cdylib`, or a bin crate);
- the test dispatch at the end of `main()`.

Helpers to reuse rather than rewrite:

| Helper                                  | Does                                                                    |
| --------------------------------------- | ----------------------------------------------------------------------- |
| `make_variables`                        | expands make variables via an `--eval` print goal                       |
| `make_link_command`                     | the link command make would run for one output (`make -Bn`)             |
| `relink_make_tools`, `relink`           | rerun a native link command with objects/archives swapped for the crate |
| `units_for_objects`, `translation_unit` | map object files to database entries; build slate's argument list       |
| `translate_executable`                  | translate one executable's objects as its own bin crate                 |
| `test_sandbox`                          | copy `test_copy`, apply `test_patches`, link `links`, run `test_build`  |
| `run_case`, `run_differential`          | run one case with timeout and capture exit/stdout                       |
| `barrier_report`                        | the barriers stage for any unit list                                    |

One crate holds one `main`. When several test executables share objects but each
defines `main`, either translate each executable as its own crate
(`translate_executable`, `translate_tests`) or translate the shared objects as a
staticlib and relink each executable from its native link command.

## Linux kernel

`e2e.py linux` is `TranslationOnly(shard_units=120, relink_vmlinux=True)`:

- native: units are the database entries with `-D__KERNEL__` (host tools
  excluded), from `~/c-corpus/linux/build-clang/compile_commands.json`.
- translate/build: units are split into ~69 shards by kbuild directory
  (`shard_units`), each its own crate under `<out>/shards/<name>/`, because one
  crate over all 3005 TUs runs out of memory. `--shards a,b` retranslates only
  those shards.
- link: `kernel_link.py` swaps the shard archives into a copy of the build
  (`build-slate/`) and links vmlinux with the kernel's own scripts.
- boot: `kernel_boot.py` boots native and translated bzImages under QEMU with a
  marker initramfs and diffs normalized dmesg (`boot.md`).
- kselftest: `kernel_selftest.py` runs `tools/testing/selftests` in a busybox
  initramfs on both kernels and compares per-test TAP results
  ([Kernel selftests](kernel-selftests.md)).
- `--from translate|build|link|boot|kselftest` skips earlier stages and reuses
  their output.

Kernel tests that do not need a booted kernel go in as separate hosted projects
rather than as stages of `linux`, so they never wait on the sharded pipeline.
The candidates are `tools/testing/{radix-tree,rbtree,scatterlist,memblock,vma}`
(slate-f2zu.14.35). They build real `lib/` and `mm/` sources as Linux programs:

- in-tree make with no `O=`; some sources are generated by sed (`radix-tree.c`,
  `idr.c`, `scatterlist.c`), and others are shims that `#include` `lib/*.c` or
  `mm/*.c`;
- every harness compiles with `-fsanitize=address -fsanitize=undefined`;
  radix-tree and vma link `-lurcu -lpthread`;
- one directory builds several executables, each with its own `main`, that share
  objects (radix-tree: `main`, `idr-test`, `multiorder`, `xarray`, `maple`);
- rbtree prints cycle timings and radix-tree runs threads, so their output needs
  normalizing before comparison.
