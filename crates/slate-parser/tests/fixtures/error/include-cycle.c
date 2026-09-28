#include "include-cycle.h"
int value;

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × #include nested too deeply
// PARSE: ╰─▶ #include nested too deeply
// PARSE: ╭─[tests/fixtures/error/include-cycle.h:1:10]
// PARSE: 1 │ #include "include-cycle.h"
// PARSE: ·          ─────────────────
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
