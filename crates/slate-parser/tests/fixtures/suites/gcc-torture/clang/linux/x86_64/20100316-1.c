struct Foo {
  int      i;
  unsigned precision : 10;
  unsigned blah      : 3;
} f;

int __attribute__((noinline, noclone)) foo(struct Foo *p) {
  struct Foo *q = p;
  return (*q).precision;
}

extern void abort(void);

int main() {
  f.i         = -1;
  f.precision = 0;
  f.blah      = -1;
  if (foo(&f) != 0)
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
// DEFAULT-NEXT:     type @type[[TYPE_Foo:[0-9]+]] Foo = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 precision: u32 : 10;
// DEFAULT-NEXT:         field2 blah: u32 : 3;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4, 5], bit_offsets=[None, Some(32), Some(42)], bit_units=[(4, 2)], field_units=[None, Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: @type[[TYPE_Foo]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_Foo]]>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<@type[[TYPE_Foo]]> [storage=automatic] = read<ptr<@type[[TYPE_Foo]]>>(%[[VALUE_p]]);
// DEFAULT-NEXT:         return reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..6, bits=0..10>(deref(read<ptr<@type[[TYPE_Foo]]>>(%[[VALUE_q]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_f]]), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..6, bits=0..10>(%[[VALUE_f]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=4..6, bits=10..13>(%[[VALUE_f]]), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_Foo]]>) -> i32>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_Foo]]>>(%[[VALUE_f]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
