unsigned long size = sizeof(svint32_t);

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × sizeless type does not have a fixed size
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/aarch64/sizeless-sizeof.c:1:22]
// PARSE:  1 │ unsigned long size = sizeof(svint32_t);
// PARSE:    ·                      ─────────────────
// PARSE:  2 │
// PARSE:    ╰────
// PARSE: Error:
// PARSE:   × sizeless type does not have a fixed size
// PARSE:    ╭─[tests/fixtures/error/gcc/linux/aarch64/sizeless-sizeof.c:1:22]
// PARSE:  1 │ unsigned long size = sizeof(svint32_t);
// PARSE:    ·                      ─────────────────
// PARSE:  2 │
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
