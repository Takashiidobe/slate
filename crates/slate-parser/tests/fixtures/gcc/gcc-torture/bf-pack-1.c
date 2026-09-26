void abort(void);
void exit(int);

struct foo {
  unsigned      half  : 16;
  unsigned long whole : 32 __attribute__((packed));
};

void f(struct foo *q) {
  if (q->half != 0x1234)
    abort();
  if (q->whole != 0x56789abcL)
    abort();
}

int main(void) {
  struct foo bar;

  bar.half  = 0x1234;
  bar.whole = 0x56789abcL;
  f(&bar);
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
// DEFAULT-NEXT:     type @type0 foo = struct {
// DEFAULT-NEXT:         field0 half: u32 : 16;
// DEFAULT-NEXT:         field1 whole: u64 : 32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 2], bit_offsets=[Some(0), Some(16)], bit_units=[(0, 6)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%7 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @f(%4 q: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..6, bits=0..16>(deref(read<ptr<@type0>>(%4))))), const<i32>(4660))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<u64>(read<u64>(bitfield1<unit=0, bytes=0..6, bits=16..48>(deref(read<ptr<@type0>>(%4)))), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1450744508)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 bar: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..6, bits=0..16>(%6), reinterpret<u32, reason=assign, fits=always>(const<i32>(4660)));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..6, bits=16..48>(%6), reinterpret<u64, reason=assign, fits=always>(const<i64>(1450744508)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%3, addr_of<ptr<@type0>>(%6));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
