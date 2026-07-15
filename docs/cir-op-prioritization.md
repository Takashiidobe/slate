# CIR Op Prioritization Draft

Planning draft for C-relevant CIR support. C++-only ops are omitted.

## Priorities

- P0: core C that blocks ordinary C.
- P1: high-leverage C features that unlock broad source coverage.
- P2: builtins, math, memory helpers, and stdlib probe coverage.
- P3: extensions or target hooks; support only with an explicit fixture.

## Checklist of ops to implement

| Priority | Notes | Ops |
| --- | --- | --- |
| P2 | math builtins/libm | `cir.acos`, `cir.asin`, `cir.atan`, `cir.atan2`, `cir.cos`, `cir.exp`, `cir.exp2`, `cir.fmaximum`, `cir.fminimum`, `cir.fmod`, `cir.llrint`, `cir.llround`, `cir.log`, `cir.log10`, `cir.log2`, `cir.lrint`, `cir.lround`, `cir.pow`, `cir.roundeven`, `cir.sin`, `cir.sqrt`, `cir.tan` |
| P2 | integer bit builtins | `cir.bitreverse`, `cir.byte_swap`, `cir.clrsb`, `cir.clz`, `cir.ctz`, `cir.ffs`, `cir.parity`, `cir.popcount`, `cir.rotate` |
| P2 | constant/object-size queries | `cir.is_constant`, `cir.objsize` |
| P3 | inline assembly | `cir.asm` |
| P3 | assumptions | `cir.assume` |
| P3 | labels-as-values/computed goto | `cir.block_address` |
| P3 | target/cache builtins | `cir.clear_cache`, `cir.prefetch` |
| P3 | setjmp/EH-adjacent lowering | `cir.eh.setjmp` |
| P3 | branch prediction hint | `cir.expect` |
| P3 | frame/stack builtins | `cir.frame_address`, `cir.stackrestore`, `cir.stacksave` |
| P3 | LLVM intrinsic escape hatch | `cir.call_llvm_intrinsic` |
| P3 | traps and unreachable paths | `cir.trap`, `cir.unreachable` |
| P3 | vector extensions | `cir.vec.*` |

## Excluded 

- **Flatten-CFG variants.** `cir.brcond`/`cir.switch.flat` come only from
  `--cir-flatten-cfg`, which Slate never runs. (`slate-2pq.2` closed.)

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
  - [x] `cir.add.overflow`
  - [x] `cir.sub.overflow`
  - [x] `cir.mul.overflow`
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
  - [x] `cir.complex.real_ptr`
  - [x] `cir.complex.imag_ptr`
  - [x] `cir.complex.add`
  - [x] `cir.complex.sub`
  - [x] `cir.complex.mul`
  - [x] `cir.complex.div`
  - [x] `cir.complex.conj`
- [x] aggregates/pointers
  - [x] `cir.extract_member`
  - [x] `cir.insert_member`
  - [x] `cir.get_member`
  - [x] `cir.get_element`
  - [x] `cir.ptr_stride`
  - [x] `cir.ptr_diff`
- [x] bitfields (per-field storage; get/set mask and sign-extend to the field width)
  - [x] `cir.get_bitfield`
  - [x] `cir.set_bitfield`
- [x] varargs
  - [x] `cir.va_start`
  - [x] `cir.va_arg`
  - [x] `cir.va_end`
- [x] atomics (real `std::sync::atomic` ops via `AtomicN::from_ptr`; int/bool only, float/ptr fall back to non-atomic RMW)
  - [x] `cir.atomic.fetch`
  - [x] `cir.atomic.xchg`
  - [x] `cir.atomic.cmpxchg`
  - [x] `cir.atomic.fence`
- [x] memory operations (byte-wise `std::ptr` ops; only clang builtins raise these — `__builtin_bzero`→memset, `__builtin_bcopy`→memmove, aggregate `__builtin_bit_cast`→memcpy, `__builtin_memchr`→memchr — while plain libc `memcpy`/`memset`/... stay `cir.call`s to the extern symbol)
  - [x] `cir.libc.memcpy` (`std::ptr::copy_nonoverlapping`)
  - [x] `cir.libc.memmove` (`std::ptr::copy`)
  - [x] `cir.libc.memset` (`std::ptr::write_bytes`)
  - [x] `cir.libc.memchr` (`__slate_memchr` prelude helper)
