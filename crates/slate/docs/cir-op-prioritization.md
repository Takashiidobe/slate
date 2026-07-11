# CIR Op Prioritization Draft

Planning draft for C-relevant CIR support. C++-only ops are omitted.

## Priorities

- P0: core C that blocks ordinary C.
- P1: high-leverage C features that unlock broad source coverage.
- P2: builtins, math, memory helpers, and stdlib probe coverage.
- P3: extensions or target hooks; support only with an explicit fixture.

## Unsupported Inventory

| Priority | Notes | Ops |
| --- | --- | --- |
| P0 | generic scalar spellings | `cir.binop`, `cir.unary` |
| P0 | low-level conditional branch variant | `cir.brcond` |
| P0 | switch lowering variant | `cir.switch.flat` |
| P1 | do-while | `cir.do` |
| P1 | decrement | `cir.dec` |
| P1 | checked/overflowing arithmetic | `cir.add.overflow`, `cir.sub.overflow`, `cir.mul.overflow`, `cir.div.overflow`, `cir.rem.overflow`, `cir.binop.overflow` |
| P1 | generic C complex arithmetic spelling | `cir.complex.binop` |
| P1 | C complex lvalue access | `cir.complex.real_ptr`, `cir.complex.imag_ptr` |
| P1 | aggregate value mutation/extraction | `cir.extract_member`, `cir.insert_member` |
| P1 | C bitfields | `cir.get_bitfield`, `cir.set_bitfield` |
| P1 | memory operations | `cir.libc.memchr`, `cir.libc.memcpy`, `cir.libc.memmove`, `cir.libc.memset`, `cir.memcpy_inline`, `cir.memset_inline` |
| P2 | math builtins/libm | `cir.acos`, `cir.asin`, `cir.atan`, `cir.atan2`, `cir.cos`, `cir.exp`, `cir.exp2`, `cir.fmaximum`, `cir.fminimum`, `cir.fmod`, `cir.llrint`, `cir.llround`, `cir.log`, `cir.log10`, `cir.log2`, `cir.lrint`, `cir.lround`, `cir.pow`, `cir.roundeven`, `cir.sin`, `cir.sqrt`, `cir.tan` |
| P2 | integer bit builtins | `cir.bit_reverse`, `cir.byte_swap`, `cir.clrsb`, `cir.clz`, `cir.ctz`, `cir.ffs`, `cir.parity`, `cir.popcount`, `cir.rotate` |
| P2 | constant/object-size queries | `cir.is_constant`, `cir.objsize` |
| P3 | inline assembly | `cir.asm` |
| P3 | assumptions and alignment hints | `cir.assume`, `cir.assume.aligned`, `cir.assume.separate_storage` |
| P3 | labels-as-values/computed goto | `cir.blockaddress` |
| P3 | target/cache builtins | `cir.clear_cache`, `cir.prefetch` |
| P3 | setjmp/EH-adjacent lowering | `cir.eh.setjmp` |
| P3 | branch prediction hint | `cir.expect` |
| P3 | frame/stack builtins | `cir.frame_address`, `cir.stack_restore`, `cir.stack_save` |
| P3 | LLVM intrinsic escape hatch | `cir.llvm.intrinsic` |
| P3 | pointer masking | `cir.ptr_mask` |
| P3 | traps and unreachable paths | `cir.trap`, `cir.unreachable` |
| P3 | vector extensions | `cir.vec.*` |

## Supported Checklist

- [x] module/function/global
  - [x] `cir.func`
  - [x] `cir.global`
  - [x] `cir.get_global`
- [x] storage/value
  - [x] `cir.alloca`
  - [x] `cir.const`
  - [x] `cir.load`
  - [x] `cir.store`
  - [x] `cir.copy`
  - [x] `cir.cast`
- [x] calls/returns
  - [x] `cir.call`
  - [x] `cir.return`
- [x] structured control flow
  - [x] `cir.scope`
  - [x] `cir.if`
  - [x] `cir.for`
  - [x] `cir.while`
  - [x] `cir.switch`
  - [x] `cir.case`
  - [x] `cir.condition`
  - [x] `cir.yield`
- [x] unstructured control flow
  - [x] `cir.break`
  - [x] `cir.continue`
  - [x] `cir.goto`
  - [x] `cir.label`
  - [x] `cir.br`
- [x] scalar arithmetic
  - [x] `cir.add`
  - [x] `cir.sub`
  - [x] `cir.mul`
  - [x] `cir.div`
  - [x] `cir.rem`
  - [x] `cir.inc`
  - [x] `cir.minus`
- [x] scalar bitwise/logical
  - [x] `cir.and`
  - [x] `cir.or`
  - [x] `cir.xor`
  - [x] `cir.not`
  - [x] `cir.shift`
- [x] comparisons/selection
  - [x] `cir.cmp`
  - [x] `cir.select`
  - [x] `cir.ternary`
- [x] float arithmetic
  - [x] `cir.fadd`
  - [x] `cir.fsub`
  - [x] `cir.fmul`
  - [x] `cir.fdiv`
- [x] float math/classification
  - [x] `cir.abs`
  - [x] `cir.ceil`
  - [x] `cir.copysign`
  - [x] `cir.fabs`
  - [x] `cir.fmaxnum`
  - [x] `cir.fminnum`
  - [x] `cir.floor`
  - [x] `cir.is_fp_class`
  - [x] `cir.modf`
  - [x] `cir.nearbyint`
  - [x] `cir.rint`
  - [x] `cir.round`
  - [x] `cir.signbit`
  - [x] `cir.trunc`
- [x] complex
  - [x] `cir.complex.create`
  - [x] `cir.complex.real`
  - [x] `cir.complex.imag`
  - [x] `cir.complex.add`
  - [x] `cir.complex.sub`
- [x] aggregates/pointers
  - [x] `cir.get_member`
  - [x] `cir.get_element`
  - [x] `cir.ptr_stride`
  - [x] `cir.ptr_diff`
- [x] varargs
  - [x] `cir.va_start`
  - [x] `cir.va_arg`
  - [x] `cir.va_end`
- [x] atomics (lowered as non-atomic RMW; correct single-threaded)
  - [x] `cir.atomic.fetch`
  - [x] `cir.atomic.xchg`
  - [x] `cir.atomic.cmpxchg`
  - [x] `cir.atomic.fence`
