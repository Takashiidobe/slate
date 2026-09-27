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

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %0 @f(%1 p: ptr<array<i32, incomplete>>, %2 q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<ptr<array<i32, incomplete>>>(read<ptr<array<i32, incomplete>>>(%1), pointer_cast<ptr<array<i32, incomplete>>, reason=usual_arith>(read<ptr<array<i32, 3>>>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @f2(%4 p: ptr<array<i32, incomplete>>, %5 q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<ptr<array<i32, incomplete>>>(read<ptr<array<i32, incomplete>>>(%4), pointer_cast<ptr<array<i32, incomplete>>, reason=usual_arith>(read<ptr<array<i32, 3>>>(%5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f3(%7 p: ptr<array<i32, incomplete>>, %8 q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<ptr<array<i32, incomplete>>>(read<ptr<array<i32, incomplete>>>(%7), pointer_cast<ptr<array<i32, incomplete>>, reason=usual_arith>(read<ptr<array<i32, 3>>>(%8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @f4(%10 p: ptr<array<i32, incomplete>>, %11 q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<ptr<array<i32, incomplete>>>(read<ptr<array<i32, incomplete>>>(%10), pointer_cast<ptr<array<i32, incomplete>>, reason=usual_arith>(read<ptr<array<i32, 3>>>(%11))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @g(%13 p: ptr<array<i32, incomplete>>, %14 q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>>(%14), pointer_cast<ptr<array<i32, 3>>, reason=usual_arith>(read<ptr<array<i32, incomplete>>>(%13))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @g2(%16 p: ptr<array<i32, incomplete>>, %17 q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>>(%17), pointer_cast<ptr<array<i32, 3>>, reason=usual_arith>(read<ptr<array<i32, incomplete>>>(%16))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @g3(%19 p: ptr<array<i32, incomplete>>, %20 q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>>(%20), pointer_cast<ptr<array<i32, 3>>, reason=usual_arith>(read<ptr<array<i32, incomplete>>>(%19))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @g4(%22 p: ptr<array<i32, incomplete>>, %23 q: ptr<array<i32, 3>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>>(%23), pointer_cast<ptr<array<i32, 3>>, reason=usual_arith>(read<ptr<array<i32, incomplete>>>(%22))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
