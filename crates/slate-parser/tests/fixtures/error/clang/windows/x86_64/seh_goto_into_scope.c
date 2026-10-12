// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-ERROR SEMANTIC

int into_try(void) {
  goto in;
  __try {
  in:;
  } __finally {
  }
  return 0;
}

int into_except(void) {
  goto in;
  __try {
  } __except (1) {
  in:;
  }
  return 0;
}

int into_finally(void) {
  goto in;
  __try {
  } __finally {
  in:;
  }
  return 0;
}

int out_of_try_to_handler(void) {
  __try {
    goto in;
  } __except (1) {
  in:;
  }
  return 0;
}

int within_scopes(void) {
  __try {
    goto in;
  in:;
  } __except (1) {
    goto out;
  }
out:
  return 0;
}

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × cannot jump from this goto statement to its label
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/x86_64/seh_goto_into_scope.c:3:3]
// SEMANTIC: 2 │ int into_try(void) {
// SEMANTIC: 3 │   goto in;
// SEMANTIC: ·   ────────
// SEMANTIC: 4 │   __try {
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × cannot jump from this goto statement to its label
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/x86_64/seh_goto_into_scope.c:12:3]
// SEMANTIC: 11 │ int into_except(void) {
// SEMANTIC: 12 │   goto in;
// SEMANTIC: ·   ────────
// SEMANTIC: 13 │   __try {
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × cannot jump from this goto statement to its label
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/x86_64/seh_goto_into_scope.c:21:3]
// SEMANTIC: 20 │ int into_finally(void) {
// SEMANTIC: 21 │   goto in;
// SEMANTIC: ·   ────────
// SEMANTIC: 22 │   __try {
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × cannot jump from this goto statement to its label
// SEMANTIC: ╭─[tests/fixtures/error/clang/windows/x86_64/seh_goto_into_scope.c:31:5]
// SEMANTIC: 30 │   __try {
// SEMANTIC: 31 │     goto in;
// SEMANTIC: ·     ────────
// SEMANTIC: 32 │   } __except (1) {
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
