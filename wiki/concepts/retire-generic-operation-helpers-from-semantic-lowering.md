# Retire generic operation helpers from semantic lowering

_created 2026-08-24_

Slate semantic lowering consumes `clang_ir::model::{Module, Function, Global,
Op}`. It must not inspect `Function::raw`, `Global::raw`, operation mnemonics,
generic operands/results, or generic operation attributes.

When semantic data is missing, add it to the hand-maintained `clang-ir` model
or `clang-ir-types-gen`, then regenerate `clang-ir-types`. Regression tests for
generated fields belong in `clang-ir`, not in the generated crate.

Generic operations remain valid only in Slate's CIR normalization and
flattening boundary. Generic type and attribute alias tables remain available
on the typed module for structural resolution; the typed model does not retain
the generic tree or raw function/global operations as an alternate semantic
model.

Slate's build script enforces this boundary across `frontend/lowerer.rs` and
its submodules. Generic operation types, retained generic trees, and mnemonic
dispatch fail the build if reintroduced there.
