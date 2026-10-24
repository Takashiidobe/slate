#pragma GCC poison skipped_name
#if 1
#elif skipped_name
#endif


// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × attempt to use a poisoned identifier
// PARSE:   ╰─▶ attempt to use a poisoned identifier
// PARSE:    ╭─[tests/fixtures/error/clang/linux/x86_64/poison-elif-after-taken-branch.c:3:7]
// PARSE:  2 │ #if 1
// PARSE:  3 │ #elif skipped_name
// PARSE:    ·       ────────────
// PARSE:  4 │ #endif
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
