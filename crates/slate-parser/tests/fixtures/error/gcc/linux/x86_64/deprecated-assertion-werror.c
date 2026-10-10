// SLATE-FILECHECK-ARGS -std=gnu99 -Werror=deprecated
// SLATE-FILECHECK-ERROR PARSE

#assert machine(slate)
#if #machine(slate)
int x;
#endif

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × preprocessing failed
// PARSE: Error: -Wdeprecated
// PARSE: × '#assert' is a deprecated GCC extension
// PARSE: ╭─[tests/fixtures/error/gcc/linux/x86_64/deprecated-assertion-werror.c:2:2]
// PARSE: 1 │
// PARSE: 2 │ #assert machine(slate)
// PARSE: ·  ──────
// PARSE: 3 │ #if #machine(slate)
// PARSE: ╰────
// PARSE: Error: -Wdeprecated
// PARSE: × assertions are a deprecated extension
// PARSE: ╭─[tests/fixtures/error/gcc/linux/x86_64/deprecated-assertion-werror.c:3:5]
// PARSE: 2 │ #assert machine(slate)
// PARSE: 3 │ #if #machine(slate)
// PARSE: ·     ─
// PARSE: 4 │ int x;
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
