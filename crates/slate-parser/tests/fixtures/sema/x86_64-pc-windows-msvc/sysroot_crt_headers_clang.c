// SLATE-FILECHECK-FLAVOR clang
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
// IR-NEXT:     type @type0 size_t = u64;
// IR-NEXT:     type @type1 _iobuf = struct {
// IR-NEXT:         field0 _Placeholder: ptr<void>;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type2 FILE = @type1;
// IR-NEXT:     type @type3 size_t = u64;
// IR-NEXT:     global %5 windows_version: i32 [storage=static] = const<i32>(2560) [linkage=external];
// IR-NEXT:     fn %3 @abs(%8 _Number: i32) -> i32 [linkage=external] [memory=none];
// IR-NEXT:     fn %4 @strlen(%9 _Str: ptr<const i8>) -> u64 [linkage=external];
// IR-NEXT:     fn %6 @magnitude_digits(%7 digits: ptr<const i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<u64, overflow=wrap>(call<u64, signature=fn(ptr<const i8>) -> u64>(%4, read<ptr<const i8>>(%7)), reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(call<i32, signature=fn(i32) -> i32>(%3, neg<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(8))))))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
