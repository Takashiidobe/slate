void abort(void);
void exit(int);

struct x {
  unsigned x1 : 1;
  unsigned x2 : 2;
  unsigned x3 : 3;
};

void foobar(int x, int y, int z) {
  struct x  a = {x, y, z};
  struct x  b = {x, y, z};
  struct x *c = &b;

  c->x3 += (a.x2 - a.x1) * c->x2;
  if (a.x1 != 1 || c->x3 != 5)
    abort();
  exit(0);
}

int main(void) { foobar(1, 2, 3); }


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
// DEFAULT-NEXT:     type @type0 x = struct {
// DEFAULT-NEXT:         field0 x1: u32 : 1;
// DEFAULT-NEXT:         field1 x2: u32 : 2;
// DEFAULT-NEXT:         field2 x3: u32 : 3;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0], bit_offsets=[Some(0), Some(1), Some(3)], bit_units=[(0, 1)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @foobar(%4 x: i32, %5 y: i32, %6 z: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 a: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%4)), field1 = reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%5)), field2 = reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%6)));
// DEFAULT-NEXT:         let %8 b: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%4)), field1 = reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%5)), field2 = reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%6)));
// DEFAULT-NEXT:         let %9 c: ptr<@type0> [storage=automatic] = addr_of<ptr<@type0>>(%8);
// DEFAULT-NEXT:         let %12: ptr<@type0> [synthetic] = read<ptr<@type0>>(%9);
// DEFAULT-NEXT:         let %13: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..1, bits=3..6>(deref(read<ptr<@type0>>(%12))));
// DEFAULT-NEXT:         let %14: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%13)), mul<i32, overflow=ub>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..1, bits=1..3>(%7))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%7)))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..1, bits=1..3>(deref(read<ptr<@type0>>(%9))))))));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..1, bits=3..6>(deref(read<ptr<@type0>>(%12))), read<u32>(%14));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%7))), const<i32>(1)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..1, bits=3..6>(deref(read<ptr<@type0>>(%9))))), const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32) -> void>(%3, const<i32>(1), const<i32>(2), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
