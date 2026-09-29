# Preprocessor

<!-- toc -->
- [Pipeline](#pipeline)
- [Predefines](#predefines)
- [Includes](#includes)
- [Provenance](#provenance)
- [Macro provenance](#macro-provenance)
- [Expansion](#expansion)
- [Conditionals](#conditionals)
- [Pragmas and line control](#pragmas-and-line-control)
<!-- /toc -->

`src/pp/`: one preprocessing run per configuration, producing a flat
stream of `PPNode`s (`Code`, `Comment`, `Pragma`) that the parser reads as
`ParserInput`.

| File | Contents |
| --- | --- |
| `mod.rs` | `Preprocessor`, predefine seeding, `walk_group`, directives, conditionals, pragma operators, provenance |
| `syntax.rs` | lexing into logical lines, groups ([pp-logical-line-merging](pp-logical-line-merging.md)) |
| `expand.rs` | macro expansion, builtin macros, `#if` operand protection |
| `define.rs` | `#define` / `#undef` parsing |
| `include.rs` | include resolution, `#pragma once`, depth limit, outermost system header |
| `has_checks.rs` | hand-maintained `__has_builtin` / `__has_feature` / … answers, seeded from clang tablegen and extended per flavor ([attributes](attributes.md#preprocessor-queries)) |

Include directories come from `compiler_headers.rs` and `sysroot.rs`,
which point at headers installed by `../slate-sysroots`.

## Pipeline

```text
Preprocessor::new → configure            predefines, target options, -D/-U, forced files
parse_file → parse_source
  lex → syntax::parse                    logical lines, merged, grouped
  walk_group
    Item::Text        → expand_line      → Code / Pragma nodes
    Item::Conditional → walk_conditional → walk_group on the taken branch
    Item::Directive   → define, include, embed, pragma, line, error/warning
```

Unknown directives are `UnsupportedDirective`; `#ident` and `#` are
ignored.

## Predefines

Seeded in order; each group is parsed as `#define`s under a pseudo-file.
Later groups remove earlier definitions of the names they set.

| Pseudo-file | Kind | Contents |
| --- | --- | --- |
| snapshot name | System | `target.profile.predefines(flavor)`, captured from the real compiler ([adding-a-target](adding-a-target.md)); missing snapshot is an error |
| `<slate-target-defaults>` | System | slate-side target defaults |
| `<slate-gnu-namespace-predefines>` | System | gnu modes only |
| `<standard predefines>` | System | `__STDC_VERSION__`, `__STRICT_ANSI__`, GNU inline macros, C23 `char8_t` / `bool` / `_FMTb__` macros, recomputed for the selected standard |
| `<target options>` | User | long-double and ISA macros (not for msvc on Windows), `__ROUNDING_MATH__` (gcc, `-frounding-math`), msvc explicit `__STDC_VERSION__`, inline-semantics swap |
| `<command line>` | User | `-D` / `-U` in order |

Forced files (`-imacros`, `-include`) follow, resolved as quoted includes
from `<command line>` ([compiler-arg-rules](compiler-arg-rules.md#forced-files-and-macros)).

## Includes

- Search order: [compiler-arg-rules](compiler-arg-rules.md). A quoted
  include tries the includer's directory first and inherits its
  `HeaderKind`; `#include_next` resumes after the current file's
  directory.
- Angled names are taken from raw source between `<` and `>`; from a macro,
  the tokens are joined.
- `#pragma once` keys on the canonical path.
- `MAX_INCLUDE_DEPTH = 200`. Cycles are detected only by the limit, as in
  clang and gcc.

## Provenance

`Provenance { file, kind, line, system_header }` on every token
([ast-spec](ast-spec.md#locations-and-provenance)).

- `outermost_system_header` is set on entering the first System header
  from user code and restored on leaving it, so every token below
  `<stdio.h>` reports `<stdio.h>`. Slate uses it to recognize libc
  declarations.
- `expand_line` gives every token of an expanded line the line's
  provenance, including tokens spelled in a macro defined elsewhere.

## Macro provenance

`macro_origin` records which macro produced a token, so Slate can emit a
named Rust constant (`CHAR_MAX`) instead of its value.

- `origin_for_expansion`: a fresh token gets
  `MacroOrigin { name, definition, inner: None }`. A token already carrying
  an origin keeps its outer `name`/`definition` and gains an `inner` link,
  so `name` is the macro written at the use site and `inner` chains toward
  the innermost replacement.
- `Stamp::apply` sets `expansion` to the invocation's location and the
  origin on every replacement token. Substituted arguments keep their own
  origin.
- `Span::cover` keeps the origin only when all covered tokens share it, so
  an expression built entirely from one macro carries it.

## Expansion

- `expand_rescanning` walks the tokens: builtins (`__LINE__`, `__FILE__`,
  `__FILE_NAME__`, `__BASE_FILE__`, `__INCLUDE_LEVEL__`, `__COUNTER__`,
  `__DATE__`, `__TIME__`) first, then macros not in the `disabled` set.
- Function-like: `invocation` may complete the argument list from the
  caller's tail. Arguments are prescanned only where used outside `#`/`##`
  (all of them if `__VA_OPT__` appears). `substitute_function_macro`
  handles `#`, `##` (re-lex; a paste that doesn't form one token keeps both),
  `__VA_ARGS__`, `__VA_OPT__`.
- `rescan` expands the replacement alone, and against the following tokens
  only when `wants_more` (unbalanced `(` or trailing function-like name).
- The first result token inherits the invocation's leading space.
- `expand_line` then classifies keywords per the dialect's features.

## Conditionals

- `#ifdef`-family: `is_defined` = defined macro or `is_defined_operator`
  (`__has_include` etc. for the flavor).
- `#if`: `expand_condition` (operands of `defined` and `__has_*` left
  unexpanded) → `__has_embed` → `__has_include` → `expand_has_checks` →
  `const_expr::Parser::evaluate_with_defined`.

## Pragmas and line control

- `_Pragma("...")` is destringized and re-lexed at the operator's location;
  `__pragma(...)` (with `microsoft_extensions`) takes its balanced tokens.
  Both become `Pragma` nodes, like `#pragma`.
- `record_pragma` acts on `push_macro`, `pop_macro`, and `once`; every
  other pragma passes through to the parser.
- `#line` and GNU line markers set `line_overrides`, read by
  `presumed_location` for `__LINE__` / `__FILE__`.
- `#error` / `#warning` go to `directive_diagnostics`.
