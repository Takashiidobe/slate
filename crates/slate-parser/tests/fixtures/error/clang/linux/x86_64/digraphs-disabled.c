// SLATE-FILECHECK-ARGS -std=c99 -fno-digraphs
// SLATE-FILECHECK-ERROR PARSE

int array <: 2 :>;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `;`
// PARSE: ╰─▶ expected `;`
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/digraphs-disabled.c:2:11]
// PARSE: 1 │
// PARSE: 2 │ int array <: 2 :>;
// PARSE: ·           ─
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
