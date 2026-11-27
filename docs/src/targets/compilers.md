# Compilers

Use the flavor of the original C compiler. For a single file, Slate defaults to
`--flavor=clang`. Project translation infers the flavor from the compiler named
in the compilation database. You can override it with `--flavor`.

| Compiler        | Flavor  | What Slate models                                                                                                                                                     |
| --------------- | ------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Clang           | `clang` | Clang's builtin headers and extensions such as [Blocks](../lowering/clang-blocks.md). Use the Clang flavor for Apple Clang on macOS.                                  |
| GCC             | `gcc`   | GCC's predefines, builtin headers, and flag semantics. The GCC compiler-header bundles are grouped by Linux architecture family.                                      |
| MSVC (`cl.exe`) | `msvc`  | Microsoft declarations such as `__declspec`, calling conventions such as `__stdcall`, and options such as `/D` and `/I`. Slate uses the conforming MSVC preprocessor. |

The compiler flavor and target ABI are separate settings. For example, Clang can
target `x86_64-pc-windows-msvc` using Clang language rules and the Windows MSVC
ABI.

Even shared flags can behave differently. With `-ftrapv -fwrapv`, GCC uses
wrapping signed arithmetic because the last flag wins. Clang keeps trapping
arithmetic while `-ftrapv` is active. See
[Translation Unit Configuration](../parsing/configuration.md) for other ways
that flags affect translation.
