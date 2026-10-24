// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir

int __attribute__((visibility("bogus"))) bogus_visibility = 1;
__attribute__((visibility("bogus"))) int bogus_visibility_fn(void) { return 0; }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT:   × visibility
// DEFAULT:    ╭─[tests/fixtures/error/gcc/linux/x86_64/ir_visibility_bogus.c:2:42]
// DEFAULT:  1 │
// DEFAULT:  2 │ int __attribute__((visibility("bogus"))) bogus_visibility = 1;
// DEFAULT:    ·                                          ────────────────────
// DEFAULT:  3 │ __attribute__((visibility("bogus"))) int bogus_visibility_fn(void) { return 0; }
// DEFAULT:    ╰────
// DEFAULT: Error:
// DEFAULT:   × visibility
// DEFAULT:    ╭─[tests/fixtures/error/gcc/linux/x86_64/ir_visibility_bogus.c:3:1]
// DEFAULT:  2 │ int __attribute__((visibility("bogus"))) bogus_visibility = 1;
// DEFAULT:  3 │ __attribute__((visibility("bogus"))) int bogus_visibility_fn(void) { return 0; }
// DEFAULT:    · ────────────────────────────────────────────────────────────────────────────────
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
