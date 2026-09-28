// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-DEFINES ASM_TO_C ASM_TO_C
// SLATE-FILECHECK-DEFINES C_TO_ASM C_TO_ASM
// SLATE-FILECHECK-IR-ERROR ASM_TO_C
// SLATE-FILECHECK-IR-ERROR C_TO_ASM

#if defined(ASM_TO_C)
int asm_to_c(void) {
  __asm jmp done
done:
  return 0;
}
#endif

#if defined(C_TO_ASM)
int c_to_asm(void) {
  goto inside;
  __asm {
  inside:
    nop
  }
  return 0;
}
#endif

// SLATE-FILECHECK-BEGIN ASM_TO_C
// ASM_TO_C: Error:   × semantic analysis failed
// ASM_TO_C: Error:
// ASM_TO_C: × unresolved label name `done`
// ASM_TO_C: ╭─[tests/fixtures/sema/i686-pc-windows-msvc/ms_asm_cross_labels_clang.c:4:13]
// ASM_TO_C: 3 │ int asm_to_c(void) {
// ASM_TO_C: 4 │   __asm jmp done
// ASM_TO_C: ·             ────
// ASM_TO_C: 5 │ done:
// ASM_TO_C: ╰────
// SLATE-FILECHECK-END ASM_TO_C
// SLATE-FILECHECK-BEGIN C_TO_ASM
// C_TO_ASM: Error:   × semantic analysis failed
// C_TO_ASM: Error:
// C_TO_ASM: × unresolved label name `inside`
// C_TO_ASM: ╭─[tests/fixtures/sema/i686-pc-windows-msvc/ms_asm_cross_labels_clang.c:12:8]
// C_TO_ASM: 11 │ int c_to_asm(void) {
// C_TO_ASM: 12 │   goto inside;
// C_TO_ASM: ·        ──────
// C_TO_ASM: 13 │   __asm {
// C_TO_ASM: ╰────
// SLATE-FILECHECK-END C_TO_ASM
