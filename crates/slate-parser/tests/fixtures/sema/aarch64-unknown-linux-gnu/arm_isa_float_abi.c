// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR
// SLATE-FILECHECK-ARGS -mfloat-abi=hard

int value;

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × all rules failed: target ISA options: float ABI, FPU, and Thumb options
// SLATE-FILECHECK-END ERROR
