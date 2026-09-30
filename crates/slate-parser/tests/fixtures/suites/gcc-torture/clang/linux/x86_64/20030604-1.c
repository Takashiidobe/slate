// SLATE-FILECHECK-DEFINES DEFAULT

/* PR optimization/10876 */
/* Contributed by Christian Ehrhardt */

/* Verify that the SPARC port doesn't emit
   (minus) (reg) (const_int) insns.  */

void f(void)
{
  unsigned int butterfly, block, offset;
  double *Z;

  for (block = 0; block < 512; block += 512) {
    double T1re, T2re;
    offset = butterfly + block;
    T1re += T2re;
    T2re = Z[offset] + T1re;
  }
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
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_butterfly:[0-9]+]] butterfly: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_block:[0-9]+]] block: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_offset:[0-9]+]] offset: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_Z:[0-9]+]] Z: ptr<f64> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_block]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%[[VALUE_block]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(512)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_block]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE1]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(512)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_block]], read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_T1re:[0-9]+]] T1re: f64 [storage=automatic];
// DEFAULT-NEXT:                     let %[[VALUE_T2re:[0-9]+]] T2re: f64 [storage=automatic];
// DEFAULT-NEXT:                     write<u32>(%[[VALUE_offset]], add<u32, overflow=wrap>(read<u32>(%[[VALUE_butterfly]]), read<u32>(%[[VALUE_block]])));
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: f64 [synthetic] = read<f64>(%[[VALUE_T1re]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE3]]), read<f64>(%[[VALUE_T2re]]));
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_T1re]], read<f64>(%[[VALUE4]]));
// DEFAULT-NEXT:                     write<f64>(%[[VALUE_T2re]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(%[[VALUE_Z]]), read<u32>(%[[VALUE_offset]])))), read<f64>(%[[VALUE_T1re]])));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
