// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

const int count = 3;
int array[count];

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × variable length array with static storage duration
// SEMANTIC: ╭─[tests/fixtures/error/gcc/linux/x86_64/const_object_array_bound.c:3:5]
// SEMANTIC: 2 │ const int count = 3;
// SEMANTIC: 3 │ int array[count];
// SEMANTIC: ·     ────────────
// SEMANTIC: 4 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
