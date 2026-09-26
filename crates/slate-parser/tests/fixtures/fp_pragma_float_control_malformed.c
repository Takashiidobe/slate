#pragma float_control(precise, ON)
int x;

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × invalid in this context: pragma float_control is malformed; use
// SLATE-FILECHECK-END DEFAULT
