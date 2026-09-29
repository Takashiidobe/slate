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
// DEFAULT-NEXT:     fn %[[VALUE_my_alloc1:[0-9]+]] @my_alloc1(%[[VALUE_len:[0-9]+]] len: i32, %[[VALUE_align:[0-9]+]] align: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_my_alloc2:[0-9]+]] @my_alloc2(%[[VALUE_align_2:[0-9]+]] align: i32, %[[VALUE_len_2:[0-9]+]] len: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_len_3:[0-9]+]] len: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(i32, i32) -> ptr<i8>>(%[[VALUE_my_alloc1]], read<i32>(%[[VALUE_len_3]]), const<i32>(32));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%[[VALUE_p]])), widen<i64, reason=usual_arith>(const<i32>(31))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_len_4:[0-9]+]] len: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(i32, i32) -> ptr<i8>>(%[[VALUE_my_alloc2]], const<i32>(32), read<i32>(%[[VALUE_len_4]]));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%[[VALUE_p_2]])), widen<i64, reason=usual_arith>(const<i32>(31))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_len_5:[0-9]+]] len: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_3:[0-9]+]] p: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(i32, i32) -> ptr<i8>>(%[[VALUE_my_alloc1]], read<i32>(%[[VALUE_len_5]]), const<i32>(16));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%[[VALUE_p_3]])), widen<i64, reason=usual_arith>(const<i32>(15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_len_6:[0-9]+]] len: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_4:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_4:[0-9]+]] p: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(i32, i32) -> ptr<i8>>(%[[VALUE_my_alloc2]], const<i32>(16), read<i32>(%[[VALUE_len_6]]));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%[[VALUE_p_4]])), widen<i64, reason=usual_arith>(const<i32>(15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5(%[[VALUE_len_7:[0-9]+]] len: i32, %[[VALUE_align_3:[0-9]+]] align: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_5:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_5:[0-9]+]] p: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(i32, i32) -> ptr<i8>>(%[[VALUE_my_alloc1]], read<i32>(%[[VALUE_len_7]]), read<i32>(%[[VALUE_align_3]]));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%[[VALUE_p_5]])), widen<i64, reason=usual_arith>(const<i32>(15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6:[0-9]+]] @test6(%[[VALUE_len_8:[0-9]+]] len: i32, %[[VALUE_align_4:[0-9]+]] align: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i_6:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_6:[0-9]+]] p: ptr<i8> [storage=automatic] = call<ptr<i8>, signature=fn(i32, i32) -> ptr<i8>>(%[[VALUE_my_alloc2]], read<i32>(%[[VALUE_align_4]]), read<i32>(%[[VALUE_len_8]]));
// DEFAULT-NEXT:         return truncate<i32, reason=return, fits=unknown>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<i8>>(%[[VALUE_p_6]])), widen<i64, reason=usual_arith>(const<i32>(15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
