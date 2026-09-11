int main( {
  return 3;
}

// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × expected `)`
// DEFAULT: ╰─▶ expected `)`
// DEFAULT: ╭─[tests/fixtures/parse_errors.c:1:11]
// DEFAULT: 1 │ int main( {
// DEFAULT: ·           ─
// DEFAULT: 2 │   return 3;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
