/* PR optimization/13313 */
/* Origin: Mike Lerwill <mike@ml-solutions.co.uk> */

extern void abort(void);

void DisplayNumber(unsigned long v) {
  if (v != 0x9aL)
    abort();
}

unsigned long ReadNumber(void) { return 0x009a0000L; }

int main(void) {
  unsigned long tmp;
  tmp = (ReadNumber() & 0x00ff0000L) >> 16;
  DisplayNumber(tmp);
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
// DEFAULT-NEXT:     fn %[[VALUE_DisplayNumber:[0-9]+]] @DisplayNumber(%[[VALUE_v:[0-9]+]] v: u64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_v]]), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(154)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ReadNumber:[0-9]+]] @ReadNumber() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=always>(const<i64>(10092544));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_tmp:[0-9]+]] tmp: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%[[VALUE_tmp]], shr<u64, amount_out_of_range=ub, fill=zero_extend>(and<u64>(call<u64, signature=fn() -> u64>(%[[VALUE_ReadNumber]]), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(16711680))), const<i32>(16)));
// DEFAULT-NEXT:         call<void, signature=fn(u64) -> void>(%[[VALUE_DisplayNumber]], read<u64>(%[[VALUE_tmp]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
