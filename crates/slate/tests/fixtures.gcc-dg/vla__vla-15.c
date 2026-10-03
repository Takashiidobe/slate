/* Test for VLA size evaluation in sizeof typeof.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do run } */
/* { dg-options "-std=gnu99" } */
/* Blocked by CIR emission: Clang crashes while emitting CIR for this VLA type expression. */

#include <stdarg.h>

extern void exit(int);
extern void abort(void);

char a[1];

void f1(void) {
  int i = 0;
  int j = sizeof(typeof(*(++i, (char (*)[i])a)));
  if (i != 1 || j != 1)
    abort();
}

int main(void) {
  f1();
  exit(0);
}
