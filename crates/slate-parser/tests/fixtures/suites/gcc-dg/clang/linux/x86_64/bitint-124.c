/* PR tree-optimization/121131 */
/* { dg-do run { target bitint } } */
/* { dg-options "-O2" } */

#if __BITINT_MAXWIDTH__ >= 156
struct A { _BitInt(156) b : 135; };

static inline _BitInt(156)
foo (struct A *x)
{
  return x[1].b;
}

__attribute__((noipa)) _BitInt(156)
bar (void)
{
  struct A a[] = { 1, 1, -13055525270329736316393717310914023773847wb,
		   1, 1, 1, 1, 1, 1, 1, 1, 1 };
  return foo (&a[1]);
}
#endif

int
main ()
{
#if __BITINT_MAXWIDTH__ >= 156
  if (bar () != -13055525270329736316393717310914023773847wb)
    __builtin_abort ();
#endif
}

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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 b: i156b : 135;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 17)], field_units=[Some(0)]];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: ptr<@type0>) -> i156b [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i156b>(bitfield0<unit=0, bytes=0..17, bits=0..135>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%2), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar() -> i156b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 a: array<@type0, 12> [storage=automatic] [align=16] = aggregate<array<@type0, 12>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index1 = aggregate<@type0, zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index2 = aggregate<@type0, zero_fill=false>(field0 = widen<i156b, reason=assign>(neg<i135b, overflow=ub>(const<i135b>(13055525270329736316393717310914023773847)))), index3 = aggregate<@type0, zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index4 = aggregate<@type0, zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index5 = aggregate<@type0, zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index6 = aggregate<@type0, zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index7 = aggregate<@type0, zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index8 = aggregate<@type0, zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index9 = aggregate<@type0, zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index10 = aggregate<@type0, zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index11 = aggregate<@type0, zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))));
// DEFAULT-NEXT:         return call<i156b, signature=fn(ptr<@type0>) -> i156b>(%1, addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(12)>(%4), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i156b>(call<i156b, signature=fn() -> i156b>(%3), widen<i156b, reason=usual_arith>(neg<i135b, overflow=ub>(const<i135b>(13055525270329736316393717310914023773847))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
