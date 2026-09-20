// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
asm("top_basic");
__asm__("con" "cat");

#pragma pack(push, 1)
int packed;
#pragma pack(pop)

int value = 1;

int in_function(void) {
    _Pragma("clang diagnostic push")
    return value;
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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     asm "top_basic";
// IR-NEXT:     asm "concat";
// IR-NEXT:     global %0 packed: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %1 value: i32 [storage=static] = const<i32>(1) [linkage=external];
// IR-NEXT:     fn %2 @in_function() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%1);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
