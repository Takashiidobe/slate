/* Copyright (C) 2002 Free Software Foundation, Inc.

   Source: Neil Booth, 12 Feb 2002.

   In the declaration of proc, x must be parsed as a typedef name (C99
   6.7.5.3 p11.  Also see C89 DR #009, which was erroneously omitted
   from C99, and resubmitted as DR #249: if in a parameter
   declaration, an identifier can be read as a typedef name or a
   parameter name, it is read as a typedef name).  */

/* { dg-do compile } */

typedef int x;
typedef int y;
int proc(int (x));	/* x is a typedef, param to proc is a function.  */
int proc2(int x);	/* x is an identifier, param is an int.  */

/* Parameter to proc3 is unnamed, with type a function that returns
   int and takes a single argument of type function with one int
   parameter returning int.  In particular, proc3 is not a function
   that takes a parameter y that is a function with one int parameter
   returning int.  8-)  */
int proc3(int (y (x)));

int main ()
{
  proc (proc2);		/* { dg-bogus "integer from pointer" } */
  return proc3 (proc);  /* { dg-bogus "incompatible pointer type" } */
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT c89
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 x = i32;
// DEFAULT-NEXT:     type @type1 y = i32;
// DEFAULT-NEXT:     fn %2 @proc(%6 <unnamed>: ptr<fn(i32) -> i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @proc2(%7 x: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @proc3(%8 <unnamed>: ptr<fn(ptr<fn(i32) -> i32>) -> i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<fn(i32) -> i32>) -> i32>(%2, function_decay<ptr<fn(i32) -> i32>>(%3));
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<fn(ptr<fn(i32) -> i32>) -> i32>) -> i32>(%4, function_decay<ptr<fn(ptr<fn(i32) -> i32>) -> i32>>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
