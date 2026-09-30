enum E { A = 1 [[deprecated]] };

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unexpected token `LBracket`
// DEFAULT: ╰─▶ unexpected token `LBracket`
// DEFAULT: ╭─[tests/fixtures/error/clang/linux/x86_64/enumerator_attribute_after_value.c:1:14]
// DEFAULT: 1 │ enum E { A = 1 {{\[\[}}deprecated]] };
// DEFAULT: ·              ─
// DEFAULT: 2 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
