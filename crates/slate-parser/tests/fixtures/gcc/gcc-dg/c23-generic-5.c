/* Test references to never-defined static functions in _Generic: allowed in
   certain places for C23 but not before.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

static int ok1_c23 ();
static int ok2_c23 ();
static int ok3_c23 ();
static int ok4_c23 ();
static int ok5_c23 ();
static int ok6 ();
static int ok7 ();
static int ok8 ();
static int ok9 ();
static int ok10 ();
static int ok11 ();
static int ok12 ();
static int not_ok1 (); /* { dg-error "used but never defined" } */
static int not_ok2 (); /* { dg-error "used but never defined" } */

void
f ()
{
  _Generic (ok1_c23 (), int: 2);
  _Generic (1, int: 2, default: ok2_c23 ());
  _Generic (1, default: ok3_c23 (), int: 3);
  _Generic (1, int: 2, float: ok4_c23 ());
  _Generic (1, float: ok5_c23 (), int: 3);
  sizeof (_Generic (ok8 (), int: 2));
  sizeof (_Generic (1, int: 2, default: ok9 ()));
  sizeof (_Generic (1, default: ok10 (), int: 3));
  sizeof (_Generic (1, int: 2, float: ok11 ()));
  sizeof (_Generic (1, float: ok12 (), int: 3));
  _Generic (1.0, int: 2, default: not_ok1 ());
  _Generic (1.0, default: not_ok2 (), int: 3);
  sizeof (_Generic (1.0, int: 2, default: ok6 ()));
  sizeof (_Generic (1.0, default: ok7 (), int: 3));
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     fn %0 @ok1_c23() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %1 @ok2_c23() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %2 @ok3_c23() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %3 @ok4_c23() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %4 @ok5_c23() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %5 @ok6() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %6 @ok7() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %7 @ok8() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %8 @ok9() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %9 @ok10() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %10 @ok11() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %11 @ok12() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %12 @not_ok1() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %13 @not_ok2() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %14 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         const<i32>(2);
// DEFAULT-NEXT:         const<i32>(2);
// DEFAULT-NEXT:         const<i32>(3);
// DEFAULT-NEXT:         const<i32>(2);
// DEFAULT-NEXT:         const<i32>(3);
// DEFAULT-NEXT:         const<u64>(4);
// DEFAULT-NEXT:         const<u64>(4);
// DEFAULT-NEXT:         const<u64>(4);
// DEFAULT-NEXT:         const<u64>(4);
// DEFAULT-NEXT:         const<u64>(4);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%12);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%13);
// DEFAULT-NEXT:         const<u64>(4);
// DEFAULT-NEXT:         const<u64>(4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
