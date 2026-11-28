# Builtin Inventories

Use these inventories to look up individual spellings, prototypes, and target
restrictions. A compiler entry or a positive `__has_builtin` query confirms
recognition in the applicable configuration. A call also needs a Rust lowering
for its operand types and features.

## Clang

- [General and x86 builtins](https://github.com/Takashiidobe/slate/blob/main/crates/slate-parser/src/sema/clang_builtins.rs).
- [AArch64 and NEON builtins](https://github.com/Takashiidobe/slate/blob/main/crates/slate-parser/src/sema/clang_aarch64_builtins.rs).

For C syntax and argument semantics, see
[Clang Language Extensions](https://clang.llvm.org/docs/LanguageExtensions.html).

## GCC

- [Generic and library builtins](https://github.com/Takashiidobe/slate/blob/main/crates/slate-parser/src/sema/gcc_builtins.rs).
- [x86 builtins](https://github.com/Takashiidobe/slate/blob/main/crates/slate-parser/src/sema/gcc_x86_builtins.rs).
- [Arm and AArch64 builtins](https://github.com/Takashiidobe/slate/blob/main/crates/slate-parser/src/sema/gcc_arm_builtins.rs).

For C semantics, see
[GCC C Extensions](https://gcc.gnu.org/onlinedocs/gcc/C-Extensions.html).

## MSVC

- [Alphabetical MSVC inventory](https://github.com/Takashiidobe/slate/blob/main/crates/slate-parser/tools/msvc_intrinsics_alphabetical.jsonl)
  includes architecture, declaration sources, and lowering probe results where
  available.
- [MSVC parser table](https://github.com/Takashiidobe/slate/blob/main/crates/slate-parser/src/sema/msvc_builtins.rs)
  contains the prototypes and availability used by Slate.

For C signatures and semantics, see
[Microsoft Compiler Intrinsics](https://learn.microsoft.com/en-us/cpp/intrinsics/compiler-intrinsics).

## Rust lowering mappings

- [LLVM intrinsic catalog](https://github.com/Takashiidobe/slate/blob/main/crates/slate/src/frontend/lowerer/intrinsics_table.rs)
  records builtin aliases, signatures, and immediate-argument constraints.
- [AArch64 SVE mappings](https://github.com/Takashiidobe/slate/blob/main/crates/slate/src/frontend/lowerer/sve_table.rs)
  list GCC SVE names Slate maps to Rust architecture intrinsics.
- [Handwritten builtin handling](https://github.com/Takashiidobe/slate/blob/main/crates/slate-parser/src/sema/builtins.rs)
  covers additional names interpreted directly by Slate.

To check a specific call, run `slate lowering-barriers` with its original
compiler flags. For regeneration, see
[Generate Builtin Inventories](../../tools/builtin-inventories.md) and
[Generate the Intrinsic Catalog](../../tools/intrinsic-catalog.md).
