int        real_global = 12;
extern int alias_global __attribute__((alias("real_global")));



int main(void) {
  int result;
  result = alias_global;
  return result;
}

// SLATE-FILECHECK-ERROR GCC

// SLATE-FILECHECK-BEGIN GCC
// GCC: Error:   × unexpected tokens after expression
// GCC: ╰─▶ unexpected tokens after expression
// GCC: ╭─[tests/fixtures/ported-slate-alias-global.c:1:1]
// GCC: 1 │ int        real_global = 12;
// GCC: · ────────────
// GCC: 2 │ extern int alias_global __attribute__((alias("real_global")));
// GCC: ╰────
// SLATE-FILECHECK-END GCC
