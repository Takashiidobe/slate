// SLATE-FILECHECK-ERROR PARSE

static int pick(int x) __attribute__((overloadable)) { return 1; }
static int pick(double x) __attribute__((overloadable)) { return 2; }

int pick_long(long l) {
  return pick(l);
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE: × call to overloaded function is ambiguous
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/overloadable-ambiguous.c:6:10]
// PARSE: 5 │ int pick_long(long l) {
// PARSE: 6 │   return pick(l);
// PARSE: ·          ───────
// PARSE: 7 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
