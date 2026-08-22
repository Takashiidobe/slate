# setjmp / longjmp

`setjmp`/`longjmp` are one of the control-flow constructs in C that Rust has no
native equivalent for: a call that can return twice, once normally and once
again, with the same return slot, but a different value

## Why `returns_twice` exists, and why we can't lean on it

C compilers don't get this right by accident. `setjmp` is declared
`__attribute__((returns_twice))` (LLVM's `returns_twice` function
attribute), and that attribute exists specifically to stop the optimizer
from doing things that are normally always safe across a call: caching a
value in a register across the call site instead of reloading it, hoisting
a load above it, sinking a store below it, and so on. All of those
transforms assume control resumes exactly once, right after the call
returns, with nothing else able to observe or mutate state in between.
`returns_twice` tells LLVM that assumption doesn't hold here, so it has to
treat the call as a much harder optimization barrier, otherwise a
value the optimizer decided to keep live in a register across the `setjmp`
call could still hold the _pre-jump_ value after control resumes via
`longjmp`, silently producing the wrong answer with no crash to point at.

Rust gives us no way to attach that attribute to a call. There's no
`#[ffi_returns_twice]`, and declaring `setjmp` as an ordinary `extern "C"` function
and calling it doesn't make LLVM treat the call site specially just because
the C side has the attribute. the attribute has to be on the call, and Rust
has no syntax for it. So "translate `setjmp`/`longjmp` to Rust calling the
real libc `setjmp`/`longjmp` through FFI" isn't safe, because LLVM can
over-optimize and break your code.

## What the baseline lowering actually does

Given that, the CIR-level lowering doesn't attempt to model "returns twice"
at all. `cir.eh.setjmp` lowers to a constant `0` unconditionally, as far as
raw lowering is concerned, `setjmp` always looks like the first, normal
return.

## Recovering the common idiom instead of modeling every case

A general fix would be to CPS-transform the call graph: turn every
function reachable from inside a `setjmp` guard into one that can return an
early "we jumped" signal instead of its normal result, and thread that
signal back up through every intermediate caller by hand, since Rust can't
resume a stack frame the way `longjmp` does. That's a whole-program dataflow
problem. Every function transitively callable from the guarded region
would need a second return path, for a construct that in practice is
almost always used in one shape: `if (setjmp(buf) != 0) { recover } else {
normal path that may call longjmp(buf, ...) }`.

So instead of the general transform, a fixup pass (`SetjmpRecovery`)
pattern-matches exactly that idiom and rewrites it onto a mechanism Rust
already has for "the call underneath you does not return normally": stack
unwinding.

- The guard's normal path becomes a closure run through
  `std::panic::catch_unwind(AssertUnwindSafe(...))`.
- Every `longjmp(buf, val)` targeting a buffer in that guard becomes
  `std::panic::panic_any(Payload { value: val })`, where `Payload` is a
  per-buffer struct generated so distinct `setjmp` buffers can't be confused
  with each other's unwind payloads.
- The `catch_unwind` result's `Err` arm downcasts to that payload (falling
  back to `resume_unwind` if it's some other panic entirely) and runs the
  original recovery body with the longjmp'd value bound; the `Ok` arm
  continues normally, with `break`/`continue` that would have escaped the
  closure rewritten to signal codes the caller dispatches on afterward,
  since a closure can't `break` a label defined outside it.

Unwinding is a real, optimizer-aware control-flow mechanism in LLVM.
Unlike a bare `returns_twice` call, the compiler already knows a function
that can unwind might not return normally to its caller, so none of the
register-caching hazards above apply. This sidesteps the whole
`returns_twice` problem for the idiom it covers, instead of trying to solve
it.

## The unwind-ABI DAG walk

`catch_unwind` only catches unwinds that are actually allowed to cross the
frames between the panic and the catch point. Every ordinary Rust `fn` (and
every `extern "C"` function, which is the default translation target) is
not unwind-safe to cross — a panic reaching that boundary aborts the
process instead of propagating, which would make the whole scheme UB at
the first frame in between. So once a `longjmp` is rewritten to `panic_any`,
every function on some call path between the `setjmp` guard and that
`longjmp` has to be re-signed `extern "C-unwind"` for the unwind to be
well-defined all the way through.

Figuring out which functions those are means walking the call graph
outward from the rewritten functions to a fixed point: start from the
functions containing the rewritten guard/`longjmp`, flip their ABI, then
repeatedly pull in anything that calls a now-`C-unwind` function directly,
anything reachable indirectly through a function pointer whose signature
now matches a flipped one (since the concrete callee behind an indirect
call isn't known statically), and any address-taken sibling function with
a matching C-ABI shape that could be substituted through the same pointer
type. That closure runs per translation unit and then again across the
whole project, so a `longjmp` that unwinds through a call into another
source file still gets a correctly-signed chain the whole way up.
