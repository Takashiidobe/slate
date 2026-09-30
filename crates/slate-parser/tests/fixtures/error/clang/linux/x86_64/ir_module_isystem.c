// SLATE-FILECHECK-ERROR PARSE
// SLATE-FILECHECK-ISYSTEM tests/fixtures/inputs/error/ir-module-headers

#include <ir_module_required_error.h>

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × #error isystem header reached for IR fixture
// PARSE: ╭─[tests/fixtures/inputs/error/ir-module-headers/ir_module_required_error.h:1:1]
// PARSE: 1 │ #error isystem header reached for IR fixture
// PARSE: · ────────────────────────────────────────────
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
