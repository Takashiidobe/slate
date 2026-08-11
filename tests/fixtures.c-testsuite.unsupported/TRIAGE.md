case_00018, case_00019, case_00052, case_00053: cannot find type `S`/`T` in scope — a typedef's name is dropped/not emitted in the generated Rust, so a later `TypeName varname = ...` reference is dangling.

case_00051: undeclared label `'__dispatch0` — generated Rust references a dispatch loop label the translator never defines; switch/loop lowering bug.

case_00133, case_00134, case_00135: literal out of range for `i32` — an unsigned constant like `4294967295` (UINT_MAX) is emitted without a type suffix and inferred as `i32` instead of the C source's unsigned type, so the literal overflows.

case_00136: `#ifdef`/`#undef`/`#error` nesting — clang's preprocessor never reaches the `#error bad branch` line, but the generated Rust contains a literal `compile_error!("bad branch")`, so slate's branch selection for this directive nest disagrees with clang's.

case_00143: Duff's device (case labels interleaved inside a `do { ... } while` body) — switch/loop lowering produces wrong runtime behavior (C exits 0, Rust exits 1) rather than a build failure.

case_00159: `None.unwrap()()` — a NULL function-pointer call site recovered as `Option<fn>` loses the concrete function type, so type inference on the call fails.

case_00170: mismatched types passing an enum through a helper expecting a pointer-sized int — enum representation/cast bug.

case_00186: `sprintf(buf, "->%02d<-\n", n)` — zero-padded width in an sprintf format string; matches the printf width/precision gap already tracked as a known limitation. Rust binary is killed by a signal (no exit code) rather than mismatching output.

case_00189: `static mut fprintfptr: Option<extern "C" fn(*mut FILE, *mut i8) -> i32> = Some(fprintf)` — function-pointer-typed global initialized from `fprintf`'s real (variadic) signature; the recovered type doesn't match.

case_00198: `h::e as i32` — an enum type or anonymous enum member is emitted as a path expression that resolves to a module instead of the enum, so the enum's own name doesn't resolve.

case_00199: `goto done;` skipping over a nested block's locals — Rust panics at runtime (exit 101) instead of matching C's exit 0; goto-across-scope handling bug.

case_00204: `static mut hfa11: hfa11 = hfa11 { ... }` — a global variable and its anonymous-struct-derived type end up with the same identifier, which collides in Rust's single namespace (legal in C's separate tag/ordinary namespaces).

case_00207: VLA (`char test[argc]`) combined with a `goto` into code that's dead on first read but reachable via the label — Rust panics at runtime (exit 101).

case_00209: `int (*fp)()` called with one argument — pre-C23 K&R "unspecified parameters" semantics; slate always parses with `-std=c23`, under which `()` means zero parameters, so clang itself rejects the call before slate ever sees CIR. Not fixable without also supporting an older `-std=` mode.

case_00216: `anon_0` — nested anonymous struct/union field naming resolution bug; same family as the already-tracked slate-nk3.52.1 anonymous struct/union naming issue.
