# IR Spec

<!-- toc -->
- [Rules](#rules)
- [Subpages](#subpages)
- [Worked example](#worked-example)
<!-- /toc -->

What the IR means and what is implemented, split by area under
[`ir/`](ir/). The printed syntax is in [IR Grammar](ir-grammar.md); the
principles every IR change is judged against are in
[architecture](architecture.md). Epic: `slate-parser-lh7`.

The IR is not Clang IR and does not aim for CIR compatibility. It exists for
translation to Rust, not optimization.

## Rules

1. **Shown types and ops are concrete and Rust-shaped.** `int` is `i32`,
   `long double` is `f80`, a conversion is `widen<i32>(x)`. The Rust
   lowering never needs C's conversion rules.
2. **C-specific information is metadata.** Original C type, typedef chain,
   conversion reason, header provenance: kept, optional to read, never
   needed to reconstruct semantics.
3. **No temporaries or storage beyond the source's**, except `[synthetic]`
   ones that side-effect hoisting needs.
4. **Every semantic choice is explicit.** UB, rounding, flavor-specific
   layout, and evaluation order are printed on the node they affect.
5. **Prune what isn't useful.** C bookkeeping with no translation signal is
   dropped once resolved.

## Subpages

| Page | Covers |
| --- | --- |
| [pipeline](ir/pipeline.md) | placement in sema, failure kinds, node ids and metadata, dump commands, required constant folding, scopes, reachability pruning |
| [types](ir/types.md) | shown vs C types, scalar formats, integer literals, `_BitInt`, tags, enums, record layout (pack, `ms_struct`, Microsoft), `typeof`/`auto` |
| [type families](ir/type-families.md) | complex, imaginary, vector, fixed-point |
| [operations](ir/operations.md) | arithmetic and comparison policies, floating settings and pragmas, conversions, `?:` |
| [places and pointers](ir/places-pointers.md) | places, members, bit-fields, pointer arithmetic and conversions, `nullptr_t`, MS mixed-size pointers, qualified access |
| [atomics](ir/atomics.md) | `_Atomic` layout and ABI per flavor, atomic builtins |
| [control flow](ir/control-flow.md) | structured control flow, function ends, side-effect hoisting, unsequenced order, `_Generic`, pragmas |
| [declarations](ir/declarations.md) | linkage, redeclaration conflicts, symbol and object attributes, alignment, inline and noreturn |
| [initialization](ir/initialization.md) | aggregate initializers, strings, compound literals, flexible arrays, VLAs |
| [calls and ABI](ir/calls-abi.md) | call signatures, `AbiSignature`, `native_c` divergences, vector ABI, calling conventions, targets |
| [builtins](ir/builtins.md) | builtin registry, implicit declarations, custom lowering, `va_list`, source-location builtins |
| [asm](ir/asm.md) | GNU and MSVC inline asm, operand selection, Rust options, naked and file-scope asm |
| [open design](ir/open-design.md) | agreed but unimplemented shapes and open questions |

Update the matching subpage in the same change as any IR change. When sema
knowingly departs from clang, record it there with the reason.

## Worked example

```c
#include <stdio.h>
int add(int a, int b) { int c = a + b; return c; }
int main(void) { printf("%d\n", add(2, 3)); }
```

`slate-parser ir` (`ir_add.c`), target header and string global omitted:

```text
fn %1 @printf(%8 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
fn %2 @add(%3 a: i32, %4 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
    let %5 c: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%3), read<i32>(%4));
    return read<i32>(%5);
}
fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
    call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%9)), call<i32, signature=fn(i32, i32) -> i32>(%2, const<i32>(2), const<i32>(3)));
}
```

With `--show-metadata`, the C types appear:

```text
fn %2 @add(%3 a: i32 [c="int"], %4 b: i32 [c="int"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(int, int)"] {
    let %5 c: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%3), read<i32>(%4)) [c="int"];
```
