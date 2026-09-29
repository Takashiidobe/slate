void abort(void);
void exit(int);

int f(unsigned bitcount, int mant) {
  int mask = -1 << bitcount;
  {
    if (!(mant & -mask))
      goto ab;
    if (mant & ~mask)
      goto auf;
  }
ab:
  return 0;
auf:
  return 1;
}

int main(void) {
  if (f(0, -1))
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_bitcount:[0-9]+]] bitcount: u32, %[[VALUE_mant:[0-9]+]] mant: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_mask:[0-9]+]] mask: i32 [storage=automatic] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(neg<i32, overflow=ub>(const<i32>(1)), read<u32>(%[[VALUE_bitcount]]));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             if not<bool>(ne<i32>(and<i32>(read<i32>(%[[VALUE_mant]]), neg<i32, overflow=ub>(read<i32>(%[[VALUE_mask]]))), const<i32>(0)))
// DEFAULT-NEXT:                 goto %[[VALUE_ab:[0-9]+]];
// DEFAULT-NEXT:             if ne<i32>(and<i32>(read<i32>(%[[VALUE_mant]]), not<i32>(read<i32>(%[[VALUE_mask]]))), const<i32>(0))
// DEFAULT-NEXT:                 goto %[[VALUE_auf:[0-9]+]];
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         label %[[VALUE_ab]] ab:
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         label %[[VALUE_auf]] auf:
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(u32, i32) -> i32>(%[[VALUE_f]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)), neg<i32, overflow=ub>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
