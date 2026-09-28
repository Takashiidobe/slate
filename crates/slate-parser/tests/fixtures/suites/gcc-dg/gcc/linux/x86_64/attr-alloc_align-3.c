/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

char *my_alloc1 (int len, int align) __attribute__((__alloc_align__ (2)));
char *my_alloc2 (int align, int len) __attribute__((alloc_align (1)));

int
test1 (int len)
{
  int i;
  char *p = my_alloc1 (len, 32);
  return ((__INTPTR_TYPE__) p) & 31;
}

int
test2 (int len)
{
  int i;
  char *p = my_alloc2 (32, len);
  return ((__INTPTR_TYPE__) p) & 31;
}

int
test3 (int len)
{
  int i;
  char *p = my_alloc1 (len, 16);
  return ((__INTPTR_TYPE__) p) & 15;
}

int
test4 (int len)
{
  int i;
  char *p = my_alloc2 (16, len);
  return ((__INTPTR_TYPE__) p) & 15;
}

int
test5 (int len, int align)
{
  int i;
  char *p = my_alloc1 (len, align);
  return ((__INTPTR_TYPE__) p) & 15;
}

int
test6 (int len, int align)
{
  int i;
  char *p = my_alloc2 (align, len);
  return ((__INTPTR_TYPE__) p) & 15;
}

/* { dg-final { scan-tree-dump-times "return 0" 4 "optimized" } } */

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
// DEFAULT-NEXT:     fn %2 @my_alloc1(%32 len: i32, %33 align: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %5 @my_alloc2(%34 align: i32, %35 len: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %6 @test1(%7 len: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9 p: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(i32, i32) -> ptr<i8>>(%2, read<i32>(%7), const<i32>(32));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%9)), widen<i64, reason=usual_arith>(const<i32>(31))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test2(%11 len: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 p: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(i32, i32) -> ptr<i8>>(%5, const<i32>(32), read<i32>(%11));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%13)), widen<i64, reason=usual_arith>(const<i32>(31))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test3(%15 len: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %17 p: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(i32, i32) -> ptr<i8>>(%2, read<i32>(%15), const<i32>(16));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%17)), widen<i64, reason=usual_arith>(const<i32>(15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test4(%19 len: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %21 p: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(i32, i32) -> ptr<i8>>(%5, const<i32>(16), read<i32>(%19));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%21)), widen<i64, reason=usual_arith>(const<i32>(15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @test5(%23 len: i32, %24 align: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %25 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %26 p: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(i32, i32) -> ptr<i8>>(%2, read<i32>(%23), read<i32>(%24));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%26)), widen<i64, reason=usual_arith>(const<i32>(15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @test6(%28 len: i32, %29 align: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %30 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %31 p: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(i32, i32) -> ptr<i8>>(%5, read<i32>(%29), read<i32>(%28));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%31)), widen<i64, reason=usual_arith>(const<i32>(15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
