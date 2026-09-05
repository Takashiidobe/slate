/* { dg-skip-if "gdb hang" { hppa*-*-linux* } } */
/* { dg-options "-g" } */
/* { dg-do run { xfail { ! aarch64*-*-* } } } */
/* { dg-xfail-run-if "" aarch64*-*-* "*" { "-O[01g]" } } */

#define GUALITY_DONT_FORCE_LIVE_AFTER -1

#ifndef STATIC_INLINE
#define STATIC_INLINE /*static*/
#endif

/* Infrastructure to test the quality of debug information.
   Copyright (C) 2008, 2009, 2010 Free Software Foundation, Inc.
   Contributed by Alexandre Oliva <aoliva@redhat.com>.
