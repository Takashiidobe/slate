# Architecture and principles

<!-- toc -->
- [Pipeline](#pipeline)
- [Checker first, lowering trusts](#checker-first-lowering-trusts)
- [What goes in the IR](#what-goes-in-the-ir)
- [`Unimplemented`](#unimplemented)
- [Strictness policy](#strictness-policy)
- [Priorities](#priorities)
- [Oracles and inputs](#oracles-and-inputs)
<!-- /toc -->

The decisions every change is judged against. The detail lives in the
linked pages.

## Pipeline

```text
argv → Dialect → preprocess → parse → sema: names → check → lower → IR → ../slate → Rust
```

- One run means one configuration: flavor, standard, target, and flags
  ([configuration-threading](configuration-threading.md)).
- slate-parser emits the IR, and Slate (`../slate`) turns it into Rust. We
  own the IR, so we also own that boundary.

## Checker first, lowering trusts

- The checker (`sema/assertion.rs` and friends) decides whether a
  declaration or expression is valid. Put as much logic as possible there.
- Lowering assumes checked input. A failure there is an `Internal` error,
  which marks a gap in the checker, never a user diagnostic.
- When a rule exists in both places today, the fix is to move it into the
  checker or into a rule function both share. Don't grow a second copy.

## What goes in the IR

- Emit something only if the Rust side can use it.
- A source construct that changes semantic meaning must be represented in
  the IR. This is source-to-source, not an optimizer, so dropping it is a
  miscompile.
- If Rust can already express it through `extern` (for example an MSVC
  `fastcall` import), pass it through and let Slate handle it.
- If Rust cannot express it directly, the IR must carry the resolved
  meaning, so that the Rust lowering stays mechanical. Example: gcc and
  clang give `_Atomic` aggregates different layouts and access rules, so
  the IR emits a different shape per flavor instead of a bare "atomic".

## `Unimplemented`

`ResolveError::Unimplemented` is like LLVM's NYI: a construct we will
model but haven't yet. It is not a rejection. A fixture that hits it
records work still to do, not accepted behavior.

## Strictness policy

- Too strict is a bug: if gcc, clang, or MSVC accepts the code with a
  given flag set, slate-parser must accept it with the same flags. The
  priority depends on how common the construct is.
- Too permissive is fine. Accepting code that one of the three rejects is
  P4 at most, since users compile their code with a real compiler first.
- Error fixtures (`tests/fixtures/error/`) assert only that the input is
  rejected. Matching a compiler's wording is not a goal.

## Priorities

- Get the clang flavor mostly complete first.
- The yardstick is real-world C. zstd is a deliberately hard target.
  Parse it under clang, then under gcc and MSVC.
- Rerun the sweeps (`tools/gcc_dg_sweep.py`, `tools/llvm_lit_sweep.py`,
  `tools/corpus_sweep.py`) once a good share of an epic's beads is closed,
  not after every fix.

## Oracles and inputs

| Input | Source |
| --- | --- |
| clang | 22.1.8, native |
| gcc | 16.2, native |
| MSVC | `tools/cl.exe` ([msvc-oracle](msvc-oracle.md)), 19.51.36257 (VS 2026) for x86, x64 and ARM64 (`MSVC_ARCH=x86\|arm64`). Arm32 predefines come from 19.44.35228, the last toolset that targets Arm32. They are frozen, and no local compiler exists to recheck them |
| Predefined macros | `src/predefines/*.h`, captured by hand with each oracle's `-dM -E`. Macros that vary with arch, ISA, or version are removed from the snapshots and defined by `Preprocessor::configure` |
| Sysroots and compiler headers | `../slate-sysroots` (`cargo run -- install <triple>`, `install compiler-headers <flavor>`), found through `SLATE_SYSROOTS`. Fix broken or missing headers (for example missing intrinsics headers) there, not here |

Differences between versions of the same compiler are out of scope.
