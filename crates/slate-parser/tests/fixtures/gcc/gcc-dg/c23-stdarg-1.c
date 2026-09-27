/* Test C23 variadic functions with no named parameters.  Compilation tests,
   valid code.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

int f (...);
int g (int (...));
int h (...) { return 0; }

typedef int A[];
typedef int A2[2];

A *f1 (...);
A2 *f1 (...);
A *f1 (...) { return 0; }

A2 *f2 (...);
A *f2 (...);
A2 *f2 (...) { return 0; }
typeof (f1) f2;

int t () { return f () + f (1) + f (1, 2) + h () + h (1.5, 2, f1) + g (f); }

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
// DEFAULT-NEXT:     type @type0 A = array<i32, incomplete>;
// DEFAULT-NEXT:     type @type1 A2 = array<i32, 2>;
// DEFAULT-NEXT:     fn %0 @f(...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @g(%8 <unnamed>: ptr<fn(...) -> i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @h(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @f1(...) -> ptr<array<i32, incomplete>> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<array<i32, incomplete>>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f2(...) -> ptr<array<i32, 2>> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<array<i32, 2>>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @t() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(...) -> i32>(%0), call<i32, signature=fn(...) -> i32>(%0, const<i32>(1))), call<i32, signature=fn(...) -> i32>(%0, const<i32>(1), const<i32>(2))), call<i32, signature=fn(...) -> i32>(%2)), call<i32, signature=fn(...) -> i32>(%2, const<f64>(1.5), const<i32>(2), function_decay<ptr<fn(...) -> ptr<array<i32, 2>>>>(%5))), call<i32, signature=fn(ptr<fn(...) -> i32>) -> i32>(%1, function_decay<ptr<fn(...) -> i32>>(%0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
