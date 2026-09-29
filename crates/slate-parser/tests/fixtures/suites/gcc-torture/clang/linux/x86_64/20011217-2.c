// SLATE-FILECHECK-DEFINES DEFAULT

/* Test that the initializer of a compound literal is properly walked
   when tree inlining.  */
/* Origin: glibc (as reported in PR c/5105) from <aj@suse.de>.  */

inline int
finite (double __x)
{
  return (__extension__
	  (((((union { double __d; int __i[2]; }) {__d: __x}).__i[1]
	     | 0x800fffffu) + 1) >> 31));
}

int
main (void)
{
  double x = 1.0;
  
  return finite (x);
}

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 __d: f64;
// DEFAULT-NEXT:         field1 __i: array<i32, 2>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_finite:[0-9]+]] @finite(%[[VALUE___x:[0-9]+]] __x: f64) -> i32 [linkage=external] [inline=hint] [definition=inline_only] [memory=none] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(add<u32, overflow=wrap>(or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field1(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = read<f64>(%[[VALUE___x]])))), const<i32>(1))))), const<u32>(2148532223)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), const<i32>(31)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: f64 [storage=automatic] = const<f64>(1.0);
// DEFAULT-NEXT:         return call<i32, signature=fn(f64) -> i32>(%[[VALUE_finite]], read<f64>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
