svint32_t *pointer;
long distance(void) { return pointer + 1 - pointer; }

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × sizeless type does not have a fixed size
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/aarch64/sizeless-pointer-arithmetic.c:2:30]
// PARSE:  1 │ svint32_t *pointer;
// PARSE:  2 │ long distance(void) { return pointer + 1 - pointer; }
// PARSE:    ·                              ───────────
// PARSE:  3 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
