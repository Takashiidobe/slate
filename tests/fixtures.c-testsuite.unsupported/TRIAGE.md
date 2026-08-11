






case_00199: `goto done;` skipping over a nested block's locals — Rust panics at runtime (exit 101) instead of matching C's exit 0; goto-across-scope handling bug.

case_00204: `static mut hfa11: hfa11 = hfa11 { ... }` — a global variable and its anonymous-struct-derived type end up with the same identifier, which collides in Rust's single namespace (legal in C's separate tag/ordinary namespaces).

case_00207: VLA (`char test[argc]`) combined with a `goto` into code that's dead on first read but reachable via the label — Rust panics at runtime (exit 101).

case_00209: `int (*fp)()` called with one argument — pre-C23 K&R "unspecified parameters" semantics; slate always parses with `-std=c23`, under which `()` means zero parameters, so clang itself rejects the call before slate ever sees CIR. Not fixable without also supporting an older `-std=` mode.

case_00216: `anon_0` — nested anonymous struct/union field naming resolution bug; same family as the already-tracked slate-nk3.52.1 anonymous struct/union naming issue.
