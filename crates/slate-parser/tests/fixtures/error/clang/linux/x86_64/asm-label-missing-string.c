extern int value asm(name);

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected string literal in `asm`
// PARSE: ╰─▶ expected string literal in `asm`
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/asm-label-missing-string.c:1:22]
// PARSE: 1 │ extern int value asm(name);
// PARSE: ·                      ────
// PARSE: 2 │
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
