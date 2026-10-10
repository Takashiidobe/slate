// SLATE-FILECHECK-ERROR PARSE

int spaced = 1 + \ 
2;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `;`
// PARSE: ╰─▶ expected `;`
// PARSE: ╭─[tests/fixtures/error/msvc/windows/x86_64/line-splice-whitespace.c:3:1]
// PARSE: 2 │ int spaced = 1 + \
// PARSE: 3 │ 2;
// PARSE: · ─
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
