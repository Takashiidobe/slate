// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ERROR CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions -fwrapv --flavor=msvc

void operations(void) {
    1 + 2;
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: Error:   × unsupported argument for msvc flavor: -fwrapv
// SLATE-FILECHECK-END CHECK
