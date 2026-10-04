typedef __WCHAR_TYPE__ wchar_t;

#ifdef __CHAR_UNSIGNED__
#define CHAR_UNSIGNED 1
#else
#define CHAR_UNSIGNED 0
#endif

int char_unsigned = CHAR_UNSIGNED;
int char_negative = (char)-1 < 0;
int wchar_size = sizeof(wchar_t);
int wide_char_size = sizeof(L'a');
int wide_string_size = sizeof(L"ab");
int wchar_negative = (wchar_t)-1 < 0;
int wchar_width = __WCHAR_WIDTH__;
wchar_t wide[] = L"\xffff";

int widen(char c) { return c; }

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES UNSIGNED
// SLATE-FILECHECK-PREFIX-ARGS UNSIGNED -funsigned-char
// SLATE-FILECHECK-DEFINES NOSIGNED
// SLATE-FILECHECK-PREFIX-ARGS NOSIGNED -fno-signed-char
// SLATE-FILECHECK-DEFINES RESIGNED
// SLATE-FILECHECK-PREFIX-ARGS RESIGNED -funsigned-char -fsigned-char
// SLATE-FILECHECK-DEFINES SHORT
// SLATE-FILECHECK-PREFIX-ARGS SHORT -fshort-wchar
// SLATE-FILECHECK-DEFINES UNSHORT
// SLATE-FILECHECK-PREFIX-ARGS UNSHORT -fshort-wchar -fno-short-wchar

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
// DEFAULT-NEXT:     type @type[[TYPE_wchar_t:[0-9]+]] wchar_t = i32;
// DEFAULT-NEXT:     global %[[VALUE_char_unsigned:[0-9]+]] char_unsigned: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_char_negative:[0-9]+]] char_negative: i32 [storage=static] = from_bool<i32>(lt<i32>(widen<i32>(truncate<i8>(neg<i32>(const<i32>(1)))), const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wchar_size:[0-9]+]] wchar_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wide_char_size:[0-9]+]] wide_char_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wide_string_size:[0-9]+]] wide_string_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(12))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wchar_negative:[0-9]+]] wchar_negative: i32 [storage=static] = from_bool<i32>(lt<i32>(neg<i32>(const<i32>(1)), const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wchar_width:[0-9]+]] wchar_width: i32 [storage=static] = const<i32>(32) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: array<i32, 2> [storage=static] = code_units<array<i32, 2>>([65535, 0]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_widen:[0-9]+]] @widen(%[[VALUE_c:[0-9]+]] c: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i32>(read<i8>(%[[VALUE_c]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN UNSIGNED
// UNSIGNED: module {
// UNSIGNED-NEXT:     target "x86_64-unknown-linux-gnu" {
// UNSIGNED-NEXT:         endian = little;
// UNSIGNED-NEXT:         pointer [size=8, align=8];
// UNSIGNED-NEXT:         stack_alignment = 16;
// UNSIGNED-NEXT:         long_double = f80;
// UNSIGNED-NEXT:         storage bool [size=1, align=1];
// UNSIGNED-NEXT:         storage i8, u8 [size=1, align=1];
// UNSIGNED-NEXT:         storage i16, u16 [size=2, align=2];
// UNSIGNED-NEXT:         storage i32, u32 [size=4, align=4];
// UNSIGNED-NEXT:         storage i64, u64 [size=8, align=8];
// UNSIGNED-NEXT:         storage i128, u128 [size=16, align=16];
// UNSIGNED-NEXT:         storage bf16 [size=2, align=2];
// UNSIGNED-NEXT:         storage f16 [size=2, align=2];
// UNSIGNED-NEXT:         storage f32 [size=4, align=4];
// UNSIGNED-NEXT:         storage f64 [size=8, align=8];
// UNSIGNED-NEXT:         storage f80 [size=16, align=16];
// UNSIGNED-NEXT:         storage f128 [size=16, align=16];
// UNSIGNED-NEXT:         storage d32 [size=4, align=4];
// UNSIGNED-NEXT:         storage d64 [size=8, align=8];
// UNSIGNED-NEXT:         storage d128 [size=16, align=16];
// UNSIGNED-NEXT:     }
// UNSIGNED-NEXT:     type @type[[TYPE_wchar_t:[0-9]+]] wchar_t = i32;
// UNSIGNED-NEXT:     global %[[VALUE_char_unsigned:[0-9]+]] char_unsigned: i32 [storage=static] = const<i32>(1) [linkage=external];
// UNSIGNED-NEXT:     global %[[VALUE_char_negative:[0-9]+]] char_negative: i32 [storage=static] = from_bool<i32>(lt<i32>(reinterpret<i32>(widen<u32>(reinterpret<u8>(truncate<i8>(neg<i32>(const<i32>(1)))))), const<i32>(0))) [linkage=external];
// UNSIGNED-NEXT:     global %[[VALUE_wchar_size:[0-9]+]] wchar_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// UNSIGNED-NEXT:     global %[[VALUE_wide_char_size:[0-9]+]] wide_char_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// UNSIGNED-NEXT:     global %[[VALUE_wide_string_size:[0-9]+]] wide_string_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(12))) [linkage=external];
// UNSIGNED-NEXT:     global %[[VALUE_wchar_negative:[0-9]+]] wchar_negative: i32 [storage=static] = from_bool<i32>(lt<i32>(neg<i32>(const<i32>(1)), const<i32>(0))) [linkage=external];
// UNSIGNED-NEXT:     global %[[VALUE_wchar_width:[0-9]+]] wchar_width: i32 [storage=static] = const<i32>(32) [linkage=external];
// UNSIGNED-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: array<i32, 2> [storage=static] = code_units<array<i32, 2>>([65535, 0]) [linkage=external];
// UNSIGNED-NEXT:     fn %[[VALUE_widen:[0-9]+]] @widen(%[[VALUE_c:[0-9]+]] c: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// UNSIGNED-NEXT:         return reinterpret<i32>(widen<u32>(read<u8>(%[[VALUE_c]])));
// UNSIGNED-NEXT:     }
// UNSIGNED-NEXT: }
// SLATE-FILECHECK-END UNSIGNED
// SLATE-FILECHECK-BEGIN NOSIGNED
// NOSIGNED: module {
// NOSIGNED-NEXT:     target "x86_64-unknown-linux-gnu" {
// NOSIGNED-NEXT:         endian = little;
// NOSIGNED-NEXT:         pointer [size=8, align=8];
// NOSIGNED-NEXT:         stack_alignment = 16;
// NOSIGNED-NEXT:         long_double = f80;
// NOSIGNED-NEXT:         storage bool [size=1, align=1];
// NOSIGNED-NEXT:         storage i8, u8 [size=1, align=1];
// NOSIGNED-NEXT:         storage i16, u16 [size=2, align=2];
// NOSIGNED-NEXT:         storage i32, u32 [size=4, align=4];
// NOSIGNED-NEXT:         storage i64, u64 [size=8, align=8];
// NOSIGNED-NEXT:         storage i128, u128 [size=16, align=16];
// NOSIGNED-NEXT:         storage bf16 [size=2, align=2];
// NOSIGNED-NEXT:         storage f16 [size=2, align=2];
// NOSIGNED-NEXT:         storage f32 [size=4, align=4];
// NOSIGNED-NEXT:         storage f64 [size=8, align=8];
// NOSIGNED-NEXT:         storage f80 [size=16, align=16];
// NOSIGNED-NEXT:         storage f128 [size=16, align=16];
// NOSIGNED-NEXT:         storage d32 [size=4, align=4];
// NOSIGNED-NEXT:         storage d64 [size=8, align=8];
// NOSIGNED-NEXT:         storage d128 [size=16, align=16];
// NOSIGNED-NEXT:     }
// NOSIGNED-NEXT:     type @type[[TYPE_wchar_t:[0-9]+]] wchar_t = i32;
// NOSIGNED-NEXT:     global %[[VALUE_char_unsigned:[0-9]+]] char_unsigned: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOSIGNED-NEXT:     global %[[VALUE_char_negative:[0-9]+]] char_negative: i32 [storage=static] = from_bool<i32>(lt<i32>(reinterpret<i32>(widen<u32>(reinterpret<u8>(truncate<i8>(neg<i32>(const<i32>(1)))))), const<i32>(0))) [linkage=external];
// NOSIGNED-NEXT:     global %[[VALUE_wchar_size:[0-9]+]] wchar_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// NOSIGNED-NEXT:     global %[[VALUE_wide_char_size:[0-9]+]] wide_char_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// NOSIGNED-NEXT:     global %[[VALUE_wide_string_size:[0-9]+]] wide_string_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(12))) [linkage=external];
// NOSIGNED-NEXT:     global %[[VALUE_wchar_negative:[0-9]+]] wchar_negative: i32 [storage=static] = from_bool<i32>(lt<i32>(neg<i32>(const<i32>(1)), const<i32>(0))) [linkage=external];
// NOSIGNED-NEXT:     global %[[VALUE_wchar_width:[0-9]+]] wchar_width: i32 [storage=static] = const<i32>(32) [linkage=external];
// NOSIGNED-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: array<i32, 2> [storage=static] = code_units<array<i32, 2>>([65535, 0]) [linkage=external];
// NOSIGNED-NEXT:     fn %[[VALUE_widen:[0-9]+]] @widen(%[[VALUE_c:[0-9]+]] c: u8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// NOSIGNED-NEXT:         return reinterpret<i32>(widen<u32>(read<u8>(%[[VALUE_c]])));
// NOSIGNED-NEXT:     }
// NOSIGNED-NEXT: }
// SLATE-FILECHECK-END NOSIGNED
// SLATE-FILECHECK-BEGIN RESIGNED
// RESIGNED: module {
// RESIGNED-NEXT:     target "x86_64-unknown-linux-gnu" {
// RESIGNED-NEXT:         endian = little;
// RESIGNED-NEXT:         pointer [size=8, align=8];
// RESIGNED-NEXT:         stack_alignment = 16;
// RESIGNED-NEXT:         long_double = f80;
// RESIGNED-NEXT:         storage bool [size=1, align=1];
// RESIGNED-NEXT:         storage i8, u8 [size=1, align=1];
// RESIGNED-NEXT:         storage i16, u16 [size=2, align=2];
// RESIGNED-NEXT:         storage i32, u32 [size=4, align=4];
// RESIGNED-NEXT:         storage i64, u64 [size=8, align=8];
// RESIGNED-NEXT:         storage i128, u128 [size=16, align=16];
// RESIGNED-NEXT:         storage bf16 [size=2, align=2];
// RESIGNED-NEXT:         storage f16 [size=2, align=2];
// RESIGNED-NEXT:         storage f32 [size=4, align=4];
// RESIGNED-NEXT:         storage f64 [size=8, align=8];
// RESIGNED-NEXT:         storage f80 [size=16, align=16];
// RESIGNED-NEXT:         storage f128 [size=16, align=16];
// RESIGNED-NEXT:         storage d32 [size=4, align=4];
// RESIGNED-NEXT:         storage d64 [size=8, align=8];
// RESIGNED-NEXT:         storage d128 [size=16, align=16];
// RESIGNED-NEXT:     }
// RESIGNED-NEXT:     type @type[[TYPE_wchar_t:[0-9]+]] wchar_t = i32;
// RESIGNED-NEXT:     global %[[VALUE_char_unsigned:[0-9]+]] char_unsigned: i32 [storage=static] = const<i32>(0) [linkage=external];
// RESIGNED-NEXT:     global %[[VALUE_char_negative:[0-9]+]] char_negative: i32 [storage=static] = from_bool<i32>(lt<i32>(widen<i32>(truncate<i8>(neg<i32>(const<i32>(1)))), const<i32>(0))) [linkage=external];
// RESIGNED-NEXT:     global %[[VALUE_wchar_size:[0-9]+]] wchar_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// RESIGNED-NEXT:     global %[[VALUE_wide_char_size:[0-9]+]] wide_char_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// RESIGNED-NEXT:     global %[[VALUE_wide_string_size:[0-9]+]] wide_string_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(12))) [linkage=external];
// RESIGNED-NEXT:     global %[[VALUE_wchar_negative:[0-9]+]] wchar_negative: i32 [storage=static] = from_bool<i32>(lt<i32>(neg<i32>(const<i32>(1)), const<i32>(0))) [linkage=external];
// RESIGNED-NEXT:     global %[[VALUE_wchar_width:[0-9]+]] wchar_width: i32 [storage=static] = const<i32>(32) [linkage=external];
// RESIGNED-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: array<i32, 2> [storage=static] = code_units<array<i32, 2>>([65535, 0]) [linkage=external];
// RESIGNED-NEXT:     fn %[[VALUE_widen:[0-9]+]] @widen(%[[VALUE_c:[0-9]+]] c: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// RESIGNED-NEXT:         return widen<i32>(read<i8>(%[[VALUE_c]]));
// RESIGNED-NEXT:     }
// RESIGNED-NEXT: }
// SLATE-FILECHECK-END RESIGNED
// SLATE-FILECHECK-BEGIN SHORT
// SHORT: module {
// SHORT-NEXT:     target "x86_64-unknown-linux-gnu" {
// SHORT-NEXT:         endian = little;
// SHORT-NEXT:         pointer [size=8, align=8];
// SHORT-NEXT:         stack_alignment = 16;
// SHORT-NEXT:         long_double = f80;
// SHORT-NEXT:         storage bool [size=1, align=1];
// SHORT-NEXT:         storage i8, u8 [size=1, align=1];
// SHORT-NEXT:         storage i16, u16 [size=2, align=2];
// SHORT-NEXT:         storage i32, u32 [size=4, align=4];
// SHORT-NEXT:         storage i64, u64 [size=8, align=8];
// SHORT-NEXT:         storage i128, u128 [size=16, align=16];
// SHORT-NEXT:         storage bf16 [size=2, align=2];
// SHORT-NEXT:         storage f16 [size=2, align=2];
// SHORT-NEXT:         storage f32 [size=4, align=4];
// SHORT-NEXT:         storage f64 [size=8, align=8];
// SHORT-NEXT:         storage f80 [size=16, align=16];
// SHORT-NEXT:         storage f128 [size=16, align=16];
// SHORT-NEXT:         storage d32 [size=4, align=4];
// SHORT-NEXT:         storage d64 [size=8, align=8];
// SHORT-NEXT:         storage d128 [size=16, align=16];
// SHORT-NEXT:     }
// SHORT-NEXT:     type @type[[TYPE_wchar_t:[0-9]+]] wchar_t = u16;
// SHORT-NEXT:     global %[[VALUE_char_unsigned:[0-9]+]] char_unsigned: i32 [storage=static] = const<i32>(0) [linkage=external];
// SHORT-NEXT:     global %[[VALUE_char_negative:[0-9]+]] char_negative: i32 [storage=static] = from_bool<i32>(lt<i32>(widen<i32>(truncate<i8>(neg<i32>(const<i32>(1)))), const<i32>(0))) [linkage=external];
// SHORT-NEXT:     global %[[VALUE_wchar_size:[0-9]+]] wchar_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(2))) [linkage=external];
// SHORT-NEXT:     global %[[VALUE_wide_char_size:[0-9]+]] wide_char_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(2))) [linkage=external];
// SHORT-NEXT:     global %[[VALUE_wide_string_size:[0-9]+]] wide_string_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(6))) [linkage=external];
// SHORT-NEXT:     global %[[VALUE_wchar_negative:[0-9]+]] wchar_negative: i32 [storage=static] = from_bool<i32>(lt<i32>(reinterpret<i32>(widen<u32>(reinterpret<u16>(truncate<i16>(neg<i32>(const<i32>(1)))))), const<i32>(0))) [linkage=external];
// SHORT-NEXT:     global %[[VALUE_wchar_width:[0-9]+]] wchar_width: i32 [storage=static] = const<i32>(16) [linkage=external];
// SHORT-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: array<u16, 2> [storage=static] = code_units<array<u16, 2>>([65535, 0]) [linkage=external];
// SHORT-NEXT:     fn %[[VALUE_widen:[0-9]+]] @widen(%[[VALUE_c:[0-9]+]] c: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// SHORT-NEXT:         return widen<i32>(read<i8>(%[[VALUE_c]]));
// SHORT-NEXT:     }
// SHORT-NEXT: }
// SLATE-FILECHECK-END SHORT
// SLATE-FILECHECK-BEGIN UNSHORT
// UNSHORT: module {
// UNSHORT-NEXT:     target "x86_64-unknown-linux-gnu" {
// UNSHORT-NEXT:         endian = little;
// UNSHORT-NEXT:         pointer [size=8, align=8];
// UNSHORT-NEXT:         stack_alignment = 16;
// UNSHORT-NEXT:         long_double = f80;
// UNSHORT-NEXT:         storage bool [size=1, align=1];
// UNSHORT-NEXT:         storage i8, u8 [size=1, align=1];
// UNSHORT-NEXT:         storage i16, u16 [size=2, align=2];
// UNSHORT-NEXT:         storage i32, u32 [size=4, align=4];
// UNSHORT-NEXT:         storage i64, u64 [size=8, align=8];
// UNSHORT-NEXT:         storage i128, u128 [size=16, align=16];
// UNSHORT-NEXT:         storage bf16 [size=2, align=2];
// UNSHORT-NEXT:         storage f16 [size=2, align=2];
// UNSHORT-NEXT:         storage f32 [size=4, align=4];
// UNSHORT-NEXT:         storage f64 [size=8, align=8];
// UNSHORT-NEXT:         storage f80 [size=16, align=16];
// UNSHORT-NEXT:         storage f128 [size=16, align=16];
// UNSHORT-NEXT:         storage d32 [size=4, align=4];
// UNSHORT-NEXT:         storage d64 [size=8, align=8];
// UNSHORT-NEXT:         storage d128 [size=16, align=16];
// UNSHORT-NEXT:     }
// UNSHORT-NEXT:     type @type[[TYPE_wchar_t:[0-9]+]] wchar_t = i32;
// UNSHORT-NEXT:     global %[[VALUE_char_unsigned:[0-9]+]] char_unsigned: i32 [storage=static] = const<i32>(0) [linkage=external];
// UNSHORT-NEXT:     global %[[VALUE_char_negative:[0-9]+]] char_negative: i32 [storage=static] = from_bool<i32>(lt<i32>(widen<i32>(truncate<i8>(neg<i32>(const<i32>(1)))), const<i32>(0))) [linkage=external];
// UNSHORT-NEXT:     global %[[VALUE_wchar_size:[0-9]+]] wchar_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// UNSHORT-NEXT:     global %[[VALUE_wide_char_size:[0-9]+]] wide_char_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// UNSHORT-NEXT:     global %[[VALUE_wide_string_size:[0-9]+]] wide_string_size: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(12))) [linkage=external];
// UNSHORT-NEXT:     global %[[VALUE_wchar_negative:[0-9]+]] wchar_negative: i32 [storage=static] = from_bool<i32>(lt<i32>(neg<i32>(const<i32>(1)), const<i32>(0))) [linkage=external];
// UNSHORT-NEXT:     global %[[VALUE_wchar_width:[0-9]+]] wchar_width: i32 [storage=static] = const<i32>(32) [linkage=external];
// UNSHORT-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: array<i32, 2> [storage=static] = code_units<array<i32, 2>>([65535, 0]) [linkage=external];
// UNSHORT-NEXT:     fn %[[VALUE_widen:[0-9]+]] @widen(%[[VALUE_c:[0-9]+]] c: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// UNSHORT-NEXT:         return widen<i32>(read<i8>(%[[VALUE_c]]));
// UNSHORT-NEXT:     }
// UNSHORT-NEXT: }
// SLATE-FILECHECK-END UNSHORT
