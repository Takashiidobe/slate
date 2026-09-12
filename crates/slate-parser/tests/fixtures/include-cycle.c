#include "include-cycle.h"
int value;

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × include cycle detected: tests/fixtures/include-cycle.h
// PARSE: ╰─▶ include cycle detected: tests/fixtures/include-cycle.h
// PARSE: ╭─[tests/fixtures/include-cycle.h:1:10]
// PARSE: 1 │ #include "include-cycle.h"
// PARSE: ·          ─────────────────
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
