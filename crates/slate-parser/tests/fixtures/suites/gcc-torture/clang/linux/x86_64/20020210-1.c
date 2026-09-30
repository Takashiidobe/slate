// SLATE-FILECHECK-DEFINES DEFAULT

/* PR c/5615 */
void f(int a, struct {int b[a];} c) {}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unsupported scalar storage layout for vla<i32, *>
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/clang/linux/x86_64/20020210-1.c:3:1]
// DEFAULT: 2 │ /* PR c/5615 */
// DEFAULT: 3 │ void f(int a, struct {int b[a];} c) {}
// DEFAULT: · ──────────────────────────────────────
// DEFAULT: 4 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
