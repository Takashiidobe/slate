
struct a {
  int b : 4;
} d;
static int            c, e;
static const struct a f;
static void           g(const struct a h) {
  for (; c < 1; c++)
    d = h;
  e = h.b;
  c = h.b;
}
int main() {
  g(f);
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
// DEFAULT-NEXT:     type @type[[TYPE_a:[0-9]+]] a = struct {
// DEFAULT-NEXT:         field0 b: i32 : 4;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: @type[[TYPE_a]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: @type[[TYPE_a]] [storage=static] [const] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE_h:[0-9]+]] h: @type[[TYPE_a]] [const]) -> void [linkage=internal] [abi=sysv64(native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_c]]), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_c]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<@type[[TYPE_a]]>(%[[VALUE_d]], copy<@type[[TYPE_a]], reason=assign>(read<@type[[TYPE_a]]>(%[[VALUE_h]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_e]], read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..4>(%[[VALUE_h]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_c]], read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..4>(%[[VALUE_h]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_a]]) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_g]], copy<@type[[TYPE_a]], reason=arg>(read<@type[[TYPE_a]]>(%[[VALUE_f]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
