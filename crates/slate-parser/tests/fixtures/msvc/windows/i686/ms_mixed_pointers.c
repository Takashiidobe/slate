int *__ptr64 wide;
int *__ptr64 const wide_const = 0;
int *__ptr32 plain_narrow;
int *__uptr zero_extended;

_Static_assert(sizeof(int *__ptr64) == 8 && _Alignof(int *__ptr64) == 8, "ptr64 storage");
_Static_assert(_Generic(wide, int *: 0, int *__ptr64: 1), "ptr64 is distinct");
_Static_assert(_Generic(plain_narrow, int *: 1, default: 0), "ptr32 is plain");
_Static_assert(sizeof(zero_extended) == 4 && _Generic(zero_extended, int *: 0, default: 1), "uptr is distinct");

int *narrow(void) { return wide; }
int *__ptr64 widen(int *p) { return p; }
int *widen_unsigned(void) { return zero_extended; }

// SLATE-FILECHECK-STD DEFAULT c17
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
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
// DEFAULT-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: ptr<i32, ptr64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wide_const:[0-9]+]] wide_const: ptr<i32, ptr64> [storage=static] [const] = null<ptr<i32, ptr64>> [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_plain_narrow:[0-9]+]] plain_narrow: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_zero_extended:[0-9]+]] zero_extended: ptr<i32, ptr32_uptr> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_narrow:[0-9]+]] @narrow() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return address_space_cast<ptr<i32>, reason=return>(read<ptr<i32, ptr64>>(%[[VALUE_wide]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_widen:[0-9]+]] @widen(%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> ptr<i32, ptr64> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return address_space_cast<ptr<i32, ptr64>, reason=return>(read<ptr<i32>>(%[[VALUE_p]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_widen_unsigned:[0-9]+]] @widen_unsigned() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return address_space_cast<ptr<i32>, reason=return>(read<ptr<i32, ptr32_uptr>>(%[[VALUE_zero_extended]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
