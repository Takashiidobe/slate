// SLATE-FILECHECK-ERROR PARSE

struct counted {
  int n;
  long name[] __attribute__((counted_by(n)));
};

void cast_argument(struct counted *p) {
  __builtin_counted_by_ref((long *)p->name);
}

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE: × '__builtin_counted_by_ref' argument must be a flexible array or pointer
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/counted-by-ref-not-member.c:8:3]
// PARSE: 7 │ void cast_argument(struct counted *p) {
// PARSE: 8 │   __builtin_counted_by_ref((long *)p->name);
// PARSE: ·   ─────────────────────────────────────────
// PARSE: 9 │ }
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
