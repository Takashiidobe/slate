void abort(void);
void exit(int);

typedef unsigned long long ULL;
ULL                        back;
ULL                        hpart, lpart;
ULL                        build(long h, long l) {
  hpart   = h;
  hpart <<= 32;
  lpart   = l;
  lpart  &= 0xFFFFFFFFLL;
  back    = hpart | lpart;
  return back;
}

int main(void) {
  if (build(0, 1) != 0x0000000000000001LL)
    abort();
  if (build(0, 0) != 0x0000000000000000LL)
    abort();
  if (build(0, 0xFFFFFFFF) != 0x00000000FFFFFFFFLL)
    abort();
  if (build(0, 0xFFFFFFFE) != 0x00000000FFFFFFFELL)
    abort();
  if (build(1, 1) != 0x0000000100000001LL)
    abort();
  if (build(1, 0) != 0x0000000100000000LL)
    abort();
  if (build(1, 0xFFFFFFFF) != 0x00000001FFFFFFFFLL)
    abort();
  if (build(1, 0xFFFFFFFE) != 0x00000001FFFFFFFELL)
    abort();
  if (build(0xFFFFFFFF, 1) != 0xFFFFFFFF00000001LL)
    abort();
  if (build(0xFFFFFFFF, 0) != 0xFFFFFFFF00000000LL)
    abort();
  if (build(0xFFFFFFFF, 0xFFFFFFFF) != 0xFFFFFFFFFFFFFFFFLL)
    abort();
  if (build(0xFFFFFFFF, 0xFFFFFFFE) != 0xFFFFFFFFFFFFFFFELL)
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
// DEFAULT-NEXT:     type @type[[TYPE_ULL:[0-9]+]] ULL = u64;
// DEFAULT-NEXT:     global %[[VALUE_back:[0-9]+]] back: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_hpart:[0-9]+]] hpart: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_lpart:[0-9]+]] lpart: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_build:[0-9]+]] @build(%[[VALUE_h:[0-9]+]] h: i64, %[[VALUE_l:[0-9]+]] l: i64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u64>(%[[VALUE_hpart]], reinterpret<u64, reason=assign, fits=unknown>(read<i64>(%[[VALUE_h]])));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_hpart]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%[[VALUE1]]), const<i32>(32));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_hpart]], read<u64>(%[[VALUE2]]));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_lpart]], reinterpret<u64, reason=assign, fits=unknown>(read<i64>(%[[VALUE_l]])));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_lpart]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u64 [synthetic] = and<u64>(read<u64>(%[[VALUE3]]), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(4294967295)));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_lpart]], read<u64>(%[[VALUE4]]));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_back]], or<u64>(read<u64>(%[[VALUE_hpart]]), read<u64>(%[[VALUE_lpart]])));
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_back]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i64, i64) -> u64>(%[[VALUE_build]], widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i64, i64) -> u64>(%[[VALUE_build]], widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i64, i64) -> u64>(%[[VALUE_build]], widen<i64, reason=arg>(const<i32>(0)), reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(const<u32>(4294967295)))), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(4294967295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i64, i64) -> u64>(%[[VALUE_build]], widen<i64, reason=arg>(const<i32>(0)), reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(const<u32>(4294967294)))), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(4294967294)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i64, i64) -> u64>(%[[VALUE_build]], widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(const<i32>(1))), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(4294967297)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i64, i64) -> u64>(%[[VALUE_build]], widen<i64, reason=arg>(const<i32>(1)), widen<i64, reason=arg>(const<i32>(0))), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(4294967296)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i64, i64) -> u64>(%[[VALUE_build]], widen<i64, reason=arg>(const<i32>(1)), reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(const<u32>(4294967295)))), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(8589934591)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i64, i64) -> u64>(%[[VALUE_build]], widen<i64, reason=arg>(const<i32>(1)), reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(const<u32>(4294967294)))), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(8589934590)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i64, i64) -> u64>(%[[VALUE_build]], reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(const<u32>(4294967295))), widen<i64, reason=arg>(const<i32>(1))), const<u64>(18446744069414584321))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i64, i64) -> u64>(%[[VALUE_build]], reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(const<u32>(4294967295))), widen<i64, reason=arg>(const<i32>(0))), const<u64>(18446744069414584320))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i64, i64) -> u64>(%[[VALUE_build]], reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(const<u32>(4294967295))), reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(const<u32>(4294967295)))), const<u64>(18446744073709551615))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(i64, i64) -> u64>(%[[VALUE_build]], reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(const<u32>(4294967295))), reinterpret<i64, reason=arg, fits=unknown>(widen<u64, reason=arg>(const<u32>(4294967294)))), const<u64>(18446744073709551614))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
