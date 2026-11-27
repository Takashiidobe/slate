# MSVC Flags

Use `--flavor=msvc` for MSVC inputs. Pass `--target=TRIPLE` to select the
Windows ABI for the original build.

```sh
slate translate --flavor=msvc --target=x86_64-pc-windows-msvc /std:c17 /DFEATURE=1 input.c
```

## C standard flags

The last `/std:` option wins.

- `/std:c11` selects C11.
- `/std:c17` selects C17.
- `/std:clatest` selects C23 and defines `__STDC_VERSION__` as `202312L`.
- `/experimental:c11atomics` removes the `__STDC_NO_ATOMICS__` predefine when a
  C standard was explicitly selected.

## Preprocessor and header search flags

- `/DNAME[=VALUE]` defines a macro. Omitting the value defines it as `1`.
- `/UNAME` undefines a macro. Slate applies `/D` and `/U` in command-line order.
- `/I DIR` adds an include directory. The joined form `/IDIR` also works.
- `/FI FILE` forces a file to be included. The joined form `/FIFILE` also works.
- `/X` skips the default system include directories.
- `/Zc:preprocessor` selects the conforming preprocessor, which Slate uses by
  default. Slate rejects `/Zc:preprocessor-`.
- `/Zc:trigraphs` enables trigraph replacement.
- `/Zc:trigraphs-` disables trigraph replacement, the default.
- `/Zc:__STDC__` defines `__STDC__` as `1`.

## Language and character set flags

- `/Za` disables Microsoft extensions and defines `__STDC__`. It conflicts with
  `/Ze` and `/std:`.
- `/Ze` enables Microsoft extensions, the default.
- `/utf-8` sets both source and execution character sets to UTF-8. It conflicts
  with either of the individual charset flags.
- `/source-charset:NAME` selects the source encoding. Slate accepts UTF-8 and
  supported single-byte code pages.
- `/execution-charset:NAME` selects the encoding for ordinary character and
  string literals. It accepts the same kinds of encodings.

## OpenMP flags

- `/openmp` enables OpenMP pragma parsing and sets `_OPENMP` to `200203`.
- `/openmp:llvm` enables OpenMP and also defines `_OPENMP_LLVM_RUNTIME`.
- `/openmp:experimental` enables OpenMP and sets `_OPENMP` to `2019`.
- `/openmp-` disables OpenMP and clears its predefines.

## Target and calling convention flags

- `/Gd` selects cdecl, the default on x86.
- `/Gr` selects fastcall on x86.
- `/Gz` selects stdcall on x86.
- `/Gv` selects vectorcall on x86 and x64.
- `/arch:AVX2` enables AVX2 on x86 and x64 and affects ISA predefines and vector
  ABI.
- `/arch:armv8.1` selects Armv8.1 on AArch64 and enables Large System Extensions
  (LSE).

## Type and record layout flags

- `/J` makes plain `char` unsigned and defines `_CHAR_UNSIGNED`.
- `/Zp1` caps field alignment at one byte. Bare `/Zp` has the same effect.
- `/Zp2` caps field alignment at two bytes.
- `/Zp4` caps field alignment at four bytes.
- `/Zp8` caps field alignment at eight bytes.
- `/Zp16` caps field alignment at sixteen bytes.

## Floating-point flags

The last model selection wins. Slate checks conflicts as it reads the flags.

- `/fp:precise` selects the precise model and defines `_M_FP_PRECISE`.
- `/fp:strict` selects the strict model, with runtime rounding and observable
  exceptions.
- `/fp:fast` selects the fast model and permits contraction.
- `/fp:contract` permits floating-point contraction. It conflicts with
  `/fp:strict`.
- `/fp:except` enables observable floating-point exceptions. It conflicts with
  `/fp:fast`.
- `/fp:except-` clears the exception request. The strict model still implies
  observable exceptions.

## Diagnostic flags

- `/W0` suppresses warnings. The lowercase `/w` spelling has the same effect.
- `/W1` selects warning level 1.
- `/W2` selects warning level 2.
- `/W3` selects warning level 3.
- `/W4` selects warning level 4, including modeled diagnostics such as C4213 for
  casting an lvalue used in an assignment.
- `/Wall` enables the highest warning level.

## CRT and driver flags

- `/MT` selects the static release CRT predefines, the default.
- `/MTd` selects the static debug CRT predefines.
- `/MD` selects the dynamic release CRT predefines.
- `/MDd` selects the dynamic debug CRT predefines.
- `/c` is accepted as a compile-only driver option.
- `/nologo` is accepted as a driver option.
- `/FoFILE` accepts the original object file path. Slate writes its output
  according to the Slate command you run.
- `/O2` is accepted and participates in option conflict checks such as `/RTC`
  with optimization.
- `/Zi` accepts the original debug information setting.
