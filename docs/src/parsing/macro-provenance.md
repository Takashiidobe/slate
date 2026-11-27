# Macro Provenance

The preprocessor passes expanded tokens to the parser and records where they
came from. Slate uses those records to recover a macro call in Rust, preserve
names such as `CHAR_MAX`, and identify library declarations by their headers.

## Follow an expansion

```c
#define STEP 1
#define NEXT(x) ((x) + STEP)
int next(int n) { return NEXT(n); }
```

The parser receives the tokens for `((n) + 1)`. The `+` comes from the body of
`NEXT`. The `n` comes from its first argument. The `1` comes from expanding
`STEP` inside `NEXT`. The expansion record lets Slate distinguish the pieces
that belong in a macro template from the expression to substitute at the call
site.

## Read the records

- **Source locations.** The spelling location identifies the original text. The
  expansion location identifies the invocation. Header provenance records the
  declaring file and the outermost system header reached from user code.
- **Token origin.** `MacroOrigin` records the producing macro and its
  definition, with an inner link for nested replacements. Substituted arguments
  preserve their own origin. When a span covers tokens with different origins,
  it drops the single-origin label.
- **Invocation and argument tracking.** `MacroInvocations` records definitions
  and tracked calls in user code. A call written in source can carry the source
  range of each argument and link to its enclosing expansion.
  `Span::macro_nesting` labels tokens by expansion ID and by body or argument.
  Token pasting creates a new body token. For example, the result of `a ## b`
  does not stand for either input argument.

The invocation table can record a call whose replacement is empty, even though
the call leaves no IR node. It skips expansions in directives and calls spelled
in system headers. A system macro invoked from user code can still have a
record.

## Carry the records into IR

The AST keeps macro spans and carries the invocation table on `TranslationUnit`.
Sema copies the table to `Module::macros`. When macro structure is enabled, sema
can mark statement regions and value arguments that correspond to a recorded
expansion.

Slate checks whether it can keep each expansion as a Rust macro call. For
expansions emitted inline, it strips the wrappers and lowers the expanded code.
Kept wrappers constrain which transforms may move or rewrite code. See
[Transforms](../writing-a-transform.md#what-the-macro-slots-see).

## Trace configuration dependencies

Project planning uses an optional trace to record why tokens exist in one
configuration. The trace follows conditional branches and macro dependencies.
Rust macro recovery uses the invocation table and argument ranges. See
[Conditional Compilation](conditional-compilation.md#explain-why-a-token-exists).

Start with `pp/invocations.rs` under `crates/slate-parser/src/`.
