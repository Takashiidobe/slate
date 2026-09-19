// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

int next(void);

unsigned long size(int n) { return sizeof(int[n]); }

unsigned long once(int n) { return sizeof(int[n++][next()]); }

unsigned long branch(int c, int n) { return c ? sizeof(int[n]) : 0; }

unsigned long pointer(int n) { return sizeof(int (*)[n]) + _Alignof(int[n][2]); }

int cast(int n, void *p) {
  int (*q)[n] = (int (*)[n])p;
  (void)(int (*)[next()])p;
  return q[1][2] + ((int (*)[n])p)[1][2];
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
// IR-NEXT:     fn %0 @next() -> i32 [linkage=external];
// IR-NEXT:     fn %1 @size(%2 n: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %14: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%2)));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%14), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %3 @once(%4 n: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %24: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// IR-NEXT:         write<i32>(%4, read<i32>(%25));
// IR-NEXT:         let %15: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%24)));
// IR-NEXT:         let %16: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%15), mul<u64, overflow=wrap>(read<u64>(%16), const<u64>(4)));
// IR-NEXT:     }
// IR-NEXT:     fn %5 @branch(%6 c: i32, %7 n: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %26: u64 [synthetic];
// IR-NEXT:         if ne<i32>(read<i32>(%6), const<i32>(0))
// IR-NEXT:             let %17: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%7)));
// IR-NEXT:             write<u64>(%26, mul<u64, overflow=wrap>(read<u64>(%17), const<u64>(4)));
// IR-NEXT:         else
// IR-NEXT:             write<u64>(%26, reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))));
// IR-NEXT:         return read<u64>(%26);
// IR-NEXT:     }
// IR-NEXT:     fn %8 @pointer(%9 n: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<u64, overflow=wrap>(const<u64>(8), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %10 @cast(%11 n: i32, %12 p: ptr<void>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %20: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%11)));
// IR-NEXT:         let %13 q: ptr<vla<i32, %20>> [storage=automatic];
// IR-NEXT:         let %21: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%11)));
// IR-NEXT:         write<ptr<vla<i32, %20>>>(%13, pointer_cast<ptr<vla<i32, %20>>, reason=assign>(pointer_cast<ptr<vla<i32, %21>>, reason=explicit>(read<ptr<void>>(%12))));
// IR-NEXT:         let %22: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// IR-NEXT:         let %23: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%11)));
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %20>>, subtract=false, element=vla<i32, %20>, overflow=ub>(read<ptr<vla<i32, %20>>>(%13), const<i32>(1)))), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %23>>, subtract=false, element=vla<i32, %23>, overflow=ub>(pointer_cast<ptr<vla<i32, %23>>, reason=explicit>(read<ptr<void>>(%12)), const<i32>(1)))), const<i32>(2)))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
