// SLATE-FILECHECK-DEFINES DEFAULT

void
dr106_1(void *pv, int i)
{
	*pv;
	i ? *pv : *pv;
	*pv, *pv;
}

void
dr106_2(const void *pcv, volatile void *pvv, int i)
{
	*pcv;
	i ? *pcv : *pcv;
	*pcv, *pcv;

	*pvv;
	i ? *pvv : *pvv;
	*pvv, *pvv;
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
// DEFAULT-NEXT:     fn %[[VALUE_dr106_1:[0-9]+]] @dr106_1(%[[VALUE_pv:[0-9]+]] pv: ptr<void>, %[[VALUE_i:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         read<void>(deref(read<ptr<void>>(%[[VALUE_pv]])));
// DEFAULT-NEXT:         conditional<void>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), read<void>(deref(read<ptr<void>>(%[[VALUE_pv]]))), read<void>(deref(read<ptr<void>>(%[[VALUE_pv]]))));
// DEFAULT-NEXT:         read<void>(deref(read<ptr<void>>(%[[VALUE_pv]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dr106_2:[0-9]+]] @dr106_2(%[[VALUE_pcv:[0-9]+]] pcv: ptr<const void>, %[[VALUE_pvv:[0-9]+]] pvv: ptr<volatile void>, %[[VALUE_i_2:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         read<void>(deref(read<ptr<const void>>(%[[VALUE_pcv]])));
// DEFAULT-NEXT:         conditional<void>(ne<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(0)), read<void>(deref(read<ptr<const void>>(%[[VALUE_pcv]]))), read<void>(deref(read<ptr<const void>>(%[[VALUE_pcv]]))));
// DEFAULT-NEXT:         read<void>(deref(read<ptr<const void>>(%[[VALUE_pcv]])));
// DEFAULT-NEXT:         read<void, volatile>(deref(read<ptr<volatile void>>(%[[VALUE_pvv]])));
// DEFAULT-NEXT:         conditional<void>(ne<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(0)), read<void, volatile>(deref(read<ptr<volatile void>>(%[[VALUE_pvv]]))), read<void, volatile>(deref(read<ptr<volatile void>>(%[[VALUE_pvv]]))));
// DEFAULT-NEXT:         read<void, volatile>(deref(read<ptr<volatile void>>(%[[VALUE_pvv]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
