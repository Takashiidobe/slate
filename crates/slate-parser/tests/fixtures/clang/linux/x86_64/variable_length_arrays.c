// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

int total(int n, int m) {
  int a[n];
  int b[n][3];
  int (*p)[m];
  long c[n][m];
  a[1] = 2;
  return a[1] + sizeof a + sizeof(a) + sizeof b + sizeof c + sizeof(*p);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_total:[0-9]+]] @total(%[[VALUE_n:[0-9]+]] n: i32, %[[VALUE_m:[0-9]+]] m: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// IR-NEXT:         let %[[VALUE_a:[0-9]+]] a: vla<i32, %[[VALUE0]]> [storage=automatic];
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// IR-NEXT:         let %[[VALUE_b:[0-9]+]] b: vla<array<i32, 3>, %[[VALUE1]]> [storage=automatic];
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_m]])));
// IR-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<vla<i32, %[[VALUE2]]>> [storage=automatic];
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_m]])));
// IR-NEXT:         let %[[VALUE_c:[0-9]+]] c: vla<vla<i64, %[[VALUE4]]>, %[[VALUE3]]> [storage=automatic];
// IR-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%[[VALUE_a]]), const<i32>(1))), const<i32>(2));
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%[[VALUE_a]]), const<i32>(1)))))), mul<u64, overflow=wrap>(read<u64>(%[[VALUE0]]), const<u64>(4))), mul<u64, overflow=wrap>(read<u64>(%[[VALUE0]]), const<u64>(4))), mul<u64, overflow=wrap>(read<u64>(%[[VALUE1]]), const<u64>(12))), mul<u64, overflow=wrap>(read<u64>(%[[VALUE3]]), mul<u64, overflow=wrap>(read<u64>(%[[VALUE4]]), const<u64>(8)))), mul<u64, overflow=wrap>(read<u64>(%[[VALUE2]]), const<u64>(4)))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
