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
- [Oracle](#oracle)
<!-- /toc -->

`src/pp/`: one preprocessing run per configuration, producing a flat
stream of `PPNode`s (`Code`, `Comment`, `Pragma`) that the parser reads as
`ParserInput`.

| File | Contents |
| --- | --- |
| `mod.rs` | `Preprocessor`, predefine seeding, `process`, directives, the conditional stack, pragma operators, provenance |
| `syntax.rs` | `TokenSource`: directive and physical text lines read from the token stream |
| `expand.rs` | `Stream`, the hide-set expander, substitution, builtin macros, `#if` operand protection |
| `hide_set.rs` | interned hide sets with memoized union and intersection |
| `define.rs` | `#define` / `#undef` parsing |
| `include.rs` | include resolution, `#pragma once`, depth limit, outermost system header |
| `has_checks.rs` | hand-maintained `__has_builtin` / `__has_feature` / … answers, seeded from clang tablegen and extended per flavor ([attributes](attributes.md#preprocessor-queries)) |

Include directories come from `compiler_headers.rs` and `sysroot.rs`,
which point at headers installed by the `slate-sysroots` workspace package.

## Pipeline

```text
Preprocessor::new → configure            predefines, target options, -D/-U, forced files
parse_file → parse_source
  Lexer::tokenize_lines                  tokens flagged at_line_start, no newline tokens
  process(TokenSource)                   pulls one physical line at a time
    text line       → Stream → next_raw / expand_token → emit_line → Code / Pragma nodes
    #if family      → conditional stack; a false branch runs skip_group
    other directive → run_directive      → define, include, embed, pragma, line, error/warning
```

A text line becomes one group. Expansion reads that line's tokens, and an
invocation whose `(` or arguments lie on later lines pulls those lines into
the same group, so a group is one physical line unless a macro call spans
lines. Its Code node takes the first line's provenance. Comments of the
first line come before the node, comments of pulled lines after it, then
any nodes produced by directives met inside the arguments.

A directive is a line whose first non-comment token is `#`. Conditionals
are evaluated when the stream reaches them. `skip_group` reads only the
directives of a skipped group. It tracks nesting and `#else` ordering there,
and evaluates a later `#elif` of its own conditional only while no branch
has been taken. Structural errors (stray `#endif`, `#else` after `#else`,
unterminated `#if`) surface where the stream reaches them. Earlier
evaluation errors take precedence.

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
| `<microsoft modes>` | System | clang: delta from the snapshot's default [MS modes](compiler-flags.md#ms-modes) |
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
- A header name spelled before expansion (`"..."`, or `<` with a `>` on the
  line) is used as is, and tokens after it are ignored. Otherwise the
  operands are expanded first (`#include <foo` with `#define foo stddef.h>`).
  Angled names come from raw source between `<` and `>`, or, when either
  bracket came from a macro, from the tokens joined with their leading
  spaces.
- `#pragma once` keys on the canonical path.
- Multiple-include optimization, as in clang: a file whose only code and
  directives are one `#ifndef X` / `#if !defined(X)` group with no
  `#elif`/`#else` at its level (comments may sit outside) records `X`
  against its `FileId`. A later include of that `FileId` while `X` is
  defined returns nothing without reading or lexing the file. Repeat
  includes therefore drop the comments outside the guard.
- `MAX_INCLUDE_DEPTH = 200`. Cycles are detected only by the limit, as in
  clang and gcc.

## Provenance

`Provenance { file, kind, line, system_header }` on every token
([ast-spec](ast-spec.md#locations-and-provenance)).

- `outermost_system_header` is set on entering the first System header
  from user code and restored on leaving it, so every token below
  `<stdio.h>` reports `<stdio.h>`. Slate uses it to recognize libc
  declarations.
- `emit_line` gives every token of a group the group's
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

Prosser's algorithm (as in chibicc's `expand_macro`). A `PPToken` is a
token plus an interned `HideSet`; file tokens start empty.

- `Stream` is a pushback stack in front of an optional `FileInput`.
  `next_raw` pops the stack; when reading an invocation it pulls further
  physical lines from the file, running any directive it meets (so
  `#ifdef` inside arguments selects an argument, as in gcc and clang).
- `expand_token` returns a final token, or pushes a replacement back:
  - builtins (`__LINE__`, `__FILE__`, `__COUNTER__`, `__DATE__`,
    `__TIME__`, `__TIMESTAMP__`, and for gcc and clang `__FILE_NAME__`,
    `__BASE_FILE__`, `__INCLUDE_LEVEL__`) are entries in `macros` marked
    `builtin`, seeded first by `configure`, so `defined`, `#undef`,
    `#define` and `push_macro` see them. msvc ignores `#define`/`#undef` of
    its builtins (C4117; `is_reserved_macro`). `__TIMESTAMP__` is the
    current file's modification time. `__LINE__` differs per flavor when an invocation spans
    lines: gcc reports the line of the outermost macro name, clang the line
    of its closing `)` (a `PPToken`'s `end`, carried by `Stamp`), and msvc
    how far the source has been read (`source_position`), even for a
    `__LINE__` written in an argument;
  - a name in its own hide set is final (painted);
  - object-like: the replacement gets `hs(name) ∪ {name}`;
  - function-like: only if the next token is `(`. `Stream::next_is_lparen`
    peeks the stack, else the file's next non-comment token, so a `#`
    line in between means no invocation. The replacement gets
    `(hs(name) ∩ hs(')')) ∪ {name}`. An unterminated call or a wrong
    argument count pushes the tokens back and leaves the name.
- Arguments are prescanned by `expand_isolated`, a stream without a file,
  only where used outside `#`/`##` (all of them if `__VA_OPT__` appears).
  `substitute_function_macro` handles `#`, `##` (re-lex; a paste that
  doesn't form one token keeps both), `__VA_ARGS__`, `__VA_OPT__`. The
  lexer scans numbers as pp-numbers (letters, digits, `_`, `.`, and a sign
  after `e`/`p`), so `name##2_cb` pastes onto the single token `2_cb`.
- Comma elision (`comma_elision`): gcc and clang drop the comma of
  `, ## __VA_ARGS__` when the variadic argument is omitted (`F(a)`, not
  `F(a,)`); for a macro whose only parameter is `...`, `H()` counts as
  omitted in gnu modes only. msvc's traditional preprocessor drops a comma
  before any `__VA_ARGS__` that is empty after expansion, with or without
  `##`.
- GNU named variadics (`#define F(fmt, args...)`): `#define` keeps `args`
  out of the fixed parameters and rewrites its uses in the replacement to
  `__VA_ARGS__`, so arity, `#`, `##` and comma elision share the
  `__VA_ARGS__` paths. Linux's `bpf.h` (`___BPF_FUNC_MAPPER(FN, ctx...)`)
  depends on it.
- The first replacement token inherits the invocation's leading space.
- `#if`, `#include`, `#embed` and `#line` operands expand with
  `expand_isolated` too (`expand_macros`), so they never read past the
  directive.
- `read_piece` takes each final token of a group and recognizes `_Pragma`
  and `__pragma` there, with an expanded lookahead for `(`. The pragma
  takes effect at that point, so a `pop_macro` changes the rest of the
  line. `emit_line` classifies keywords per the dialect's features.

## Conditionals

- `#ifdef`-family: `is_defined` = defined macro or `is_defined_operator`
  (`__has_include` etc. for the flavor).
- `#if`: `expand_condition` → `__has_embed` → `__has_include` →
  `expand_has_checks` → `const_expr::Parser::evaluate_with_defined`.
  `expand_condition` runs the stream expander over the directive, and a
  `defined` or `__has_*` name takes its operand unexpanded from the stream.
  That includes a `defined` an expansion produced, with its operand from
  the expansion or from the directive after it, as gcc and clang do.
- clang only: `__has_declspec_attribute` answers from
  `attribute_support::declspec_registered`, and only with
  `microsoft_extensions` (0 on Linux, as clang without `-fms-extensions`).
  `__has_warning("-Wx")` and `__is_identifier(x)` answer from
  `has_checks/clang.rs` (generated, [generated-sources](generated-sources.md)):
  clang's warning groups, and its keywords sorted by the C modes that
  reserve them (`asm` gnu only, `typeof` gnu or C23, `restrict` C99+).

## Pragmas and line control

- `_Pragma("...")` is destringized and re-lexed at the operator's location;
  `__pragma(...)` (with `microsoft_extensions`) takes its balanced tokens.
  Both are recognized in the expanded stream ([Expansion](#expansion)).
  Both become `Pragma` nodes, like `#pragma`.
- `record_pragma` acts on `push_macro`, `pop_macro`, and `once`; every
  other pragma passes through to the parser.
- `#line` and GNU line markers set `line_overrides`, read by
  `presumed_location` for `__LINE__` / `__FILE__`.
- `#error` / `#warning` go to `directive_diagnostics`.

## Oracle

`slate-parser pp <file> [args]` prints the preprocessed stream: one line
per `Code` node, `Pragma` nodes as `#pragma ...`, comments dropped. Keywords
print with their source spelling (`__inline__`, not `inline`), and a space is
inserted wherever adjacent tokens would otherwise relex as one.

`tools/pp_diff.py` compares it with `clang -E -P`, token by token, ignoring
whitespace and line breaks (`--corpus [project ...]`, `--compdb`, or
`--file x.c -- args`). The report goes to `target/pp-diff.md`. Both sides
are run under matching conditions:

- clang runs with `-nostdinc` over slate's own compiler headers and sysroot,
  so both preprocess the same headers.
- Without a `-std` in the compile command, both get `-std=gnu17`, which is
  clang's default. Slate defaults to gnu23 (slate-parser-6x05.6).
- Pragmas that clang consumes (`once`, `push_macro`, `pop_macro`, `region`,
  `GCC system_header`/`poison`, `clang deprecated`/`final`/`diagnostic`) are
  skipped on both sides.
