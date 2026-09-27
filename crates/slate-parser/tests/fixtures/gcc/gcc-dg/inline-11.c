/* Test misuses of inline.  */
/* Origin: Joseph Myers <jsm@polyomino.org.uk> */
/* { dg-do compile } */
/* { dg-options "-std=gnu99" } */

/* These should perhaps be hard errors, but are pedwarns at
   present.  */

inline int a; /* { dg-warning "variable 'a' declared 'inline'" } */
inline int (*b)(void); /* { dg-warning "variable 'b' declared 'inline'" } */
typedef inline void c(void); /* { dg-warning "typedef 'c' declared 'inline'" } */
typedef inline int d; /* { dg-warning "typedef 'd' declared 'inline'" } */
void e(inline int f(void)); /* { dg-warning "parameter 'f' declared 'inline'" } */
void g(inline int(void)); /* { dg-warning "parameter '\\({anonymous}\\)' declared 'inline'" } */

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     type @type0 c = fn() -> void;
// DEFAULT-NEXT:     type @type1 d = i32;
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 b: ptr<fn() -> i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @e(%6 f: ptr<fn() -> i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @g(%7 <unnamed>: ptr<fn() -> i32>) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
