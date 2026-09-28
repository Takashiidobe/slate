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
// DEFAULT-NEXT:     type @type0 ULONGLONG = u64;
// DEFAULT-NEXT:     type @type1 LONGLONG = i64;
// DEFAULT-NEXT:     type @type2 DWORD = u32;
// DEFAULT-NEXT:     type @type3 Result = enum : u32 {
// DEFAULT-NEXT:         %0 First = const<i32>(0);
// DEFAULT-NEXT:         %1 Second = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type4 Pair = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %3 @Int64ShllMod32(%4 Value: u64, %5 ShiftCount: u32) -> u64 [linkage=external] [fallthrough=ret(or<u64>(widen<u64, reason=return>(read<u32>(%42)), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%43)), const<u32>(32))))] {
// DEFAULT-NEXT:         let %42: u32 [synthetic];
// DEFAULT-NEXT:         let %43: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov ecx, ShiftCount\nmov eax, dword ptr [Value]\nmov edx, dword ptr [Value + 4]\nshld edx, eax, cl\nshl eax, cl" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov ecx, " addr(%2) "\nmov eax, " addr<dword>(%3) "\nmov edx, " addr<dword>(%3 + 4) "\nshld edx, eax, cl\nshl eax, cl";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%42);
// DEFAULT-NEXT:             out 1 "{edx}" [{dx}] width 32 place<u32>(%43);
// DEFAULT-NEXT:             in 2 [ShiftCount] mem<read> place<u32>(%5);
// DEFAULT-NEXT:             in 3 [Value] mem<read> place<u64>(%4);
// DEFAULT-NEXT:             clobbers: "ecx" as cx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @Int64ShraMod32(%7 Value: i64, %8 ShiftCount: u32) -> i64 [linkage=external] [fallthrough=ret(reinterpret<i64, reason=return, fits=unknown>(or<u64>(widen<u64, reason=return>(read<u32>(%44)), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%45)), const<u32>(32)))))] {
// DEFAULT-NEXT:         let %44: u32 [synthetic];
// DEFAULT-NEXT:         let %45: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov ecx, ShiftCount\nmov eax, dword ptr [Value]\nmov edx, dword ptr [Value + 4]\nshrd eax, edx, cl\nsar edx, cl" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov ecx, " addr(%2) "\nmov eax, " addr<dword>(%3) "\nmov edx, " addr<dword>(%3 + 4) "\nshrd eax, edx, cl\nsar edx, cl";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%44);
// DEFAULT-NEXT:             out 1 "{edx}" [{dx}] width 32 place<u32>(%45);
// DEFAULT-NEXT:             in 2 [ShiftCount] mem<read> place<u32>(%8);
// DEFAULT-NEXT:             in 3 [Value] mem<read> place<i64>(%7);
// DEFAULT-NEXT:             clobbers: "ecx" as cx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @Int64ShrlMod32(%10 Value: u64, %11 ShiftCount: u32) -> u64 [linkage=external] [fallthrough=ret(or<u64>(widen<u64, reason=return>(read<u32>(%46)), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%47)), const<u32>(32))))] {
// DEFAULT-NEXT:         let %46: u32 [synthetic];
// DEFAULT-NEXT:         let %47: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov ecx, ShiftCount\nmov eax, dword ptr [Value]\nmov edx, dword ptr [Value + 4]\nshrd eax, edx, cl\nshr edx, cl" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov ecx, " addr(%2) "\nmov eax, " addr<dword>(%3) "\nmov edx, " addr<dword>(%3 + 4) "\nshrd eax, edx, cl\nshr edx, cl";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%46);
// DEFAULT-NEXT:             out 1 "{edx}" [{dx}] width 32 place<u32>(%47);
// DEFAULT-NEXT:             in 2 [ShiftCount] mem<read> place<u32>(%11);
// DEFAULT-NEXT:             in 3 [Value] mem<read> place<u64>(%10);
// DEFAULT-NEXT:             clobbers: "ecx" as cx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @word(%13 input: i32) -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%48)))] {
// DEFAULT-NEXT:         let %48: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, input" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, " addr(%1);
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%48);
// DEFAULT-NEXT:             in 1 [input] mem<read> place<i32>(%13);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @byte() -> u8 [linkage=external] [fallthrough=ret(truncate<u8, reason=return, fits=unknown>(read<u32>(%49)))] {
// DEFAULT-NEXT:         let %49: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov al, 255" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov al, 255";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%49);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @half() -> i16 [linkage=external] [fallthrough=ret(reinterpret<i16, reason=return, fits=unknown>(truncate<u16, reason=return, fits=unknown>(read<u32>(%50))))] {
// DEFAULT-NEXT:         let %50: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov ax, 32768" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov ax, 32768";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%50);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @boolean() -> bool [linkage=external] [fallthrough=ret(ne<u1, reason=return>(truncate<u1, reason=return, fits=unknown>(read<u32>(%51)), const<u1>(0)))] {
// DEFAULT-NEXT:         let %51: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, 2" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 2";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%51);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @pointer() -> ptr<void> [linkage=external] [fallthrough=ret(int_to_ptr<ptr<void>, reason=return>(read<u32>(%52)))] {
// DEFAULT-NEXT:         let %52: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, 4096" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 4096";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%52);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @enumeration() -> @type3 [linkage=external] [fallthrough=ret(int_to_enum<@type3, reason=return>(read<u32>(%53)))] {
// DEFAULT-NEXT:         let %53: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, 1" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 1";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%53);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @bits() -> u33b [linkage=external] [fallthrough=ret(truncate<u33b, reason=return, fits=unknown>(or<u64>(widen<u64, reason=return>(read<u32>(%54)), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%55)), const<u32>(32)))))] {
// DEFAULT-NEXT:         let %54: u32 [synthetic];
// DEFAULT-NEXT:         let %55: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, 1" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 1";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%54);
// DEFAULT-NEXT:             lateout 1 "{edx}" [{dx}] width 32 place<u32>(%55);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @branches(%24 condition: i32) -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%56)))] {
// DEFAULT-NEXT:         let %56: u32 [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%24), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 asm volatile "mov eax, 1" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:                     template: "mov eax, 1";
// DEFAULT-NEXT:                     out 0 "{eax}" [{ax}] width 32 place<u32>(%56);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 asm volatile "mov eax, 2" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:                     template: "mov eax, 2";
// DEFAULT-NEXT:                     out 0 "{eax}" [{ax}] width 32 place<u32>(%56);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @multiple() -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%57)))] {
// DEFAULT-NEXT:         let %57: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, 3" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 3";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%57);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "nop" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:             lateout 0 "{eax}" [{ax}] width 32 place<u32>(%57);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @high_only() -> i64 [linkage=external] [fallthrough=ret(reinterpret<i64, reason=return, fits=unknown>(or<u64>(widen<u64, reason=return>(read<u32>(%58)), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%59)), const<u32>(32)))))] {
// DEFAULT-NEXT:         let %58: u32 [synthetic];
// DEFAULT-NEXT:         let %59: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov edx, 4" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov edx, 4";
// DEFAULT-NEXT:             lateout 0 "{eax}" [{ax}] width 32 place<u32>(%58);
// DEFAULT-NEXT:             out 1 "{edx}" [{dx}] width 32 place<u32>(%59);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @low_only() -> i64 [linkage=external] [fallthrough=ret(reinterpret<i64, reason=return, fits=unknown>(or<u64>(widen<u64, reason=return>(read<u32>(%60)), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%61)), const<u32>(32)))))] {
// DEFAULT-NEXT:         let %60: u32 [synthetic];
// DEFAULT-NEXT:         let %61: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, 5" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 5";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%60);
// DEFAULT-NEXT:             lateout 1 "{edx}" [{dx}] width 32 place<u32>(%61);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @explicit_return() -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%62)))] {
// DEFAULT-NEXT:         let %62: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, 6" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 6";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%62);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @partial_return(%30 condition: i32) -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%63)))] {
// DEFAULT-NEXT:         let %63: u32 [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%30), const<i32>(0))
// DEFAULT-NEXT:             return const<i32>(8);
// DEFAULT-NEXT:         asm volatile "mov eax, 9" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 9";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%63);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @nothing() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "nop" [dialect=intel] {
// DEFAULT-NEXT:             template: "nop";
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @floating() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm volatile "fldz" [dialect=intel] {
// DEFAULT-NEXT:             template: "fldz";
// DEFAULT-NEXT:             clobbers: "st" as st, "st(1)" as st(1), "st(2)" as st(2), "st(3)" as st(3), "st(4)" as st(4), "st(5)" as st(5), "st(6)" as st(6), "st(7)" as st(7);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @aggregate() -> @type4 [linkage=external] [abi=x86_win32() -> coerce<i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm volatile "mov eax, 1" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, 1";
// DEFAULT-NEXT:             clobbers: "eax" as ax;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @wide() -> u65b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm volatile "mov eax, 1" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, 1";
// DEFAULT-NEXT:             clobbers: "eax" as ax;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @naked() -> i32 [linkage=external] [naked] [fallthrough=ub] {
// DEFAULT-NEXT:         asm volatile "mov eax, 1" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, 1";
// DEFAULT-NEXT:             clobbers: "eax" as ax;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @never() -> i32 [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         let %64: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "int 3" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "int 3";
// DEFAULT-NEXT:             lateout 0 "{eax}" [{ax}] width 32 place<u32>(%64);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @no_asm() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @main(%40 argc: i32, %41 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%65)))] {
// DEFAULT-NEXT:         let %65: u32 [synthetic] = const<u32>(0);
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%40), const<i32>(1))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 asm volatile "mov eax, 10" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:                     template: "mov eax, 10";
// DEFAULT-NEXT:                     out 0 "{eax}" [{ax}] width 32 place<u32>(%65);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
