#pragma clang poison clang_poisoned
#define clang_poisoned 1


// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × attempt to use a poisoned identifier
// PARSE:   ╰─▶ attempt to use a poisoned identifier
// PARSE:    ╭─[tests/fixtures/error/clang/linux/x86_64/poison-clang-namespace.c:2:9]
// PARSE:  1 │ #pragma clang poison clang_poisoned
// PARSE:  2 │ #define clang_poisoned 1
// PARSE:    ·         ──────────────
// PARSE:  3 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
