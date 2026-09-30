void abort(void);
void exit(int);

int main(void) {
  long int i  = -2147483647L - 1L; /* 0x80000000 */
  char     ca = 1;

  if (i >> ca != -1073741824L)
    abort();

  if (i >> i / -2000000000L != -1073741824L)
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
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i64 [storage=automatic] = sub<i64, overflow=ub>(neg<i64, overflow=ub>(const<i64>(2147483647)), const<i64>(1));
// DEFAULT-NEXT:         let %[[VALUE_ca:[0-9]+]] ca: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         if ne<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_i]]), widen<i32, reason=promotion>(read<i8>(%[[VALUE_ca]]))), neg<i64, overflow=ub>(const<i64>(1073741824)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_i]]), div<i64, by_zero=ub, min_by_neg_one=ub>(read<i64>(%[[VALUE_i]]), neg<i64, overflow=ub>(const<i64>(2000000000)))), neg<i64, overflow=ub>(const<i64>(1073741824)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
