// SLATE-FILECHECK-ERROR PARSE

struct s { int v; };

static int pick(int x) __attribute__((overloadable)) { return 1; }
static int pick(double x) __attribute__((overloadable)) { return 2; }

int pick_struct(struct s v) {
  return pick(v);
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE: × no matching function for call to overloaded function
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/overloadable-no-match.c:8:10]
// PARSE: 7 │ int pick_struct(struct s v) {
// PARSE: 8 │   return pick(v);
// PARSE: ·          ───────
// PARSE: 9 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
