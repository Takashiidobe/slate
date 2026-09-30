void f(int x) {
  asm goto("%l0" : : "r"(x) : : done);
done:
  return;
}

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × `%l` operand isn't a label
// PARSE: ╰─▶ `%l` operand isn't a label
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/asm-template-label-operand.c:2:12]
// PARSE: 1 │ void f(int x) {
// PARSE: 2 │   asm goto("%l0" : : "r"(x) : : done);
// PARSE: ·            ─────
// PARSE: 3 │ done:
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
