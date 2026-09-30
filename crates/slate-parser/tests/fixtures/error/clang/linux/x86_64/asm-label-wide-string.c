extern int value asm(L"wide_name");

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × cannot use wide string literal in `asm`
// PARSE: ╰─▶ cannot use wide string literal in `asm`
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/asm-label-wide-string.c:1:22]
// PARSE: 1 │ extern int value asm(L"wide_name");
// PARSE: ·                      ────────────
// PARSE: 2 │
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
