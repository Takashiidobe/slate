struct inner { int a; };

struct outer {
    struct inner;
    int b;
};

int get(struct outer *o) { return o->a; }

// SLATE-FILECHECK-ARGS -fms-extensions -fno-ms-anonymous-structs
// SLATE-FILECHECK-ERROR SEMA

// SLATE-FILECHECK-BEGIN SEMA
// SEMA: Error:   × semantic analysis failed
// SEMA: Error:
// SEMA: × unknown member
// SEMA: ╭─[tests/fixtures/error/clang/linux/x86_64/ms-anonymous-structs-disabled.c:8:35]
// SEMA: 7 │
// SEMA: 8 │ int get(struct outer *o) { return o->a; }
// SEMA: ·                                   ────
// SEMA: 9 │
// SEMA: ╰────
// SLATE-FILECHECK-END SEMA
