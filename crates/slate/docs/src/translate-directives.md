# Translate directives

`frontend::preprocess` records source directives and their activity using
slate-parser's preprocessor. `record-cfg <file.c> [compiler arguments]` prints
that information as JSON.

`frontend::directive_translate` translates supported whole-item conditional
branches into Rust cfg items. It rejects conditions that cannot be mapped or
selected, conditional boundaries inside bodies, and excessive variant counts.
Source spans are preserved while diagnostic directives are blanked for variant
parsing.

```bash
cargo run --release -p slate-c2rust -- record-cfg input.c --target=x86_64-unknown-linux-gnu
cargo run --release -p slate-c2rust -- translate \
  --targets=x86_64-unknown-linux-gnu,aarch64-unknown-linux-gnu input.c
```

`translate` may expand supported directives automatically; otherwise it uses
the selected concrete preprocessing configuration. Target headers come
from slate-sysroots. Project generation consumes one compile-command
configuration per translation unit and does not merge target variants.
