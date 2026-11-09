// SLATE-FILECHECK-DEFINES ASM_IMAG ASM_IMAG
// SLATE-FILECHECK-DEFINES ASM_RVALUE ASM_RVALUE
// SLATE-FILECHECK-DEFINES ASSIGN_IMAG ASSIGN_IMAG
// SLATE-FILECHECK-DEFINES FIXED_KEYWORD FIXED_KEYWORD
// SLATE-FILECHECK-DEFINES FIXED_LITERAL FIXED_LITERAL
// SLATE-FILECHECK-ERROR ASM_IMAG
// SLATE-FILECHECK-ERROR ASM_RVALUE
// SLATE-FILECHECK-ERROR ASSIGN_IMAG
// SLATE-FILECHECK-ERROR FIXED_KEYWORD
// SLATE-FILECHECK-ERROR FIXED_LITERAL
// SLATE-FILECHECK-ARGS --dump-ir

#ifdef FIXED_KEYWORD
_Fract f;
#endif
#ifdef FIXED_LITERAL
int size = sizeof(0.5hr);
#endif

void invalid(int i, _Complex int c) {
#ifdef ASM_IMAG
  __asm__("" : "=r"(__imag__ i));
#endif
#ifdef ASM_RVALUE
  __asm__("" : "=r"(__imag__ (c + c)));
#endif
#ifdef ASSIGN_IMAG
  __imag__ i = 1;
#endif
}

// SLATE-FILECHECK-BEGIN ASM_IMAG
// ASM_IMAG: Error:   × semantic analysis failed
// ASM_IMAG: Error:
// ASM_IMAG:   × invalid lvalue in asm output
// ASM_IMAG:     ╭─[tests/fixtures/error/clang/linux/x86_64/real_imag_invalid.c:11:3]
// ASM_IMAG:  10 │ #ifdef ASM_IMAG
// ASM_IMAG:  11 │   __asm__("" : "=r"(__imag__ i));
// ASM_IMAG:     ·   ───────────────────────────────
// ASM_IMAG:  12 │ #endif
// ASM_IMAG:     ╰────
// SLATE-FILECHECK-END ASM_IMAG
// SLATE-FILECHECK-BEGIN ASM_RVALUE
// ASM_RVALUE: Error:   × semantic analysis failed
// ASM_RVALUE: Error:
// ASM_RVALUE:   × invalid lvalue in asm output
// ASM_RVALUE:     ╭─[tests/fixtures/error/clang/linux/x86_64/real_imag_invalid.c:14:21]
// ASM_RVALUE:  13 │ #ifdef ASM_RVALUE
// ASM_RVALUE:  14 │   __asm__("" : "=r"(__imag__ (c + c)));
// ASM_RVALUE:     ·                     ────────────────
// ASM_RVALUE:  15 │ #endif
// ASM_RVALUE:     ╰────
// SLATE-FILECHECK-END ASM_RVALUE
// SLATE-FILECHECK-BEGIN ASSIGN_IMAG
// ASSIGN_IMAG: Error:   × semantic analysis failed
// ASSIGN_IMAG: Error:
// ASSIGN_IMAG:   × expression is not assignable
// ASSIGN_IMAG:     ╭─[tests/fixtures/error/clang/linux/x86_64/real_imag_invalid.c:17:3]
// ASSIGN_IMAG:  16 │ #ifdef ASSIGN_IMAG
// ASSIGN_IMAG:  17 │   __imag__ i = 1;
// ASSIGN_IMAG:     ·   ──────────────
// ASSIGN_IMAG:  18 │ #endif
// ASSIGN_IMAG:     ╰────
// SLATE-FILECHECK-END ASSIGN_IMAG
// SLATE-FILECHECK-BEGIN FIXED_KEYWORD
// FIXED_KEYWORD: Error:   × semantic analysis failed
// FIXED_KEYWORD: Error:
// FIXED_KEYWORD:   × unknown type name `_Fract`
// FIXED_KEYWORD:    ╭─[tests/fixtures/error/clang/linux/x86_64/real_imag_invalid.c:3:1]
// FIXED_KEYWORD:  2 │ #ifdef FIXED_KEYWORD
// FIXED_KEYWORD:  3 │ _Fract f;
// FIXED_KEYWORD:    · ──────
// FIXED_KEYWORD:  4 │ #endif
// FIXED_KEYWORD:    ╰────
// SLATE-FILECHECK-END FIXED_KEYWORD
// SLATE-FILECHECK-BEGIN FIXED_LITERAL
// FIXED_LITERAL: Error:   × semantic analysis failed
// FIXED_LITERAL: Error:
// FIXED_LITERAL:   × invalid suffix 'hr' on floating constant
// FIXED_LITERAL:    ╭─[tests/fixtures/error/clang/linux/x86_64/real_imag_invalid.c:6:19]
// FIXED_LITERAL:  5 │ #ifdef FIXED_LITERAL
// FIXED_LITERAL:  6 │ int size = sizeof(0.5hr);
// FIXED_LITERAL:    ·                   ─────
// FIXED_LITERAL:  7 │ #endif
// FIXED_LITERAL:    ╰────
// SLATE-FILECHECK-END FIXED_LITERAL
