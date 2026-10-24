/* PR c/71858 */
/* Only consider function names, function pointers and macros for implicit function declarations.  */
/* { dg-do compile } */
/* { dg-options "-fpermissive -Wimplicit-function-declaration -fdiagnostics-show-caret" } */

void fn1abcd (void);

void
test_1 (int fn1bacd)
{
  fn1badc (); /* { dg-warning "3: implicit declaration of function .fn1badc.; did you mean .fn1abcd.?" } */
  /* { dg-begin-multiline-output "" }
   fn1badc ();
   ^~~~~~~
   fn1abcd
   { dg-end-multiline-output "" } */
}

void fn2efgh (void);
void (*fn2efhg) (void);

void
test_2 (void)
{
  fn2fehg (); /* { dg-warning "3: implicit declaration of function .fn2fehg.; did you mean .fn2efhg.?" } */
  /* { dg-begin-multiline-output "" }
   fn2fehg ();
   ^~~~~~~
   fn2efhg
   { dg-end-multiline-output "" } */
}

void fn3ijkl (void);
typedef int fn3ijlk;

void
test_3 (void)
{
  fn3jilk (); /* { dg-warning "3: implicit declaration of function .fn3jilk.; did you mean .fn3ijkl.?" } */
  /* { dg-begin-multiline-output "" }
   fn3jilk ();
   ^~~~~~~
   fn3ijkl
   { dg-end-multiline-output "" } */
}

// SLATE-FILECHECK-ARGS -fpermissive
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_fn3ijlk:[0-9]+]] fn3ijlk = i32;
// DEFAULT-NEXT:     global %[[VALUE_fn2efhg:[0-9]+]] fn2efhg: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn1abcd:[0-9]+]] @fn1abcd() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn1badc:[0-9]+]] @fn1badc(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_1:[0-9]+]] @test_1(%[[VALUE_fn1bacd:[0-9]+]] fn1bacd: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_fn1badc]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2efgh:[0-9]+]] @fn2efgh() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn2fehg:[0-9]+]] @fn2fehg(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_2:[0-9]+]] @test_2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_fn2fehg]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3ijkl:[0-9]+]] @fn3ijkl() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn3jilk:[0-9]+]] @fn3jilk(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_3:[0-9]+]] @test_3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_fn3jilk]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
