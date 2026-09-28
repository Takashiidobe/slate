// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

int next(void);

unsigned long cast(int n, int *p) { return sizeof(*(int (*)[n])p); }

unsigned long once(int *p) { return sizeof(*(int (*)[next()])p); }

unsigned long literal(int n, int *p) {
  return sizeof(*(int (*)[n]){(int (*)[n])p});
}

unsigned long declared(int n, int (*q)[n]) { return sizeof(*q); }

unsigned long pointer(int *p) { return sizeof((int (*)[next()])p); }

unsigned long alignment(int n, int *p) { return _Alignof(*(int (*)[n])p); }

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
// IR-NEXT:     fn %1 @cast(%2 n: i32, %3 p: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %17: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%2)));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%17), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %4 @once(%5 p: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %18: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%0)));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%18), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %6 @literal(%7 n: i32, %8 p: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %19: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%7)));
// IR-NEXT:         let %20: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%7)));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%19), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %9 @declared(%10 n: i32, %11 q: ptr<vla<i32, %22>>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %22: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%10)));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%22), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %12 @pointer(%13 p: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(8);
// IR-NEXT:     }
// IR-NEXT:     fn %14 @alignment(%15 n: i32, %16 p: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(4);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
