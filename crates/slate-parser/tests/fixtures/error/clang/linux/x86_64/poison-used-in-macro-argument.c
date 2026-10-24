#define CALL(f) f()
#pragma GCC poison helper
int value = CALL(
    helper);


// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × attempt to use a poisoned identifier
// PARSE:   ╰─▶ attempt to use a poisoned identifier
// PARSE:    ╭─[tests/fixtures/error/clang/linux/x86_64/poison-used-in-macro-argument.c:4:5]
// PARSE:  3 │ int value = CALL(
// PARSE:  4 │     helper);
// PARSE:    ·     ──────
// PARSE:  5 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
