# clang-ast

CIR is the primary lowering input (see [clang-ir](./clang-ir.md)), but CIR is
an optimizer-facing IR: it only keeps what's needed to generate code, and
throws away source-level facts that have no bearing on codegen. Slate still
needs some of those facts to produce readable, idiomatic Rust, so it also
parses the Clang AST as a second, complementary source and joins the two by
source location.

`src/frontend/c_ast.rs` runs this by invoking
`$SLATE_CLANG -Xclang -ast-dump=json -fsyntax-only -fparse-all-comments`
(with the [macro dump plugin](./macro-dump-plugin.md), since its
provenance events are stitched into the same `Unit`) and deserializes the
result into a `Unit` of `Record`, `Enum`, `Function`, and `DeclarationComment`
facts, keyed by `offset`/`Loc` so the lowerer can look them up while walking
CIR ops.

## What CIR doesn't have

- Comments: CIR carries no doc comments at all. Struct/enum/field/function
  doc comments come entirely from Clang's `FullComment` AST nodes
  (`attached_comment` in `c_ast.rs`), so we run with `-fparse-all-comments`.
- Macro identity: CIR only sees the expanded text. Which literal came
  from a `#define`, and what to name it in Rust, comes from
  `MACRO_EXPANSION` events off the macro dump plugin.
- Header provenance: Whether a call's declaration came from a
  trusted header (so it's safe to lower against `libc-shim`) is a
  preprocessor/AST-level fact that the AST saves and the CIR has
  resolved.
- Bit-field widths, `#pragmas`/`__attribute__((packed))` alignment,
  `weak`/`naked`/`asm` attributes, and `goto` label names are all
  source-level declaration facts that CIR throws away.
- Exact `long double` bit patterns for literals and constant-evaluable
  global initializers, since Clang's constant evaluator gives Slate the raw
  `APFloat` bits where CIR's own handling isn't reliable yet (see the plugin
  doc's `FLOATING_LITERAL`/`GLOBAL_LONG_DOUBLE` events).
