void __attribute__((ms_abi)) ms(void);
void __attribute__((sysv_abi)) sysv(void);
void __attribute__((regparm(1 + 2))) registers(int, int);
void __attribute__((fastcall)) fast(void);
void __attribute__((vectorcall)) vector(void);
void __attribute__((cdecl)) caller(void);
void __attribute__((stdcall)) callee(void);
void __attribute__((thiscall)) method(void *);
void __attribute__((pcs("aapcs"))) arm(void);
void __attribute__((pcs("aapcs-vfp"))) arm_vfp(void);
typedef void (__attribute__((__ms_abi__)) *callback)(void);
void (* __attribute__((sysv_abi)) pointer)(void);
void trailing(void) __attribute__((__stdcall__));
void accepts(void (__attribute__((fastcall)) *callback)(int));
void (__attribute__((regparm(3))) *register_pointer)(void);
void (__attribute__((vectorcall)) *vector_pointer)(void);
void (__attribute__((cdecl)) *cdecl_pointer)(void);
void (__attribute__((stdcall)) *stdcall_pointer)(void);
void (__attribute__((thiscall)) *thiscall_pointer)(void);
void (__attribute__((pcs("aapcs"))) *arm_pointer)(void);
void (__attribute__((pcs("aapcs-vfp"))) *arm_vfp_pointer)(void);
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
// DEFAULT-NEXT:     type @type[[TYPE_callback:[0-9]+]] callback = ptr<fn() -> void>;
// DEFAULT-NEXT:     global %[[VALUE_pointer:[0-9]+]] pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_register_pointer:[0-9]+]] register_pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_vector_pointer:[0-9]+]] vector_pointer: ptr<fn vectorcall() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cdecl_pointer:[0-9]+]] cdecl_pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_stdcall_pointer:[0-9]+]] stdcall_pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_thiscall_pointer:[0-9]+]] thiscall_pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arm_pointer:[0-9]+]] arm_pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arm_vfp_pointer:[0-9]+]] arm_vfp_pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ms:[0-9]+]] @ms() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sysv:[0-9]+]] @sysv() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_registers:[0-9]+]] @registers(%[[VALUE0:[0-9]+]] <unnamed>: i32, %[[VALUE1:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fast:[0-9]+]] @fast() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_vector:[0-9]+]] @vector() -> void [linkage=external] [abi=sysv64 vectorcall() -> void];
// DEFAULT-NEXT:     fn %[[VALUE_caller:[0-9]+]] @caller() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_callee:[0-9]+]] @callee() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_method:[0-9]+]] @method(%[[VALUE2:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_arm:[0-9]+]] @arm() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_arm_vfp:[0-9]+]] @arm_vfp() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_trailing:[0-9]+]] @trailing() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_accepts:[0-9]+]] @accepts(%[[VALUE_callback:[0-9]+]] callback: ptr<fn(i32) -> void>) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
