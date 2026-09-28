// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

int f(void) {
  __asm mov eax, [eax * 2 + ebx * 4]
  return 0;
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × invalid in this context: `__asm` operand scales two registers
// DEFAULT: ╭─[tests/fixtures/sema/i686-pc-windows-msvc/ms_asm_two_scaled_registers.c:3:3]
// DEFAULT: 2 │ int f(void) {
// DEFAULT: 3 │   __asm mov eax, [eax * 2 + ebx * 4]
// DEFAULT: ·   ──────────────────────────────────
// DEFAULT: 4 │   return 0;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
