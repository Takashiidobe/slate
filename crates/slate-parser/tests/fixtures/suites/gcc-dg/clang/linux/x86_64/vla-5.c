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
// DEFAULT-NEXT:     fn %0 @foo1(%2 o: ptr<fn(ptr<i32>) -> i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @foo2(%31 o: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @foo3(%32 o: ptr<vla<i32, *>> [array=4]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @foo4(%12 j: i32, %13 a: ptr<i32> [array=%39]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %39: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%12)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @foo5(%18 a: i32, %19 b: ptr<array<i32, 10>> [array=10], %20 c: ptr<i32> [array=400]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @foo6(%25 a: i32, %26 b: ptr<vla<i32, %47>> [array=%46], %27 c: ptr<i32> [array=%48]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %46: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%25)));
// DEFAULT-NEXT:         let %47: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%25)));
// DEFAULT-NEXT:         let %48: u64 [synthetic] = mul<u64, overflow=wrap>(read<u64>(%47), const<u64>(4));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @foo7(%49 i: ptr<fn(ptr<i32>) -> i32>) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
