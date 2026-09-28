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
// ASM_TO_C: Error:   × unresolved label name `done`
// SLATE-FILECHECK-END ASM_TO_C
// SLATE-FILECHECK-BEGIN C_TO_ASM
// C_TO_ASM: Error:   × unresolved label name `inside`
// SLATE-FILECHECK-END C_TO_ASM
