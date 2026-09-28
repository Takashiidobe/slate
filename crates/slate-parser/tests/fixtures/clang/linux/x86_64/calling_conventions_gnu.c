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
// DEFAULT-NEXT:     type @type0 callback = ptr<fn() -> void>;
// DEFAULT-NEXT:     global %11 pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 register_pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 vector_pointer: ptr<fn vectorcall() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 cdecl_pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 stdcall_pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %18 thiscall_pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %19 arm_pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %20 arm_vfp_pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @ms() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @sysv() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @registers(%21 <unnamed>: i32, %22 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @fast() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @vector() -> void [linkage=external] [abi=sysv64 vectorcall() -> void];
// DEFAULT-NEXT:     fn %5 @caller() -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @callee() -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @method(%23 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %8 @arm() -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @arm_vfp() -> void [linkage=external];
// DEFAULT-NEXT:     fn %12 @trailing() -> void [linkage=external];
// DEFAULT-NEXT:     fn %13 @accepts(%24 callback: ptr<fn(i32) -> void>) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
