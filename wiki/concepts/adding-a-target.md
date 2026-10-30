# Adding a target

<!-- toc -->
- [TargetSpec fields](#targetspec-fields)
- [Steps](#steps)
- [Linux musl](#linux-musl)
- [Clang resource headers](#clang-resource-headers)
- [Sysroot installation locks](#sysroot-installation-locks)
- [macOS SDK registration](#macos-sdk-registration)
<!-- /toc -->

Every supported triple is one `TargetSpec` in `src/target_registry.rs`.
Nothing else matches on the triple string.

## TargetSpec fields

| Field                   | Meaning                                                                                                                                                                                                   |
| ----------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `layout`                | `TargetInfo` constructor in `src/target_info.rs`: scalar storage, widths, `long double` format, family/OS/environment, ISA baseline. `for_triple` fills `triple` and `profile`.                           |
| `profile.predefines`    | One `Predefines` per flavor group. A missing flavor is rejected at argument parsing. On Linux the gcc flavor gets gcc's `-dM` snapshot; clang and msvc get clang's.                                       |
| `profile.sysroot`       | `WindowsKits` or `Unix { multiarch }`, probed by `sysroot::include_paths_at`. `multiarch` is the Debian cross triplet probed as `usr/<triplet>/include` (i686 is `i686-linux-gnu`, not `i386-linux-gnu`). |
| `profile.clang_headers` | `AppleFirst` prefers `apple-clang-current/include`, then the newest `apple-clang-*` bundle, then upstream `clang-*`.                                                                                                                                           |
| `profile.gcc_headers`   | GCC header family (`x86`, `aarch64`, `arm`) for `gcc-*/<family>/include`; `None` without a gcc flavor.                                                                                                    |
| `profile.va_list`       | What `__builtin_va_list` is.                                                                                                                                                                              |
| `profile.convention`    | ABI convention. `Aapcs32` becomes `Aapcs32HardFloat` for non-variadic calls on a hard-float ISA; the only runtime adjustment.                                                                             |

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

## Linux musl

- `x86_64-unknown-linux-musl` and `aarch64-unknown-linux-musl` support Clang.
- Clang target predefines are captured separately from GNU/Linux.
- Layout and calling conventions match the corresponding Linux SysV/AAPCS
  profiles. Musl uses its own sysroot and does not add synthetic Slate macros.
- Musl has no Debian cross-triplet include subdirectory.

## Clang resource headers

- `slate-sysroots/src/compiler_headers.rs` pins Debian's
  `libclang-common-22-dev` package from a fixed snapshot with SHA-256.
- Extract the built resource include directory and Debian copyright notice;
  the same header bundle serves every host and supported target.
- Source-only `clang/lib/Headers` omits generated ARM headers. Debian's built
  bundle includes NEON, SVE, SME, MVE, CDE, and their generated dependencies.
- When updating Clang, update the package, snapshot, checksum, extraction
  paths, and required-header checks together. The package manifest replaces
  installations acquired with older pins or the source-only installer.

## Sysroot installation locks

- Target sysroots and compiler bundles use `File::try_lock` on a sibling
  `.lock` file; dropping the handle or terminating the process releases it.
- Keep lock files after release. Unlinking permits concurrent installers to
  lock different files at the same path.
- Existing unlocked lock files, including stale PID files, are reused.
  Contention returns `WouldBlock` with an installation-in-progress message.

## macOS SDK registration

- `slate-sysroots/src/darwin.rs` canonicalizes supplied SDK paths and uses the
  SDK manifest as the staged install identity.
- A different SDK replaces the registration, even if the old SDK is missing;
  the same canonical path reuses a valid registration.
- `OSX_CROSS_SDK` overrides existing registrations. Without an override,
  installation reuses an existing registration before attempting discovery.
- Validate replacement headers before publishing; failed replacement retains
  the old registration.
