struct record {
  int field asm("field_name");
};

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `;` at end of declaration list
// PARSE: ╰─▶ expected `;` at end of declaration list
// PARSE: ╭─[tests/fixtures/error/asm-label-field.c:2:13]
// PARSE: 1 │ struct record {
// PARSE: 2 │   int field asm("field_name");
// PARSE: ·             ───
// PARSE: 3 │ };
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
