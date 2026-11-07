// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-IR-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

int f(int n) {
    struct { char a[n]; int after; } x;
    return x.after;
}

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR:   × not implemented: variably modified member other than a trailing array
// IR:    ╭─[tests/fixtures/gcc/linux/x86_64/vla-record-member-not-trailing.c:3:5]
// IR:  2 │ int f(int n) {
// IR:  3 │     struct { char a[n]; int after; } x;
// IR:    ·     ───────────────────────────────────
// IR:  4 │     return x.after;
// IR:    ╰────
// SLATE-FILECHECK-END IR
