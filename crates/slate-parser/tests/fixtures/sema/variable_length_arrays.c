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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %0 @total(%1 n: i32, %2 m: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %7: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%1)));
// IR-NEXT:         let %3 a: vla<i32, %7> [storage=automatic];
// IR-NEXT:         let %8: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%1)));
// IR-NEXT:         let %4 b: vla<array<i32, 3>, %8> [storage=automatic];
// IR-NEXT:         let %9: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%2)));
// IR-NEXT:         let %5 p: ptr<vla<i32, %9>> [storage=automatic];
// IR-NEXT:         let %10: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%1)));
// IR-NEXT:         let %11: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%2)));
// IR-NEXT:         let %6 c: vla<vla<i64, %11>, %10> [storage=automatic];
// IR-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%3), const<i32>(1))), const<i32>(2));
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(%3), const<i32>(1)))))), mul<u64, overflow=wrap>(read<u64>(%7), const<u64>(4))), mul<u64, overflow=wrap>(read<u64>(%7), const<u64>(4))), mul<u64, overflow=wrap>(read<u64>(%8), const<u64>(12))), mul<u64, overflow=wrap>(read<u64>(%10), mul<u64, overflow=wrap>(read<u64>(%11), const<u64>(8)))), mul<u64, overflow=wrap>(read<u64>(%9), const<u64>(4)))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
