# IR-to-Rust lowering

## Types and storage

| Parser IR | Rust |
| --- | --- |
| Incomplete array | `[T; 0]`, retaining element alignment |
| Flexible record member | Zero-length field at the parser-provided offset |
| Flexible-array decay | Raw field address cast to an element pointer |
| C function pointer | `Option<unsafe extern "C-unwind" fn(...) -> R>` |
| Variadic C function pointer | Fixed parameters followed by `...` |
| Const-qualified address | `&raw const`; mutable addresses use `&raw mut` |
| Volatile read / write | `ptr::read_volatile` / `ptr::write_volatile` on raw addresses |
| f32 / f64 classification | Float classification methods; zero compares equal to either signed zero |
| NaN constructors | libc `nan` / `nanf` / `nanl`, preserving payload and argument evaluation |
| f32 / f64 infinity constructors | `f32::INFINITY` / `f64::INFINITY` |

- Record size, alignment, offsets, and pointer strides must match the C layout.
- Aggregate initialization must account for zero-length flexible fields even
  though C does not provide initializers for them.
- Static address constants may use recursively constant arithmetic for pointer
  offsets and indices; aggregate members follow the same rule.
- Explicit function addresses and function decay share lowering and ABI tracking.
- Indirect variadic calls use the parser's explicit default argument promotions.
- Volatile record fields keep ordinary C storage layout; qualification belongs
  to the memory operation. Volatile bit-field access remains a barrier.

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
- Dynamic orders, non-system scopes, atomic aggregates, updates, and compare-exchange
  remain barriers.

## Control flow

- Source: `crates/slate/src/frontend/lowerer/control_flow.rs`.
- Input: slate-parser structured statements and resolved binding/loop/switch IDs.
- Functions with ordinary labels/gotos use a function-local CFG and Rust
  `loop { match state { ... } }` dispatch; other functions retain structured lowering.
- Switches with statements before their first case also use dispatch: direct
  case entry skips those statements and their initializers.

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
- Differential fixtures cover forward/backward jumps, nested and sibling loops,
  irreducible control flow, jumps into switch cases, cross-state locals, skipped
  initializers, and condition/increment effects.
