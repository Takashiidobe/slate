# libc-test functional harness: admission and companion-file vendoring

`tools/filter-libc-test.sh` decides which of upstream libc-test's
`src/functional/*.c` cases are admitted into slate's vendored corpus, by
actually compiling and running each candidate natively first — a case is
only vendored if it behaves identically (same exit behavior) outside slate.
This is stricter than "compiles for slate": a case whose native behavior is
already broken/undefined (e.g. `dlopen()` failing identically on both sides
because no companion library was ever vendored) is only accidentally
"admitted," not really exercising the feature.

## Companion `.so` support

Some functional cases need a real shared library at runtime, declared via
their `.mk` (`$(N).BINS` empty, `$(N).LIBS` set) and a sibling `*_dso.c`
source. The vendoring script now detects this convention, builds the
companion as a real `.so`, and stages it at the path the fixture's `dlopen()`
call hard-codes (`src/functional/<dso>.so`) during the native admission
probe — then copies the built `.so` into the vendored output tree under its
own name. The output name is the DSO's own basename (e.g.
`tls_init_dso.so`), **not** the `_dlopen`-suffixed test name — it has to
match the literal path string the fixture's `dlopen()` call requests, not
the test's own name.

Getting this right flips cases like `tls_init_dlopen` from
`run-crashed:139` to admitted — the crash was an upstream test bug
(unconditional use of a symbol after a failed `dlsym`), which only manifests
when `dlopen()` was failing in the first place; fixing dlopen sidesteps it.

## Wiring on the test-runner side

`tests/support/mod.rs`'s `RunConfig` gained `extra_files: Vec<PathBuf>`,
staged into `<run_dir>/src/functional/<basename>` before every case run —
both the C and Rust sides share the same `run_dir`, so one staging step
covers both. `libc_test_functional_suite.rs` derives which companion to
stage **by naming convention**: for a `<stem>_dlopen` fixture, look for a
sibling `<stem>_dso.so`. There's no `.mk` available at test time to consult
directly, so this convention is deliberately duplicated between the shell
vendoring script (`tools/filter-libc-test.sh`) and the Rust suite — keep
both in sync if the naming convention ever changes.

## Known gap

`functional/dlopen.c` itself needs `t_pathrel` (an argv0-relative path
helper from libc-test's `common/`) that isn't vendored — a separate, larger
lift than a companion `.so`, left out of scope.
