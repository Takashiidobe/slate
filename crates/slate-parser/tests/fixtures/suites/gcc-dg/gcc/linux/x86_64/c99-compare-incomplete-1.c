/* Test comparisons of pointers to complete and incomplete types are
   diagnosed in C99 mode: -pedantic.  */
/* { dg-do compile } */
/* { dg-options "-std=c99 -pedantic" } */

int
f (int (*p)[], int (*q)[3])
{
  return p < q; /* { dg-warning "complete and incomplete" } */
}

int
f2 (int (*p)[], int (*q)[3])
{
  return p <= q; /* { dg-warning "complete and incomplete" } */
}

int
f3 (int (*p)[], int (*q)[3])
{
  return p > q; /* { dg-warning "complete and incomplete" } */
}

int
f4 (int (*p)[], int (*q)[3])
{
  return p >= q; /* { dg-warning "complete and incomplete" } */
}

int
g (int (*p)[], int (*q)[3])
{
  return q < p; /* { dg-warning "complete and incomplete" } */
}

int
g2 (int (*p)[], int (*q)[3])
{
  return q <= p; /* { dg-warning "complete and incomplete" } */
}

int
g3 (int (*p)[], int (*q)[3])
{
  return q > p; /* { dg-warning "complete and incomplete" } */
}

int
g4 (int (*p)[], int (*q)[3])
{
  return q >= p; /* { dg-warning "complete and incomplete" } */
}

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_p:[0-9]+]] p: ptr<array<i32, incomplete>>, %[[VALUE_q:[0-9]+]] q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<ptr<array<i32, incomplete>>>(read<ptr<array<i32, incomplete>>>(%[[VALUE_p]]), pointer_cast<ptr<array<i32, incomplete>>, reason=usual_arith>(read<ptr<array<i32, 3>>>(%[[VALUE_q]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_p_2:[0-9]+]] p: ptr<array<i32, incomplete>>, %[[VALUE_q_2:[0-9]+]] q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<ptr<array<i32, incomplete>>>(read<ptr<array<i32, incomplete>>>(%[[VALUE_p_2]]), pointer_cast<ptr<array<i32, incomplete>>, reason=usual_arith>(read<ptr<array<i32, 3>>>(%[[VALUE_q_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_p_3:[0-9]+]] p: ptr<array<i32, incomplete>>, %[[VALUE_q_3:[0-9]+]] q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<ptr<array<i32, incomplete>>>(read<ptr<array<i32, incomplete>>>(%[[VALUE_p_3]]), pointer_cast<ptr<array<i32, incomplete>>, reason=usual_arith>(read<ptr<array<i32, 3>>>(%[[VALUE_q_3]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_p_4:[0-9]+]] p: ptr<array<i32, incomplete>>, %[[VALUE_q_4:[0-9]+]] q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<ptr<array<i32, incomplete>>>(read<ptr<array<i32, incomplete>>>(%[[VALUE_p_4]]), pointer_cast<ptr<array<i32, incomplete>>, reason=usual_arith>(read<ptr<array<i32, 3>>>(%[[VALUE_q_4]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE_p_5:[0-9]+]] p: ptr<array<i32, incomplete>>, %[[VALUE_q_5:[0-9]+]] q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>>(%[[VALUE_q_5]]), pointer_cast<ptr<array<i32, 3>>, reason=usual_arith>(read<ptr<array<i32, incomplete>>>(%[[VALUE_p_5]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g2:[0-9]+]] @g2(%[[VALUE_p_6:[0-9]+]] p: ptr<array<i32, incomplete>>, %[[VALUE_q_6:[0-9]+]] q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>>(%[[VALUE_q_6]]), pointer_cast<ptr<array<i32, 3>>, reason=usual_arith>(read<ptr<array<i32, incomplete>>>(%[[VALUE_p_6]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g3:[0-9]+]] @g3(%[[VALUE_p_7:[0-9]+]] p: ptr<array<i32, incomplete>>, %[[VALUE_q_7:[0-9]+]] q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>>(%[[VALUE_q_7]]), pointer_cast<ptr<array<i32, 3>>, reason=usual_arith>(read<ptr<array<i32, incomplete>>>(%[[VALUE_p_7]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g4:[0-9]+]] @g4(%[[VALUE_p_8:[0-9]+]] p: ptr<array<i32, incomplete>>, %[[VALUE_q_8:[0-9]+]] q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>>(%[[VALUE_q_8]]), pointer_cast<ptr<array<i32, 3>>, reason=usual_arith>(read<ptr<array<i32, incomplete>>>(%[[VALUE_p_8]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
