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
// IR-NEXT:     fn %[[VALUE_next:[0-9]+]] @next() -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_cast:[0-9]+]] @cast(%[[VALUE_n:[0-9]+]] n: i32, %[[VALUE_p:[0-9]+]] p: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n]])));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%[[VALUE0]]), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_once:[0-9]+]] @once(%[[VALUE_p_2:[0-9]+]] p: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn() -> i32>(%[[VALUE_next]])));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%[[VALUE1]]), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_literal:[0-9]+]] @literal(%[[VALUE_n_2:[0-9]+]] n: i32, %[[VALUE_p_3:[0-9]+]] p: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_2]])));
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_2]])));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%[[VALUE2]]), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_declared:[0-9]+]] @declared(%[[VALUE_n_3:[0-9]+]] n: i32, %[[VALUE_q:[0-9]+]] q: ptr<vla<i32, %[[VALUE4:[0-9]+]]>>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE4]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_n_3]])));
// IR-NEXT:         return mul<u64, overflow=wrap>(read<u64>(%[[VALUE4]]), const<u64>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_pointer:[0-9]+]] @pointer(%[[VALUE_p_4:[0-9]+]] p: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(8);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_alignment:[0-9]+]] @alignment(%[[VALUE_n_4:[0-9]+]] n: i32, %[[VALUE_p_5:[0-9]+]] p: ptr<i32>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(4);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
