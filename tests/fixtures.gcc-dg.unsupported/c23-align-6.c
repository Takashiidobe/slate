/* Test C23 alignof returning minimum alignment for a type.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */

#define _Alignas alignas
#define _Alignof alignof

/* Test C11 _Alignof returning minimum alignment for a type.  PR
   52023.  */
/* { dg-do run } */
/* { dg-options "-std=c11" } */

extern void abort(void);
extern void exit(int);
