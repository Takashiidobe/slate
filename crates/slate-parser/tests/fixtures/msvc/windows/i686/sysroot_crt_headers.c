// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
#include <sdkddkver.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int windows_version = _WIN32_WINNT_WIN10;

size_t magnitude_digits(const char *digits) {
    return strlen(digits) + (size_t)abs(-(int)sizeof(FILE *));
}

int format_number(char *buffer, size_t size, int value) {
    return snprintf(buffer, size, "%d", value);
}

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
// IR-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = ptr<i8>;
// IR-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u32;
// IR-NEXT:     type @type[[TYPE___crt_locale_pointers:[0-9]+]] __crt_locale_pointers = struct {
// IR-NEXT:         field0 locinfo: ptr<@type[[TYPE___crt_locale_data:[0-9]+]]>;
// IR-NEXT:         field1 mbcinfo: ptr<@type[[TYPE___crt_multibyte_data:[0-9]+]]>;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE___crt_locale_data]] __crt_locale_data = struct incomplete;
// IR-NEXT:     type @type[[TYPE___crt_multibyte_data]] __crt_multibyte_data = struct incomplete;
// IR-NEXT:     type @type[[TYPE___crt_locale_pointers_2:[0-9]+]] __crt_locale_pointers = @type[[TYPE___crt_locale_pointers]];
// IR-NEXT:     type @type[[TYPE__locale_t:[0-9]+]] _locale_t = ptr<@type[[TYPE___crt_locale_pointers]]>;
// IR-NEXT:     type @type[[TYPE__iobuf:[0-9]+]] _iobuf = struct {
// IR-NEXT:         field0 _Placeholder: ptr<void>;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_FILE:[0-9]+]] FILE = @type[[TYPE__iobuf]];
// IR-NEXT:     global %[[VALUE__OptionsStorage:[0-9]+]] _OptionsStorage: u64 [storage=static] [linkage=internal];
// IR-NEXT:     global %[[VALUE_windows_version:[0-9]+]] windows_version: i32 [storage=static] = const<i32>(2560) [linkage=external];
// IR-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// IR-NEXT:     fn %[[VALUE___local_stdio_printf_options:[0-9]+]] @__local_stdio_printf_options() -> ptr<u64> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// IR-NEXT:         return addr_of<ptr<u64>>(%[[VALUE__OptionsStorage]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE___stdio_common_vsprintf:[0-9]+]] @__stdio_common_vsprintf(%[[VALUE__Options:[0-9]+]] _Options: u64, %[[VALUE__Buffer:[0-9]+]] _Buffer: ptr<i8>, %[[VALUE__BufferCount:[0-9]+]] _BufferCount: u32, %[[VALUE__Format:[0-9]+]] _Format: ptr<const i8>, %[[VALUE__Locale:[0-9]+]] _Locale: ptr<@type[[TYPE___crt_locale_pointers]]>, %[[VALUE__ArgList:[0-9]+]] _ArgList: ptr<i8>) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_vsnprintf:[0-9]+]] @vsnprintf(%[[VALUE__Buffer_2:[0-9]+]] _Buffer: ptr<i8> [const], %[[VALUE__BufferCount_2:[0-9]+]] _BufferCount: u32 [const], %[[VALUE__Format_2:[0-9]+]] _Format: ptr<const i8> [const], %[[VALUE__ArgList_2:[0-9]+]] _ArgList: ptr<i8>) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE__Result:[0-9]+]] _Result: i32 [storage=automatic] [const] = call<i32, signature=fn(u64, ptr<i8>, u32, ptr<const i8>, ptr<@type[[TYPE___crt_locale_pointers]]>, ptr<i8>) -> i32>(%[[VALUE___stdio_common_vsprintf]], or<u64>(read<u64>(deref(call<ptr<u64>, signature=fn() -> ptr<u64>>(%[[VALUE___local_stdio_printf_options]]))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(1))), read<ptr<i8>>(%[[VALUE__Buffer_2]]), read<u32>(%[[VALUE__BufferCount_2]]), read<ptr<const i8>>(%[[VALUE__Format_2]]), null<ptr<@type[[TYPE___crt_locale_pointers]]>>, read<ptr<i8>>(%[[VALUE__ArgList_2]]));
// IR-NEXT:         return conditional<i32>(lt<i32>(read<i32>(%[[VALUE__Result]]), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(1)), read<i32>(%[[VALUE__Result]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_snprintf:[0-9]+]] @snprintf(%[[VALUE__Buffer_3:[0-9]+]] _Buffer: ptr<i8> [const], %[[VALUE__BufferCount_3:[0-9]+]] _BufferCount: u32 [const], %[[VALUE__Format_3:[0-9]+]] _Format: ptr<const i8> [const], ...) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE__Result_2:[0-9]+]] _Result: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE__ArgList_3:[0-9]+]] _ArgList: ptr<i8> [storage=automatic];
// IR-NEXT:         write<ptr<i8>>(%[[VALUE__ArgList_3]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<const ptr<const i8>>>(%[[VALUE__Format_3]])), and<u32>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(const<u32>(4), const<u32>(4)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), not<u32>(sub<u32, overflow=wrap>(const<u32>(4), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))));
// IR-NEXT:         write<i32>(%[[VALUE__Result_2]], call<i32, signature=fn(ptr<i8>, u32, ptr<const i8>, ptr<i8>) -> i32>(%[[VALUE_vsnprintf]], read<ptr<i8>>(%[[VALUE__Buffer_3]]), read<u32>(%[[VALUE__BufferCount_3]]), read<ptr<const i8>>(%[[VALUE__Format_3]]), read<ptr<i8>>(%[[VALUE__ArgList_3]])));
// IR-NEXT:         write<ptr<i8>>(%[[VALUE__ArgList_3]], null<ptr<i8>>);
// IR-NEXT:         return read<i32>(%[[VALUE__Result_2]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_abs:[0-9]+]] @abs(%[[VALUE__Number:[0-9]+]] _Number: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE__Str:[0-9]+]] _Str: ptr<const i8>) -> u32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_magnitude_digits:[0-9]+]] @magnitude_digits(%[[VALUE_digits:[0-9]+]] digits: ptr<const i8>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<u32, overflow=wrap>(call<u32, signature=fn(ptr<const i8>) -> u32>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE_digits]])), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_abs]], neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=always>(const<u32>(4))))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_format_number:[0-9]+]] @format_number(%[[VALUE_buffer:[0-9]+]] buffer: ptr<i8>, %[[VALUE_size:[0-9]+]] size: u32, %[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(ptr<i8>, u32, ptr<const i8>, ...) -> i32>(%[[VALUE_snprintf]], read<ptr<i8>>(%[[VALUE_buffer]]), read<u32>(%[[VALUE_size]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])), read<i32>(%[[VALUE_value]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
