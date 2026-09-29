void abort(void);
void exit(int);

unsigned test(unsigned one, unsigned bit) {
  unsigned val  = bit & 1;
  unsigned zero = one >> 1;

  val++;
  return zero + (val >> 1);
}

int main() {
  if (test(1, 0) != 0)
    abort();
  if (test(1, 1) != 1)
    abort();
  if (test(1, 65535) != 1)
    abort();
  exit(0);

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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_one:[0-9]+]] one: u32, %[[VALUE_bit:[0-9]+]] bit: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_val:[0-9]+]] val: u32 [storage=automatic] = and<u32>(read<u32>(%[[VALUE_bit]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_zero:[0-9]+]] zero: u32 [storage=automatic] = shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_one]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_val]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE1]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_val]], read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(read<u32>(%[[VALUE_zero]]), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_val]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32, u32) -> u32>(%[[VALUE_test]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32, u32) -> u32>(%[[VALUE_test]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(u32, u32) -> u32>(%[[VALUE_test]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(65535))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
