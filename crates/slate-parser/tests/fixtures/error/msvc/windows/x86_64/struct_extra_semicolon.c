// SLATE-FILECHECK-ERROR PARSE

struct s {
  int x;
  ;
};

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected declaration type, found Semi
// PARSE: ╰─▶ expected declaration type, found Semi
// PARSE: ╭─[tests/fixtures/error/msvc/windows/x86_64/struct_extra_semicolon.c:4:3]
// PARSE: 3 │   int x;
// PARSE: 4 │   ;
// PARSE: ·   ─
// PARSE: 5 │ };
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
