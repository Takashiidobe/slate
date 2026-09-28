/* { dg-require-alias "" } */
int fn2(int);
int fn3(int);

__attribute__((flatten))
int fn1(int p1)
{
  int a = fn2(p1);
  return fn3(a);
}
__attribute__((flatten))
__attribute__((alias("fn1")))
int fn4(int p1);

/* Again, but this time the target doesn't have the attribute.  */
int fn1a(int p1)
{
  int a = fn2(p1);
  return fn3(a);
}
__attribute__((flatten))
__attribute__((alias("fn1a")))
int fn4a(int p1); /* { dg-warning "ignored" } */

int
test ()
{
  return fn4(1)+fn4a(1);
}

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
// DEFAULT-NEXT:     fn %0 @fn2(%13 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @fn3(%14 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @fn1(%3 p1: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 a: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%3));
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @fn4(%15 p1: i32) -> i32 [linkage=external] [alias="fn1"];
// DEFAULT-NEXT:     fn %7 @fn1a(%8 p1: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 a: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%0, read<i32>(%8));
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%1, read<i32>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @fn4a(%16 p1: i32) -> i32 [linkage=external] [alias="fn1a"];
// DEFAULT-NEXT:     fn %12 @test(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%6, const<i32>(1)), call<i32, signature=fn(i32) -> i32>(%11, const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
