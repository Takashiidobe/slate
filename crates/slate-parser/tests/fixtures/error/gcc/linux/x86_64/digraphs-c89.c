// SLATE-FILECHECK-ARGS -std=c89
// SLATE-FILECHECK-ERROR PARSE

int array <: 2 :>;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `;`
// PARSE: ╰─▶ expected `;`
// PARSE: ╭─[tests/fixtures/error/gcc/linux/x86_64/digraphs-c89.c:2:11]
// PARSE: 1 │
// PARSE: 2 │ int array <: 2 :>;
// PARSE: ·           ─
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
