// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

int identity(int value) {
    return value;
}

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × unsupported in numeric IR lowering: function parameters
// SLATE-FILECHECK-END IR
