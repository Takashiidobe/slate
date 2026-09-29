
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
// DEFAULT-NEXT:     fn %[[VALUE__InterlockedExchange_HLEAcquire:[0-9]+]] @_InterlockedExchange_HLEAcquire(%[[VALUE0:[0-9]+]] <unnamed>: ptr<volatile i32>, %[[VALUE1:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__InterlockedExchange_HLERelease:[0-9]+]] @_InterlockedExchange_HLERelease(%[[VALUE2:[0-9]+]] <unnamed>: ptr<volatile i32>, %[[VALUE3:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__InterlockedCompareExchange_HLEAcquire:[0-9]+]] @_InterlockedCompareExchange_HLEAcquire(%[[VALUE4:[0-9]+]] <unnamed>: ptr<volatile i32>, %[[VALUE5:[0-9]+]] <unnamed>: i32, %[[VALUE6:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__InterlockedCompareExchange_HLERelease:[0-9]+]] @_InterlockedCompareExchange_HLERelease(%[[VALUE7:[0-9]+]] <unnamed>: ptr<volatile i32>, %[[VALUE8:[0-9]+]] <unnamed>: i32, %[[VALUE9:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__pconfig_u32:[0-9]+]] @_pconfig_u32(%[[VALUE10:[0-9]+]] <unnamed>: i32 [const], %[[VALUE___data:[0-9]+]] __data: ptr<u64>) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__encls_u32:[0-9]+]] @_encls_u32(%[[VALUE11:[0-9]+]] <unnamed>: i32 [const], %[[VALUE___data_2:[0-9]+]] __data: ptr<u64>) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__enclu_u32:[0-9]+]] @_enclu_u32(%[[VALUE12:[0-9]+]] <unnamed>: i32 [const], %[[VALUE___data_3:[0-9]+]] __data: ptr<u64>) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__enclv_u32:[0-9]+]] @_enclv_u32(%[[VALUE13:[0-9]+]] <unnamed>: i32 [const], %[[VALUE___data_4:[0-9]+]] __data: ptr<u64>) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE__hreset:[0-9]+]] @_hreset(%[[VALUE14:[0-9]+]] <unnamed>: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___cpuid:[0-9]+]] @__cpuid(%[[VALUE15:[0-9]+]] <unnamed>: ptr<i32> [array=4], %[[VALUE16:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___cpuidex:[0-9]+]] @__cpuidex(%[[VALUE17:[0-9]+]] <unnamed>: ptr<i32> [array=4], %[[VALUE18:[0-9]+]] <unnamed>: i32, %[[VALUE19:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___halt:[0-9]+]] @__halt() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___movsb:[0-9]+]] @__movsb(%[[VALUE20:[0-9]+]] <unnamed>: ptr<u8>, %[[VALUE21:[0-9]+]] <unnamed>: ptr<const u8>, %[[VALUE22:[0-9]+]] <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___movsd:[0-9]+]] @__movsd(%[[VALUE23:[0-9]+]] <unnamed>: ptr<u32>, %[[VALUE24:[0-9]+]] <unnamed>: ptr<const u32>, %[[VALUE25:[0-9]+]] <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___movsw:[0-9]+]] @__movsw(%[[VALUE26:[0-9]+]] <unnamed>: ptr<u16>, %[[VALUE27:[0-9]+]] <unnamed>: ptr<const u16>, %[[VALUE28:[0-9]+]] <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___nop:[0-9]+]] @__nop() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___readcr3:[0-9]+]] @__readcr3() -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___readmsr:[0-9]+]] @__readmsr(%[[VALUE29:[0-9]+]] <unnamed>: u32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___stosb:[0-9]+]] @__stosb(%[[VALUE30:[0-9]+]] <unnamed>: ptr<u8>, %[[VALUE31:[0-9]+]] <unnamed>: u8, %[[VALUE32:[0-9]+]] <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___stosd:[0-9]+]] @__stosd(%[[VALUE33:[0-9]+]] <unnamed>: ptr<u32>, %[[VALUE34:[0-9]+]] <unnamed>: u32, %[[VALUE35:[0-9]+]] <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___stosw:[0-9]+]] @__stosw(%[[VALUE36:[0-9]+]] <unnamed>: ptr<u16>, %[[VALUE37:[0-9]+]] <unnamed>: u16, %[[VALUE38:[0-9]+]] <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___writecr3:[0-9]+]] @__writecr3(%[[VALUE39:[0-9]+]] <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___SSC_MARK:[0-9]+]] @__SSC_MARK(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE___SSC_MARK]], const<i32>(4));
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE__hreset]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%[[VALUE_a]])));
// DEFAULT-NEXT:         call<u32, signature=fn(i32, ptr<u64>) -> u32>(%[[VALUE__pconfig_u32]], const<i32>(0), null<ptr<u64>>);
// DEFAULT-NEXT:         call<u32, signature=fn(i32, ptr<u64>) -> u32>(%[[VALUE__encls_u32]], const<i32>(0), null<ptr<u64>>);
// DEFAULT-NEXT:         call<u32, signature=fn(i32, ptr<u64>) -> u32>(%[[VALUE__enclu_u32]], const<i32>(0), null<ptr<u64>>);
// DEFAULT-NEXT:         call<u32, signature=fn(i32, ptr<u64>) -> u32>(%[[VALUE__enclv_u32]], const<i32>(0), null<ptr<u64>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u8>, ptr<const u8>, u64) -> void>(%[[VALUE___movsb]], null<ptr<u8>>, null<ptr<const u8>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u32>, ptr<const u32>, u64) -> void>(%[[VALUE___movsd]], null<ptr<u32>>, null<ptr<const u32>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u16>, ptr<const u16>, u64) -> void>(%[[VALUE___movsw]], null<ptr<u16>>, null<ptr<const u16>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u8>, u8, u64) -> void>(%[[VALUE___stosb]], null<ptr<u8>>, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u32>, u32, u64) -> void>(%[[VALUE___stosd]], null<ptr<u32>>, reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u16>, u16, u64) -> void>(%[[VALUE___stosw]], null<ptr<u16>>, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32) -> void>(%[[VALUE___cpuid]], null<ptr<i32>>, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>, i32, i32) -> void>(%[[VALUE___cpuidex]], null<ptr<i32>>, const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE___halt]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE___nop]]);
// DEFAULT-NEXT:         call<u64, signature=fn(u32) -> u64>(%[[VALUE___readmsr]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<u64, signature=fn() -> u64>(%[[VALUE___readcr3]]);
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%[[VALUE___writecr3]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<volatile i32>, i32) -> i32>(%[[VALUE__InterlockedExchange_HLEAcquire]], null<ptr<volatile i32>>, const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<volatile i32>, i32) -> i32>(%[[VALUE__InterlockedExchange_HLERelease]], null<ptr<volatile i32>>, const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<volatile i32>, i32, i32) -> i32>(%[[VALUE__InterlockedCompareExchange_HLEAcquire]], null<ptr<volatile i32>>, const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<volatile i32>, i32, i32) -> i32>(%[[VALUE__InterlockedCompareExchange_HLERelease]], null<ptr<volatile i32>>, const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         asm "mov eax, ebx" [dialect=att] [options=nostack];
// DEFAULT-NEXT:         asm ".att_syntax\\nmovl %ebx, %eax" [dialect=att] [options=nostack];
// DEFAULT-NEXT:         asm "mov eax, ebx" [dialect=att] [options=nostack];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
