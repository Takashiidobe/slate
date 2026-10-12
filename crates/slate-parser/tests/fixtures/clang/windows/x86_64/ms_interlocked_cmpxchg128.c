// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT -Wno-implicit-function-declaration

unsigned char Cmpxchg128(__int64 volatile *d, __int64 h, __int64 l, __int64 *c) { return _InterlockedCompareExchange128(d, h, l, c); }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Cmpxchg128:[0-9]+]] @Cmpxchg128(%[[VALUE_d:[0-9]+]] d: ptr<volatile i64>, %[[VALUE_h:[0-9]+]] h: i64, %[[VALUE_l:[0-9]+]] l: i64, %[[VALUE_c:[0-9]+]] c: ptr<i64>) -> u8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic] = compare_exchange<u128, volatile, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(pointer_cast<ptr<volatile u128>>(read<ptr<volatile i64>>(%[[VALUE_d]]))), pointer_cast<ptr<u128>>(read<ptr<i64>>(%[[VALUE_c]])), or<u128>(shl<u128>(widen<u128>(reinterpret<u64>(read<i64>(%[[VALUE_h]]))), const<u128>(64)), widen<u128>(reinterpret<u64>(read<i64>(%[[VALUE_l]])))));
// DEFAULT-NEXT:         return from_bool<u8>(read<bool>(%[[VALUE0]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
