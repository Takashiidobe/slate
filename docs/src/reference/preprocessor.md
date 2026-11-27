# Preprocessor Reference

The preprocessor runs once per translation unit configuration. It reads source
and header files, executes directives, and emits a flat sequence of `Code`,
`Comment`, and `Pragma` nodes. The parser consumes these tokens directly.

## Processing algorithm

1. Seed the macro table with the compiler and target snapshot, then apply
   Slate's target defaults and the selected dialect, standard, and target
   options. Apply command-line definitions and removals in order. Process forced
   inputs before reading the main file. `-imacros` keeps macro changes and
   discards the emitted nodes. `-include` also contributes its emitted nodes.
2. Lex each file into preprocessing tokens. The lexer handles an initial UTF-8
   BOM, trigraphs where enabled, and backslash-newline splicing before scanning.
   It marks the first token of a line so directive handling needs no newline
   token. Digraphs retain their original spelling for stringification and paste.
3. A line whose first non-comment token is `#` enters the directive dispatcher.
   Active text enters the macro expander. A function-like invocation can pull in
   later lines while collecting its arguments. Directive operands use an
   isolated expansion stream and stop at the end of the directive.
4. Process includes recursively with the same macro table. An include inherits
   the active configuration and updates the current file and header provenance.
5. Emit expanded code and source context. Preprocessor-handled pragmas can
   change the current run. Pragmas for semantic analysis remain in the stream.

## Macro expansion

Slate uses Prosser's hide-set algorithm. The expander reads from a pushback
stack in front of the file stream. Each token carries an interned set of macro
names that may no longer expand at that position. File tokens start with an
empty set.

Let `H(t)` be the hide set of token `t` and `M` the invoked name. An object-like
replacement takes `H(M) ∪ {M}`. A function-like replacement takes
`(H(M) ∩ H(closing parenthesis)) ∪ {M}`. A name already in its own hide set is
emitted as a final token. Once substitution is complete, the replacement goes
back on the stream for rescanning, so it can expand further macros and form an
invocation with following tokens.

```c
#define ADD_ONE(x) ((x) + 1)
#define APPLY ADD_ONE
#define N 2
APPLY(N)
```

Rescanning `APPLY` finds `ADD_ONE` followed by the original `(`. Argument
prescan turns `N` into `2`, then substitution emits `((2) + 1)`. In a recursive
pair such as `#define A B` and `#define B A`, expanding `A` eventually yields an
`A` whose hide set prevents another expansion.

| Construct                   | Expansion behavior                                                                                                                                                         |
| --------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Object-like macro           | Substitute the replacement list as soon as the name expands.                                                                                                               |
| Function-like macro         | Recognize the invocation only when the next preprocessing token is `(`. Collect arguments with balanced parentheses.                                                       |
| Plain parameter occurrence  | Expand the argument in isolation before substitution.                                                                                                                      |
| `#parameter`                | Stringify the unexpanded spelling, normalizing whitespace and escaping string and character literal contents.                                                              |
| `##`                        | Paste the operand spellings and lex the result again. GCC and Clang diagnose a paste that makes more than one token. The MSVC flavor keeps the tokens in that case.        |
| `__VA_ARGS__`, `__VA_OPT__` | Substitute the variadic argument or include the optional replacement when the expanded variadic argument has tokens. GNU named variadics reuse the same substitution path. |
| `, ## __VA_ARGS__`          | In the GNU extension, GCC and Clang elide the comma for an omitted variadic argument. A supplied empty argument can behave differently.                                    |

Builtin macros participate in macro lookup. `__LINE__` and `__FILE__` use the
presumed location, `__COUNTER__` advances within the run, and date and time
macros use the configured preprocessing context. GCC and Clang also expose
`__BASE_FILE__`, `__FILE_NAME__`, and `__INCLUDE_LEVEL__`. The MSVC flavor
emulates the conforming preprocessor and reports `_MSVC_TRADITIONAL` as zero.

## Conditional evaluation

The conditional stack tracks nesting, whether an arm has already been selected,
and whether the group has seen an `#else`. A skipped group scans conditional
directives for nesting and structural errors. It evaluates a later `#elif` only
if that conditional is active and no earlier arm was taken. Inactive text does
not expand macros or enter the parser.

For `#if`, Slate expands the expression with special handling for `defined` and
the selected flavor's `__has_*` queries. `defined` takes a macro name without
expanding the operand. Queries have their own operand rules. The evaluator
handles integer constant expressions and treats remaining unknown identifiers as
zero. GCC-only assertions are expanded before ordinary macros.

| Query family                                                           | Input and result                                                                                                                                                                  |
| ---------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `defined`, `#ifdef`, `#ifndef`                                         | Test whether a macro or a recognized conditional operator is defined.                                                                                                             |
| `__has_include`, `__has_include_next`                                  | Test header resolution using the configured include search.                                                                                                                       |
| `__has_embed`                                                          | Parse the same resource window as `#embed`. Returns `0` for unavailable resources or unsupported parameters, `1` for a nonempty window, and `2` for an empty one.                 |
| `__has_builtin`, `__has_feature`, `__has_extension`, attribute queries | Consult flavor, language mode, target, and registered names. The MSVC flavor treats unrecognized query spellings as ordinary identifiers, so a user macro can provide a fallback. |
| Clang target and warning queries                                       | `__is_target_*`, `__is_identifier`, and `__has_warning` consult the target triple or Clang-specific keyword and warning tables.                                                   |

## Directives

| Directive                                              | Behavior                                                                                                                                                                              |
| ------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `#define`, `#undef`                                    | Add or remove a macro definition.                                                                                                                                                     |
| `#include`                                             | Resolve a quoted, angled, or macro-expanded header operand.                                                                                                                           |
| `#include_next`, `#import`                             | GCC and Clang extensions. `include_next` resumes after the current header's search directory. `import` makes a header once-only and skips it if already entered.                      |
| `#if`, `#ifdef`, `#ifndef`, `#elif`, `#else`, `#endif` | Select and delimit conditional groups.                                                                                                                                                |
| `#elifdef`, `#elifndef`                                | Available in Clang modes, GCC GNU modes and C23, and MSVC C23.                                                                                                                        |
| `#embed`                                               | Emit resource bytes from a quoted path with `limit`, `prefix`, `suffix`, and `if_empty`. Vendor-specific `offset` is applied before `limit`. Angled resource lookup is unsupported.   |
| `#pragma`                                              | Process preprocessor pragmas such as `once`, macro push/pop, and poison. Forward other pragmas to the parser. GCC optimize and option-stack pragmas also update preprocessing macros. |
| `#line`, GNU line markers                              | Change presumed file and line information used by builtins and diagnostics.                                                                                                           |
| `#error`, `#warning`                                   | Report directive diagnostics.                                                                                                                                                         |
| `#assert`, `#unassert`                                 | Add or remove assertion answers in the GCC flavor.                                                                                                                                    |
| `#ident`, empty `#`                                    | Ignore. Unknown directives are errors in an active group.                                                                                                                             |

`_Pragma` destringizes and re-lexes its operand. With Microsoft extensions,
`__pragma` takes balanced tokens directly. Both execute at their position in the
expanded stream, so a macro restored by `pop_macro` can affect later tokens on
the same line.

`#embed` and `__has_embed` accept the same window parameters. `offset` uses the
`gnu::` namespace in GCC and `clang::` in Clang. The window starts at the
offset, then takes at most `limit` bytes. A negative offset or limit is an
error. An unsupported parameter is an error for `#embed` and yields zero for
`__has_embed`. `prefix` and `suffix` surround a nonempty byte sequence, while
`if_empty` supplies the replacement for an empty one.

## Header and expansion provenance

Quoted includes first search the including file's directory, then `-iquote`,
`-I`, and system paths. Angled includes start with `-I`. System paths include
explicit `-isystem` directories, compiler headers, and target sysroot headers. A
directly spelled header name is used as written. Other operands expand to obtain
the name. Include guards and `#pragma once` can suppress a repeated include.
Header kind follows the search result, so an angled header found via `-I` can
still be a user header.

`#pragma once` tracks the canonical path. The include-guard optimization
recognizes an outer `#ifndef X` or `#if !defined(X)` that covers the file and
has no alternate arm at that level. A later include can skip the file while `X`
is defined. The MSVC type-library form of `#import` is unsupported.

Token provenance identifies the file, header kind, source location, and the
outermost system header entered from user code. Macro expansion also records the
spelling and invocation locations and labels tokens as replacement-body tokens
or occurrences of a specific argument. Stringification and paste produce body
tokens. The invocation table can retain an expansion that emits no tokens, and
directive expansions do not enter the source invocation table.

An optional preprocessing trace records conditional arms, macro events, and
token dependencies for one configuration. The trace records definitions and
removals seen in skipped groups, which can explain why a later test found a name
undefined. Its dependency sets can over-approximate which settings would change
the output.

## Implementation map

Paths start at `crates/slate-parser/src/`.

| Path                                | Responsibility                                                                    |
| ----------------------------------- | --------------------------------------------------------------------------------- |
| `lexer.rs`, `pp/syntax.rs`          | Preprocessing tokens, source locations, and directive lines.                      |
| `pp/mod.rs`                         | `Preprocessor`, configuration, directive dispatch, conditional stack, and output. |
| `pp/expand.rs`, `pp/hide_set.rs`    | Stream, argument prescan, substitution, rescanning, and interned hide sets.       |
| `pp/define.rs`, `pp/math.rs`        | Definition syntax and GCC pragma-driven macro updates.                            |
| `pp/include.rs`, `pp/embed.rs`      | Header and resource resolution, include guards, and embed parameters.             |
| `pp/has_checks.rs`, `const_expr.rs` | Conditional queries and integer expression evaluation.                            |
| `pp/invocations.rs`, `pp/trace.rs`  | Source macro invocation records and configuration dependencies.                   |

Run `slate-parser pp input.c` to inspect preprocessed code and retained pragmas.
The output inserts spaces where adjacent tokens would otherwise re-lex as one.
