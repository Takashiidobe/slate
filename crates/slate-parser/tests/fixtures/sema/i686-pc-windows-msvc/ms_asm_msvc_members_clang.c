// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-DEFINES UNTYPED UNTYPED
// SLATE-FILECHECK-DEFINES SCALAR SCALAR
// SLATE-FILECHECK-DEFINES TYPE_KEYWORD TYPE_KEYWORD
// SLATE-FILECHECK-IR-ERROR UNTYPED
// SLATE-FILECHECK-IR-ERROR SCALAR
// SLATE-FILECHECK-IR-ERROR TYPE_KEYWORD

struct wide { int w0; short half; char tail; int last; };
int global;

#if defined(UNTYPED)
int untyped(void) { __asm mov eax, [ebx].last }
#endif

#if defined(SCALAR)
int scalar(void) { __asm mov eax, global.last }
#endif

#if defined(TYPE_KEYWORD)
int type_keyword(void) { __asm mov eax, TYPE int }
#endif

// SLATE-FILECHECK-BEGIN UNTYPED
// UNTYPED: Error:   × semantic analysis failed
// UNTYPED: Error:
// UNTYPED: × invalid in this context: `__asm` member of an untyped operand
// UNTYPED: ╭─[tests/fixtures/sema/i686-pc-windows-msvc/ms_asm_msvc_members_clang.c:6:21]
// UNTYPED: 5 │ #if defined(UNTYPED)
// UNTYPED: 6 │ int untyped(void) { __asm mov eax, [ebx].last }
// UNTYPED: ·                     ─────────────────────────
// UNTYPED: 7 │ #endif
// UNTYPED: ╰────
// SLATE-FILECHECK-END UNTYPED
// SLATE-FILECHECK-BEGIN SCALAR
// SCALAR: Error:   × semantic analysis failed
// SCALAR: Error:
// SCALAR: × invalid in this context: no such struct or union member in `__asm`
// SCALAR: ╭─[tests/fixtures/sema/i686-pc-windows-msvc/ms_asm_msvc_members_clang.c:10:20]
// SCALAR: 9 │ #if defined(SCALAR)
// SCALAR: 10 │ int scalar(void) { __asm mov eax, global.last }
// SCALAR: ·                    ──────────────────────────
// SCALAR: 11 │ #endif
// SCALAR: ╰────
// SLATE-FILECHECK-END SCALAR
// SLATE-FILECHECK-BEGIN TYPE_KEYWORD
// TYPE_KEYWORD: Error:   × expected `__asm` operand
// TYPE_KEYWORD: ╰─▶ expected `__asm` operand
// TYPE_KEYWORD: ╭─[tests/fixtures/sema/i686-pc-windows-msvc/ms_asm_msvc_members_clang.c:14:46]
// TYPE_KEYWORD: 13 │ #if defined(TYPE_KEYWORD)
// TYPE_KEYWORD: 14 │ int type_keyword(void) { __asm mov eax, TYPE int }
// TYPE_KEYWORD: ·                                              ───
// TYPE_KEYWORD: 15 │ #endif
// TYPE_KEYWORD: ╰────
// SLATE-FILECHECK-END TYPE_KEYWORD
