# Macro Dump Plugin

The macro dump plugin, located at:`tools/macro-dump-plugin/MacroDump.cpp`
is a clang plugin that Slate always loads when it
invokes `SLATE_CLANG` to parse C (see `src/frontend/c_ast.rs`). Clang's AST
and CIR don't retain which macro produced a construct or which header a
declaration came from, so the plugin recovers those source facts during the
same parse and emits them as line-oriented JSON on stderr, keyed by physical
source offset. `c_ast.rs` reads that stream back and joins it to the AST by
offset.

## Usage

The plugin is called with `-Xclang -plugin-arg-macro-dump -Xclang
-trusted-root=<dir>` for the libc shim directory and Clang's resource-dir
include path. Only system includes `<$includes>` under a trusted root
path count for provenance purposes; if no trusted roots are given,
everything is a trusted root.

## Event kinds

Each line is `EVENT_NAME {json}`:

- **`MACRO_EXPANSION`**: a macro expanded in the main file: its name, the
  expansion site, and (if it has one) the header the definition came from.
- **`INCLUDE_PROVENANCE`**: an `#include` directive: written spelling,
  angled/system-ness, resolved path, and file identity. Also feeds the
  trusted-header table used by the other events.
- **`FUNCTION_PROVENANCE`**: every direct/indirect call: whether the callee
  has a trusted-header declaration, whether an untrusted definition or a
  symbol-changing attribute (`alias`, `asm label`, `ifunc`, `weak`) shadows
  it, and evidence per redeclaration. This is what lets Slate decide whether
  a call is safe to lower against `libc-shim`'s definition.
- **`RECORD_PACKING`**: record types with `#pragma pack`/`__attribute__
((packed))` alignment, reported via `MaxFieldAlignmentAttr`.
- **`FLOATING_LITERAL`** / **`GLOBAL_LONG_DOUBLE`**: exact bit patterns for
  `long double` literals and constant-evaluable `long double` expressions in
  global initializers, since Clang's AST only exposes these as host
  `APFloat`s and Slate needs the raw bits to reproduce them in Rust, to
  get around a bug in upstream CIR for now.

## Building

The plugin must be rebuilt against the same Clang tree `SLATE_CLANG` points
at whenever that tree changes, since it links against that build's headers:

```bash
SLATE_CLANG=~/llvm-project/build-cir/bin/clang ./tools/macro-dump-plugin/build.sh
```

This builds `tools/macro-dump-plugin/build/MacroDump.so` and installs it to
`<clang-build>/lib/SlateMacroDump.so`, the default `SLATE_MACRO_DUMP_PLUGIN`
path.
