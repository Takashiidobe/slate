// SLATE-FILECHECK-DEFINES DEFAULT

/* PR c/5615 */
void f(int a, struct {int b[a];} c) {}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × unsupported scalar storage layout for vla<i32, *>
// SLATE-FILECHECK-END DEFAULT
