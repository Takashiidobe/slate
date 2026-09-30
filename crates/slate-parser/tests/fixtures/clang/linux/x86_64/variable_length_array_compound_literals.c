// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

int next(void);

int pointer(int n, int (*p)[n]) {
  int (*q)[n] = (int (*)[n]){p};
  return q[1][2];
}

int once(int (*p)[next()]) { return (*(int (*)[next()]){p})[0]; }

int nested(int n, int m, int (*p)[n][m]) {
  return (*(int (*)[n][m]){p})[1][2];
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
// IR-NEXT:     fn %[[VALUE_next:[0-9]+]] @next() -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_pointer:[0-9]+]] @pointer(%[[VALUE_n:[0-9]+]] n: i32, %[[VALUE_p:[0-9]+]] p: ptr<vla<i32, %[[VALUE0:[0-9]+]]>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// IR-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<vla<i32, %[[VALUE1]]>> [storage=automatic];
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// IR-NEXT:         write<ptr<vla<i32, %[[VALUE1]]>>>(%[[VALUE_q]], pointer_cast<ptr<vla<i32, %[[VALUE1]]>>, reason=assign>(read<ptr<vla<i32, %[[VALUE2]]>>>(compound_literal %[[VALUE3:[0-9]+]] [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE2]]>>, reason=assign>(read<ptr<vla<i32, %[[VALUE0]]>>>(%[[VALUE_p]])))));
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE1]]>>, subtract=false, element=vla<i32, %[[VALUE1]]>, overflow=ub>(read<ptr<vla<i32, %[[VALUE1]]>>>(%[[VALUE_q]]), const<i32>(1)))), const<i32>(2))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_once:[0-9]+]] @once(%[[VALUE_p_2:[0-9]+]] p: ptr<vla<i32, %[[VALUE4:[0-9]+]]>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE4]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%[[VALUE_next]])));
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%[[VALUE_next]])));
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(read<ptr<vla<i32, %[[VALUE5]]>>>(compound_literal %[[VALUE6:[0-9]+]] [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE5]]>>, reason=assign>(read<ptr<vla<i32, %[[VALUE4]]>>>(%[[VALUE_p_2]]))))), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_nested:[0-9]+]] @nested(%[[VALUE_n_2:[0-9]+]] n: i32, %[[VALUE_m:[0-9]+]] m: i32, %[[VALUE_p_3:[0-9]+]] p: ptr<vla<vla<i32, %[[VALUE7:[0-9]+]]>, %[[VALUE8:[0-9]+]]>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE8]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_2]])));
// IR-NEXT:         let %[[VALUE7]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_m]])));
// IR-NEXT:         let %[[VALUE9:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_2]])));
// IR-NEXT:         let %[[VALUE10:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_m]])));
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE10]]>>, subtract=false, element=vla<i32, %[[VALUE10]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE10]]>>, length=None>(deref(read<ptr<vla<vla<i32, %[[VALUE10]]>, %[[VALUE9]]>>>(compound_literal %[[VALUE11:[0-9]+]] [storage=automatic] = pointer_cast<ptr<vla<vla<i32, %[[VALUE10]]>, %[[VALUE9]]>>, reason=assign>(read<ptr<vla<vla<i32, %[[VALUE7]]>, %[[VALUE8]]>>>(%[[VALUE_p_3]]))))), const<i32>(1)))), const<i32>(2))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
