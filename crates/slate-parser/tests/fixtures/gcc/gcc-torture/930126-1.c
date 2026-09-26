void abort(void);
void exit(int);

struct s {
  unsigned long long a : 8, b : 32;
};

struct s f(struct s x) {
  x.b = 0xcdef1234;
  return x;
}

int main(void) {
  static struct s i;
  i.a = 12;
  i   = f(i);
  if (i.a != 12 || i.b != 0xcdef1234)
    abort();
  exit(0);
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
// DEFAULT-NEXT:         field0 a: u64 : 8;
// DEFAULT-NEXT:         field1 b: u64 : 32;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 1], bit_offsets=[Some(0), Some(8)], bit_units=[(0, 5)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     global %6 i: @type0 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%7 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @f(%4 x: @type0) -> @type0 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..5, bits=8..40>(%4), widen<u64, reason=assign>(const<u32>(3454997044)));
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..5, bits=0..8>(%6), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(12))));
// DEFAULT-NEXT:         write<@type0>(%6, copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(native_c) -> native_c>(%3, copy<@type0, reason=arg>(read<@type0>(%6)))));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(native_c) -> native_c>(%3, copy<@type0, reason=arg>(read<@type0>(%6))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..5, bits=0..8>(%6)))), const<i32>(12)), ne<u64>(read<u64>(bitfield1<unit=0, bytes=0..5, bits=8..40>(%6)), widen<u64, reason=usual_arith>(const<u32>(3454997044))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
