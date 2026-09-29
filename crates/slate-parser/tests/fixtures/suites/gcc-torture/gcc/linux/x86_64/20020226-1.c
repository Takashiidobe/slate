/* This tests the rotate patterns that some machines support.  */

#include <limits.h>

void abort(void);
void exit(int);

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

#define ROR(a, b) (((a) >> (b)) | ((a) << ((sizeof(a) * CHAR_BIT) - (b))))
#define ROL(a, b) (((a) << (b)) | ((a) >> ((sizeof(a) * CHAR_BIT) - (b))))

#define CHAR_VALUE  ((unsigned char)0x1234U)
#define SHORT_VALUE ((unsigned short)0x1234U)
#define INT_VALUE   0x1234U
#define LONG_VALUE  0x12345678LU
#define LL_VALUE    0x12345678abcdef0LLU

#define SHIFT1 4
#define SHIFT2 ((sizeof(long long) * CHAR_BIT) - SHIFT1)

unsigned char      uc     = CHAR_VALUE;
unsigned short     us     = SHORT_VALUE;
unsigned int       ui     = INT_VALUE;
unsigned long      ul     = LONG_VALUE;
unsigned long long ull    = LL_VALUE;
int                shift1 = SHIFT1;
int                shift2 = SHIFT2;

int main(void) {
  if (ROR(uc, shift1) != ROR(CHAR_VALUE, SHIFT1))
    abort();

  if (ROR(uc, SHIFT1) != ROR(CHAR_VALUE, SHIFT1))
    abort();

  if (ROR(us, shift1) != ROR(SHORT_VALUE, SHIFT1))
    abort();

  if (ROR(us, SHIFT1) != ROR(SHORT_VALUE, SHIFT1))
    abort();

  if (ROR(ui, shift1) != ROR(INT_VALUE, SHIFT1))
    abort();

  if (ROR(ui, SHIFT1) != ROR(INT_VALUE, SHIFT1))
    abort();

  if (ROR(ul, shift1) != ROR(LONG_VALUE, SHIFT1))
    abort();

  if (ROR(ul, SHIFT1) != ROR(LONG_VALUE, SHIFT1))
    abort();

  if (ROR(ull, shift1) != ROR(LL_VALUE, SHIFT1))
    abort();

  if (ROR(ull, SHIFT1) != ROR(LL_VALUE, SHIFT1))
    abort();

  if (ROR(ull, shift2) != ROR(LL_VALUE, SHIFT2))
    abort();

  if (ROR(ull, SHIFT2) != ROR(LL_VALUE, SHIFT2))
    abort();

  if (ROL(uc, shift1) != ROL(CHAR_VALUE, SHIFT1))
    abort();

  if (ROL(uc, SHIFT1) != ROL(CHAR_VALUE, SHIFT1))
    abort();

  if (ROL(us, shift1) != ROL(SHORT_VALUE, SHIFT1))
    abort();

  if (ROL(us, SHIFT1) != ROL(SHORT_VALUE, SHIFT1))
    abort();

  if (ROL(ui, shift1) != ROL(INT_VALUE, SHIFT1))
    abort();

  if (ROL(ui, SHIFT1) != ROL(INT_VALUE, SHIFT1))
    abort();

  if (ROL(ul, shift1) != ROL(LONG_VALUE, SHIFT1))
    abort();

  if (ROL(ul, SHIFT1) != ROL(LONG_VALUE, SHIFT1))
    abort();

  if (ROL(ull, shift1) != ROL(LL_VALUE, SHIFT1))
    abort();

  if (ROL(ull, SHIFT1) != ROL(LL_VALUE, SHIFT1))
    abort();

  if (ROL(ull, shift2) != ROL(LL_VALUE, SHIFT2))
    abort();

  if (ROL(ull, SHIFT2) != ROL(LL_VALUE, SHIFT2))
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
// DEFAULT-NEXT:     global %[[VALUE_uc:[0-9]+]] uc: u8 [storage=static] = truncate<u8, reason=explicit, fits=unknown>(const<u32>(4660)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_us:[0-9]+]] us: u16 [storage=static] = truncate<u16, reason=explicit, fits=always>(const<u32>(4660)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ui:[0-9]+]] ui: u32 [storage=static] = const<u32>(4660) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ul:[0-9]+]] ul: u64 [storage=static] = const<u64>(305419896) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ull:[0-9]+]] ull: u64 [storage=static] = const<u64>(81985529234382576) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_shift1:[0-9]+]] shift1: i32 [storage=static] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_shift2:[0-9]+]] shift2: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u8>(%[[VALUE_uc]]))),
// DEFAULT-SAME: read<i32>(%[[VALUE_shift1]])), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u8>(%[[VALUE_uc]]))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith,
// DEFAULT-SAME: fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64,
// DEFAULT-SAME: reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion,
// DEFAULT-SAME: fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(const<u32>(4660)))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(const<u32>(4660)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u8>(%[[VALUE_uc]]))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion,
// DEFAULT-SAME: fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_uc]]))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64,
// DEFAULT-SAME: reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(const<u32>(4660)))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(const<u32>(4660)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u16>(%[[VALUE_us]]))),
// DEFAULT-SAME: read<i32>(%[[VALUE_shift1]])), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u16>(%[[VALUE_us]]))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith,
// DEFAULT-SAME: fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64,
// DEFAULT-SAME: reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion,
// DEFAULT-SAME: fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=always>(const<u32>(4660)))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=always>(const<u32>(4660)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u16>(%[[VALUE_us]]))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion,
// DEFAULT-SAME: fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_us]]))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64,
// DEFAULT-SAME: reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=always>(const<u32>(4660)))), const<i32>(4)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=always>(const<u32>(4660)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(or<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_ui]]), read<i32>(%[[VALUE_shift1]])), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_ui]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4660), const<i32>(4)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4660), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(or<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_ui]]), const<i32>(4)), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_ui]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4660), const<i32>(4)), shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4660), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_ul]]), read<i32>(%[[VALUE_shift1]])), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_ul]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(const<u64>(305419896), const<i32>(4)), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(305419896), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_ul]]), const<i32>(4)), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_ul]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(const<u64>(305419896), const<i32>(4)), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(305419896), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_ull]]), read<i32>(%[[VALUE_shift1]])), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_ull]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(const<u64>(81985529234382576), const<i32>(4)), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_ull]]), const<i32>(4)), shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_ull]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(const<u64>(81985529234382576), const<i32>(4)), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_ull]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_shift2]])), shl<u64, overflow=wrap,
// DEFAULT-SAME: amount_out_of_range=ub>(read<u64>(%[[VALUE_ull]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith,
// DEFAULT-SAME: fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64,
// DEFAULT-SAME: reason=usual_arith>(read<i32>(%[[VALUE_shift2]])))))), or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(const<u64>(81985529234382576), sub<u64,
// DEFAULT-SAME: overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_ull]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8),
// DEFAULT-SAME: reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shl<u64, overflow=wrap,
// DEFAULT-SAME: amount_out_of_range=ub>(read<u64>(%[[VALUE_ull]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith,
// DEFAULT-SAME: fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))), or<u64>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(const<u64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u8>(%[[VALUE_uc]]))),
// DEFAULT-SAME: read<i32>(%[[VALUE_shift1]])), shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u8>(%[[VALUE_uc]]))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith,
// DEFAULT-SAME: fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64,
// DEFAULT-SAME: reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32,
// DEFAULT-SAME: reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(const<u32>(4660)))), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(const<u32>(4660)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u8>(%[[VALUE_uc]]))), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion,
// DEFAULT-SAME: fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_uc]]))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64,
// DEFAULT-SAME: reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(const<u32>(4660)))), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(const<u32>(4660)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u16>(%[[VALUE_us]]))),
// DEFAULT-SAME: read<i32>(%[[VALUE_shift1]])), shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u16>(%[[VALUE_us]]))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith,
// DEFAULT-SAME: fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64,
// DEFAULT-SAME: reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32,
// DEFAULT-SAME: reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=always>(const<u32>(4660)))), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=always>(const<u32>(4660)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32,
// DEFAULT-SAME: reason=promotion>(read<u16>(%[[VALUE_us]]))), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion,
// DEFAULT-SAME: fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_us]]))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64,
// DEFAULT-SAME: reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=always>(const<u32>(4660)))), const<i32>(4)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u16, reason=explicit, fits=always>(const<u32>(4660)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_ui]]), read<i32>(%[[VALUE_shift1]])), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_ui]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4660), const<i32>(4)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4660), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_ui]]), const<i32>(4)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_ui]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(const<u32>(4660), const<i32>(4)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(const<u32>(4660), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_ul]]), read<i32>(%[[VALUE_shift1]])), shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_ul]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(305419896), const<i32>(4)), shr<u64, amount_out_of_range=ub, fill=zero_extend>(const<u64>(305419896), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_ul]]), const<i32>(4)), shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_ul]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(305419896), const<i32>(4)), shr<u64, amount_out_of_range=ub, fill=zero_extend>(const<u64>(305419896), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_ull]]), read<i32>(%[[VALUE_shift1]])), shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_ull]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_shift1]])))))), or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(81985529234382576), const<i32>(4)), shr<u64, amount_out_of_range=ub, fill=zero_extend>(const<u64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_ull]]), const<i32>(4)), shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_ull]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))), or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(81985529234382576), const<i32>(4)), shr<u64, amount_out_of_range=ub, fill=zero_extend>(const<u64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_ull]]),
// DEFAULT-SAME: read<i32>(%[[VALUE_shift2]])), shr<u64, amount_out_of_range=ub,
// DEFAULT-SAME: fill=zero_extend>(read<u64>(%[[VALUE_ull]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith,
// DEFAULT-SAME: fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64,
// DEFAULT-SAME: reason=usual_arith>(read<i32>(%[[VALUE_shift2]])))))), or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(81985529234382576), sub<u64,
// DEFAULT-SAME: overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shr<u64, amount_out_of_range=ub, fill=zero_extend>(const<u64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE_ull]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8),
// DEFAULT-SAME: reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shr<u64, amount_out_of_range=ub,
// DEFAULT-SAME: fill=zero_extend>(read<u64>(%[[VALUE_ull]]), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith,
// DEFAULT-SAME: fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))), or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))), shr<u64, amount_out_of_range=ub, fill=zero_extend>(const<u64>(81985529234382576), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
