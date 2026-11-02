// SLATE-FILECHECK-ARGS -fno-blocks
// SLATE-FILECHECK-ERROR PARSE

int (^callback)(int);

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected declarator
// PARSE:   ╰─▶ expected declarator
// PARSE:    ╭─[tests/fixtures/error/clang/linux/x86_64/block_pointer_without_fblocks.c:2:6]
// PARSE:  1 │
// PARSE:  2 │ int (^callback)(int);
// PARSE:    ·      ─
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
