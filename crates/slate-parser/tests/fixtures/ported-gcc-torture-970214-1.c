void exit(int);

#define L 1
int main(void) { exit(L'1' != L'1'); }

// SLATE-FILECHECK-ERROR GCC

// SLATE-FILECHECK-BEGIN GCC
// GCC: Error:   × expected `}`
// GCC: ╰─▶ expected `}`
// GCC: ╭─[tests/fixtures/ported-gcc-torture-970214-1.c:4:16]
// GCC: 3 │ #define L 1
// GCC: 4 │ int main(void) { exit(L'1' != L'1'); }
// GCC: ·                ─
// GCC: 5 │
// GCC: ╰────
// SLATE-FILECHECK-END GCC
