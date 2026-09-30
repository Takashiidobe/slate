/* Test for constraints on constant expressions.  In C90 it is clear that
   certain constructs are not permitted in unevaluated parts of an
   expression (except in sizeof); in C99 it might fall within implementation
   latitude.
*/
/* Origin: Joseph Myers <jsm28@cam.ac.uk>; inspired by
   http://deja.com/getdoc.xp?AN=524271595&fmt=text by Peter Seebach.
*/
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1990 -pedantic-errors" } */

extern int bar (void);

void
foo (void)
{
  int i;
  static int j = (1 ? 0 : (i = 2)); /* { dg-error "initial" "assignment" } */
  static int k = (1 ? 0 : ++i); /* { dg-error "initial" "increment" } */
  static int l = (1 ? 0 : --i); /* { dg-error "initial" "decrement" } */
  static int m = (1 ? 0 : bar ()); /* { dg-error "initial" "function call" } */
  static int n = (1 ? 0 : (2, 3)); /* { dg-error "initial" "comma" } */
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1990
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
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: i32 [storage=static] = conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), const<i32>(0), store<i32>(%[[VALUE_i:[0-9]+]], const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: i32 [storage=static] = conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), const<i32>(0), update<i32, result=new>(%[[VALUE_i]], add<i32, overflow=ub>(old<i32>, const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: i32 [storage=static] = conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), const<i32>(0), update<i32, result=new>(%[[VALUE_i]], sub<i32, overflow=ub>(old<i32>, const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_m:[0-9]+]] m: i32 [storage=static] = conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), const<i32>(0), call<i32, signature=fn() -> i32>(%[[VALUE_bar:[0-9]+]])) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: i32 [storage=static] = conditional<i32>(ne<i32>(const<i32>(1), const<i32>(0)), const<i32>(0), sequence<i32>(const<i32>(2), const<i32>(3))) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_bar]] @bar() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i]] i: i32 [storage=automatic];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
