// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir -fcommon

int tentative;

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × all rules failed: MSVC stack alignment options: MSVC does not support
// SLATE-FILECHECK-END DEFAULT
