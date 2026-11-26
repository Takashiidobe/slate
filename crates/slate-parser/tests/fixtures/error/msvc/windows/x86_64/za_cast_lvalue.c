void update(void *p) { ((int *)p)++; }
void assign(void *p) { ((int *)p) = 0; }
int **address(void **p) { return &((int *)*p); }

// SLATE-FILECHECK-ARGS /Za /W4
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × expression is not assignable
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/za_cast_lvalue.c:1:24]
// PARSE:  1 │ void update(void *p) { ((int *)p)++; }
// PARSE:    ·                        ────────────
// PARSE:  2 │ void assign(void *p) { ((int *)p) = 0; }
// PARSE:    ╰────
// PARSE: Error:
// PARSE:   × expression is not assignable
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/za_cast_lvalue.c:2:24]
// PARSE:  1 │ void update(void *p) { ((int *)p)++; }
// PARSE:  2 │ void assign(void *p) { ((int *)p) = 0; }
// PARSE:    ·                        ──────────────
// PARSE:  3 │ int **address(void **p) { return &((int *)*p); }
// PARSE:    ╰────
// PARSE: Error:
// PARSE:   × address of an rvalue
// PARSE:    ╭─[tests/fixtures/error/msvc/windows/x86_64/za_cast_lvalue.c:3:34]
// PARSE:  2 │ void assign(void *p) { ((int *)p) = 0; }
// PARSE:  3 │ int **address(void **p) { return &((int *)*p); }
// PARSE:    ·                                  ────────────
// PARSE:  4 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
