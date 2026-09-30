void abort(void);
void exit(int);

#include <limits.h>

#if ULONG_LONG_MAX != 18446744073709551615ull &&                               \
    ULONG_MAX != 18446744073709551615ull
int main(void) { exit(0); }
#else
#if ULONG_MAX != 18446744073709551615ull
typedef unsigned long long ull;
#else
typedef unsigned long ull;
#endif

#include <stdio.h>

void checkit(int);

int main(void) {
  const ull a = 0x1400000000ULL;
  const ull b = 0x80000000ULL;
  const ull c = a / b;
  const ull d = 0x1400000000ULL / 0x80000000ULL;

  checkit((int)c);
  checkit((int)d);

  exit(0);
}

void checkit(int a) {
  if (a != 40)
    abort();
}
#endif



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
// DEFAULT-NEXT:     type @type[[TYPE_ull:[0-9]+]] ull = u64;
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_checkit:[0-9]+]] @checkit(%[[VALUE_a:[0-9]+]] a: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(40))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: u64 [storage=automatic] [const] = const<u64>(85899345920);
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: u64 [storage=automatic] [const] = const<u64>(2147483648);
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: u64 [storage=automatic] [const] = div<u64, by_zero=ub>(read<u64>(%[[VALUE_a_2]]), read<u64>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: u64 [storage=automatic] [const] = div<u64, by_zero=ub>(const<u64>(85899345920), const<u64>(2147483648));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_checkit]], reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(%[[VALUE_c]]))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_checkit]], reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(read<u64>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
