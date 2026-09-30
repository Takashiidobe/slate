int utf16_multiple = u'ab';
int wide_multiple = L'ab';
int utf16_too_large = u'\U0001F600';
int utf8_too_large = u8'Ω';

// SLATE-FILECHECK-ERROR SEMA
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

// SLATE-FILECHECK-BEGIN SEMA
// SEMA: Error:   × semantic analysis failed
// SEMA: Error:
// SEMA: × Unicode character literals may not contain multiple characters
// SEMA: ╭─[tests/fixtures/error/clang/linux/x86_64/character_constant_diagnostics.c:1:22]
// SEMA: 1 │ int utf16_multiple = u'ab';
// SEMA: ·                      ─────
// SEMA: 2 │ int wide_multiple = L'ab';
// SEMA: ╰────
// SEMA: Error:
// SEMA: × wide character literals may not contain multiple characters
// SEMA: ╭─[tests/fixtures/error/clang/linux/x86_64/character_constant_diagnostics.c:2:21]
// SEMA: 1 │ int utf16_multiple = u'ab';
// SEMA: 2 │ int wide_multiple = L'ab';
// SEMA: ·                     ─────
// SEMA: 3 │ int utf16_too_large = u'\U0001F600';
// SEMA: ╰────
// SEMA: Error:
// SEMA: × character too large for enclosing character literal type
// SEMA: ╭─[tests/fixtures/error/clang/linux/x86_64/character_constant_diagnostics.c:3:23]
// SEMA: 2 │ int wide_multiple = L'ab';
// SEMA: 3 │ int utf16_too_large = u'\U0001F600';
// SEMA: ·                       ─────────────
// SEMA: 4 │ int utf8_too_large = u8'Ω';
// SEMA: ╰────
// SEMA: Error:
// SEMA: × character too large for enclosing character literal type
// SEMA: ╭─[tests/fixtures/error/clang/linux/x86_64/character_constant_diagnostics.c:4:22]
// SEMA: 3 │ int utf16_too_large = u'\U0001F600';
// SEMA: 4 │ int utf8_too_large = u8'Ω';
// SEMA: ·                      ─────
// SEMA: 5 │
// SEMA: ╰────
// SLATE-FILECHECK-END SEMA
