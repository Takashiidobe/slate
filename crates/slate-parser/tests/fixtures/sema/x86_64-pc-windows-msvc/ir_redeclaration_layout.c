// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir


int object;
long object;

int returns(int);
long returns(int);

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wconflicting-types
// WARN: ⚠ redeclaration with a different integer type of the same size
// WARN: ╭─[tests/fixtures/sema/x86_64-pc-windows-msvc/ir_redeclaration_layout.c:4:6]
// WARN: 3 │ int object;
// WARN: 4 │ long object;
// WARN: ·      ──────
// WARN: 5 │
// WARN: ╰────
// WARN: -Wconflicting-types
// WARN: ⚠ function redeclared with a different integer return type of the same size
// WARN: ╭─[tests/fixtures/sema/x86_64-pc-windows-msvc/ir_redeclaration_layout.c:7:6]
// WARN: 6 │ int returns(int);
// WARN: 7 │ long returns(int);
// WARN: ·      ────────────
// WARN: 8 │
// WARN: ╰────
// SLATE-FILECHECK-END WARN
