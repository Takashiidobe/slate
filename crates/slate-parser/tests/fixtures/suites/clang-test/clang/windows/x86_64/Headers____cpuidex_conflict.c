// Make sure that __cpuidex in cpuid.h doesn't conflict with the MS
// extensions built in by ensuring compilation succeeds:

// Ensure that we do not run into conflicts when offloading.

typedef __SIZE_TYPE__ size_t;

// We declare __cpuidex here as where the buitlin should be exposed (MSVC), the
// declaration is in <intrin.h>, but <intrin.h> is not available from all the
// targets that are being tested here.
IS_STATIC void __cpuidex (int[4], int, int);

#include <cpuid.h>

int cpuid_info[4];

void test_cpuidex(unsigned level, unsigned count) {
  __cpuidex(cpuid_info, level, count);
}

// SLATE-FILECHECK-DEFINES DEFAULT -DIS_STATIC=
// SLATE-FILECHECK-STD DEFAULT gnu17

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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %5 cpuid_info: array<i32, 4> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %1 @__cpuidex(%2 __cpu_info: ptr<i32> [array=4], %3 __leaf: i32, %4 __subleaf: i32) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm "  xchg{q|}  {%%|}rbx,%q1\\n  cpuid\\n  xchg{q|}  {%%|}rbx,%q1" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "  xchgq  " %% "rbx," %q1(64) "\\n  cpuid\\n  xchgq  " %% "rbx," %q1(64);
// DEFAULT-NEXT:             inlateout 0 "a" [{ax}] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%2), const<i32>(0)))) from read<i32>(%3);
// DEFAULT-NEXT:             lateout 1 "r" [reg] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%2), const<i32>(1))));
// DEFAULT-NEXT:             inlateout 2 "c" [{cx}] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%2), const<i32>(2)))) from read<i32>(%4);
// DEFAULT-NEXT:             lateout 3 "d" [{dx}] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%2), const<i32>(3))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_cpuidex(%7 level: u32, %8 count: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32, i32) -> void>(%1, array_decay<ptr<i32>, length=Some(4)>(%5), reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%7)), reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
