/* It is a constraint violation for a static function to be declared
   but not defined if it is used except in a sizeof expression.  The
   use of the function simply being unevaluated is not enough.  */
/* Origin: Joseph Myers <jsm@polyomino.org.uk> */
/* { dg-do compile } */
/* { dg-options "-O2 -std=iso9899:1990 -pedantic-errors" } */

/* Constraint violation (trivial case, where function is used).  */
static void f0(void); /* { dg-error "used but never defined" } */
void g0(void) { f0(); }

/* Constraint violation.  */
static void f1(void); /* { dg-error "used but never defined" } */
void g1(void) { if (0) { f1(); } }

/* Constraint violation.  */
static int f2(void); /* { dg-error "used but never defined" } */
void g2(void) { 0 ? f2() : 0; }

/* OK.  */
static int f3(void);
void g3(void) { sizeof(f3()); }

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
// DEFAULT-NEXT:     fn %0 @f0() -> void [linkage=internal];
// DEFAULT-NEXT:     fn %1 @g0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @f1() -> void [linkage=internal];
// DEFAULT-NEXT:     fn %3 @g1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f2() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %5 @g2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8: i32 [synthetic];
// DEFAULT-NEXT:         if ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%8, call<i32, signature=fn() -> i32>(%4));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f3() -> i32 [linkage=internal];
// DEFAULT-NEXT:     fn %7 @g3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         const<u64>(4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
