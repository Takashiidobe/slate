# Macro Expansion

Slate expands macros before parsing because the replacement can change the
grammar and meaning of the C program. It preserves the boundary between
replacement tokens and caller arguments because Rust macro recovery needs that
boundary after sema.

## Expand and rescan

The preprocessor collects the actual arguments and expands them where
substitution requires it. It pushes the replacement tokens back onto the stream
to expand nested calls. Each token carries an interned hide set that prevents
reexpansion through a recursive replacement chain.

```c
#define STEP 1
#define NEXT(x) ((x) + STEP)
int next(int n) { return NEXT(n); }
```

The rescan expands `STEP` inside `NEXT`. The parser sees `((n) + 1)`, while the
token labels still identify the nested replacement and the occurrence of `n`
from the caller. That distinction gives Rust lowering a candidate body and an
argument to substitute. An argument expanded before substitution can carry its
own origin into the result.

## Keep the result of token operations

Stringification and pasting produce tokens whose values depend on the argument
spelling. For example, `CAT(foo, bar)` with a body of `a ## b` produces the
identifier `foobar`. Slate labels that identifier as a token produced by the
body. Lowering cannot substitute either input for it as if it were a value
argument. Keeping the expanded identifier preserves the C binding when the macro
cannot be recovered.

Directives can also expand macros, for example to obtain an include name. Slate
expands a directive operand within that directive, then runs the directive on
the result. This can change which file is read without producing a macro
invocation in the parsed C program.

Start with `pp/expand.rs` in `crates/slate-parser/src/`. For the records sema
receives, see [Macro Provenance](macro-provenance.md).
