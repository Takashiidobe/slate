# Attributes

Slate checks attributes using the compiler flavor, language mode, target, and
declaration. Depending on the spelling, it can change generated Rust, record
metadata, accept the spelling for compatibility, or report a diagnostic.

GNU names go inside `__attribute__((...))`. GCC and Clang also accept registered
scoped spellings such as `[[gnu::packed]]`. Clang-specific attributes can use a
`clang::` or `gnu::` scope depending on the name. Use the Microsoft page for
`__declspec(...)` and `[[msvc::...]]`.

The examples show the relevant excerpts from Slate's generated Rust. They use
Clang mode on x86-64 Linux unless a different flavor or target is specified.

- [Standard C attributes](attributes/standard.md)
- [Size, alignment, and layout](attributes/layout.md)
- [Objects and symbols](attributes/symbols.md)
- [Function behavior and targeting](attributes/functions.md)
- [Allocation, bounds, and cleanup](attributes/allocation-bounds-cleanup.md)
- [Function contracts and compiler hints](attributes/contracts-hints.md)
- [Calling convention attributes](attributes/calling-conventions.md)
- [Microsoft spellings](attributes/microsoft.md)
- [AArch64 SME attributes](attributes/sme.md)
- [Compiler Attribute Registry](attributes/registry.md)

A positive feature query establishes recognition of a spelling. Check the
attribute's page for its translation behavior. See
[Preprocessor Reference](preprocessor.md) for the feature queries.
