# Source Context and Macro Recovery

IR carries the resolved program and keeps source context for emitting readable
Rust. The same arithmetic operation can carry a written C type, a comment
anchor, or a macro argument marker.

## Follow one macro into IR

**C input**

```c
typedef unsigned int word;
#define PLUS_ONE(x) ((x) + 1u)
// Increment a word.
word next(word n) { return PLUS_ONE(n); }
```

**IR excerpt**

```ir
return add<u32, overflow=wrap>(read<u32>(%2) [c="word"], reinterpret<u32, reason=usual_arith>(const<u32>(1)) [c="word"]) [c="word"];
```

With metadata, macros, comments, and spans enabled, the dump includes the same
add with origin labels:

```ir
return add<u32, overflow=wrap>(
    read<u32>(%2) [c="word"] [macro_arg="#0.0"] [macro=PLUS_ONE#0 arg0],
    reinterpret<u32, reason=usual_arith>(const<u32>(1) [macro=PLUS_ONE#0 body]) [c="word"] [macro=PLUS_ONE#0 body]) [c="word"] [macro_value="#0"] [macro=PLUS_ONE#0 body];
```

The read of `n` is argument zero of expansion zero. The literal comes from the
macro body. The `macro_value` marker connects the result to the whole expansion.
The invocation table records which definition and argument ranges produced it:

```ir
type @type0 word = u32 [c="unsigned int"];
comment leading "// Increment a word." [spelling=7:58+20, expansion=7:58+20];
macro_definition #0 PLUS_ONE(x) [defined=7:1] = "( ( x ) + 1u )";
macro_expansion #0 PLUS_ONE [definition=#0] [site=7:106+11] [arguments=115+1] [covers=expr] [arg0=expr];
```

Lowering can use those boundaries to recover an eligible Rust macro call. The
add and its wrapping policy describe the computation even when Slate emits the
expansion inline. The written type `word` preserves a name for emission, and the
comment anchor keeps the comment beside the declaration it describes.

## Inspect provenance

```bash
slate-parser ir input.c --show-macros --show-metadata --show-comments --show-spans
```

See [Macro Provenance](../macro-provenance.md) for how preprocessing produces
these records and how sema carries them into the module.
