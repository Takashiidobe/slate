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
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 half: u32 : 16;
// DEFAULT-NEXT:         field1 whole: u64 : 32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 2], bit_offsets=[Some(0), Some(16)], bit_units=[(0, 6)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_q:[0-9]+]] q: ptr<@type[[TYPE_foo]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..6, bits=0..16>(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_q]]))))), const<i32>(4660))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(read<u64>(bitfield1<unit=0, bytes=0..6, bits=16..48>(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_q]])))), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1450744508)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_bar:[0-9]+]] bar: @type[[TYPE_foo]] [storage=automatic];
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..6, bits=0..16>(%[[VALUE_bar]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(4660)));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..6, bits=16..48>(%[[VALUE_bar]]), reinterpret<u64, reason=assign, fits=always>(const<i64>(1450744508)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_foo]]>) -> void>(%[[VALUE_f]], addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_bar]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
