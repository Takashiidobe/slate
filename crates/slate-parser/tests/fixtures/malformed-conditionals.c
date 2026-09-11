#if 1
int value;
#else
#elif 0
#endif

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × #elif after #else
// PARSE: 3 │ #else
// PARSE: 4 │ #elif 0
// PARSE: 5 │ #endif
// SLATE-FILECHECK-END PARSE
