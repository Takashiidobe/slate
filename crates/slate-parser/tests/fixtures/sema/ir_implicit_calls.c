// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata

int old();
int exact(void);
int variadic(int first, ...);

int calls(short small, float real) {
    old(small, real);
    variadic(1, small, real);
    return exact();
}

// SLATE-FILECHECK-BEGIN C17
// C17: module {
// C17-NEXT:     target "x86_64-unknown-linux-gnu" {
// C17-NEXT:         endian = little;
// C17-NEXT:         pointer [size=8, align=8];
// C17-NEXT:         stack_alignment = 16;
// C17-NEXT:         long_double = f80;
// C17-NEXT:         storage bool [size=1, align=1];
// C17-NEXT:         storage i8, u8 [size=1, align=1];
// C17-NEXT:         storage i16, u16 [size=2, align=2];
// C17-NEXT:         storage i32, u32 [size=4, align=4];
// C17-NEXT:         storage i64, u64 [size=8, align=8];
// C17-NEXT:         storage i128, u128 [size=16, align=16];
// C17-NEXT:         storage f16 [size=2, align=2];
// C17-NEXT:         storage f32 [size=4, align=4];
// C17-NEXT:         storage f64 [size=8, align=8];
// C17-NEXT:         storage f80 [size=16, align=16];
// C17-NEXT:         storage f128 [size=16, align=16];
// C17-NEXT:     }
// C17-NEXT:     fn %0 @old(unprototyped) -> i32 [linkage=external] [c="int()"];
// C17-NEXT:     fn %1 @exact() -> i32 [linkage=external] [c="int(void)"];
// C17-NEXT:     fn %2 @variadic(%6 first: i32 [c="int"], ...) -> i32 [linkage=external] [c="int(int, ...)"];
// C17-NEXT:     fn %3 @calls(%4 small: i16 [c="short"], %5 real: f32 [c="float"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(short, float)"] {
// C17-NEXT:         call<i32>(%0, widen<i32, reason=vararg>(read<i16>(%4)), float_widen<f64, reason=vararg>(read<f32>(%5)));
// C17-NEXT:         call<i32>(%2, const<i32>(1), widen<i32, reason=vararg>(read<i16>(%4)), float_widen<f64, reason=vararg>(read<f32>(%5)));
// C17-NEXT:         return call<i32>(%1);
// C17-NEXT:     }
// C17-NEXT: }
// SLATE-FILECHECK-END C17
