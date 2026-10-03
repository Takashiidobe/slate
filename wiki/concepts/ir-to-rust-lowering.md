# IR-to-Rust lowering

<!-- toc -->
- [Types and storage](#types-and-storage)
- [Atomics](#atomics)
- [Control flow](#control-flow)
- [Translation units](#translation-units)
<!-- /toc -->

## Types and storage

| Parser IR | Rust |
| --- | --- |
| Incomplete array | `[T; 0]`, retaining element alignment |
| `vla<T, %e>` (pointee) | `[T; 0]`; only reached through raw pointers |
| VLA local | Zero-filled `Vec<E>` of the innermost fixed element, sized by the captured extents; the place is `*(v.as_mut_ptr() as *mut [T; 0])` |
| Flexible record member | Zero-length field at the parser-provided offset |
| Flexible-array decay | Raw field address cast to an element pointer |
| C function pointer | `Option<unsafe extern "C-unwind" fn(...) -> R>` |
| Variadic C function pointer | Fixed parameters followed by `...` |
| `storage=thread` global | `#[thread_local] static mut` (crate `#![feature(thread_local)]`); exported ones add `#[unsafe(no_mangle)]`, imports are `#[thread_local]` statics in the extern block |
| Global, local, or dispatch slot whose `align` exceeds its type's natural alignment | `__SlateAlignN<T>` (`#[repr(C, align(N))]` newtype); the place is `.0`. Includes the x86-64 ABI 16-byte alignment of arrays of 16+ bytes, which clang's vector loads rely on (`object_alignment.c`); extern declarations stay unwrapped |
| `[visibility=default]` global | Same as no attribute (an exported static already has default visibility); hidden/protected/internal remain barriers |
| Const-qualified address | `&raw const`; mutable addresses use `&raw mut` |
| Volatile read / write | `ptr::read_volatile` / `ptr::write_volatile` on raw addresses |
| f32 / f64 classification | Float classification methods; zero compares equal to either signed zero |
| Scalar `__builtin_clz*` / `ctz*` / `popcount*` calls | Unsigned operand's `leading_zeros()` / `trailing_zeros()` / `count_ones()`, cast to the C result type |
| `__builtin_expect`, `expect_with_probability`, `unpredictable` | The first operand (IR operands are side-effect free) |
| `__builtin_prefetch` | Empty block |
| `__builtin_abort`, `nan*`, `memcpy`, `memmove`, `memset`, `memcmp` | Extern with the library `link_name` |
| `target("...")` function | `#[target_feature(enable = ...)] unsafe fn` (clang names mapped to rustc: `bmi`→`bmi1`, `pclmul`→`pclmulqdq`, `cx16`→`cmpxchg16b`, `rdrnd`→`rdrand`); `rtm` adds `#![feature(rtm_target_feature)]` |
| `__builtin_cpu_supports("x")` / `__builtin_cpu_init()` | `std::arch::is_x86_feature_detected!("x")` / empty block |
| f80 classification | `__slate_f80_is_fp_class(x, llvm fpclass mask)` / `__slate_f80_signbit`; signaling-NaN tests remain barriers |
| Infinity constructors | `f32::INFINITY` / `f64::INFINITY` / x87 +inf `LongDouble` literal |
| NaN constructors | libc `nan` / `nanf` / `nanl`, preserving payload and argument evaluation |

- Record size, alignment, offsets, and pointer strides must match the C layout.
- Aggregate initialization must account for zero-length flexible fields even
  though C does not provide initializers for them.
- Static address constants may use recursively constant arithmetic for pointer
  offsets and indices; aggregate members follow the same rule.
- Explicit function addresses and function decay share lowering and ABI tracking.
- Indirect variadic calls use the parser's explicit default argument promotions.
- Variadic function-pointer arguments travel as raw pointer representations;
  `VaArg` reads that representation and restores the nullable function-pointer type.
- VLA storage lives until the enclosing Rust block ends, so a VLA in a loop body
  is freed each iteration as in C. Decay and `addr_of` of a VLA place cast the
  storage pointer, never a reference to the zero-length array. `ptr_offset` and
  `ptr_diff` over a VLA element use `byte_offset` / byte `offset_from` with a
  runtime stride: the captured extents times the fixed element size
  (`vla_locals.c`). A VLA local in a goto-dispatched function is a barrier: its
  slot would never be freed.
- Null-based field-address differences use parser layout offsets, avoiding Rust
  null dereferences and pointer arithmetic.
- Leading-zero counts preserve the C operand width; zero input is undefined in C.
- A call to an extern codegen-only builtin with neither a lowering nor a
  library name is a barrier: clang expands those in codegen, so the symbol
  never links (and `lowering-barriers` would otherwise report the function as
  ok). Codegen-only means any `__builtin_*` name, a `c_builtin_kind` other than
  `library`, or a `c_builtin_header` ending in `intrin.h` (`_mm_pause`,
  `__readfsdword`); `codegen_only_builtin_extern.c` is unsupported.
- `target` features: `no-x`, `tune=`, `branch-protection=`, and `default` are
  ignored (codegen only); `arch=` and features without a rustc mapping are
  barriers. Target functions are `unsafe fn` because callers may lack the feature.
- Volatile record fields keep ordinary C storage layout; qualification belongs
  to the memory operation. Volatile bit-field access remains a barrier.
- A record is `#[repr(C, packed(A))]`, A = the record's IR alignment, when an
  ordinary field's natural alignment exceeds A or its offset is misaligned
  (`packed`, `#pragma pack`, packed members). Fields are placed at
  `min(field_align, A)` plus explicit padding. Packed field updates are read
  and write of the copied value, never references. Still barriers: packed
  plus a raised record alignment (`packed, aligned(N)`; Rust rejects
  `packed` with `align`), and packed records transitively containing a
  `repr(align)` record (E0588).

## Atomics

| IR | Rust |
| --- | --- |
| Scalar atomic read / write | `Atomic*::from_ptr(address).load(order)` / `.store(value, order)` |
| Function-pointer atomic access | `AtomicPtr<u8>` with representation-preserving transmutation |
| Consume ordering | Acquire |
| Thread / signal fence | `atomic::fence` / `atomic::compiler_fence` |
| Relaxed fence | Empty block |
| Volatile atomic access | Nightly `Atomic*::from_ptr_raw` and `load_volatile` / `store_volatile` |
| 128-bit atomic access on Linux | Opaque calls to libatomic load/store entry points |

- Preserve byte width and raw-pointer storage, including const pointer values.
- Volatile atomic operations retain the requested ordering and remain observable
  when their result is unused; raw APIs avoid creating references to I/O memory.
- `f32` / `f64` atomics use `AtomicU32` / `AtomicU64` on the bit representation
  (`to_bits` / `from_bits`), for access, updates, and compare-exchange.
- `update<T, result=old|new, atomic=o>(place, f(old))`: when `f` is
  `add|sub|and|or|xor(old, x)` (add/and/or/xor also commuted), `not(and(old, x))`,
  or ignores `old`, x is bound once and the update is `fetch_add` / `fetch_sub` /
  `fetch_and` / `fetch_or` / `fetch_xor` / `fetch_nand` / `swap` (clang's
  `atomicrmw`). `result=new` recombines the old value and x (`wrapping_*` for
  add/sub). Any other `f` (mul, shifts, floats, pointer offsets, `_Atomic`
  compound assignment) is a `fetch_update` CAS loop with `old` bound to the
  closure argument; the loop's fetch order drops the release half. It applies
  only when `f` is pure (constants, `old`, conversions, arithmetic, compares,
  conditionals, pointer offsets, non-volatile non-atomic reads), since the loop
  re-evaluates `f`.
- `compare_exchange` / `compare_exchange_weak`: `form=write_back` assigns the
  observed value through the expected pointer only on failure; `success` is
  `is_ok()`; `old` returns either variant's value.
- Dynamic orders and weakness, non-system scopes, volatile updates, atomic
  aggregates, fn-pointer and 128-bit updates, and impure non-native update
  computations remain barriers.

## Control flow

- Source: `crates/slate/src/frontend/lowerer/control_flow.rs`.
- Input: slate-parser structured statements and resolved binding/loop/switch IDs.
- Functions with ordinary labels/gotos use a function-local CFG and Rust
  `loop { match state { ... } }` dispatch; other functions retain structured lowering.
- Switches with statements before their first case also use dispatch: direct
  case entry skips those statements and their initializers.
- Before emission, thread forwarding jumps, prune unreachable nodes, and coalesce
  straight-line chains into basic blocks with one terminator each.
- Function entry, joins, and branch/switch successors start blocks. Forwarding
  cycles retain a self-loop; effectful cycles retain their operations and backedges.

| IR | CFG behavior |
| --- | --- |
| Label / goto | One shared target state per label ID |
| If | Conditional successors |
| While / do-while | Condition effects repeat at the condition entry |
| For | Continue enters increment, then condition effects |
| Switch / case / default | Discriminant evaluated once; cases enter their original nested positions |
| Break / continue | Resolved owner ID selects the successor |

- Every local and synthetic temporary has hoisted `MaybeUninit<T>` storage
  and a stable raw pointer; parameters retain their existing representation.
- Declaration initializers execute in their original states through `ptr::write`;
  a jump past a declaration skips initialization.
- Raw slot pointers avoid creating fresh mutable references that could
  invalidate C pointers retained across states.
- CFG construction borrows parser IR and preserves source sites for ordinary
  statement lowering. Computed goto and asm goto remain unsupported.
- Dispatch can retain costly copies of locals at the common loop merge even in
  release builds; inspect optimized code before relying on elimination.
- Retained Rust-to-Rust structuring and proposed tail-call optimization are in
  [control-flow rewrites](rewrite-engine-v2.md#control-flow-rewrites).
- Differential fixtures cover forward/backward jumps, nested and sibling loops,
  irreducible control flow, jumps into switch cases, cross-state locals, skipped
  initializers, and condition/increment effects.

## Translation units

- Give generated Rust modules their own namespace and retain source filenames
  with `#[path]`; C tags may share a translation unit's filename.
- Collect feature requirements from all units onto the Rust crate root.
