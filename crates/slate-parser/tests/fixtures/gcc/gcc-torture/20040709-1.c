/* { dg-require-effective-target int32plus } */

/* Test arithmetics on bitfields.  */

extern void abort(void);
extern void exit(int);

unsigned int myrnd(void) {
  static unsigned int s  = 1388815473;
  s                     *= 1103515245;
  s                     += 12345;
  return (s / 65536) % 2048;
}

#define T(S)                                                                   \
  struct S s##S;                                                               \
  struct S retme##S(struct S x) {                                              \
    return x;                                                                  \
  }                                                                            \
                                                                               \
  unsigned int                                                                 \
  fn1##S(unsigned int x) {                                                     \
    struct S y  = s##S;                                                        \
    y.k        += x;                                                           \
    y           = retme##S(y);                                                 \
    return y.k;                                                                \
  }                                                                            \
                                                                               \
  unsigned int fn2##S(unsigned int x) {                                        \
    struct S y  = s##S;                                                        \
    y.k        += x;                                                           \
    y.k        %= 15;                                                          \
    return y.k;                                                                \
  }                                                                            \
                                                                               \
  unsigned int retit##S(void) { return s##S.k; }                               \
                                                                               \
  unsigned int fn3##S(unsigned int x) {                                        \
    s##S.k += x;                                                               \
    return retit##S();                                                         \
  }                                                                            \
                                                                               \
  void test##S(void) {                                                         \
    int          i;                                                            \
    unsigned int mask, v, a, r;                                                \
    struct S     x;                                                            \
    char        *p = (char *)&s##S;                                            \
    for (i = 0; i < sizeof(s##S); ++i)                                         \
      *p++ = myrnd();                                                          \
    if (__builtin_classify_type(s##S.l) == 8)                                  \
      s##S.l = 5.25;                                                           \
    s##S.k = -1;                                                               \
    mask   = s##S.k;                                                           \
    v      = myrnd();                                                          \
    a      = myrnd();                                                          \
    s##S.k = v;                                                                \
    x      = s##S;                                                             \
    r      = fn1##S(a);                                                        \
    if (x.i != s##S.i || x.j != s##S.j || x.k != s##S.k || x.l != s##S.l ||    \
        ((v + a) & mask) != r)                                                 \
      abort();                                                                 \
    v      = myrnd();                                                          \
    a      = myrnd();                                                          \
    s##S.k = v;                                                                \
    x      = s##S;                                                             \
    r      = fn2##S(a);                                                        \
    if (x.i != s##S.i || x.j != s##S.j || x.k != s##S.k || x.l != s##S.l ||    \
        ((((v + a) & mask) % 15) & mask) != r)                                 \
      abort();                                                                 \
    v      = myrnd();                                                          \
    a      = myrnd();                                                          \
    s##S.k = v;                                                                \
    x      = s##S;                                                             \
    r      = fn3##S(a);                                                        \
    if (x.i != s##S.i || x.j != s##S.j || s##S.k != r || x.l != s##S.l ||      \
        ((v + a) & mask) != r)                                                 \
      abort();                                                                 \
  }

struct A {
  unsigned int i : 6, l : 1, j : 10, k : 15;
};
T(A)
struct B {
  unsigned int i : 6, j : 11, k : 15;
  unsigned int l;
};
T(B)
struct C {
  unsigned int l;
  unsigned int i : 6, j : 11, k : 15;
};
T(C)
struct D {
  unsigned long long l : 6, i : 6, j : 23, k : 29;
};
T(D)
struct E {
  unsigned long long l, i : 12, j : 23, k : 29;
};
T(E)
struct F {
  unsigned long long i : 12, j : 23, k : 29, l;
};
T(F)
struct G {
  unsigned int       i : 12, j : 13, k : 7;
  unsigned long long l;
};
T(G)
struct H {
  unsigned int       i : 12, j : 11, k : 9;
  unsigned long long l;
};
T(H)
struct I {
  unsigned short     i : 1, j : 6, k : 9;
  unsigned long long l;
};
T(I)
struct J {
  unsigned short i : 1, j : 8, k : 7;
  unsigned short l;
};
T(J)
struct K {
  unsigned int k : 6, l : 1, j : 10, i : 15;
};
T(K)
struct L {
  unsigned int k : 6, j : 11, i : 15;
  unsigned int l;
};
T(L)
struct M {
  unsigned int l;
  unsigned int k : 6, j : 11, i : 15;
};
T(M)
struct N {
  unsigned long long l : 6, k : 6, j : 23, i : 29;
};
T(N)
struct O {
  unsigned long long l, k : 12, j : 23, i : 29;
};
T(O)
struct P {
  unsigned long long k : 12, j : 23, i : 29, l;
};
T(P)
struct Q {
  unsigned int       k : 12, j : 13, i : 7;
  unsigned long long l;
};
T(Q)
struct R {
  unsigned int       k : 12, j : 11, i : 9;
  unsigned long long l;
};
T(R)
struct S {
  unsigned short     k : 1, j : 6, i : 9;
  unsigned long long l;
};
T(S)
struct T {
  unsigned short k : 1, j : 8, i : 7;
  unsigned short l;
};
T(T)
struct U {
  unsigned short     j : 6, k : 1, i : 9;
  unsigned long long l;
};
T(U)
struct V {
  unsigned short j : 8, k : 1, i : 7;
  unsigned short l;
};
T(V)
struct W {
  long double  l;
  unsigned int k : 12, j : 13, i : 7;
};
T(W)
struct X {
  unsigned int k : 12, j : 13, i : 7;
  long double  l;
};
T(X)
struct Y {
  unsigned int k : 12, j : 11, i : 9;
  long double  l;
};
T(Y)
struct Z {
  long double  l;
  unsigned int j : 13, i : 7, k : 12;
};
T(Z)

int main(void) {
  testA();
  testB();
  testC();
  testD();
  testE();
  testF();
  testG();
  testH();
  testI();
  testJ();
  testK();
  testL();
  testM();
  testN();
  testO();
  testP();
  testQ();
  testR();
  testS();
  testT();
  testU();
  testV();
  testW();
  testX();
  testY();
  testZ();
  exit(0);
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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 i: u32 : 6;
// DEFAULT-NEXT:         field1 l: u32 : 1;
// DEFAULT-NEXT:         field2 j: u32 : 10;
// DEFAULT-NEXT:         field3 k: u32 : 15;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0, 2], bit_offsets=[Some(0), Some(6), Some(7), Some(17)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type1 B = struct {
// DEFAULT-NEXT:         field0 i: u32 : 6;
// DEFAULT-NEXT:         field1 j: u32 : 11;
// DEFAULT-NEXT:         field2 k: u32 : 15;
// DEFAULT-NEXT:         field3 l: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0, 2, 4], bit_offsets=[Some(0), Some(6), Some(17), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type2 C = struct {
// DEFAULT-NEXT:         field0 l: u32;
// DEFAULT-NEXT:         field1 i: u32 : 6;
// DEFAULT-NEXT:         field2 j: u32 : 11;
// DEFAULT-NEXT:         field3 k: u32 : 15;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4, 4, 6], bit_offsets=[None, Some(32), Some(38), Some(49)], bit_units=[(4, 4)], field_units=[None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type3 D = struct {
// DEFAULT-NEXT:         field0 l: u64 : 6;
// DEFAULT-NEXT:         field1 i: u64 : 6;
// DEFAULT-NEXT:         field2 j: u64 : 23;
// DEFAULT-NEXT:         field3 k: u64 : 29;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 1, 4], bit_offsets=[Some(0), Some(6), Some(12), Some(35)], bit_units=[(0, 8)], field_units=[Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type4 E = struct {
// DEFAULT-NEXT:         field0 l: u64;
// DEFAULT-NEXT:         field1 i: u64 : 12;
// DEFAULT-NEXT:         field2 j: u64 : 23;
// DEFAULT-NEXT:         field3 k: u64 : 29;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 9, 12], bit_offsets=[None, Some(64), Some(76), Some(99)], bit_units=[(8, 8)], field_units=[None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type5 F = struct {
// DEFAULT-NEXT:         field0 i: u64 : 12;
// DEFAULT-NEXT:         field1 j: u64 : 23;
// DEFAULT-NEXT:         field2 k: u64 : 29;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 1, 4, 8], bit_offsets=[Some(0), Some(12), Some(35), None], bit_units=[(0, 8)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type6 G = struct {
// DEFAULT-NEXT:         field0 i: u32 : 12;
// DEFAULT-NEXT:         field1 j: u32 : 13;
// DEFAULT-NEXT:         field2 k: u32 : 7;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 1, 3, 8], bit_offsets=[Some(0), Some(12), Some(25), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type7 H = struct {
// DEFAULT-NEXT:         field0 i: u32 : 12;
// DEFAULT-NEXT:         field1 j: u32 : 11;
// DEFAULT-NEXT:         field2 k: u32 : 9;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 1, 2, 8], bit_offsets=[Some(0), Some(12), Some(23), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type8 I = struct {
// DEFAULT-NEXT:         field0 i: u16 : 1;
// DEFAULT-NEXT:         field1 j: u16 : 6;
// DEFAULT-NEXT:         field2 k: u16 : 9;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0, 0, 8], bit_offsets=[Some(0), Some(1), Some(7), None], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type9 J = struct {
// DEFAULT-NEXT:         field0 i: u16 : 1;
// DEFAULT-NEXT:         field1 j: u16 : 8;
// DEFAULT-NEXT:         field2 k: u16 : 7;
// DEFAULT-NEXT:         field3 l: u16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 0, 1, 2], bit_offsets=[Some(0), Some(1), Some(9), None], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type10 K = struct {
// DEFAULT-NEXT:         field0 k: u32 : 6;
// DEFAULT-NEXT:         field1 l: u32 : 1;
// DEFAULT-NEXT:         field2 j: u32 : 10;
// DEFAULT-NEXT:         field3 i: u32 : 15;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0, 2], bit_offsets=[Some(0), Some(6), Some(7), Some(17)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type11 L = struct {
// DEFAULT-NEXT:         field0 k: u32 : 6;
// DEFAULT-NEXT:         field1 j: u32 : 11;
// DEFAULT-NEXT:         field2 i: u32 : 15;
// DEFAULT-NEXT:         field3 l: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0, 2, 4], bit_offsets=[Some(0), Some(6), Some(17), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type12 M = struct {
// DEFAULT-NEXT:         field0 l: u32;
// DEFAULT-NEXT:         field1 k: u32 : 6;
// DEFAULT-NEXT:         field2 j: u32 : 11;
// DEFAULT-NEXT:         field3 i: u32 : 15;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4, 4, 6], bit_offsets=[None, Some(32), Some(38), Some(49)], bit_units=[(4, 4)], field_units=[None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type13 N = struct {
// DEFAULT-NEXT:         field0 l: u64 : 6;
// DEFAULT-NEXT:         field1 k: u64 : 6;
// DEFAULT-NEXT:         field2 j: u64 : 23;
// DEFAULT-NEXT:         field3 i: u64 : 29;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 1, 4], bit_offsets=[Some(0), Some(6), Some(12), Some(35)], bit_units=[(0, 8)], field_units=[Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type14 O = struct {
// DEFAULT-NEXT:         field0 l: u64;
// DEFAULT-NEXT:         field1 k: u64 : 12;
// DEFAULT-NEXT:         field2 j: u64 : 23;
// DEFAULT-NEXT:         field3 i: u64 : 29;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 9, 12], bit_offsets=[None, Some(64), Some(76), Some(99)], bit_units=[(8, 8)], field_units=[None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type15 P = struct {
// DEFAULT-NEXT:         field0 k: u64 : 12;
// DEFAULT-NEXT:         field1 j: u64 : 23;
// DEFAULT-NEXT:         field2 i: u64 : 29;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 1, 4, 8], bit_offsets=[Some(0), Some(12), Some(35), None], bit_units=[(0, 8)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type16 Q = struct {
// DEFAULT-NEXT:         field0 k: u32 : 12;
// DEFAULT-NEXT:         field1 j: u32 : 13;
// DEFAULT-NEXT:         field2 i: u32 : 7;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 1, 3, 8], bit_offsets=[Some(0), Some(12), Some(25), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type17 R = struct {
// DEFAULT-NEXT:         field0 k: u32 : 12;
// DEFAULT-NEXT:         field1 j: u32 : 11;
// DEFAULT-NEXT:         field2 i: u32 : 9;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 1, 2, 8], bit_offsets=[Some(0), Some(12), Some(23), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type18 S = struct {
// DEFAULT-NEXT:         field0 k: u16 : 1;
// DEFAULT-NEXT:         field1 j: u16 : 6;
// DEFAULT-NEXT:         field2 i: u16 : 9;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0, 0, 8], bit_offsets=[Some(0), Some(1), Some(7), None], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type19 T = struct {
// DEFAULT-NEXT:         field0 k: u16 : 1;
// DEFAULT-NEXT:         field1 j: u16 : 8;
// DEFAULT-NEXT:         field2 i: u16 : 7;
// DEFAULT-NEXT:         field3 l: u16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 0, 1, 2], bit_offsets=[Some(0), Some(1), Some(9), None], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type20 U = struct {
// DEFAULT-NEXT:         field0 j: u16 : 6;
// DEFAULT-NEXT:         field1 k: u16 : 1;
// DEFAULT-NEXT:         field2 i: u16 : 9;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0, 0, 8], bit_offsets=[Some(0), Some(6), Some(7), None], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type21 V = struct {
// DEFAULT-NEXT:         field0 j: u16 : 8;
// DEFAULT-NEXT:         field1 k: u16 : 1;
// DEFAULT-NEXT:         field2 i: u16 : 7;
// DEFAULT-NEXT:         field3 l: u16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 1, 1, 2], bit_offsets=[Some(0), Some(8), Some(9), None], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type22 W = struct {
// DEFAULT-NEXT:         field0 l: f80;
// DEFAULT-NEXT:         field1 k: u32 : 12;
// DEFAULT-NEXT:         field2 j: u32 : 13;
// DEFAULT-NEXT:         field3 i: u32 : 7;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16, 17, 19], bit_offsets=[None, Some(128), Some(140), Some(153)], bit_units=[(16, 4)], field_units=[None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type23 X = struct {
// DEFAULT-NEXT:         field0 k: u32 : 12;
// DEFAULT-NEXT:         field1 j: u32 : 13;
// DEFAULT-NEXT:         field2 i: u32 : 7;
// DEFAULT-NEXT:         field3 l: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 1, 3, 16], bit_offsets=[Some(0), Some(12), Some(25), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type24 Y = struct {
// DEFAULT-NEXT:         field0 k: u32 : 12;
// DEFAULT-NEXT:         field1 j: u32 : 11;
// DEFAULT-NEXT:         field2 i: u32 : 9;
// DEFAULT-NEXT:         field3 l: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 1, 2, 16], bit_offsets=[Some(0), Some(12), Some(23), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type25 Z = struct {
// DEFAULT-NEXT:         field0 l: f80;
// DEFAULT-NEXT:         field1 j: u32 : 13;
// DEFAULT-NEXT:         field2 i: u32 : 7;
// DEFAULT-NEXT:         field3 k: u32 : 12;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16, 17, 18], bit_offsets=[None, Some(128), Some(141), Some(148)], bit_units=[(16, 4)], field_units=[None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %3 s: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(1388815473)) [linkage=internal];
// DEFAULT-NEXT:     global %5 sA: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %26 sB: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %47 sC: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %68 sD: @type3 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %89 sE: @type4 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %110 sF: @type5 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %131 sG: @type6 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %152 sH: @type7 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %173 sI: @type8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %194 sJ: @type9 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %215 sK: @type10 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %236 sL: @type11 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %257 sM: @type12 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %278 sN: @type13 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %299 sO: @type14 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %320 sP: @type15 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %341 sQ: @type16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %362 sR: @type17 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %383 sS: @type18 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %404 sT: @type19 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %425 sU: @type20 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %446 sV: @type21 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %467 sW: @type22 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %488 sX: @type23 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %509 sY: @type24 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %530 sZ: @type25 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%551 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @myrnd() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %578: u32 [synthetic] = read<u32>(%3);
// DEFAULT-NEXT:         let %579: u32 [synthetic] = mul<u32, overflow=wrap>(read<u32>(%578), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1103515245)));
// DEFAULT-NEXT:         write<u32>(%3, read<u32>(%579));
// DEFAULT-NEXT:         let %580: u32 [synthetic] = read<u32>(%3);
// DEFAULT-NEXT:         let %581: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%580), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(12345)));
// DEFAULT-NEXT:         write<u32>(%3, read<u32>(%581));
// DEFAULT-NEXT:         return rem<u32, by_zero=ub>(div<u32, by_zero=ub>(read<u32>(%3), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(65536))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2048)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @retmeA(%7 x: @type0) -> @type0 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @fn1A(%9 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 y: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(%5));
// DEFAULT-NEXT:         let %582: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%10));
// DEFAULT-NEXT:         let %583: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%582))), read<u32>(%9));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%10), read<u32>(%583));
// DEFAULT-NEXT:         write<@type0>(%10, copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(native_c) -> native_c>(%6, copy<@type0, reason=arg>(read<@type0>(%10)))));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn(@type0) -> @type0, abi=sysv64(native_c) -> native_c>(%6, copy<@type0, reason=arg>(read<@type0>(%10))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%10))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @fn2A(%12 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 y: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(%5));
// DEFAULT-NEXT:         let %584: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%13));
// DEFAULT-NEXT:         let %585: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%584))), read<u32>(%12));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%13), read<u32>(%585));
// DEFAULT-NEXT:         let %586: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%13));
// DEFAULT-NEXT:         let %587: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%586)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%13), read<u32>(%587));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%13))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @retitA() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @fn3A(%16 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %588: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%5));
// DEFAULT-NEXT:         let %589: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%588))), read<u32>(%16));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%5), read<u32>(%589));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @testA() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %18 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %19 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %20 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %21 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %22 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %23 x: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %24 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type0>>(%5));
// DEFAULT-NEXT:         for %552
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%18, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%18))), const<u64>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %590: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:                 let %591: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%590), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%18, read<i32>(%591));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %592: ptr<i8> [synthetic] = read<ptr<i8>>(%24);
// DEFAULT-NEXT:                 let %593: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%592), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%24, read<ptr<i8>>(%593));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%592)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%5), float_to_int<u32, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%5), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%19, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%5)))));
// DEFAULT-NEXT:         write<u32>(%20, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%21, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%5), read<u32>(%20));
// DEFAULT-NEXT:         write<@type0>(%23, copy<@type0, reason=assign>(read<@type0>(%5)));
// DEFAULT-NEXT:         write<u32>(%22, call<u32, signature=fn(u32) -> u32>(%8, read<u32>(%21)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%8, read<u32>(%21));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%23))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%5)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%23))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%5))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%23))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%5))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%23))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%5))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%20), read<u32>(%21)), read<u32>(%19)), read<u32>(%22)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%20, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%21, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%5), read<u32>(%20));
// DEFAULT-NEXT:         write<@type0>(%23, copy<@type0, reason=assign>(read<@type0>(%5)));
// DEFAULT-NEXT:         write<u32>(%22, call<u32, signature=fn(u32) -> u32>(%11, read<u32>(%21)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%11, read<u32>(%21));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%23))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%5)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%23))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%5))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%23))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%5))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%23))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%5))))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%20), read<u32>(%21)), read<u32>(%19)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%19)), read<u32>(%22)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%20, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%21, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%5), read<u32>(%20));
// DEFAULT-NEXT:         write<@type0>(%23, copy<@type0, reason=assign>(read<@type0>(%5)));
// DEFAULT-NEXT:         write<u32>(%22, call<u32, signature=fn(u32) -> u32>(%15, read<u32>(%21)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%15, read<u32>(%21));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%23))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%5)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%23))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%5))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%5)))), read<u32>(%22))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%23))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%5))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%20), read<u32>(%21)), read<u32>(%19)), read<u32>(%22)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @retmeB(%28 x: @type1) -> @type1 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @fn1B(%30 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %31 y: @type1 [storage=automatic] = copy<@type1, reason=assign>(read<@type1>(%26));
// DEFAULT-NEXT:         let %594: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%31));
// DEFAULT-NEXT:         let %595: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%594))), read<u32>(%30));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%31), read<u32>(%595));
// DEFAULT-NEXT:         write<@type1>(%31, copy<@type1, reason=assign>(call<@type1, signature=fn(@type1) -> @type1, abi=sysv64(native_c) -> native_c>(%27, copy<@type1, reason=arg>(read<@type1>(%31)))));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(call<@type1, signature=fn(@type1) -> @type1, abi=sysv64(native_c) -> native_c>(%27, copy<@type1, reason=arg>(read<@type1>(%31))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%31))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @fn2B(%33 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %34 y: @type1 [storage=automatic] = copy<@type1, reason=assign>(read<@type1>(%26));
// DEFAULT-NEXT:         let %596: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%34));
// DEFAULT-NEXT:         let %597: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%596))), read<u32>(%33));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%34), read<u32>(%597));
// DEFAULT-NEXT:         let %598: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%34));
// DEFAULT-NEXT:         let %599: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%598)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%34), read<u32>(%599));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%34))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @retitB() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%26))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @fn3B(%37 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %600: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%26));
// DEFAULT-NEXT:         let %601: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%600))), read<u32>(%37));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%26), read<u32>(%601));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%35);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @testB() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %39 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %40 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %41 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %42 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %43 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %44 x: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %45 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type1>>(%26));
// DEFAULT-NEXT:         for %553
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%39, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%39))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %602: i32 [synthetic] = read<i32>(%39);
// DEFAULT-NEXT:                 let %603: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%602), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%39, read<i32>(%603));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %604: ptr<i8> [synthetic] = read<ptr<i8>>(%45);
// DEFAULT-NEXT:                 let %605: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%604), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%45, read<ptr<i8>>(%605));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%604)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u32>(field3(%26), float_to_int<u32, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%26), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%40, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%26)))));
// DEFAULT-NEXT:         write<u32>(%41, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%42, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%26), read<u32>(%41));
// DEFAULT-NEXT:         write<@type1>(%44, copy<@type1, reason=assign>(read<@type1>(%26)));
// DEFAULT-NEXT:         write<u32>(%43, call<u32, signature=fn(u32) -> u32>(%29, read<u32>(%42)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%29, read<u32>(%42));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%44))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%26)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%44))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%26))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%44))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%26))))), ne<u32>(read<u32>(field3(%44)), read<u32>(field3(%26)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%41), read<u32>(%42)), read<u32>(%40)), read<u32>(%43)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%41, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%42, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%26), read<u32>(%41));
// DEFAULT-NEXT:         write<@type1>(%44, copy<@type1, reason=assign>(read<@type1>(%26)));
// DEFAULT-NEXT:         write<u32>(%43, call<u32, signature=fn(u32) -> u32>(%32, read<u32>(%42)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%32, read<u32>(%42));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%44))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%26)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%44))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%26))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%44))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%26))))), ne<u32>(read<u32>(field3(%44)), read<u32>(field3(%26)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%41), read<u32>(%42)), read<u32>(%40)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%40)), read<u32>(%43)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%41, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%42, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%26), read<u32>(%41));
// DEFAULT-NEXT:         write<@type1>(%44, copy<@type1, reason=assign>(read<@type1>(%26)));
// DEFAULT-NEXT:         write<u32>(%43, call<u32, signature=fn(u32) -> u32>(%36, read<u32>(%42)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%36, read<u32>(%42));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%44))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%26)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%44))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%26))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%26)))), read<u32>(%43))), ne<u32>(read<u32>(field3(%44)), read<u32>(field3(%26)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%41), read<u32>(%42)), read<u32>(%40)), read<u32>(%43)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @retmeC(%49 x: @type2) -> @type2 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type2, reason=return>(read<@type2>(%49));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @fn1C(%51 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %52 y: @type2 [storage=automatic] = copy<@type2, reason=assign>(read<@type2>(%47));
// DEFAULT-NEXT:         let %606: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%52));
// DEFAULT-NEXT:         let %607: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%606))), read<u32>(%51));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%52), read<u32>(%607));
// DEFAULT-NEXT:         write<@type2>(%52, copy<@type2, reason=assign>(call<@type2, signature=fn(@type2) -> @type2, abi=sysv64(native_c) -> native_c>(%48, copy<@type2, reason=arg>(read<@type2>(%52)))));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(call<@type2, signature=fn(@type2) -> @type2, abi=sysv64(native_c) -> native_c>(%48, copy<@type2, reason=arg>(read<@type2>(%52))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%52))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %53 @fn2C(%54 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %55 y: @type2 [storage=automatic] = copy<@type2, reason=assign>(read<@type2>(%47));
// DEFAULT-NEXT:         let %608: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%55));
// DEFAULT-NEXT:         let %609: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%608))), read<u32>(%54));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%55), read<u32>(%609));
// DEFAULT-NEXT:         let %610: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%55));
// DEFAULT-NEXT:         let %611: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%610)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%55), read<u32>(%611));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%55))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @retitC() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%47))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @fn3C(%58 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %612: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%47));
// DEFAULT-NEXT:         let %613: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%612))), read<u32>(%58));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%47), read<u32>(%613));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%56);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %59 @testC() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %60 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %61 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %62 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %63 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %64 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %65 x: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %66 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type2>>(%47));
// DEFAULT-NEXT:         for %554
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%60, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%60))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %614: i32 [synthetic] = read<i32>(%60);
// DEFAULT-NEXT:                 let %615: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%614), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%60, read<i32>(%615));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %616: ptr<i8> [synthetic] = read<ptr<i8>>(%66);
// DEFAULT-NEXT:                 let %617: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%616), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%66, read<ptr<i8>>(%617));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%616)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u32>(field0(%47), float_to_int<u32, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%47), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%61, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%47)))));
// DEFAULT-NEXT:         write<u32>(%62, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%63, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%47), read<u32>(%62));
// DEFAULT-NEXT:         write<@type2>(%65, copy<@type2, reason=assign>(read<@type2>(%47)));
// DEFAULT-NEXT:         write<u32>(%64, call<u32, signature=fn(u32) -> u32>(%50, read<u32>(%63)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%50, read<u32>(%63));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%65))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%47)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%65))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%47))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%65))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%47))))), ne<u32>(read<u32>(field0(%65)), read<u32>(field0(%47)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%62), read<u32>(%63)), read<u32>(%61)), read<u32>(%64)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%62, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%63, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%47), read<u32>(%62));
// DEFAULT-NEXT:         write<@type2>(%65, copy<@type2, reason=assign>(read<@type2>(%47)));
// DEFAULT-NEXT:         write<u32>(%64, call<u32, signature=fn(u32) -> u32>(%53, read<u32>(%63)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%53, read<u32>(%63));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%65))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%47)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%65))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%47))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%65))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%47))))), ne<u32>(read<u32>(field0(%65)), read<u32>(field0(%47)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%62), read<u32>(%63)), read<u32>(%61)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%61)), read<u32>(%64)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%62, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%63, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%47), read<u32>(%62));
// DEFAULT-NEXT:         write<@type2>(%65, copy<@type2, reason=assign>(read<@type2>(%47)));
// DEFAULT-NEXT:         write<u32>(%64, call<u32, signature=fn(u32) -> u32>(%57, read<u32>(%63)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%57, read<u32>(%63));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%65))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%47)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%65))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%47))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%47)))), read<u32>(%64))), ne<u32>(read<u32>(field0(%65)), read<u32>(field0(%47)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%62), read<u32>(%63)), read<u32>(%61)), read<u32>(%64)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %69 @retmeD(%70 x: @type3) -> @type3 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type3, reason=return>(read<@type3>(%70));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %71 @fn1D(%72 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %73 y: @type3 [storage=automatic] = copy<@type3, reason=assign>(read<@type3>(%68));
// DEFAULT-NEXT:         let %618: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%73));
// DEFAULT-NEXT:         let %619: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%618)))), read<u32>(%72)));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%73), read<u64>(%619));
// DEFAULT-NEXT:         write<@type3>(%73, copy<@type3, reason=assign>(call<@type3, signature=fn(@type3) -> @type3, abi=sysv64(native_c) -> native_c>(%69, copy<@type3, reason=arg>(read<@type3>(%73)))));
// DEFAULT-NEXT:         copy<@type3, reason=assign>(call<@type3, signature=fn(@type3) -> @type3, abi=sysv64(native_c) -> native_c>(%69, copy<@type3, reason=arg>(read<@type3>(%73))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%73)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %74 @fn2D(%75 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %76 y: @type3 [storage=automatic] = copy<@type3, reason=assign>(read<@type3>(%68));
// DEFAULT-NEXT:         let %620: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%76));
// DEFAULT-NEXT:         let %621: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%620)))), read<u32>(%75)));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%76), read<u64>(%621));
// DEFAULT-NEXT:         let %622: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%76));
// DEFAULT-NEXT:         let %623: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%622))), const<i32>(15))));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%76), read<u64>(%623));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%76)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %77 @retitD() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%68)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %78 @fn3D(%79 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %624: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%68));
// DEFAULT-NEXT:         let %625: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%624)))), read<u32>(%79)));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%68), read<u64>(%625));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%77);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %80 @testD() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %81 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %82 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %83 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %84 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %85 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %86 x: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %87 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type3>>(%68));
// DEFAULT-NEXT:         for %555
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%81, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%81))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %626: i32 [synthetic] = read<i32>(%81);
// DEFAULT-NEXT:                 let %627: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%626), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%81, read<i32>(%627));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %628: ptr<i8> [synthetic] = read<ptr<i8>>(%87);
// DEFAULT-NEXT:                 let %629: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%628), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%87, read<ptr<i8>>(%629));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%628)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%68), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%68), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%82, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%68))))));
// DEFAULT-NEXT:         write<u32>(%83, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%84, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%68), widen<u64, reason=assign>(read<u32>(%83)));
// DEFAULT-NEXT:         write<@type3>(%86, copy<@type3, reason=assign>(read<@type3>(%68)));
// DEFAULT-NEXT:         write<u32>(%85, call<u32, signature=fn(u32) -> u32>(%71, read<u32>(%84)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%71, read<u32>(%84));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%86)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%68))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%86)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%68)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%86)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%68)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%86)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%68)))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%83), read<u32>(%84)), read<u32>(%82)), read<u32>(%85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%83, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%84, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%68), widen<u64, reason=assign>(read<u32>(%83)));
// DEFAULT-NEXT:         write<@type3>(%86, copy<@type3, reason=assign>(read<@type3>(%68)));
// DEFAULT-NEXT:         write<u32>(%85, call<u32, signature=fn(u32) -> u32>(%74, read<u32>(%84)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%74, read<u32>(%84));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%86)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%68))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%86)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%68)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%86)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%68)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%86)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%68)))))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%83), read<u32>(%84)), read<u32>(%82)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%82)), read<u32>(%85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%83, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%84, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%68), widen<u64, reason=assign>(read<u32>(%83)));
// DEFAULT-NEXT:         write<@type3>(%86, copy<@type3, reason=assign>(read<@type3>(%68)));
// DEFAULT-NEXT:         write<u32>(%85, call<u32, signature=fn(u32) -> u32>(%78, read<u32>(%84)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%78, read<u32>(%84));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%86)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%68))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%86)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%68)))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%68))))), read<u32>(%85))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%86)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%68)))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%83), read<u32>(%84)), read<u32>(%82)), read<u32>(%85)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %90 @retmeE(%91 x: @type4) -> @type4 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type4, reason=return>(read<@type4>(%91));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %92 @fn1E(%93 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %94 y: @type4 [storage=automatic] = copy<@type4, reason=assign>(read<@type4>(%89));
// DEFAULT-NEXT:         let %630: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%94));
// DEFAULT-NEXT:         let %631: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%630)))), read<u32>(%93)));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%94), read<u64>(%631));
// DEFAULT-NEXT:         write<@type4>(%94, copy<@type4, reason=assign>(call<@type4, signature=fn(@type4) -> @type4, abi=sysv64(native_c) -> native_c>(%90, copy<@type4, reason=arg>(read<@type4>(%94)))));
// DEFAULT-NEXT:         copy<@type4, reason=assign>(call<@type4, signature=fn(@type4) -> @type4, abi=sysv64(native_c) -> native_c>(%90, copy<@type4, reason=arg>(read<@type4>(%94))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%94)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %95 @fn2E(%96 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %97 y: @type4 [storage=automatic] = copy<@type4, reason=assign>(read<@type4>(%89));
// DEFAULT-NEXT:         let %632: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%97));
// DEFAULT-NEXT:         let %633: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%632)))), read<u32>(%96)));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%97), read<u64>(%633));
// DEFAULT-NEXT:         let %634: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%97));
// DEFAULT-NEXT:         let %635: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%634))), const<i32>(15))));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%97), read<u64>(%635));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%97)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %98 @retitE() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%89)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %99 @fn3E(%100 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %636: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%89));
// DEFAULT-NEXT:         let %637: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%636)))), read<u32>(%100)));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%89), read<u64>(%637));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%98);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %101 @testE() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %102 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %103 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %104 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %105 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %106 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %107 x: @type4 [storage=automatic];
// DEFAULT-NEXT:         let %108 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type4>>(%89));
// DEFAULT-NEXT:         for %556
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%102, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%102))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %638: i32 [synthetic] = read<i32>(%102);
// DEFAULT-NEXT:                 let %639: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%638), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%102, read<i32>(%639));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %640: ptr<i8> [synthetic] = read<ptr<i8>>(%108);
// DEFAULT-NEXT:                 let %641: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%640), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%108, read<ptr<i8>>(%641));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%640)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field0(%89), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%89), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%103, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%89))))));
// DEFAULT-NEXT:         write<u32>(%104, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%105, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%89), widen<u64, reason=assign>(read<u32>(%104)));
// DEFAULT-NEXT:         write<@type4>(%107, copy<@type4, reason=assign>(read<@type4>(%89)));
// DEFAULT-NEXT:         write<u32>(%106, call<u32, signature=fn(u32) -> u32>(%92, read<u32>(%105)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%92, read<u32>(%105));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%107)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%89))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%107)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%89)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%107)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%89)))))), ne<u64>(read<u64>(field0(%107)), read<u64>(field0(%89)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%104), read<u32>(%105)), read<u32>(%103)), read<u32>(%106)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%104, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%105, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%89), widen<u64, reason=assign>(read<u32>(%104)));
// DEFAULT-NEXT:         write<@type4>(%107, copy<@type4, reason=assign>(read<@type4>(%89)));
// DEFAULT-NEXT:         write<u32>(%106, call<u32, signature=fn(u32) -> u32>(%95, read<u32>(%105)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%95, read<u32>(%105));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%107)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%89))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%107)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%89)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%107)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%89)))))), ne<u64>(read<u64>(field0(%107)), read<u64>(field0(%89)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%104), read<u32>(%105)), read<u32>(%103)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%103)), read<u32>(%106)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%104, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%105, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%89), widen<u64, reason=assign>(read<u32>(%104)));
// DEFAULT-NEXT:         write<@type4>(%107, copy<@type4, reason=assign>(read<@type4>(%89)));
// DEFAULT-NEXT:         write<u32>(%106, call<u32, signature=fn(u32) -> u32>(%99, read<u32>(%105)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%99, read<u32>(%105));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%107)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%89))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%107)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%89)))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%89))))), read<u32>(%106))), ne<u64>(read<u64>(field0(%107)), read<u64>(field0(%89)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%104), read<u32>(%105)), read<u32>(%103)), read<u32>(%106)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %111 @retmeF(%112 x: @type5) -> @type5 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type5, reason=return>(read<@type5>(%112));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %113 @fn1F(%114 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %115 y: @type5 [storage=automatic] = copy<@type5, reason=assign>(read<@type5>(%110));
// DEFAULT-NEXT:         let %642: u64 [synthetic] = read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%115));
// DEFAULT-NEXT:         let %643: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%642)))), read<u32>(%114)));
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%115), read<u64>(%643));
// DEFAULT-NEXT:         write<@type5>(%115, copy<@type5, reason=assign>(call<@type5, signature=fn(@type5) -> @type5, abi=sysv64(native_c) -> native_c>(%111, copy<@type5, reason=arg>(read<@type5>(%115)))));
// DEFAULT-NEXT:         copy<@type5, reason=assign>(call<@type5, signature=fn(@type5) -> @type5, abi=sysv64(native_c) -> native_c>(%111, copy<@type5, reason=arg>(read<@type5>(%115))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%115)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %116 @fn2F(%117 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %118 y: @type5 [storage=automatic] = copy<@type5, reason=assign>(read<@type5>(%110));
// DEFAULT-NEXT:         let %644: u64 [synthetic] = read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%118));
// DEFAULT-NEXT:         let %645: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%644)))), read<u32>(%117)));
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%118), read<u64>(%645));
// DEFAULT-NEXT:         let %646: u64 [synthetic] = read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%118));
// DEFAULT-NEXT:         let %647: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%646))), const<i32>(15))));
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%118), read<u64>(%647));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%118)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %119 @retitF() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%110)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %120 @fn3F(%121 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %648: u64 [synthetic] = read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%110));
// DEFAULT-NEXT:         let %649: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%648)))), read<u32>(%121)));
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%110), read<u64>(%649));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%119);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %122 @testF() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %123 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %124 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %125 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %126 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %127 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %128 x: @type5 [storage=automatic];
// DEFAULT-NEXT:         let %129 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type5>>(%110));
// DEFAULT-NEXT:         for %557
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%123, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%123))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %650: i32 [synthetic] = read<i32>(%123);
// DEFAULT-NEXT:                 let %651: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%650), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%123, read<i32>(%651));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %652: ptr<i8> [synthetic] = read<ptr<i8>>(%129);
// DEFAULT-NEXT:                 let %653: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%652), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%129, read<ptr<i8>>(%653));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%652)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%110), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%110), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%124, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%110))))));
// DEFAULT-NEXT:         write<u32>(%125, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%126, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%110), widen<u64, reason=assign>(read<u32>(%125)));
// DEFAULT-NEXT:         write<@type5>(%128, copy<@type5, reason=assign>(read<@type5>(%110)));
// DEFAULT-NEXT:         write<u32>(%127, call<u32, signature=fn(u32) -> u32>(%113, read<u32>(%126)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%113, read<u32>(%126));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%128)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%110))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%128)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%110)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%128)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%110)))))), ne<u64>(read<u64>(field3(%128)), read<u64>(field3(%110)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%125), read<u32>(%126)), read<u32>(%124)), read<u32>(%127)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%125, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%126, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%110), widen<u64, reason=assign>(read<u32>(%125)));
// DEFAULT-NEXT:         write<@type5>(%128, copy<@type5, reason=assign>(read<@type5>(%110)));
// DEFAULT-NEXT:         write<u32>(%127, call<u32, signature=fn(u32) -> u32>(%116, read<u32>(%126)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%116, read<u32>(%126));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%128)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%110))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%128)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%110)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%128)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%110)))))), ne<u64>(read<u64>(field3(%128)), read<u64>(field3(%110)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%125), read<u32>(%126)), read<u32>(%124)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%124)), read<u32>(%127)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%125, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%126, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%110), widen<u64, reason=assign>(read<u32>(%125)));
// DEFAULT-NEXT:         write<@type5>(%128, copy<@type5, reason=assign>(read<@type5>(%110)));
// DEFAULT-NEXT:         write<u32>(%127, call<u32, signature=fn(u32) -> u32>(%120, read<u32>(%126)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%120, read<u32>(%126));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%128)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%110))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%128)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%110)))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%110))))), read<u32>(%127))), ne<u64>(read<u64>(field3(%128)), read<u64>(field3(%110)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%125), read<u32>(%126)), read<u32>(%124)), read<u32>(%127)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %132 @retmeG(%133 x: @type6) -> @type6 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type6, reason=return>(read<@type6>(%133));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %134 @fn1G(%135 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %136 y: @type6 [storage=automatic] = copy<@type6, reason=assign>(read<@type6>(%131));
// DEFAULT-NEXT:         let %654: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%136));
// DEFAULT-NEXT:         let %655: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%654))), read<u32>(%135));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%136), read<u32>(%655));
// DEFAULT-NEXT:         write<@type6>(%136, copy<@type6, reason=assign>(call<@type6, signature=fn(@type6) -> @type6, abi=sysv64(native_c) -> native_c>(%132, copy<@type6, reason=arg>(read<@type6>(%136)))));
// DEFAULT-NEXT:         copy<@type6, reason=assign>(call<@type6, signature=fn(@type6) -> @type6, abi=sysv64(native_c) -> native_c>(%132, copy<@type6, reason=arg>(read<@type6>(%136))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%136))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %137 @fn2G(%138 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %139 y: @type6 [storage=automatic] = copy<@type6, reason=assign>(read<@type6>(%131));
// DEFAULT-NEXT:         let %656: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%139));
// DEFAULT-NEXT:         let %657: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%656))), read<u32>(%138));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%139), read<u32>(%657));
// DEFAULT-NEXT:         let %658: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%139));
// DEFAULT-NEXT:         let %659: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%658)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%139), read<u32>(%659));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%139))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %140 @retitG() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%131))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %141 @fn3G(%142 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %660: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%131));
// DEFAULT-NEXT:         let %661: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%660))), read<u32>(%142));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%131), read<u32>(%661));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%140);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %143 @testG() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %144 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %145 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %146 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %147 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %148 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %149 x: @type6 [storage=automatic];
// DEFAULT-NEXT:         let %150 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type6>>(%131));
// DEFAULT-NEXT:         for %558
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%144, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%144))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %662: i32 [synthetic] = read<i32>(%144);
// DEFAULT-NEXT:                 let %663: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%662), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%144, read<i32>(%663));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %664: ptr<i8> [synthetic] = read<ptr<i8>>(%150);
// DEFAULT-NEXT:                 let %665: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%664), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%150, read<ptr<i8>>(%665));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%664)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%131), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%131), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%145, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%131)))));
// DEFAULT-NEXT:         write<u32>(%146, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%147, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%131), read<u32>(%146));
// DEFAULT-NEXT:         write<@type6>(%149, copy<@type6, reason=assign>(read<@type6>(%131)));
// DEFAULT-NEXT:         write<u32>(%148, call<u32, signature=fn(u32) -> u32>(%134, read<u32>(%147)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%134, read<u32>(%147));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%149))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%131)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%149))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%131))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%149))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%131))))), ne<u64>(read<u64>(field3(%149)), read<u64>(field3(%131)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%146), read<u32>(%147)), read<u32>(%145)), read<u32>(%148)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%146, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%147, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%131), read<u32>(%146));
// DEFAULT-NEXT:         write<@type6>(%149, copy<@type6, reason=assign>(read<@type6>(%131)));
// DEFAULT-NEXT:         write<u32>(%148, call<u32, signature=fn(u32) -> u32>(%137, read<u32>(%147)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%137, read<u32>(%147));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%149))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%131)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%149))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%131))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%149))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%131))))), ne<u64>(read<u64>(field3(%149)), read<u64>(field3(%131)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%146), read<u32>(%147)), read<u32>(%145)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%145)), read<u32>(%148)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%146, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%147, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%131), read<u32>(%146));
// DEFAULT-NEXT:         write<@type6>(%149, copy<@type6, reason=assign>(read<@type6>(%131)));
// DEFAULT-NEXT:         write<u32>(%148, call<u32, signature=fn(u32) -> u32>(%141, read<u32>(%147)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%141, read<u32>(%147));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%149))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%131)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%149))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%131))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%131)))), read<u32>(%148))), ne<u64>(read<u64>(field3(%149)), read<u64>(field3(%131)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%146), read<u32>(%147)), read<u32>(%145)), read<u32>(%148)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %153 @retmeH(%154 x: @type7) -> @type7 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type7, reason=return>(read<@type7>(%154));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %155 @fn1H(%156 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %157 y: @type7 [storage=automatic] = copy<@type7, reason=assign>(read<@type7>(%152));
// DEFAULT-NEXT:         let %666: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%157));
// DEFAULT-NEXT:         let %667: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%666))), read<u32>(%156));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%157), read<u32>(%667));
// DEFAULT-NEXT:         write<@type7>(%157, copy<@type7, reason=assign>(call<@type7, signature=fn(@type7) -> @type7, abi=sysv64(native_c) -> native_c>(%153, copy<@type7, reason=arg>(read<@type7>(%157)))));
// DEFAULT-NEXT:         copy<@type7, reason=assign>(call<@type7, signature=fn(@type7) -> @type7, abi=sysv64(native_c) -> native_c>(%153, copy<@type7, reason=arg>(read<@type7>(%157))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%157))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %158 @fn2H(%159 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %160 y: @type7 [storage=automatic] = copy<@type7, reason=assign>(read<@type7>(%152));
// DEFAULT-NEXT:         let %668: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%160));
// DEFAULT-NEXT:         let %669: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%668))), read<u32>(%159));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%160), read<u32>(%669));
// DEFAULT-NEXT:         let %670: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%160));
// DEFAULT-NEXT:         let %671: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%670)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%160), read<u32>(%671));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%160))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %161 @retitH() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%152))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %162 @fn3H(%163 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %672: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%152));
// DEFAULT-NEXT:         let %673: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%672))), read<u32>(%163));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%152), read<u32>(%673));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%161);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %164 @testH() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %165 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %166 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %167 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %168 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %169 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %170 x: @type7 [storage=automatic];
// DEFAULT-NEXT:         let %171 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type7>>(%152));
// DEFAULT-NEXT:         for %559
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%165, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%165))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %674: i32 [synthetic] = read<i32>(%165);
// DEFAULT-NEXT:                 let %675: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%674), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%165, read<i32>(%675));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %676: ptr<i8> [synthetic] = read<ptr<i8>>(%171);
// DEFAULT-NEXT:                 let %677: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%676), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%171, read<ptr<i8>>(%677));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%676)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%152), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%152), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%166, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%152)))));
// DEFAULT-NEXT:         write<u32>(%167, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%168, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%152), read<u32>(%167));
// DEFAULT-NEXT:         write<@type7>(%170, copy<@type7, reason=assign>(read<@type7>(%152)));
// DEFAULT-NEXT:         write<u32>(%169, call<u32, signature=fn(u32) -> u32>(%155, read<u32>(%168)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%155, read<u32>(%168));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%170))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%152)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%170))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%152))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%170))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%152))))), ne<u64>(read<u64>(field3(%170)), read<u64>(field3(%152)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%167), read<u32>(%168)), read<u32>(%166)), read<u32>(%169)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%167, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%168, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%152), read<u32>(%167));
// DEFAULT-NEXT:         write<@type7>(%170, copy<@type7, reason=assign>(read<@type7>(%152)));
// DEFAULT-NEXT:         write<u32>(%169, call<u32, signature=fn(u32) -> u32>(%158, read<u32>(%168)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%158, read<u32>(%168));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%170))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%152)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%170))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%152))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%170))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%152))))), ne<u64>(read<u64>(field3(%170)), read<u64>(field3(%152)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%167), read<u32>(%168)), read<u32>(%166)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%166)), read<u32>(%169)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%167, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%168, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%152), read<u32>(%167));
// DEFAULT-NEXT:         write<@type7>(%170, copy<@type7, reason=assign>(read<@type7>(%152)));
// DEFAULT-NEXT:         write<u32>(%169, call<u32, signature=fn(u32) -> u32>(%162, read<u32>(%168)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%162, read<u32>(%168));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%170))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%152)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%170))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%152))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%152)))), read<u32>(%169))), ne<u64>(read<u64>(field3(%170)), read<u64>(field3(%152)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%167), read<u32>(%168)), read<u32>(%166)), read<u32>(%169)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %174 @retmeI(%175 x: @type8) -> @type8 [linkage=external] [abi=sysv64(coerce<i16, i64>) -> coerce<i16, i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type8, reason=return>(read<@type8>(%175));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %176 @fn1I(%177 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %178 y: @type8 [storage=automatic] = copy<@type8, reason=assign>(read<@type8>(%173));
// DEFAULT-NEXT:         let %678: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%178));
// DEFAULT-NEXT:         let %679: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%678)))), read<u32>(%177)));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%178), read<u16>(%679));
// DEFAULT-NEXT:         write<@type8>(%178, copy<@type8, reason=assign>(call<@type8, signature=fn(@type8) -> @type8, abi=sysv64(coerce<i16, i64>) -> coerce<i16, i64>>(%174, copy<@type8, reason=arg>(read<@type8>(%178)))));
// DEFAULT-NEXT:         copy<@type8, reason=assign>(call<@type8, signature=fn(@type8) -> @type8, abi=sysv64(coerce<i16, i64>) -> coerce<i16, i64>>(%174, copy<@type8, reason=arg>(read<@type8>(%178))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%178)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %179 @fn2I(%180 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %181 y: @type8 [storage=automatic] = copy<@type8, reason=assign>(read<@type8>(%173));
// DEFAULT-NEXT:         let %680: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%181));
// DEFAULT-NEXT:         let %681: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%680)))), read<u32>(%180)));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%181), read<u16>(%681));
// DEFAULT-NEXT:         let %682: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%181));
// DEFAULT-NEXT:         let %683: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%682))), const<i32>(15))));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%181), read<u16>(%683));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%181)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %182 @retitI() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%173)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %183 @fn3I(%184 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %684: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%173));
// DEFAULT-NEXT:         let %685: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%684)))), read<u32>(%184)));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%173), read<u16>(%685));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%182);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %185 @testI() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %186 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %187 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %188 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %189 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %190 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %191 x: @type8 [storage=automatic];
// DEFAULT-NEXT:         let %192 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type8>>(%173));
// DEFAULT-NEXT:         for %560
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%186, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%186))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %686: i32 [synthetic] = read<i32>(%186);
// DEFAULT-NEXT:                 let %687: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%686), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%186, read<i32>(%687));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %688: ptr<i8> [synthetic] = read<ptr<i8>>(%192);
// DEFAULT-NEXT:                 let %689: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%688), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%192, read<ptr<i8>>(%689));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%688)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%173), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%173), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%187, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%173))))));
// DEFAULT-NEXT:         write<u32>(%188, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%189, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%173), truncate<u16, reason=assign, fits=unknown>(read<u32>(%188)));
// DEFAULT-NEXT:         write<@type8>(%191, copy<@type8, reason=assign>(read<@type8>(%173)));
// DEFAULT-NEXT:         write<u32>(%190, call<u32, signature=fn(u32) -> u32>(%176, read<u32>(%189)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%176, read<u32>(%189));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%191)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%173))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%191)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%173)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%191)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%173)))))), ne<u64>(read<u64>(field3(%191)), read<u64>(field3(%173)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%188), read<u32>(%189)), read<u32>(%187)), read<u32>(%190)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%188, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%189, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%173), truncate<u16, reason=assign, fits=unknown>(read<u32>(%188)));
// DEFAULT-NEXT:         write<@type8>(%191, copy<@type8, reason=assign>(read<@type8>(%173)));
// DEFAULT-NEXT:         write<u32>(%190, call<u32, signature=fn(u32) -> u32>(%179, read<u32>(%189)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%179, read<u32>(%189));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%191)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%173))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%191)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%173)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%191)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%173)))))), ne<u64>(read<u64>(field3(%191)), read<u64>(field3(%173)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%188), read<u32>(%189)), read<u32>(%187)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%187)), read<u32>(%190)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%188, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%189, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%173), truncate<u16, reason=assign, fits=unknown>(read<u32>(%188)));
// DEFAULT-NEXT:         write<@type8>(%191, copy<@type8, reason=assign>(read<@type8>(%173)));
// DEFAULT-NEXT:         write<u32>(%190, call<u32, signature=fn(u32) -> u32>(%183, read<u32>(%189)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%183, read<u32>(%189));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%191)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%173))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%191)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%173)))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%173))))), read<u32>(%190))), ne<u64>(read<u64>(field3(%191)), read<u64>(field3(%173)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%188), read<u32>(%189)), read<u32>(%187)), read<u32>(%190)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %195 @retmeJ(%196 x: @type9) -> @type9 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type9, reason=return>(read<@type9>(%196));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %197 @fn1J(%198 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %199 y: @type9 [storage=automatic] = copy<@type9, reason=assign>(read<@type9>(%194));
// DEFAULT-NEXT:         let %690: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%199));
// DEFAULT-NEXT:         let %691: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%690)))), read<u32>(%198)));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%199), read<u16>(%691));
// DEFAULT-NEXT:         write<@type9>(%199, copy<@type9, reason=assign>(call<@type9, signature=fn(@type9) -> @type9, abi=sysv64(native_c) -> native_c>(%195, copy<@type9, reason=arg>(read<@type9>(%199)))));
// DEFAULT-NEXT:         copy<@type9, reason=assign>(call<@type9, signature=fn(@type9) -> @type9, abi=sysv64(native_c) -> native_c>(%195, copy<@type9, reason=arg>(read<@type9>(%199))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%199)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %200 @fn2J(%201 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %202 y: @type9 [storage=automatic] = copy<@type9, reason=assign>(read<@type9>(%194));
// DEFAULT-NEXT:         let %692: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%202));
// DEFAULT-NEXT:         let %693: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%692)))), read<u32>(%201)));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%202), read<u16>(%693));
// DEFAULT-NEXT:         let %694: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%202));
// DEFAULT-NEXT:         let %695: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%694))), const<i32>(15))));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%202), read<u16>(%695));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%202)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %203 @retitJ() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%194)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %204 @fn3J(%205 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %696: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%194));
// DEFAULT-NEXT:         let %697: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%696)))), read<u32>(%205)));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%194), read<u16>(%697));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%203);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %206 @testJ() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %207 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %208 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %209 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %210 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %211 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %212 x: @type9 [storage=automatic];
// DEFAULT-NEXT:         let %213 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type9>>(%194));
// DEFAULT-NEXT:         for %561
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%207, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%207))), const<u64>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %698: i32 [synthetic] = read<i32>(%207);
// DEFAULT-NEXT:                 let %699: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%698), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%207, read<i32>(%699));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %700: ptr<i8> [synthetic] = read<ptr<i8>>(%213);
// DEFAULT-NEXT:                 let %701: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%700), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%213, read<ptr<i8>>(%701));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%700)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u16>(field3(%194), float_to_int<u16, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%194), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%208, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%194))))));
// DEFAULT-NEXT:         write<u32>(%209, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%210, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%194), truncate<u16, reason=assign, fits=unknown>(read<u32>(%209)));
// DEFAULT-NEXT:         write<@type9>(%212, copy<@type9, reason=assign>(read<@type9>(%194)));
// DEFAULT-NEXT:         write<u32>(%211, call<u32, signature=fn(u32) -> u32>(%197, read<u32>(%210)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%197, read<u32>(%210));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%212)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%194))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%212)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%194)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%212)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%194)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%212)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%194)))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%209), read<u32>(%210)), read<u32>(%208)), read<u32>(%211)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%209, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%210, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%194), truncate<u16, reason=assign, fits=unknown>(read<u32>(%209)));
// DEFAULT-NEXT:         write<@type9>(%212, copy<@type9, reason=assign>(read<@type9>(%194)));
// DEFAULT-NEXT:         write<u32>(%211, call<u32, signature=fn(u32) -> u32>(%200, read<u32>(%210)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%200, read<u32>(%210));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%212)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%194))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%212)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%194)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%212)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%194)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%212)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%194)))))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%209), read<u32>(%210)), read<u32>(%208)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%208)), read<u32>(%211)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%209, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%210, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%194), truncate<u16, reason=assign, fits=unknown>(read<u32>(%209)));
// DEFAULT-NEXT:         write<@type9>(%212, copy<@type9, reason=assign>(read<@type9>(%194)));
// DEFAULT-NEXT:         write<u32>(%211, call<u32, signature=fn(u32) -> u32>(%204, read<u32>(%210)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%204, read<u32>(%210));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%212)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%194))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%212)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%194)))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%194))))), read<u32>(%211))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%212)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%194)))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%209), read<u32>(%210)), read<u32>(%208)), read<u32>(%211)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %216 @retmeK(%217 x: @type10) -> @type10 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type10, reason=return>(read<@type10>(%217));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %218 @fn1K(%219 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %220 y: @type10 [storage=automatic] = copy<@type10, reason=assign>(read<@type10>(%215));
// DEFAULT-NEXT:         let %702: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%220));
// DEFAULT-NEXT:         let %703: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%702))), read<u32>(%219));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%220), read<u32>(%703));
// DEFAULT-NEXT:         write<@type10>(%220, copy<@type10, reason=assign>(call<@type10, signature=fn(@type10) -> @type10, abi=sysv64(native_c) -> native_c>(%216, copy<@type10, reason=arg>(read<@type10>(%220)))));
// DEFAULT-NEXT:         copy<@type10, reason=assign>(call<@type10, signature=fn(@type10) -> @type10, abi=sysv64(native_c) -> native_c>(%216, copy<@type10, reason=arg>(read<@type10>(%220))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%220))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %221 @fn2K(%222 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %223 y: @type10 [storage=automatic] = copy<@type10, reason=assign>(read<@type10>(%215));
// DEFAULT-NEXT:         let %704: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%223));
// DEFAULT-NEXT:         let %705: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%704))), read<u32>(%222));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%223), read<u32>(%705));
// DEFAULT-NEXT:         let %706: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%223));
// DEFAULT-NEXT:         let %707: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%706)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%223), read<u32>(%707));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%223))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %224 @retitK() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%215))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %225 @fn3K(%226 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %708: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%215));
// DEFAULT-NEXT:         let %709: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%708))), read<u32>(%226));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%215), read<u32>(%709));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%224);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %227 @testK() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %228 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %229 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %230 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %231 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %232 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %233 x: @type10 [storage=automatic];
// DEFAULT-NEXT:         let %234 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type10>>(%215));
// DEFAULT-NEXT:         for %562
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%228, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%228))), const<u64>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %710: i32 [synthetic] = read<i32>(%228);
// DEFAULT-NEXT:                 let %711: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%710), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%228, read<i32>(%711));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %712: ptr<i8> [synthetic] = read<ptr<i8>>(%234);
// DEFAULT-NEXT:                 let %713: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%712), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%234, read<ptr<i8>>(%713));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%712)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%215), float_to_int<u32, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%215), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%229, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%215)))));
// DEFAULT-NEXT:         write<u32>(%230, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%231, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%215), read<u32>(%230));
// DEFAULT-NEXT:         write<@type10>(%233, copy<@type10, reason=assign>(read<@type10>(%215)));
// DEFAULT-NEXT:         write<u32>(%232, call<u32, signature=fn(u32) -> u32>(%218, read<u32>(%231)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%218, read<u32>(%231));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%233))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%215)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%233))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%215))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%233))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%215))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%233))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%215))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%230), read<u32>(%231)), read<u32>(%229)), read<u32>(%232)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%230, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%231, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%215), read<u32>(%230));
// DEFAULT-NEXT:         write<@type10>(%233, copy<@type10, reason=assign>(read<@type10>(%215)));
// DEFAULT-NEXT:         write<u32>(%232, call<u32, signature=fn(u32) -> u32>(%221, read<u32>(%231)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%221, read<u32>(%231));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%233))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%215)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%233))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%215))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%233))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%215))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%233))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%215))))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%230), read<u32>(%231)), read<u32>(%229)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%229)), read<u32>(%232)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%230, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%231, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%215), read<u32>(%230));
// DEFAULT-NEXT:         write<@type10>(%233, copy<@type10, reason=assign>(read<@type10>(%215)));
// DEFAULT-NEXT:         write<u32>(%232, call<u32, signature=fn(u32) -> u32>(%225, read<u32>(%231)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%225, read<u32>(%231));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%233))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%215)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%233))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%215))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%215)))), read<u32>(%232))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%233))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%215))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%230), read<u32>(%231)), read<u32>(%229)), read<u32>(%232)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %237 @retmeL(%238 x: @type11) -> @type11 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type11, reason=return>(read<@type11>(%238));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %239 @fn1L(%240 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %241 y: @type11 [storage=automatic] = copy<@type11, reason=assign>(read<@type11>(%236));
// DEFAULT-NEXT:         let %714: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%241));
// DEFAULT-NEXT:         let %715: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%714))), read<u32>(%240));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%241), read<u32>(%715));
// DEFAULT-NEXT:         write<@type11>(%241, copy<@type11, reason=assign>(call<@type11, signature=fn(@type11) -> @type11, abi=sysv64(native_c) -> native_c>(%237, copy<@type11, reason=arg>(read<@type11>(%241)))));
// DEFAULT-NEXT:         copy<@type11, reason=assign>(call<@type11, signature=fn(@type11) -> @type11, abi=sysv64(native_c) -> native_c>(%237, copy<@type11, reason=arg>(read<@type11>(%241))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%241))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %242 @fn2L(%243 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %244 y: @type11 [storage=automatic] = copy<@type11, reason=assign>(read<@type11>(%236));
// DEFAULT-NEXT:         let %716: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%244));
// DEFAULT-NEXT:         let %717: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%716))), read<u32>(%243));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%244), read<u32>(%717));
// DEFAULT-NEXT:         let %718: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%244));
// DEFAULT-NEXT:         let %719: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%718)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%244), read<u32>(%719));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%244))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %245 @retitL() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%236))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %246 @fn3L(%247 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %720: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%236));
// DEFAULT-NEXT:         let %721: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%720))), read<u32>(%247));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%236), read<u32>(%721));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%245);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %248 @testL() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %249 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %250 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %251 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %252 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %253 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %254 x: @type11 [storage=automatic];
// DEFAULT-NEXT:         let %255 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type11>>(%236));
// DEFAULT-NEXT:         for %563
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%249, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%249))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %722: i32 [synthetic] = read<i32>(%249);
// DEFAULT-NEXT:                 let %723: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%722), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%249, read<i32>(%723));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %724: ptr<i8> [synthetic] = read<ptr<i8>>(%255);
// DEFAULT-NEXT:                 let %725: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%724), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%255, read<ptr<i8>>(%725));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%724)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u32>(field3(%236), float_to_int<u32, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%236), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%250, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%236)))));
// DEFAULT-NEXT:         write<u32>(%251, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%252, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%236), read<u32>(%251));
// DEFAULT-NEXT:         write<@type11>(%254, copy<@type11, reason=assign>(read<@type11>(%236)));
// DEFAULT-NEXT:         write<u32>(%253, call<u32, signature=fn(u32) -> u32>(%239, read<u32>(%252)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%239, read<u32>(%252));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%254))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%236)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%254))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%236))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%254))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%236))))), ne<u32>(read<u32>(field3(%254)), read<u32>(field3(%236)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%251), read<u32>(%252)), read<u32>(%250)), read<u32>(%253)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%251, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%252, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%236), read<u32>(%251));
// DEFAULT-NEXT:         write<@type11>(%254, copy<@type11, reason=assign>(read<@type11>(%236)));
// DEFAULT-NEXT:         write<u32>(%253, call<u32, signature=fn(u32) -> u32>(%242, read<u32>(%252)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%242, read<u32>(%252));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%254))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%236)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%254))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%236))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%254))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%236))))), ne<u32>(read<u32>(field3(%254)), read<u32>(field3(%236)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%251), read<u32>(%252)), read<u32>(%250)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%250)), read<u32>(%253)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%251, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%252, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%236), read<u32>(%251));
// DEFAULT-NEXT:         write<@type11>(%254, copy<@type11, reason=assign>(read<@type11>(%236)));
// DEFAULT-NEXT:         write<u32>(%253, call<u32, signature=fn(u32) -> u32>(%246, read<u32>(%252)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%246, read<u32>(%252));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%254))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%236)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%254))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%236))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%236)))), read<u32>(%253))), ne<u32>(read<u32>(field3(%254)), read<u32>(field3(%236)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%251), read<u32>(%252)), read<u32>(%250)), read<u32>(%253)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %258 @retmeM(%259 x: @type12) -> @type12 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type12, reason=return>(read<@type12>(%259));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %260 @fn1M(%261 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %262 y: @type12 [storage=automatic] = copy<@type12, reason=assign>(read<@type12>(%257));
// DEFAULT-NEXT:         let %726: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%262));
// DEFAULT-NEXT:         let %727: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%726))), read<u32>(%261));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%262), read<u32>(%727));
// DEFAULT-NEXT:         write<@type12>(%262, copy<@type12, reason=assign>(call<@type12, signature=fn(@type12) -> @type12, abi=sysv64(native_c) -> native_c>(%258, copy<@type12, reason=arg>(read<@type12>(%262)))));
// DEFAULT-NEXT:         copy<@type12, reason=assign>(call<@type12, signature=fn(@type12) -> @type12, abi=sysv64(native_c) -> native_c>(%258, copy<@type12, reason=arg>(read<@type12>(%262))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%262))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %263 @fn2M(%264 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %265 y: @type12 [storage=automatic] = copy<@type12, reason=assign>(read<@type12>(%257));
// DEFAULT-NEXT:         let %728: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%265));
// DEFAULT-NEXT:         let %729: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%728))), read<u32>(%264));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%265), read<u32>(%729));
// DEFAULT-NEXT:         let %730: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%265));
// DEFAULT-NEXT:         let %731: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%730)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%265), read<u32>(%731));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%265))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %266 @retitM() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%257))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %267 @fn3M(%268 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %732: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%257));
// DEFAULT-NEXT:         let %733: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%732))), read<u32>(%268));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%257), read<u32>(%733));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%266);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %269 @testM() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %270 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %271 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %272 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %273 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %274 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %275 x: @type12 [storage=automatic];
// DEFAULT-NEXT:         let %276 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type12>>(%257));
// DEFAULT-NEXT:         for %564
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%270, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%270))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %734: i32 [synthetic] = read<i32>(%270);
// DEFAULT-NEXT:                 let %735: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%734), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%270, read<i32>(%735));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %736: ptr<i8> [synthetic] = read<ptr<i8>>(%276);
// DEFAULT-NEXT:                 let %737: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%736), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%276, read<ptr<i8>>(%737));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%736)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u32>(field0(%257), float_to_int<u32, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%257), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%271, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%257)))));
// DEFAULT-NEXT:         write<u32>(%272, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%273, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%257), read<u32>(%272));
// DEFAULT-NEXT:         write<@type12>(%275, copy<@type12, reason=assign>(read<@type12>(%257)));
// DEFAULT-NEXT:         write<u32>(%274, call<u32, signature=fn(u32) -> u32>(%260, read<u32>(%273)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%260, read<u32>(%273));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%275))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%257)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%275))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%257))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%275))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%257))))), ne<u32>(read<u32>(field0(%275)), read<u32>(field0(%257)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%272), read<u32>(%273)), read<u32>(%271)), read<u32>(%274)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%272, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%273, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%257), read<u32>(%272));
// DEFAULT-NEXT:         write<@type12>(%275, copy<@type12, reason=assign>(read<@type12>(%257)));
// DEFAULT-NEXT:         write<u32>(%274, call<u32, signature=fn(u32) -> u32>(%263, read<u32>(%273)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%263, read<u32>(%273));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%275))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%257)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%275))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%257))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%275))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%257))))), ne<u32>(read<u32>(field0(%275)), read<u32>(field0(%257)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%272), read<u32>(%273)), read<u32>(%271)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%271)), read<u32>(%274)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%272, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%273, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%257), read<u32>(%272));
// DEFAULT-NEXT:         write<@type12>(%275, copy<@type12, reason=assign>(read<@type12>(%257)));
// DEFAULT-NEXT:         write<u32>(%274, call<u32, signature=fn(u32) -> u32>(%267, read<u32>(%273)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%267, read<u32>(%273));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%275))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%257)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%275))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%257))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%257)))), read<u32>(%274))), ne<u32>(read<u32>(field0(%275)), read<u32>(field0(%257)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%272), read<u32>(%273)), read<u32>(%271)), read<u32>(%274)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %279 @retmeN(%280 x: @type13) -> @type13 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type13, reason=return>(read<@type13>(%280));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %281 @fn1N(%282 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %283 y: @type13 [storage=automatic] = copy<@type13, reason=assign>(read<@type13>(%278));
// DEFAULT-NEXT:         let %738: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%283));
// DEFAULT-NEXT:         let %739: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%738)))), read<u32>(%282)));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%283), read<u64>(%739));
// DEFAULT-NEXT:         write<@type13>(%283, copy<@type13, reason=assign>(call<@type13, signature=fn(@type13) -> @type13, abi=sysv64(native_c) -> native_c>(%279, copy<@type13, reason=arg>(read<@type13>(%283)))));
// DEFAULT-NEXT:         copy<@type13, reason=assign>(call<@type13, signature=fn(@type13) -> @type13, abi=sysv64(native_c) -> native_c>(%279, copy<@type13, reason=arg>(read<@type13>(%283))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%283)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %284 @fn2N(%285 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %286 y: @type13 [storage=automatic] = copy<@type13, reason=assign>(read<@type13>(%278));
// DEFAULT-NEXT:         let %740: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%286));
// DEFAULT-NEXT:         let %741: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%740)))), read<u32>(%285)));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%286), read<u64>(%741));
// DEFAULT-NEXT:         let %742: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%286));
// DEFAULT-NEXT:         let %743: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%742))), const<i32>(15))));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%286), read<u64>(%743));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%286)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %287 @retitN() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%278)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %288 @fn3N(%289 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %744: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%278));
// DEFAULT-NEXT:         let %745: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%744)))), read<u32>(%289)));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%278), read<u64>(%745));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%287);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %290 @testN() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %291 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %292 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %293 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %294 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %295 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %296 x: @type13 [storage=automatic];
// DEFAULT-NEXT:         let %297 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type13>>(%278));
// DEFAULT-NEXT:         for %565
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%291, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%291))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %746: i32 [synthetic] = read<i32>(%291);
// DEFAULT-NEXT:                 let %747: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%746), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%291, read<i32>(%747));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %748: ptr<i8> [synthetic] = read<ptr<i8>>(%297);
// DEFAULT-NEXT:                 let %749: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%748), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%297, read<ptr<i8>>(%749));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%748)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%278), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%278), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%292, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%278))))));
// DEFAULT-NEXT:         write<u32>(%293, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%294, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%278), widen<u64, reason=assign>(read<u32>(%293)));
// DEFAULT-NEXT:         write<@type13>(%296, copy<@type13, reason=assign>(read<@type13>(%278)));
// DEFAULT-NEXT:         write<u32>(%295, call<u32, signature=fn(u32) -> u32>(%281, read<u32>(%294)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%281, read<u32>(%294));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%296)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%278))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%296)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%278)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%296)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%278)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%296)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%278)))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%293), read<u32>(%294)), read<u32>(%292)), read<u32>(%295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%293, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%294, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%278), widen<u64, reason=assign>(read<u32>(%293)));
// DEFAULT-NEXT:         write<@type13>(%296, copy<@type13, reason=assign>(read<@type13>(%278)));
// DEFAULT-NEXT:         write<u32>(%295, call<u32, signature=fn(u32) -> u32>(%284, read<u32>(%294)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%284, read<u32>(%294));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%296)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%278))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%296)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%278)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%296)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%278)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%296)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%278)))))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%293), read<u32>(%294)), read<u32>(%292)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%292)), read<u32>(%295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%293, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%294, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%278), widen<u64, reason=assign>(read<u32>(%293)));
// DEFAULT-NEXT:         write<@type13>(%296, copy<@type13, reason=assign>(read<@type13>(%278)));
// DEFAULT-NEXT:         write<u32>(%295, call<u32, signature=fn(u32) -> u32>(%288, read<u32>(%294)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%288, read<u32>(%294));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%296)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%278))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%296)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%278)))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%278))))), read<u32>(%295))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%296)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%278)))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%293), read<u32>(%294)), read<u32>(%292)), read<u32>(%295)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %300 @retmeO(%301 x: @type14) -> @type14 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type14, reason=return>(read<@type14>(%301));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %302 @fn1O(%303 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %304 y: @type14 [storage=automatic] = copy<@type14, reason=assign>(read<@type14>(%299));
// DEFAULT-NEXT:         let %750: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%304));
// DEFAULT-NEXT:         let %751: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%750)))), read<u32>(%303)));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%304), read<u64>(%751));
// DEFAULT-NEXT:         write<@type14>(%304, copy<@type14, reason=assign>(call<@type14, signature=fn(@type14) -> @type14, abi=sysv64(native_c) -> native_c>(%300, copy<@type14, reason=arg>(read<@type14>(%304)))));
// DEFAULT-NEXT:         copy<@type14, reason=assign>(call<@type14, signature=fn(@type14) -> @type14, abi=sysv64(native_c) -> native_c>(%300, copy<@type14, reason=arg>(read<@type14>(%304))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%304)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %305 @fn2O(%306 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %307 y: @type14 [storage=automatic] = copy<@type14, reason=assign>(read<@type14>(%299));
// DEFAULT-NEXT:         let %752: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%307));
// DEFAULT-NEXT:         let %753: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%752)))), read<u32>(%306)));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%307), read<u64>(%753));
// DEFAULT-NEXT:         let %754: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%307));
// DEFAULT-NEXT:         let %755: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%754))), const<i32>(15))));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%307), read<u64>(%755));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%307)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %308 @retitO() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%299)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %309 @fn3O(%310 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %756: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%299));
// DEFAULT-NEXT:         let %757: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%756)))), read<u32>(%310)));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%299), read<u64>(%757));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%308);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %311 @testO() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %312 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %313 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %314 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %315 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %316 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %317 x: @type14 [storage=automatic];
// DEFAULT-NEXT:         let %318 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type14>>(%299));
// DEFAULT-NEXT:         for %566
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%312, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%312))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %758: i32 [synthetic] = read<i32>(%312);
// DEFAULT-NEXT:                 let %759: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%758), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%312, read<i32>(%759));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %760: ptr<i8> [synthetic] = read<ptr<i8>>(%318);
// DEFAULT-NEXT:                 let %761: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%760), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%318, read<ptr<i8>>(%761));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%760)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field0(%299), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%299), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%313, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%299))))));
// DEFAULT-NEXT:         write<u32>(%314, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%315, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%299), widen<u64, reason=assign>(read<u32>(%314)));
// DEFAULT-NEXT:         write<@type14>(%317, copy<@type14, reason=assign>(read<@type14>(%299)));
// DEFAULT-NEXT:         write<u32>(%316, call<u32, signature=fn(u32) -> u32>(%302, read<u32>(%315)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%302, read<u32>(%315));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%317)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%299))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%317)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%299)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%317)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%299)))))), ne<u64>(read<u64>(field0(%317)), read<u64>(field0(%299)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%314), read<u32>(%315)), read<u32>(%313)), read<u32>(%316)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%314, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%315, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%299), widen<u64, reason=assign>(read<u32>(%314)));
// DEFAULT-NEXT:         write<@type14>(%317, copy<@type14, reason=assign>(read<@type14>(%299)));
// DEFAULT-NEXT:         write<u32>(%316, call<u32, signature=fn(u32) -> u32>(%305, read<u32>(%315)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%305, read<u32>(%315));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%317)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%299))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%317)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%299)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%317)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%299)))))), ne<u64>(read<u64>(field0(%317)), read<u64>(field0(%299)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%314), read<u32>(%315)), read<u32>(%313)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%313)), read<u32>(%316)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%314, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%315, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%299), widen<u64, reason=assign>(read<u32>(%314)));
// DEFAULT-NEXT:         write<@type14>(%317, copy<@type14, reason=assign>(read<@type14>(%299)));
// DEFAULT-NEXT:         write<u32>(%316, call<u32, signature=fn(u32) -> u32>(%309, read<u32>(%315)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%309, read<u32>(%315));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%317)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%299))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%317)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%299)))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%299))))), read<u32>(%316))), ne<u64>(read<u64>(field0(%317)), read<u64>(field0(%299)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%314), read<u32>(%315)), read<u32>(%313)), read<u32>(%316)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %321 @retmeP(%322 x: @type15) -> @type15 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type15, reason=return>(read<@type15>(%322));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %323 @fn1P(%324 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %325 y: @type15 [storage=automatic] = copy<@type15, reason=assign>(read<@type15>(%320));
// DEFAULT-NEXT:         let %762: u64 [synthetic] = read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%325));
// DEFAULT-NEXT:         let %763: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%762)))), read<u32>(%324)));
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%325), read<u64>(%763));
// DEFAULT-NEXT:         write<@type15>(%325, copy<@type15, reason=assign>(call<@type15, signature=fn(@type15) -> @type15, abi=sysv64(native_c) -> native_c>(%321, copy<@type15, reason=arg>(read<@type15>(%325)))));
// DEFAULT-NEXT:         copy<@type15, reason=assign>(call<@type15, signature=fn(@type15) -> @type15, abi=sysv64(native_c) -> native_c>(%321, copy<@type15, reason=arg>(read<@type15>(%325))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%325)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %326 @fn2P(%327 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %328 y: @type15 [storage=automatic] = copy<@type15, reason=assign>(read<@type15>(%320));
// DEFAULT-NEXT:         let %764: u64 [synthetic] = read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%328));
// DEFAULT-NEXT:         let %765: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%764)))), read<u32>(%327)));
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%328), read<u64>(%765));
// DEFAULT-NEXT:         let %766: u64 [synthetic] = read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%328));
// DEFAULT-NEXT:         let %767: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%766))), const<i32>(15))));
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%328), read<u64>(%767));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%328)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %329 @retitP() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%320)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %330 @fn3P(%331 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %768: u64 [synthetic] = read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%320));
// DEFAULT-NEXT:         let %769: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%768)))), read<u32>(%331)));
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%320), read<u64>(%769));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%329);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %332 @testP() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %333 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %334 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %335 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %336 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %337 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %338 x: @type15 [storage=automatic];
// DEFAULT-NEXT:         let %339 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type15>>(%320));
// DEFAULT-NEXT:         for %567
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%333, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%333))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %770: i32 [synthetic] = read<i32>(%333);
// DEFAULT-NEXT:                 let %771: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%770), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%333, read<i32>(%771));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %772: ptr<i8> [synthetic] = read<ptr<i8>>(%339);
// DEFAULT-NEXT:                 let %773: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%772), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%339, read<ptr<i8>>(%773));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%772)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%320), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%320), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%334, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%320))))));
// DEFAULT-NEXT:         write<u32>(%335, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%336, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%320), widen<u64, reason=assign>(read<u32>(%335)));
// DEFAULT-NEXT:         write<@type15>(%338, copy<@type15, reason=assign>(read<@type15>(%320)));
// DEFAULT-NEXT:         write<u32>(%337, call<u32, signature=fn(u32) -> u32>(%323, read<u32>(%336)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%323, read<u32>(%336));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%338)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%320))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%338)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%320)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%338)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%320)))))), ne<u64>(read<u64>(field3(%338)), read<u64>(field3(%320)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%335), read<u32>(%336)), read<u32>(%334)), read<u32>(%337)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%335, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%336, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%320), widen<u64, reason=assign>(read<u32>(%335)));
// DEFAULT-NEXT:         write<@type15>(%338, copy<@type15, reason=assign>(read<@type15>(%320)));
// DEFAULT-NEXT:         write<u32>(%337, call<u32, signature=fn(u32) -> u32>(%326, read<u32>(%336)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%326, read<u32>(%336));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%338)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%320))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%338)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%320)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%338)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%320)))))), ne<u64>(read<u64>(field3(%338)), read<u64>(field3(%320)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%335), read<u32>(%336)), read<u32>(%334)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%334)), read<u32>(%337)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%335, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%336, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%320), widen<u64, reason=assign>(read<u32>(%335)));
// DEFAULT-NEXT:         write<@type15>(%338, copy<@type15, reason=assign>(read<@type15>(%320)));
// DEFAULT-NEXT:         write<u32>(%337, call<u32, signature=fn(u32) -> u32>(%330, read<u32>(%336)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%330, read<u32>(%336));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%338)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%320))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%338)))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%320)))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%320))))), read<u32>(%337))), ne<u64>(read<u64>(field3(%338)), read<u64>(field3(%320)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%335), read<u32>(%336)), read<u32>(%334)), read<u32>(%337)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %342 @retmeQ(%343 x: @type16) -> @type16 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type16, reason=return>(read<@type16>(%343));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %344 @fn1Q(%345 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %346 y: @type16 [storage=automatic] = copy<@type16, reason=assign>(read<@type16>(%341));
// DEFAULT-NEXT:         let %774: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%346));
// DEFAULT-NEXT:         let %775: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%774))), read<u32>(%345));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%346), read<u32>(%775));
// DEFAULT-NEXT:         write<@type16>(%346, copy<@type16, reason=assign>(call<@type16, signature=fn(@type16) -> @type16, abi=sysv64(native_c) -> native_c>(%342, copy<@type16, reason=arg>(read<@type16>(%346)))));
// DEFAULT-NEXT:         copy<@type16, reason=assign>(call<@type16, signature=fn(@type16) -> @type16, abi=sysv64(native_c) -> native_c>(%342, copy<@type16, reason=arg>(read<@type16>(%346))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%346))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %347 @fn2Q(%348 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %349 y: @type16 [storage=automatic] = copy<@type16, reason=assign>(read<@type16>(%341));
// DEFAULT-NEXT:         let %776: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%349));
// DEFAULT-NEXT:         let %777: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%776))), read<u32>(%348));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%349), read<u32>(%777));
// DEFAULT-NEXT:         let %778: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%349));
// DEFAULT-NEXT:         let %779: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%778)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%349), read<u32>(%779));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%349))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %350 @retitQ() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%341))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %351 @fn3Q(%352 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %780: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%341));
// DEFAULT-NEXT:         let %781: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%780))), read<u32>(%352));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%341), read<u32>(%781));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%350);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %353 @testQ() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %354 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %355 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %356 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %357 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %358 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %359 x: @type16 [storage=automatic];
// DEFAULT-NEXT:         let %360 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type16>>(%341));
// DEFAULT-NEXT:         for %568
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%354, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%354))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %782: i32 [synthetic] = read<i32>(%354);
// DEFAULT-NEXT:                 let %783: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%782), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%354, read<i32>(%783));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %784: ptr<i8> [synthetic] = read<ptr<i8>>(%360);
// DEFAULT-NEXT:                 let %785: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%784), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%360, read<ptr<i8>>(%785));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%784)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%341), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%341), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%355, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%341)))));
// DEFAULT-NEXT:         write<u32>(%356, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%357, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%341), read<u32>(%356));
// DEFAULT-NEXT:         write<@type16>(%359, copy<@type16, reason=assign>(read<@type16>(%341)));
// DEFAULT-NEXT:         write<u32>(%358, call<u32, signature=fn(u32) -> u32>(%344, read<u32>(%357)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%344, read<u32>(%357));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%359))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%341)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%359))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%341))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%359))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%341))))), ne<u64>(read<u64>(field3(%359)), read<u64>(field3(%341)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%356), read<u32>(%357)), read<u32>(%355)), read<u32>(%358)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%356, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%357, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%341), read<u32>(%356));
// DEFAULT-NEXT:         write<@type16>(%359, copy<@type16, reason=assign>(read<@type16>(%341)));
// DEFAULT-NEXT:         write<u32>(%358, call<u32, signature=fn(u32) -> u32>(%347, read<u32>(%357)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%347, read<u32>(%357));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%359))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%341)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%359))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%341))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%359))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%341))))), ne<u64>(read<u64>(field3(%359)), read<u64>(field3(%341)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%356), read<u32>(%357)), read<u32>(%355)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%355)), read<u32>(%358)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%356, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%357, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%341), read<u32>(%356));
// DEFAULT-NEXT:         write<@type16>(%359, copy<@type16, reason=assign>(read<@type16>(%341)));
// DEFAULT-NEXT:         write<u32>(%358, call<u32, signature=fn(u32) -> u32>(%351, read<u32>(%357)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%351, read<u32>(%357));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%359))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%341)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%359))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%341))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%341)))), read<u32>(%358))), ne<u64>(read<u64>(field3(%359)), read<u64>(field3(%341)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%356), read<u32>(%357)), read<u32>(%355)), read<u32>(%358)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %363 @retmeR(%364 x: @type17) -> @type17 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type17, reason=return>(read<@type17>(%364));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %365 @fn1R(%366 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %367 y: @type17 [storage=automatic] = copy<@type17, reason=assign>(read<@type17>(%362));
// DEFAULT-NEXT:         let %786: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%367));
// DEFAULT-NEXT:         let %787: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%786))), read<u32>(%366));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%367), read<u32>(%787));
// DEFAULT-NEXT:         write<@type17>(%367, copy<@type17, reason=assign>(call<@type17, signature=fn(@type17) -> @type17, abi=sysv64(native_c) -> native_c>(%363, copy<@type17, reason=arg>(read<@type17>(%367)))));
// DEFAULT-NEXT:         copy<@type17, reason=assign>(call<@type17, signature=fn(@type17) -> @type17, abi=sysv64(native_c) -> native_c>(%363, copy<@type17, reason=arg>(read<@type17>(%367))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%367))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %368 @fn2R(%369 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %370 y: @type17 [storage=automatic] = copy<@type17, reason=assign>(read<@type17>(%362));
// DEFAULT-NEXT:         let %788: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%370));
// DEFAULT-NEXT:         let %789: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%788))), read<u32>(%369));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%370), read<u32>(%789));
// DEFAULT-NEXT:         let %790: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%370));
// DEFAULT-NEXT:         let %791: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%790)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%370), read<u32>(%791));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%370))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %371 @retitR() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%362))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %372 @fn3R(%373 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %792: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%362));
// DEFAULT-NEXT:         let %793: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%792))), read<u32>(%373));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%362), read<u32>(%793));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%371);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %374 @testR() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %375 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %376 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %377 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %378 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %379 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %380 x: @type17 [storage=automatic];
// DEFAULT-NEXT:         let %381 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type17>>(%362));
// DEFAULT-NEXT:         for %569
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%375, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%375))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %794: i32 [synthetic] = read<i32>(%375);
// DEFAULT-NEXT:                 let %795: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%794), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%375, read<i32>(%795));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %796: ptr<i8> [synthetic] = read<ptr<i8>>(%381);
// DEFAULT-NEXT:                 let %797: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%796), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%381, read<ptr<i8>>(%797));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%796)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%362), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%362), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%376, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%362)))));
// DEFAULT-NEXT:         write<u32>(%377, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%378, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%362), read<u32>(%377));
// DEFAULT-NEXT:         write<@type17>(%380, copy<@type17, reason=assign>(read<@type17>(%362)));
// DEFAULT-NEXT:         write<u32>(%379, call<u32, signature=fn(u32) -> u32>(%365, read<u32>(%378)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%365, read<u32>(%378));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%380))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%362)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%380))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%362))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%380))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%362))))), ne<u64>(read<u64>(field3(%380)), read<u64>(field3(%362)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%377), read<u32>(%378)), read<u32>(%376)), read<u32>(%379)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%377, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%378, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%362), read<u32>(%377));
// DEFAULT-NEXT:         write<@type17>(%380, copy<@type17, reason=assign>(read<@type17>(%362)));
// DEFAULT-NEXT:         write<u32>(%379, call<u32, signature=fn(u32) -> u32>(%368, read<u32>(%378)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%368, read<u32>(%378));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%380))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%362)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%380))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%362))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%380))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%362))))), ne<u64>(read<u64>(field3(%380)), read<u64>(field3(%362)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%377), read<u32>(%378)), read<u32>(%376)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%376)), read<u32>(%379)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%377, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%378, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%362), read<u32>(%377));
// DEFAULT-NEXT:         write<@type17>(%380, copy<@type17, reason=assign>(read<@type17>(%362)));
// DEFAULT-NEXT:         write<u32>(%379, call<u32, signature=fn(u32) -> u32>(%372, read<u32>(%378)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%372, read<u32>(%378));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%380))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%362)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%380))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%362))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%362)))), read<u32>(%379))), ne<u64>(read<u64>(field3(%380)), read<u64>(field3(%362)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%377), read<u32>(%378)), read<u32>(%376)), read<u32>(%379)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %384 @retmeS(%385 x: @type18) -> @type18 [linkage=external] [abi=sysv64(coerce<i16, i64>) -> coerce<i16, i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type18, reason=return>(read<@type18>(%385));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %386 @fn1S(%387 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %388 y: @type18 [storage=automatic] = copy<@type18, reason=assign>(read<@type18>(%383));
// DEFAULT-NEXT:         let %798: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%388));
// DEFAULT-NEXT:         let %799: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%798)))), read<u32>(%387)));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%388), read<u16>(%799));
// DEFAULT-NEXT:         write<@type18>(%388, copy<@type18, reason=assign>(call<@type18, signature=fn(@type18) -> @type18, abi=sysv64(coerce<i16, i64>) -> coerce<i16, i64>>(%384, copy<@type18, reason=arg>(read<@type18>(%388)))));
// DEFAULT-NEXT:         copy<@type18, reason=assign>(call<@type18, signature=fn(@type18) -> @type18, abi=sysv64(coerce<i16, i64>) -> coerce<i16, i64>>(%384, copy<@type18, reason=arg>(read<@type18>(%388))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%388)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %389 @fn2S(%390 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %391 y: @type18 [storage=automatic] = copy<@type18, reason=assign>(read<@type18>(%383));
// DEFAULT-NEXT:         let %800: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%391));
// DEFAULT-NEXT:         let %801: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%800)))), read<u32>(%390)));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%391), read<u16>(%801));
// DEFAULT-NEXT:         let %802: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%391));
// DEFAULT-NEXT:         let %803: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%802))), const<i32>(15))));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%391), read<u16>(%803));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%391)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %392 @retitS() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%383)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %393 @fn3S(%394 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %804: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%383));
// DEFAULT-NEXT:         let %805: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%804)))), read<u32>(%394)));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%383), read<u16>(%805));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%392);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %395 @testS() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %396 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %397 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %398 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %399 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %400 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %401 x: @type18 [storage=automatic];
// DEFAULT-NEXT:         let %402 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type18>>(%383));
// DEFAULT-NEXT:         for %570
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%396, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%396))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %806: i32 [synthetic] = read<i32>(%396);
// DEFAULT-NEXT:                 let %807: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%806), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%396, read<i32>(%807));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %808: ptr<i8> [synthetic] = read<ptr<i8>>(%402);
// DEFAULT-NEXT:                 let %809: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%808), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%402, read<ptr<i8>>(%809));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%808)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%383), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%383), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%397, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%383))))));
// DEFAULT-NEXT:         write<u32>(%398, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%399, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%383), truncate<u16, reason=assign, fits=unknown>(read<u32>(%398)));
// DEFAULT-NEXT:         write<@type18>(%401, copy<@type18, reason=assign>(read<@type18>(%383)));
// DEFAULT-NEXT:         write<u32>(%400, call<u32, signature=fn(u32) -> u32>(%386, read<u32>(%399)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%386, read<u32>(%399));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%401)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%383))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%401)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%383)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%401)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%383)))))), ne<u64>(read<u64>(field3(%401)), read<u64>(field3(%383)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%398), read<u32>(%399)), read<u32>(%397)), read<u32>(%400)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%398, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%399, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%383), truncate<u16, reason=assign, fits=unknown>(read<u32>(%398)));
// DEFAULT-NEXT:         write<@type18>(%401, copy<@type18, reason=assign>(read<@type18>(%383)));
// DEFAULT-NEXT:         write<u32>(%400, call<u32, signature=fn(u32) -> u32>(%389, read<u32>(%399)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%389, read<u32>(%399));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%401)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%383))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%401)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%383)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%401)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%383)))))), ne<u64>(read<u64>(field3(%401)), read<u64>(field3(%383)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%398), read<u32>(%399)), read<u32>(%397)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%397)), read<u32>(%400)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%398, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%399, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%383), truncate<u16, reason=assign, fits=unknown>(read<u32>(%398)));
// DEFAULT-NEXT:         write<@type18>(%401, copy<@type18, reason=assign>(read<@type18>(%383)));
// DEFAULT-NEXT:         write<u32>(%400, call<u32, signature=fn(u32) -> u32>(%393, read<u32>(%399)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%393, read<u32>(%399));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%401)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%383))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%401)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%383)))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%383))))), read<u32>(%400))), ne<u64>(read<u64>(field3(%401)), read<u64>(field3(%383)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%398), read<u32>(%399)), read<u32>(%397)), read<u32>(%400)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %405 @retmeT(%406 x: @type19) -> @type19 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type19, reason=return>(read<@type19>(%406));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %407 @fn1T(%408 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %409 y: @type19 [storage=automatic] = copy<@type19, reason=assign>(read<@type19>(%404));
// DEFAULT-NEXT:         let %810: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%409));
// DEFAULT-NEXT:         let %811: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%810)))), read<u32>(%408)));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%409), read<u16>(%811));
// DEFAULT-NEXT:         write<@type19>(%409, copy<@type19, reason=assign>(call<@type19, signature=fn(@type19) -> @type19, abi=sysv64(native_c) -> native_c>(%405, copy<@type19, reason=arg>(read<@type19>(%409)))));
// DEFAULT-NEXT:         copy<@type19, reason=assign>(call<@type19, signature=fn(@type19) -> @type19, abi=sysv64(native_c) -> native_c>(%405, copy<@type19, reason=arg>(read<@type19>(%409))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%409)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %410 @fn2T(%411 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %412 y: @type19 [storage=automatic] = copy<@type19, reason=assign>(read<@type19>(%404));
// DEFAULT-NEXT:         let %812: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%412));
// DEFAULT-NEXT:         let %813: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%812)))), read<u32>(%411)));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%412), read<u16>(%813));
// DEFAULT-NEXT:         let %814: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%412));
// DEFAULT-NEXT:         let %815: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%814))), const<i32>(15))));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%412), read<u16>(%815));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%412)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %413 @retitT() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%404)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %414 @fn3T(%415 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %816: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%404));
// DEFAULT-NEXT:         let %817: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%816)))), read<u32>(%415)));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%404), read<u16>(%817));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%413);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %416 @testT() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %417 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %418 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %419 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %420 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %421 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %422 x: @type19 [storage=automatic];
// DEFAULT-NEXT:         let %423 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type19>>(%404));
// DEFAULT-NEXT:         for %571
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%417, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%417))), const<u64>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %818: i32 [synthetic] = read<i32>(%417);
// DEFAULT-NEXT:                 let %819: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%818), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%417, read<i32>(%819));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %820: ptr<i8> [synthetic] = read<ptr<i8>>(%423);
// DEFAULT-NEXT:                 let %821: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%820), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%423, read<ptr<i8>>(%821));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%820)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u16>(field3(%404), float_to_int<u16, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%404), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%418, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%404))))));
// DEFAULT-NEXT:         write<u32>(%419, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%420, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%404), truncate<u16, reason=assign, fits=unknown>(read<u32>(%419)));
// DEFAULT-NEXT:         write<@type19>(%422, copy<@type19, reason=assign>(read<@type19>(%404)));
// DEFAULT-NEXT:         write<u32>(%421, call<u32, signature=fn(u32) -> u32>(%407, read<u32>(%420)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%407, read<u32>(%420));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%422)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%404))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%422)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%404)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%422)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%404)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%422)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%404)))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%419), read<u32>(%420)), read<u32>(%418)), read<u32>(%421)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%419, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%420, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%404), truncate<u16, reason=assign, fits=unknown>(read<u32>(%419)));
// DEFAULT-NEXT:         write<@type19>(%422, copy<@type19, reason=assign>(read<@type19>(%404)));
// DEFAULT-NEXT:         write<u32>(%421, call<u32, signature=fn(u32) -> u32>(%410, read<u32>(%420)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%410, read<u32>(%420));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%422)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%404))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%422)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%404)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%422)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%404)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%422)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%404)))))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%419), read<u32>(%420)), read<u32>(%418)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%418)), read<u32>(%421)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%419, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%420, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%404), truncate<u16, reason=assign, fits=unknown>(read<u32>(%419)));
// DEFAULT-NEXT:         write<@type19>(%422, copy<@type19, reason=assign>(read<@type19>(%404)));
// DEFAULT-NEXT:         write<u32>(%421, call<u32, signature=fn(u32) -> u32>(%414, read<u32>(%420)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%414, read<u32>(%420));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%422)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%404))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%422)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%404)))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%404))))), read<u32>(%421))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%422)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%404)))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%419), read<u32>(%420)), read<u32>(%418)), read<u32>(%421)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %426 @retmeU(%427 x: @type20) -> @type20 [linkage=external] [abi=sysv64(coerce<i16, i64>) -> coerce<i16, i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type20, reason=return>(read<@type20>(%427));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %428 @fn1U(%429 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %430 y: @type20 [storage=automatic] = copy<@type20, reason=assign>(read<@type20>(%425));
// DEFAULT-NEXT:         let %822: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%430));
// DEFAULT-NEXT:         let %823: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%822)))), read<u32>(%429)));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%430), read<u16>(%823));
// DEFAULT-NEXT:         write<@type20>(%430, copy<@type20, reason=assign>(call<@type20, signature=fn(@type20) -> @type20, abi=sysv64(coerce<i16, i64>) -> coerce<i16, i64>>(%426, copy<@type20, reason=arg>(read<@type20>(%430)))));
// DEFAULT-NEXT:         copy<@type20, reason=assign>(call<@type20, signature=fn(@type20) -> @type20, abi=sysv64(coerce<i16, i64>) -> coerce<i16, i64>>(%426, copy<@type20, reason=arg>(read<@type20>(%430))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%430)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %431 @fn2U(%432 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %433 y: @type20 [storage=automatic] = copy<@type20, reason=assign>(read<@type20>(%425));
// DEFAULT-NEXT:         let %824: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%433));
// DEFAULT-NEXT:         let %825: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%824)))), read<u32>(%432)));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%433), read<u16>(%825));
// DEFAULT-NEXT:         let %826: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%433));
// DEFAULT-NEXT:         let %827: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%826))), const<i32>(15))));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%433), read<u16>(%827));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%433)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %434 @retitU() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%425)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %435 @fn3U(%436 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %828: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%425));
// DEFAULT-NEXT:         let %829: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%828)))), read<u32>(%436)));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%425), read<u16>(%829));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%434);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %437 @testU() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %438 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %439 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %440 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %441 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %442 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %443 x: @type20 [storage=automatic];
// DEFAULT-NEXT:         let %444 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type20>>(%425));
// DEFAULT-NEXT:         for %572
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%438, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%438))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %830: i32 [synthetic] = read<i32>(%438);
// DEFAULT-NEXT:                 let %831: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%830), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%438, read<i32>(%831));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %832: ptr<i8> [synthetic] = read<ptr<i8>>(%444);
// DEFAULT-NEXT:                 let %833: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%832), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%444, read<ptr<i8>>(%833));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%832)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%425), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%425), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%439, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%425))))));
// DEFAULT-NEXT:         write<u32>(%440, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%441, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%425), truncate<u16, reason=assign, fits=unknown>(read<u32>(%440)));
// DEFAULT-NEXT:         write<@type20>(%443, copy<@type20, reason=assign>(read<@type20>(%425)));
// DEFAULT-NEXT:         write<u32>(%442, call<u32, signature=fn(u32) -> u32>(%428, read<u32>(%441)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%428, read<u32>(%441));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%443)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%425))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..6>(%443)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..6>(%425)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%443)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%425)))))), ne<u64>(read<u64>(field3(%443)), read<u64>(field3(%425)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%440), read<u32>(%441)), read<u32>(%439)), read<u32>(%442)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%440, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%441, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%425), truncate<u16, reason=assign, fits=unknown>(read<u32>(%440)));
// DEFAULT-NEXT:         write<@type20>(%443, copy<@type20, reason=assign>(read<@type20>(%425)));
// DEFAULT-NEXT:         write<u32>(%442, call<u32, signature=fn(u32) -> u32>(%431, read<u32>(%441)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%431, read<u32>(%441));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%443)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%425))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..6>(%443)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..6>(%425)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%443)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%425)))))), ne<u64>(read<u64>(field3(%443)), read<u64>(field3(%425)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%440), read<u32>(%441)), read<u32>(%439)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%439)), read<u32>(%442)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%440, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%441, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%425), truncate<u16, reason=assign, fits=unknown>(read<u32>(%440)));
// DEFAULT-NEXT:         write<@type20>(%443, copy<@type20, reason=assign>(read<@type20>(%425)));
// DEFAULT-NEXT:         write<u32>(%442, call<u32, signature=fn(u32) -> u32>(%435, read<u32>(%441)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%435, read<u32>(%441));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%443)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%425))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..6>(%443)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..6>(%425)))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%425))))), read<u32>(%442))), ne<u64>(read<u64>(field3(%443)), read<u64>(field3(%425)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%440), read<u32>(%441)), read<u32>(%439)), read<u32>(%442)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %447 @retmeV(%448 x: @type21) -> @type21 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type21, reason=return>(read<@type21>(%448));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %449 @fn1V(%450 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %451 y: @type21 [storage=automatic] = copy<@type21, reason=assign>(read<@type21>(%446));
// DEFAULT-NEXT:         let %834: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%451));
// DEFAULT-NEXT:         let %835: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%834)))), read<u32>(%450)));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%451), read<u16>(%835));
// DEFAULT-NEXT:         write<@type21>(%451, copy<@type21, reason=assign>(call<@type21, signature=fn(@type21) -> @type21, abi=sysv64(native_c) -> native_c>(%447, copy<@type21, reason=arg>(read<@type21>(%451)))));
// DEFAULT-NEXT:         copy<@type21, reason=assign>(call<@type21, signature=fn(@type21) -> @type21, abi=sysv64(native_c) -> native_c>(%447, copy<@type21, reason=arg>(read<@type21>(%451))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%451)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %452 @fn2V(%453 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %454 y: @type21 [storage=automatic] = copy<@type21, reason=assign>(read<@type21>(%446));
// DEFAULT-NEXT:         let %836: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%454));
// DEFAULT-NEXT:         let %837: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%836)))), read<u32>(%453)));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%454), read<u16>(%837));
// DEFAULT-NEXT:         let %838: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%454));
// DEFAULT-NEXT:         let %839: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%838))), const<i32>(15))));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%454), read<u16>(%839));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%454)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %455 @retitV() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%446)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %456 @fn3V(%457 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %840: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%446));
// DEFAULT-NEXT:         let %841: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%840)))), read<u32>(%457)));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%446), read<u16>(%841));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%455);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %458 @testV() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %459 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %460 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %461 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %462 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %463 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %464 x: @type21 [storage=automatic];
// DEFAULT-NEXT:         let %465 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type21>>(%446));
// DEFAULT-NEXT:         for %573
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%459, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%459))), const<u64>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %842: i32 [synthetic] = read<i32>(%459);
// DEFAULT-NEXT:                 let %843: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%842), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%459, read<i32>(%843));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %844: ptr<i8> [synthetic] = read<ptr<i8>>(%465);
// DEFAULT-NEXT:                 let %845: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%844), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%465, read<ptr<i8>>(%845));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%844)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u16>(field3(%446), float_to_int<u16, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%446), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%460, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%446))))));
// DEFAULT-NEXT:         write<u32>(%461, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%462, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%446), truncate<u16, reason=assign, fits=unknown>(read<u32>(%461)));
// DEFAULT-NEXT:         write<@type21>(%464, copy<@type21, reason=assign>(read<@type21>(%446)));
// DEFAULT-NEXT:         write<u32>(%463, call<u32, signature=fn(u32) -> u32>(%449, read<u32>(%462)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%449, read<u32>(%462));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%464)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%446))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%464)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%446)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%464)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%446)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%464)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%446)))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%461), read<u32>(%462)), read<u32>(%460)), read<u32>(%463)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%461, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%462, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%446), truncate<u16, reason=assign, fits=unknown>(read<u32>(%461)));
// DEFAULT-NEXT:         write<@type21>(%464, copy<@type21, reason=assign>(read<@type21>(%446)));
// DEFAULT-NEXT:         write<u32>(%463, call<u32, signature=fn(u32) -> u32>(%452, read<u32>(%462)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%452, read<u32>(%462));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%464)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%446))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%464)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%446)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%464)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%446)))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%464)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%446)))))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%461), read<u32>(%462)), read<u32>(%460)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%460)), read<u32>(%463)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%461, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%462, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%446), truncate<u16, reason=assign, fits=unknown>(read<u32>(%461)));
// DEFAULT-NEXT:         write<@type21>(%464, copy<@type21, reason=assign>(read<@type21>(%446)));
// DEFAULT-NEXT:         write<u32>(%463, call<u32, signature=fn(u32) -> u32>(%456, read<u32>(%462)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%456, read<u32>(%462));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%464)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%446))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%464)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%446)))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%446))))), read<u32>(%463))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%464)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%446)))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%461), read<u32>(%462)), read<u32>(%460)), read<u32>(%463)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %468 @retmeW(%469 x: @type22) -> @type22 [linkage=external] [abi=sysv64(byval<align=16>) -> sret<align=16>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type22, reason=return>(read<@type22>(%469));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %470 @fn1W(%471 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %472 y: @type22 [storage=automatic] = copy<@type22, reason=assign>(read<@type22>(%467));
// DEFAULT-NEXT:         let %846: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%472));
// DEFAULT-NEXT:         let %847: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%846))), read<u32>(%471));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%472), read<u32>(%847));
// DEFAULT-NEXT:         write<@type22>(%472, copy<@type22, reason=assign>(call<@type22, signature=fn(@type22) -> @type22, abi=sysv64(byval<align=16>) -> sret<align=16>>(%468, copy<@type22, reason=arg>(read<@type22>(%472)))));
// DEFAULT-NEXT:         copy<@type22, reason=assign>(call<@type22, signature=fn(@type22) -> @type22, abi=sysv64(byval<align=16>) -> sret<align=16>>(%468, copy<@type22, reason=arg>(read<@type22>(%472))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%472))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %473 @fn2W(%474 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %475 y: @type22 [storage=automatic] = copy<@type22, reason=assign>(read<@type22>(%467));
// DEFAULT-NEXT:         let %848: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%475));
// DEFAULT-NEXT:         let %849: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%848))), read<u32>(%474));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%475), read<u32>(%849));
// DEFAULT-NEXT:         let %850: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%475));
// DEFAULT-NEXT:         let %851: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%850)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%475), read<u32>(%851));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%475))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %476 @retitW() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%467))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %477 @fn3W(%478 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %852: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%467));
// DEFAULT-NEXT:         let %853: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%852))), read<u32>(%478));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%467), read<u32>(%853));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%476);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %479 @testW() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %480 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %481 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %482 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %483 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %484 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %485 x: @type22 [storage=automatic];
// DEFAULT-NEXT:         let %486 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type22>>(%467));
// DEFAULT-NEXT:         for %574
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%480, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%480))), const<u64>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %854: i32 [synthetic] = read<i32>(%480);
// DEFAULT-NEXT:                 let %855: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%854), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%480, read<i32>(%855));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %856: ptr<i8> [synthetic] = read<ptr<i8>>(%486);
// DEFAULT-NEXT:                 let %857: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%856), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%486, read<ptr<i8>>(%857));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%856)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(8), const<i32>(8))
// DEFAULT-NEXT:             write<f80>(field0(%467), float_widen<f80, reason=assign>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%467), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%481, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%467)))));
// DEFAULT-NEXT:         write<u32>(%482, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%483, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%467), read<u32>(%482));
// DEFAULT-NEXT:         write<@type22>(%485, copy<@type22, reason=assign>(read<@type22>(%467)));
// DEFAULT-NEXT:         write<u32>(%484, call<u32, signature=fn(u32) -> u32>(%470, read<u32>(%483)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%470, read<u32>(%483));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=25..32>(%485))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=25..32>(%467)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=12..25>(%485))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=12..25>(%467))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%485))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%467))))), ne<f80, exceptions=ignore>(read<f80>(field0(%485)), read<f80>(field0(%467)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%482), read<u32>(%483)), read<u32>(%481)), read<u32>(%484)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%482, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%483, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%467), read<u32>(%482));
// DEFAULT-NEXT:         write<@type22>(%485, copy<@type22, reason=assign>(read<@type22>(%467)));
// DEFAULT-NEXT:         write<u32>(%484, call<u32, signature=fn(u32) -> u32>(%473, read<u32>(%483)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%473, read<u32>(%483));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=25..32>(%485))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=25..32>(%467)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=12..25>(%485))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=12..25>(%467))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%485))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%467))))), ne<f80, exceptions=ignore>(read<f80>(field0(%485)), read<f80>(field0(%467)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%482), read<u32>(%483)), read<u32>(%481)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%481)), read<u32>(%484)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%482, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%483, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%467), read<u32>(%482));
// DEFAULT-NEXT:         write<@type22>(%485, copy<@type22, reason=assign>(read<@type22>(%467)));
// DEFAULT-NEXT:         write<u32>(%484, call<u32, signature=fn(u32) -> u32>(%477, read<u32>(%483)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%477, read<u32>(%483));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=25..32>(%485))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=25..32>(%467)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=12..25>(%485))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=12..25>(%467))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%467)))), read<u32>(%484))), ne<f80, exceptions=ignore>(read<f80>(field0(%485)), read<f80>(field0(%467)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%482), read<u32>(%483)), read<u32>(%481)), read<u32>(%484)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %489 @retmeX(%490 x: @type23) -> @type23 [linkage=external] [abi=sysv64(byval<align=16>) -> sret<align=16>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type23, reason=return>(read<@type23>(%490));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %491 @fn1X(%492 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %493 y: @type23 [storage=automatic] = copy<@type23, reason=assign>(read<@type23>(%488));
// DEFAULT-NEXT:         let %858: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%493));
// DEFAULT-NEXT:         let %859: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%858))), read<u32>(%492));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%493), read<u32>(%859));
// DEFAULT-NEXT:         write<@type23>(%493, copy<@type23, reason=assign>(call<@type23, signature=fn(@type23) -> @type23, abi=sysv64(byval<align=16>) -> sret<align=16>>(%489, copy<@type23, reason=arg>(read<@type23>(%493)))));
// DEFAULT-NEXT:         copy<@type23, reason=assign>(call<@type23, signature=fn(@type23) -> @type23, abi=sysv64(byval<align=16>) -> sret<align=16>>(%489, copy<@type23, reason=arg>(read<@type23>(%493))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%493))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %494 @fn2X(%495 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %496 y: @type23 [storage=automatic] = copy<@type23, reason=assign>(read<@type23>(%488));
// DEFAULT-NEXT:         let %860: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%496));
// DEFAULT-NEXT:         let %861: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%860))), read<u32>(%495));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%496), read<u32>(%861));
// DEFAULT-NEXT:         let %862: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%496));
// DEFAULT-NEXT:         let %863: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%862)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%496), read<u32>(%863));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%496))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %497 @retitX() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%488))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %498 @fn3X(%499 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %864: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%488));
// DEFAULT-NEXT:         let %865: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%864))), read<u32>(%499));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%488), read<u32>(%865));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%497);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %500 @testX() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %501 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %502 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %503 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %504 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %505 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %506 x: @type23 [storage=automatic];
// DEFAULT-NEXT:         let %507 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type23>>(%488));
// DEFAULT-NEXT:         for %575
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%501, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%501))), const<u64>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %866: i32 [synthetic] = read<i32>(%501);
// DEFAULT-NEXT:                 let %867: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%866), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%501, read<i32>(%867));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %868: ptr<i8> [synthetic] = read<ptr<i8>>(%507);
// DEFAULT-NEXT:                 let %869: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%868), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%507, read<ptr<i8>>(%869));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%868)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(8), const<i32>(8))
// DEFAULT-NEXT:             write<f80>(field3(%488), float_widen<f80, reason=assign>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%488), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%502, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%488)))));
// DEFAULT-NEXT:         write<u32>(%503, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%504, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%488), read<u32>(%503));
// DEFAULT-NEXT:         write<@type23>(%506, copy<@type23, reason=assign>(read<@type23>(%488)));
// DEFAULT-NEXT:         write<u32>(%505, call<u32, signature=fn(u32) -> u32>(%491, read<u32>(%504)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%491, read<u32>(%504));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%506))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%488)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%506))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%488))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%506))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%488))))), ne<f80, exceptions=ignore>(read<f80>(field3(%506)), read<f80>(field3(%488)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%503), read<u32>(%504)), read<u32>(%502)), read<u32>(%505)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%503, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%504, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%488), read<u32>(%503));
// DEFAULT-NEXT:         write<@type23>(%506, copy<@type23, reason=assign>(read<@type23>(%488)));
// DEFAULT-NEXT:         write<u32>(%505, call<u32, signature=fn(u32) -> u32>(%494, read<u32>(%504)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%494, read<u32>(%504));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%506))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%488)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%506))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%488))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%506))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%488))))), ne<f80, exceptions=ignore>(read<f80>(field3(%506)), read<f80>(field3(%488)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%503), read<u32>(%504)), read<u32>(%502)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%502)), read<u32>(%505)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%503, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%504, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%488), read<u32>(%503));
// DEFAULT-NEXT:         write<@type23>(%506, copy<@type23, reason=assign>(read<@type23>(%488)));
// DEFAULT-NEXT:         write<u32>(%505, call<u32, signature=fn(u32) -> u32>(%498, read<u32>(%504)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%498, read<u32>(%504));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%506))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%488)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%506))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%488))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%488)))), read<u32>(%505))), ne<f80, exceptions=ignore>(read<f80>(field3(%506)), read<f80>(field3(%488)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%503), read<u32>(%504)), read<u32>(%502)), read<u32>(%505)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %510 @retmeY(%511 x: @type24) -> @type24 [linkage=external] [abi=sysv64(byval<align=16>) -> sret<align=16>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type24, reason=return>(read<@type24>(%511));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %512 @fn1Y(%513 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %514 y: @type24 [storage=automatic] = copy<@type24, reason=assign>(read<@type24>(%509));
// DEFAULT-NEXT:         let %870: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%514));
// DEFAULT-NEXT:         let %871: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%870))), read<u32>(%513));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%514), read<u32>(%871));
// DEFAULT-NEXT:         write<@type24>(%514, copy<@type24, reason=assign>(call<@type24, signature=fn(@type24) -> @type24, abi=sysv64(byval<align=16>) -> sret<align=16>>(%510, copy<@type24, reason=arg>(read<@type24>(%514)))));
// DEFAULT-NEXT:         copy<@type24, reason=assign>(call<@type24, signature=fn(@type24) -> @type24, abi=sysv64(byval<align=16>) -> sret<align=16>>(%510, copy<@type24, reason=arg>(read<@type24>(%514))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%514))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %515 @fn2Y(%516 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %517 y: @type24 [storage=automatic] = copy<@type24, reason=assign>(read<@type24>(%509));
// DEFAULT-NEXT:         let %872: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%517));
// DEFAULT-NEXT:         let %873: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%872))), read<u32>(%516));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%517), read<u32>(%873));
// DEFAULT-NEXT:         let %874: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%517));
// DEFAULT-NEXT:         let %875: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%874)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%517), read<u32>(%875));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%517))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %518 @retitY() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%509))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %519 @fn3Y(%520 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %876: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%509));
// DEFAULT-NEXT:         let %877: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%876))), read<u32>(%520));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%509), read<u32>(%877));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%518);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %521 @testY() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %522 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %523 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %524 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %525 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %526 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %527 x: @type24 [storage=automatic];
// DEFAULT-NEXT:         let %528 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type24>>(%509));
// DEFAULT-NEXT:         for %576
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%522, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%522))), const<u64>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %878: i32 [synthetic] = read<i32>(%522);
// DEFAULT-NEXT:                 let %879: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%878), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%522, read<i32>(%879));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %880: ptr<i8> [synthetic] = read<ptr<i8>>(%528);
// DEFAULT-NEXT:                 let %881: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%880), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%528, read<ptr<i8>>(%881));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%880)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(8), const<i32>(8))
// DEFAULT-NEXT:             write<f80>(field3(%509), float_widen<f80, reason=assign>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%509), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%523, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%509)))));
// DEFAULT-NEXT:         write<u32>(%524, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%525, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%509), read<u32>(%524));
// DEFAULT-NEXT:         write<@type24>(%527, copy<@type24, reason=assign>(read<@type24>(%509)));
// DEFAULT-NEXT:         write<u32>(%526, call<u32, signature=fn(u32) -> u32>(%512, read<u32>(%525)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%512, read<u32>(%525));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%527))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%509)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%527))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%509))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%527))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%509))))), ne<f80, exceptions=ignore>(read<f80>(field3(%527)), read<f80>(field3(%509)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%524), read<u32>(%525)), read<u32>(%523)), read<u32>(%526)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%524, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%525, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%509), read<u32>(%524));
// DEFAULT-NEXT:         write<@type24>(%527, copy<@type24, reason=assign>(read<@type24>(%509)));
// DEFAULT-NEXT:         write<u32>(%526, call<u32, signature=fn(u32) -> u32>(%515, read<u32>(%525)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%515, read<u32>(%525));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%527))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%509)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%527))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%509))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%527))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%509))))), ne<f80, exceptions=ignore>(read<f80>(field3(%527)), read<f80>(field3(%509)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%524), read<u32>(%525)), read<u32>(%523)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%523)), read<u32>(%526)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%524, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%525, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%509), read<u32>(%524));
// DEFAULT-NEXT:         write<@type24>(%527, copy<@type24, reason=assign>(read<@type24>(%509)));
// DEFAULT-NEXT:         write<u32>(%526, call<u32, signature=fn(u32) -> u32>(%519, read<u32>(%525)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%519, read<u32>(%525));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%527))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%509)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%527))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%509))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%509)))), read<u32>(%526))), ne<f80, exceptions=ignore>(read<f80>(field3(%527)), read<f80>(field3(%509)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%524), read<u32>(%525)), read<u32>(%523)), read<u32>(%526)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %531 @retmeZ(%532 x: @type25) -> @type25 [linkage=external] [abi=sysv64(byval<align=16>) -> sret<align=16>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type25, reason=return>(read<@type25>(%532));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %533 @fn1Z(%534 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %535 y: @type25 [storage=automatic] = copy<@type25, reason=assign>(read<@type25>(%530));
// DEFAULT-NEXT:         let %882: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%535));
// DEFAULT-NEXT:         let %883: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%882))), read<u32>(%534));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%535), read<u32>(%883));
// DEFAULT-NEXT:         write<@type25>(%535, copy<@type25, reason=assign>(call<@type25, signature=fn(@type25) -> @type25, abi=sysv64(byval<align=16>) -> sret<align=16>>(%531, copy<@type25, reason=arg>(read<@type25>(%535)))));
// DEFAULT-NEXT:         copy<@type25, reason=assign>(call<@type25, signature=fn(@type25) -> @type25, abi=sysv64(byval<align=16>) -> sret<align=16>>(%531, copy<@type25, reason=arg>(read<@type25>(%535))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%535))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %536 @fn2Z(%537 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %538 y: @type25 [storage=automatic] = copy<@type25, reason=assign>(read<@type25>(%530));
// DEFAULT-NEXT:         let %884: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%538));
// DEFAULT-NEXT:         let %885: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%884))), read<u32>(%537));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%538), read<u32>(%885));
// DEFAULT-NEXT:         let %886: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%538));
// DEFAULT-NEXT:         let %887: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%886)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%538), read<u32>(%887));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%538))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %539 @retitZ() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%530))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %540 @fn3Z(%541 x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %888: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%530));
// DEFAULT-NEXT:         let %889: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%888))), read<u32>(%541));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%530), read<u32>(%889));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%539);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %542 @testZ() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %543 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %544 mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %545 v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %546 a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %547 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %548 x: @type25 [storage=automatic];
// DEFAULT-NEXT:         let %549 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type25>>(%530));
// DEFAULT-NEXT:         for %577
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%543, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%543))), const<u64>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %890: i32 [synthetic] = read<i32>(%543);
// DEFAULT-NEXT:                 let %891: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%890), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%543, read<i32>(%891));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %892: ptr<i8> [synthetic] = read<ptr<i8>>(%549);
// DEFAULT-NEXT:                 let %893: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%892), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%549, read<ptr<i8>>(%893));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%892)), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2))));
// DEFAULT-NEXT:                 reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%2)));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(8), const<i32>(8))
// DEFAULT-NEXT:             write<f80>(field0(%530), float_widen<f80, reason=assign>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%530), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%544, reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%530)))));
// DEFAULT-NEXT:         write<u32>(%545, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%546, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%530), read<u32>(%545));
// DEFAULT-NEXT:         write<@type25>(%548, copy<@type25, reason=assign>(read<@type25>(%530)));
// DEFAULT-NEXT:         write<u32>(%547, call<u32, signature=fn(u32) -> u32>(%533, read<u32>(%546)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%533, read<u32>(%546));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=13..20>(%548))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=13..20>(%530)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..13>(%548))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..13>(%530))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%548))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%530))))), ne<f80, exceptions=ignore>(read<f80>(field0(%548)), read<f80>(field0(%530)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%545), read<u32>(%546)), read<u32>(%544)), read<u32>(%547)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%545, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%546, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%530), read<u32>(%545));
// DEFAULT-NEXT:         write<@type25>(%548, copy<@type25, reason=assign>(read<@type25>(%530)));
// DEFAULT-NEXT:         write<u32>(%547, call<u32, signature=fn(u32) -> u32>(%536, read<u32>(%546)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%536, read<u32>(%546));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=13..20>(%548))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=13..20>(%530)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..13>(%548))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..13>(%530))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%548))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%530))))), ne<f80, exceptions=ignore>(read<f80>(field0(%548)), read<f80>(field0(%530)))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%545), read<u32>(%546)), read<u32>(%544)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%544)), read<u32>(%547)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(%545, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(%546, call<u32, signature=fn() -> u32>(%2));
// DEFAULT-NEXT:         call<u32, signature=fn() -> u32>(%2);
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%530), read<u32>(%545));
// DEFAULT-NEXT:         write<@type25>(%548, copy<@type25, reason=assign>(read<@type25>(%530)));
// DEFAULT-NEXT:         write<u32>(%547, call<u32, signature=fn(u32) -> u32>(%540, read<u32>(%546)));
// DEFAULT-NEXT:         call<u32, signature=fn(u32) -> u32>(%540, read<u32>(%546));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=13..20>(%548))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=13..20>(%530)))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..13>(%548))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..13>(%530))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%530)))), read<u32>(%547))), ne<f80, exceptions=ignore>(read<f80>(field0(%548)), read<f80>(field0(%530)))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%545), read<u32>(%546)), read<u32>(%544)), read<u32>(%547)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %550 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%59);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%80);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%101);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%122);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%164);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%185);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%206);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%227);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%248);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%269);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%290);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%311);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%332);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%353);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%374);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%395);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%416);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%437);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%458);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%479);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%500);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%521);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%542);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
