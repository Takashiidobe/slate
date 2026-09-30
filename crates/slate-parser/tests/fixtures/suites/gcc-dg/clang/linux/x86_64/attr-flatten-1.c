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
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn3:[0-9]+]] @fn3(%[[VALUE1:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1(%[[VALUE_p1:[0-9]+]] p1: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%[[VALUE_fn2]], read<i32>(%[[VALUE_p1]]));
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%[[VALUE_fn3]], read<i32>(%[[VALUE_a]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4:[0-9]+]] @fn4(%[[VALUE_p1_2:[0-9]+]] p1: i32) -> i32 [linkage=external] [alias="fn1"];
// DEFAULT-NEXT:     fn %[[VALUE_fn1a:[0-9]+]] @fn1a(%[[VALUE_p1_3:[0-9]+]] p1: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%[[VALUE_fn2]], read<i32>(%[[VALUE_p1_3]]));
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%[[VALUE_fn3]], read<i32>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4a:[0-9]+]] @fn4a(%[[VALUE_p1_4:[0-9]+]] p1: i32) -> i32 [linkage=external] [alias="fn1a"];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_fn4]], const<i32>(1)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_fn4a]], const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
