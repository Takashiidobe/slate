int value;

// SLATE-FILECHECK-ARGS -iwithprefix=tests/fixtures/inputs/include_search/user
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-iwithprefix=tests/fixtures/inputs/
// SLATE-FILECHECK-END PARSE
