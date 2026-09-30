// SLATE-FILECHECK-DEFINES REFERENCE REFERENCE
// SLATE-FILECHECK-DEFINES FORWARD FORWARD
// SLATE-FILECHECK-DEFINES DEFINITION DEFINITION
// SLATE-FILECHECK-ERROR REFERENCE
// SLATE-FILECHECK-ERROR FORWARD
// SLATE-FILECHECK-ERROR DEFINITION
// SLATE-FILECHECK-ARGS --dump-ir

#ifdef REFERENCE
struct S { int a; };
union S *p;
#endif

#ifdef FORWARD
union S;
struct S;
#endif

#ifdef DEFINITION
enum S;
struct S { int a; };
#endif

// SLATE-FILECHECK-BEGIN REFERENCE
// REFERENCE: Error:   × semantic analysis failed
// REFERENCE: Error:
// REFERENCE: × use of tag with a kind that does not match its previous declaration
// REFERENCE: ╭─[tests/fixtures/error/clang/linux/x86_64/tag_kind_mismatch.c:4:1]
// REFERENCE: 3 │ struct S { int a; };
// REFERENCE: 4 │ union S *p;
// REFERENCE: · ───────────
// REFERENCE: 5 │ #endif
// REFERENCE: ╰────
// SLATE-FILECHECK-END REFERENCE
// SLATE-FILECHECK-BEGIN FORWARD
// FORWARD: Error:   × semantic analysis failed
// FORWARD: Error:
// FORWARD: × use of tag with a kind that does not match its previous declaration
// FORWARD: ╭─[tests/fixtures/error/clang/linux/x86_64/tag_kind_mismatch.c:9:1]
// FORWARD: 8 │ union S;
// FORWARD: 9 │ struct S;
// FORWARD: · ─────────
// FORWARD: 10 │ #endif
// FORWARD: ╰────
// SLATE-FILECHECK-END FORWARD
// SLATE-FILECHECK-BEGIN DEFINITION
// DEFINITION: Error:   × semantic analysis failed
// DEFINITION: Error:
// DEFINITION: × use of tag with a kind that does not match its previous declaration
// DEFINITION: ╭─[tests/fixtures/error/clang/linux/x86_64/tag_kind_mismatch.c:14:1]
// DEFINITION: 13 │ enum S;
// DEFINITION: 14 │ struct S { int a; };
// DEFINITION: · ────────────────────
// DEFINITION: 15 │ #endif
// DEFINITION: ╰────
// SLATE-FILECHECK-END DEFINITION
