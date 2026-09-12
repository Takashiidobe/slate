#include "slate-missing-header.h"
int value;

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × header not found in search path: "slate-missing-header.h"
// PARSE: ╰─▶ header not found in search path: "slate-missing-header.h"
// PARSE: ╭─[tests/fixtures/missing-include.c:1:10]
// PARSE: 1 │ #include "slate-missing-header.h"
// PARSE: ·          ────────────────────────
// PARSE: 2 │ int value;
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
