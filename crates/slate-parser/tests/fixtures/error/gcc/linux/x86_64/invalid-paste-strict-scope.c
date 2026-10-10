// SLATE-FILECHECK-ARGS -std=c17
// SLATE-FILECHECK-ERROR PARSE

#define PASTE(a, b) a##b
int x = PASTE(:, :);

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × pasting ':' and ':' does not give a valid preprocessing token
// PARSE: ╰─▶ pasting ':' and ':' does not give a valid preprocessing token
// PARSE: ╭─[tests/fixtures/error/gcc/linux/x86_64/invalid-paste-strict-scope.c:3:15]
// PARSE: 2 │ #define PASTE(a, b) a##b
// PARSE: 3 │ int x = PASTE(:, :);
// PARSE: ·               ────
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
