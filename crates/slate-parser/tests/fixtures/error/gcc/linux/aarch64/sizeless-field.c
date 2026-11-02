struct holder { svint32_t value; };

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × sizeless type does not have a fixed size
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/aarch64/sizeless-field.c:1:1]
// PARSE:  1 │ struct holder { svint32_t value; };
// PARSE:    · ───────────────────────────────────
// PARSE:  2 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
