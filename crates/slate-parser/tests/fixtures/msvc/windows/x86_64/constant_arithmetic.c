// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir -std=c17

enum MsvcConstants {
    DIV_ZERO = 1 / 0,
    SHIFT_HIGH = 1 << 32,
    SHIFT_NEGATIVE = 1 << -1,
    NEGATIVE_RIGHT = -1 >> -1,
};

_Static_assert(DIV_ZERO == 0, "");
_Static_assert(SHIFT_HIGH == 0, "");
_Static_assert(SHIFT_NEGATIVE == 0, "");
_Static_assert(NEGATIVE_RIGHT == -1, "");
_Static_assert((1 / 0) == 0, "");

int global_div_zero = 1 / 0;
void local_div_zero(void) { static int local = 1 / 0; }

#if 1 << -1
int pp_shift_taken;
#else
#error MSVC takes this branch
#endif

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
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
// IR-NEXT:     type @type0 MsvcConstants = enum : i32 {
// IR-NEXT:         %0 DIV_ZERO = const<i32>(0);
// IR-NEXT:         %1 SHIFT_HIGH = const<i32>(0);
// IR-NEXT:         %2 SHIFT_NEGATIVE = const<i32>(0);
// IR-NEXT:         %3 NEGATIVE_RIGHT = const<i32>(-1);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     global %5 global_div_zero: i32 [storage=static] = const<i32>(0) [linkage=external];
// IR-NEXT:     global %7 local: i32 [storage=static] = const<i32>(0) [linkage=internal];
// IR-NEXT:     global %8 pp_shift_taken: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %6 @local_div_zero() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
