#include <limits.h>

void abort(void);
void exit(int);

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

#define ROR(a, b) (((a) >> (b)) | ((a) << ((sizeof(a) * CHAR_BIT) - (b))))
#define ROL(a, b) (((a) << (b)) | ((a) >> ((sizeof(a) * CHAR_BIT) - (b))))

#define CHAR_VALUE  ((char)0xf234)
#define SHORT_VALUE ((short)0xf234)
#define INT_VALUE   ((int)0xf234)
#define LONG_VALUE  ((long)0xf2345678L)
#define LL_VALUE    ((long long)0xf2345678abcdef0LL)

#define SHIFT1 4
#define SHIFT2 ((sizeof(long long) * CHAR_BIT) - SHIFT1)

char      c      = CHAR_VALUE;
short     s      = SHORT_VALUE;
int       i      = INT_VALUE;
long      l      = LONG_VALUE;
long long ll     = LL_VALUE;
int       shift1 = SHIFT1;
int       shift2 = SHIFT2;

int main(void) {
  if (ROR(c, shift1) != ROR(CHAR_VALUE, SHIFT1))
    abort();

  if (ROR(c, SHIFT1) != ROR(CHAR_VALUE, SHIFT1))
    abort();

  if (ROR(s, shift1) != ROR(SHORT_VALUE, SHIFT1))
    abort();

  if (ROR(s, SHIFT1) != ROR(SHORT_VALUE, SHIFT1))
    abort();

  if (ROR(i, shift1) != ROR(INT_VALUE, SHIFT1))
    abort();

  if (ROR(i, SHIFT1) != ROR(INT_VALUE, SHIFT1))
    abort();

  if (ROR(l, shift1) != ROR(LONG_VALUE, SHIFT1))
    abort();

  if (ROR(l, SHIFT1) != ROR(LONG_VALUE, SHIFT1))
    abort();

  if (ROR(ll, shift1) != ROR(LL_VALUE, SHIFT1))
    abort();

  if (ROR(ll, SHIFT1) != ROR(LL_VALUE, SHIFT1))
    abort();

  if (ROR(ll, shift2) != ROR(LL_VALUE, SHIFT2))
    abort();

  if (ROR(ll, SHIFT2) != ROR(LL_VALUE, SHIFT2))
    abort();

  if (ROL(c, shift1) != ROL(CHAR_VALUE, SHIFT1))
    abort();

  if (ROL(c, SHIFT1) != ROL(CHAR_VALUE, SHIFT1))
    abort();

  if (ROL(s, shift1) != ROL(SHORT_VALUE, SHIFT1))
    abort();

  if (ROL(s, SHIFT1) != ROL(SHORT_VALUE, SHIFT1))
    abort();

  if (ROL(i, shift1) != ROL(INT_VALUE, SHIFT1))
    abort();

  if (ROL(i, SHIFT1) != ROL(INT_VALUE, SHIFT1))
    abort();

  if (ROL(l, shift1) != ROL(LONG_VALUE, SHIFT1))
    abort();

  if (ROL(l, SHIFT1) != ROL(LONG_VALUE, SHIFT1))
    abort();

  if (ROL(ll, shift1) != ROL(LL_VALUE, SHIFT1))
    abort();

  if (ROL(ll, SHIFT1) != ROL(LL_VALUE, SHIFT1))
    abort();

  if (ROL(ll, shift2) != ROL(LL_VALUE, SHIFT2))
    abort();

  if (ROL(ll, SHIFT2) != ROL(LL_VALUE, SHIFT2))
    abort();

  exit(0);
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
// DEFAULT-NEXT:     global %2 c: i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(const<i32>(62004)) [linkage=external];
// DEFAULT-NEXT:     global %3 s: i16 [storage=static] = truncate<i16, reason=explicit, fits=unknown>(const<i32>(62004)) [linkage=external];
// DEFAULT-NEXT:     global %4 i: i32 [storage=static] = const<i32>(62004) [linkage=external];
// DEFAULT-NEXT:     global %5 l: i64 [storage=static] = const<i64>(4063516280) [linkage=external];
// DEFAULT-NEXT:     global %6 ll: i64 [storage=static] = const<i64>(1090791845765373680) [linkage=external];
// DEFAULT-NEXT:     global %7 shift1: i32 [storage=static] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     global %8 shift2: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%10 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%2)), read<i32>(%7)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(%2)), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7)))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(62004))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(62004))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(%2)), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(62004))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(62004))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%3)), read<i32>(%7)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i16>(%3)), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7)))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(62004))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(62004))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%3)), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i16>(%3)), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(62004))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(62004))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%4), read<i32>(%7)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%4), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7)))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(62004), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(62004), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%4), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%4), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(62004), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(62004), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%5), read<i32>(%7)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%5), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7)))))), or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(4063516280), const<i32>(4)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(4063516280), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%5), const<i32>(4)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%5), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(4063516280), const<i32>(4)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(4063516280), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%6), read<i32>(%7)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%6), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7)))))), or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(1090791845765373680), const<i32>(4)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(1090791845765373680), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%6), const<i32>(4)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%6), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(1090791845765373680), const<i32>(4)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(1090791845765373680), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%6), read<i32>(%8)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%6), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%8)))))), or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(1090791845765373680), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(1090791845765373680), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%6), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%6), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))), or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(1090791845765373680), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(1090791845765373680), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(%2)), read<i32>(%7)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%2)), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7)))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(62004))), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(62004))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%2)), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(62004))), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(62004))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i16>(%3)), read<i32>(%7)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%3)), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7)))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(62004))), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(62004))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i16>(%3)), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%3)), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(62004))), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=unknown>(const<i32>(62004))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%4), read<i32>(%7)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%4), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7)))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(62004), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(62004), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%4), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%4), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(62004), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(62004), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%5), read<i32>(%7)), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%5), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7)))))), or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(4063516280), const<i32>(4)), shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(4063516280), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%5), const<i32>(4)), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%5), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(4063516280), const<i32>(4)), shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(4063516280), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%6), read<i32>(%7)), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%6), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%7)))))), or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(1090791845765373680), const<i32>(4)), shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(1090791845765373680), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%6), const<i32>(4)), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%6), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(1090791845765373680), const<i32>(4)), shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(1090791845765373680), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%6), read<i32>(%8)), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%6), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%8)))))), or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(1090791845765373680), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(1090791845765373680), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%6), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%6), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))), or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(1090791845765373680), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(1090791845765373680), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
