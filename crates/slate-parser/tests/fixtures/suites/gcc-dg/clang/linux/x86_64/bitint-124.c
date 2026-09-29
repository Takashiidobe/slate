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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 b: i156b : 135;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 17)], field_units=[Some(0)]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_A]]>) -> i156b [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i156b>(bitfield0<unit=0, bytes=0..17, bits=0..135>(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_x]]), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> i156b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<@type[[TYPE_A]], 12> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_A]], 12>, zero_fill=false>(index0 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index1 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index2 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = widen<i156b, reason=assign>(neg<i135b, overflow=ub>(const<i135b>(13055525270329736316393717310914023773847)))), index3 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index4 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index5 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index6 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index7 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index8 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index9 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index10 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))), index11 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = widen<i156b, reason=assign>(const<i32>(1))));
// DEFAULT-NEXT:         return call<i156b, signature=fn(ptr<@type[[TYPE_A]]>) -> i156b>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_A]]>>(deref(ptr_offset<ptr<@type[[TYPE_A]]>, subtract=false, element=@type[[TYPE_A]], overflow=ub>(array_decay<ptr<@type[[TYPE_A]]>, length=Some(12)>(%[[VALUE_a]]), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i156b>(call<i156b, signature=fn() -> i156b>(%[[VALUE_bar]]), widen<i156b, reason=usual_arith>(neg<i135b, overflow=ub>(const<i135b>(13055525270329736316393717310914023773847))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
