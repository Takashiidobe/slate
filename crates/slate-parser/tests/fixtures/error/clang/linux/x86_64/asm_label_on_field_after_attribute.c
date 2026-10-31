struct s {
    int x __attribute__((unused)) __asm__("y");
};

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × expected `;`
// PARSE:   ╰─▶ expected `;`
// PARSE:    ╭─[tests/fixtures/error/clang/linux/x86_64/asm_label_on_field_after_attribute.c:2:35]
// PARSE:  1 │ struct s {
// PARSE:  2 │     int x __attribute__((unused)) __asm__("y");
// PARSE:    ·                                   ───────
// PARSE:  3 │ };
// PARSE:    ╰────
// SLATE-FILECHECK-END PARSE
