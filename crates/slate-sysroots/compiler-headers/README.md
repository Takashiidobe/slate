# Compiler headers

Compiler-provided headers are kept separately from target libc sysroots. Each
acquisition writes to `compiler-headers/<compiler-version>/`. Clang and GCC
source headers are kept with the repository and retain their license files.
Apple Clang and MSVC bundles are ignored by Git because their toolchain terms
do not allow us to redistribute them. Keep upstream license and notice files
with each extracted tree.

The bundles are:

- `clang-<version>/include`: `clang/lib/Headers` from the matching upstream
  LLVM source release.
- `apple-clang-<version>/include`: the resource headers from the selected
  Apple Clang installation. Apple Clang shares much of upstream Clang's header
  tree, but is a separately built downstream toolchain; use its own versioned
  bundle.
- `gcc-<version>/include`: GCC's generic `gcc/ginclude` headers from the matching
  GCC source release. Target-specific GCC headers and generated `include-fixed`
  headers depend on the configured GCC build and are not represented here.
- `msvc-<version>/<target>/include`: MSVC CRT compiler-support headers from
  the corresponding locally acquired Windows SDK/CRT sysroot. The Microsoft
  headers are license-restricted and must not be redistributed by Slate.

The download scripts live in `scripts/`. LLVM and GCC source archives are
downloaded from their upstream release repositories. Apple Clang is acquired
from an installed Xcode or Command Line Tools installation because Apple does
not publish its toolchain headers as an upstream LLVM release. MSVC acquisition
uses `xwin`, which handles Microsoft's license acceptance as part of the
existing sysroot download flow. Apple Clang and upstream Clang share much of
their header code, but should stay separately versioned because Apple ships a
downstream build and may carry different contents.

Examples:

```sh
scripts/fetch-clang-compiler-headers.sh 22.1.8
scripts/fetch-gcc-compiler-headers.sh 16.1.0
scripts/fetch-apple-clang-compiler-headers.sh
scripts/fetch-msvc-compiler-headers.sh 14.50 x86_64-pc-windows-msvc
```

The MSVC version label is supplied by the caller and should match the CRT
version selected for the local sysroot. The script fetches that sysroot through
the existing `xwin` flow when it is not already present.
