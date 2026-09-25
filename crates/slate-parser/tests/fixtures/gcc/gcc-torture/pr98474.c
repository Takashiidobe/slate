/* PR tree-optimization/98474 */

#ifdef __SIZEOF_INT128__
typedef __uint128_t T;
#define N (__SIZEOF_INT128__ * __CHAR_BIT__ / 2)
#else
typedef unsigned long long T;
#define N (__SIZEOF_LONG_LONG__ * __CHAR_BIT__ / 2)
#endif

__attribute__((noipa)) void foo(T *x) { *x += ((T)1) << (N + 1); }

int main() {
  T a = ((T)1) << (N + 1);
  T b = a;
  T n;
  foo(&b);
  n = b;
  while (n >= a)
    n -= a;
  if ((int)(n >> N) != 0)
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
// DEFAULT-NEXT:     type @type0 T = u128;
// DEFAULT-NEXT:     fn %1 @foo(%2 x: ptr<u128>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8: ptr<u128> [synthetic] = read<ptr<u128>>(%2);
// DEFAULT-NEXT:         let %9: u128 [synthetic] = read<u128>(deref(read<ptr<u128>>(%8)));
// DEFAULT-NEXT:         let %10: u128 [synthetic] = add<u128, overflow=wrap>(read<u128>(%9), shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), add<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(16), const<i32>(8)), const<i32>(2)), const<i32>(1))));
// DEFAULT-NEXT:         write<u128>(deref(read<ptr<u128>>(%8)), read<u128>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 a: u128 [storage=automatic] = shl<u128, overflow=wrap, amount_out_of_range=ub>(reinterpret<u128, reason=explicit, fits=unknown>(widen<i128, reason=explicit>(const<i32>(1))), add<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(16), const<i32>(8)), const<i32>(2)), const<i32>(1)));
// DEFAULT-NEXT:         let %5 b: u128 [storage=automatic] = read<u128>(%4);
// DEFAULT-NEXT:         let %6 n: u128 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u128>) -> void>(%1, addr_of<ptr<u128>>(%5));
// DEFAULT-NEXT:         write<u128>(%6, read<u128>(%5));
// DEFAULT-NEXT:         while %7 ge<u128>(read<u128>(%6), read<u128>(%4))
// DEFAULT-NEXT:             let %11: u128 [synthetic] = read<u128>(%6);
// DEFAULT-NEXT:             let %12: u128 [synthetic] = sub<u128, overflow=wrap>(read<u128>(%11), read<u128>(%4));
// DEFAULT-NEXT:             write<u128>(%6, read<u128>(%12));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(shr<u128, amount_out_of_range=ub, fill=zero_extend>(read<u128>(%6), div<i32, by_zero=ub, min_by_neg_one=ub>(mul<i32, overflow=ub>(const<i32>(16), const<i32>(8)), const<i32>(2))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
