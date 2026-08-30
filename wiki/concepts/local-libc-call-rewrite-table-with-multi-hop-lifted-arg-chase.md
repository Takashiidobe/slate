# Local libc-call rewrite table with multi-hop lifted-arg chase

_created 2026-08-29_

Generalized `backend/engine/rules/libc_call.rs` from the fixed exit/abort
shape into a flat table of local rewrites, one entry per `Known`, each a
`Box<dyn Fn(&CallCtx) -> Option<Expr>>`. `None` leaves the raw call in place.
This is the intended replacement for the local-consumption half of `facts/`
for calls whose whole effect is local to args+return (malloc/free excluded —
global effects).

`CallCtx::lifted_arg(i, want)` is the core accessor. The pointer-lattice pass
(`interproc/pointer_lattice.rs`) lifts a binding's type but leaves a raw-ptr
view at call sites so unrewritten calls still compile, so at e.g. a `strlen`
site the arg is not `arg.as_ptr()` directly but a cast around a shim local
`let bytes: *mut u8 = arg2.as_ptr() as *mut u8`. `lifted_arg` therefore does a
multi-hop chase: peel casts/`as_ptr`/`as_mut_ptr`/empty-unsafe → land on a
`Var` → if its type already satisfies `want`, done; if it's a raw-ptr shim and
not reassigned, hop through its `let` initializer and recurse. Reassignment is
rejected via `def_use_neighbors` (Assign/CompoundAssign targeting the name).

Supporting change: `arena.rs` gained a `defs: HashMap<Ident, NodeId>` name→def
index (maintained in `fill`/`take`) plus `definition()`. `def_uses` is a
readers list and never contains a binding's own defining node, so it can't
answer "what is this binding's declared type/init" — the chase needs the def.

Registered for `Expr`/`Let`/`Return` kinds (calls are hoisted to temps, so the
call sits at the head of an init/return, reachable via the existing
`call_anchor` machinery). Rewrite replaces only the inner `Call` node, leaving
surrounding casts/`unsafe` (harmless, covered by the `unused` allow).

Shipped entries: exit, abort, strlen (`-> s.len()`). Verified e2e on
`ptr_len_str_worklist` differential. Follow-ups (slate-y0qs.6): strcmp/strcpy
family, and mem\*/memset which need result-value/node handling (they return
`dst` into a typed let) — a distinct shape from the value-returning chase.
