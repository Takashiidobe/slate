# Parsing

`crates/slate/src/frontend.rs` invokes slate-parser as a Rust library:
preprocessing, parsing, semantic analysis, then typed IR lowering. Compiler
arguments configure dialect, target, macros, forced inputs and include paths.
Headers come from slate-sysroots.

```bash
cargo run --release -p slate -- emit-slate-ir [compiler arguments] input.c
```

Slate consumes the typed module directly; printed IR is a debugging artifact.
The [IR specification](https://github.com/takashiidobe/slate/blob/main/wiki/concepts/ir-spec.md)
defines the parser-to-translator contract.
