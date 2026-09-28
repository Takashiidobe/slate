#include <quote_only.h>

// SLATE-FILECHECK-ARGS -iquote tests/fixtures/inputs/include_search/quote
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × header not found in search path: <quote_only.h>
// PARSE: ╰─▶ header not found in search path: <quote_only.h>
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/quote_only_angled.c:1:10]
// PARSE: 1 │ #include <quote_only.h>
// PARSE: ·          ──────────────
// PARSE: 2 │
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
