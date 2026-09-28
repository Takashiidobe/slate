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
  missing here is rejected at argument parsing. Linux entries give the gcc
  flavor gcc's own `-dM` snapshot and clang/msvc clang's.
- `profile.sysroot`: `WindowsKits` or `Unix { multiarch }`, the directories
  `sysroot::include_paths_at` probes under the sysroot. `multiarch` is the
  Debian cross-package triplet probed as `usr/<triplet>/include` (what
  slate-sysroots installs), not the multiarch `usr/include/<triplet>`: i686
  is `i686-linux-gnu` there, although Debian's multiarch name is
  `i386-linux-gnu`.
- `profile.clang_headers`: `AppleFirst` prefers an `apple-clang-*` builtin
  header profile over upstream clang's.
- `profile.gcc_headers`: the GCC header family (`x86`, `aarch64`, `arm`)
  whose `gcc-*/<family>/include` the gcc flavor searches; `None` when the
  target has no gcc flavor.
- `profile.va_list`: what `__builtin_va_list` is.
- `profile.convention`: the ABI convention. `Aapcs32` becomes
  `Aapcs32HardFloat` for non-variadic calls when the ISA is hard-float; that is
  the only runtime adjustment.

Steps:

1. Capture predefines from the oracle (`clang --target=<triple> -dM -E -x c
   /dev/null`, the target's gcc, `tools/cl.exe` for the msvc flavor) into
   `src/predefines/`. Delete every macro the ISA generator in `src/target/`
   emits (arch, FPU, float-ABI, CPU macros) and the GNU-namespace ones
   (`linux`, `unix`, `i386`), since those are regenerated per flag set.
   Check the result with `tools/gcc_macro_diff.py '<cc> <flags>' '--flavor=...
   -target=<triple> <flags>'`; only `__SLATE_*` should differ. The armv7 gcc
   oracle is Arm's `arm-none-linux-gnueabihf` toolchain (on PATH as
   `arm-linux-gnueabihf-gcc`); `arm-none-eabi-gcc` is bare-metal and has
   different integer typedefs.
2. Add the layout constructor and the `TargetSpec`.
3. Give the target a fixture directory per supported flavor (see
   `wiki/concepts/fixture-layout.md`): `tests/fixtures/<flavor>/<os>/<arch>/`
   if the arch is the OS's usual one, added to `CANONICAL_TRIPLES` in both
   `tests/filecheck.rs` and `tools/update_filecheck.py`, otherwise
   `tests/fixtures/<flavor>/<os>/<triple>/`. A new OS also needs an
   `OS_DIRECTORIES` entry in both. Copy `target_registry.c` from another leaf
   into each new directory and regenerate it.
4. `tools/corpus_sweep.py` mirrors the sysroot candidate lists in
   `sysroot_include_paths`; update it if the new layout differs.
