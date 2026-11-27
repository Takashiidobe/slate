# GCC Flags

Use `--flavor=gcc` for GCC inputs. The target and original flags determine which
GNU extensions, predefined macros, and ABI rules apply.

## C standard flags

Pass `-std=MODE` to select the C language mode. The last `-std` or `-ansi`
setting wins. GNU modes enable GNU extensions for the corresponding ISO C
version.

- `-std=c89` selects ISO C90, also known as ANSI C89.
- `-std=gnu89` selects C90 with GNU extensions.
- `-std=iso9899:199409` selects the C90 amendment mode, with C90 language rules
  and `__STDC_VERSION__=199409L`.
- `-std=c99` selects ISO C99.
- `-std=gnu99` selects C99 with GNU extensions.
- `-std=c11` selects ISO C11.
- `-std=gnu11` selects C11 with GNU extensions.
- `-std=c17` selects ISO C17, also known as C18.
- `-std=gnu17` selects C17 with GNU extensions.
- `-std=c23` selects ISO C23.
- `-std=gnu23` selects C23 with GNU extensions.
- `-std=c2y` selects Slate's mode for the draft C standard after C23.
- `-std=gnu2y` selects the draft C2Y mode with GNU extensions.
- `-ansi` selects the same mode as `-std=c89`.

### Accepted standard aliases

- `-std=c90` selects `c89`.
- `-std=gnu90` selects `gnu89`.
- `-std=c9x` selects `c99`.
- `-std=gnu9x` selects `gnu99`.
- `-std=c1x` selects `c11`.
- `-std=gnu1x` selects `gnu11`.
- `-std=c18` selects `c17`.
- `-std=gnu18` selects `gnu17`.
- `-std=c2x` selects `c23`.
- `-std=gnu2x` selects `gnu23`.
- `-std=iso9899:1990` selects `c89`.
- `-std=iso9899:1999` selects `c99`.
- `-std=iso9899:199x` selects `c99`.
- `-std=iso9899:2011` selects `c11`.
- `-std=iso9899:2017` selects `c17`.
- `-std=iso9899:2018` selects `c17`.
- `-std=iso9899:2024` selects `c23`.

## Preprocessor flags

- `-DNAME[=VALUE]` defines a macro. Omitting the value defines it as `1`.
- `-UNAME` undefines a macro. Slate applies `-D` and `-U` in command-line order,
  after establishing the compiler and target predefines.
- `-imacros FILE` imports the file's macros and discards its declarations.
- `-include FILE` reads the file before the primary input. Slate processes every
  `-imacros` file before every `-include` file.

## Header search flags

See [Headers and Include Search](../../parsing/headers.md) for search order.

- `-I DIR` adds a user include directory for quoted and angle-bracket includes.
- `-iquote DIR` adds a user include directory for quoted includes.
- `-isystem DIR` adds a system include directory ahead of the default system
  directories.
- `-idirafter DIR` adds a system include directory searched after the defaults.
- `--sysroot=DIR` replaces the default system header root.
- `-isysroot DIR` replaces the header root and takes precedence over
  `--sysroot`.
- `-nostdinc` skips default compiler builtin and system header directories.

## Language and extension flags

- `-ffreestanding` selects a freestanding environment and sets `__STDC_HOSTED__`
  to `0`.
- `-fhosted` selects a hosted environment and sets `__STDC_HOSTED__` to `1`. The
  later of `-ffreestanding` and `-fhosted` wins.
- `-fno-builtin` disables builtin treatment of library functions.
- `-fno-builtin-NAME` disables builtin treatment of one library function.
- `-fgnu89-inline` selects GNU89 inline definition rules.
- `-fcommon` makes tentative definitions common symbols.

## Target and ABI flags

- `-m32` selects the 32-bit x86 variant when available.
- `-m64` selects the 64-bit architecture variant when available.
- `-march=x86-64-v3` selects the x86-64-v3 ISA baseline.
- `-mavx2` enables the x86 AVX2 feature.
- `-mno-avx2` disables the x86 AVX2 feature.
- `-mfloat-abi=hard` selects the hard-float calling convention on Arm32.
- `-mfpu=neon` selects NEON as the Arm32 FPU setting.
- `-mthumb` selects Thumb instruction encoding on Arm32.
- `-mabi=ms` selects the Microsoft default calling convention on x86-64.
- `-mabi=sysv` selects the System V default calling convention on x86-64.
- `-mregparm=N`. On 32-bit x86, pass up to `N` eligible integer arguments in
  registers.
- `-mpreferred-stack-boundary=N`. On x86, set the preferred stack alignment to 2
  raised to the power `N` bytes.

## Type and record layout flags

- `-funsigned-char` makes plain `char` unsigned.
- `-fsigned-char` makes plain `char` signed.
- `-fshort-enums` uses the smallest fitting integer type for an enum without a
  fixed base type.
- `-fshort-wchar` uses `unsigned short` for `wchar_t`.
- `-fpack-struct[=N]` sets the default maximum field alignment. Bare
  `-fpack-struct` packs fields to one byte.
- `-funsigned-bitfields` makes plain integer bit-fields unsigned.

## Arithmetic flags

- `-fwrapv` makes signed addition, subtraction, and multiplication wrap.
- `-ftrapv` traps on signed overflow in those operations. The last active
  overflow setting wins.
- `-ffp-contract=off` disables floating-point contraction.
- `-frounding-math` makes floating-point operations honor the runtime rounding
  mode. See [Floating-point Environment](../../floating-point-environment.md).
- `-fno-delete-null-pointer-checks` keeps a dereference from implying that a
  pointer is non-null.
- `-ffast-math` enables a group of floating-point assumptions and permissions.
- `-fsingle-precision-constant` gives unsuffixed floating-point literals type
  `float`.

## Optimization and diagnostic flags

- `-O0` selects the unoptimized C mode.
- `-O2` selects the C optimization level used for predefines and resolving
  inline assembly operands that need constants after inlining.
- `-Os` selects the size optimization mode and its predefines.
- `-Werror` promotes enabled warnings that Slate models to errors.
- `-Werror=NAME` enables the named modeled warning and makes it an error.
- `-Wno-error=NAME` lets the named warning remain a warning.
- `-Wno-NAME` suppresses the named warning, including ones that are errors by
  default.
- `-w` suppresses modeled warnings while retaining diagnostics that are errors
  by default unless separately demoted.
