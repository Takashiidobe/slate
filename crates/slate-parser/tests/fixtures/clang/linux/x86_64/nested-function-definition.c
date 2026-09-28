void nested_outer(int n) {
#ifdef DOUBLE_INNER
  int inner(int x) {
    return x * 2;
  }
#else
  int inner(int x) {
    return x + 1;
  }
#endif
  inner(n);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES DOUBLED DOUBLE_INNER

// SLATE-FILECHECK-IR-ERROR DEFAULT
// SLATE-FILECHECK-IR-ERROR DOUBLED

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × invalid in this context: function definition is not allowed here
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/nested-function-definition.c:7:3]
// DEFAULT: 6 │     #else
// DEFAULT: 7 │ ╭─▶   int inner(int x) {
// DEFAULT: 8 │ │       return x + 1;
// DEFAULT: 9 │ ╰─▶   }
// DEFAULT: 10 │     #endif
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN DOUBLED
// DOUBLED: Error:   × semantic analysis failed
// DOUBLED: Error:
// DOUBLED: × invalid in this context: function definition is not allowed here
// DOUBLED: ╭─[tests/fixtures/clang/linux/x86_64/nested-function-definition.c:3:3]
// DOUBLED: 2 │     #ifdef DOUBLE_INNER
// DOUBLED: 3 │ ╭─▶   int inner(int x) {
// DOUBLED: 4 │ │       return x * 2;
// DOUBLED: 5 │ ╰─▶   }
// DOUBLED: 6 │     #else
// DOUBLED: ╰────
// SLATE-FILECHECK-END DOUBLED
