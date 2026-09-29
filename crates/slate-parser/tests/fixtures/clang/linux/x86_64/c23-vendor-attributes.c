[[gnu::cdecl]] void caller(void);
[[gnu::stdcall]] void callee(void);
[[gnu::fastcall]] void fast(void);
[[clang::vectorcall]] void vector(void);
[[gnu::thiscall]] void method(void *);
[[gnu::ms_abi]] void ms(void);
[[gnu::sysv_abi]] void sysv(void);
[[gnu::regparm(1 + 2)]] void registers(int, int);
[[gnu::pcs("aapcs")]] void arm(void);
[[gnu::pcs("aapcs-vfp")]] void arm_vfp(void);
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "x86_64-unknown-linux-gnu" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=8, align=8];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f80;
// C23-NEXT:         storage bool [size=1, align=1];
// C23-NEXT:         storage i8, u8 [size=1, align=1];
// C23-NEXT:         storage i16, u16 [size=2, align=2];
// C23-NEXT:         storage i32, u32 [size=4, align=4];
// C23-NEXT:         storage i64, u64 [size=8, align=8];
// C23-NEXT:         storage i128, u128 [size=16, align=16];
// C23-NEXT:         storage bf16 [size=2, align=2];
// C23-NEXT:         storage f16 [size=2, align=2];
// C23-NEXT:         storage f32 [size=4, align=4];
// C23-NEXT:         storage f64 [size=8, align=8];
// C23-NEXT:         storage f80 [size=16, align=16];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:         storage d32 [size=4, align=4];
// C23-NEXT:         storage d64 [size=8, align=8];
// C23-NEXT:         storage d128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     fn %[[VALUE_caller:[0-9]+]] @caller() -> void [linkage=external];
// C23-NEXT:     fn %[[VALUE_callee:[0-9]+]] @callee() -> void [linkage=external];
// C23-NEXT:     fn %[[VALUE_fast:[0-9]+]] @fast() -> void [linkage=external];
// C23-NEXT:     fn %[[VALUE_vector:[0-9]+]] @vector() -> void [linkage=external] [abi=sysv64 vectorcall() -> void];
// C23-NEXT:     fn %[[VALUE_method:[0-9]+]] @method(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// C23-NEXT:     fn %[[VALUE_ms:[0-9]+]] @ms() -> void [linkage=external];
// C23-NEXT:     fn %[[VALUE_sysv:[0-9]+]] @sysv() -> void [linkage=external];
// C23-NEXT:     fn %[[VALUE_registers:[0-9]+]] @registers(%[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// C23-NEXT:     fn %[[VALUE_arm:[0-9]+]] @arm() -> void [linkage=external];
// C23-NEXT:     fn %[[VALUE_arm_vfp:[0-9]+]] @arm_vfp() -> void [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
