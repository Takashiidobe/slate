// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

extern int incomplete[];

unsigned long pointer(int p[]) { return _Countof (p); }

unsigned long scalar(void) { return _Countof (int); }

unsigned long unknown_bound(void) { return _Countof (incomplete); }

void variable(int n) { _Static_assert (_Countof (int [n][3]) == 7); }

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × invalid application of _Countof to a non-array type
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/countof_invalid_operand.c:4:41]
// SEMANTIC: 3 │
// SEMANTIC: 4 │ unsigned long pointer(int p[]) { return _Countof (p); }
// SEMANTIC: ·                                         ────────────
// SEMANTIC: 5 │
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × invalid application of _Countof to a non-array type
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/countof_invalid_operand.c:6:37]
// SEMANTIC: 5 │
// SEMANTIC: 6 │ unsigned long scalar(void) { return _Countof (int); }
// SEMANTIC: ·                                     ──────────────
// SEMANTIC: 7 │
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × _Countof of an incomplete array type
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/countof_invalid_operand.c:8:44]
// SEMANTIC: 7 │
// SEMANTIC: 8 │ unsigned long unknown_bound(void) { return _Countof (incomplete); }
// SEMANTIC: ·                                            ─────────────────────
// SEMANTIC: 9 │
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × static assertion requires an integer constant expression: _Countof of a
// SEMANTIC: ╭─[tests/fixtures/error/clang/linux/x86_64/countof_invalid_operand.c:10:40]
// SEMANTIC: 9 │
// SEMANTIC: 10 │ void variable(int n) { _Static_assert (_Countof (int [n][3]) == 7); }
// SEMANTIC: ·                                        ──────────────────────────
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
