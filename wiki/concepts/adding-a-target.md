# Adding a target

Every supported triple is one `TargetSpec` in `src/target_registry.rs`.
Nothing else matches on the triple string.

## TargetSpec fields

| Field | Meaning |
| --- | --- |
| `layout` | `TargetInfo` constructor in `src/target_info.rs`: scalar storage, widths, `long double` format, family/OS/environment, ISA baseline. `for_triple` fills `triple` and `profile`. |
| `profile.predefines` | One `Predefines` per flavor group. A missing flavor is rejected at argument parsing. On Linux the gcc flavor gets gcc's `-dM` snapshot; clang and msvc get clang's. |
| `profile.sysroot` | `WindowsKits` or `Unix { multiarch }`, probed by `sysroot::include_paths_at`. `multiarch` is the Debian cross triplet probed as `usr/<triplet>/include` (i686 is `i686-linux-gnu`, not `i386-linux-gnu`). |
| `profile.clang_headers` | `AppleFirst` prefers an `apple-clang-*` builtin header profile. |
| `profile.gcc_headers` | GCC header family (`x86`, `aarch64`, `arm`) for `gcc-*/<family>/include`; `None` without a gcc flavor. |
| `profile.va_list` | What `__builtin_va_list` is. |
| `profile.convention` | ABI convention. `Aapcs32` becomes `Aapcs32HardFloat` for non-variadic calls on a hard-float ISA; the only runtime adjustment. |

## Steps

1. Capture predefines into `src/predefines/` from the oracle
   (`clang --target=<triple> -dM -E -x c /dev/null`, the target's gcc,
   `tools/cl.exe`; see [generated-sources](generated-sources.md#predefines)). Delete the macros the ISA generator in `src/target/`
   emits (arch, FPU, float-ABI, CPU) and the GNU-namespace ones (`linux`,
   `unix`, `i386`). Check with `tools/gcc_macro_diff.py '<cc> <flags>'
   '--flavor=... -target=<triple> <flags>'`; only `__SLATE_*` may differ.
   The armv7 gcc oracle is `arm-none-linux-gnueabihf` (as
   `arm-linux-gnueabihf-gcc`), not bare-metal `arm-none-eabi-gcc`.
2. Add the layout constructor and the `TargetSpec`.
3. Add a fixture directory per flavor ([fixture-layout](fixture-layout.md)):
   `<flavor>/<os>/<arch>/` for the OS's usual arch (add it to
   `CANONICAL_TRIPLES` in `tests/filecheck.rs` and
   `tools/update_filecheck.py`), else `<flavor>/<os>/<triple>/`. A new OS
   also needs `OS_DIRECTORIES` in both. Copy `target_registry.c` into each
   new directory and regenerate it.
4. Update `tools/corpus_sweep.py`'s `sysroot_include_paths` if the sysroot
   layout differs.
