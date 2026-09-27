/* Test for non-lvalue arrays: test that qualifiers on non-lvalues
   containing arrays do not remain when those arrays decay to
   pointers.  PR 35235.  */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

int a;

void
f (void)
{
  const struct {
    int a[1];
  } s;
  int *p1 = s.a; /* { dg-error "qualifier" } */
  int *p2 = (a ? s : s).a;
  /* In this case, the qualifier is properly on the array element type
     not on the rvalue structure and so is not discarded.  */
  struct {
    const int a[1];
  } t;
  int *p3 = t.a; /* { dg-error "qualifier" } */
  int *p4 = (a ? t : t).a; /* { dg-error "qualifier" } */
  /* The issue could also lead to code being wrongly accepted.  */
  const struct {
    int a[1][1];
  } u;
  const int (*p5)[1] = u.a;
  const int (*p6)[1] = (a ? u : u).a; /* { dg-error "pointer" } */
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 a: array<i32, 1>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 a: const array<i32, 1>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 a: array<array<i32, 1>, 1>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 s: @type0 [storage=automatic] [const];
// DEFAULT-NEXT:         let %4 p1: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(array_decay<ptr<const i32>, length=Some(1)>(field0(%3)));
// DEFAULT-NEXT:         let %5 p2: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(1)>(field0(temporary %14 = conditional<@type0>(ne<i32>(read<i32>(%0), const<i32>(0)), read<@type0>(%3), read<@type0>(%3))));
// DEFAULT-NEXT:         let %7 t: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %8 p3: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(array_decay<ptr<const i32>, length=Some(1)>(field0(%7)));
// DEFAULT-NEXT:         let %9 p4: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(array_decay<ptr<const i32>, length=Some(1)>(field0(temporary %15 = conditional<@type1>(ne<i32>(read<i32>(%0), const<i32>(0)), read<@type1>(%7), read<@type1>(%7)))));
// DEFAULT-NEXT:         let %11 u: @type2 [storage=automatic] [const];
// DEFAULT-NEXT:         let %12 p5: ptr<const array<i32, 1>> [storage=automatic] = array_decay<ptr<const array<i32, 1>>, length=Some(1)>(field0(%11));
// DEFAULT-NEXT:         let %13 p6: ptr<const array<i32, 1>> [storage=automatic] = pointer_cast<ptr<const array<i32, 1>>, reason=assign>(array_decay<ptr<array<i32, 1>>, length=Some(1)>(field0(temporary %16 = conditional<@type2>(ne<i32>(read<i32>(%0), const<i32>(0)), read<@type2>(%11), read<@type2>(%11)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
