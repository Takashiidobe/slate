# Lowerer phase file boundaries

_created 2026-08-24_

The typed lowering pipeline is divided by ownership:

- `module_index.rs` discovers translation-unit symbols, features, linkage, and
  record dependencies without producing Rust items.
- `function_setup.rs` constructs per-function state and Rust signatures before
  typed body lowering begins.
- `dispatch.rs` owns the only lowering traversal of typed function regions and
  the generated `Op` dispatch match.
- `types.rs` converts structural CIR/C types and answers type properties.
- `record_analysis.rs` discovers and reconciles anonymous record identities and
  field metadata.
- `lowerer.rs` owns shared state plus module/global/record emission and constant
  initializer rendering.

Keep these as free functions or inherent `Lowerer`/`FunctionLowerer` methods.
Do not add phase traits or another operation traversal to enforce the layout.
