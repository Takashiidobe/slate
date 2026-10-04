typedef __WCHAR_TYPE__ wchar_t;

#ifdef _CHAR_UNSIGNED
#define CHAR_UNSIGNED 1
#else
#define CHAR_UNSIGNED 0
#endif

int char_unsigned = CHAR_UNSIGNED;
int wchar_size = sizeof(wchar_t);
int wchar_width = __WCHAR_WIDTH__;
wchar_t wide[] = L"a";

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES LONG
// SLATE-FILECHECK-PREFIX-ARGS LONG -fno-short-wchar -funsigned-char

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
// DEFAULT-NEXT:     type @type[[TYPE_wchar_t:[0-9]+]] wchar_t = u16;
// DEFAULT-NEXT:     global %[[VALUE_char_unsigned:[0-9]+]] char_unsigned: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wchar_size:[0-9]+]] wchar_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wchar_width:[0-9]+]] wchar_width: i32 [storage=static] = const<i32>(16) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: array<u16, 2> [storage=static] = code_units<array<u16, 2>>([97, 0]) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN LONG
// LONG: module {
// LONG-NEXT:     target "x86_64-pc-windows-msvc" {
// LONG-NEXT:         endian = little;
// LONG-NEXT:         pointer [size=8, align=8];
// LONG-NEXT:         stack_alignment = 16;
// LONG-NEXT:         long_double = f64;
// LONG-NEXT:         storage bool [size=1, align=1];
// LONG-NEXT:         storage i8, u8 [size=1, align=1];
// LONG-NEXT:         storage i16, u16 [size=2, align=2];
// LONG-NEXT:         storage i32, u32 [size=4, align=4];
// LONG-NEXT:         storage i64, u64 [size=8, align=8];
// LONG-NEXT:         storage i128, u128 [size=16, align=16];
// LONG-NEXT:         storage bf16 [size=2, align=2];
// LONG-NEXT:         storage f16 [size=2, align=2];
// LONG-NEXT:         storage f32 [size=4, align=4];
// LONG-NEXT:         storage f64 [size=8, align=8];
// LONG-NEXT:         storage f128 [size=16, align=16];
// LONG-NEXT:         storage d32 [size=4, align=4];
// LONG-NEXT:         storage d64 [size=8, align=8];
// LONG-NEXT:         storage d128 [size=16, align=16];
// LONG-NEXT:     }
// LONG-NEXT:     type @type[[TYPE_wchar_t:[0-9]+]] wchar_t = i32;
// LONG-NEXT:     global %[[VALUE_char_unsigned:[0-9]+]] char_unsigned: i32 [storage=static] = const<i32>(1) [linkage=external];
// LONG-NEXT:     global %[[VALUE_wchar_size:[0-9]+]] wchar_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// LONG-NEXT:     global %[[VALUE_wchar_width:[0-9]+]] wchar_width: i32 [storage=static] = const<i32>(32) [linkage=external];
// LONG-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: array<i32, 2> [storage=static] = code_units<array<i32, 2>>([97, 0]) [linkage=external];
// LONG-NEXT: }
// SLATE-FILECHECK-END LONG
