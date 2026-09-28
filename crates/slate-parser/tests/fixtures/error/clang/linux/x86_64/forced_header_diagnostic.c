int value;

// SLATE-FILECHECK-ARGS -include tests/fixtures/inputs/forced_inputs/failing_header.h
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × #error forced header failed
// PARSE: ╭─[tests/fixtures/inputs/forced_inputs/failing_header.h:1:1]
// PARSE: 1 │ #error forced header failed
// PARSE: · ───────────────────────────
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
