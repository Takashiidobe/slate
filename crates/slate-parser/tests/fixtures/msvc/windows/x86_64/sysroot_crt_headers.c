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
// IR-NEXT:     type @type0 va_list = ptr<i8>;
// IR-NEXT:     type @type1 size_t = u64;
// IR-NEXT:     type @type2 __crt_locale_pointers = struct {
// IR-NEXT:         field0 locinfo: ptr<@type3>;
// IR-NEXT:         field1 mbcinfo: ptr<@type4>;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type3 __crt_locale_data = struct incomplete;
// IR-NEXT:     type @type4 __crt_multibyte_data = struct incomplete;
// IR-NEXT:     type @type5 __crt_locale_pointers = @type2;
// IR-NEXT:     type @type6 _locale_t = ptr<@type2>;
// IR-NEXT:     type @type7 _iobuf = struct {
// IR-NEXT:         field0 _Placeholder: ptr<void>;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type8 FILE = @type7;
// IR-NEXT:     global %9 _OptionsStorage: u64 [storage=static] [linkage=internal];
// IR-NEXT:     global %35 windows_version: i32 [storage=static] = const<i32>(2560) [linkage=external];
// IR-NEXT:     global %51 .str51: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// IR-NEXT:     fn %1 @__va_start(%42 <unnamed>: ptr<ptr<i8>>, ...) -> void [linkage=external];
// IR-NEXT:     fn %8 @__local_stdio_printf_options() -> ptr<u64> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// IR-NEXT:         return addr_of<ptr<u64>>(%9);
// IR-NEXT:     }
// IR-NEXT:     fn %18 @__stdio_common_vsprintf(%43 _Options: u64, %44 _Buffer: ptr<i8>, %45 _BufferCount: u64, %46 _Format: ptr<const i8>, %47 _Locale: ptr<@type2>, %48 _ArgList: ptr<i8>) -> i32 [linkage=external];
// IR-NEXT:     fn %19 @vsnprintf(%20 _Buffer: ptr<i8> [const], %21 _BufferCount: u64 [const], %22 _Format: ptr<const i8> [const], %23 _ArgList: ptr<i8>) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// IR-NEXT:         let %24 _Result: i32 [storage=automatic] [const] = call<i32, signature=fn(u64, ptr<i8>, u64, ptr<const i8>, ptr<@type2>, ptr<i8>) -> i32>(%18, or<u64>(read<u64>(deref(call<ptr<u64>, signature=fn() -> ptr<u64>>(%8))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(1))), read<ptr<i8>>(%20), read<u64>(%21), read<ptr<const i8>>(%22), null<ptr<@type2>>, read<ptr<i8>>(%23));
// IR-NEXT:         return conditional<i32>(lt<i32>(read<i32>(%24), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(1)), read<i32>(%24));
// IR-NEXT:     }
// IR-NEXT:     fn %25 @snprintf(%26 _Buffer: ptr<i8> [const], %27 _BufferCount: u64 [const], %28 _Format: ptr<const i8> [const], ...) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// IR-NEXT:         let %29 _Result: i32 [storage=automatic];
// IR-NEXT:         let %30 _ArgList: ptr<i8> [storage=automatic];
// IR-NEXT:         call<void, signature=fn(ptr<ptr<i8>>, ...) -> void>(%1, addr_of<ptr<ptr<i8>>>(%30), read<ptr<const i8>>(%28));
// IR-NEXT:         write<i32>(%29, call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, ptr<i8>) -> i32>(%19, read<ptr<i8>>(%26), read<u64>(%27), read<ptr<const i8>>(%28), read<ptr<i8>>(%30)));
// IR-NEXT:         call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, ptr<i8>) -> i32>(%19, read<ptr<i8>>(%26), read<u64>(%27), read<ptr<const i8>>(%28), read<ptr<i8>>(%30));
// IR-NEXT:         write<ptr<i8>>(%30, null<ptr<i8>>);
// IR-NEXT:         return read<i32>(%29);
// IR-NEXT:     }
// IR-NEXT:     fn %32 @abs(%49 _Number: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %34 @strlen(%50 _Str: ptr<const i8>) -> u64 [linkage=external];
// IR-NEXT:     fn %36 @magnitude_digits(%37 digits: ptr<const i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<u64, overflow=wrap>(call<u64, signature=fn(ptr<const i8>) -> u64>(%34, read<ptr<const i8>>(%37)), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(call<i32, signature=fn(i32) -> i32>(%32, neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(8))))))));
// IR-NEXT:     }
// IR-NEXT:     fn %38 @format_number(%39 buffer: ptr<i8>, %40 size: u64, %41 value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(ptr<i8>, u64, ptr<const i8>, ...) -> i32>(%25, read<ptr<i8>>(%39), read<u64>(%40), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%51)), read<i32>(%41));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
