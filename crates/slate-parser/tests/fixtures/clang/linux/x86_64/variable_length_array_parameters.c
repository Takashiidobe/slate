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
// IR-NEXT:     fn %[[VALUE_first:[0-9]+]] @first(%[[VALUE_n:[0-9]+]] n: i32, %[[VALUE_a:[0-9]+]] a: ptr<i32> [array=%[[VALUE0:[0-9]+]]]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_a]]), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_grid:[0-9]+]] @grid(%[[VALUE_n_2:[0-9]+]] n: i32, %[[VALUE_m:[0-9]+]] m: i32, %[[VALUE_a_2:[0-9]+]] a: ptr<vla<i32, %[[VALUE1:[0-9]+]]>> [array=%[[VALUE2:[0-9]+]]]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE2]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_2]])));
// IR-NEXT:         let %[[VALUE1]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_m]])));
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE1]]>>, subtract=false, element=vla<i32, %[[VALUE1]]>, overflow=ub>(read<ptr<vla<i32, %[[VALUE1]]>>>(%[[VALUE_a_2]]), const<i32>(1)))), const<i32>(2)))))), mul<u64, overflow=wrap>(read<u64>(%[[VALUE1]]), const<u64>(4)))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_row:[0-9]+]] @row(%[[VALUE_n_3:[0-9]+]] n: i32, %[[VALUE_a_3:[0-9]+]] a: ptr<vla<i32, %[[VALUE3:[0-9]+]]>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE3]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_3]])));
// IR-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(mul<u64, overflow=wrap>(read<u64>(%[[VALUE3]]), const<u64>(4))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_unnamed:[0-9]+]] @unnamed(%[[VALUE4:[0-9]+]] <unnamed>: i32, %[[VALUE_a_4:[0-9]+]] a: ptr<i32> [array=*]) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_later:[0-9]+]] @later(%[[VALUE_n_4:[0-9]+]] n: i32, %[[VALUE_a_5:[0-9]+]] a: ptr<vla<i32, *>> [array=*]) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_local:[0-9]+]] @local(%[[VALUE_n_5:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_5]])));
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_5]])));
// IR-NEXT:         let %[[VALUE_a_6:[0-9]+]] a: vla<vla<i32, %[[VALUE6]]>, %[[VALUE5]]> [storage=automatic];
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_5]])));
// IR-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<vla<i32, %[[VALUE7]]>> [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE7]]>>, reason=assign>(array_decay<ptr<vla<i32, %[[VALUE6]]>>, length=None>(%[[VALUE_a_6]]));
// IR-NEXT:         let %[[VALUE_g:[0-9]+]] g: ptr<fn(i32, i32, ptr<vla<i32, *>>) -> i32> [storage=automatic] = function_decay<ptr<fn(i32, i32, ptr<vla<i32, *>>) -> i32>>(%[[VALUE_grid]]);
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE6]]>>, subtract=false, element=vla<i32, %[[VALUE6]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE6]]>>, length=None>(%[[VALUE_a_6]]), const<i32>(1)))), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE7]]>>, subtract=false, element=vla<i32, %[[VALUE7]]>, overflow=ub>(read<ptr<vla<i32, %[[VALUE7]]>>>(%[[VALUE_p]]), const<i32>(1)))), const<i32>(2))))), truncate<i32, reason=explicit, fits=unknown>(ptr_diff<i64, element=vla<i32, %[[VALUE7]]>, same_array=required, overflow=ub>(ptr_offset<ptr<vla<i32, %[[VALUE7]]>>, subtract=false, element=vla<i32, %[[VALUE7]]>, overflow=ub>(read<ptr<vla<i32, %[[VALUE7]]>>>(%[[VALUE_p]]), const<i32>(1)), read<ptr<vla<i32, %[[VALUE7]]>>>(%[[VALUE_p]])))), call<i32, signature=fn(i32, i32, ptr<vla<i32, *>>) -> i32>(read<ptr<fn(i32, i32, ptr<vla<i32, *>>) -> i32>>(%[[VALUE_g]]), read<i32>(%[[VALUE_n_5]]), read<i32>(%[[VALUE_n_5]]), pointer_cast<ptr<vla<i32, *>>, reason=arg>(array_decay<ptr<vla<i32, %[[VALUE6]]>>, length=None>(%[[VALUE_a_6]]))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
