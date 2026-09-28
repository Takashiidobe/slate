#include <stdio.h>

struct Bits {
  unsigned a : 3;
  int      b : 5;
  unsigned c : 1;
  unsigned d : 12;
};

struct Wide {
  unsigned long long x : 40;
  long long          y : 40;
};

int main(void) {
  struct Bits s;
  s.a = 5;
  s.b = -3;
  s.c = 1;
  s.d = 4000;
  printf("%u %d %u %u\n", s.a, s.b, s.c, s.d);

  s.a = 13;
  s.b = 20;
  s.c = 3;
  printf("%u %d %u\n", s.a, s.b, s.c);

  s.a += 4;
  s.b -= 1;
  printf("%u %d\n", s.a, s.b);

  s.a = 7;
  s.b = 15;
  s.c = 0;
  s.d = 4095;
  printf("%u %d %u %u\n", s.a, s.b, s.c, s.d);

  struct Wide w;
  w.x = 1099511627775ULL;
  w.y = -500000;
  printf("%llu %lld\n", w.x, w.y);
  w.x += 1;
  printf("%llu\n", w.x);

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
// DEFAULT-NEXT:     type @type0 Bits = struct {
// DEFAULT-NEXT:         field0 a: u32 : 3;
// DEFAULT-NEXT:         field1 b: i32 : 5;
// DEFAULT-NEXT:         field2 c: u32 : 1;
// DEFAULT-NEXT:         field3 d: u32 : 12;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 1, 1], bit_offsets=[Some(0), Some(3), Some(8), Some(9)], bit_units=[(0, 3)], field_units=[Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type1 Wide = struct {
// DEFAULT-NEXT:         field0 x: u64 : 40;
// DEFAULT-NEXT:         field1 y: i64 : 40;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8], bit_offsets=[Some(0), Some(64)], bit_units=[(0, 5), (8, 5)], field_units=[Some(0), Some(1)]];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 117, 32, 37, 100, 32, 37, 117, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 117, 32, 37, 100, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 117, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 117, 32, 37, 100, 32, 37, 117, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([37, 108, 108, 117, 32, 37, 108, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%7 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..3, bits=0..3>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(5)));
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..3, bits=3..8>(%5), neg<i32, overflow=ub>(const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..3, bits=8..9>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..3, bits=9..21>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(4000)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%8)), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..3, bits=0..3>(%5))), read<i32>(bitfield1<unit=0, bytes=0..3, bits=3..8>(%5)), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..3, bits=8..9>(%5))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..3, bits=9..21>(%5))));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..3, bits=0..3>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(13)));
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..3, bits=3..8>(%5), const<i32>(20));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..3, bits=8..9>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%9)), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..3, bits=0..3>(%5))), read<i32>(bitfield1<unit=0, bytes=0..3, bits=3..8>(%5)), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..3, bits=8..9>(%5))));
// DEFAULT-NEXT:         let %14: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..3, bits=0..3>(%5));
// DEFAULT-NEXT:         let %15: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%14)), const<i32>(4)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..3, bits=0..3>(%5), read<u32>(%15));
// DEFAULT-NEXT:         let %16: i32 [synthetic] = read<i32>(bitfield1<unit=0, bytes=0..3, bits=3..8>(%5));
// DEFAULT-NEXT:         let %17: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..3, bits=3..8>(%5), read<i32>(%17));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%10)), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..3, bits=0..3>(%5))), read<i32>(bitfield1<unit=0, bytes=0..3, bits=3..8>(%5)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..3, bits=0..3>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(7)));
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=0, bytes=0..3, bits=3..8>(%5), const<i32>(15));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..3, bits=8..9>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..3, bits=9..21>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(4095)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%11)), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..3, bits=0..3>(%5))), read<i32>(bitfield1<unit=0, bytes=0..3, bits=3..8>(%5)), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..3, bits=8..9>(%5))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..3, bits=9..21>(%5))));
// DEFAULT-NEXT:         let %6 w: @type1 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..5, bits=0..40>(%6), const<u64>(1099511627775));
// DEFAULT-NEXT:         write<i64>(bitfield1<unit=1, bytes=8..13, bits=0..40>(%6), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(500000))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%12)), read<u64>(bitfield0<unit=0, bytes=0..5, bits=0..40>(%6)), read<i64>(bitfield1<unit=1, bytes=8..13, bits=0..40>(%6)));
// DEFAULT-NEXT:         let %18: u64 [synthetic] = read<u64>(bitfield0<unit=0, bytes=0..5, bits=0..40>(%6));
// DEFAULT-NEXT:         let %19: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%18), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..5, bits=0..40>(%6), read<u64>(%19));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%13)), read<u64>(bitfield0<unit=0, bytes=0..5, bits=0..40>(%6)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
