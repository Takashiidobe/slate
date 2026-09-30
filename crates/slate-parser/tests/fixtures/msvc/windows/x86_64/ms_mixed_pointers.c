extern int *__ptr32 extension_redeclared;
extern int *__ptr32 __uptr extension_redeclared;
int *__ptr32 __uptr zero_extended;
int *__ptr32 sign_extended;

_Static_assert(_Generic(zero_extended, int *__ptr32: 1, default: 0), "cl: __uptr is not part of the type");
_Static_assert(_Generic(sign_extended, int *: 0, int *__ptr32: 1), "ptr32 is distinct");

void swap_extension(void) { sign_extended = zero_extended; zero_extended = sign_extended; }

// SLATE-FILECHECK-STD DEFAULT c17
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
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
// DEFAULT-NEXT:     extern %[[VALUE_extension_redeclared:[0-9]+]] extension_redeclared: ptr<i32, ptr32_sptr> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_zero_extended:[0-9]+]] zero_extended: ptr<i32, ptr32_uptr> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sign_extended:[0-9]+]] sign_extended: ptr<i32, ptr32_sptr> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_swap_extension:[0-9]+]] @swap_extension() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32, ptr32_sptr>>(%[[VALUE_sign_extended]], address_space_cast<ptr<i32, ptr32_sptr>, reason=assign>(read<ptr<i32, ptr32_uptr>>(%[[VALUE_zero_extended]])));
// DEFAULT-NEXT:         write<ptr<i32, ptr32_uptr>>(%[[VALUE_zero_extended]], address_space_cast<ptr<i32, ptr32_uptr>, reason=assign>(read<ptr<i32, ptr32_sptr>>(%[[VALUE_sign_extended]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
