# OpenMP lowering

<!-- toc -->

- [Runtimes](#runtimes)
- [Parallel regions](#parallel-regions)
- [Data sharing](#data-sharing)
- [Testing](#testing)
- [Not yet lowered](#not-yet-lowered)
<!-- /toc -->

How slate lowers the IR's OpenMP regions to Rust. The parser side (clauses,
data-sharing resolution, checker rules) is in
[ir/control-flow](ir/control-flow.md#openmp-regions). Work is tracked under the
`slate-3amj.24` epic.

## Runtimes

| Flavor     | Runtime                            | Entry point     |
| ---------- | ---------------------------------- | --------------- |
| gcc, clang | libgomp (`#[link(name = "gomp")]`) | `GOMP_parallel` |
| msvc       | vcomp140 (`slate-3amj.24.12`)      | `_vcomp_fork`   |

Only the runtime entry points must match the C compiler. The data block
passed to the runtime is private to slate and need not match gcc's
`.omp_data_s` layout. Until the vcomp lowering lands, a region on an msvc
target is a lowering barrier.

## Parallel regions

`frontend/support/libgomp.rs` (module `__slate_libgomp`, emitted only when a
region is lowered) wraps `GOMP_parallel` in a generic `parallel<F: Fn()>`. Its
`extern "C"` trampoline casts the data pointer back to `&F` and calls it. The
ABI is `"C"`, not `"C-unwind"`: a panic escaping a region aborts, as an
exception escaping a structured block terminates in OpenMP.

`lowerer/openmp.rs` lowers a region to

```rust
{
    let __slate_omp_<shared> = std::ptr::addr_of_mut!(x);
    let __slate_omp_first_<copy> = std::ptr::addr_of!(f);
    __slate_libgomp::parallel(threads, || unsafe {
        let mut f: i32 = 0i32;
        f = std::ptr::read(__slate_omp_first_<copy>);
        /* body, with x spelled (*__slate_omp_<shared>) */
    });
}
```

- `threads` is `num_threads` or `0` (the runtime default, `OMP_NUM_THREADS`).
  An `if` clause makes it `if cond { threads } else { 1 }`.
- The closure is not `move` and must be `Fn`: it only captures the raw
  pointers by shared reference. If a later change makes the body mutate a
  captured local directly, the closure becomes `FnMut` and the Rust fails to
  compile instead of racing silently.
- A body with labels or `goto` gets its own goto dispatch
  (`control_flow::needs_dispatch`) inside the closure. The function-level
  dispatch does not descend into regions, since jumps cannot cross the region
  boundary.

## Data sharing

- **Shared** variables are captured as raw pointers taken once, outside the
  closure, and every use in the body goes through them (`omp_shared` in
  `FunctionLowerer`, consulted by `lower_binding` and `vla_address`). Taking
  `&mut` per thread would alias `&mut` across threads, which is UB even when
  the C program is race-free. A shared VLA is captured by its element pointer.
- A nested region takes its pointer from the enclosing one
  (`addr_of_mut!(*__slate_omp_N)`), so no reference is created there either.
- Globals and static locals are not captured; the body names the static.
- **Private** copies are declared inside the closure through the ordinary
  `Let` lowering, so they start zeroed (the C value is indeterminate).
- **Firstprivate** copies are then assigned with `std::ptr::read` from a
  pointer to the original taken outside the closure, which works for any type
  and for globals.

## Testing

The differential harness runs both the C oracle and the translated binary with
`OMP_NUM_THREADS=4` (`run_with_emulator` in `tests/support/mod.rs`). Fixtures
must print results that do not depend on scheduling, for example by writing
per-thread slots indexed by `omp_get_thread_num()`.

`crates/slate/tests/fixtures/gcc/openmp_parallel_gcc.c` covers default and
explicit team sizes, `if`, private, firstprivate (scalar and struct), shared
scalars, VLAs and parameters, static locals, `goto` inside a region, and
nested regions. clang flavor has no runtime oracle: `clang -fopenmp` needs
libomp, which is not installed.

The sysroot's gcc compiler headers do not include `omp.h` yet, so fixtures
declare the `omp_*` functions they call.

## Not yet lowered

`for`, `reduction`, the synchronization directives and `atomic` are separate
children of `slate-3amj.24`.
