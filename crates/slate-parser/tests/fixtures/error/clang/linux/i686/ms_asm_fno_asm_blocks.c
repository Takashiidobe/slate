// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR
// SLATE-FILECHECK-ARGS -fasm-blocks -fno-asm-blocks

int value;

void set_value(void) {
    __asm { mov value, 1 }
}

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × expected `;`
// ERROR: ╰─▶ expected `;`
// ERROR: ╭─[tests/fixtures/error/clang/linux/i686/ms_asm_fno_asm_blocks.c:5:5]
// ERROR: 4 │ void set_value(void) {
// ERROR: 5 │     __asm { mov value, 1 }
// ERROR: ·     ─────
// ERROR: 6 │ }
// ERROR: ╰────
// SLATE-FILECHECK-END ERROR
