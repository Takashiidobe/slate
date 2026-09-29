_BitInt(256) large = 340282366920938463463374607431768211456wb;
_BitInt(8) signed_wb = 1wb;
unsigned _BitInt(8) unsigned_uwb = 2uwb;
unsigned _BitInt(8) unsigned_wbu = 3wbu;
unsigned _BitInt(8) unsigned_upper_uwb = 4UWB;
unsigned _BitInt(8) unsigned_upper_wbu = 5WBU;
unsigned _BitInt(8) unsigned_mixed_uwb = 6uWb;
unsigned _BitInt(8) unsigned_mixed_wbu = 7wBu;


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
// DEFAULT-NEXT:     global %[[VALUE_large:[0-9]+]] large: i256b [storage=static] = widen<i256b, reason=assign>(const<i130b>(340282366920938463463374607431768211456)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_signed_wb:[0-9]+]] signed_wb: i8b [storage=static] = widen<i8b, reason=assign>(const<i2b>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_unsigned_uwb:[0-9]+]] unsigned_uwb: u8b [storage=static] = widen<u8b, reason=assign>(const<u2b>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_unsigned_wbu:[0-9]+]] unsigned_wbu: u8b [storage=static] = widen<u8b, reason=assign>(const<u2b>(3)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_unsigned_upper_uwb:[0-9]+]] unsigned_upper_uwb: u8b [storage=static] = widen<u8b, reason=assign>(const<u3b>(4)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_unsigned_upper_wbu:[0-9]+]] unsigned_upper_wbu: u8b [storage=static] = widen<u8b, reason=assign>(const<u3b>(5)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_unsigned_mixed_uwb:[0-9]+]] unsigned_mixed_uwb: u8b [storage=static] = widen<u8b, reason=assign>(const<u3b>(6)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_unsigned_mixed_wbu:[0-9]+]] unsigned_mixed_wbu: u8b [storage=static] = widen<u8b, reason=assign>(const<u3b>(7)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
