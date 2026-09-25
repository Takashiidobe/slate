register long ok_global asm("rsp");
register long bad_global asm("ebx");
register long percent_global asm("%rsp");
register double float_global asm("rbp");
register long unlabeled_global;
struct S { int m; };
struct S get(void);
int f(int x, int *p) {
  register int local asm("eax");
  register int unknown asm("notareg");
  register int empty asm("");
  struct S s;
  asm("" : "=r"(x), "=r"(*p), "=r"(p[0]), "=r"(s.m), "=r"("str") : "r"(x));
  asm("" : "=r"((struct S){0}.m) : "r"(x));
  asm("" : "=r"(1));
  asm("" : "=r"(x + 1));
  asm("" : "=r"(get().m));
  asm("" : "=r"((int)x));
  asm("" : "=r,m"(x) : "r"(x));
  asm("" : "=r"(x), "=r,m"(*p));
  asm goto("" : : : : done, missing);
  asm goto("" : : : : done, done);
  asm goto("" : : : : in_stmt_expr, in_raw_stmt_expr);
  ({ in_stmt_expr: ; });
  x = ({ in_raw_stmt_expr: x; });
done:
  return 0;
}

void jump_scopes(void) {
outer:
  ({ asm goto("" : : : : outer, inner); inner: ; });
}

// SLATE-FILECHECK-ERROR SEMANTIC

// SLATE-FILECHECK-BEGIN SEMANTIC
// SEMANTIC: Error:   × semantic analysis failed
// SEMANTIC: Error:
// SEMANTIC: × register 'ebx' unsuitable for global register variables on this target
// SEMANTIC: ╭─[tests/fixtures/error/asm-sema.c:2:26]
// SEMANTIC: 1 │ register long ok_global asm("rsp");
// SEMANTIC: 2 │ register long bad_global asm("ebx");
// SEMANTIC: ·                          ──────────
// SEMANTIC: 3 │ register long percent_global asm("%rsp");
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × register '%rsp' unsuitable for global register variables on this target
// SEMANTIC: ╭─[tests/fixtures/error/asm-sema.c:3:30]
// SEMANTIC: 2 │ register long bad_global asm("ebx");
// SEMANTIC: 3 │ register long percent_global asm("%rsp");
// SEMANTIC: ·                              ───────────
// SEMANTIC: 4 │ register double float_global asm("rbp");
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × unsupported type for named register variable
// SEMANTIC: ╭─[tests/fixtures/error/asm-sema.c:4:1]
// SEMANTIC: 3 │ register long percent_global asm("%rsp");
// SEMANTIC: 4 │ register double float_global asm("rbp");
// SEMANTIC: · ────────────────────────────────────────
// SEMANTIC: 5 │ register long unlabeled_global;
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × illegal storage class on file-scoped variable
// SEMANTIC: ╭─[tests/fixtures/error/asm-sema.c:5:1]
// SEMANTIC: 4 │ register double float_global asm("rbp");
// SEMANTIC: 5 │ register long unlabeled_global;
// SEMANTIC: · ───────────────────────────────
// SEMANTIC: 6 │ struct S { int m; };
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × invalid lvalue in asm output
// SEMANTIC: ╭─[tests/fixtures/error/asm-sema.c:15:17]
// SEMANTIC: 14 │   asm("" : "=r"((struct S){0}.m) : "r"(x));
// SEMANTIC: 15 │   asm("" : "=r"(1));
// SEMANTIC: ·                 ─
// SEMANTIC: 16 │   asm("" : "=r"(x + 1));
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × invalid lvalue in asm output
// SEMANTIC: ╭─[tests/fixtures/error/asm-sema.c:16:17]
// SEMANTIC: 15 │   asm("" : "=r"(1));
// SEMANTIC: 16 │   asm("" : "=r"(x + 1));
// SEMANTIC: ·                 ─────
// SEMANTIC: 17 │   asm("" : "=r"(get().m));
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × invalid lvalue in asm output
// SEMANTIC: ╭─[tests/fixtures/error/asm-sema.c:17:17]
// SEMANTIC: 16 │   asm("" : "=r"(x + 1));
// SEMANTIC: 17 │   asm("" : "=r"(get().m));
// SEMANTIC: ·                 ───────
// SEMANTIC: 18 │   asm("" : "=r"((int)x));
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × invalid use of a cast in an inline asm context requiring an lvalue
// SEMANTIC: ╭─[tests/fixtures/error/asm-sema.c:18:17]
// SEMANTIC: 17 │   asm("" : "=r"(get().m));
// SEMANTIC: 18 │   asm("" : "=r"((int)x));
// SEMANTIC: ·                 ──────
// SEMANTIC: 19 │   asm("" : "=r,m"(x) : "r"(x));
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × asm constraint has an unexpected number of alternatives: 2 vs 1
// SEMANTIC: ╭─[tests/fixtures/error/asm-sema.c:19:28]
// SEMANTIC: 18 │   asm("" : "=r"((int)x));
// SEMANTIC: 19 │   asm("" : "=r,m"(x) : "r"(x));
// SEMANTIC: ·                            ─
// SEMANTIC: 20 │   asm("" : "=r"(x), "=r,m"(*p));
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × asm constraint has an unexpected number of alternatives: 1 vs 2
// SEMANTIC: ╭─[tests/fixtures/error/asm-sema.c:20:28]
// SEMANTIC: 19 │   asm("" : "=r,m"(x) : "r"(x));
// SEMANTIC: 20 │   asm("" : "=r"(x), "=r,m"(*p));
// SEMANTIC: ·                            ──
// SEMANTIC: 21 │   asm goto("" : : : : done, missing);
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × use of undeclared label 'missing'
// SEMANTIC: ╭─[tests/fixtures/error/asm-sema.c:21:29]
// SEMANTIC: 20 │   asm("" : "=r"(x), "=r,m"(*p));
// SEMANTIC: 21 │   asm goto("" : : : : done, missing);
// SEMANTIC: ·                             ───────
// SEMANTIC: 22 │   asm goto("" : : : : done, done);
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × duplicate use of asm operand name "done"
// SEMANTIC: ╭─[tests/fixtures/error/asm-sema.c:22:29]
// SEMANTIC: 21 │   asm goto("" : : : : done, missing);
// SEMANTIC: 22 │   asm goto("" : : : : done, done);
// SEMANTIC: ·                             ────
// SEMANTIC: 23 │   asm goto("" : : : : in_stmt_expr, in_raw_stmt_expr);
// SEMANTIC: ╰────
// SEMANTIC: Error:
// SEMANTIC: × cannot jump from this asm goto statement to one of its possible targets
// SEMANTIC: ╭─[tests/fixtures/error/asm-sema.c:23:3]
// SEMANTIC: 22 │   asm goto("" : : : : done, done);
// SEMANTIC: 23 │   asm goto("" : : : : in_stmt_expr, in_raw_stmt_expr);
// SEMANTIC: ·   ────────────────────────────────────────────────────
// SEMANTIC: 24 │   ({ in_stmt_expr: ; });
// SEMANTIC: ╰────
// SLATE-FILECHECK-END SEMANTIC
