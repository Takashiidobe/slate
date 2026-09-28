int omega = 'Ω';
int escape = '\xff';
int multi = 'ab';

// SLATE-FILECHECK-ERROR SEMA
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

// SLATE-FILECHECK-BEGIN SEMA
// SEMA: Error:   × semantic analysis failed
// SEMA: Error:
// SEMA: × character too large for enclosing character literal type
// SEMA: ╭─[tests/fixtures/error/clang/linux/x86_64/clang_plain_character_constant.c:1:13]
// SEMA: 1 │ int omega = 'Ω';
// SEMA: ·             ───
// SEMA: 2 │ int escape = '\xff';
// SEMA: ╰────
// SLATE-FILECHECK-END SEMA
