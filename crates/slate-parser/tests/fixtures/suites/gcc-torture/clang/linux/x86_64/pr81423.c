/* PR rtl-optimization/81423 */

extern void abort(void);

unsigned long long int ll   = 0;
unsigned long long int ull1 = 1ULL;
unsigned long long int ull2 = 12008284144813806346ULL;
unsigned long long int ull3;

unsigned long long int __attribute__((noinline)) foo(void) {
  ll = -5597998501375493990LL;

  ll = (unsigned int)(5677365550390624949LL - ll) - (ull1 > 0);
  unsigned long long int ull3;
  ull3 = (unsigned int)(2067854353LL
                        << (((ll + -2129105131LL) ^ 10280750144413668236ULL) -
                            10280750143997242009ULL)) >>
         ((2873442921854271231ULL | ull2) - 12098357307243495419ULL);

  return ull3;
}

int main(void) {
  /* We need a long long of exactly 64 bits and int of exactly 32 bits
     for this test.  */
  if (__SIZEOF_LONG_LONG__ * __CHAR_BIT__ != 64 ||
      __SIZEOF_INT__ * __CHAR_BIT__ != 32)
    return 0;

  ull3 = foo();
  if (ull3 != 3998784)
    abort();
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
// DEFAULT-NEXT:     global %1 ll: u64 [storage=static] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %2 ull1: u64 [storage=static] = const<u64>(1) [linkage=external];
// DEFAULT-NEXT:     global %3 ull2: u64 [storage=static] = const<u64>(12008284144813806346) [linkage=external];
// DEFAULT-NEXT:     global %4 ull3: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @foo() -> u64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u64>(%1, reinterpret<u64, reason=assign, fits=unknown>(neg<i64, overflow=ub>(const<i64>(5597998501375493990))));
// DEFAULT-NEXT:         write<u64>(%1, widen<u64, reason=assign>(sub<u32, overflow=wrap>(truncate<u32, reason=explicit, fits=unknown>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(5677365550390624949)), read<u64>(%1))), reinterpret<u32, reason=usual_arith, fits=always>(from_bool<i32, reason=promotion>(gt<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0)))))))));
// DEFAULT-NEXT:         let %6 ull3: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%6, widen<u64, reason=assign>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u32, reason=explicit, fits=unknown>(truncate<i32, reason=explicit, fits=unknown>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(2067854353), sub<u64, overflow=wrap>(xor<u64>(add<u64, overflow=wrap>(read<u64>(%1), reinterpret<u64, reason=usual_arith, fits=unknown>(neg<i64, overflow=ub>(const<i64>(2129105131)))), const<u64>(10280750144413668236)), const<u64>(10280750143997242009))))), sub<u64, overflow=wrap>(or<u64>(const<u64>(2873442921854271231), read<u64>(%3)), const<u64>(12098357307243495419)))));
// DEFAULT-NEXT:         return read<u64>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(mul<i32, overflow=ub>(const<i32>(8), const<i32>(8)), const<i32>(64)), ne<i32>(mul<i32, overflow=ub>(const<i32>(4), const<i32>(8)), const<i32>(32)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<u64>(%4, call<u64, signature=fn() -> u64>(%5));
// DEFAULT-NEXT:         call<u64, signature=fn() -> u64>(%5);
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3998784))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
