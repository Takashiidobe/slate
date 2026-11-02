# Itanium mangling of overloadable functions

<!-- toc -->
- [Status](#status)
- [Why](#why)
- [Mangler](#mangler)
- [Wiring](#wiring)
- [Redeclarations](#redeclarations)
- [TODO](#todo)
<!-- /toc -->

## Status

Implemented for non-MSVC targets. Tracked by `slate-parser-6x05.41`.

## Why

clang links an `extern` `__attribute__((overloadable))` function under its
Itanium-mangled name (`void f(int)` is `_Z1fi`; static ones are `_ZL1fi`).
Slate needs the symbol in the IR to emit `#[link_name]` / `#[export_name]`.
Static overloads never leave the TU and keep their C name.

## Mangler

`itanium_function_symbol` in `src/sema/ctype/mangle.rs`, written from the
"Mangling" section of the full Itanium C++ ABI (`abi.html`; the
`abi-mangling.html` page is only the table of codes).

- Builtin codes, `_BitInt`, `nullptr`, complex, imaginary, vector.
- Pointers, arrays, function types, `struct`/`union`/`enum` source names.
- `r`/`V`/`K` and `_Atomic` qualifiers.
- Parameter adjustment: arrays and functions decay, top-level cv dropped,
  typedefs desugared.
- Substitutions (`S_`, `S<seq-id>_`): children numbered before their composite,
  builtins excluded.

Returns `None` for what it cannot encode: anonymous tags, non-default pointer
address spaces, block pointers, `va_list`, sizeless types, `Decimal64x`. The
checker then rejects the declaration as `Unimplemented`.

## Wiring

- The symbol is set on `Function.symbol.asm_name` by `mangle_external_overload`
  (`sema/module.rs`), called from `sema/function.rs` after the function's
  semantics are final. It skips `inline_only` functions, internal linkage and
  functions with an explicit asm label.
- MSVC targets are not mangled. clang uses Microsoft mangling there
  (`?f@@$$J0YAHH@Z`). Declarations still lower; a call to or definition of an
  extern overload stays `Unimplemented` on those targets.
- The oracle fixtures are `ir_overloadable_extern*.c`; their symbols match
  clang's `-emit-llvm` output.

## Redeclarations

clang requires `overloadable` on every declaration of an overload (one unmarked
first declaration is allowed and keeps its C name). Name resolution cannot
tell a redeclaration from a new overload, so each declaration gets its own
binding. `declare_object` (`sema/assertion.rs`) detects a compatible earlier
overload and `TypeResolver::merge_overload_redeclaration` (`sema/types.rs`)
aliases the new binding to it: `declarations`, `references`, `overload_sets` and
`overload_candidates` are rewritten, and `declaration_id` reads
`types.declarations` so the lowerer sees the same binding. This also fixes
forward declarations of static overloads
(`ir_overloadable_forward_declaration.c`).

## TODO

- Anonymous tags: clang mangles them through their typedef name for linkage.
- Microsoft mangling for MSVC targets.
- Taking the address of an overloaded name (`fp = f`) is still
  `Unimplemented` ("overloaded function used outside a call").
- Slate lowering: emit `#[link_name]` from `asm_name` and a distinct Rust
  identifier per overload. Not checked against the `slate` profile.
