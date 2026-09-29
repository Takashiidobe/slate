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
// DEFAULT-NEXT:     type @type[[TYPE_x:[0-9]+]] x = struct {
// DEFAULT-NEXT:         field0 x1: u32 : 1;
// DEFAULT-NEXT:         field1 x2: u32 : 2;
// DEFAULT-NEXT:         field2 x3: u32 : 3;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0], bit_offsets=[Some(0), Some(1), Some(3)], bit_units=[(0, 1)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foobar:[0-9]+]] @foobar(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32, %[[VALUE_z:[0-9]+]] z: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_x]] [storage=automatic] = aggregate<@type[[TYPE_x]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%[[VALUE_x]])), field1 = reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%[[VALUE_y]])), field2 = reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%[[VALUE_z]])));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_x]] [storage=automatic] = aggregate<@type[[TYPE_x]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%[[VALUE_x]])), field1 = reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%[[VALUE_y]])), field2 = reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%[[VALUE_z]])));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: ptr<@type[[TYPE_x]]> [storage=automatic] = addr_of<ptr<@type[[TYPE_x]]>>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<@type[[TYPE_x]]> [synthetic] = read<ptr<@type[[TYPE_x]]>>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..1, bits=3..6>(deref(read<ptr<@type[[TYPE_x]]>>(%[[VALUE1]]))));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE2]])), mul<i32, overflow=ub>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..1, bits=1..3>(%[[VALUE_a]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_a]])))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..1, bits=1..3>(deref(read<ptr<@type[[TYPE_x]]>>(%[[VALUE_c]]))))))));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..1, bits=3..6>(deref(read<ptr<@type[[TYPE_x]]>>(%[[VALUE1]]))), read<u32>(%[[VALUE3]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..1, bits=0..1>(%[[VALUE_a]]))), const<i32>(1)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..1, bits=3..6>(deref(read<ptr<@type[[TYPE_x]]>>(%[[VALUE_c]]))))), const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32) -> void>(%[[VALUE_foobar]], const<i32>(1), const<i32>(2), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
