
/// Accept intel inline asm but write it out as att:

/// Accept intel inline asm and write it out as intel:

/// Check MS compat mode (_MSC_VER defined). The driver always picks intel
/// output in that mode, so test only that.

// Test that intrinsics headers still work with -masm=intel.
#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

void f(void) {
  // Intrinsic headers contain macros and inline functions.
  // Inline assembly in both are checked only when they are
  // referenced, so reference a few intrinsics here.
  __SSC_MARK(4);
  int a;
  _hreset(a);
  _pconfig_u32(0, (void*)0);

  _encls_u32(0, (void*)0);
  _enclu_u32(0, (void*)0);
  _enclv_u32(0, (void*)0);
#ifdef _MSC_VER
  __movsb((void*)0, (void*)0, 0);
  __movsd((void*)0, (void*)0, 0);
  __movsw((void*)0, (void*)0, 0);
  __stosb((void*)0, 0, 0);
  __stosd((void*)0, 0, 0);
  __stosw((void*)0, 0, 0);
#ifdef __x86_64__
  __movsq((void*)0, (void*)0, 0);
  __stosq((void*)0, 0, 0);
#endif
  __cpuid((void*)0, 0);
  __cpuidex((void*)0, 0, 0);
  __halt();
  __nop();
  __readmsr(0);
  __readcr3();
  __writecr3(0);

  _InterlockedExchange_HLEAcquire((void*)0, 0);
  _InterlockedExchange_HLERelease((void*)0, 0);
  _InterlockedCompareExchange_HLEAcquire((void*)0, 0, 0);
  _InterlockedCompareExchange_HLERelease((void*)0, 0, 0);
#ifdef __x86_64__
  _InterlockedExchange64_HLEAcquire((void*)0, 0);
  _InterlockedExchange64_HLERelease((void*)0, 0);
  _InterlockedCompareExchange64_HLEAcquire((void*)0, 0, 0);
  _InterlockedCompareExchange64_HLERelease((void*)0, 0, 0);
#endif
#endif


  __asm__("mov eax, ebx");

  // Explicitly overriding asm style per block works:
  __asm__(".att_syntax\nmovl %ebx, %eax");

  // The .att_syntax was only scoped to the previous statement.
  // (This is different from gcc, where `.att_syntax` is in
  // effect from that point on, so portable code would want an
  // explicit `.intel_syntax noprefix\n` at the start of this string).
  __asm__("mov eax, ebx");
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17
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
// DEFAULT-NEXT:     fn %1 @_InterlockedExchange_HLEAcquire(%26 <unnamed>: ptr<volatile i32>, %27 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @_InterlockedExchange_HLERelease(%28 <unnamed>: ptr<volatile i32>, %29 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @_InterlockedCompareExchange_HLEAcquire(%30 <unnamed>: ptr<volatile i32>, %31 <unnamed>: i32, %32 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @_InterlockedCompareExchange_HLERelease(%33 <unnamed>: ptr<volatile i32>, %34 <unnamed>: i32, %35 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @_pconfig_u32(%36 <unnamed>: i32 [const], %37 __data: ptr<u32>) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @_encls_u32(%38 <unnamed>: i32 [const], %39 __data: ptr<u32>) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %7 @_enclu_u32(%40 <unnamed>: i32 [const], %41 __data: ptr<u32>) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @_enclv_u32(%42 <unnamed>: i32 [const], %43 __data: ptr<u32>) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @_hreset(%44 <unnamed>: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %10 @__cpuid(%45 <unnamed>: ptr<i32> [array=4], %46 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %11 @__cpuidex(%47 <unnamed>: ptr<i32> [array=4], %48 <unnamed>: i32, %49 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %12 @__halt() -> void [linkage=external];
// DEFAULT-NEXT:     fn %13 @__movsb(%50 <unnamed>: ptr<u8>, %51 <unnamed>: ptr<const u8>, %52 <unnamed>: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %14 @__movsd(%53 <unnamed>: ptr<u32>, %54 <unnamed>: ptr<const u32>, %55 <unnamed>: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %15 @__movsw(%56 <unnamed>: ptr<u16>, %57 <unnamed>: ptr<const u16>, %58 <unnamed>: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %16 @__nop() -> void [linkage=external];
// DEFAULT-NEXT:     fn %17 @__readcr3() -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %18 @__readmsr(%59 <unnamed>: u32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %19 @__stosb(%60 <unnamed>: ptr<u8>, %61 <unnamed>: u8, %62 <unnamed>: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %20 @__stosd(%63 <unnamed>: ptr<u32>, %64 <unnamed>: u32, %65 <unnamed>: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %21 @__stosw(%66 <unnamed>: ptr<u16>, %67 <unnamed>: u16, %68 <unnamed>: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %22 @__writecr3(%69 <unnamed>: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %24 @__SSC_MARK(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %23 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%24, const<i32>(4));
// DEFAULT-NEXT:         let %25 a: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%9, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%25)));
// DEFAULT-NEXT:         call<u32, signature=fn(i32, ptr<u32>) -> u32>(%5, const<i32>(0), null<ptr<u32>>);
// DEFAULT-NEXT:         call<u32, signature=fn(i32, ptr<u32>) -> u32>(%6, const<i32>(0), null<ptr<u32>>);
// DEFAULT-NEXT:         call<u32, signature=fn(i32, ptr<u32>) -> u32>(%7, const<i32>(0), null<ptr<u32>>);
// DEFAULT-NEXT:         call<u32, signature=fn(i32, ptr<u32>) -> u32>(%8, const<i32>(0), null<ptr<u32>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u8>, ptr<const u8>, u32) -> void>(%13, null<ptr<u8>>, null<ptr<const u8>>, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u32>, ptr<const u32>, u32) -> void>(%14, null<ptr<u32>>, null<ptr<const u32>>, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u16>, ptr<const u16>, u32) -> void>(%15, null<ptr<u16>>, null<ptr<const u16>>, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u8>, u8, u32) -> void>(%19, null<ptr<u8>>, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u32>, u32, u32) -> void>(%20, null<ptr<u32>>, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u16>, u16, u32) -> void>(%21, null<ptr<u16>>, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%10, null<ptr<i32>>, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32, i32) -> void>(%11, null<ptr<i32>>, const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         call<u64, signature=fn(u32) -> u64>(%18, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%17);
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%22, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<volatile i32>, i32) -> i32>(%1, null<ptr<volatile i32>>, const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<volatile i32>, i32) -> i32>(%2, null<ptr<volatile i32>>, const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<volatile i32>, i32, i32) -> i32>(%3, null<ptr<volatile i32>>, const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<volatile i32>, i32, i32) -> i32>(%4, null<ptr<volatile i32>>, const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         asm "mov eax, ebx" [dialect=att] [options=nostack];
// DEFAULT-NEXT:         asm ".att_syntax\\nmovl %ebx, %eax" [dialect=att] [options=nostack];
// DEFAULT-NEXT:         asm "mov eax, ebx" [dialect=att] [options=nostack];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
