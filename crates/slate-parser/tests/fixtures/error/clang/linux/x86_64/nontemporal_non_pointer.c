// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR

void store(int x) { __builtin_nontemporal_store(1, x); }

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × semantic analysis failed
// ERROR: Error:
// ERROR:   × address argument to nontemporal builtin must be a pointer
// ERROR:    ╭─[tests/fixtures/error/clang/linux/x86_64/nontemporal_non_pointer.c:2:21]
// ERROR:  1 │
// ERROR:  2 │ void store(int x) { __builtin_nontemporal_store(1, x); }
// ERROR:    ·                     ─────────────────────────────────
// ERROR:    ╰────
// SLATE-FILECHECK-END ERROR
