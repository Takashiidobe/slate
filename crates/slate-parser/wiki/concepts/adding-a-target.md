# Adding a target

Every supported triple is one `TargetSpec` in `src/target_registry.rs`. Nothing
else matches on the triple string, so a triple is either fully wired or not
accepted at all.

A spec holds:

- `layout`: a `TargetInfo` constructor in `src/target_info.rs` (scalar
  storage, widths, `long double` format, `TargetFamily`/`TargetOs`/
  `TargetEnvironment`, ISA baseline). `for_triple` fills in `triple` and
  `profile` from the spec, so constructors don't set them.
- `profile.predefines`: one `Predefines` per group of flavors. A flavor
  missing here is rejected at argument parsing. Linux entries currently give
  every flavor clang's predefines.
- `profile.sysroot`: `WindowsKits` or `Unix { multiarch }`, the directories
  `sysroot::include_paths_at` probes under the sysroot.
- `profile.clang_headers`: `AppleFirst` prefers an `apple-clang-*` builtin
  header profile over upstream clang's.
- `profile.va_list`: what `__builtin_va_list` is.
- `profile.convention`: the ABI convention. `Aapcs32` becomes
  `Aapcs32HardFloat` for non-variadic calls when the ISA is hard-float; that is
  the only runtime adjustment.

Steps:

1. Capture predefines from the oracle (`clang --target=<triple> -dM -E -x c
   /dev/null`, `tools/cl.exe` for the msvc flavor) into `src/predefines/`.
2. Add the layout constructor and the `TargetSpec`.
3. Add a `DEFINES`/`PREFIX-ARGS` pair per supported flavor to
   `tests/fixtures/sema/target_registry.c` and regenerate it. Fixtures under
   `tests/fixtures/sema/<triple>/` run with that target automatically.
4. `tools/corpus_sweep.py` mirrors the sysroot candidate lists in
   `sysroot_include_paths`; update it if the new layout differs.
