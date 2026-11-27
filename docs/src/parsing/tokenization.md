# Tokenization

Slate's lexer gives preprocessing a token kind and enough source information
to recover its spelling. The parser consumes the expanded tokens directly.

Token kind and spelling can differ. In a dialect with digraphs enabled,
`<:` has the same token kind as `[`, but `token_spelling` still returns
`<:`. For example, these tokens produce the string literal `"<:"`:

```c
#define STR(x) #x
const char *text = STR(<:);
```

Literals also keep their spelling for sema. The suffix in `0xFFu` affects
which C type sema chooses. Rust lowering uses the resolved type and value.

Spans preserve where the text was spelled and, after macro expansion, where
it was used. A token written in a macro body can point to the definition
while its expansion points to the call. See
[Macro Provenance](macro-provenance.md) for how Slate uses these locations
and origin labels to recover source structure.
