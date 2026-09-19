// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

// Dropping _Atomic from a pointee: clang and gcc reject, MSVC accepts
// silently, so the consensus rule accepts it with a warning (slate-parser-hdt).
int *dropped(_Atomic int *value) { return value; }

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wincompatible-pointer-types
// WARN: ⚠ pointer conversion drops _Atomic from the pointee
// WARN: ╭─[tests/fixtures/sema/ir_atomic_pointee.c:4:43]
// WARN: 3 │ // silently, so the consensus rule accepts it with a warning (slate-parser-hdt).
// WARN: 4 │ int *dropped(_Atomic int *value) { return value; }
// WARN: ·                                           ─────
// WARN: 5 │
// WARN: ╰────
// SLATE-FILECHECK-END WARN
