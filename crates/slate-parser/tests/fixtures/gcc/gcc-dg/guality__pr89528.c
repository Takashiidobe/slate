/* PR debug/89528 */
/* { dg-do run } */
/* { dg-options "-g" } */

#include <stdio.h>

char         b;
int          d, e;
static int   i = 1;
void         a(int l) { printf("", l); }
char         c(char l) { return l || b && l == 1 ? b : b % l; }
short        f(int l, int m) { return l * m; }
short        g(short l, short m) { return m || l == 767 && m == 1; }
int          h(int l, int m) { return (l ^ m & l ^ (m & 647) - m ^ m) < m; }
static int   j(int l) { return d == 0 || l == 647 && d == 1 ? l : l % d; }
short        k(int l) { return l >= 2 >> l; }
void         optimize_me_not() { asm(""); }
static short n(void) {
  int l_1127 = ~j(9 || 0) ^ 65535;
  optimize_me_not(); /* { dg-final { gdb-test . "l_1127+1" "-65534" } } */
  f(l_1127, i && e ^ 4) && g(0, 0);
  e = 0;
  return 5;
}
int main() { n(); }




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
// DEFAULT-NEXT:     global %1 b: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 i: i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%26 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @a(%6 l: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%27)), read<i32>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @c(%8 l: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(conditional<i32>(logical_or<bool>(ne<i8>(read<i8>(%8), const<i8>(0)), logical_and<bool>(ne<i8>(read<i8>(%1), const<i8>(0)), eq<i32>(widen<i32, reason=promotion>(read<i8>(%8)), const<i32>(1)))), widen<i32, reason=promotion>(read<i8>(%1)), rem<i32, by_zero=ub, min_by_neg_one=ub>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%8)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @f(%10 l: i32, %11 m: i32) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=unknown>(mul<i32, overflow=ub>(read<i32>(%10), read<i32>(%11)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @g(%13 l: i16, %14 m: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i16, reason=return>(logical_or<bool>(ne<i16>(read<i16>(%14), const<i16>(0)), logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i16>(%13)), const<i32>(767)), eq<i32>(widen<i32, reason=promotion>(read<i16>(%14)), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @h(%16 l: i32, %17 m: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<i32>(xor<i32>(xor<i32>(xor<i32>(read<i32>(%16), and<i32>(read<i32>(%17), read<i32>(%16))), sub<i32, overflow=ub>(and<i32>(read<i32>(%17), const<i32>(647)), read<i32>(%17))), read<i32>(%17)), read<i32>(%17)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @j(%19 l: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_or<bool>(eq<i32>(read<i32>(%2), const<i32>(0)), logical_and<bool>(eq<i32>(read<i32>(%19), const<i32>(647)), eq<i32>(read<i32>(%2), const<i32>(1)))), read<i32>(%19), rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%19), read<i32>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @k(%21 l: i32) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i16, reason=return>(ge<i32>(read<i32>(%21), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(2), read<i32>(%21))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @optimize_me_not() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm "" [dialect=att];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @n() -> i16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %24 l_1127: i32 [storage=automatic] = xor<i32>(not<i32>(call<i32, signature=fn(i32) -> i32>(%18, from_bool<i32, reason=arg>(logical_or<bool>(ne<i32>(const<i32>(9), const<i32>(0)), ne<i32>(const<i32>(0), const<i32>(0)))))), const<i32>(65535));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%22);
// DEFAULT-NEXT:         let %28: bool [synthetic];
// DEFAULT-NEXT:         if ne<i16>(call<i16, signature=fn(i32, i32) -> i16>(%9, read<i32>(%24), from_bool<i32, reason=arg>(logical_and<bool>(ne<i32>(read<i32>(%4), const<i32>(0)), ne<i32>(xor<i32>(read<i32>(%3), const<i32>(4)), const<i32>(0))))), const<i16>(0))
// DEFAULT-NEXT:             write<bool>(%28, ne<i16>(call<i16, signature=fn(i16, i16) -> i16>(%12, truncate<i16, reason=arg, fits=always>(const<i32>(0)), truncate<i16, reason=arg, fits=always>(const<i32>(0))), const<i16>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%28, const<bool>(false));
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:         return truncate<i16, reason=return, fits=always>(const<i32>(5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i16, signature=fn() -> i16>(%23);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
