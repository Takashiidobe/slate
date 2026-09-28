// SLATE-FILECHECK-DEFINES ERROR
// SLATE-FILECHECK-ERROR ERROR
// SLATE-FILECHECK-ARGS -mfloat-abi=softfp

int value;

// SLATE-FILECHECK-BEGIN ERROR
// ERROR: Error:   × all rules failed: target ISA options: Windows on Arm requires the hard
// SLATE-FILECHECK-END ERROR
