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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: array<i32, 1>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: const array<i32, 1>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: array<array<i32, 1>, 1>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE0]] [storage=automatic] [const];
// DEFAULT-NEXT:         let %[[VALUE_p1:[0-9]+]] p1: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(array_decay<ptr<const i32>, length=Some(1)>(field0(%[[VALUE_s]])));
// DEFAULT-NEXT:         let %[[VALUE_p2:[0-9]+]] p2: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(1)>(field0(temporary %[[VALUE0:[0-9]+]] = conditional<@type[[TYPE0]]>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), read<@type[[TYPE0]]>(%[[VALUE_s]]), read<@type[[TYPE0]]>(%[[VALUE_s]]))));
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p3:[0-9]+]] p3: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(array_decay<ptr<const i32>, length=Some(1)>(field0(%[[VALUE_t]])));
// DEFAULT-NEXT:         let %[[VALUE_p4:[0-9]+]] p4: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(array_decay<ptr<const i32>, length=Some(1)>(field0(temporary %[[VALUE1:[0-9]+]] = conditional<@type[[TYPE1]]>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), read<@type[[TYPE1]]>(%[[VALUE_t]]), read<@type[[TYPE1]]>(%[[VALUE_t]])))));
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE2]] [storage=automatic] [const];
// DEFAULT-NEXT:         let %[[VALUE_p5:[0-9]+]] p5: ptr<const array<i32, 1>> [storage=automatic] = array_decay<ptr<const array<i32, 1>>, length=Some(1)>(field0(%[[VALUE_u]]));
// DEFAULT-NEXT:         let %[[VALUE_p6:[0-9]+]] p6: ptr<const array<i32, 1>> [storage=automatic] = pointer_cast<ptr<const array<i32, 1>>, reason=assign>(array_decay<ptr<array<i32, 1>>, length=Some(1)>(field0(temporary %[[VALUE2:[0-9]+]] = conditional<@type[[TYPE2]]>(ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0)), read<@type[[TYPE2]]>(%[[VALUE_u]]), read<@type[[TYPE2]]>(%[[VALUE_u]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
