// SLATE-FILECHECK-DEFINES DEFAULT

typedef struct
{
  long double l;
} ld;

ld a (ld x, ld y)
{
  ld b;
  b.l = x.l + y.l;
}

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 l: f80;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_ld:[0-9]+]] ld = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_a:[0-9]+]] @a(%[[VALUE_x:[0-9]+]] x: @type[[TYPE0]], %[[VALUE_y:[0-9]+]] y: @type[[TYPE0]]) -> @type[[TYPE0]] [linkage=external] [abi=sysv64(byval<align=16>, byval<align=16>) -> coerce<f80>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<f80>(field0(%[[VALUE_b]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(field0(%[[VALUE_x]])), read<f80>(field0(%[[VALUE_y]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
