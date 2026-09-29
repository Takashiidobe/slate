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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r1:[0-9]+]] r1: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r2:[0-9]+]] r2: u64 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], neg<i32, overflow=ub>(const<i32>(16)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_j]], const<i32>(1));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_u]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j]])))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_r1]], shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_u]]), const<i32>(1)));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_r2]], shr<u64, amount_out_of_range=ub, fill=zero_extend>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j]])))), const<i32>(1)));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%[[VALUE_r1]]), read<u64>(%[[VALUE_r2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
