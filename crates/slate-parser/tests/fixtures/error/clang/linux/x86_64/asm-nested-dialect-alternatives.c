void f(int x) {
  asm("{a{b|c}|d}" : : "r"(x));
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × nested assembly dialect alternatives
// PARSE: ╰─▶ nested assembly dialect alternatives
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/asm-nested-dialect-alternatives.c:2:7]
// PARSE: 1 │ void f(int x) {
// PARSE: 2 │   asm("{a{b|c}|d}" : : "r"(x));
// PARSE: ·       ────────────
// PARSE: 3 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
