# Reference

This chapter collects detailed specifications and design references for the
parser and its IR. Add focused references here as the developer guide grows.

## Preprocessor directives

Slate processes directives before parsing C syntax. Conditional branches are
selected for the active configuration; inactive code does not reach the parser.

| Directive | Support |
| --- | --- |
| `#define`, `#undef` | Object-like and function-like macros, variadics, stringification, token pasting, and compiler-specific macro behavior. |
| `#include` | Quoted and angled includes, macro-expanded operands, include search paths, and `#include_next` for GCC and Clang. |
| `#import` | Include once for GCC and Clang. MSVC type-library imports are unsupported. |
| `#if`, `#ifdef`, `#ifndef`, `#elif`, `#else`, `#endif` | Conditional inclusion with compiler and standard gates for `#elifdef` and `#elifndef`. Conditions support `defined` and preprocessor integer expressions. |
| `#embed` | Embed resource bytes with `limit`, `prefix`, `suffix`, and `if_empty`; `__has_embed` uses the same parameter syntax. |
| `#pragma` | `once`, macro push/pop, supported poison forms, and pass-through pragmas. GCC optimize pragmas update preprocessing math-option macros. |
| `#line` | Set presumed file and line for `__FILE__` and `__LINE__`; GNU line markers are also recognized. |
| `#error`, `#warning` | Produce preprocessing diagnostics. |
| `#assert`, `#unassert` | GCC flavor only. |
| `#ident` and empty `#` | Ignored. Other unknown directives are errors in active groups. |

`_Pragma` is destringized and processed like a pragma. With Microsoft
extensions enabled, `__pragma` is also recognized. Support can vary by compiler
flavor and language standard; use the selected target configuration when
checking a directive's availability.

For the full behavior matrix and implementation details, see the [preprocessor
concept](https://github.com/takashiidobe/slate/blob/main/wiki/concepts/preprocessor.md).
