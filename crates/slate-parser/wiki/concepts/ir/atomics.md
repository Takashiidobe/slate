# Atomics

<!-- toc -->
- [`_Atomic` layout per flavor](#_atomic-layout-per-flavor)
  - [Argument passing](#argument-passing)
- [Atomic updates](#atomic-updates)
- [Atomic builtins](#atomic-builtins)
  - [Orderings and arguments](#orderings-and-arguments)
  - [Arithmetic](#arithmetic)
  - [Lock-free queries](#lock-free-queries)
<!-- /toc -->

Part of the [IR spec](../ir-spec.md). How atomic accesses print is in
[qualified access](places-pointers.md#qualified-access).

## `_Atomic` layout per flavor

`_Atomic` changes layout, and the three compilers disagree on aggregates.
Scalars agree because natural alignment already satisfies every rule. Sizes
reaching the IR are already adjusted.

| Flavor | Rule | `char a[3]` | `char a[16]` |
| --- | --- | --- | --- |
| clang | pad to a power of two and align to it, up to the widest lock-free width (16 on x86-64/AArch64, 8 on i686/ARM32); zero-sized gets 1 byte | 4/4 | 16/16 |
| gcc | no change; non-lock-free accesses go through libatomic | 3/1 | 16/1 |
| msvc (`/experimental:c11atomics`) | sizes 1/2/4/8 keep size, aligned to it; else `struct { int lock; T value; }` | 8/4 | 20/4 |

- `_Atomic` never qualifies an array; an array of atomics adjusts each
  element.
- Enclosing offsets differ too: `struct { char head; _Atomic struct { char
  a[3]; } value; char tail; }` is 12 bytes, value at 4 (clang); 5 bytes at
  1 (gcc); 16 bytes at 4, tail at 12 (msvc).
- The MSVC lock word is modeled as size and alignment only: C forbids
  reaching members of an atomic aggregate, so interior offsets aren't
  observable.
- Deliberately not reproduced: MSVC gives `_Atomic struct S` (size 4,
  align 1) 4/4 but `_Atomic T` through a typedef of the same struct 4/1.
  Layout can't see the spelling, and an unaligned lock-free atomic can't
  work, so every spelling gets 4/4.
- Measured against clang 22.1.8, gcc 16.2.1, cl.exe 19.51. Fixtures:
  `{clang,gcc}/linux/x86_64/ir_atomic_layout.c`,
  `msvc/windows/x86_64/ir_atomic_layout.c`.

### Argument passing

`AbiOperand` carries an `atomic` flag from the C qualifiers, because the
lowered type lost them (`_Atomic struct S` is the same `Type::Defined`).

- clang: an atomic record or complex argument is MEMORY on SysV64 and x86
  cdecl regardless of size. On AArch64 clang (and MSVC for records) passes
  atomic records and complex as integers or indirectly, from the qualified
  layout.
- gcc: the unqualified record ABI, HFAs included.
- win64: the lock-prefixed layout is what gets classified, so
  `struct { char a, b, c; }` (3 bytes, indirect) passes in a register when
  `_Atomic` (8 bytes). Returns agree.

## Atomic updates

An atomic compound assignment, `++`/`--` on an `_Atomic` object, or a
fetch/exchange builtin is one read-modify-write, so hoisting keeps
`update<T, result=..., atomic=...>(place, f(old))` whole in a synthetic
temporary instead of splitting it.

## Atomic builtins

`__c11_atomic_*` (what clang's `<stdatomic.h>` expands to), `__atomic_*`,
`__scoped_atomic_*`, and `__sync_*` resolve in sema without declarations,
typed from the pointer argument's pointee. They reuse the access nodes;
only compare-exchange and fences get their own.

| Source | IR |
| --- | --- |
| `load(p, o)` | `read<T, atomic=o>(deref(p))` |
| `store(p, v, o)` | `write<T, atomic=o>(deref(p), v)` |
| `fetch_OP(p, v, o)` | `update<T, result=old, atomic=o>(deref(p), OP(old, v))` |
| `__atomic_OP_fetch(p, v, o)` | `update<T, result=new, atomic=o>(..)` |
| `exchange(p, v, o)` | `update<T, result=old, atomic=o>(deref(p), v)` |
| nand / min / max | `not(and(old, v))` / `conditional(lt\|gt(old, v), old, v)` |
| `__atomic_test_and_set` / `__atomic_clear` | `update` to 1 / `write` of 0 (a `void *` pointee is `u8`) |
| generic `__atomic_load/store/exchange` | the same nodes through the pointer arguments |
| `__c11_atomic_init(p, v)` | non-atomic `write` |
| compare-exchange | `compare_exchange<T, weak=, success=, failure=>(place, expected, desired)` |
| `*_thread_fence` / `*_signal_fence` | `fence<scope=thread\|signal, order=o>` |
| `__sync_fetch_and_OP` / `__sync_OP_and_fetch` | `update`, `result=old`/`new`, `atomic=seq_cst` |
| `__sync_bool/val_compare_and_swap` | `compare_exchange<T, form=success\|old, weak=false, success=seq_cst, failure=seq_cst>` |
| `__sync_lock_test_and_set` / `__sync_swap` | `update<T, result=old>`, `acquire` / `seq_cst` |
| `__sync_lock_release` | `write<T, atomic=release>` of 0 |
| `__sync_synchronize` | `fence<scope=thread, order=seq_cst>` |
| lock-free queries | `const<bool>` when decidable, else a call to libatomic `__atomic_is_lock_free` |

### Orderings and arguments

- Orderings are recorded as written, including UB ones (an acquire store, a
  failure stronger than success); `consume` stays distinct. A constant folds
  to its name; a non-constant prints `atomic=dynamic(v)`. A non-constant gcc
  `weak` prints `weak=dynamic(v)`.
- `__scoped_atomic_*` adds `sync_scope=` (`device`, `workgroup`,
  `wavefront`, `single`, `cluster`, `dynamic(v)`; `system` is the default
  and never printed). The C scope is kept, not the target's mapping (clang
  drops it on CPUs, maps it on `amdgcn`). The family has no
  `signal_fence`, `test_and_set`, `clear`, `init`, or lock-free query.
- `__sync_*` are `seq_cst` except `lock_test_and_set` (acquire, per gcc;
  clang strengthens it) and `lock_release` (release). Trailing "protected
  variable" arguments are ignored. Size-suffixed spellings
  (`__sync_fetch_and_add_1` .. `_16`) lower like the unsuffixed ones, the
  width coming from the pointee.
- Metadata: `c_builtin` names the builtin (an `update` no longer names
  itself); `c_macro` names the innermost macro, so `<stdatomic.h>` users see
  `atomic_fetch_add_explicit` even through a wrapper macro.

### Arithmetic

- Fetch arithmetic is in `T` with `overflow=wrap` (C11 7.17.7.5 has no UB
  results). On a pointer, `__c11_atomic_fetch_add` offsets in elements and
  `__atomic_*`/`__sync_*` in bytes (`element=u8`).
- Float `add`/`sub` fetches use ambient float semantics. Float `min`/`max`
  are `minnum`/`maxnum` (compare-and-select would answer NaN for a NaN `v`);
  `__atomic_fetch_fminimum`/`fmaximum` and `_num` variants map to
  `minimum`/`maximum` and `minimum_num`/`maximum_num`, float objects only.
  `__sync_fetch_and_min/max` stay integer-only.
- `__atomic_fetch_uinc`/`udec` (LLVM `uinc_wrap`/`udec_wrap`): `uinc` is
  `ge(old, v) ? 0 : add(old, 1)`, `udec` is
  `logical_or(eq(old, 0), gt(old, v)) ? v : sub(old, 1)`, compared unsigned
  through `reinterpret`, as `__sync_fetch_and_umin/umax` do.
  `__sync_fetch_and_min/max` compare signed.
- Names outside a family's set (`__atomic_uinc_fetch`,
  `__c11_atomic_fetch_fminimum`) are not builtins and fall through to
  ordinary name resolution.
- `_Bool` fetch arithmetic runs on its storage byte as `update<u8, ..>`
  (clang's `atomicrmw add i8`); the operand converts to `bool` then widens,
  and the result converts back. Reading a byte above 1 differs from clang's
  low-bit truncation, but that object is a trap representation.
  `load`/`store`/`exchange`/`test_and_set`/`clear` stay `bool`.

### Lock-free queries

`__c11_atomic_is_lock_free(n)`, `__atomic_is_lock_free(n, p)`, and
`__atomic_always_lock_free(n, p)` follow clang's evaluator: true when `n` is a
power of two no wider than `TargetInfo::max_atomic_inline_bytes` (16 on
AArch64 and x86-64 with `cx16`, else 8) and `n == 1`, `p` is null, or `p`'s
pointee is aligned to `n`. Otherwise `always_lock_free` is false and the
others call libatomic's `__atomic_is_lock_free`, declared on first use.

Fixtures: `ir_atomic_builtins.c`, `ir_atomic_stdatomic.c`, `ir_atomic_sync.c`,
`ir_atomic_sync_gcc.c`, `ir_atomic_extensions.c`, `ir_atomic_scoped.c`,
`ir_atomic_lock_free.c`, plus AArch64 and `-mcx16` variants.
