static extern int invalid_storage;

// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × multiple storage classes
// DEFAULT: 1 │ static extern int invalid_storage;
// DEFAULT: 2 │
// SLATE-FILECHECK-END DEFAULT
