extern void abort();

int main() {
  int    i;
  double oldrho;
  double beta = 0.0;
  double work = 1.0;
  for (i = 1; i <= 2; i++) {
    double rho = work * work;
    if (i != 1)
      beta = rho / oldrho;
    if (beta == 1.0)
      abort();

    /* All targets even remotely likely to ever get supported
       use at least an even base, so there will never be any
       floating-point rounding. All computation in this test
       case is exact for even bases.  */
    work   /= 2.0;
    oldrho  = rho;
  }
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
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_oldrho:[0-9]+]] oldrho: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_beta:[0-9]+]] beta: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %[[VALUE_work:[0-9]+]] work: f64 [storage=automatic] = const<f64>(1.0);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(1));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_rho:[0-9]+]] rho: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_work]]), read<f64>(%[[VALUE_work]]));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1))
// DEFAULT-NEXT:                         write<f64>(%[[VALUE_beta]], div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_rho]]), read<f64>(%[[VALUE_oldrho]])));
// DEFAULT-NEXT:                     if eq<f64, exceptions=observable>(read<f64>(%[[VALUE_beta]]), const<f64>(1.0))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: f64 [synthetic] = read<f64>(%[[VALUE_work]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: f64 [synthetic] = div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE3]]), const<f64>(2.0));
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_work]], read<f64>(%[[VALUE4]]));
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_oldrho]], read<f64>(%[[VALUE_rho]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
