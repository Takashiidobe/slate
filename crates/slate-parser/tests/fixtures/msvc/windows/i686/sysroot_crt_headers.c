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
// IR-NEXT:     type @type0 va_list = ptr<i8>;
// IR-NEXT:     type @type1 size_t = u32;
// IR-NEXT:     type @type2 __crt_locale_pointers = struct {
// IR-NEXT:         field0 locinfo: ptr<@type3>;
// IR-NEXT:         field1 mbcinfo: ptr<@type4>;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type3 __crt_locale_data = struct incomplete;
// IR-NEXT:     type @type4 __crt_multibyte_data = struct incomplete;
// IR-NEXT:     type @type5 __crt_locale_pointers = @type2;
// IR-NEXT:     type @type6 _locale_t = ptr<@type2>;
// IR-NEXT:     type @type7 _iobuf = struct {
// IR-NEXT:         field0 _Placeholder: ptr<void>;
// IR-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-NEXT:     type @type8 FILE = @type7;
// IR-NEXT:     global %8 _OptionsStorage: u64 [storage=static] [linkage=internal];
// IR-NEXT:     global %26 windows_version: i32 [storage=static] = const<i32>(2560) [linkage=external];
// IR-NEXT:     global %41 .str41: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 100, 0]) [linkage=internal];
// IR-NEXT:     fn %7 @__local_stdio_printf_options() -> ptr<u64> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// IR-NEXT:         return addr_of<ptr<u64>>(%8);
// IR-NEXT:     }
// IR-NEXT:     fn %11 @__stdio_common_vsprintf(%33 _Options: u64, %34 _Buffer: ptr<i8>, %35 _BufferCount: u32, %36 _Format: ptr<const i8>, %37 _Locale: ptr<@type2>, %38 _ArgList: ptr<i8>) -> i32 [linkage=external];
// IR-NEXT:     fn %12 @vsnprintf(%13 _Buffer: ptr<i8> [const], %14 _BufferCount: u32 [const], %15 _Format: ptr<const i8> [const], %16 _ArgList: ptr<i8>) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// IR-NEXT:         let %17 _Result: i32 [storage=automatic] [const] = call<i32, signature=fn(u64, ptr<i8>, u32, ptr<const i8>, ptr<@type2>, ptr<i8>) -> i32>(%11, or<u64>(read<u64>(deref(call<ptr<u64>, signature=fn() -> ptr<u64>>(%7))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), const<i32>(1))), read<ptr<i8>>(%13), read<u32>(%14), read<ptr<const i8>>(%15), null<ptr<@type2>>, read<ptr<i8>>(%16));
// IR-NEXT:         return conditional<i32>(lt<i32>(read<i32>(%17), const<i32>(0)), neg<i32, overflow=ub>(const<i32>(1)), read<i32>(%17));
// IR-NEXT:     }
// IR-NEXT:     fn %18 @snprintf(%19 _Buffer: ptr<i8> [const], %20 _BufferCount: u32 [const], %21 _Format: ptr<const i8> [const], ...) -> i32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// IR-NEXT:         let %22 _Result: i32 [storage=automatic];
// IR-NEXT:         let %23 _ArgList: ptr<i8> [storage=automatic];
// IR-NEXT:         write<ptr<i8>>(%23, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<const ptr<const i8>>>(%21)), and<u32>(sub<u32, overflow=wrap>(add<u32, overflow=wrap>(const<u32>(4), const<u32>(4)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), not<u32>(sub<u32, overflow=wrap>(const<u32>(4), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))));
// IR-NEXT:         write<i32>(%22, call<i32, signature=fn(ptr<i8>, u32, ptr<const i8>, ptr<i8>) -> i32>(%12, read<ptr<i8>>(%19), read<u32>(%20), read<ptr<const i8>>(%21), read<ptr<i8>>(%23)));
// IR-NEXT:         call<i32, signature=fn(ptr<i8>, u32, ptr<const i8>, ptr<i8>) -> i32>(%12, read<ptr<i8>>(%19), read<u32>(%20), read<ptr<const i8>>(%21), read<ptr<i8>>(%23));
// IR-NEXT:         write<ptr<i8>>(%23, null<ptr<i8>>);
// IR-NEXT:         return read<i32>(%22);
// IR-NEXT:     }
// IR-NEXT:     fn %24 @abs(%39 _Number: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %25 @strlen(%40 _Str: ptr<const i8>) -> u32 [linkage=external];
// IR-NEXT:     fn %27 @magnitude_digits(%28 digits: ptr<const i8>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<u32, overflow=wrap>(call<u32, signature=fn(ptr<const i8>) -> u32>(%25, read<ptr<const i8>>(%28)), reinterpret<u32, reason=explicit, fits=unknown>(call<i32, signature=fn(i32) -> i32>(%24, neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=always>(const<u32>(4))))));
// IR-NEXT:     }
// IR-NEXT:     fn %29 @format_number(%30 buffer: ptr<i8>, %31 size: u32, %32 value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(ptr<i8>, u32, ptr<const i8>, ...) -> i32>(%18, read<ptr<i8>>(%30), read<u32>(%31), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%41)), read<i32>(%32));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
