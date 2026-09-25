void abort(void);

void bug(double *Cref, char transb, int m, int n, int k, double a, double *A,
         int fdA, double *B, int fdB, double b, double *C, int fdC) {
  if (C != Cref)
    abort();
}

int main(void) {
  double A[1], B[1], C[1];

  bug(C, 'B', 1, 2, 3, 4.0, A, 5, B, 6, 7.0, C, 8);

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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @bug(%2 Cref: ptr<f64>, %3 transb: i8, %4 m: i32, %5 n: i32, %6 k: i32, %7 a: f64, %8 A: ptr<f64>, %9 fdA: i32, %10 B: ptr<f64>, %11 fdB: i32, %12 b: f64, %13 C: ptr<f64>, %14 fdC: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<ptr<f64>>(read<ptr<f64>>(%13), read<ptr<f64>>(%2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %16 A: array<f64, 1> [storage=automatic];
// DEFAULT-NEXT:         let %17 B: array<f64, 1> [storage=automatic];
// DEFAULT-NEXT:         let %18 C: array<f64, 1> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<f64>, i8, i32, i32, i32, f64, ptr<f64>, i32, ptr<f64>, i32, f64, ptr<f64>, i32) -> void>(%1, array_decay<ptr<f64>, length=Some(1)>(%18), truncate<i8, reason=arg, fits=always>(const<i32>(66)), const<i32>(1), const<i32>(2), const<i32>(3), const<f64>(4.0), array_decay<ptr<f64>, length=Some(1)>(%16), const<i32>(5), array_decay<ptr<f64>, length=Some(1)>(%17), const<i32>(6), const<f64>(7.0), array_decay<ptr<f64>, length=Some(1)>(%18), const<i32>(8));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
