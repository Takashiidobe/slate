/* { dg-require-effective-target int32plus } */
extern void abort(void);

struct s {
  unsigned long long a : 16;
  unsigned long long b : 32;
  unsigned long long c : 16;
};

__attribute__((noinline)) unsigned f(struct s s, unsigned i) {
  return s.b == i;
}

struct s s = {1, 0x87654321u, 2};

int main() {
  if (!f(s, 0x87654321u))
    abort();
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 a: u64 : 16;
// DEFAULT-NEXT:         field1 b: u64 : 32;
// DEFAULT-NEXT:         field2 c: u64 : 16;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 2, 6], bit_offsets=[Some(0), Some(16), Some(48)], bit_units=[(0, 8)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %5 s: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1))), field1 = widen<u64, reason=assign>(const<u32>(2271560481)), field2 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(2)))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f(%3 s: @type0, %4 i: u32) -> u32 [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<u32, reason=return>(eq<u64>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=16..48>(%3)), widen<u64, reason=usual_arith>(read<u32>(%4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if not<bool>(ne<u32>(call<u32, signature=fn(@type0, u32) -> u32, abi=sysv64(native_c, scalar) -> scalar>(%2, copy<@type0, reason=arg>(read<@type0>(%5)), const<u32>(2271560481)), const<u32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
