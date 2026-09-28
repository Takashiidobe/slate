/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

char *my_alloc1 (int len) __attribute__((__assume_aligned__ (32)));
char *my_alloc2 (int len) __attribute__((assume_aligned (32, 4)));

int
test1 (int len)
{
  int i;
  char *p = my_alloc1 (len);
  return ((__INTPTR_TYPE__) p) & 31;
}

int
test2 (int len)
{
  int i;
  char *p = my_alloc2 (len);
  return (((__INTPTR_TYPE__) p) & 31) != 4;
}

/* { dg-final { scan-tree-dump-times "return 0" 2 "optimized" } } */

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
// DEFAULT-NEXT:     fn %1 @my_alloc1(%12 len: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %3 @my_alloc2(%13 len: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %4 @test1(%5 len: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 p: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(i32) -> ptr<i8>>(%1, read<i32>(%5));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%7)), widen<i64, reason=usual_arith>(const<i32>(31))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test2(%9 len: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 p: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(i32) -> ptr<i8>>(%3, read<i32>(%9));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%11)), widen<i64, reason=usual_arith>(const<i32>(31))), widen<i64, reason=usual_arith>(const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
