// SLATE-FILECHECK-ERROR PARSE

#line 18446744073709551616
int x;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × #line number out of range
// PARSE: ╰─▶ #line number out of range
// PARSE: ╭─[tests/fixtures/error/msvc/windows/x86_64/line-out-of-range.c:2:7]
// PARSE: 1 │
// PARSE: 2 │ #line 18446744073709551616
// PARSE: ·       ────────────────────
// PARSE: 3 │ int x;
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
