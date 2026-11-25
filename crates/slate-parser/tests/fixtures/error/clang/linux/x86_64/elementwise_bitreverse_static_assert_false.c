// SLATE-FILECHECK-ERROR PARSE

_Static_assert(__builtin_elementwise_bitreverse((unsigned char)0x12) == 0x49, "wrong byte");

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE:   × static assertion failed: wrong byte
// PARSE:    ╭─[tests/fixtures/error/clang/linux/x86_64/elementwise_bitreverse_static_assert_false.c:2:16]
// PARSE:  1 │
// PARSE:  2 │ _Static_assert(__builtin_elementwise_bitreverse((unsigned char)0x12) == 0x49, "wrong byte");
// PARSE:    ·                ─────────────────────────────────────────────────────────────
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
