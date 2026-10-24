unsigned char data[] = {
#embed "slate-missing-embed.bin"
};



// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × 'slate-missing-embed.bin' file not found
// PARSE: ╰─▶ 'slate-missing-embed.bin' file not found
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/embed-missing-resource.c:2:8]
// PARSE: 1 │ unsigned char data[] = {
// PARSE: 2 │ #embed "slate-missing-embed.bin"
// PARSE: ·        ─────────────────────────
// PARSE: 3 │ };
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
