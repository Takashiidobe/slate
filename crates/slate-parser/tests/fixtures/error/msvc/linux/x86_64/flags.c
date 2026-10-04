// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ERROR CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions -fwrapv

void operations(void) {
    1 + 2;
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: Error:   × invalid compiler argument `-fwrapv`: unknown option for the msvc flavor
// SLATE-FILECHECK-END CHECK
