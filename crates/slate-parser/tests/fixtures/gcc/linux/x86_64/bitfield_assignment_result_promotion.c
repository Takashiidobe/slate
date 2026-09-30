// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
struct S { unsigned bf : 8; } s;

int promoted(int x) {
  return ((s.bf = x) > -1) + (++s.bf > -1) + (s.bf++ > -1) + (s.bf-- > -1);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// IR-NEXT:         field0 bf: u32 : 8;
// IR-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_promoted:[0-9]+]] @promoted(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: u32 [synthetic, unsequenced] = reinterpret<u32>(read<i32>(%[[VALUE_x]]));
// IR-NEXT:         write<u32, unsequenced>(bitfield0<unit=0, bytes=0..1, bits=0..8>(%[[VALUE_s]]), read<u32>(%[[VALUE0]]));
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: u32 [synthetic, unsequenced] = read<u32>(bitfield0<unit=0, bytes=0..1, bits=0..8>(%[[VALUE_s]]));
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: u32 [synthetic, unsequenced] = reinterpret<u32>(add<i32>(reinterpret<i32>(read<u32>(%[[VALUE1]])), const<i32>(1)));
// IR-NEXT:         write<u32, unsequenced>(bitfield0<unit=0, bytes=0..1, bits=0..8>(%[[VALUE_s]]), read<u32>(%[[VALUE2]]));
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: u32 [synthetic, unsequenced] = read<u32>(bitfield0<unit=0, bytes=0..1, bits=0..8>(%[[VALUE_s]]));
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: u32 [synthetic, unsequenced] = reinterpret<u32>(add<i32>(reinterpret<i32>(read<u32>(%[[VALUE3]])), const<i32>(1)));
// IR-NEXT:         write<u32, unsequenced>(bitfield0<unit=0, bytes=0..1, bits=0..8>(%[[VALUE_s]]), read<u32>(%[[VALUE4]]));
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: u32 [synthetic, unsequenced] = read<u32>(bitfield0<unit=0, bytes=0..1, bits=0..8>(%[[VALUE_s]]));
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: u32 [synthetic, unsequenced] = reinterpret<u32>(sub<i32>(reinterpret<i32>(read<u32>(%[[VALUE5]])), const<i32>(1)));
// IR-NEXT:         write<u32, unsequenced>(bitfield0<unit=0, bytes=0..1, bits=0..8>(%[[VALUE_s]]), read<u32>(%[[VALUE6]]));
// IR-NEXT:         return add<i32>(add<i32>(add<i32>(from_bool<i32>(gt<i32>(reinterpret<i32>(widen<u32>(truncate<u8b>(read<u32>(%[[VALUE0]])))), neg<i32>(const<i32>(1)))), from_bool<i32>(gt<i32>(reinterpret<i32>(widen<u32>(truncate<u8b>(read<u32>(%[[VALUE2]])))), neg<i32>(const<i32>(1))))), from_bool<i32>(gt<i32>(reinterpret<i32>(read<u32>(%[[VALUE3]])), neg<i32>(const<i32>(1))))), from_bool<i32>(gt<i32>(reinterpret<i32>(read<u32>(%[[VALUE5]])), neg<i32>(const<i32>(1)))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
