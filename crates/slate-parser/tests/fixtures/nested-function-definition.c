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
// DEFAULT: Error:   × invalid in this context: function definition is not allowed here
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN DOUBLED
// DOUBLED: Error:   × invalid in this context: function definition is not allowed here
// SLATE-FILECHECK-END DOUBLED
