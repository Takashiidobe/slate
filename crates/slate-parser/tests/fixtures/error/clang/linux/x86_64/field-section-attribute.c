// SLATE-FILECHECK-ARGS --dump-ir

struct Placed {
  int value __attribute__((section("data")));
};

int size(void) { return sizeof(struct Placed); }

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × invalid in this context: 'section' attribute only applies to functions and
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/field-section-attribute.c:2:1]
// SEMANTIC: 1 │
// SEMANTIC: 2 │ ╭─▶ struct Placed {
// SEMANTIC: 3 │ │     int value __attribute__((section("data")));
// SEMANTIC: 4 │ ╰─▶ };
// SEMANTIC: 5 │
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
