int main( {
  return 3;
}

// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × expected declaration type, found LBrace
// DEFAULT: ╰─▶ expected declaration type, found LBrace
// DEFAULT: ╭─[tests/fixtures/error/parse_errors.c:2:3]
// DEFAULT: 1 │ int main( {
// DEFAULT: 2 │   return 3;
// DEFAULT: ·   ──────
// DEFAULT: 3 │ }
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
