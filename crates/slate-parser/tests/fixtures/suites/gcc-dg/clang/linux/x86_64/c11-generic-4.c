/* Test references to never-defined static functions in _Generic: allowed in
   certain places for C23 but not before.  */
/* { dg-do compile } */
/* { dg-options "-std=c11 -pedantic-errors" } */

static int ok1_c23 (); /* { dg-error "used but never defined" } */
static int ok2_c23 (); /* { dg-error "used but never defined" } */
static int ok3_c23 (); /* { dg-error "used but never defined" } */
static int ok4_c23 (); /* { dg-error "used but never defined" } */
static int ok5_c23 (); /* { dg-error "used but never defined" } */
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

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     fn %[[VALUE_ok1_c23:[0-9]+]] @ok1_c23(unprototyped) -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_ok2_c23:[0-9]+]] @ok2_c23(unprototyped) -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_ok3_c23:[0-9]+]] @ok3_c23(unprototyped) -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_ok4_c23:[0-9]+]] @ok4_c23(unprototyped) -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_ok5_c23:[0-9]+]] @ok5_c23(unprototyped) -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_ok6:[0-9]+]] @ok6(unprototyped) -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_ok7:[0-9]+]] @ok7(unprototyped) -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_ok8:[0-9]+]] @ok8(unprototyped) -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_ok9:[0-9]+]] @ok9(unprototyped) -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_ok10:[0-9]+]] @ok10(unprototyped) -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_ok11:[0-9]+]] @ok11(unprototyped) -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_ok12:[0-9]+]] @ok12(unprototyped) -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_not_ok1:[0-9]+]] @not_ok1(unprototyped) -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_not_ok2:[0-9]+]] @not_ok2(unprototyped) -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
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
// DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_not_ok1]]);
// DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_not_ok2]]);
// DEFAULT-NEXT:         const<u64>(4);
// DEFAULT-NEXT:         const<u64>(4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
