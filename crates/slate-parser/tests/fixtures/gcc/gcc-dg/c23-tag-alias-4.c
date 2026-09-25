/* { dg-do run }
 * { dg-options "-std=c23 -O2" }
 */

/* Here we check that structs with flexible array
 * members can alias a compatible redefinition.  */

struct bar {
  int x;
  int f[];
};

int test_bar1(struct bar *a, void *b) {
  a->x = 1;

  struct bar {
    int x;
    int f[];
  }          *p = b;
  struct bar *q = a;
  p->x          = 2;

  return a->x;
}

int main() {
  struct bar z;

  if (2 != test_bar1(&z, &z))
    __builtin_abort();

  return 0;
}





// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error: -Wincompatible-pointer-types
// DEFAULT: × incompatible pointer types
// DEFAULT: ╭─[tests/fixtures/gcc/gcc-dg/c23-tag-alias-4.c:20:19]
// DEFAULT: 19 │   }          *p = b;
// DEFAULT: 20 │   struct bar *q = a;
// DEFAULT: ·                   ─
// DEFAULT: 21 │   p->x          = 2;
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
