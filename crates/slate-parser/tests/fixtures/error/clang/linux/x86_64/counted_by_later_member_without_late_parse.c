// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir

struct later {
    int *items __attribute__((counted_by(count)));
    int count;
};

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR: × unresolved member name `count`
// IR: ╭─[tests/fixtures/error/clang/linux/x86_64/counted_by_later_member_without_late_parse.c:3:31]
// IR: 2 │ struct later {
// IR: 3 │     int *items __attribute__((counted_by(count)));
// IR: ·                               ──────────
// IR: 4 │     int count;
// IR: ╰────
// SLATE-FILECHECK-END IR
