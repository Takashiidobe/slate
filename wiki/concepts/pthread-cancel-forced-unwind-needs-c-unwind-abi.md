# pthread_cancel forced unwind requires `extern "C-unwind"`, not `extern "C"`

Root cause of the `pthread_cancel-points` libc-test SIGABRT
(`slate-cu47`). Kept as a reference doc because this bites any generated
function that can be on the stack under a cancellable libc call, and the
symptom (silent `SIGABRT`, no diagnostic output) looks nothing like the
cause.

## Symptom

`tests/fixtures.libc-test/functional/supported/pthread_cancel-points.c`'s
generated Rust aborts with `SIGABRT` and **no output at all** — no panic
message, no glibc "Fatal glibc error" line, nothing on stderr. The abort
signature under gdb:

```
#0  pthread_kill ()
#1  raise ()
#2  abort ()
#3  ?? ()                          <- inside __pthread_unwind (nptl/unwind.c)
#4  pthread_testcancel ()
#5  sem_wait ()
#6  execute_sem_wait ()            <- generated Rust
#7  run_execute ()                 <- generated Rust
#8  ?? ()                          <- glibc thread startup trampoline
```

Initial suspicion (recorded in an earlier, since-corrected investigation)
was a nondeterministic race or a `sem_t` ABI/alignment mismatch. Both were
real leads worth ruling out but neither was the cause: alignment was
independently wrong and got fixed in `libc-shim/include/semaphore.h` (real
glibc `sem_t` is `union { char[32]; long }`, 8-byte aligned; libc-shim had
the musl-shaped 4-byte-aligned struct — a legitimate fix, kept, but
orthogonal to this crash), and the crash reproduces 100% deterministically,
not intermittently.

## Root cause

`execute_sem_wait` and `run_execute` are generated as plain `extern "C" fn`.
Per Rust's ABI contract (RFC 2945, the `C`/`C-unwind` split), a plain
`extern "C" fn` **asserts nothing unwinds across its frame boundary** — not
just Rust panics, any unwind, including a foreign one. The compiler can't
prove that statically (dead branches aren't provably dead at the ABI level),
so the codegen it emits is: if an unwind *does* reach this frame, run a
guard that calls `core::panicking::panic_cannot_unwind`, which aborts.

`pthread_cancel()` delivers cancellation via glibc's own foreign forced
unwind: `__pthread_unwind` (`nptl/unwind.c`) calls `_Unwind_ForcedUnwind()`,
which walks the stack frame by frame — including through `execute_sem_wait`
and `run_execute` — running any landing pad it finds along the way (that's
required, so C++/Rust cleanup code still runs during cancellation). It's a
foreign exception class, not a Rust panic, but Rust's landing pad doesn't
special-case that: it just sees "an unwind is crossing a non-`-unwind`
`extern "C"` boundary" and calls `panic_cannot_unwind`, which aborts.
`_Unwind_ForcedUnwind` therefore never completes and returns an error to
`__pthread_unwind`, whose own handling for that case is `abort()` — glibc's
side of the contract, "this should never happen." Neither abort call prints
anything, which is why the failure is silent.

Confirmed by diffing `rustc --emit=asm` for the function with and without a
plain (non-variadic, non-nightly) Rust call in the branch that's actually
dead at runtime:

```
.Lfunc_begin3:
	.cfi_startproc
	.cfi_personality 155, DW.ref.rust_eh_personality
	.cfi_lsda 27, .Lexception3
        ...
.LBB8_3:
.Ltmp11:
	callq	*_RNv...9panicking19panic_cannot_unwind@GOTPCREL(%rip)
```

That landing pad exists purely because the (unreached) branch calls an
ordinary Rust function (`CStr::as_ptr` in the real fixture, `plain_rust_fn`
in the minimal repro below) — any such call forces the compiler to assume
the frame could see an unwind. Remove the call and the landing pad — and the
crash — disappear. This has nothing to do with `c_variadic` or nightly
Rust; an earlier pass of this investigation chased `c_variadic` codegen as
the culprit because the fixture happened to use a variadic `t_printf`, but
the actual minimal reproducer needs neither nightly features nor variadics.

## Minimal reproducer

Stable Rust, `libc` as the only dependency, ~55 lines. Crashes deterministically
(5/5 runs, exit 134):

```rust
use std::ffi::c_void;

static mut SEM_SEQ: libc::sem_t = unsafe { std::mem::zeroed() };
static mut SEM_TEST: libc::sem_t = unsafe { std::mem::zeroed() };

const PTHREAD_CANCEL_ENABLE: i32 = 0;
const PTHREAD_CANCEL_DISABLE: i32 = 1;

extern "C" {
    fn pthread_setcancelstate(state: i32, oldstate: *mut i32) -> i32;
}

#[inline(never)]
fn plain_rust_fn(x: i32) -> i32 {
    x + 1
}

extern "C" fn execute_sem_wait(_arg: *mut c_void) {
    let r = unsafe { libc::sem_wait(std::ptr::addr_of_mut!(SEM_TEST)) };
    if r != 0 {
        let v = plain_rust_fn(r);
        eprintln!("sem_wait failed: {v}");
    }
}

extern "C" fn run_execute(_arg: *mut c_void) -> *mut c_void {
    unsafe {
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, std::ptr::null_mut());
        loop {
            let r = libc::sem_wait(std::ptr::addr_of_mut!(SEM_SEQ));
            if r == 0 {
                break;
            }
        }
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, std::ptr::null_mut());
        execute_sem_wait(std::ptr::null_mut());
    }
    std::ptr::null_mut()
}

fn main() {
    unsafe {
        libc::sem_init(std::ptr::addr_of_mut!(SEM_SEQ), 0, 0);
        libc::sem_init(std::ptr::addr_of_mut!(SEM_TEST), 0, 0);

        let mut thr: libc::pthread_t = std::mem::zeroed();
        libc::pthread_create(&mut thr, std::ptr::null(), run_execute, std::ptr::null_mut());

        libc::pthread_cancel(thr);
        libc::sem_post(std::ptr::addr_of_mut!(SEM_SEQ));

        let mut res: *mut c_void = std::ptr::null_mut();
        libc::pthread_join(thr, &mut res);

        libc::sem_destroy(std::ptr::addr_of_mut!(SEM_TEST));
        libc::sem_destroy(std::ptr::addr_of_mut!(SEM_SEQ));
    }
    println!("OK");
}
```

Also present, gitignored, at `diagnostics/rustc-extern-c-forced-unwind-repro/`
(local-only — the block above is the durable copy).

**The fix**: mark `execute_sem_wait` and `run_execute` (every frame between
the pthread entry point and the actual cancellation-point call) as
`extern "C-unwind" fn`. C and C-unwind share the same calling convention —
only unwind-table/landing-pad semantics differ — so a raw fn-pointer cast is
enough to hand a `C-unwind` function to an FFI signature expecting plain
`extern "C"` (e.g. `pthread_create`'s start routine parameter):

```rust
let start: extern "C" fn(*mut c_void) -> *mut c_void =
    std::mem::transmute(run_execute as extern "C-unwind" fn(*mut c_void) -> *mut c_void);
libc::pthread_create(&mut thr, std::ptr::null(), start, std::ptr::null_mut());
```

With both functions changed to `extern "C-unwind"`, the repro passes 5/5.

## Not a rustc bug

This is intended, documented `C`/`C-unwind` ABI behavior, not a compiler
defect: a plain `extern "C" fn` is contractually asserted not to unwind, and
letting an unwind (including a foreign one) silently pass through would be
UB, so aborting is the only sound response. `pthread_cancel`'s forced unwind
is exactly the kind of foreign-unwind-crossing-a-non-unwind-boundary case
RFC 2945 exists for.

## Why an "override list of known-cancellable functions" doesn't fully solve it

The tempting fix is: tag libc functions that are POSIX cancellation points
(`sem_wait`, `pthread_join`, `read`, `sleep`, ... — a standardized list from
`pthread_cancel(3)`) in slate's known-function table, and only mark
generated functions `extern "C-unwind"` if they transitively reach one.

That's exact and cheap for the *known-function* tier, but incomplete on its
own:

- Unwind-tolerance is a per-frame property. Every function between the
  cancellable call and the thread entry point needs it — not just the leaf
  that makes the call. (Confirmed empirically above: marking only
  `execute_sem_wait` and leaving `run_execute` alone still aborts, just one
  frame further up.)
- For **user-provided C code**, slate has no annotation to read from —
  reachability has to come from a closed-world reverse call-graph walk
  starting at the known-cancellation-point roots. That's possible in
  principle (slate has the whole program's call graph at translation time),
  but it isn't sound in the presence of function pointers / indirect calls
  (exactly what this fixture does — `execute` is stored in a struct and
  invoked through a `transmute`d function pointer) without conservatively
  assuming any indirect call could reach a cancellation point, which
  collapses back toward "almost everything" for real-world programs anyway.

## Decision

Mark every slate-generated `extern "C"` function definition as
`extern "C-unwind"` unconditionally, rather than trying to precisely compute
reachability. Rationale:

- Precise tagging is soundness-fragile (indirect calls, cross-TU calls) in
  exactly the way that would silently reintroduce this bug for programs
  slate hasn't been tested against.
- The performance cost is smaller than it sounds: `panic = "unwind"` builds
  already carry `.eh_frame` for every Rust function regardless of `C` vs
  `C-unwind` — the difference is only whether the `nounwind`-gated landing
  pad/abort guard exists, which is dead weight off the hot path unless an
  unwind actually happens.
- Matches the existing "transliterate first, idiomatize later" convention
  ([architecture.md](architecture.md)): get an unconditionally-correct
  baseline now; if profiling ever shows this mattering, a later verified
  fixup pass can narrow it via real call-graph reachability, the same way
  other idiomization fixups build on a correct raw-lowering baseline.

This only applies to **function definitions** slate emits (translated C
functions, pthread worker/callback functions). FFI **declarations** for
calling into real external C libraries (`extern "C" { fn sem_wait(...); }`)
are untouched — the crash is about the Rust-defined frame's own ABI
attribute, not about how its libc callees are declared.

## Investigation notes (tooling that worked)

- `strace -f -e trace=write,openat` ruled out the "libgcc_s.so.1 missing"
  fatal path in one shot (the dlopen it does succeeds; no diagnostic write
  ever happens), which killed a plausible-looking red herring fast.
- `objdump -d` on the whole `.text` section, anchored on a nearby *exported*
  symbol (`__pthread_unwind_next`) for alignment, let the unknown `??`
  frames in the gdb backtrace be read directly as real glibc source shape
  (`_Unwind_ForcedUnwind(...); abort();`) without needing matching debug
  symbols for this system's glibc build (a custom znver4 rebuild with no
  matching debuginfod entry).
- `rustc --emit=asm`, diffed between a crashing and passing variant of the
  same function, was the decisive tool — it shows `.cfi_personality`,
  `.cfi_lsda`, and the literal `panic_cannot_unwind` landing pad directly,
  which is far more legible than diffing post-link `.eh_frame` bytes.
- A blind bisection on "how many of N dead call sites are real" (in the
  original, much larger generated fixture) gave a deterministic 10-vs-11
  threshold before the real cause was understood — worth noting as a trap:
  the sharp threshold looked like it implicated a specific call site count
  or size boundary, but was actually just "does *this* function have a
  Rust-call-shaped landing pad or not," which happened to correlate with
  count in that specific file's call ordering.
