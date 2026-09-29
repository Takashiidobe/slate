#include <limits.h>

void abort(void);
void exit(int);

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

#define ROR(a, b) (((a) >> (b)) | ((a) << ((sizeof(a) * CHAR_BIT) - (b))))
#define ROL(a, b) (((a) << (b)) | ((a) >> ((sizeof(a) * CHAR_BIT) - (b))))

#define CHAR_VALUE  ((char)0x1234)
#define SHORT_VALUE ((short)0x1234)
#define INT_VALUE   ((int)0x1234)
#define LONG_VALUE  ((long)0x12345678L)
#define LL_VALUE    ((long long)0x12345678abcdef0LL)

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
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i8 [storage=static] = truncate<i8, reason=explicit, fits=unknown>(const<i32>(4660)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(4660)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] = const<i32>(4660) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: i64 [storage=static] = const<i64>(305419896) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ll:[0-9]+]] ll: i64 [storage=static] = const<i64>(81985529234382576) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_shift1:[0-9]+]] shift1: i32 [storage=static] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_shift2:[0-9]+]] shift2: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), read<i32>(%[[VALUE_shift1]])), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(4660))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(4660))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(4660))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(4660))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_s]])), read<i32>(%[[VALUE_shift1]])), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_s]])), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(4660))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(4660))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_s]])), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_s]])), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(4660))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(4660))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_shift1]])), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE_i]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(4660), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(4660), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_i]]), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE_i]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(4660), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(4660), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_l]]), read<i32>(%[[VALUE_shift1]])), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%[[VALUE_l]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(305419896), const<i32>(4)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(305419896), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_l]]), const<i32>(4)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%[[VALUE_l]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(305419896), const<i32>(4)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(305419896), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_ll]]), read<i32>(%[[VALUE_shift1]])), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%[[VALUE_ll]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(81985529234382576), const<i32>(4)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_ll]]), const<i32>(4)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%[[VALUE_ll]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(81985529234382576), const<i32>(4)), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_ll]]), read<i32>(%[[VALUE_shift2]])), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%[[VALUE_ll]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift2]])))))), or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_ll]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%[[VALUE_ll]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))), or<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), read<i32>(%[[VALUE_shift1]])), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(4660))), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(4660))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_c]])), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(4660))), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(4660))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_s]])), read<i32>(%[[VALUE_shift1]])), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_s]])), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(4660))), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(4660))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_s]])), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_s]])), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(4660))), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(widen<i32, reason=promotion>(truncate<i16, reason=explicit, fits=always>(const<i32>(4660))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_shift1]])), shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_i]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(4660), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(4660), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE_i]]), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE_i]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(4660), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(4660), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%[[VALUE_l]]), read<i32>(%[[VALUE_shift1]])), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_l]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(305419896), const<i32>(4)), shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(305419896), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%[[VALUE_l]]), const<i32>(4)), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_l]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(305419896), const<i32>(4)), shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(305419896), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%[[VALUE_ll]]), read<i32>(%[[VALUE_shift1]])), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_ll]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(81985529234382576), const<i32>(4)), shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%[[VALUE_ll]]), const<i32>(4)), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_ll]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(81985529234382576), const<i32>(4)), shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%[[VALUE_ll]]), read<i32>(%[[VALUE_shift2]])), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_ll]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift2]])))))), or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%[[VALUE_ll]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_ll]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))), or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shr<i64, amount_out_of_range=ub, fill=sign_extend>(const<i64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
