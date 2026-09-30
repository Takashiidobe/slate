
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
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_size_t_2:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_size_t_3:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     fn %[[VALUE___cpuid:[0-9]+]] @__cpuid(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i32> [array=4], %[[VALUE1:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___cpuidex:[0-9]+]] @__cpuidex(%[[VALUE2:[0-9]+]] <unnamed>: ptr<i32> [array=4], %[[VALUE3:[0-9]+]] <unnamed>: i32, %[[VALUE4:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test__cpuid:[0-9]+]] @test__cpuid(%[[VALUE_cpuInfo:[0-9]+]] cpuInfo: ptr<i32> [array=4], %[[VALUE_function_id:[0-9]+]] function_id: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%[[VALUE___cpuid]], read<ptr<i32>>(%[[VALUE_cpuInfo]]), read<i32>(%[[VALUE_function_id]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test__cpuidex:[0-9]+]] @test__cpuidex(%[[VALUE_cpuInfo_2:[0-9]+]] cpuInfo: ptr<i32> [array=4], %[[VALUE_function_id_2:[0-9]+]] function_id: i32, %[[VALUE_subfunction_id:[0-9]+]] subfunction_id: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32, i32) -> void>(%[[VALUE___cpuidex]], read<ptr<i32>>(%[[VALUE_cpuInfo_2]]), read<i32>(%[[VALUE_function_id_2]]), read<i32>(%[[VALUE_subfunction_id]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
