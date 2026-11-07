// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-IR-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

void f(int n) {
    struct S { char a[n]; } x, y;
    x = y;
}

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR:   × not implemented: value of a record with a variably modified member
// IR:    ╭─[tests/fixtures/gcc/linux/x86_64/vla-record-value.c:4:9]
// IR:  3 │     struct S { char a[n]; } x, y;
// IR:  4 │     x = y;
// IR:    ·         ─
// IR:  5 │ }
// IR:    ╰────
// SLATE-FILECHECK-END IR
