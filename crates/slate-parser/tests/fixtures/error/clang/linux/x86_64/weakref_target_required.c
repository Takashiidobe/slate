// SLATE-FILECHECK-DEFINES BARE BARE
// SLATE-FILECHECK-ERROR BARE
// SLATE-FILECHECK-DEFINES SPLIT SPLIT
// SLATE-FILECHECK-ERROR SPLIT
// SLATE-FILECHECK-DEFINES DEFINITION DEFINITION
// SLATE-FILECHECK-ERROR DEFINITION
// SLATE-FILECHECK-DEFINES INITIALIZED INITIALIZED
// SLATE-FILECHECK-ERROR INITIALIZED

extern int target(void);

#ifdef BARE
static int reference(void) __attribute__((weakref));
#endif

#ifdef SPLIT
static int reference(void) __attribute__((weakref));
extern int reference(void) __attribute__((alias("target")));
#endif

#ifdef DEFINITION
static int __attribute__((weakref("target"))) reference(void) { return 0; }
#endif

#ifdef INITIALIZED
static int __attribute__((weakref, alias("target"))) reference = 0;
#endif

// SLATE-FILECHECK-BEGIN BARE
// BARE: Error:   × semantic analysis failed
// BARE: Error:
// BARE: × weakref declaration must also have an alias attribute
// BARE: ╭─[tests/fixtures/error/clang/linux/x86_64/weakref_target_required.c:5:12]
// BARE: 4 │ #ifdef BARE
// BARE: 5 │ static int reference(void) __attribute__((weakref));
// BARE: ·            ────────────────────────────────────────
// BARE: 6 │ #endif
// BARE: ╰────
// SLATE-FILECHECK-END BARE
// SLATE-FILECHECK-BEGIN SPLIT
// SPLIT: Error:   × semantic analysis failed
// SPLIT: Error:
// SPLIT: × weakref declaration must also have an alias attribute
// SPLIT: ╭─[tests/fixtures/error/clang/linux/x86_64/weakref_target_required.c:9:12]
// SPLIT: 8 │ #ifdef SPLIT
// SPLIT: 9 │ static int reference(void) __attribute__((weakref));
// SPLIT: ·            ────────────────────────────────────────
// SPLIT: 10 │ extern int reference(void) __attribute__((alias("target")));
// SPLIT: ╰────
// SLATE-FILECHECK-END SPLIT
// SLATE-FILECHECK-BEGIN DEFINITION
// DEFINITION: Error:   × semantic analysis failed
// DEFINITION: Error:
// DEFINITION: × weakref declaration cannot be a definition
// DEFINITION: ╭─[tests/fixtures/error/clang/linux/x86_64/weakref_target_required.c:14:1]
// DEFINITION: 13 │ #ifdef DEFINITION
// DEFINITION: 14 │ static int __attribute__((weakref("target"))) reference(void) { return 0; }
// DEFINITION: · ───────────────────────────────────────────────────────────────────────────
// DEFINITION: 15 │ #endif
// DEFINITION: ╰────
// SLATE-FILECHECK-END DEFINITION
// SLATE-FILECHECK-BEGIN INITIALIZED
// INITIALIZED: Error:   × semantic analysis failed
// INITIALIZED: Error:
// INITIALIZED: × weakref declaration cannot be a definition
// INITIALIZED: ╭─[tests/fixtures/error/clang/linux/x86_64/weakref_target_required.c:18:54]
// INITIALIZED: 17 │ #ifdef INITIALIZED
// INITIALIZED: 18 │ static int __attribute__((weakref, alias("target"))) reference = 0;
// INITIALIZED: ·                                                      ─────────────
// INITIALIZED: 19 │ #endif
// INITIALIZED: ╰────
// SLATE-FILECHECK-END INITIALIZED
