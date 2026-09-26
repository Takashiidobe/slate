/* { dg-do run } */
/* { dg-require-effective-target int32plus } */

long long int          a = -465274079317386463LL;
int                    b = 856872806;
int                    c = -1940894202;
int                    d = 1718449211;
int                    e = -392681565;
unsigned long long int f = 13521452247506316486ULL;
int                    g = -13194608;

__attribute__((noinline, noclone)) void foo() {
  if (!a - a)
    c = b = 0;
  else
    d = 3UL * a == 0;
  if (g / a)
    e = 0 < -a + 500849970701012771LL + (unsigned long)-a;
  else
    f = 4081116982543369LL & a;
}

int
main() {
  asm volatile("" : : : "memory");
  foo();
  if (f != 2818598057803777LL)
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
// DEFAULT-NEXT:     global %0 a: i64 [storage=static] = neg<i64, overflow=ub>(const<i64>(465274079317386463)) [linkage=external];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] = const<i32>(856872806) [linkage=external];
// DEFAULT-NEXT:     global %2 c: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1940894202)) [linkage=external];
// DEFAULT-NEXT:     global %3 d: i32 [storage=static] = const<i32>(1718449211) [linkage=external];
// DEFAULT-NEXT:     global %4 e: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(392681565)) [linkage=external];
// DEFAULT-NEXT:     global %5 f: u64 [storage=static] = const<u64>(13521452247506316486) [linkage=external];
// DEFAULT-NEXT:     global %6 g: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(13194608)) [linkage=external];
// DEFAULT-NEXT:     fn %7 @foo() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i64>(sub<i64, overflow=ub>(widen<i64, reason=usual_arith>(from_bool<i32, reason=promotion>(not<bool>(ne<i64>(read<i64>(%0), const<i64>(0))))), read<i64>(%0)), const<i64>(0))
// DEFAULT-NEXT:             write<i32>(%1, const<i32>(0));
// DEFAULT-NEXT:             write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%3, from_bool<i32, reason=assign>(eq<u64>(mul<u64, overflow=wrap>(const<u64>(3), reinterpret<u64, reason=usual_arith, fits=unknown>(read<i64>(%0))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         if ne<i64>(div<i64, by_zero=ub, min_by_neg_one=ub>(widen<i64, reason=usual_arith>(read<i32>(%6)), read<i64>(%0)), const<i64>(0))
// DEFAULT-NEXT:             write<i32>(%4, from_bool<i32, reason=assign>(lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(add<i64, overflow=ub>(neg<i64, overflow=ub>(read<i64>(%0)), const<i64>(500849970701012771))), reinterpret<u64, reason=explicit, fits=unknown>(neg<i64, overflow=ub>(read<i64>(%0)))))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<u64>(%5, reinterpret<u64, reason=assign, fits=unknown>(and<i64>(const<i64>(4081116982543369), read<i64>(%0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%5), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(2818598057803777)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
