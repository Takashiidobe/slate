int value;

// SLATE-FILECHECK-ARGS -iwithprefix=tests/fixtures/include_search/user
// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × invalid compiler argument `-iwithprefix=tests/fixtures/include_search/
// SLATE-FILECHECK-END PARSE
