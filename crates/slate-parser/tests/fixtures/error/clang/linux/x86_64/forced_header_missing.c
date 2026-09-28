int value;

// SLATE-FILECHECK-ARGS -includemissing-forced-header.h
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × header not found in search path: "missing-forced-header.h"
// PARSE: ╰─▶ header not found in search path: "missing-forced-header.h"
// SLATE-FILECHECK-END PARSE
