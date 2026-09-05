/* Test trigraphs supported in C23 with -trigraphs used.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors -trigraphs" } */

/* { dg-do run } */
/* { dg-options "-ansi" } */

extern void abort (void);

/* Basic tests for trigraph conversion.
   All of them are here, but not in all possible contexts.  *??/
/

??=include <stdio.h>
