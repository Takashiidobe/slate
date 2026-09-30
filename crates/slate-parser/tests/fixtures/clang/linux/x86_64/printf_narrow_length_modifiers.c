#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
  short          s  = 300;
  unsigned short us = 60000;
  signed char    c  = -5;
  unsigned char  uc = 200;
  intmax_t       j  = 123456789L;
  uintmax_t      ju = 123456789UL;
  ptrdiff_t      t  = -7;
  printf("%hd %hu %hhd %hhu %jd %ju %td\n", s, us, c, uc, j, ju, t);

  int          full_int = 300;
  int          negative = -1;
  unsigned int wide     = 70000;
  printf("%hhd %hhd %hhu %hd %hu\n", full_int, uc, negative, wide, wide);
  return 0;
}




// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
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
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_ptrdiff_t:[0-9]+]] ptrdiff_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___intmax_t:[0-9]+]] __intmax_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___uintmax_t:[0-9]+]] __uintmax_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_intmax_t:[0-9]+]] intmax_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_uintmax_t:[0-9]+]] uintmax_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 31> [storage=static] = code_units<array<i8, 31>>([37, 104, 100, 32, 37, 104, 117, 32, 37, 104, 104, 100, 32, 37, 104, 104, 117, 32, 37, 106, 100, 32, 37, 106, 117, 32, 37, 116, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 24> [storage=static] = code_units<array<i8, 24>>([37, 104, 104, 100, 32, 37, 104, 104, 100, 32, 37, 104, 104, 117, 32, 37, 104, 100, 32, 37, 104, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: i16 [storage=automatic] = truncate<i16, reason=assign, fits=always>(const<i32>(300));
// DEFAULT-NEXT:         let %[[VALUE_us:[0-9]+]] us: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(const<i32>(60000)));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(5)));
// DEFAULT-NEXT:         let %[[VALUE_uc:[0-9]+]] uc: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(200)));
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i64 [storage=automatic] = const<i64>(123456789);
// DEFAULT-NEXT:         let %[[VALUE_ju:[0-9]+]] ju: u64 [storage=automatic] = const<u64>(123456789);
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: i64 [storage=automatic] = widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(7)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(31)>(%[[VALUE_str]])), widen<i32, reason=vararg>(read<i16>(%[[VALUE_s]])), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u16>(%[[VALUE_us]]))), widen<i32, reason=vararg>(read<i8>(%[[VALUE_c]])), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(%[[VALUE_uc]]))), read<i64>(%[[VALUE_j]]), read<u64>(%[[VALUE_ju]]), read<i64>(%[[VALUE_t]]));
// DEFAULT-NEXT:         let %[[VALUE_full_int:[0-9]+]] full_int: i32 [storage=automatic] = const<i32>(300);
// DEFAULT-NEXT:         let %[[VALUE_negative:[0-9]+]] negative: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_wide:[0-9]+]] wide: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(70000));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(24)>(%[[VALUE_str_2]])), read<i32>(%[[VALUE_full_int]]), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(%[[VALUE_uc]]))), read<i32>(%[[VALUE_negative]]), read<u32>(%[[VALUE_wide]]), read<u32>(%[[VALUE_wide]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
