svint32_t values[2];

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × array elements cannot have a sizeless type
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/aarch64/sizeless-array.c:1:1]
// PARSE:  1 │ svint32_t values[2];
// PARSE:    · ────────────────────
// PARSE:  2 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
