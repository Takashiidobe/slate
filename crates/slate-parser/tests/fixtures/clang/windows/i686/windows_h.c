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
// IR-NEXT:     type @type[[TYPE_DWORD:[0-9]+]] DWORD = u32;
// IR-NEXT:     type @type[[TYPE_LONG_PTR:[0-9]+]] LONG_PTR = i32;
// IR-NEXT:     type @type[[TYPE_PLONG_PTR:[0-9]+]] PLONG_PTR = ptr<i32>;
// IR-NEXT:     type @type[[TYPE_LONG:[0-9]+]] LONG = i32;
// IR-NEXT:     type @type[[TYPE_HANDLE:[0-9]+]] HANDLE = ptr<void>;
// IR-NEXT:     type @type[[TYPE_LONGLONG:[0-9]+]] LONGLONG = i64;
// IR-NEXT:     type @type[[TYPE_ULONGLONG:[0-9]+]] ULONGLONG = u64;
// IR-NEXT:     type @type[[TYPE__LARGE_INTEGER:[0-9]+]] _LARGE_INTEGER = union {
// IR-NEXT:         field0 <anonymous>: @type[[TYPE0:[0-9]+]];
// IR-NEXT:         field1 u: @type[[TYPE1:[0-9]+]];
// IR-NEXT:         field2 QuadPart: i64;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 0, 0]];
// IR-NEXT:     type @type[[TYPE0]] = struct {
// IR-NEXT:         field0 LowPart: u32;
// IR-NEXT:         field1 HighPart: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE1]] = struct {
// IR-NEXT:         field0 LowPart: u32;
// IR-NEXT:         field1 HighPart: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_LARGE_INTEGER:[0-9]+]] LARGE_INTEGER = @type[[TYPE__LARGE_INTEGER]];
// IR-NEXT:     global %[[VALUE_handle_size:[0-9]+]] handle_size: u32 [storage=static] = const<u32>(4) [linkage=external];
// IR-NEXT:     global %[[VALUE_long_ptr_size:[0-9]+]] long_ptr_size: u32 [storage=static] = const<u32>(4) [linkage=external];
// IR-NEXT:     global %[[VALUE_large_integer_size:[0-9]+]] large_integer_size: u32 [storage=static] = const<u32>(8) [linkage=external];
// IR-NEXT:     global %[[VALUE_large_integer_align:[0-9]+]] large_integer_align: u32 [storage=static] = const<u32>(8) [linkage=external];
// IR-NEXT:     fn %[[VALUE_Int64ShllMod32:[0-9]+]] @Int64ShllMod32(%[[VALUE_Value:[0-9]+]] Value: u64, %[[VALUE_ShiftCount:[0-9]+]] ShiftCount: u32) -> u64 [linkage=external] [inline=hint] [definition=emitted] [abi=x86_win32 stdcall(scalar, scalar) -> scalar] [fallthrough=ret(or<u64>(widen<u64, reason=return>(read<u32>(%[[VALUE0:[0-9]+]])), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%[[VALUE1:[0-9]+]])), const<u32>(32))))] {
// IR-NEXT:         let %[[VALUE0]]: u32 [synthetic];
// IR-NEXT:         let %[[VALUE1]]: u32 [synthetic];
// IR-NEXT:         asm volatile "mov ecx, ShiftCount\nmov eax, dword ptr [Value]\nmov edx, dword ptr [Value + 4]\nshld edx, eax, cl\nshl eax, cl" [dialect=intel] [alternative=none] {
// IR-NEXT:             template: "mov ecx, " addr(%2) "\nmov eax, " addr<dword>(%3) "\nmov edx, " addr<dword>(%3 + 4) "\nshld edx, eax, cl\nshl eax, cl";
// IR-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE0]]);
// IR-NEXT:             out 1 "{edx}" [{dx}] width 32 place<u32>(%[[VALUE1]]);
// IR-NEXT:             in 2 [ShiftCount] mem<read> place<u32>(%[[VALUE_ShiftCount]]);
// IR-NEXT:             in 3 [Value] mem<read> place<u64>(%[[VALUE_Value]]);
// IR-NEXT:             clobbers: "ecx" as cx;
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_shift_left:[0-9]+]] @shift_left(%[[VALUE_value:[0-9]+]] value: u64, %[[VALUE_count:[0-9]+]] count: u32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<u64, signature=fn stdcall(u64, u32) -> u64, abi=x86_win32 stdcall(scalar, scalar) -> scalar>(%[[VALUE_Int64ShllMod32]], read<u64>(%[[VALUE_value]]), read<u32>(%[[VALUE_count]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
