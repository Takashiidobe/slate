# atoi/atol/atoll Tier B prelude helper + handwritten filecheck

_created 2026-08-29_

Tier B of the numeric-parse lift (slate-y0qs.6.6): when an ato\* arg is not a
compile-time literal (Tier A can't fold), lift the call to a pure-Rust prelude
helper instead of leaving libc.

- `backend/engine/rules/libc_call.rs`: `ato_helper` emits `__slate_atoi(arg as
*const i8)` (i32) / `__slate_atol` (i64, shared by atol+atoll) as the
  `.or_else` fallback after `fold_atoi`. Redundant `*const i8` cast is elided.
- `backend/engine/prelude.rs` (new): post-pass `inject(program)` scans emitted
  calls (via `Stmt::collect_calls`, which only records `Expr::Var` callees — so
  the helper call must use `Var`, not `Path`) and splices the needed helper
  `Item::Fn`s in once. Helper reads the C string with `CStr::from_ptr` + byte
  scan (C isspace incl 0x0b, optional sign, digit run) then `from_utf8(..).parse`.
- Injection runs at backend `apply()`, unlike memchr which injects at frontend.

Tooling: `tools/update_filecheck.py` now skips a profile whose directives exist
outside the SLATE-FILECHECK markers (handwritten wins; logs the skip). The
gnu_libc_algorithms rewrites block is now handwritten (asserts only the
\_\_slate_atoi idiom). Also taught `strip-ai-comments.py` (claude hooks) to
preserve `// SLATE-` marker lines so Edits don't drop BEGIN/END markers.

Flake: cross_tu tests serialized via nextest `[test-groups] cross-tu
{max-threads=1}` — they contend on cargo build resources under load.

rewrites profile 110/110.
