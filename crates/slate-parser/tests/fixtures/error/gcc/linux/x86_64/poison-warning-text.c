#pragma GCC poison warned_name
#warning warned_name


// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × attempt to use a poisoned identifier
// PARSE:   ╰─▶ attempt to use a poisoned identifier
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/x86_64/poison-warning-text.c:2:10]
// PARSE:  1 │ #pragma GCC poison warned_name
// PARSE:  2 │ #warning warned_name
// PARSE:    ·          ───────────
// PARSE:  3 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
