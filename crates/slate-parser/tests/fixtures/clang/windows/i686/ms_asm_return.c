// SLATE-FILECHECK-DEFINES DEFAULT

#include "ms_asm_return.h"

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
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
// DEFAULT-NEXT:     type @type[[TYPE_ULONGLONG:[0-9]+]] ULONGLONG = u64;
// DEFAULT-NEXT:     type @type[[TYPE_LONGLONG:[0-9]+]] LONGLONG = i64;
// DEFAULT-NEXT:     type @type[[TYPE_DWORD:[0-9]+]] DWORD = u32;
// DEFAULT-NEXT:     type @type[[TYPE_Result:[0-9]+]] Result = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_First:[0-9]+]] First = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_Second:[0-9]+]] Second = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_Pair:[0-9]+]] Pair = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %[[VALUE_Int64ShllMod32:[0-9]+]] @Int64ShllMod32(%[[VALUE_Value:[0-9]+]] Value: u64, %[[VALUE_ShiftCount:[0-9]+]] ShiftCount: u32) -> u64 [linkage=external] [fallthrough=ret(or<u64>(widen<u64, reason=return>(read<u32>(%[[VALUE0:[0-9]+]])), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%[[VALUE1:[0-9]+]])), const<u32>(32))))] {
// DEFAULT-NEXT:         let %[[VALUE0]]: u32 [synthetic];
// DEFAULT-NEXT:         let %[[VALUE1]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov ecx, ShiftCount\nmov eax, dword ptr [Value]\nmov edx, dword ptr [Value + 4]\nshld edx, eax, cl\nshl eax, cl" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov ecx, " addr(%2) "\nmov eax, " addr<dword>(%3) "\nmov edx, " addr<dword>(%3 + 4) "\nshld edx, eax, cl\nshl eax, cl";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE0]]);
// DEFAULT-NEXT:             out 1 "{edx}" [{dx}] width 32 place<u32>(%[[VALUE1]]);
// DEFAULT-NEXT:             in 2 [ShiftCount] mem<read> place<u32>(%[[VALUE_ShiftCount]]);
// DEFAULT-NEXT:             in 3 [Value] mem<read> place<u64>(%[[VALUE_Value]]);
// DEFAULT-NEXT:             clobbers: "ecx" as cx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Int64ShraMod32:[0-9]+]] @Int64ShraMod32(%[[VALUE_Value_2:[0-9]+]] Value: i64, %[[VALUE_ShiftCount_2:[0-9]+]] ShiftCount: u32) -> i64 [linkage=external] [fallthrough=ret(reinterpret<i64, reason=return, fits=unknown>(or<u64>(widen<u64, reason=return>(read<u32>(%[[VALUE2:[0-9]+]])), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%[[VALUE3:[0-9]+]])), const<u32>(32)))))] {
// DEFAULT-NEXT:         let %[[VALUE2]]: u32 [synthetic];
// DEFAULT-NEXT:         let %[[VALUE3]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov ecx, ShiftCount\nmov eax, dword ptr [Value]\nmov edx, dword ptr [Value + 4]\nshrd eax, edx, cl\nsar edx, cl" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov ecx, " addr(%2) "\nmov eax, " addr<dword>(%3) "\nmov edx, " addr<dword>(%3 + 4) "\nshrd eax, edx, cl\nsar edx, cl";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE2]]);
// DEFAULT-NEXT:             out 1 "{edx}" [{dx}] width 32 place<u32>(%[[VALUE3]]);
// DEFAULT-NEXT:             in 2 [ShiftCount] mem<read> place<u32>(%[[VALUE_ShiftCount_2]]);
// DEFAULT-NEXT:             in 3 [Value] mem<read> place<i64>(%[[VALUE_Value_2]]);
// DEFAULT-NEXT:             clobbers: "ecx" as cx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_Int64ShrlMod32:[0-9]+]] @Int64ShrlMod32(%[[VALUE_Value_3:[0-9]+]] Value: u64, %[[VALUE_ShiftCount_3:[0-9]+]] ShiftCount: u32) -> u64 [linkage=external] [fallthrough=ret(or<u64>(widen<u64, reason=return>(read<u32>(%[[VALUE4:[0-9]+]])), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%[[VALUE5:[0-9]+]])), const<u32>(32))))] {
// DEFAULT-NEXT:         let %[[VALUE4]]: u32 [synthetic];
// DEFAULT-NEXT:         let %[[VALUE5]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov ecx, ShiftCount\nmov eax, dword ptr [Value]\nmov edx, dword ptr [Value + 4]\nshrd eax, edx, cl\nshr edx, cl" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov ecx, " addr(%2) "\nmov eax, " addr<dword>(%3) "\nmov edx, " addr<dword>(%3 + 4) "\nshrd eax, edx, cl\nshr edx, cl";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE4]]);
// DEFAULT-NEXT:             out 1 "{edx}" [{dx}] width 32 place<u32>(%[[VALUE5]]);
// DEFAULT-NEXT:             in 2 [ShiftCount] mem<read> place<u32>(%[[VALUE_ShiftCount_3]]);
// DEFAULT-NEXT:             in 3 [Value] mem<read> place<u64>(%[[VALUE_Value_3]]);
// DEFAULT-NEXT:             clobbers: "ecx" as cx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_word:[0-9]+]] @word(%[[VALUE_input:[0-9]+]] input: i32) -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE6:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE6]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, input" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, " addr(%1);
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE6]]);
// DEFAULT-NEXT:             in 1 [input] mem<read> place<i32>(%[[VALUE_input]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_byte:[0-9]+]] @byte() -> u8 [linkage=external] [fallthrough=ret(truncate<u8, reason=return, fits=unknown>(read<u32>(%[[VALUE7:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE7]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov al, 255" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov al, 255";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE7]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_half:[0-9]+]] @half() -> i16 [linkage=external] [fallthrough=ret(reinterpret<i16, reason=return, fits=unknown>(truncate<u16, reason=return, fits=unknown>(read<u32>(%[[VALUE8:[0-9]+]]))))] {
// DEFAULT-NEXT:         let %[[VALUE8]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov ax, 32768" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov ax, 32768";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE8]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_boolean:[0-9]+]] @boolean() -> bool [linkage=external] [fallthrough=ret(ne<u1, reason=return>(truncate<u1, reason=return, fits=unknown>(read<u32>(%[[VALUE9:[0-9]+]])), const<u1>(0)))] {
// DEFAULT-NEXT:         let %[[VALUE9]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, 2" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 2";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE9]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pointer:[0-9]+]] @pointer() -> ptr<void> [linkage=external] [fallthrough=ret(int_to_ptr<ptr<void>, reason=return>(read<u32>(%[[VALUE10:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE10]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, 4096" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 4096";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE10]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_enumeration:[0-9]+]] @enumeration() -> @type[[TYPE_Result]] [linkage=external] [fallthrough=ret(int_to_enum<@type[[TYPE_Result]], reason=return>(read<u32>(%[[VALUE11:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE11]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, 1" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 1";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE11]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bits:[0-9]+]] @bits() -> u33b [linkage=external] [fallthrough=ret(truncate<u33b, reason=return, fits=unknown>(or<u64>(widen<u64, reason=return>(read<u32>(%[[VALUE12:[0-9]+]])), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%[[VALUE13:[0-9]+]])), const<u32>(32)))))] {
// DEFAULT-NEXT:         let %[[VALUE12]]: u32 [synthetic];
// DEFAULT-NEXT:         let %[[VALUE13]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, 1" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 1";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE12]]);
// DEFAULT-NEXT:             lateout 1 "{edx}" [{dx}] width 32 place<u32>(%[[VALUE13]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_branches:[0-9]+]] @branches(%[[VALUE_condition:[0-9]+]] condition: i32) -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE14:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE14]]: u32 [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_condition]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 asm volatile "mov eax, 1" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:                     template: "mov eax, 1";
// DEFAULT-NEXT:                     out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE14]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 asm volatile "mov eax, 2" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:                     template: "mov eax, 2";
// DEFAULT-NEXT:                     out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE14]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_multiple:[0-9]+]] @multiple() -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE15:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE15]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, 3" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 3";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE15]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "nop" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             lateout 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE15]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_high_only:[0-9]+]] @high_only() -> i64 [linkage=external] [fallthrough=ret(reinterpret<i64, reason=return, fits=unknown>(or<u64>(widen<u64, reason=return>(read<u32>(%[[VALUE16:[0-9]+]])), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%[[VALUE17:[0-9]+]])), const<u32>(32)))))] {
// DEFAULT-NEXT:         let %[[VALUE16]]: u32 [synthetic];
// DEFAULT-NEXT:         let %[[VALUE17]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov edx, 4" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov edx, 4";
// DEFAULT-NEXT:             lateout 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE16]]);
// DEFAULT-NEXT:             out 1 "{edx}" [{dx}] width 32 place<u32>(%[[VALUE17]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_low_only:[0-9]+]] @low_only() -> i64 [linkage=external] [fallthrough=ret(reinterpret<i64, reason=return, fits=unknown>(or<u64>(widen<u64, reason=return>(read<u32>(%[[VALUE18:[0-9]+]])), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%[[VALUE19:[0-9]+]])), const<u32>(32)))))] {
// DEFAULT-NEXT:         let %[[VALUE18]]: u32 [synthetic];
// DEFAULT-NEXT:         let %[[VALUE19]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, 5" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 5";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE18]]);
// DEFAULT-NEXT:             lateout 1 "{edx}" [{dx}] width 32 place<u32>(%[[VALUE19]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_explicit_return:[0-9]+]] @explicit_return() -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE20:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE20]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, 6" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 6";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE20]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_partial_return:[0-9]+]] @partial_return(%[[VALUE_condition_2:[0-9]+]] condition: i32) -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE21:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE21]]: u32 [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_condition_2]]), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(8);
// DEFAULT-NEXT:         asm volatile "mov eax, 9" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 9";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE21]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nothing:[0-9]+]] @nothing() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "nop" [dialect=intel] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_floating:[0-9]+]] @floating() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm volatile "fldz" [dialect=intel] {
// DEFAULT-NEXT:             template: "fldz";
// DEFAULT-NEXT:             clobbers: "st" as st, "st(1)" as st(1), "st(2)" as st(2), "st(3)" as st(3), "st(4)" as st(4), "st(5)" as st(5), "st(6)" as st(6), "st(7)" as st(7);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_aggregate:[0-9]+]] @aggregate() -> @type[[TYPE_Pair]] [linkage=external] [abi=x86_win32() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm volatile "mov eax, 1" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, 1";
// DEFAULT-NEXT:             clobbers: "eax" as ax;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_wide:[0-9]+]] @wide() -> u65b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm volatile "mov eax, 1" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, 1";
// DEFAULT-NEXT:             clobbers: "eax" as ax;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_naked:[0-9]+]] @naked() -> i32 [linkage=external] [naked] [fallthrough=ub] {
// DEFAULT-NEXT:         asm volatile "mov eax, 1" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, 1";
// DEFAULT-NEXT:             clobbers: "eax" as ax;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_never:[0-9]+]] @never() -> i32 [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "int 3" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "int 3";
// DEFAULT-NEXT:             lateout 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE22]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_no_asm:[0-9]+]] @no_asm() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE23:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE23]]: u32 [synthetic] = const<u32>(0);
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE_argc]]), const<i32>(1))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 asm volatile "mov eax, 10" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:                     template: "mov eax, 10";
// DEFAULT-NEXT:                     out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE23]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
