// SLATE-FILECHECK-ARGS -std=gnu99 -Werror=traditional -Wno-deprecated -Wno-long-long
// SLATE-FILECHECK-ERROR PARSE

#assert x(x)
int ok = 1;

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error: -Wtraditional
// PARSE: × suggest hiding '#assert' from traditional C with an indented '#'
// PARSE: ╭─[tests/fixtures/error/gcc/linux/x86_64/traditional-werror.c:2:2]
// PARSE: 1 │
// PARSE: 2 │ #assert x(x)
// PARSE: ·  ──────
// PARSE: 3 │ int ok = 1;
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
