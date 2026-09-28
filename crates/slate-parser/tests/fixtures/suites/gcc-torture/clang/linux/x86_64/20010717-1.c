extern void abort(void);

int main() {
  int           i, j;
  unsigned long u, r1, r2;

  i = -16;
  j = 1;
  u = i + j;

  /* no sign extension upon shift */
  r1 = u >> 1;
  /* sign extension upon shift, but there shouldn't be */
  r2 = ((unsigned long)(i + j)) >> 1;

  if (r1 != r2)
    abort();

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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %3 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %4 u: u64 [storage=automatic];
// DEFAULT-NEXT:         let %5 r1: u64 [storage=automatic];
// DEFAULT-NEXT:         let %6 r2: u64 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%2, neg<i32, overflow=ub>(const<i32>(16)));
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(1));
// DEFAULT-NEXT:         write<u64>(%4, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%2), read<i32>(%3)))));
// DEFAULT-NEXT:         write<u64>(%5, shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%4), const<i32>(1)));
// DEFAULT-NEXT:         write<u64>(%6, shr<u64, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(add<i32, overflow=ub>(read<i32>(%2), read<i32>(%3)))), const<i32>(1)));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%5), read<u64>(%6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
