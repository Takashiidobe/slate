/* PR tree-optimization/113736 */
/* { dg-do compile { target bitint } } */
/* { dg-options "-O2 -std=gnu23 -w" } */

#if __BITINT_MAXWIDTH__ >= 710
struct S { _BitInt(710) a; };
struct T { struct S b[4]; };

#ifdef __x86_64__
#define SEG __seg_gs
#elif defined __i386__
#define SEG __seg_fs
#else
#define SEG
#endif

void
foo (SEG struct T *p)
{
  struct S s;
  p->b[0] = s;
}

void
bar (SEG struct T *p, _BitInt(710) x, int y, double z)
{
  p->b[0].a = x + 42;
  p->b[1].a = x << y;
  p->b[2].a = x >> y;
  p->b[3].a = z;
}

int
baz (SEG struct T *p, _BitInt(710) x, _BitInt(710) y)
{
  return __builtin_add_overflow (x, y, &p->b[1].a);
}
#else
int i;
#endif

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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: i710b;
// DEFAULT-NEXT:     } [size=96, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = struct {
// DEFAULT-NEXT:         field0 b: array<@type[[TYPE_S]], 4>;
// DEFAULT-NEXT:     } [size=384, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_T]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p]])))), const<i32>(0))), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_s]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_T]]>, %[[VALUE_x:[0-9]+]] x: i710b, %[[VALUE_y:[0-9]+]] y: i32, %[[VALUE_z:[0-9]+]] z: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i710b>(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p_2]])))), const<i32>(0)))), add<i710b, overflow=ub>(read<i710b>(%[[VALUE_x]]), widen<i710b, reason=usual_arith>(const<i32>(42))));
// DEFAULT-NEXT:         write<i710b>(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p_2]])))), const<i32>(1)))), shl<i710b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i710b>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]])));
// DEFAULT-NEXT:         write<i710b>(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p_2]])))), const<i32>(2)))), shr<i710b, amount_out_of_range=ub, fill=sign_extend>(read<i710b>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]])));
// DEFAULT-NEXT:         write<i710b>(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p_2]])))), const<i32>(3)))), float_to_int<i710b, reason=assign, out_of_range=ub, exceptions=observable>(read<f64>(%[[VALUE_z]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_p_3:[0-9]+]] p: ptr<@type[[TYPE_T]]>, %[[VALUE_x_2:[0-9]+]] x: i710b, %[[VALUE_y_2:[0-9]+]] y: i710b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(overflow_add<bool>(read<i710b>(%[[VALUE_x_2]]), read<i710b>(%[[VALUE_y_2]]), deref(addr_of<ptr<i710b>>(field0(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_p_3]])))), const<i32>(1))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
