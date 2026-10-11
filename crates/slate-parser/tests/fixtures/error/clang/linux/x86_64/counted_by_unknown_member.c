// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ERROR IR
// SLATE-FILECHECK-ARGS --dump-ir -fexperimental-late-parse-attributes

struct missing {
    int count;
    struct {
        int tag;
        int tail[] __attribute__((counted_by(length)));
    };
};

// SLATE-FILECHECK-BEGIN IR
// IR: Error:   × semantic analysis failed
// IR: Error:
// IR: × unresolved member name `length`
// IR: ╭─[tests/fixtures/error/clang/linux/x86_64/counted_by_unknown_member.c:6:35]
// IR: 5 │         int tag;
// IR: 6 │         int tail[] __attribute__((counted_by(length)));
// IR: ·                                   ──────────
// IR: 7 │     };
// IR: ╰────
// SLATE-FILECHECK-END IR
