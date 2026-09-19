// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

int first(int n, int a[n]) { return a[0]; }

int grid(int n, int m, int a[n][m]) { return a[1][2] + sizeof a[0]; }

int row(int n, int (*a)[n]) { return sizeof *a; }

int unnamed(int, int a[*]);

int later(int n, int a[*][n]);

int local(int n) {
  int a[n][n];
  int (*p)[n] = a;
  int (*g)(int, int, int[*][*]) = grid;
  return a[1][2] + p[1][2] + (int)(p + 1 - p) + g(n, n, a);
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
// IR-NEXT:     fn %0 @first(%1 n: i32, %2 a: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %17: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%1)));
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%2), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %3 @grid(%4 n: i32, %5 m: i32, %6 a: ptr<vla<i32, %19>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %18: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%4)));
// IR-NEXT:         let %19: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%5)));
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %19>>, subtract=false, element=vla<i32, %19>, overflow=ub>(read<ptr<vla<i32, %19>>>(%6), const<i32>(1)))), const<i32>(2)))))), mul<u64, overflow=wrap>(read<u64>(%19), const<u64>(4)))));
// IR-NEXT:     }
// IR-NEXT:     fn %7 @row(%8 n: i32, %9 a: ptr<vla<i32, %20>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %20: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%8)));
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(mul<u64, overflow=wrap>(read<u64>(%20), const<u64>(4))));
// IR-NEXT:     }
// IR-NEXT:     fn %10 @unnamed(%21 <unnamed>: i32, %22 a: ptr<i32>) -> i32 [linkage=external];
// IR-NEXT:     fn %11 @later(%23 n: i32, %24 a: ptr<vla<i32, *>>) -> i32 [linkage=external];
// IR-NEXT:     fn %12 @local(%13 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %25: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%13)));
// IR-NEXT:         let %26: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%13)));
// IR-NEXT:         let %14 a: vla<vla<i32, %26>, %25> [storage=automatic];
// IR-NEXT:         let %27: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%13)));
// IR-NEXT:         let %15 p: ptr<vla<i32, %27>> [storage=automatic] = pointer_cast<ptr<vla<i32, %27>>, reason=assign>(array_decay<ptr<vla<i32, %26>>, length=None>(%14));
// IR-NEXT:         let %16 g: ptr<fn(i32, i32, ptr<vla<i32, *>>) -> i32> [storage=automatic] = function_decay<ptr<fn(i32, i32, ptr<vla<i32, *>>) -> i32>>(%3);
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %26>>, subtract=false, element=vla<i32, %26>, overflow=ub>(array_decay<ptr<vla<i32, %26>>, length=None>(%14), const<i32>(1)))), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %27>>, subtract=false, element=vla<i32, %27>, overflow=ub>(read<ptr<vla<i32, %27>>>(%15), const<i32>(1)))), const<i32>(2))))), truncate<i32, reason=explicit, fits=unknown>(ptr_diff<i64, element=vla<i32, %27>, same_array=required, overflow=ub>(ptr_offset<ptr<vla<i32, %27>>, subtract=false, element=vla<i32, %27>, overflow=ub>(read<ptr<vla<i32, %27>>>(%15), const<i32>(1)), read<ptr<vla<i32, %27>>>(%15)))), call<i32, signature=fn(i32, i32, ptr<vla<i32, *>>) -> i32>(read<ptr<fn(i32, i32, ptr<vla<i32, *>>) -> i32>>(%16), read<i32>(%13), read<i32>(%13), pointer_cast<ptr<vla<i32, *>>, reason=arg>(array_decay<ptr<vla<i32, %26>>, length=None>(%14))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
