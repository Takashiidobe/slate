// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

int out_of_range_case(int x) {
    switch (x) {
    case (int)1e100: return 1;
    default: return 0;
    }
}

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × unsupported in numeric IR lowering: nonconstant case expression
// SLATE-FILECHECK-END IR
