/* { dg-require-effective-target int128 } */

typedef unsigned __int128 u128;

int  a, c, d;
u128 b;

unsigned long long g0, g1;

void store(unsigned long long a0, unsigned long long a1) {
  g0 = a0;
  g1 = a1;
}

void foo(void) {
  b      += a;
  c       = d != 84347;
  b      /= c;
  u128 x  = b;
  store(x >> 0, x >> 64);
}

int main(void) {
  foo();
  if (g0 != 0 || g1 != 0)
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type0 u128 = u128;
// DEFAULT-NEXT:     global %1 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 b: u128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 g0: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 g1: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %7 @store(%8 a0: u64, %9 a1: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u64>(%5, read<u64>(%8));
// DEFAULT-NEXT:         write<u64>(%6, read<u64>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13: u128 [synthetic] = read<u128>(%4);
// DEFAULT-NEXT:         let %14: u128 [synthetic] = add<u128, overflow=wrap>(read<u128>(%13), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(read<i32>(%1))));
// DEFAULT-NEXT:         write<u128>(%4, read<u128>(%14));
// DEFAULT-NEXT:         write<i32>(%2, from_bool<i32, reason=assign>(ne<i32>(read<i32>(%3), const<i32>(84347))));
// DEFAULT-NEXT:         let %15: u128 [synthetic] = read<u128>(%4);
// DEFAULT-NEXT:         let %16: u128 [synthetic] = div<u128, by_zero=ub>(read<u128>(%15), reinterpret<u128, reason=usual_arith, fits=unknown>(widen<i128, reason=usual_arith>(read<i32>(%2))));
// DEFAULT-NEXT:         write<u128>(%4, read<u128>(%16));
// DEFAULT-NEXT:         let %11 x: u128 [storage=automatic] = read<u128>(%4);
// DEFAULT-NEXT:         call<void, signature=fn(u64, u64) -> void>(%7, truncate<u64, reason=arg, fits=unknown>(shr<u128, amount_out_of_range=ub, fill=zero_extend>(read<u128>(%11), const<i32>(0))), truncate<u64, reason=arg, fits=unknown>(shr<u128, amount_out_of_range=ub, fill=zero_extend>(read<u128>(%11), const<i32>(64))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         if logical_or<bool>(ne<u64>(read<u64>(%5), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))), ne<u64>(read<u64>(%6), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
