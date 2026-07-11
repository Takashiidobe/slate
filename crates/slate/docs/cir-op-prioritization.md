# CIR Op Prioritization Draft

Planning draft for C-relevant CIR support. C++-only ops are omitted.

## Priorities

- P0: core C or CIR spelling drift that blocks ordinary C.
- P1: high-leverage C features that unlock broad source coverage.
- P2: builtins, math, memory helpers, and stdlib probe coverage.
- P3: extensions or target hooks; support only with an explicit fixture.

## Unsupported Inventory

| Op                            | Priority | Notes                                                                              |
| ----------------------------- | -------- | ---------------------------------------------------------------------------------- |
| `cir.binop`                   | P0       | generic scalar binary op spelling; check whether current CIR still emits split ops |
| `cir.binop.overflow`          | P0       | checked/overflowing arithmetic form                                                |
| `cir.brcond`                  | P0       | low-level conditional branch variant                                               |
| `cir.do`                      | P0       | do-while                                                                           |
| `cir.switch.flat`             | P0       | switch lowering variant                                                            |
| `cir.unary`                   | P0       | generic scalar unary op spelling; check whether current CIR still emits split ops  |
| `cir.atomic.clear`            | P1       | C atomics                                                                          |
| `cir.atomic.cmp_xchg`         | P1       | C atomics                                                                          |
| `cir.atomic.fence`            | P1       | C atomics                                                                          |
| `cir.atomic.fetch`            | P1       | C atomics                                                                          |
| `cir.atomic.test_and_set`     | P1       | C atomics                                                                          |
| `cir.atomic.xchg`             | P1       | C atomics                                                                          |
| `cir.complex.binop`           | P1       | generic C complex arithmetic spelling                                              |
| `cir.complex.imag_ptr`        | P1       | C complex lvalue access                                                            |
| `cir.complex.real_ptr`        | P1       | C complex lvalue access                                                            |
| `cir.extract_member`          | P1       | aggregate extraction                                                               |
| `cir.get_bitfield`            | P1       | C bitfields                                                                        |
| `cir.insert_member`           | P1       | aggregate mutation                                                                 |
| `cir.libc.memchr`             | P1       | memory libc call                                                                   |
| `cir.libc.memcpy`             | P1       | memory copy                                                                        |
| `cir.libc.memmove`            | P1       | memory move                                                                        |
| `cir.libc.memset`             | P1       | memory set                                                                         |
| `cir.memcpy_inline`           | P1       | memory copy                                                                        |
| `cir.memset_inline`           | P1       | memory set                                                                         |
| `cir.set_bitfield`            | P1       | C bitfields                                                                        |
| `cir.va.arg`                  | P1       | listed spelling differs from supported `cir.va_arg`                                |
| `cir.va.copy`                 | P1       | C varargs                                                                          |
| `cir.va.end`                  | P1       | listed spelling differs from supported `cir.va_end`                                |
| `cir.va.start`                | P1       | listed spelling differs from supported `cir.va_start`                              |
| `cir.acos`                    | P2       | math builtin/libm                                                                  |
| `cir.asin`                    | P2       | math builtin/libm                                                                  |
| `cir.atan`                    | P2       | math builtin/libm                                                                  |
| `cir.atan2`                   | P2       | math builtin/libm                                                                  |
| `cir.bit_reverse`             | P2       | integer bit builtin                                                                |
| `cir.byte_swap`               | P2       | integer bit builtin                                                                |
| `cir.clrsb`                   | P2       | integer bit builtin                                                                |
| `cir.clz`                     | P2       | integer bit builtin                                                                |
| `cir.cos`                     | P2       | math builtin/libm                                                                  |
| `cir.ctz`                     | P2       | integer bit builtin                                                                |
| `cir.exp`                     | P2       | math builtin/libm                                                                  |
| `cir.exp2`                    | P2       | math builtin/libm                                                                  |
| `cir.ffs`                     | P2       | integer bit builtin                                                                |
| `cir.fmaximum`                | P2       | math builtin/libm                                                                  |
| `cir.fminimum`                | P2       | math builtin/libm                                                                  |
| `cir.fmod`                    | P2       | math builtin/libm                                                                  |
| `cir.is_constant`             | P2       | constant query builtin                                                             |
| `cir.llrint`                  | P2       | math builtin/libm                                                                  |
| `cir.llround`                 | P2       | math builtin/libm                                                                  |
| `cir.log`                     | P2       | math builtin/libm                                                                  |
| `cir.log10`                   | P2       | math builtin/libm                                                                  |
| `cir.log2`                    | P2       | math builtin/libm                                                                  |
| `cir.lrint`                   | P2       | math builtin/libm                                                                  |
| `cir.lround`                  | P2       | math builtin/libm                                                                  |
| `cir.objsize`                 | P2       | object-size builtin                                                                |
| `cir.parity`                  | P2       | integer bit builtin                                                                |
| `cir.popcount`                | P2       | integer bit builtin                                                                |
| `cir.pow`                     | P2       | math builtin/libm                                                                  |
| `cir.rotate`                  | P2       | integer bit builtin                                                                |
| `cir.roundeven`               | P2       | math builtin/libm                                                                  |
| `cir.sin`                     | P2       | math builtin/libm                                                                  |
| `cir.sqrt`                    | P2       | math builtin/libm                                                                  |
| `cir.tan`                     | P2       | math builtin/libm                                                                  |
| `cir.asm`                     | P3       | inline assembly                                                                    |
| `cir.assume`                  | P3       | optimizer assumption                                                               |
| `cir.assume.aligned`          | P3       | alignment assumption builtin                                                       |
| `cir.assume.separate_storage` | P3       | optimizer assumption                                                               |
| `cir.blockaddress`            | P3       | labels-as-values/computed goto                                                     |
| `cir.clear_cache`             | P3       | target/cache builtin                                                               |
| `cir.eh.setjmp`               | P3       | setjmp/EH-adjacent lowering                                                        |
| `cir.expect`                  | P3       | branch prediction hint                                                             |
| `cir.frame_address`           | P3       | frame-address builtin                                                              |
| `cir.llvm.intrinsic`          | P3       | LLVM intrinsic escape hatch                                                        |
| `cir.prefetch`                | P3       | prefetch builtin                                                                   |
| `cir.ptr_mask`                | P3       | pointer masking                                                                    |
| `cir.stack_restore`           | P3       | stack builtin                                                                      |
| `cir.stack_save`              | P3       | stack builtin                                                                      |
| `cir.trap`                    | P3       | trap builtin                                                                       |
| `cir.unreachable`             | P3       | unreachable builtin/path                                                           |
| `cir.vec.cmp`                 | P3       | vector extension                                                                   |
| `cir.vec.create`              | P3       | vector extension                                                                   |
| `cir.vec.extract`             | P3       | vector extension                                                                   |
| `cir.vec.insert`              | P3       | vector extension                                                                   |
| `cir.vec.shuffle`             | P3       | vector extension                                                                   |
| `cir.vec.shuffle.dynamic`     | P3       | vector extension                                                                   |
| `cir.vec.splat`               | P3       | vector extension                                                                   |
| `cir.vec.ternary`             | P3       | vector extension                                                                   |

## Supported Checklist

- [x] `cir.func`
- [x] `cir.global`
- [x] `cir.alloca`
- [x] `cir.const`
- [x] `cir.load`
- [x] `cir.store`
- [x] `cir.copy`
- [x] `cir.cast`
- [x] `cir.get_global`
- [x] `cir.call`
- [x] `cir.return`
- [x] `cir.scope`
- [x] `cir.if`
- [x] `cir.for`
- [x] `cir.while`
- [x] `cir.switch`
- [x] `cir.case`
- [x] `cir.break`
- [x] `cir.continue`
- [x] `cir.goto`
- [x] `cir.label`
- [x] `cir.br`
- [x] `cir.condition`
- [x] `cir.yield`
- [x] `cir.shift`
- [x] `cir.cmp`
- [x] `cir.select`
- [x] `cir.ternary`
- [x] `cir.get_member`
- [x] `cir.get_element`
- [x] `cir.ptr_stride`
- [x] `cir.ptr_diff`
- [x] `cir.abs`
- [x] `cir.ceil`
- [x] `cir.copysign`
- [x] `cir.fabs`
- [x] `cir.fmaxnum`
- [x] `cir.fminnum`
- [x] `cir.floor`
- [x] `cir.is_fp_class`
- [x] `cir.nearbyint`
- [x] `cir.rint`
- [x] `cir.round`
- [x] `cir.signbit`
- [x] `cir.trunc`
- [x] `cir.complex.create`
- [x] `cir.complex.real`
- [x] `cir.complex.imag`

## Supported Extra Spellings

These are handled by Slate but were not in `ops.txt`.

- [x] `cir.add`
- [x] `cir.sub`
- [x] `cir.mul`
- [x] `cir.div`
- [x] `cir.rem`
- [x] `cir.and`
- [x] `cir.or`
- [x] `cir.xor`
- [x] `cir.not`
- [x] `cir.minus`
- [x] `cir.fadd`
- [x] `cir.fsub`
- [x] `cir.fmul`
- [x] `cir.fdiv`
- [x] `cir.complex.add`
- [x] `cir.complex.sub`
- [x] `cir.inc`
- [x] `cir.modf`
- [x] `cir.va_start`
- [x] `cir.va_arg`
- [x] `cir.va_end`
