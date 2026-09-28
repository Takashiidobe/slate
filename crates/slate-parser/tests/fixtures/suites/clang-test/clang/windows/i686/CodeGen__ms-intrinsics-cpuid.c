
// intrin.h needs size_t, but -ffreestanding prevents us from getting it from
// stddef.h.  Work around it with this typedef.
typedef __SIZE_TYPE__ size_t;

#include <intrin.h>

#pragma intrinsic(__cpuid)

void test__cpuid(int cpuInfo[4], int function_id) {
  __cpuid(cpuInfo, function_id);
}


#pragma intrinsic(__cpuidex)

void test__cpuidex(int cpuInfo[4], int function_id, int subfunction_id) {
  __cpuidex(cpuInfo, function_id, subfunction_id);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT -Werror

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
// DEFAULT-NEXT:     type @type0 size_t = u32;
// DEFAULT-NEXT:     type @type1 size_t = u32;
// DEFAULT-NEXT:     type @type2 size_t = u32;
// DEFAULT-NEXT:     fn %1 @__cpuid(%10 <unnamed>: ptr<i32> [array=4], %11 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @__cpuidex(%12 <unnamed>: ptr<i32> [array=4], %13 <unnamed>: i32, %14 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @test__cpuid(%4 cpuInfo: ptr<i32> [array=4], %5 function_id: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%1, read<ptr<i32>>(%4), read<i32>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test__cpuidex(%7 cpuInfo: ptr<i32> [array=4], %8 function_id: i32, %9 subfunction_id: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32, i32) -> void>(%2, read<ptr<i32>>(%7), read<i32>(%8), read<i32>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
