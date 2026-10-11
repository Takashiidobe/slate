// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

void work(void);
struct S { int x; };

int leave_outside(void) {
  __leave;
  return 0;
}

int leave_in_handlers(void) {
  __try {
    work();
  } __except (1) {
    __leave;
  }
  __try {
    work();
  } __finally {
    __leave;
  }
  return 0;
}

int code_outside(void) { return _exception_code(); }

void *info_in_except_block(void) {
  __try {
    work();
  } __except (1) {
    return _exception_info();
  }
  return 0;
}

int non_integral_filters(struct S s, double d, int *p) {
  __try { work(); } __except (s) {}
  __try { work(); } __except (d) {}
  __try { work(); } __except (p) {}
  return 0;
}

int noop_void(void) { return __noop(work()); }

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × '__leave' statement not in __try block
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/x86_64/seh_misuse.c:6:3]
// SEMANTIC: 5 │ int leave_outside(void) {
// SEMANTIC: 6 │   __leave;
// SEMANTIC: ·   ────────
// SEMANTIC: 7 │   return 0;
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × '__leave' statement not in __try block
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/x86_64/seh_misuse.c:14:5]
// SEMANTIC: 13 │   } __except (1) {
// SEMANTIC: 14 │     __leave;
// SEMANTIC: ·     ────────
// SEMANTIC: 15 │   }
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × '__leave' statement not in __try block
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/x86_64/seh_misuse.c:19:5]
// SEMANTIC: 18 │   } __finally {
// SEMANTIC: 19 │     __leave;
// SEMANTIC: ·     ────────
// SEMANTIC: 20 │   }
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × '_exception_code' only allowed in __except block or filter expression
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/x86_64/seh_misuse.c:24:33]
// SEMANTIC: 23 │
// SEMANTIC: 24 │ int code_outside(void) { return _exception_code(); }
// SEMANTIC: ·                                 ───────────────
// SEMANTIC: 25 │
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × '_exception_info' only allowed in __except filter expression
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/x86_64/seh_misuse.c:30:12]
// SEMANTIC: 29 │   } __except (1) {
// SEMANTIC: 30 │     return _exception_info();
// SEMANTIC: ·            ───────────────
// SEMANTIC: 31 │   }
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × filter expression has non-integral type
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/x86_64/seh_misuse.c:36:31]
// SEMANTIC: 35 │ int non_integral_filters(struct S s, double d, int *p) {
// SEMANTIC: 36 │   __try { work(); } __except (s) {}
// SEMANTIC: ·                               ─
// SEMANTIC: 37 │   __try { work(); } __except (d) {}
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × filter expression has non-integral type
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/x86_64/seh_misuse.c:37:31]
// SEMANTIC: 36 │   __try { work(); } __except (s) {}
// SEMANTIC: 37 │   __try { work(); } __except (d) {}
// SEMANTIC: ·                               ─
// SEMANTIC: 38 │   __try { work(); } __except (p) {}
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × filter expression has non-integral type
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/x86_64/seh_misuse.c:38:31]
// SEMANTIC: 37 │   __try { work(); } __except (d) {}
// SEMANTIC: 38 │   __try { work(); } __except (p) {}
// SEMANTIC: ·                               ─
// SEMANTIC: 39 │   return 0;
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × argument type 'void' is incomplete
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/x86_64/seh_misuse.c:42:37]
// SEMANTIC: 41 │
// SEMANTIC: 42 │ int noop_void(void) { return __noop(work()); }
// SEMANTIC: ·                                     ──────
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
