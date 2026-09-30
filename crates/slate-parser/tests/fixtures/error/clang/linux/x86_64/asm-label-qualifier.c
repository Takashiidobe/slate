extern int value asm volatile("name");

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × meaningless `volatile` on asm outside function
// PARSE: ╰─▶ meaningless `volatile` on asm outside function
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/asm-label-qualifier.c:1:22]
// PARSE: 1 │ extern int value asm volatile("name");
// PARSE: ·                      ────────
// PARSE: 2 │
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
