# Builtins

Slate resolves compiler builtins using the selected flavor, target, and ISA
flags. The topic pages describe the builtin families and show C examples with
excerpts of the generated Rust. Examples use Clang mode and x86-64 Linux unless
specified otherwise.

- [Compile-time queries and types](builtins/compile-time.md)
- [Source locations and names](builtins/source-locations.md)
- [Integer arithmetic and bits](builtins/integer.md)
- [Floating point](builtins/floating-point.md)
- [Control flow and hints](builtins/control-flow.md)
- [Variadic arguments and storage](builtins/variadic-storage.md)
- [Atomics](builtins/atomics.md)
- [Library, memory, and string builtins](builtins/library-memory.md)
- [Vector and target intrinsics](builtins/vector-target.md)
- [Microsoft intrinsics](builtins/microsoft.md)
- [Builtin Inventories](builtins/inventories.md)

`__has_builtin` queries recognition. To check Rust lowering for a particular
call, use `slate lowering-barriers` with the original compiler flags and source.
Lowering can depend on operand types, enabled ISA features, and whether an
immediate argument folds to a constant. To inspect the parser's output, see
[Builtins and Inline Assembly](../parsing/sema/builtins-asm.md).
