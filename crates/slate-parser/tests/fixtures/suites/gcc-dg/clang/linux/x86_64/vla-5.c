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
// DEFAULT-NEXT:     fn %[[VALUE_foo1:[0-9]+]] @foo1(%[[VALUE_o:[0-9]+]] o: ptr<fn(ptr<i32>) -> i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo2:[0-9]+]] @foo2(%[[VALUE_o_2:[0-9]+]] o: ptr<i32> [array=*]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo3:[0-9]+]] @foo3(%[[VALUE_o_3:[0-9]+]] o: ptr<vla<i32, *>> [array=4]) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo4:[0-9]+]] @foo4(%[[VALUE_j:[0-9]+]] j: i32, %[[VALUE_a:[0-9]+]] a: ptr<i32> [array=%[[VALUE0:[0-9]+]]]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_j]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo5:[0-9]+]] @foo5(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: ptr<array<i32, 10>> [array=10], %[[VALUE_c:[0-9]+]] c: ptr<i32> [array=400]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo6:[0-9]+]] @foo6(%[[VALUE_a_3:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: ptr<vla<i32, %[[VALUE1:[0-9]+]]>> [array=%[[VALUE2:[0-9]+]]], %[[VALUE_c_2:[0-9]+]] c: ptr<i32> [array=%[[VALUE3:[0-9]+]]]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE2]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_a_3]])));
// DEFAULT-NEXT:         let %[[VALUE1]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_a_3]])));
// DEFAULT-NEXT:         let %[[VALUE3]]: u64 [synthetic] = mul<u64, overflow=wrap>(read<u64>(%[[VALUE1]]), const<u64>(4));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=always>(const<u64>(8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo7:[0-9]+]] @foo7(%[[VALUE_i:[0-9]+]] i: ptr<fn(ptr<i32>) -> i32>) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
