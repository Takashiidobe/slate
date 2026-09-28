void first(void) { g(4); }
void second(void) { double d = g(1, 2.0f); }
int g(int x) { return x; }

// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "i686-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 4;
// IR-NEXT:         long_double = f64;
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
// IR-NEXT:     fn %1 @g(%4 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%4);
// IR-NEXT:     }
// IR-NEXT:     fn %0 @first() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         call<i32>(%1, const<i32>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %2 @second() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %3 d: f64 [storage=automatic] = int_to_float<f64>(call<i32>(%1, const<i32>(1), float_widen<f64>(const<f32>(2.0))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
