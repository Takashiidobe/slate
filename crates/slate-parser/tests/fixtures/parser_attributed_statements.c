// SLATE-FILECHECK-DEFINES AST
// SLATE-FILECHECK-STD AST gnu23

int attributed(int x) {
    [[vendor::hint]] if (x) [[vendor::hint]] return 1; else return 2;
    [[vendor::hint]] while (x) { x--; }
    [[vendor::hint]] for (; x; x--) ;
    [[vendor::hint]] goto done;
    [[vendor::hint]] done: x++;
    switch (x) {
    [[vendor::hint]] case 1: x++;
    [[fallthrough]];
    default: break;
    }
    [[maybe_unused]] int local = x;
    return local;
}

// SLATE-FILECHECK-IR-ERROR AST

// SLATE-FILECHECK-BEGIN AST
// AST: Error:   × unsupported in numeric IR lowering: module statement
// SLATE-FILECHECK-END AST
