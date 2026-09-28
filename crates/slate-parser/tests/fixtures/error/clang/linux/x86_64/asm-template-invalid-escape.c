void f(int x) {
  asm("%!" : : "r"(x));
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid % escape in inline assembly string
// PARSE: ╰─▶ invalid % escape in inline assembly string
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/asm-template-invalid-escape.c:2:7]
// PARSE: 1 │ void f(int x) {
// PARSE: 2 │   asm("%!" : : "r"(x));
// PARSE: ·       ────
// PARSE: 3 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
