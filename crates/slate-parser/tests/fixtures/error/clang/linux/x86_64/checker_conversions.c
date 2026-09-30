struct S { int x; };
void v(void);
int takes(int);
int assign(struct S s) { int x; x = s; return x; }
int ret(void) { return v(); }
int arg(struct S s) { return takes(s); }
int cast(struct S s) { return (int)s; }
int init(struct S s) { int x = s; return x; }

// SLATE-FILECHECK-ERROR PARSE

// SLATE-FILECHECK-BEGIN PARSE
// PARSE: Error:   × semantic analysis failed
// PARSE: Error:
// PARSE: × conversion between a struct or union and an unrelated type
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/checker_conversions.c:4:37]
// PARSE: 3 │ int takes(int);
// PARSE: 4 │ int assign(struct S s) { int x; x = s; return x; }
// PARSE: ·                                     ─
// PARSE: 5 │ int ret(void) { return v(); }
// PARSE: ╰────
// PARSE: Error:
// PARSE: × conversion from void
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/checker_conversions.c:5:24]
// PARSE: 4 │ int assign(struct S s) { int x; x = s; return x; }
// PARSE: 5 │ int ret(void) { return v(); }
// PARSE: ·                        ───
// PARSE: 6 │ int arg(struct S s) { return takes(s); }
// PARSE: ╰────
// PARSE: Error:
// PARSE: × conversion between a struct or union and an unrelated type
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/checker_conversions.c:6:36]
// PARSE: 5 │ int ret(void) { return v(); }
// PARSE: 6 │ int arg(struct S s) { return takes(s); }
// PARSE: ·                                    ─
// PARSE: 7 │ int cast(struct S s) { return (int)s; }
// PARSE: ╰────
// PARSE: Error:
// PARSE: × conversion between a struct or union and an unrelated type
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/checker_conversions.c:7:31]
// PARSE: 6 │ int arg(struct S s) { return takes(s); }
// PARSE: 7 │ int cast(struct S s) { return (int)s; }
// PARSE: ·                               ──────
// PARSE: 8 │ int init(struct S s) { int x = s; return x; }
// PARSE: ╰────
// PARSE: Error:
// PARSE: × conversion between a struct or union and an unrelated type
// PARSE: ╭─[tests/fixtures/error/clang/linux/x86_64/checker_conversions.c:8:32]
// PARSE: 7 │ int cast(struct S s) { return (int)s; }
// PARSE: 8 │ int init(struct S s) { int x = s; return x; }
// PARSE: ·                                ─
// PARSE: 9 │
// PARSE: ╰────
// SLATE-FILECHECK-END PARSE
