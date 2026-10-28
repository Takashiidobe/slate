register long ok_global asm("rsp");
register long bad_global asm("ebx");
register long percent_global asm("%rsp");
register double float_global asm("rbp");
register long unlabeled_global;
struct S { int m; };
struct S get(void);
int f(int x, int *p) {
  register int local asm("eax");
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
// SEMANTIC: Error:   × unknown register name '' in asm
// SEMANTIC:   ╰─▶ unknown register name '' in asm
// SEMANTIC:     ╭─[tests/fixtures/error/clang/linux/x86_64/asm-sema.c:10:26]
// SEMANTIC:   9 │   register int local asm("eax");
// SEMANTIC:  10 │   register int empty asm("");
// SEMANTIC:     ·                          ──
// SEMANTIC:  11 │   struct S s;
// SEMANTIC:     ╰────
// SLATE-FILECHECK-END SEMANTIC
