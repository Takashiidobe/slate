// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata -std=c23

_BitInt(2) narrow_signed = 1wb;
unsigned _BitInt(1) narrow_unsigned = 0uwb;
_BitInt(9) hex_signed = 0xffwb;
unsigned _BitInt(8) hex_unsigned = 255uwb;
_BitInt(200) wide = 170141183460469231731687303715884105728wb;
unsigned _BitInt(4) suffix_order = 12wbu;

int widened(void) { return (int)(3wb + 4wb); }

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
// IR-NEXT:     global %[[VALUE_narrow_signed:[0-9]+]] narrow_signed: i2b [storage=static] = const<i2b>(1) [linkage=external] [c="_BitInt(2)"];
// IR-NEXT:     global %[[VALUE_narrow_unsigned:[0-9]+]] narrow_unsigned: u1b [storage=static] = const<u1b>(0) [linkage=external] [c="unsigned _BitInt(1)"];
// IR-NEXT:     global %[[VALUE_hex_signed:[0-9]+]] hex_signed: i9b [storage=static] = const<i9b>(255) [linkage=external] [c="_BitInt(9)"];
// IR-NEXT:     global %[[VALUE_hex_unsigned:[0-9]+]] hex_unsigned: u8b [storage=static] = const<u8b>(255) [linkage=external] [c="unsigned _BitInt(8)"];
// IR-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: i200b [storage=static] = widen<i200b, reason=assign>(const<i129b>(170141183460469231731687303715884105728)) [linkage=external] [c="_BitInt(200)"];
// IR-NEXT:     global %[[VALUE_suffix_order:[0-9]+]] suffix_order: u4b [storage=static] = const<u4b>(12) [linkage=external] [c="unsigned _BitInt(4)"];
// IR-NEXT:     fn %[[VALUE_widened:[0-9]+]] @widened() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// IR-NEXT:         return widen<i32, reason=explicit>(add<i4b, overflow=ub>(widen<i4b, reason=usual_arith>(const<i3b>(3)), const<i4b>(4)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
