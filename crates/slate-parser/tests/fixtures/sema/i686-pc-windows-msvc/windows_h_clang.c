// SLATE-FILECHECK-FLAVOR clang
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
// IR-NEXT:     global %14 handle_size: u32 [storage=static] = const<u32>(4) [linkage=external];
// IR-NEXT:     global %15 long_ptr_size: u32 [storage=static] = const<u32>(4) [linkage=external];
// IR-NEXT:     global %16 large_integer_size: u32 [storage=static] = const<u32>(8) [linkage=external];
// IR-NEXT:     global %17 large_integer_align: u32 [storage=static] = const<u32>(8) [linkage=external];
// IR-NEXT:     fn %11 @Int64ShllMod32(%12 Value: u64, %13 ShiftCount: u32) -> u64 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret(or<u64>(widen<u64, reason=return>(read<u32>(%23)), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%24)), const<u32>(32))))] {
// IR-NEXT:         let %23: u32 [synthetic];
// IR-NEXT:         let %24: u32 [synthetic];
// IR-NEXT:         asm volatile "mov ecx, ShiftCount\nmov eax, dword ptr [Value]\nmov edx, dword ptr [Value + 4]\nshld edx, eax, cl\nshl eax, cl" [dialect=intel] [alternative=none] {
// IR-NEXT:             template: "mov ecx, " addr(%2) "\nmov eax, " addr<dword>(%3) "\nmov edx, " addr<dword>(%3 + 4) "\nshld edx, eax, cl\nshl eax, cl";
// IR-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%23);
// IR-NEXT:             out 1 "{edx}" [{dx}] width 32 place<u32>(%24);
// IR-NEXT:             in 2 [ShiftCount] mem<read> place<u32>(%13);
// IR-NEXT:             in 3 [Value] mem<read> place<u64>(%12);
// IR-NEXT:             clobbers: "ecx" as cx;
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %18 @shift_left(%19 value: u64, %20 count: u32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<u64, signature=fn(u64, u32) -> u64>(%11, read<u64>(%19), read<u32>(%20));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
