// SLATE-FILECHECK-DEFINES DEFAULT

// the i686 winnt.h shapes: EDX:EAX results and a bare `__asm int` statement.
typedef unsigned __int64 ULONGLONG;
typedef long long LONGLONG;
typedef unsigned long DWORD;

#pragma warning(disable:4035)

__inline ULONGLONG Int64ShllMod32(ULONGLONG Value, DWORD ShiftCount) {
    __asm {
        mov     ecx, ShiftCount
        mov     eax, dword ptr [Value]
        mov     edx, dword ptr [Value+4]
        shld    edx, eax, cl
        shl     eax, cl
    }
}

__inline LONGLONG Int64ShraMod32(LONGLONG Value, DWORD ShiftCount) {
    __asm {
        mov     ecx, ShiftCount
        mov     eax, dword ptr [Value]
        mov     edx, dword ptr [Value+4]
        shrd    eax, edx, cl
        sar     edx, cl
    }
}

#pragma warning(default:4035)

__inline void DbgRaiseAssertionFailure(void) {
    __asm int 0x2c
}

ULONGLONG shift(ULONGLONG value, DWORD count) {
    DbgRaiseAssertionFailure();
    return Int64ShllMod32(value, count) + (ULONGLONG)Int64ShraMod32((LONGLONG)value, count);
}

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
// DEFAULT-NEXT:     fn %3 @Int64ShllMod32(%4 Value: u64, %5 ShiftCount: u32) -> u64 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret(or<u64>(widen<u64, reason=return>(read<u32>(%13)), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%14)), const<u32>(32))))] {
// DEFAULT-NEXT:         let %13: u32 [synthetic];
// DEFAULT-NEXT:         let %14: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov ecx, ShiftCount\nmov eax, dword ptr [Value]\nmov edx, dword ptr [Value + 4]\nshld edx, eax, cl\nshl eax, cl" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov ecx, " addr(%2) "\nmov eax, " addr<dword>(%3) "\nmov edx, " addr<dword>(%3 + 4) "\nshld edx, eax, cl\nshl eax, cl";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%13);
// DEFAULT-NEXT:             out 1 "{edx}" [{dx}] width 32 place<u32>(%14);
// DEFAULT-NEXT:             in 2 [ShiftCount] mem<read> place<u32>(%5);
// DEFAULT-NEXT:             in 3 [Value] mem<read> place<u64>(%4);
// DEFAULT-NEXT:             clobbers: "ecx" as cx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @Int64ShraMod32(%7 Value: i64, %8 ShiftCount: u32) -> i64 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret(reinterpret<i64, reason=return, fits=unknown>(or<u64>(widen<u64, reason=return>(read<u32>(%15)), shl<u64, overflow=wrap, amount_out_of_range=ub>(widen<u64, reason=return>(read<u32>(%16)), const<u32>(32)))))] {
// DEFAULT-NEXT:         let %15: u32 [synthetic];
// DEFAULT-NEXT:         let %16: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov ecx, ShiftCount\nmov eax, dword ptr [Value]\nmov edx, dword ptr [Value + 4]\nshrd eax, edx, cl\nsar edx, cl" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov ecx, " addr(%2) "\nmov eax, " addr<dword>(%3) "\nmov edx, " addr<dword>(%3 + 4) "\nshrd eax, edx, cl\nsar edx, cl";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%15);
// DEFAULT-NEXT:             out 1 "{edx}" [{dx}] width 32 place<u32>(%16);
// DEFAULT-NEXT:             in 2 [ShiftCount] mem<read> place<u32>(%8);
// DEFAULT-NEXT:             in 3 [Value] mem<read> place<i64>(%7);
// DEFAULT-NEXT:             clobbers: "ecx" as cx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @DbgRaiseAssertionFailure() -> void [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "int 44" [dialect=intel] {
// DEFAULT-NEXT:             template: "int 44";
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @shift(%11 value: u64, %12 count: u32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         return add<u64, overflow=wrap>(call<u64, signature=fn(u64, u32) -> u64>(%3, read<u64>(%11), read<u32>(%12)), reinterpret<u64, reason=explicit, fits=unknown>(call<i64, signature=fn(i64, u32) -> i64>(%6, reinterpret<i64, reason=explicit, fits=unknown>(read<u64>(%11)), read<u32>(%12))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
