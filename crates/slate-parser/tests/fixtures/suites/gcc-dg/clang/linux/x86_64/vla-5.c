/* { dg-options "-std=c99 -pedantic-errors" } */

void foo1(int (*o)(int p[*])) { }

void foo2(int o[*]);
void foo3(int o[4][*]);

void foo4(int j, int a[j]);
void foo4(int, int a[*]);
void foo4(int, int a[]);
void foo4(int j, int a[j]) {
}

int foo5(int a, int b[*][*], int c[static sizeof(*b)]);
int foo5(int a, int b[10][10], int c[400]) {
  return sizeof (c); /* { dg-warning "on array function parameter" } */
}

int foo6(int a, int b[*][*], int c[static sizeof(*b)]);
int foo6(int a, int b[a][a], int c[sizeof(*b)]) {
  return sizeof (c); /* { dg-warning "on array function parameter" } */
}

void foo7(__typeof__ (int (*)(int o[*])) i);

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     fn %0 @foo1(%1 o: ptr<fn(ptr<i32>) -> i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @foo2(%16 o: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo3(%17 o: ptr<vla<i32, *>> [array=4]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo4(%5 j: i32, %6 a: ptr<i32> [array=%24]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %24: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @foo5(%8 a: i32, %9 b: ptr<array<i32, 10>> [array=10], %10 c: ptr<i32> [array=400]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @foo6(%12 a: i32, %13 b: ptr<vla<i32, %32>> [array=%31], %14 c: ptr<i32> [array=%33]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %31: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%12)));
// DEFAULT-NEXT:         let %32: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%12)));
// DEFAULT-NEXT:         let %33: u64 [synthetic] = mul<u64, overflow=wrap>(read<u64>(%32), const<u64>(4));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @foo7(%34 i: ptr<fn(ptr<i32>) -> i32>) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
