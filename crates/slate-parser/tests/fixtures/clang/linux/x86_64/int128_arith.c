#include <stdio.h>

static __int128 add128(__int128 a, __int128 b) { return a + b; }

static unsigned __int128 mul128(unsigned __int128 a, unsigned __int128 b) {
  return a * b;
}

static void print128(unsigned __int128 v) {
  unsigned long long hi = (unsigned long long)(v >> 64);
  unsigned long long lo = (unsigned long long)v;
  printf("%llu:%llu\n", hi, lo);
}

int main(void) {
  __int128 a   = (__int128)9000000000000000000LL;
  __int128 b   = (__int128)9000000000000000000LL;
  __int128 sum = add128(a, b);
  print128((unsigned __int128)sum);

  unsigned __int128 x    = (unsigned __int128)1000000000000ULL;
  unsigned __int128 y    = (unsigned __int128)1000000000000ULL;
  unsigned __int128 prod = mul128(x, y);
  print128(prod);

  int cmp = (sum > 0) ? 1 : 0;
  printf("%d\n", cmp);

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
// DEFAULT-NEXT:     global %21 .str21: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([37, 108, 108, 117, 58, 37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%20 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @add128(%3 a: i128, %4 b: i128) -> i128 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i128, overflow=ub>(read<i128>(%3), read<i128>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @mul128(%6 a: u128, %7 b: u128) -> u128 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<u128, overflow=wrap>(read<u128>(%6), read<u128>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @print128(%9 v: u128) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10 hi: u64 [storage=automatic] = truncate<u64, reason=explicit, fits=unknown>(shr<u128, amount_out_of_range=ub, fill=zero_extend>(read<u128>(%9), const<i32>(64)));
// DEFAULT-NEXT:         let %11 lo: u64 [storage=automatic] = truncate<u64, reason=explicit, fits=unknown>(read<u128>(%9));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%21)), read<u64>(%10), read<u64>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 a: i128 [storage=automatic] = widen<i128, reason=explicit>(const<i64>(9000000000000000000));
// DEFAULT-NEXT:         let %14 b: i128 [storage=automatic] = widen<i128, reason=explicit>(const<i64>(9000000000000000000));
// DEFAULT-NEXT:         let %15 sum: i128 [storage=automatic] = call<i128, signature=fn(i128, i128) -> i128>(%2, read<i128>(%13), read<i128>(%14));
// DEFAULT-NEXT:         call<void, signature=fn(u128) -> void>(%8, reinterpret<u128, reason=explicit, fits=unknown>(read<i128>(%15)));
// DEFAULT-NEXT:         let %16 x: u128 [storage=automatic] = widen<u128, reason=explicit>(const<u64>(1000000000000));
// DEFAULT-NEXT:         let %17 y: u128 [storage=automatic] = widen<u128, reason=explicit>(const<u64>(1000000000000));
// DEFAULT-NEXT:         let %18 prod: u128 [storage=automatic] = call<u128, signature=fn(u128, u128) -> u128>(%5, read<u128>(%16), read<u128>(%17));
// DEFAULT-NEXT:         call<void, signature=fn(u128) -> void>(%8, read<u128>(%18));
// DEFAULT-NEXT:         let %19 cmp: i32 [storage=automatic] = conditional<i32>(gt<i128>(read<i128>(%15), widen<i128, reason=usual_arith>(const<i32>(0))), const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%22)), read<i32>(%19));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
