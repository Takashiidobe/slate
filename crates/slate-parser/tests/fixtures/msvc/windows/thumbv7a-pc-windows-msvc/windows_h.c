// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

#include <windows.h>

unsigned handle_size = sizeof(HANDLE);
unsigned long_ptr_size = sizeof(LONG_PTR);
unsigned large_integer_size = sizeof(LARGE_INTEGER);
unsigned large_integer_align = __alignof(LARGE_INTEGER);

ULONGLONG shift_left(ULONGLONG value, DWORD count) {
    return Int64ShllMod32(value, count);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "thumbv7a-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 8;
// IR-NEXT:         long_double = f64;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 DWORD = u32;
// IR-NEXT:     type @type1 LONG_PTR = i32;
// IR-NEXT:     type @type2 PLONG_PTR = ptr<i32>;
// IR-NEXT:     type @type3 LONG = i32;
// IR-NEXT:     type @type4 HANDLE = ptr<void>;
// IR-NEXT:     type @type5 LONGLONG = i64;
// IR-NEXT:     type @type6 ULONGLONG = u64;
// IR-NEXT:     type @type7 _LARGE_INTEGER = union {
// IR-NEXT:         field0 <anonymous>: @type8;
// IR-NEXT:         field1 u: @type9;
// IR-NEXT:         field2 QuadPart: i64;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 0, 0]];
// IR-NEXT:     type @type8 = struct {
// IR-NEXT:         field0 LowPart: u32;
// IR-NEXT:         field1 HighPart: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type9 = struct {
// IR-NEXT:         field0 LowPart: u32;
// IR-NEXT:         field1 HighPart: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type10 LARGE_INTEGER = @type7;
// IR-NEXT:     global %11 handle_size: u32 [storage=static] = const<u32>(4) [linkage=external];
// IR-NEXT:     global %12 long_ptr_size: u32 [storage=static] = const<u32>(4) [linkage=external];
// IR-NEXT:     global %13 large_integer_size: u32 [storage=static] = const<u32>(8) [linkage=external];
// IR-NEXT:     global %14 large_integer_align: u32 [storage=static] = const<u32>(8) [linkage=external];
// IR-NEXT:     fn %15 @shift_left(%16 value: u64, %17 count: u32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%16), read<u32>(%17));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
