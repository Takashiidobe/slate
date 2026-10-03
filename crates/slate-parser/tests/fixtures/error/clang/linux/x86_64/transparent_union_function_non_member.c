// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

typedef union {
  int  *i;
  long *l;
} Argument __attribute__((transparent_union));

int take(Argument);
int (*q)(char *) = take;

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error: -Wincompatible-pointer-types
// SEMANTIC: × incompatible pointer types
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/transparent_union_function_non_member.c:8:20]
// SEMANTIC: 7 │ int take(Argument);
// SEMANTIC: 8 │ int (*q)(char *) = take;
// SEMANTIC: ·                    ────
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
