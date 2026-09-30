#include <stdio.h>

void abort(void);

int shift_by_promoted_types(int seed) {
  int                i   = 4;
  unsigned           u   = 3;
  long               l   = 5;
  unsigned long long ull = 6;
  short              s   = 2;

  unsigned _BitInt(129) a   = 1;
  a                         = a << i;
  a                         = a >> u;
  a                       <<= l;
  a                       >>= s;
  a                         = a << ull;
  a                         = a + (unsigned _BitInt(129))seed;
  return (int)a;
}


// SLATE-FILECHECK-DEFINES DEFAULT

int shift_across_limbs(int seed) {
  unsigned _BitInt(129) wide = 1;
  wide                       = wide << 128;
  wide                       = wide >> 127;
  return (int)wide + seed;
}

int shift_signed_arithmetic(int seed) {
  _BitInt(256) n = -1024;
  n              = n >> 3;
  n              = n << 2;
  return (int)n + seed;
}

int shift_by_bitint(int seed) {
  _BitInt(256) amount = 4;
  _BitInt(256) v      = 3;
  v                   = v << amount;
  v                   = v >> amount;
  return (int)v + seed;
}

int main(void) {
  if (shift_by_promoted_types(0) != 1024)
    abort();
  if (shift_across_limbs(0) != 2)
    abort();
  if (shift_signed_arithmetic(0) != -512)
    abort();
  if (shift_by_bitint(0) != 3)
    abort();
  printf("%d %d %d %d\n", shift_by_promoted_types(1), shift_across_limbs(2),
         shift_signed_arithmetic(3), shift_by_bitint(4));
  return 0;
}

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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_shift_by_promoted_types:[0-9]+]] @shift_by_promoted_types(%[[VALUE_seed:[0-9]+]] seed: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(4);
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(5));
// DEFAULT-NEXT:         let %[[VALUE_ull:[0-9]+]] ull: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(6)));
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: i16 [storage=automatic] = truncate<i16, reason=assign, fits=always>(const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: u129b [storage=automatic] = reinterpret<u129b, reason=assign, fits=unknown>(widen<i129b, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         write<u129b>(%[[VALUE_a]], shl<u129b, overflow=wrap, amount_out_of_range=ub>(read<u129b>(%[[VALUE_a]]), read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         write<u129b>(%[[VALUE_a]], shr<u129b, amount_out_of_range=ub, fill=zero_extend>(read<u129b>(%[[VALUE_a]]), read<u32>(%[[VALUE_u]])));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u129b [synthetic] = read<u129b>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u129b [synthetic] = shl<u129b, overflow=wrap, amount_out_of_range=ub>(read<u129b>(%[[VALUE0]]), read<i64>(%[[VALUE_l]]));
// DEFAULT-NEXT:         write<u129b>(%[[VALUE_a]], read<u129b>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u129b [synthetic] = read<u129b>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u129b [synthetic] = shr<u129b, amount_out_of_range=ub, fill=zero_extend>(read<u129b>(%[[VALUE2]]), widen<i32, reason=promotion>(read<i16>(%[[VALUE_s]])));
// DEFAULT-NEXT:         write<u129b>(%[[VALUE_a]], read<u129b>(%[[VALUE3]]));
// DEFAULT-NEXT:         write<u129b>(%[[VALUE_a]], shl<u129b, overflow=wrap, amount_out_of_range=ub>(read<u129b>(%[[VALUE_a]]), read<u64>(%[[VALUE_ull]])));
// DEFAULT-NEXT:         write<u129b>(%[[VALUE_a]], add<u129b, overflow=wrap>(read<u129b>(%[[VALUE_a]]), reinterpret<u129b, reason=explicit, fits=unknown>(widen<i129b, reason=explicit>(read<i32>(%[[VALUE_seed]])))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(read<u129b>(%[[VALUE_a]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_shift_across_limbs:[0-9]+]] @shift_across_limbs(%[[VALUE_seed_2:[0-9]+]] seed: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_wide:[0-9]+]] wide: u129b [storage=automatic] = reinterpret<u129b, reason=assign, fits=unknown>(widen<i129b, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         write<u129b>(%[[VALUE_wide]], shl<u129b, overflow=wrap, amount_out_of_range=ub>(read<u129b>(%[[VALUE_wide]]), const<i32>(128)));
// DEFAULT-NEXT:         write<u129b>(%[[VALUE_wide]], shr<u129b, amount_out_of_range=ub, fill=zero_extend>(read<u129b>(%[[VALUE_wide]]), const<i32>(127)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(read<u129b>(%[[VALUE_wide]]))), read<i32>(%[[VALUE_seed_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_shift_signed_arithmetic:[0-9]+]] @shift_signed_arithmetic(%[[VALUE_seed_3:[0-9]+]] seed: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: i256b [storage=automatic] = widen<i256b, reason=assign>(neg<i32, overflow=ub>(const<i32>(1024)));
// DEFAULT-NEXT:         write<i256b>(%[[VALUE_n]], shr<i256b, amount_out_of_range=ub, fill=sign_extend>(read<i256b>(%[[VALUE_n]]), const<i32>(3)));
// DEFAULT-NEXT:         write<i256b>(%[[VALUE_n]], shl<i256b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i256b>(%[[VALUE_n]]), const<i32>(2)));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(truncate<i32, reason=explicit, fits=unknown>(read<i256b>(%[[VALUE_n]])), read<i32>(%[[VALUE_seed_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_shift_by_bitint:[0-9]+]] @shift_by_bitint(%[[VALUE_seed_4:[0-9]+]] seed: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_amount:[0-9]+]] amount: i256b [storage=automatic] = widen<i256b, reason=assign>(const<i32>(4));
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: i256b [storage=automatic] = widen<i256b, reason=assign>(const<i32>(3));
// DEFAULT-NEXT:         write<i256b>(%[[VALUE_v]], shl<i256b, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i256b>(%[[VALUE_v]]), read<i256b>(%[[VALUE_amount]])));
// DEFAULT-NEXT:         write<i256b>(%[[VALUE_v]], shr<i256b, amount_out_of_range=ub, fill=sign_extend>(read<i256b>(%[[VALUE_v]]), read<i256b>(%[[VALUE_amount]])));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(truncate<i32, reason=explicit, fits=unknown>(read<i256b>(%[[VALUE_v]])), read<i32>(%[[VALUE_seed_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_shift_by_promoted_types]], const<i32>(0)), const<i32>(1024))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_shift_across_limbs]], const<i32>(0)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_shift_signed_arithmetic]], const<i32>(0)), neg<i32, overflow=ub>(const<i32>(512)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_shift_by_bitint]], const<i32>(0)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_shift_by_promoted_types]], const<i32>(1)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_shift_across_limbs]], const<i32>(2)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_shift_signed_arithmetic]], const<i32>(3)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_shift_by_bitint]], const<i32>(4)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
