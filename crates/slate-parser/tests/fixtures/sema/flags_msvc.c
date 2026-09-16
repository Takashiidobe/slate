// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ERROR CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions -fwrapv --flavor=msvc

void operations(void) {
    1 + 2;
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: Error:   × all rules failed: MSVC stack alignment options: MSVC does not support
// SLATE-FILECHECK-END CHECK
