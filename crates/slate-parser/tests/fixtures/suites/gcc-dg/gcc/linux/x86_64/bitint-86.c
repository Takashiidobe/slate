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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i710b;
// DEFAULT-NEXT:     } [size=96, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 T = struct {
// DEFAULT-NEXT:         field0 b: array<@type0, 4>;
// DEFAULT-NEXT:     } [size=384, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %2 @foo(%3 p: ptr<@type1>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<@type0>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(field0(deref(read<ptr<@type1>>(%3)))), const<i32>(0))), copy<@type0, reason=assign>(read<@type0>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar(%6 p: ptr<@type1>, %7 x: i710b, %8 y: i32, %9 z: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i710b>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(field0(deref(read<ptr<@type1>>(%6)))), const<i32>(0)))), add<i710b, overflow=ub>(read<i710b>(%7), widen<i710b, reason=usual_arith>(const<i32>(42))));
// DEFAULT-NEXT:         write<i710b>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(field0(deref(read<ptr<@type1>>(%6)))), const<i32>(1)))), shl<i710b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i710b>(%7), read<i32>(%8)));
// DEFAULT-NEXT:         write<i710b>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(field0(deref(read<ptr<@type1>>(%6)))), const<i32>(2)))), shr<i710b, amount_out_of_range=ub, fill=sign_extend>(read<i710b>(%7), read<i32>(%8)));
// DEFAULT-NEXT:         write<i710b>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(field0(deref(read<ptr<@type1>>(%6)))), const<i32>(3)))), float_to_int<i710b, reason=assign, out_of_range=ub, exceptions=observable>(read<f64>(%9)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @baz(%11 p: ptr<@type1>, %12 x: i710b, %13 y: i710b) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(overflow_add<bool>(read<i710b>(%12), read<i710b>(%13), deref(addr_of<ptr<i710b>>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(4)>(field0(deref(read<ptr<@type1>>(%11)))), const<i32>(1))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
