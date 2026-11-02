// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

void streaming(void) __arm_streaming;
void compatible(void) __arm_streaming_compatible;
void shared_za(void) __arm_inout("za");
void mixed(void) __arm_in("za") __arm_out("zt0");
void kept(void) __arm_preserves("za");
void both(void) __arm_inout("za", "zt0") {}
void (*pointer)(void) __arm_streaming;
typedef void typed(int) __arm_inout("za");
__arm_locally_streaming void local(void) {}
__arm_new("za") void fresh(void) {}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "aarch64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f128;
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
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_typed:[0-9]+]] typed = fn(i32) -> void;
// IR-NEXT:     global %[[VALUE_pointer:[0-9]+]] pointer: ptr<fn() -> void> [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_streaming:[0-9]+]] @streaming() -> void [linkage=external] [sme=streaming];
// IR-NEXT:     fn %[[VALUE_compatible:[0-9]+]] @compatible() -> void [linkage=external] [sme=streaming_compatible];
// IR-NEXT:     fn %[[VALUE_shared_za:[0-9]+]] @shared_za() -> void [linkage=external] [sme=inout(za)];
// IR-NEXT:     fn %[[VALUE_mixed:[0-9]+]] @mixed() -> void [linkage=external] [sme=in(za)] [sme=out(zt0)];
// IR-NEXT:     fn %[[VALUE_kept:[0-9]+]] @kept() -> void [linkage=external] [sme=preserves(za)];
// IR-NEXT:     fn %[[VALUE_both:[0-9]+]] @both() -> void [linkage=external] [sme=inout(za,zt0)] [fallthrough=ret_void] {
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_local:[0-9]+]] @local() -> void [linkage=external] [sme=locally_streaming] [fallthrough=ret_void] {
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_fresh:[0-9]+]] @fresh() -> void [linkage=external] [sme=new(za)] [fallthrough=ret_void] {
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
