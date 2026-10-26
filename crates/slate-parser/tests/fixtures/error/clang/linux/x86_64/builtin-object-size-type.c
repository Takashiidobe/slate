// SLATE-FILECHECK-ERROR SEMA

unsigned long size(char *p) { return __builtin_object_size(p, 4); }

// SLATE-FILECHECK-BEGIN SEMA
// SEMA: Error:   × semantic analysis failed
// SEMA: Error:
// SEMA:   × object size type is outside the valid range [0, 3]
// SEMA:    ╭─[tests/fixtures/error/clang/linux/x86_64/builtin-object-size-type.c:2:38]
// SEMA:  1 │
// SEMA:  2 │ unsigned long size(char *p) { return __builtin_object_size(p, 4); }
// SEMA:    ·                                      ───────────────────────────
// SEMA:    ╰────
// SLATE-FILECHECK-END SEMA
