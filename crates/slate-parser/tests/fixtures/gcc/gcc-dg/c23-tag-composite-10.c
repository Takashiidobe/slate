/* { dg-do run } */
/* { dg-options "-std=c23" } */

// test padding works correctly

static struct fo {
  int a : 1;
  long  : 3;
  int b : 1;
} x = {};

static void foo(void *p) {
  struct fo {
    int a : 1;
    long  : 3;
    int b : 1;
  } y;

  typeof(*(1 ? &x : &y)) *z = p;
  __builtin_clear_padding(z);
}

int main() {
  struct fo *p = __builtin_malloc(sizeof *p);
  __builtin_memset(p, 0xFFFF, sizeof *p);
  foo(p);
  p->a = 0;
  p->b = 0;
  if (0 != __builtin_memcmp(p, &x, sizeof *p))
    __builtin_abort();
  return 0;
}





// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unsupported Clang builtin `__builtin_clear_padding`
// DEFAULT: ╭─[tests/fixtures/gcc/gcc-dg/c23-tag-composite-10.c:12:1]
// DEFAULT: 11 │
// DEFAULT: 12 │ ╭─▶ static void foo(void *p) {
// DEFAULT: 13 │ │     struct fo {
// DEFAULT: 14 │ │       int a : 1;
// DEFAULT: 15 │ │       long  : 3;
// DEFAULT: 16 │ │       int b : 1;
// DEFAULT: 17 │ │     } y;
// DEFAULT: 18 │ │
// DEFAULT: 19 │ │     typeof(*(1 ? &x : &y)) *z = p;
// DEFAULT: 20 │ │     __builtin_clear_padding(z);
// DEFAULT: 21 │ ╰─▶ }
// DEFAULT: 22 │
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
