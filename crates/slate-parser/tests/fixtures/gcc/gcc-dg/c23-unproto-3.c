/* Test that declaring a function with () is the same as (void) in C23.  Valid
   use cases.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors -Wstrict-prototypes" } */

void f1 ();
void f1 (void);

void f2 (void);
void f2 ();

typedef void T1 ();
typedef void T1 (void);

void f3 ();

_Static_assert (_Generic (f3,
			  void (*) (int) : 1,
			  void (*) (void) : 2,
			  default : 3) == 2);

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
// DEFAULT-NEXT:     type @type0 T1 = fn() -> void;
// DEFAULT-NEXT:     type @type1 T1 = fn() -> void;
// DEFAULT-NEXT:     fn %0 @f1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @f2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @f3() -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
