int knr(a, b)
char a;
float b;
{
  return a;
}

// SLATE-FILECHECK-ERROR PARSE
// SLATE-FILECHECK-ARGS -std=c23

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × identifier lists in function definitions were removed in C23
// PARSE: ╰─▶ identifier lists in function definitions were removed in C23
// PARSE: ╭─[tests/fixtures/error/knr_definition_c23.c:1:1]
// PARSE: 1 │ int knr(a, b)
// PARSE: · ───
// PARSE: 2 │ char a;
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
