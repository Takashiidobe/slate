#pragma GCC poison forbidden_identifier

int forbidden_identifier(void) { return 47; }

int main(void) { return forbidden_identifier(); }


// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × attempt to use a poisoned identifier
// PARSE:   ╰─▶ attempt to use a poisoned identifier
// PARSE:    ╭─[tests/fixtures/error/clang/linux/x86_64/poison-used.c:3:5]
// PARSE:  2 │
// PARSE:  3 │ int forbidden_identifier(void) { return 47; }
// PARSE:    ·     ────────────────────
// PARSE:  4 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
