int value;

// SLATE-FILECHECK-ARGS -mms-bitfields
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-mms-bitfields`: unknown option for the msvc
// SLATE-FILECHECK-END PARSE
