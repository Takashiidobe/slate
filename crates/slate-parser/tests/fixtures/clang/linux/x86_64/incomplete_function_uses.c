struct S;
struct S *p;
void take(struct S);
struct S give(void);

void defines_parameter(struct S s) {}
struct S defines_result(void) {}
void passes_argument(void) { take(*p); }
void calls_for_result(void) { give(); }

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × variable has incomplete type
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/incomplete_function_uses.c:6:24]
// DEFAULT: 5 │
// DEFAULT: 6 │ void defines_parameter(struct S s) {}
// DEFAULT: ·                        ──────────
// DEFAULT: 7 │ struct S defines_result(void) {}
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × incomplete result type in function definition
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/incomplete_function_uses.c:7:1]
// DEFAULT: 6 │ void defines_parameter(struct S s) {}
// DEFAULT: 7 │ struct S defines_result(void) {}
// DEFAULT: · ────────────────────────────────
// DEFAULT: 8 │ void passes_argument(void) { take(*p); }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × argument type is incomplete
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/incomplete_function_uses.c:8:30]
// DEFAULT: 7 │ struct S defines_result(void) {}
// DEFAULT: 8 │ void passes_argument(void) { take(*p); }
// DEFAULT: ·                              ────────
// DEFAULT: 9 │ void calls_for_result(void) { give(); }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × calling function with incomplete return type
// DEFAULT: ╭─[tests/fixtures/clang/linux/x86_64/incomplete_function_uses.c:9:31]
// DEFAULT: 8 │ void passes_argument(void) { take(*p); }
// DEFAULT: 9 │ void calls_for_result(void) { give(); }
// DEFAULT: ·                               ──────
// DEFAULT: 10 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
