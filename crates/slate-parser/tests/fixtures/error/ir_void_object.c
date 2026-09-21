// SLATE-FILECHECK-DEFINES FILE_SCOPE -DFILE_SCOPE
// SLATE-FILECHECK-DEFINES STATIC -DSTATIC
// SLATE-FILECHECK-DEFINES BLOCK -DBLOCK
// SLATE-FILECHECK-DEFINES TYPEDEF -DTYPEDEF
// SLATE-FILECHECK-ERROR FILE_SCOPE
// SLATE-FILECHECK-ERROR STATIC
// SLATE-FILECHECK-ERROR BLOCK
// SLATE-FILECHECK-ERROR TYPEDEF
// SLATE-FILECHECK-ARGS --dump-ir

#ifdef FILE_SCOPE
void bad;
#endif
#ifdef STATIC
static void bad;
#endif
#ifdef BLOCK
void f(void) { void bad; (void)bad; }
#endif
#ifdef TYPEDEF
typedef void alias;
alias bad;
#endif

// SLATE-FILECHECK-BEGIN FILE_SCOPE
// FILE_SCOPE: Error:   × semantic analysis failed
// FILE_SCOPE: Error:
// FILE_SCOPE: × object cannot have type void
// FILE_SCOPE: ╭─[tests/fixtures/error/ir_void_object.c:3:1]
// FILE_SCOPE: 2 │ #ifdef FILE_SCOPE
// FILE_SCOPE: 3 │ void bad;
// FILE_SCOPE: · ─────────
// FILE_SCOPE: 4 │ #endif
// FILE_SCOPE: ╰────
// SLATE-FILECHECK-END FILE_SCOPE
// SLATE-FILECHECK-BEGIN STATIC
// STATIC: Error:   × semantic analysis failed
// STATIC: Error:
// STATIC: × object cannot have type void
// STATIC: ╭─[tests/fixtures/error/ir_void_object.c:6:1]
// STATIC: 5 │ #ifdef STATIC
// STATIC: 6 │ static void bad;
// STATIC: · ────────────────
// STATIC: 7 │ #endif
// STATIC: ╰────
// SLATE-FILECHECK-END STATIC
// SLATE-FILECHECK-BEGIN BLOCK
// BLOCK: Error:   × invalid in this context: object cannot have type void
// SLATE-FILECHECK-END BLOCK
// SLATE-FILECHECK-BEGIN TYPEDEF
// TYPEDEF: Error:   × invalid in this context: object cannot have type void
// SLATE-FILECHECK-END TYPEDEF
