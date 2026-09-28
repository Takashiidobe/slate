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
// IR-NEXT:     fn %0 @next() -> i32 [linkage=external];
// IR-NEXT:     fn %1 @pointer(%2 n: i32, %3 p: ptr<vla<i32, %11>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %11: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%2)));
// IR-NEXT:         let %12: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%2)));
// IR-NEXT:         let %4 q: ptr<vla<i32, %12>> [storage=automatic];
// IR-NEXT:         let %13: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%2)));
// IR-NEXT:         write<ptr<vla<i32, %12>>>(%4, pointer_cast<ptr<vla<i32, %12>>, reason=assign>(read<ptr<vla<i32, %13>>>(compound_literal %14 [storage=automatic] = pointer_cast<ptr<vla<i32, %13>>, reason=assign>(read<ptr<vla<i32, %11>>>(%3)))));
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %12>>, subtract=false, element=vla<i32, %12>, overflow=ub>(read<ptr<vla<i32, %12>>>(%4), const<i32>(1)))), const<i32>(2))));
// IR-NEXT:     }
// IR-NEXT:     fn %5 @once(%6 p: ptr<vla<i32, %15>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %15: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// IR-NEXT:         let %16: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(read<ptr<vla<i32, %16>>>(compound_literal %17 [storage=automatic] = pointer_cast<ptr<vla<i32, %16>>, reason=assign>(read<ptr<vla<i32, %15>>>(%6))))), const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %7 @nested(%8 n: i32, %9 m: i32, %10 p: ptr<vla<vla<i32, %19>, %18>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %18: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%8)));
// IR-NEXT:         let %19: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9)));
// IR-NEXT:         let %20: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%8)));
// IR-NEXT:         let %21: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9)));
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %21>>, subtract=false, element=vla<i32, %21>, overflow=ub>(array_decay<ptr<vla<i32, %21>>, length=None>(deref(read<ptr<vla<vla<i32, %21>, %20>>>(compound_literal %22 [storage=automatic] = pointer_cast<ptr<vla<vla<i32, %21>, %20>>, reason=assign>(read<ptr<vla<vla<i32, %19>, %18>>>(%10))))), const<i32>(1)))), const<i32>(2))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
