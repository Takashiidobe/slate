// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR

struct S { int x; };
void store(struct S *p, struct S s) { __builtin_nontemporal_store(s, p); }

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × semantic analysis failed
// ERROR: Error:
// ERROR:   × address argument to nontemporal builtin must be a pointer to integer,
// ERROR:    ╭─[tests/fixtures/error/clang/linux/x86_64/nontemporal_record_pointee.c:3:39]
// ERROR:  2 │ struct S { int x; };
// ERROR:  3 │ void store(struct S *p, struct S s) { __builtin_nontemporal_store(s, p); }
// ERROR:    ·                                       ─────────────────────────────────
// ERROR:    ╰────
// SLATE-FILECHECK-END ERROR
