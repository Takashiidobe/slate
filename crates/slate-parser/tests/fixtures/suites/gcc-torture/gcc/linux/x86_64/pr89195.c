/* PR rtl-optimization/89195 */
/* { dg-require-effective-target int32plus } */

struct S {
  unsigned i : 24;
};

volatile unsigned char x;

__attribute__((noipa)) int foo(struct S d) { return d.i & x; }

int main() {
  struct S d = {0x123456};
  x          = 0x75;
  if (foo(d) != (0x56 & 0x75))
    __builtin_abort();
  return 0;
}


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
// DEFAULT-NEXT:         field0 i: u32 : 24;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 3)], field_units=[Some(0)]];
// DEFAULT-NEXT:     global %1 x: volatile u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 d: @type0) -> i32 [linkage=external] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..3, bits=0..24>(%3))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8, volatile>(%1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 d: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1193046)));
// DEFAULT-NEXT:         write<u8, volatile>(%1, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(117))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(@type0) -> i32, abi=sysv64(native_c) -> scalar>(%2, copy<@type0, reason=arg>(read<@type0>(%5))), and<i32>(const<i32>(86), const<i32>(117)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
