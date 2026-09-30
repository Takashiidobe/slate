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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 i: u32 : 6;
// DEFAULT-NEXT:         field1 l: u32 : 1;
// DEFAULT-NEXT:         field2 j: u32 : 10;
// DEFAULT-NEXT:         field3 k: u32 : 15;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0, 2], bit_offsets=[Some(0), Some(6), Some(7), Some(17)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 i: u32 : 6;
// DEFAULT-NEXT:         field1 j: u32 : 11;
// DEFAULT-NEXT:         field2 k: u32 : 15;
// DEFAULT-NEXT:         field3 l: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0, 2, 4], bit_offsets=[Some(0), Some(6), Some(17), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_C:[0-9]+]] C = struct {
// DEFAULT-NEXT:         field0 l: u32;
// DEFAULT-NEXT:         field1 i: u32 : 6;
// DEFAULT-NEXT:         field2 j: u32 : 11;
// DEFAULT-NEXT:         field3 k: u32 : 15;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4, 4, 6], bit_offsets=[None, Some(32), Some(38), Some(49)], bit_units=[(4, 4)], field_units=[None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_D:[0-9]+]] D = struct {
// DEFAULT-NEXT:         field0 l: u64 : 6;
// DEFAULT-NEXT:         field1 i: u64 : 6;
// DEFAULT-NEXT:         field2 j: u64 : 23;
// DEFAULT-NEXT:         field3 k: u64 : 29;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 1, 4], bit_offsets=[Some(0), Some(6), Some(12), Some(35)], bit_units=[(0, 8)], field_units=[Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_E:[0-9]+]] E = struct {
// DEFAULT-NEXT:         field0 l: u64;
// DEFAULT-NEXT:         field1 i: u64 : 12;
// DEFAULT-NEXT:         field2 j: u64 : 23;
// DEFAULT-NEXT:         field3 k: u64 : 29;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 9, 12], bit_offsets=[None, Some(64), Some(76), Some(99)], bit_units=[(8, 8)], field_units=[None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_F:[0-9]+]] F = struct {
// DEFAULT-NEXT:         field0 i: u64 : 12;
// DEFAULT-NEXT:         field1 j: u64 : 23;
// DEFAULT-NEXT:         field2 k: u64 : 29;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 1, 4, 8], bit_offsets=[Some(0), Some(12), Some(35), None], bit_units=[(0, 8)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_G:[0-9]+]] G = struct {
// DEFAULT-NEXT:         field0 i: u32 : 12;
// DEFAULT-NEXT:         field1 j: u32 : 13;
// DEFAULT-NEXT:         field2 k: u32 : 7;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 1, 3, 8], bit_offsets=[Some(0), Some(12), Some(25), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_H:[0-9]+]] H = struct {
// DEFAULT-NEXT:         field0 i: u32 : 12;
// DEFAULT-NEXT:         field1 j: u32 : 11;
// DEFAULT-NEXT:         field2 k: u32 : 9;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 1, 2, 8], bit_offsets=[Some(0), Some(12), Some(23), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_I:[0-9]+]] I = struct {
// DEFAULT-NEXT:         field0 i: u16 : 1;
// DEFAULT-NEXT:         field1 j: u16 : 6;
// DEFAULT-NEXT:         field2 k: u16 : 9;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0, 0, 8], bit_offsets=[Some(0), Some(1), Some(7), None], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_J:[0-9]+]] J = struct {
// DEFAULT-NEXT:         field0 i: u16 : 1;
// DEFAULT-NEXT:         field1 j: u16 : 8;
// DEFAULT-NEXT:         field2 k: u16 : 7;
// DEFAULT-NEXT:         field3 l: u16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 0, 1, 2], bit_offsets=[Some(0), Some(1), Some(9), None], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_K:[0-9]+]] K = struct {
// DEFAULT-NEXT:         field0 k: u32 : 6;
// DEFAULT-NEXT:         field1 l: u32 : 1;
// DEFAULT-NEXT:         field2 j: u32 : 10;
// DEFAULT-NEXT:         field3 i: u32 : 15;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0, 2], bit_offsets=[Some(0), Some(6), Some(7), Some(17)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_L:[0-9]+]] L = struct {
// DEFAULT-NEXT:         field0 k: u32 : 6;
// DEFAULT-NEXT:         field1 j: u32 : 11;
// DEFAULT-NEXT:         field2 i: u32 : 15;
// DEFAULT-NEXT:         field3 l: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0, 2, 4], bit_offsets=[Some(0), Some(6), Some(17), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_M:[0-9]+]] M = struct {
// DEFAULT-NEXT:         field0 l: u32;
// DEFAULT-NEXT:         field1 k: u32 : 6;
// DEFAULT-NEXT:         field2 j: u32 : 11;
// DEFAULT-NEXT:         field3 i: u32 : 15;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4, 4, 6], bit_offsets=[None, Some(32), Some(38), Some(49)], bit_units=[(4, 4)], field_units=[None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_N:[0-9]+]] N = struct {
// DEFAULT-NEXT:         field0 l: u64 : 6;
// DEFAULT-NEXT:         field1 k: u64 : 6;
// DEFAULT-NEXT:         field2 j: u64 : 23;
// DEFAULT-NEXT:         field3 i: u64 : 29;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 1, 4], bit_offsets=[Some(0), Some(6), Some(12), Some(35)], bit_units=[(0, 8)], field_units=[Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_O:[0-9]+]] O = struct {
// DEFAULT-NEXT:         field0 l: u64;
// DEFAULT-NEXT:         field1 k: u64 : 12;
// DEFAULT-NEXT:         field2 j: u64 : 23;
// DEFAULT-NEXT:         field3 i: u64 : 29;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 9, 12], bit_offsets=[None, Some(64), Some(76), Some(99)], bit_units=[(8, 8)], field_units=[None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_P:[0-9]+]] P = struct {
// DEFAULT-NEXT:         field0 k: u64 : 12;
// DEFAULT-NEXT:         field1 j: u64 : 23;
// DEFAULT-NEXT:         field2 i: u64 : 29;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 1, 4, 8], bit_offsets=[Some(0), Some(12), Some(35), None], bit_units=[(0, 8)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_Q:[0-9]+]] Q = struct {
// DEFAULT-NEXT:         field0 k: u32 : 12;
// DEFAULT-NEXT:         field1 j: u32 : 13;
// DEFAULT-NEXT:         field2 i: u32 : 7;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 1, 3, 8], bit_offsets=[Some(0), Some(12), Some(25), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_R:[0-9]+]] R = struct {
// DEFAULT-NEXT:         field0 k: u32 : 12;
// DEFAULT-NEXT:         field1 j: u32 : 11;
// DEFAULT-NEXT:         field2 i: u32 : 9;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 1, 2, 8], bit_offsets=[Some(0), Some(12), Some(23), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 k: u16 : 1;
// DEFAULT-NEXT:         field1 j: u16 : 6;
// DEFAULT-NEXT:         field2 i: u16 : 9;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0, 0, 8], bit_offsets=[Some(0), Some(1), Some(7), None], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = struct {
// DEFAULT-NEXT:         field0 k: u16 : 1;
// DEFAULT-NEXT:         field1 j: u16 : 8;
// DEFAULT-NEXT:         field2 i: u16 : 7;
// DEFAULT-NEXT:         field3 l: u16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 0, 1, 2], bit_offsets=[Some(0), Some(1), Some(9), None], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = struct {
// DEFAULT-NEXT:         field0 j: u16 : 6;
// DEFAULT-NEXT:         field1 k: u16 : 1;
// DEFAULT-NEXT:         field2 i: u16 : 9;
// DEFAULT-NEXT:         field3 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0, 0, 8], bit_offsets=[Some(0), Some(6), Some(7), None], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_V:[0-9]+]] V = struct {
// DEFAULT-NEXT:         field0 j: u16 : 8;
// DEFAULT-NEXT:         field1 k: u16 : 1;
// DEFAULT-NEXT:         field2 i: u16 : 7;
// DEFAULT-NEXT:         field3 l: u16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 1, 1, 2], bit_offsets=[Some(0), Some(8), Some(9), None], bit_units=[(0, 2)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_W:[0-9]+]] W = struct {
// DEFAULT-NEXT:         field0 l: f80;
// DEFAULT-NEXT:         field1 k: u32 : 12;
// DEFAULT-NEXT:         field2 j: u32 : 13;
// DEFAULT-NEXT:         field3 i: u32 : 7;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16, 17, 19], bit_offsets=[None, Some(128), Some(140), Some(153)], bit_units=[(16, 4)], field_units=[None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_X:[0-9]+]] X = struct {
// DEFAULT-NEXT:         field0 k: u32 : 12;
// DEFAULT-NEXT:         field1 j: u32 : 13;
// DEFAULT-NEXT:         field2 i: u32 : 7;
// DEFAULT-NEXT:         field3 l: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 1, 3, 16], bit_offsets=[Some(0), Some(12), Some(25), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_Y:[0-9]+]] Y = struct {
// DEFAULT-NEXT:         field0 k: u32 : 12;
// DEFAULT-NEXT:         field1 j: u32 : 11;
// DEFAULT-NEXT:         field2 i: u32 : 9;
// DEFAULT-NEXT:         field3 l: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 1, 2, 16], bit_offsets=[Some(0), Some(12), Some(23), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_Z:[0-9]+]] Z = struct {
// DEFAULT-NEXT:         field0 l: f80;
// DEFAULT-NEXT:         field1 j: u32 : 13;
// DEFAULT-NEXT:         field2 i: u32 : 7;
// DEFAULT-NEXT:         field3 k: u32 : 12;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16, 17, 18], bit_offsets=[None, Some(128), Some(141), Some(148)], bit_units=[(16, 4)], field_units=[None, Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(1388815473)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_sA:[0-9]+]] sA: @type[[TYPE_A]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sB:[0-9]+]] sB: @type[[TYPE_B]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sC:[0-9]+]] sC: @type[[TYPE_C]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sD:[0-9]+]] sD: @type[[TYPE_D]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sE:[0-9]+]] sE: @type[[TYPE_E]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sF:[0-9]+]] sF: @type[[TYPE_F]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sG:[0-9]+]] sG: @type[[TYPE_G]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sH:[0-9]+]] sH: @type[[TYPE_H]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sI:[0-9]+]] sI: @type[[TYPE_I]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sJ:[0-9]+]] sJ: @type[[TYPE_J]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sK:[0-9]+]] sK: @type[[TYPE_K]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sL:[0-9]+]] sL: @type[[TYPE_L]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sM:[0-9]+]] sM: @type[[TYPE_M]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sN:[0-9]+]] sN: @type[[TYPE_N]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sO:[0-9]+]] sO: @type[[TYPE_O]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sP:[0-9]+]] sP: @type[[TYPE_P]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sQ:[0-9]+]] sQ: @type[[TYPE_Q]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sR:[0-9]+]] sR: @type[[TYPE_R]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sS:[0-9]+]] sS: @type[[TYPE_S]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sT:[0-9]+]] sT: @type[[TYPE_T]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sU:[0-9]+]] sU: @type[[TYPE_U]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sV:[0-9]+]] sV: @type[[TYPE_V]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sW:[0-9]+]] sW: @type[[TYPE_W]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sX:[0-9]+]] sX: @type[[TYPE_X]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sY:[0-9]+]] sY: @type[[TYPE_Y]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sZ:[0-9]+]] sZ: @type[[TYPE_Z]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_myrnd:[0-9]+]] @myrnd() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_s]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u32 [synthetic] = mul<u32, overflow=wrap>(read<u32>(%[[VALUE1]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1103515245)));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_s]], read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_s]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE3]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(12345)));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_s]], read<u32>(%[[VALUE4]]));
// DEFAULT-NEXT:         return rem<u32, by_zero=ub>(div<u32, by_zero=ub>(read<u32>(%[[VALUE_s]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(65536))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2048)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeA:[0-9]+]] @retmeA(%[[VALUE_x:[0-9]+]] x: @type[[TYPE_A]]) -> @type[[TYPE_A]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_A]], reason=return>(read<@type[[TYPE_A]]>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1A:[0-9]+]] @fn1A(%[[VALUE_x_2:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: @type[[TYPE_A]] [storage=automatic] = copy<@type[[TYPE_A]], reason=assign>(read<@type[[TYPE_A]]>(%[[VALUE_sA]]));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE5]]))), read<u32>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y]]), read<u32>(%[[VALUE6]]));
// DEFAULT-NEXT:         write<@type[[TYPE_A]]>(%[[VALUE_y]], copy<@type[[TYPE_A]], reason=assign>(call<@type[[TYPE_A]], signature=fn(@type[[TYPE_A]]) -> @type[[TYPE_A]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeA]], copy<@type[[TYPE_A]], reason=arg>(read<@type[[TYPE_A]]>(%[[VALUE_y]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2A:[0-9]+]] @fn2A(%[[VALUE_x_3:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: @type[[TYPE_A]] [storage=automatic] = copy<@type[[TYPE_A]], reason=assign>(read<@type[[TYPE_A]]>(%[[VALUE_sA]]));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE7]]))), read<u32>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y_2]]), read<u32>(%[[VALUE8]]));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE9]])), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y_2]]), read<u32>(%[[VALUE10]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitA:[0-9]+]] @retitA() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sA]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3A:[0-9]+]] @fn3A(%[[VALUE_x_4:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sA]]));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE11]]))), read<u32>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sA]]), read<u32>(%[[VALUE12]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitA]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testA:[0-9]+]] @testA() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_5:[0-9]+]] x: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_sA]]));
// DEFAULT-NEXT:         for %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), const<u64>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p]], read<ptr<i8>>(%[[VALUE17]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE16]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%[[VALUE_sA]]), float_to_int<u32, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sA]]), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sA]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sA]]), read<u32>(%[[VALUE_v]]));
// DEFAULT-NEXT:         write<@type[[TYPE_A]]>(%[[VALUE_x_5]], copy<@type[[TYPE_A]], reason=assign>(read<@type[[TYPE_A]]>(%[[VALUE_sA]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1A]], read<u32>(%[[VALUE_a]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_x_5]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sA]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%[[VALUE_x_5]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%[[VALUE_sA]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_x_5]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sA]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%[[VALUE_x_5]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%[[VALUE_sA]]))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v]]), read<u32>(%[[VALUE_a]])), read<u32>(%[[VALUE_mask]])), read<u32>(%[[VALUE_r]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sA]]), read<u32>(%[[VALUE_v]]));
// DEFAULT-NEXT:         write<@type[[TYPE_A]]>(%[[VALUE_x_5]], copy<@type[[TYPE_A]], reason=assign>(read<@type[[TYPE_A]]>(%[[VALUE_sA]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2A]], read<u32>(%[[VALUE_a]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_x_5]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sA]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%[[VALUE_x_5]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%[[VALUE_sA]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_x_5]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sA]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%[[VALUE_x_5]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%[[VALUE_sA]]))))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v]]), read<u32>(%[[VALUE_a]])), read<u32>(%[[VALUE_mask]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask]])), read<u32>(%[[VALUE_r]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sA]]), read<u32>(%[[VALUE_v]]));
// DEFAULT-NEXT:         write<@type[[TYPE_A]]>(%[[VALUE_x_5]], copy<@type[[TYPE_A]], reason=assign>(read<@type[[TYPE_A]]>(%[[VALUE_sA]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3A]], read<u32>(%[[VALUE_a]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_x_5]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sA]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%[[VALUE_x_5]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%[[VALUE_sA]]))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sA]])))), read<u32>(%[[VALUE_r]]))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%[[VALUE_x_5]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%[[VALUE_sA]]))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v]]), read<u32>(%[[VALUE_a]])), read<u32>(%[[VALUE_mask]])), read<u32>(%[[VALUE_r]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeB:[0-9]+]] @retmeB(%[[VALUE_x_6:[0-9]+]] x: @type[[TYPE_B]]) -> @type[[TYPE_B]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_B]], reason=return>(read<@type[[TYPE_B]]>(%[[VALUE_x_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1B:[0-9]+]] @fn1B(%[[VALUE_x_7:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_3:[0-9]+]] y: @type[[TYPE_B]] [storage=automatic] = copy<@type[[TYPE_B]], reason=assign>(read<@type[[TYPE_B]]>(%[[VALUE_sB]]));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y_3]]));
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE18]]))), read<u32>(%[[VALUE_x_7]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y_3]]), read<u32>(%[[VALUE19]]));
// DEFAULT-NEXT:         write<@type[[TYPE_B]]>(%[[VALUE_y_3]], copy<@type[[TYPE_B]], reason=assign>(call<@type[[TYPE_B]], signature=fn(@type[[TYPE_B]]) -> @type[[TYPE_B]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeB]], copy<@type[[TYPE_B]], reason=arg>(read<@type[[TYPE_B]]>(%[[VALUE_y_3]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y_3]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2B:[0-9]+]] @fn2B(%[[VALUE_x_8:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_4:[0-9]+]] y: @type[[TYPE_B]] [storage=automatic] = copy<@type[[TYPE_B]], reason=assign>(read<@type[[TYPE_B]]>(%[[VALUE_sB]]));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y_4]]));
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE20]]))), read<u32>(%[[VALUE_x_8]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y_4]]), read<u32>(%[[VALUE21]]));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y_4]]));
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE22]])), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y_4]]), read<u32>(%[[VALUE23]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_y_4]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitB:[0-9]+]] @retitB() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sB]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3B:[0-9]+]] @fn3B(%[[VALUE_x_9:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sB]]));
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE24]]))), read<u32>(%[[VALUE_x_9]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sB]]), read<u32>(%[[VALUE25]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitB]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testB:[0-9]+]] @testB() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_2:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_2:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_10:[0-9]+]] x: @type[[TYPE_B]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_B]]>>(%[[VALUE_sB]]));
// DEFAULT-NEXT:         for %[[VALUE26:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_2]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE28:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE27]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE28]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_2]]);
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE29]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_2]], read<ptr<i8>>(%[[VALUE30]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE29]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u32>(field3(%[[VALUE_sB]]), float_to_int<u32, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sB]]), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_2]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sB]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_2]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_2]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sB]]), read<u32>(%[[VALUE_v_2]]));
// DEFAULT-NEXT:         write<@type[[TYPE_B]]>(%[[VALUE_x_10]], copy<@type[[TYPE_B]], reason=assign>(read<@type[[TYPE_B]]>(%[[VALUE_sB]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_2]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1B]], read<u32>(%[[VALUE_a_2]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_x_10]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sB]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_x_10]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_sB]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_x_10]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sB]]))))), ne<u32>(read<u32>(field3(%[[VALUE_x_10]])), read<u32>(field3(%[[VALUE_sB]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_2]]), read<u32>(%[[VALUE_a_2]])), read<u32>(%[[VALUE_mask_2]])), read<u32>(%[[VALUE_r_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_2]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_2]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sB]]), read<u32>(%[[VALUE_v_2]]));
// DEFAULT-NEXT:         write<@type[[TYPE_B]]>(%[[VALUE_x_10]], copy<@type[[TYPE_B]], reason=assign>(read<@type[[TYPE_B]]>(%[[VALUE_sB]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_2]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2B]], read<u32>(%[[VALUE_a_2]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_x_10]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sB]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_x_10]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_sB]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_x_10]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sB]]))))), ne<u32>(read<u32>(field3(%[[VALUE_x_10]])), read<u32>(field3(%[[VALUE_sB]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_2]]), read<u32>(%[[VALUE_a_2]])), read<u32>(%[[VALUE_mask_2]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_2]])), read<u32>(%[[VALUE_r_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_2]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_2]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sB]]), read<u32>(%[[VALUE_v_2]]));
// DEFAULT-NEXT:         write<@type[[TYPE_B]]>(%[[VALUE_x_10]], copy<@type[[TYPE_B]], reason=assign>(read<@type[[TYPE_B]]>(%[[VALUE_sB]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_2]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3B]], read<u32>(%[[VALUE_a_2]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_x_10]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sB]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_x_10]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_sB]]))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sB]])))), read<u32>(%[[VALUE_r_2]]))), ne<u32>(read<u32>(field3(%[[VALUE_x_10]])), read<u32>(field3(%[[VALUE_sB]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_2]]), read<u32>(%[[VALUE_a_2]])), read<u32>(%[[VALUE_mask_2]])), read<u32>(%[[VALUE_r_2]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeC:[0-9]+]] @retmeC(%[[VALUE_x_11:[0-9]+]] x: @type[[TYPE_C]]) -> @type[[TYPE_C]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_C]], reason=return>(read<@type[[TYPE_C]]>(%[[VALUE_x_11]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1C:[0-9]+]] @fn1C(%[[VALUE_x_12:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_5:[0-9]+]] y: @type[[TYPE_C]] [storage=automatic] = copy<@type[[TYPE_C]], reason=assign>(read<@type[[TYPE_C]]>(%[[VALUE_sC]]));
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_y_5]]));
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE31]]))), read<u32>(%[[VALUE_x_12]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_y_5]]), read<u32>(%[[VALUE32]]));
// DEFAULT-NEXT:         write<@type[[TYPE_C]]>(%[[VALUE_y_5]], copy<@type[[TYPE_C]], reason=assign>(call<@type[[TYPE_C]], signature=fn(@type[[TYPE_C]]) -> @type[[TYPE_C]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeC]], copy<@type[[TYPE_C]], reason=arg>(read<@type[[TYPE_C]]>(%[[VALUE_y_5]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_y_5]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2C:[0-9]+]] @fn2C(%[[VALUE_x_13:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_6:[0-9]+]] y: @type[[TYPE_C]] [storage=automatic] = copy<@type[[TYPE_C]], reason=assign>(read<@type[[TYPE_C]]>(%[[VALUE_sC]]));
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_y_6]]));
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE33]]))), read<u32>(%[[VALUE_x_13]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_y_6]]), read<u32>(%[[VALUE34]]));
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_y_6]]));
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE35]])), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_y_6]]), read<u32>(%[[VALUE36]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_y_6]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitC:[0-9]+]] @retitC() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_sC]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3C:[0-9]+]] @fn3C(%[[VALUE_x_14:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_sC]]));
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE37]]))), read<u32>(%[[VALUE_x_14]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_sC]]), read<u32>(%[[VALUE38]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitC]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testC:[0-9]+]] @testC() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_3:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_3:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_3:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_15:[0-9]+]] x: @type[[TYPE_C]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_3:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_C]]>>(%[[VALUE_sC]]));
// DEFAULT-NEXT:         for %[[VALUE39:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_3]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE40:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE41:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE40]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE41]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE42:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_3]]);
// DEFAULT-NEXT:                 let %[[VALUE43:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE42]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_3]], read<ptr<i8>>(%[[VALUE43]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE42]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u32>(field0(%[[VALUE_sC]]), float_to_int<u32, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_sC]]), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_3]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_sC]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_3]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_3]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_sC]]), read<u32>(%[[VALUE_v_3]]));
// DEFAULT-NEXT:         write<@type[[TYPE_C]]>(%[[VALUE_x_15]], copy<@type[[TYPE_C]], reason=assign>(read<@type[[TYPE_C]]>(%[[VALUE_sC]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_3]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1C]], read<u32>(%[[VALUE_a_3]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_x_15]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_sC]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%[[VALUE_x_15]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%[[VALUE_sC]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_x_15]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_sC]]))))), ne<u32>(read<u32>(field0(%[[VALUE_x_15]])), read<u32>(field0(%[[VALUE_sC]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_3]]), read<u32>(%[[VALUE_a_3]])), read<u32>(%[[VALUE_mask_3]])), read<u32>(%[[VALUE_r_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_3]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_3]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_sC]]), read<u32>(%[[VALUE_v_3]]));
// DEFAULT-NEXT:         write<@type[[TYPE_C]]>(%[[VALUE_x_15]], copy<@type[[TYPE_C]], reason=assign>(read<@type[[TYPE_C]]>(%[[VALUE_sC]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_3]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2C]], read<u32>(%[[VALUE_a_3]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_x_15]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_sC]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%[[VALUE_x_15]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%[[VALUE_sC]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_x_15]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_sC]]))))), ne<u32>(read<u32>(field0(%[[VALUE_x_15]])), read<u32>(field0(%[[VALUE_sC]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_3]]), read<u32>(%[[VALUE_a_3]])), read<u32>(%[[VALUE_mask_3]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_3]])), read<u32>(%[[VALUE_r_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_3]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_3]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_sC]]), read<u32>(%[[VALUE_v_3]]));
// DEFAULT-NEXT:         write<@type[[TYPE_C]]>(%[[VALUE_x_15]], copy<@type[[TYPE_C]], reason=assign>(read<@type[[TYPE_C]]>(%[[VALUE_sC]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_3]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3C]], read<u32>(%[[VALUE_a_3]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_x_15]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_sC]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%[[VALUE_x_15]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%[[VALUE_sC]]))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_sC]])))), read<u32>(%[[VALUE_r_3]]))), ne<u32>(read<u32>(field0(%[[VALUE_x_15]])), read<u32>(field0(%[[VALUE_sC]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_3]]), read<u32>(%[[VALUE_a_3]])), read<u32>(%[[VALUE_mask_3]])), read<u32>(%[[VALUE_r_3]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeD:[0-9]+]] @retmeD(%[[VALUE_x_16:[0-9]+]] x: @type[[TYPE_D]]) -> @type[[TYPE_D]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_D]], reason=return>(read<@type[[TYPE_D]]>(%[[VALUE_x_16]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1D:[0-9]+]] @fn1D(%[[VALUE_x_17:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_7:[0-9]+]] y: @type[[TYPE_D]] [storage=automatic] = copy<@type[[TYPE_D]], reason=assign>(read<@type[[TYPE_D]]>(%[[VALUE_sD]]));
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_7]]));
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE44]])))), read<u32>(%[[VALUE_x_17]])));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_7]]), read<u64>(%[[VALUE45]]));
// DEFAULT-NEXT:         write<@type[[TYPE_D]]>(%[[VALUE_y_7]], copy<@type[[TYPE_D]], reason=assign>(call<@type[[TYPE_D]], signature=fn(@type[[TYPE_D]]) -> @type[[TYPE_D]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeD]], copy<@type[[TYPE_D]], reason=arg>(read<@type[[TYPE_D]]>(%[[VALUE_y_7]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_7]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2D:[0-9]+]] @fn2D(%[[VALUE_x_18:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_8:[0-9]+]] y: @type[[TYPE_D]] [storage=automatic] = copy<@type[[TYPE_D]], reason=assign>(read<@type[[TYPE_D]]>(%[[VALUE_sD]]));
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_8]]));
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE46]])))), read<u32>(%[[VALUE_x_18]])));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_8]]), read<u64>(%[[VALUE47]]));
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_8]]));
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE48]]))), const<i32>(15))));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_8]]), read<u64>(%[[VALUE49]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_8]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitD:[0-9]+]] @retitD() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sD]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3D:[0-9]+]] @fn3D(%[[VALUE_x_19:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sD]]));
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE50]])))), read<u32>(%[[VALUE_x_19]])));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sD]]), read<u64>(%[[VALUE51]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitD]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testD:[0-9]+]] @testD() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_4:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_4:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_4:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_4:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_4:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_20:[0-9]+]] x: @type[[TYPE_D]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_4:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_D]]>>(%[[VALUE_sD]]));
// DEFAULT-NEXT:         for %[[VALUE52:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_4]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_4]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE53:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_4]]);
// DEFAULT-NEXT:                 let %[[VALUE54:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE53]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_4]], read<i32>(%[[VALUE54]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE55:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_4]]);
// DEFAULT-NEXT:                 let %[[VALUE56:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE55]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_4]], read<ptr<i8>>(%[[VALUE56]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE55]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%[[VALUE_sD]]), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sD]]), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_4]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sD]]))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_4]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_4]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sD]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_4]])));
// DEFAULT-NEXT:         write<@type[[TYPE_D]]>(%[[VALUE_x_20]], copy<@type[[TYPE_D]], reason=assign>(read<@type[[TYPE_D]]>(%[[VALUE_sD]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_4]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1D]], read<u32>(%[[VALUE_a_4]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_x_20]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_sD]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_x_20]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_sD]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_x_20]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sD]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%[[VALUE_x_20]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%[[VALUE_sD]])))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_4]]), read<u32>(%[[VALUE_a_4]])), read<u32>(%[[VALUE_mask_4]])), read<u32>(%[[VALUE_r_4]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_4]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_4]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sD]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_4]])));
// DEFAULT-NEXT:         write<@type[[TYPE_D]]>(%[[VALUE_x_20]], copy<@type[[TYPE_D]], reason=assign>(read<@type[[TYPE_D]]>(%[[VALUE_sD]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_4]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2D]], read<u32>(%[[VALUE_a_4]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_x_20]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_sD]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_x_20]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_sD]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_x_20]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sD]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%[[VALUE_x_20]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%[[VALUE_sD]])))))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_4]]), read<u32>(%[[VALUE_a_4]])), read<u32>(%[[VALUE_mask_4]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_4]])), read<u32>(%[[VALUE_r_4]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_4]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_4]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sD]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_4]])));
// DEFAULT-NEXT:         write<@type[[TYPE_D]]>(%[[VALUE_x_20]], copy<@type[[TYPE_D]], reason=assign>(read<@type[[TYPE_D]]>(%[[VALUE_sD]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_4]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3D]], read<u32>(%[[VALUE_a_4]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_x_20]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_sD]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_x_20]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_sD]])))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sD]]))))), read<u32>(%[[VALUE_r_4]]))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%[[VALUE_x_20]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%[[VALUE_sD]])))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_4]]), read<u32>(%[[VALUE_a_4]])), read<u32>(%[[VALUE_mask_4]])), read<u32>(%[[VALUE_r_4]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeE:[0-9]+]] @retmeE(%[[VALUE_x_21:[0-9]+]] x: @type[[TYPE_E]]) -> @type[[TYPE_E]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_E]], reason=return>(read<@type[[TYPE_E]]>(%[[VALUE_x_21]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1E:[0-9]+]] @fn1E(%[[VALUE_x_22:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_9:[0-9]+]] y: @type[[TYPE_E]] [storage=automatic] = copy<@type[[TYPE_E]], reason=assign>(read<@type[[TYPE_E]]>(%[[VALUE_sE]]));
// DEFAULT-NEXT:         let %[[VALUE57:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_y_9]]));
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE57]])))), read<u32>(%[[VALUE_x_22]])));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_y_9]]), read<u64>(%[[VALUE58]]));
// DEFAULT-NEXT:         write<@type[[TYPE_E]]>(%[[VALUE_y_9]], copy<@type[[TYPE_E]], reason=assign>(call<@type[[TYPE_E]], signature=fn(@type[[TYPE_E]]) -> @type[[TYPE_E]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeE]], copy<@type[[TYPE_E]], reason=arg>(read<@type[[TYPE_E]]>(%[[VALUE_y_9]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_y_9]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2E:[0-9]+]] @fn2E(%[[VALUE_x_23:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_10:[0-9]+]] y: @type[[TYPE_E]] [storage=automatic] = copy<@type[[TYPE_E]], reason=assign>(read<@type[[TYPE_E]]>(%[[VALUE_sE]]));
// DEFAULT-NEXT:         let %[[VALUE59:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_y_10]]));
// DEFAULT-NEXT:         let %[[VALUE60:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE59]])))), read<u32>(%[[VALUE_x_23]])));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_y_10]]), read<u64>(%[[VALUE60]]));
// DEFAULT-NEXT:         let %[[VALUE61:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_y_10]]));
// DEFAULT-NEXT:         let %[[VALUE62:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE61]]))), const<i32>(15))));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_y_10]]), read<u64>(%[[VALUE62]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_y_10]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitE:[0-9]+]] @retitE() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_sE]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3E:[0-9]+]] @fn3E(%[[VALUE_x_24:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE63:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_sE]]));
// DEFAULT-NEXT:         let %[[VALUE64:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE63]])))), read<u32>(%[[VALUE_x_24]])));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_sE]]), read<u64>(%[[VALUE64]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitE]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testE:[0-9]+]] @testE() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_5:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_5:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_5:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_5:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_5:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_25:[0-9]+]] x: @type[[TYPE_E]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_5:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_E]]>>(%[[VALUE_sE]]));
// DEFAULT-NEXT:         for %[[VALUE65:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_5]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_5]]))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE66:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_5]]);
// DEFAULT-NEXT:                 let %[[VALUE67:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE66]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_5]], read<i32>(%[[VALUE67]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE68:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_5]]);
// DEFAULT-NEXT:                 let %[[VALUE69:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE68]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_5]], read<ptr<i8>>(%[[VALUE69]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE68]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field0(%[[VALUE_sE]]), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_sE]]), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_5]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_sE]]))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_5]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_5]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_sE]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_5]])));
// DEFAULT-NEXT:         write<@type[[TYPE_E]]>(%[[VALUE_x_25]], copy<@type[[TYPE_E]], reason=assign>(read<@type[[TYPE_E]]>(%[[VALUE_sE]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_5]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1E]], read<u32>(%[[VALUE_a_5]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_x_25]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_sE]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%[[VALUE_x_25]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%[[VALUE_sE]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_x_25]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_sE]])))))), ne<u64>(read<u64>(field0(%[[VALUE_x_25]])), read<u64>(field0(%[[VALUE_sE]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_5]]), read<u32>(%[[VALUE_a_5]])), read<u32>(%[[VALUE_mask_5]])), read<u32>(%[[VALUE_r_5]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_5]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_5]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_sE]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_5]])));
// DEFAULT-NEXT:         write<@type[[TYPE_E]]>(%[[VALUE_x_25]], copy<@type[[TYPE_E]], reason=assign>(read<@type[[TYPE_E]]>(%[[VALUE_sE]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_5]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2E]], read<u32>(%[[VALUE_a_5]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_x_25]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_sE]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%[[VALUE_x_25]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%[[VALUE_sE]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_x_25]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_sE]])))))), ne<u64>(read<u64>(field0(%[[VALUE_x_25]])), read<u64>(field0(%[[VALUE_sE]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_5]]), read<u32>(%[[VALUE_a_5]])), read<u32>(%[[VALUE_mask_5]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_5]])), read<u32>(%[[VALUE_r_5]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_5]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_5]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_sE]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_5]])));
// DEFAULT-NEXT:         write<@type[[TYPE_E]]>(%[[VALUE_x_25]], copy<@type[[TYPE_E]], reason=assign>(read<@type[[TYPE_E]]>(%[[VALUE_sE]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_5]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3E]], read<u32>(%[[VALUE_a_5]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_x_25]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_sE]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%[[VALUE_x_25]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%[[VALUE_sE]])))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_sE]]))))), read<u32>(%[[VALUE_r_5]]))), ne<u64>(read<u64>(field0(%[[VALUE_x_25]])), read<u64>(field0(%[[VALUE_sE]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_5]]), read<u32>(%[[VALUE_a_5]])), read<u32>(%[[VALUE_mask_5]])), read<u32>(%[[VALUE_r_5]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeF:[0-9]+]] @retmeF(%[[VALUE_x_26:[0-9]+]] x: @type[[TYPE_F]]) -> @type[[TYPE_F]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_F]], reason=return>(read<@type[[TYPE_F]]>(%[[VALUE_x_26]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1F:[0-9]+]] @fn1F(%[[VALUE_x_27:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_11:[0-9]+]] y: @type[[TYPE_F]] [storage=automatic] = copy<@type[[TYPE_F]], reason=assign>(read<@type[[TYPE_F]]>(%[[VALUE_sF]]));
// DEFAULT-NEXT:         let %[[VALUE70:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_11]]));
// DEFAULT-NEXT:         let %[[VALUE71:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE70]])))), read<u32>(%[[VALUE_x_27]])));
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_11]]), read<u64>(%[[VALUE71]]));
// DEFAULT-NEXT:         write<@type[[TYPE_F]]>(%[[VALUE_y_11]], copy<@type[[TYPE_F]], reason=assign>(call<@type[[TYPE_F]], signature=fn(@type[[TYPE_F]]) -> @type[[TYPE_F]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeF]], copy<@type[[TYPE_F]], reason=arg>(read<@type[[TYPE_F]]>(%[[VALUE_y_11]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_11]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2F:[0-9]+]] @fn2F(%[[VALUE_x_28:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_12:[0-9]+]] y: @type[[TYPE_F]] [storage=automatic] = copy<@type[[TYPE_F]], reason=assign>(read<@type[[TYPE_F]]>(%[[VALUE_sF]]));
// DEFAULT-NEXT:         let %[[VALUE72:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_12]]));
// DEFAULT-NEXT:         let %[[VALUE73:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE72]])))), read<u32>(%[[VALUE_x_28]])));
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_12]]), read<u64>(%[[VALUE73]]));
// DEFAULT-NEXT:         let %[[VALUE74:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_12]]));
// DEFAULT-NEXT:         let %[[VALUE75:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE74]]))), const<i32>(15))));
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_12]]), read<u64>(%[[VALUE75]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_y_12]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitF:[0-9]+]] @retitF() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sF]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3F:[0-9]+]] @fn3F(%[[VALUE_x_29:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE76:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sF]]));
// DEFAULT-NEXT:         let %[[VALUE77:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE76]])))), read<u32>(%[[VALUE_x_29]])));
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sF]]), read<u64>(%[[VALUE77]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitF]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testF:[0-9]+]] @testF() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_6:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_6:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_6:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_6:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_6:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_30:[0-9]+]] x: @type[[TYPE_F]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_6:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_F]]>>(%[[VALUE_sF]]));
// DEFAULT-NEXT:         for %[[VALUE78:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_6]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_6]]))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE79:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_6]]);
// DEFAULT-NEXT:                 let %[[VALUE80:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE79]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_6]], read<i32>(%[[VALUE80]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE81:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_6]]);
// DEFAULT-NEXT:                 let %[[VALUE82:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE81]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_6]], read<ptr<i8>>(%[[VALUE82]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE81]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%[[VALUE_sF]]), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sF]]), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_6]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sF]]))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_6]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_6]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sF]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_6]])));
// DEFAULT-NEXT:         write<@type[[TYPE_F]]>(%[[VALUE_x_30]], copy<@type[[TYPE_F]], reason=assign>(read<@type[[TYPE_F]]>(%[[VALUE_sF]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_6]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1F]], read<u32>(%[[VALUE_a_6]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_x_30]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_sF]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_x_30]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_sF]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_x_30]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sF]])))))), ne<u64>(read<u64>(field3(%[[VALUE_x_30]])), read<u64>(field3(%[[VALUE_sF]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_6]]), read<u32>(%[[VALUE_a_6]])), read<u32>(%[[VALUE_mask_6]])), read<u32>(%[[VALUE_r_6]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_6]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_6]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sF]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_6]])));
// DEFAULT-NEXT:         write<@type[[TYPE_F]]>(%[[VALUE_x_30]], copy<@type[[TYPE_F]], reason=assign>(read<@type[[TYPE_F]]>(%[[VALUE_sF]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_6]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2F]], read<u32>(%[[VALUE_a_6]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_x_30]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_sF]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_x_30]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_sF]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_x_30]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sF]])))))), ne<u64>(read<u64>(field3(%[[VALUE_x_30]])), read<u64>(field3(%[[VALUE_sF]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_6]]), read<u32>(%[[VALUE_a_6]])), read<u32>(%[[VALUE_mask_6]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_6]])), read<u32>(%[[VALUE_r_6]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_6]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_6]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sF]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_6]])));
// DEFAULT-NEXT:         write<@type[[TYPE_F]]>(%[[VALUE_x_30]], copy<@type[[TYPE_F]], reason=assign>(read<@type[[TYPE_F]]>(%[[VALUE_sF]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_6]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3F]], read<u32>(%[[VALUE_a_6]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_x_30]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_sF]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_x_30]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_sF]])))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sF]]))))), read<u32>(%[[VALUE_r_6]]))), ne<u64>(read<u64>(field3(%[[VALUE_x_30]])), read<u64>(field3(%[[VALUE_sF]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_6]]), read<u32>(%[[VALUE_a_6]])), read<u32>(%[[VALUE_mask_6]])), read<u32>(%[[VALUE_r_6]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeG:[0-9]+]] @retmeG(%[[VALUE_x_31:[0-9]+]] x: @type[[TYPE_G]]) -> @type[[TYPE_G]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_G]], reason=return>(read<@type[[TYPE_G]]>(%[[VALUE_x_31]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1G:[0-9]+]] @fn1G(%[[VALUE_x_32:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_13:[0-9]+]] y: @type[[TYPE_G]] [storage=automatic] = copy<@type[[TYPE_G]], reason=assign>(read<@type[[TYPE_G]]>(%[[VALUE_sG]]));
// DEFAULT-NEXT:         let %[[VALUE83:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_y_13]]));
// DEFAULT-NEXT:         let %[[VALUE84:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE83]]))), read<u32>(%[[VALUE_x_32]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_y_13]]), read<u32>(%[[VALUE84]]));
// DEFAULT-NEXT:         write<@type[[TYPE_G]]>(%[[VALUE_y_13]], copy<@type[[TYPE_G]], reason=assign>(call<@type[[TYPE_G]], signature=fn(@type[[TYPE_G]]) -> @type[[TYPE_G]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeG]], copy<@type[[TYPE_G]], reason=arg>(read<@type[[TYPE_G]]>(%[[VALUE_y_13]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_y_13]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2G:[0-9]+]] @fn2G(%[[VALUE_x_33:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_14:[0-9]+]] y: @type[[TYPE_G]] [storage=automatic] = copy<@type[[TYPE_G]], reason=assign>(read<@type[[TYPE_G]]>(%[[VALUE_sG]]));
// DEFAULT-NEXT:         let %[[VALUE85:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_y_14]]));
// DEFAULT-NEXT:         let %[[VALUE86:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE85]]))), read<u32>(%[[VALUE_x_33]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_y_14]]), read<u32>(%[[VALUE86]]));
// DEFAULT-NEXT:         let %[[VALUE87:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_y_14]]));
// DEFAULT-NEXT:         let %[[VALUE88:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE87]])), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_y_14]]), read<u32>(%[[VALUE88]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_y_14]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitG:[0-9]+]] @retitG() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sG]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3G:[0-9]+]] @fn3G(%[[VALUE_x_34:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE89:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sG]]));
// DEFAULT-NEXT:         let %[[VALUE90:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE89]]))), read<u32>(%[[VALUE_x_34]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sG]]), read<u32>(%[[VALUE90]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitG]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testG:[0-9]+]] @testG() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_7:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_7:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_7:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_7:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_7:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_35:[0-9]+]] x: @type[[TYPE_G]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_7:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_G]]>>(%[[VALUE_sG]]));
// DEFAULT-NEXT:         for %[[VALUE91:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_7]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_7]]))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE92:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_7]]);
// DEFAULT-NEXT:                 let %[[VALUE93:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE92]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_7]], read<i32>(%[[VALUE93]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE94:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_7]]);
// DEFAULT-NEXT:                 let %[[VALUE95:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE94]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_7]], read<ptr<i8>>(%[[VALUE95]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE94]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%[[VALUE_sG]]), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sG]]), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_7]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sG]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_7]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_7]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sG]]), read<u32>(%[[VALUE_v_7]]));
// DEFAULT-NEXT:         write<@type[[TYPE_G]]>(%[[VALUE_x_35]], copy<@type[[TYPE_G]], reason=assign>(read<@type[[TYPE_G]]>(%[[VALUE_sG]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_7]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1G]], read<u32>(%[[VALUE_a_7]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_x_35]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sG]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_x_35]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_sG]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_x_35]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sG]]))))), ne<u64>(read<u64>(field3(%[[VALUE_x_35]])), read<u64>(field3(%[[VALUE_sG]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_7]]), read<u32>(%[[VALUE_a_7]])), read<u32>(%[[VALUE_mask_7]])), read<u32>(%[[VALUE_r_7]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_7]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_7]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sG]]), read<u32>(%[[VALUE_v_7]]));
// DEFAULT-NEXT:         write<@type[[TYPE_G]]>(%[[VALUE_x_35]], copy<@type[[TYPE_G]], reason=assign>(read<@type[[TYPE_G]]>(%[[VALUE_sG]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_7]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2G]], read<u32>(%[[VALUE_a_7]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_x_35]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sG]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_x_35]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_sG]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_x_35]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sG]]))))), ne<u64>(read<u64>(field3(%[[VALUE_x_35]])), read<u64>(field3(%[[VALUE_sG]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_7]]), read<u32>(%[[VALUE_a_7]])), read<u32>(%[[VALUE_mask_7]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_7]])), read<u32>(%[[VALUE_r_7]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_7]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_7]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sG]]), read<u32>(%[[VALUE_v_7]]));
// DEFAULT-NEXT:         write<@type[[TYPE_G]]>(%[[VALUE_x_35]], copy<@type[[TYPE_G]], reason=assign>(read<@type[[TYPE_G]]>(%[[VALUE_sG]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_7]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3G]], read<u32>(%[[VALUE_a_7]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_x_35]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sG]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_x_35]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_sG]]))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sG]])))), read<u32>(%[[VALUE_r_7]]))), ne<u64>(read<u64>(field3(%[[VALUE_x_35]])), read<u64>(field3(%[[VALUE_sG]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_7]]), read<u32>(%[[VALUE_a_7]])), read<u32>(%[[VALUE_mask_7]])), read<u32>(%[[VALUE_r_7]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeH:[0-9]+]] @retmeH(%[[VALUE_x_36:[0-9]+]] x: @type[[TYPE_H]]) -> @type[[TYPE_H]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_H]], reason=return>(read<@type[[TYPE_H]]>(%[[VALUE_x_36]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1H:[0-9]+]] @fn1H(%[[VALUE_x_37:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_15:[0-9]+]] y: @type[[TYPE_H]] [storage=automatic] = copy<@type[[TYPE_H]], reason=assign>(read<@type[[TYPE_H]]>(%[[VALUE_sH]]));
// DEFAULT-NEXT:         let %[[VALUE96:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_y_15]]));
// DEFAULT-NEXT:         let %[[VALUE97:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE96]]))), read<u32>(%[[VALUE_x_37]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_y_15]]), read<u32>(%[[VALUE97]]));
// DEFAULT-NEXT:         write<@type[[TYPE_H]]>(%[[VALUE_y_15]], copy<@type[[TYPE_H]], reason=assign>(call<@type[[TYPE_H]], signature=fn(@type[[TYPE_H]]) -> @type[[TYPE_H]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeH]], copy<@type[[TYPE_H]], reason=arg>(read<@type[[TYPE_H]]>(%[[VALUE_y_15]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_y_15]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2H:[0-9]+]] @fn2H(%[[VALUE_x_38:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_16:[0-9]+]] y: @type[[TYPE_H]] [storage=automatic] = copy<@type[[TYPE_H]], reason=assign>(read<@type[[TYPE_H]]>(%[[VALUE_sH]]));
// DEFAULT-NEXT:         let %[[VALUE98:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_y_16]]));
// DEFAULT-NEXT:         let %[[VALUE99:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE98]]))), read<u32>(%[[VALUE_x_38]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_y_16]]), read<u32>(%[[VALUE99]]));
// DEFAULT-NEXT:         let %[[VALUE100:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_y_16]]));
// DEFAULT-NEXT:         let %[[VALUE101:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE100]])), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_y_16]]), read<u32>(%[[VALUE101]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_y_16]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitH:[0-9]+]] @retitH() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sH]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3H:[0-9]+]] @fn3H(%[[VALUE_x_39:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE102:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sH]]));
// DEFAULT-NEXT:         let %[[VALUE103:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE102]]))), read<u32>(%[[VALUE_x_39]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sH]]), read<u32>(%[[VALUE103]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitH]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testH:[0-9]+]] @testH() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_8:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_8:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_8:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_8:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_8:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_40:[0-9]+]] x: @type[[TYPE_H]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_8:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_H]]>>(%[[VALUE_sH]]));
// DEFAULT-NEXT:         for %[[VALUE104:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_8]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_8]]))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE105:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_8]]);
// DEFAULT-NEXT:                 let %[[VALUE106:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE105]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_8]], read<i32>(%[[VALUE106]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE107:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_8]]);
// DEFAULT-NEXT:                 let %[[VALUE108:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE107]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_8]], read<ptr<i8>>(%[[VALUE108]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE107]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%[[VALUE_sH]]), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sH]]), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_8]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sH]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_8]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_8]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sH]]), read<u32>(%[[VALUE_v_8]]));
// DEFAULT-NEXT:         write<@type[[TYPE_H]]>(%[[VALUE_x_40]], copy<@type[[TYPE_H]], reason=assign>(read<@type[[TYPE_H]]>(%[[VALUE_sH]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_8]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1H]], read<u32>(%[[VALUE_a_8]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_x_40]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sH]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_x_40]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_sH]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_x_40]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sH]]))))), ne<u64>(read<u64>(field3(%[[VALUE_x_40]])), read<u64>(field3(%[[VALUE_sH]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_8]]), read<u32>(%[[VALUE_a_8]])), read<u32>(%[[VALUE_mask_8]])), read<u32>(%[[VALUE_r_8]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_8]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_8]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sH]]), read<u32>(%[[VALUE_v_8]]));
// DEFAULT-NEXT:         write<@type[[TYPE_H]]>(%[[VALUE_x_40]], copy<@type[[TYPE_H]], reason=assign>(read<@type[[TYPE_H]]>(%[[VALUE_sH]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_8]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2H]], read<u32>(%[[VALUE_a_8]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_x_40]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sH]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_x_40]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_sH]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_x_40]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sH]]))))), ne<u64>(read<u64>(field3(%[[VALUE_x_40]])), read<u64>(field3(%[[VALUE_sH]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_8]]), read<u32>(%[[VALUE_a_8]])), read<u32>(%[[VALUE_mask_8]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_8]])), read<u32>(%[[VALUE_r_8]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_8]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_8]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sH]]), read<u32>(%[[VALUE_v_8]]));
// DEFAULT-NEXT:         write<@type[[TYPE_H]]>(%[[VALUE_x_40]], copy<@type[[TYPE_H]], reason=assign>(read<@type[[TYPE_H]]>(%[[VALUE_sH]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_8]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3H]], read<u32>(%[[VALUE_a_8]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_x_40]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sH]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_x_40]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_sH]]))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sH]])))), read<u32>(%[[VALUE_r_8]]))), ne<u64>(read<u64>(field3(%[[VALUE_x_40]])), read<u64>(field3(%[[VALUE_sH]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_8]]), read<u32>(%[[VALUE_a_8]])), read<u32>(%[[VALUE_mask_8]])), read<u32>(%[[VALUE_r_8]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeI:[0-9]+]] @retmeI(%[[VALUE_x_41:[0-9]+]] x: @type[[TYPE_I]]) -> @type[[TYPE_I]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_I]], reason=return>(read<@type[[TYPE_I]]>(%[[VALUE_x_41]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1I:[0-9]+]] @fn1I(%[[VALUE_x_42:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_17:[0-9]+]] y: @type[[TYPE_I]] [storage=automatic] = copy<@type[[TYPE_I]], reason=assign>(read<@type[[TYPE_I]]>(%[[VALUE_sI]]));
// DEFAULT-NEXT:         let %[[VALUE109:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_y_17]]));
// DEFAULT-NEXT:         let %[[VALUE110:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE109]])))), read<u32>(%[[VALUE_x_42]])));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_y_17]]), read<u16>(%[[VALUE110]]));
// DEFAULT-NEXT:         write<@type[[TYPE_I]]>(%[[VALUE_y_17]], copy<@type[[TYPE_I]], reason=assign>(call<@type[[TYPE_I]], signature=fn(@type[[TYPE_I]]) -> @type[[TYPE_I]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeI]], copy<@type[[TYPE_I]], reason=arg>(read<@type[[TYPE_I]]>(%[[VALUE_y_17]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_y_17]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2I:[0-9]+]] @fn2I(%[[VALUE_x_43:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_18:[0-9]+]] y: @type[[TYPE_I]] [storage=automatic] = copy<@type[[TYPE_I]], reason=assign>(read<@type[[TYPE_I]]>(%[[VALUE_sI]]));
// DEFAULT-NEXT:         let %[[VALUE111:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_y_18]]));
// DEFAULT-NEXT:         let %[[VALUE112:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE111]])))), read<u32>(%[[VALUE_x_43]])));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_y_18]]), read<u16>(%[[VALUE112]]));
// DEFAULT-NEXT:         let %[[VALUE113:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_y_18]]));
// DEFAULT-NEXT:         let %[[VALUE114:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE113]]))), const<i32>(15))));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_y_18]]), read<u16>(%[[VALUE114]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_y_18]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitI:[0-9]+]] @retitI() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sI]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3I:[0-9]+]] @fn3I(%[[VALUE_x_44:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE115:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sI]]));
// DEFAULT-NEXT:         let %[[VALUE116:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE115]])))), read<u32>(%[[VALUE_x_44]])));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sI]]), read<u16>(%[[VALUE116]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitI]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testI:[0-9]+]] @testI() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_9:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_9:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_9:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_9:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_9:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_45:[0-9]+]] x: @type[[TYPE_I]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_9:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_I]]>>(%[[VALUE_sI]]));
// DEFAULT-NEXT:         for %[[VALUE117:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_9]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_9]]))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE118:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_9]]);
// DEFAULT-NEXT:                 let %[[VALUE119:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE118]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_9]], read<i32>(%[[VALUE119]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE120:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_9]]);
// DEFAULT-NEXT:                 let %[[VALUE121:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE120]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_9]], read<ptr<i8>>(%[[VALUE121]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE120]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%[[VALUE_sI]]), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sI]]), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_9]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sI]]))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_9]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_9]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sI]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_9]])));
// DEFAULT-NEXT:         write<@type[[TYPE_I]]>(%[[VALUE_x_45]], copy<@type[[TYPE_I]], reason=assign>(read<@type[[TYPE_I]]>(%[[VALUE_sI]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_9]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1I]], read<u32>(%[[VALUE_a_9]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_x_45]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sI]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%[[VALUE_x_45]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%[[VALUE_sI]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_x_45]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sI]])))))), ne<u64>(read<u64>(field3(%[[VALUE_x_45]])), read<u64>(field3(%[[VALUE_sI]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_9]]), read<u32>(%[[VALUE_a_9]])), read<u32>(%[[VALUE_mask_9]])), read<u32>(%[[VALUE_r_9]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_9]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_9]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sI]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_9]])));
// DEFAULT-NEXT:         write<@type[[TYPE_I]]>(%[[VALUE_x_45]], copy<@type[[TYPE_I]], reason=assign>(read<@type[[TYPE_I]]>(%[[VALUE_sI]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_9]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2I]], read<u32>(%[[VALUE_a_9]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_x_45]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sI]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%[[VALUE_x_45]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%[[VALUE_sI]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_x_45]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sI]])))))), ne<u64>(read<u64>(field3(%[[VALUE_x_45]])), read<u64>(field3(%[[VALUE_sI]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_9]]), read<u32>(%[[VALUE_a_9]])), read<u32>(%[[VALUE_mask_9]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_9]])), read<u32>(%[[VALUE_r_9]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_9]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_9]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sI]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_9]])));
// DEFAULT-NEXT:         write<@type[[TYPE_I]]>(%[[VALUE_x_45]], copy<@type[[TYPE_I]], reason=assign>(read<@type[[TYPE_I]]>(%[[VALUE_sI]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_9]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3I]], read<u32>(%[[VALUE_a_9]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_x_45]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sI]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%[[VALUE_x_45]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%[[VALUE_sI]])))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sI]]))))), read<u32>(%[[VALUE_r_9]]))), ne<u64>(read<u64>(field3(%[[VALUE_x_45]])), read<u64>(field3(%[[VALUE_sI]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_9]]), read<u32>(%[[VALUE_a_9]])), read<u32>(%[[VALUE_mask_9]])), read<u32>(%[[VALUE_r_9]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeJ:[0-9]+]] @retmeJ(%[[VALUE_x_46:[0-9]+]] x: @type[[TYPE_J]]) -> @type[[TYPE_J]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_J]], reason=return>(read<@type[[TYPE_J]]>(%[[VALUE_x_46]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1J:[0-9]+]] @fn1J(%[[VALUE_x_47:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_19:[0-9]+]] y: @type[[TYPE_J]] [storage=automatic] = copy<@type[[TYPE_J]], reason=assign>(read<@type[[TYPE_J]]>(%[[VALUE_sJ]]));
// DEFAULT-NEXT:         let %[[VALUE122:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_y_19]]));
// DEFAULT-NEXT:         let %[[VALUE123:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE122]])))), read<u32>(%[[VALUE_x_47]])));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_y_19]]), read<u16>(%[[VALUE123]]));
// DEFAULT-NEXT:         write<@type[[TYPE_J]]>(%[[VALUE_y_19]], copy<@type[[TYPE_J]], reason=assign>(call<@type[[TYPE_J]], signature=fn(@type[[TYPE_J]]) -> @type[[TYPE_J]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeJ]], copy<@type[[TYPE_J]], reason=arg>(read<@type[[TYPE_J]]>(%[[VALUE_y_19]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_y_19]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2J:[0-9]+]] @fn2J(%[[VALUE_x_48:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_20:[0-9]+]] y: @type[[TYPE_J]] [storage=automatic] = copy<@type[[TYPE_J]], reason=assign>(read<@type[[TYPE_J]]>(%[[VALUE_sJ]]));
// DEFAULT-NEXT:         let %[[VALUE124:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_y_20]]));
// DEFAULT-NEXT:         let %[[VALUE125:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE124]])))), read<u32>(%[[VALUE_x_48]])));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_y_20]]), read<u16>(%[[VALUE125]]));
// DEFAULT-NEXT:         let %[[VALUE126:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_y_20]]));
// DEFAULT-NEXT:         let %[[VALUE127:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE126]]))), const<i32>(15))));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_y_20]]), read<u16>(%[[VALUE127]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_y_20]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitJ:[0-9]+]] @retitJ() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sJ]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3J:[0-9]+]] @fn3J(%[[VALUE_x_49:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE128:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sJ]]));
// DEFAULT-NEXT:         let %[[VALUE129:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE128]])))), read<u32>(%[[VALUE_x_49]])));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sJ]]), read<u16>(%[[VALUE129]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitJ]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testJ:[0-9]+]] @testJ() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_10:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_10:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_10:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_10:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_10:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_50:[0-9]+]] x: @type[[TYPE_J]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_10:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_J]]>>(%[[VALUE_sJ]]));
// DEFAULT-NEXT:         for %[[VALUE130:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_10]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_10]]))), const<u64>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE131:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_10]]);
// DEFAULT-NEXT:                 let %[[VALUE132:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE131]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_10]], read<i32>(%[[VALUE132]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE133:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_10]]);
// DEFAULT-NEXT:                 let %[[VALUE134:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE133]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_10]], read<ptr<i8>>(%[[VALUE134]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE133]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u16>(field3(%[[VALUE_sJ]]), float_to_int<u16, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sJ]]), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_10]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sJ]]))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_10]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_10]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sJ]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_10]])));
// DEFAULT-NEXT:         write<@type[[TYPE_J]]>(%[[VALUE_x_50]], copy<@type[[TYPE_J]], reason=assign>(read<@type[[TYPE_J]]>(%[[VALUE_sJ]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_10]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1J]], read<u32>(%[[VALUE_a_10]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_x_50]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sJ]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%[[VALUE_x_50]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%[[VALUE_sJ]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_x_50]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sJ]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_x_50]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_sJ]])))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_10]]), read<u32>(%[[VALUE_a_10]])), read<u32>(%[[VALUE_mask_10]])), read<u32>(%[[VALUE_r_10]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_10]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_10]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sJ]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_10]])));
// DEFAULT-NEXT:         write<@type[[TYPE_J]]>(%[[VALUE_x_50]], copy<@type[[TYPE_J]], reason=assign>(read<@type[[TYPE_J]]>(%[[VALUE_sJ]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_10]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2J]], read<u32>(%[[VALUE_a_10]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_x_50]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sJ]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%[[VALUE_x_50]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%[[VALUE_sJ]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_x_50]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sJ]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_x_50]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_sJ]])))))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_10]]), read<u32>(%[[VALUE_a_10]])), read<u32>(%[[VALUE_mask_10]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_10]])), read<u32>(%[[VALUE_r_10]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_10]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_10]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sJ]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_10]])));
// DEFAULT-NEXT:         write<@type[[TYPE_J]]>(%[[VALUE_x_50]], copy<@type[[TYPE_J]], reason=assign>(read<@type[[TYPE_J]]>(%[[VALUE_sJ]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_10]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3J]], read<u32>(%[[VALUE_a_10]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_x_50]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sJ]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%[[VALUE_x_50]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%[[VALUE_sJ]])))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sJ]]))))), read<u32>(%[[VALUE_r_10]]))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_x_50]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_sJ]])))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_10]]), read<u32>(%[[VALUE_a_10]])), read<u32>(%[[VALUE_mask_10]])), read<u32>(%[[VALUE_r_10]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeK:[0-9]+]] @retmeK(%[[VALUE_x_51:[0-9]+]] x: @type[[TYPE_K]]) -> @type[[TYPE_K]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_K]], reason=return>(read<@type[[TYPE_K]]>(%[[VALUE_x_51]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1K:[0-9]+]] @fn1K(%[[VALUE_x_52:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_21:[0-9]+]] y: @type[[TYPE_K]] [storage=automatic] = copy<@type[[TYPE_K]], reason=assign>(read<@type[[TYPE_K]]>(%[[VALUE_sK]]));
// DEFAULT-NEXT:         let %[[VALUE135:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_21]]));
// DEFAULT-NEXT:         let %[[VALUE136:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE135]]))), read<u32>(%[[VALUE_x_52]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_21]]), read<u32>(%[[VALUE136]]));
// DEFAULT-NEXT:         write<@type[[TYPE_K]]>(%[[VALUE_y_21]], copy<@type[[TYPE_K]], reason=assign>(call<@type[[TYPE_K]], signature=fn(@type[[TYPE_K]]) -> @type[[TYPE_K]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeK]], copy<@type[[TYPE_K]], reason=arg>(read<@type[[TYPE_K]]>(%[[VALUE_y_21]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_21]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2K:[0-9]+]] @fn2K(%[[VALUE_x_53:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_22:[0-9]+]] y: @type[[TYPE_K]] [storage=automatic] = copy<@type[[TYPE_K]], reason=assign>(read<@type[[TYPE_K]]>(%[[VALUE_sK]]));
// DEFAULT-NEXT:         let %[[VALUE137:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_22]]));
// DEFAULT-NEXT:         let %[[VALUE138:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE137]]))), read<u32>(%[[VALUE_x_53]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_22]]), read<u32>(%[[VALUE138]]));
// DEFAULT-NEXT:         let %[[VALUE139:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_22]]));
// DEFAULT-NEXT:         let %[[VALUE140:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE139]])), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_22]]), read<u32>(%[[VALUE140]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_22]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitK:[0-9]+]] @retitK() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sK]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3K:[0-9]+]] @fn3K(%[[VALUE_x_54:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE141:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sK]]));
// DEFAULT-NEXT:         let %[[VALUE142:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE141]]))), read<u32>(%[[VALUE_x_54]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sK]]), read<u32>(%[[VALUE142]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitK]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testK:[0-9]+]] @testK() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_11:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_11:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_11:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_11:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_11:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_55:[0-9]+]] x: @type[[TYPE_K]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_11:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_K]]>>(%[[VALUE_sK]]));
// DEFAULT-NEXT:         for %[[VALUE143:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_11]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_11]]))), const<u64>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE144:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_11]]);
// DEFAULT-NEXT:                 let %[[VALUE145:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE144]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_11]], read<i32>(%[[VALUE145]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE146:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_11]]);
// DEFAULT-NEXT:                 let %[[VALUE147:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE146]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_11]], read<ptr<i8>>(%[[VALUE147]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE146]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%[[VALUE_sK]]), float_to_int<u32, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sK]]), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_11]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sK]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_11]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_11]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sK]]), read<u32>(%[[VALUE_v_11]]));
// DEFAULT-NEXT:         write<@type[[TYPE_K]]>(%[[VALUE_x_55]], copy<@type[[TYPE_K]], reason=assign>(read<@type[[TYPE_K]]>(%[[VALUE_sK]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_11]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1K]], read<u32>(%[[VALUE_a_11]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_x_55]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sK]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%[[VALUE_x_55]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%[[VALUE_sK]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_x_55]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sK]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%[[VALUE_x_55]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%[[VALUE_sK]]))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_11]]), read<u32>(%[[VALUE_a_11]])), read<u32>(%[[VALUE_mask_11]])), read<u32>(%[[VALUE_r_11]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_11]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_11]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sK]]), read<u32>(%[[VALUE_v_11]]));
// DEFAULT-NEXT:         write<@type[[TYPE_K]]>(%[[VALUE_x_55]], copy<@type[[TYPE_K]], reason=assign>(read<@type[[TYPE_K]]>(%[[VALUE_sK]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_11]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2K]], read<u32>(%[[VALUE_a_11]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_x_55]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sK]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%[[VALUE_x_55]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%[[VALUE_sK]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_x_55]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sK]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%[[VALUE_x_55]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%[[VALUE_sK]]))))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_11]]), read<u32>(%[[VALUE_a_11]])), read<u32>(%[[VALUE_mask_11]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_11]])), read<u32>(%[[VALUE_r_11]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_11]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_11]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sK]]), read<u32>(%[[VALUE_v_11]]));
// DEFAULT-NEXT:         write<@type[[TYPE_K]]>(%[[VALUE_x_55]], copy<@type[[TYPE_K]], reason=assign>(read<@type[[TYPE_K]]>(%[[VALUE_sK]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_11]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3K]], read<u32>(%[[VALUE_a_11]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_x_55]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sK]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%[[VALUE_x_55]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=7..17>(%[[VALUE_sK]]))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sK]])))), read<u32>(%[[VALUE_r_11]]))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%[[VALUE_x_55]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..7>(%[[VALUE_sK]]))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_11]]), read<u32>(%[[VALUE_a_11]])), read<u32>(%[[VALUE_mask_11]])), read<u32>(%[[VALUE_r_11]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeL:[0-9]+]] @retmeL(%[[VALUE_x_56:[0-9]+]] x: @type[[TYPE_L]]) -> @type[[TYPE_L]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_L]], reason=return>(read<@type[[TYPE_L]]>(%[[VALUE_x_56]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1L:[0-9]+]] @fn1L(%[[VALUE_x_57:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_23:[0-9]+]] y: @type[[TYPE_L]] [storage=automatic] = copy<@type[[TYPE_L]], reason=assign>(read<@type[[TYPE_L]]>(%[[VALUE_sL]]));
// DEFAULT-NEXT:         let %[[VALUE148:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_23]]));
// DEFAULT-NEXT:         let %[[VALUE149:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE148]]))), read<u32>(%[[VALUE_x_57]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_23]]), read<u32>(%[[VALUE149]]));
// DEFAULT-NEXT:         write<@type[[TYPE_L]]>(%[[VALUE_y_23]], copy<@type[[TYPE_L]], reason=assign>(call<@type[[TYPE_L]], signature=fn(@type[[TYPE_L]]) -> @type[[TYPE_L]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeL]], copy<@type[[TYPE_L]], reason=arg>(read<@type[[TYPE_L]]>(%[[VALUE_y_23]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_23]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2L:[0-9]+]] @fn2L(%[[VALUE_x_58:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_24:[0-9]+]] y: @type[[TYPE_L]] [storage=automatic] = copy<@type[[TYPE_L]], reason=assign>(read<@type[[TYPE_L]]>(%[[VALUE_sL]]));
// DEFAULT-NEXT:         let %[[VALUE150:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_24]]));
// DEFAULT-NEXT:         let %[[VALUE151:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE150]]))), read<u32>(%[[VALUE_x_58]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_24]]), read<u32>(%[[VALUE151]]));
// DEFAULT-NEXT:         let %[[VALUE152:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_24]]));
// DEFAULT-NEXT:         let %[[VALUE153:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE152]])), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_24]]), read<u32>(%[[VALUE153]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_y_24]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitL:[0-9]+]] @retitL() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sL]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3L:[0-9]+]] @fn3L(%[[VALUE_x_59:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE154:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sL]]));
// DEFAULT-NEXT:         let %[[VALUE155:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE154]]))), read<u32>(%[[VALUE_x_59]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sL]]), read<u32>(%[[VALUE155]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitL]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testL:[0-9]+]] @testL() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_12:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_12:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_12:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_12:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_12:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_60:[0-9]+]] x: @type[[TYPE_L]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_12:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_L]]>>(%[[VALUE_sL]]));
// DEFAULT-NEXT:         for %[[VALUE156:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_12]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_12]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE157:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_12]]);
// DEFAULT-NEXT:                 let %[[VALUE158:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE157]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_12]], read<i32>(%[[VALUE158]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE159:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_12]]);
// DEFAULT-NEXT:                 let %[[VALUE160:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE159]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_12]], read<ptr<i8>>(%[[VALUE160]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE159]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u32>(field3(%[[VALUE_sL]]), float_to_int<u32, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sL]]), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_12]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sL]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_12]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_12]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sL]]), read<u32>(%[[VALUE_v_12]]));
// DEFAULT-NEXT:         write<@type[[TYPE_L]]>(%[[VALUE_x_60]], copy<@type[[TYPE_L]], reason=assign>(read<@type[[TYPE_L]]>(%[[VALUE_sL]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_12]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1L]], read<u32>(%[[VALUE_a_12]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_x_60]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sL]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_x_60]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_sL]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_x_60]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sL]]))))), ne<u32>(read<u32>(field3(%[[VALUE_x_60]])), read<u32>(field3(%[[VALUE_sL]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_12]]), read<u32>(%[[VALUE_a_12]])), read<u32>(%[[VALUE_mask_12]])), read<u32>(%[[VALUE_r_12]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_12]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_12]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sL]]), read<u32>(%[[VALUE_v_12]]));
// DEFAULT-NEXT:         write<@type[[TYPE_L]]>(%[[VALUE_x_60]], copy<@type[[TYPE_L]], reason=assign>(read<@type[[TYPE_L]]>(%[[VALUE_sL]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_12]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2L]], read<u32>(%[[VALUE_a_12]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_x_60]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sL]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_x_60]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_sL]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_x_60]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sL]]))))), ne<u32>(read<u32>(field3(%[[VALUE_x_60]])), read<u32>(field3(%[[VALUE_sL]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_12]]), read<u32>(%[[VALUE_a_12]])), read<u32>(%[[VALUE_mask_12]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_12]])), read<u32>(%[[VALUE_r_12]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_12]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_12]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sL]]), read<u32>(%[[VALUE_v_12]]));
// DEFAULT-NEXT:         write<@type[[TYPE_L]]>(%[[VALUE_x_60]], copy<@type[[TYPE_L]], reason=assign>(read<@type[[TYPE_L]]>(%[[VALUE_sL]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_12]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3L]], read<u32>(%[[VALUE_a_12]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_x_60]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%[[VALUE_sL]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_x_60]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%[[VALUE_sL]]))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%[[VALUE_sL]])))), read<u32>(%[[VALUE_r_12]]))), ne<u32>(read<u32>(field3(%[[VALUE_x_60]])), read<u32>(field3(%[[VALUE_sL]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_12]]), read<u32>(%[[VALUE_a_12]])), read<u32>(%[[VALUE_mask_12]])), read<u32>(%[[VALUE_r_12]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeM:[0-9]+]] @retmeM(%[[VALUE_x_61:[0-9]+]] x: @type[[TYPE_M]]) -> @type[[TYPE_M]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_M]], reason=return>(read<@type[[TYPE_M]]>(%[[VALUE_x_61]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1M:[0-9]+]] @fn1M(%[[VALUE_x_62:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_25:[0-9]+]] y: @type[[TYPE_M]] [storage=automatic] = copy<@type[[TYPE_M]], reason=assign>(read<@type[[TYPE_M]]>(%[[VALUE_sM]]));
// DEFAULT-NEXT:         let %[[VALUE161:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_y_25]]));
// DEFAULT-NEXT:         let %[[VALUE162:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE161]]))), read<u32>(%[[VALUE_x_62]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_y_25]]), read<u32>(%[[VALUE162]]));
// DEFAULT-NEXT:         write<@type[[TYPE_M]]>(%[[VALUE_y_25]], copy<@type[[TYPE_M]], reason=assign>(call<@type[[TYPE_M]], signature=fn(@type[[TYPE_M]]) -> @type[[TYPE_M]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeM]], copy<@type[[TYPE_M]], reason=arg>(read<@type[[TYPE_M]]>(%[[VALUE_y_25]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_y_25]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2M:[0-9]+]] @fn2M(%[[VALUE_x_63:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_26:[0-9]+]] y: @type[[TYPE_M]] [storage=automatic] = copy<@type[[TYPE_M]], reason=assign>(read<@type[[TYPE_M]]>(%[[VALUE_sM]]));
// DEFAULT-NEXT:         let %[[VALUE163:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_y_26]]));
// DEFAULT-NEXT:         let %[[VALUE164:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE163]]))), read<u32>(%[[VALUE_x_63]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_y_26]]), read<u32>(%[[VALUE164]]));
// DEFAULT-NEXT:         let %[[VALUE165:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_y_26]]));
// DEFAULT-NEXT:         let %[[VALUE166:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE165]])), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_y_26]]), read<u32>(%[[VALUE166]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_y_26]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitM:[0-9]+]] @retitM() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_sM]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3M:[0-9]+]] @fn3M(%[[VALUE_x_64:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE167:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_sM]]));
// DEFAULT-NEXT:         let %[[VALUE168:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE167]]))), read<u32>(%[[VALUE_x_64]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_sM]]), read<u32>(%[[VALUE168]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitM]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testM:[0-9]+]] @testM() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_13:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_13:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_13:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_13:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_13:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_65:[0-9]+]] x: @type[[TYPE_M]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_13:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_M]]>>(%[[VALUE_sM]]));
// DEFAULT-NEXT:         for %[[VALUE169:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_13]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_13]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE170:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_13]]);
// DEFAULT-NEXT:                 let %[[VALUE171:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE170]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_13]], read<i32>(%[[VALUE171]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE172:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_13]]);
// DEFAULT-NEXT:                 let %[[VALUE173:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE172]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_13]], read<ptr<i8>>(%[[VALUE173]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE172]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u32>(field0(%[[VALUE_sM]]), float_to_int<u32, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_sM]]), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_13]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_sM]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_13]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_13]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_sM]]), read<u32>(%[[VALUE_v_13]]));
// DEFAULT-NEXT:         write<@type[[TYPE_M]]>(%[[VALUE_x_65]], copy<@type[[TYPE_M]], reason=assign>(read<@type[[TYPE_M]]>(%[[VALUE_sM]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_13]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1M]], read<u32>(%[[VALUE_a_13]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_x_65]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_sM]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%[[VALUE_x_65]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%[[VALUE_sM]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_x_65]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_sM]]))))), ne<u32>(read<u32>(field0(%[[VALUE_x_65]])), read<u32>(field0(%[[VALUE_sM]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_13]]), read<u32>(%[[VALUE_a_13]])), read<u32>(%[[VALUE_mask_13]])), read<u32>(%[[VALUE_r_13]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_13]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_13]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_sM]]), read<u32>(%[[VALUE_v_13]]));
// DEFAULT-NEXT:         write<@type[[TYPE_M]]>(%[[VALUE_x_65]], copy<@type[[TYPE_M]], reason=assign>(read<@type[[TYPE_M]]>(%[[VALUE_sM]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_13]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2M]], read<u32>(%[[VALUE_a_13]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_x_65]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_sM]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%[[VALUE_x_65]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%[[VALUE_sM]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_x_65]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_sM]]))))), ne<u32>(read<u32>(field0(%[[VALUE_x_65]])), read<u32>(field0(%[[VALUE_sM]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_13]]), read<u32>(%[[VALUE_a_13]])), read<u32>(%[[VALUE_mask_13]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_13]])), read<u32>(%[[VALUE_r_13]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_13]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_13]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_sM]]), read<u32>(%[[VALUE_v_13]]));
// DEFAULT-NEXT:         write<@type[[TYPE_M]]>(%[[VALUE_x_65]], copy<@type[[TYPE_M]], reason=assign>(read<@type[[TYPE_M]]>(%[[VALUE_sM]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_13]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3M]], read<u32>(%[[VALUE_a_13]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_x_65]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=4..8, bits=17..32>(%[[VALUE_sM]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%[[VALUE_x_65]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=4..8, bits=6..17>(%[[VALUE_sM]]))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=4..8, bits=0..6>(%[[VALUE_sM]])))), read<u32>(%[[VALUE_r_13]]))), ne<u32>(read<u32>(field0(%[[VALUE_x_65]])), read<u32>(field0(%[[VALUE_sM]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_13]]), read<u32>(%[[VALUE_a_13]])), read<u32>(%[[VALUE_mask_13]])), read<u32>(%[[VALUE_r_13]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeN:[0-9]+]] @retmeN(%[[VALUE_x_66:[0-9]+]] x: @type[[TYPE_N]]) -> @type[[TYPE_N]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_N]], reason=return>(read<@type[[TYPE_N]]>(%[[VALUE_x_66]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1N:[0-9]+]] @fn1N(%[[VALUE_x_67:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_27:[0-9]+]] y: @type[[TYPE_N]] [storage=automatic] = copy<@type[[TYPE_N]], reason=assign>(read<@type[[TYPE_N]]>(%[[VALUE_sN]]));
// DEFAULT-NEXT:         let %[[VALUE174:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_y_27]]));
// DEFAULT-NEXT:         let %[[VALUE175:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE174]])))), read<u32>(%[[VALUE_x_67]])));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_y_27]]), read<u64>(%[[VALUE175]]));
// DEFAULT-NEXT:         write<@type[[TYPE_N]]>(%[[VALUE_y_27]], copy<@type[[TYPE_N]], reason=assign>(call<@type[[TYPE_N]], signature=fn(@type[[TYPE_N]]) -> @type[[TYPE_N]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeN]], copy<@type[[TYPE_N]], reason=arg>(read<@type[[TYPE_N]]>(%[[VALUE_y_27]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_y_27]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2N:[0-9]+]] @fn2N(%[[VALUE_x_68:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_28:[0-9]+]] y: @type[[TYPE_N]] [storage=automatic] = copy<@type[[TYPE_N]], reason=assign>(read<@type[[TYPE_N]]>(%[[VALUE_sN]]));
// DEFAULT-NEXT:         let %[[VALUE176:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_y_28]]));
// DEFAULT-NEXT:         let %[[VALUE177:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE176]])))), read<u32>(%[[VALUE_x_68]])));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_y_28]]), read<u64>(%[[VALUE177]]));
// DEFAULT-NEXT:         let %[[VALUE178:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_y_28]]));
// DEFAULT-NEXT:         let %[[VALUE179:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE178]]))), const<i32>(15))));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_y_28]]), read<u64>(%[[VALUE179]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_y_28]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitN:[0-9]+]] @retitN() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_sN]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3N:[0-9]+]] @fn3N(%[[VALUE_x_69:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE180:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_sN]]));
// DEFAULT-NEXT:         let %[[VALUE181:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE180]])))), read<u32>(%[[VALUE_x_69]])));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_sN]]), read<u64>(%[[VALUE181]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitN]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testN:[0-9]+]] @testN() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_14:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_14:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_14:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_14:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_14:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_70:[0-9]+]] x: @type[[TYPE_N]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_14:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_N]]>>(%[[VALUE_sN]]));
// DEFAULT-NEXT:         for %[[VALUE182:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_14]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_14]]))), const<u64>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE183:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_14]]);
// DEFAULT-NEXT:                 let %[[VALUE184:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE183]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_14]], read<i32>(%[[VALUE184]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE185:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_14]]);
// DEFAULT-NEXT:                 let %[[VALUE186:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE185]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_14]], read<ptr<i8>>(%[[VALUE186]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE185]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%[[VALUE_sN]]), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_sN]]), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_14]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_sN]]))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_14]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_14]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_sN]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_14]])));
// DEFAULT-NEXT:         write<@type[[TYPE_N]]>(%[[VALUE_x_70]], copy<@type[[TYPE_N]], reason=assign>(read<@type[[TYPE_N]]>(%[[VALUE_sN]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_14]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1N]], read<u32>(%[[VALUE_a_14]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_x_70]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sN]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_x_70]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_sN]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_x_70]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_sN]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%[[VALUE_x_70]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%[[VALUE_sN]])))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_14]]), read<u32>(%[[VALUE_a_14]])), read<u32>(%[[VALUE_mask_14]])), read<u32>(%[[VALUE_r_14]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_14]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_14]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_sN]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_14]])));
// DEFAULT-NEXT:         write<@type[[TYPE_N]]>(%[[VALUE_x_70]], copy<@type[[TYPE_N]], reason=assign>(read<@type[[TYPE_N]]>(%[[VALUE_sN]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_14]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2N]], read<u32>(%[[VALUE_a_14]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_x_70]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sN]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_x_70]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_sN]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_x_70]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_sN]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%[[VALUE_x_70]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%[[VALUE_sN]])))))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_14]]), read<u32>(%[[VALUE_a_14]])), read<u32>(%[[VALUE_mask_14]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_14]])), read<u32>(%[[VALUE_r_14]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_14]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_14]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_sN]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_14]])));
// DEFAULT-NEXT:         write<@type[[TYPE_N]]>(%[[VALUE_x_70]], copy<@type[[TYPE_N]], reason=assign>(read<@type[[TYPE_N]]>(%[[VALUE_sN]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_14]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3N]], read<u32>(%[[VALUE_a_14]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_x_70]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sN]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_x_70]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_sN]])))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=6..12>(%[[VALUE_sN]]))))), read<u32>(%[[VALUE_r_14]]))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%[[VALUE_x_70]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..6>(%[[VALUE_sN]])))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_14]]), read<u32>(%[[VALUE_a_14]])), read<u32>(%[[VALUE_mask_14]])), read<u32>(%[[VALUE_r_14]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeO:[0-9]+]] @retmeO(%[[VALUE_x_71:[0-9]+]] x: @type[[TYPE_O]]) -> @type[[TYPE_O]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_O]], reason=return>(read<@type[[TYPE_O]]>(%[[VALUE_x_71]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1O:[0-9]+]] @fn1O(%[[VALUE_x_72:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_29:[0-9]+]] y: @type[[TYPE_O]] [storage=automatic] = copy<@type[[TYPE_O]], reason=assign>(read<@type[[TYPE_O]]>(%[[VALUE_sO]]));
// DEFAULT-NEXT:         let %[[VALUE187:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_y_29]]));
// DEFAULT-NEXT:         let %[[VALUE188:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE187]])))), read<u32>(%[[VALUE_x_72]])));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_y_29]]), read<u64>(%[[VALUE188]]));
// DEFAULT-NEXT:         write<@type[[TYPE_O]]>(%[[VALUE_y_29]], copy<@type[[TYPE_O]], reason=assign>(call<@type[[TYPE_O]], signature=fn(@type[[TYPE_O]]) -> @type[[TYPE_O]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeO]], copy<@type[[TYPE_O]], reason=arg>(read<@type[[TYPE_O]]>(%[[VALUE_y_29]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_y_29]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2O:[0-9]+]] @fn2O(%[[VALUE_x_73:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_30:[0-9]+]] y: @type[[TYPE_O]] [storage=automatic] = copy<@type[[TYPE_O]], reason=assign>(read<@type[[TYPE_O]]>(%[[VALUE_sO]]));
// DEFAULT-NEXT:         let %[[VALUE189:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_y_30]]));
// DEFAULT-NEXT:         let %[[VALUE190:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE189]])))), read<u32>(%[[VALUE_x_73]])));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_y_30]]), read<u64>(%[[VALUE190]]));
// DEFAULT-NEXT:         let %[[VALUE191:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_y_30]]));
// DEFAULT-NEXT:         let %[[VALUE192:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE191]]))), const<i32>(15))));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_y_30]]), read<u64>(%[[VALUE192]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_y_30]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitO:[0-9]+]] @retitO() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_sO]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3O:[0-9]+]] @fn3O(%[[VALUE_x_74:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE193:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_sO]]));
// DEFAULT-NEXT:         let %[[VALUE194:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE193]])))), read<u32>(%[[VALUE_x_74]])));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_sO]]), read<u64>(%[[VALUE194]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitO]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testO:[0-9]+]] @testO() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_15:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_15:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_15:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_15:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_15:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_75:[0-9]+]] x: @type[[TYPE_O]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_15:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_O]]>>(%[[VALUE_sO]]));
// DEFAULT-NEXT:         for %[[VALUE195:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_15]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_15]]))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE196:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_15]]);
// DEFAULT-NEXT:                 let %[[VALUE197:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE196]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_15]], read<i32>(%[[VALUE197]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE198:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_15]]);
// DEFAULT-NEXT:                 let %[[VALUE199:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE198]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_15]], read<ptr<i8>>(%[[VALUE199]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE198]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field0(%[[VALUE_sO]]), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_sO]]), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_15]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_sO]]))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_15]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_15]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_sO]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_15]])));
// DEFAULT-NEXT:         write<@type[[TYPE_O]]>(%[[VALUE_x_75]], copy<@type[[TYPE_O]], reason=assign>(read<@type[[TYPE_O]]>(%[[VALUE_sO]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_15]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1O]], read<u32>(%[[VALUE_a_15]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_x_75]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_sO]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%[[VALUE_x_75]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%[[VALUE_sO]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_x_75]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_sO]])))))), ne<u64>(read<u64>(field0(%[[VALUE_x_75]])), read<u64>(field0(%[[VALUE_sO]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_15]]), read<u32>(%[[VALUE_a_15]])), read<u32>(%[[VALUE_mask_15]])), read<u32>(%[[VALUE_r_15]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_15]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_15]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_sO]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_15]])));
// DEFAULT-NEXT:         write<@type[[TYPE_O]]>(%[[VALUE_x_75]], copy<@type[[TYPE_O]], reason=assign>(read<@type[[TYPE_O]]>(%[[VALUE_sO]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_15]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2O]], read<u32>(%[[VALUE_a_15]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_x_75]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_sO]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%[[VALUE_x_75]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%[[VALUE_sO]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_x_75]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_sO]])))))), ne<u64>(read<u64>(field0(%[[VALUE_x_75]])), read<u64>(field0(%[[VALUE_sO]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_15]]), read<u32>(%[[VALUE_a_15]])), read<u32>(%[[VALUE_mask_15]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_15]])), read<u32>(%[[VALUE_r_15]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_15]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_15]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_sO]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_15]])));
// DEFAULT-NEXT:         write<@type[[TYPE_O]]>(%[[VALUE_x_75]], copy<@type[[TYPE_O]], reason=assign>(read<@type[[TYPE_O]]>(%[[VALUE_sO]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_15]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3O]], read<u32>(%[[VALUE_a_15]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_x_75]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield3<unit=0, bytes=8..16, bits=35..64>(%[[VALUE_sO]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%[[VALUE_x_75]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=8..16, bits=12..35>(%[[VALUE_sO]])))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=8..16, bits=0..12>(%[[VALUE_sO]]))))), read<u32>(%[[VALUE_r_15]]))), ne<u64>(read<u64>(field0(%[[VALUE_x_75]])), read<u64>(field0(%[[VALUE_sO]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_15]]), read<u32>(%[[VALUE_a_15]])), read<u32>(%[[VALUE_mask_15]])), read<u32>(%[[VALUE_r_15]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeP:[0-9]+]] @retmeP(%[[VALUE_x_76:[0-9]+]] x: @type[[TYPE_P]]) -> @type[[TYPE_P]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_P]], reason=return>(read<@type[[TYPE_P]]>(%[[VALUE_x_76]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1P:[0-9]+]] @fn1P(%[[VALUE_x_77:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_31:[0-9]+]] y: @type[[TYPE_P]] [storage=automatic] = copy<@type[[TYPE_P]], reason=assign>(read<@type[[TYPE_P]]>(%[[VALUE_sP]]));
// DEFAULT-NEXT:         let %[[VALUE200:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_y_31]]));
// DEFAULT-NEXT:         let %[[VALUE201:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE200]])))), read<u32>(%[[VALUE_x_77]])));
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_y_31]]), read<u64>(%[[VALUE201]]));
// DEFAULT-NEXT:         write<@type[[TYPE_P]]>(%[[VALUE_y_31]], copy<@type[[TYPE_P]], reason=assign>(call<@type[[TYPE_P]], signature=fn(@type[[TYPE_P]]) -> @type[[TYPE_P]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeP]], copy<@type[[TYPE_P]], reason=arg>(read<@type[[TYPE_P]]>(%[[VALUE_y_31]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_y_31]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2P:[0-9]+]] @fn2P(%[[VALUE_x_78:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_32:[0-9]+]] y: @type[[TYPE_P]] [storage=automatic] = copy<@type[[TYPE_P]], reason=assign>(read<@type[[TYPE_P]]>(%[[VALUE_sP]]));
// DEFAULT-NEXT:         let %[[VALUE202:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_y_32]]));
// DEFAULT-NEXT:         let %[[VALUE203:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE202]])))), read<u32>(%[[VALUE_x_78]])));
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_y_32]]), read<u64>(%[[VALUE203]]));
// DEFAULT-NEXT:         let %[[VALUE204:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_y_32]]));
// DEFAULT-NEXT:         let %[[VALUE205:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE204]]))), const<i32>(15))));
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_y_32]]), read<u64>(%[[VALUE205]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_y_32]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitP:[0-9]+]] @retitP() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_sP]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3P:[0-9]+]] @fn3P(%[[VALUE_x_79:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE206:[0-9]+]]: u64 [synthetic] = read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_sP]]));
// DEFAULT-NEXT:         let %[[VALUE207:[0-9]+]]: u64 [synthetic] = widen<u64, reason=assign>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(%[[VALUE206]])))), read<u32>(%[[VALUE_x_79]])));
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_sP]]), read<u64>(%[[VALUE207]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitP]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testP:[0-9]+]] @testP() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_16:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_16:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_16:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_16:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_16:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_80:[0-9]+]] x: @type[[TYPE_P]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_16:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_P]]>>(%[[VALUE_sP]]));
// DEFAULT-NEXT:         for %[[VALUE208:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_16]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_16]]))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE209:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_16]]);
// DEFAULT-NEXT:                 let %[[VALUE210:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE209]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_16]], read<i32>(%[[VALUE210]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE211:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_16]]);
// DEFAULT-NEXT:                 let %[[VALUE212:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE211]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_16]], read<ptr<i8>>(%[[VALUE212]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE211]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%[[VALUE_sP]]), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_sP]]), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_16]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_sP]]))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_16]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_16]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_sP]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_16]])));
// DEFAULT-NEXT:         write<@type[[TYPE_P]]>(%[[VALUE_x_80]], copy<@type[[TYPE_P]], reason=assign>(read<@type[[TYPE_P]]>(%[[VALUE_sP]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_16]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1P]], read<u32>(%[[VALUE_a_16]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_x_80]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sP]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_x_80]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_sP]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_x_80]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_sP]])))))), ne<u64>(read<u64>(field3(%[[VALUE_x_80]])), read<u64>(field3(%[[VALUE_sP]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_16]]), read<u32>(%[[VALUE_a_16]])), read<u32>(%[[VALUE_mask_16]])), read<u32>(%[[VALUE_r_16]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_16]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_16]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_sP]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_16]])));
// DEFAULT-NEXT:         write<@type[[TYPE_P]]>(%[[VALUE_x_80]], copy<@type[[TYPE_P]], reason=assign>(read<@type[[TYPE_P]]>(%[[VALUE_sP]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_16]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2P]], read<u32>(%[[VALUE_a_16]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_x_80]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sP]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_x_80]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_sP]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_x_80]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_sP]])))))), ne<u64>(read<u64>(field3(%[[VALUE_x_80]])), read<u64>(field3(%[[VALUE_sP]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_16]]), read<u32>(%[[VALUE_a_16]])), read<u32>(%[[VALUE_mask_16]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_16]])), read<u32>(%[[VALUE_r_16]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_16]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_16]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_sP]]), widen<u64, reason=assign>(read<u32>(%[[VALUE_v_16]])));
// DEFAULT-NEXT:         write<@type[[TYPE_P]]>(%[[VALUE_x_80]], copy<@type[[TYPE_P]], reason=assign>(read<@type[[TYPE_P]]>(%[[VALUE_sP]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_16]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3P]], read<u32>(%[[VALUE_a_16]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_x_80]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield2<unit=0, bytes=0..8, bits=35..64>(%[[VALUE_sP]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_x_80]])))), reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield1<unit=0, bytes=0..8, bits=12..35>(%[[VALUE_sP]])))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(truncate<u32, reason=promotion, fits=unknown>(read<u64>(bitfield0<unit=0, bytes=0..8, bits=0..12>(%[[VALUE_sP]]))))), read<u32>(%[[VALUE_r_16]]))), ne<u64>(read<u64>(field3(%[[VALUE_x_80]])), read<u64>(field3(%[[VALUE_sP]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_16]]), read<u32>(%[[VALUE_a_16]])), read<u32>(%[[VALUE_mask_16]])), read<u32>(%[[VALUE_r_16]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeQ:[0-9]+]] @retmeQ(%[[VALUE_x_81:[0-9]+]] x: @type[[TYPE_Q]]) -> @type[[TYPE_Q]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_Q]], reason=return>(read<@type[[TYPE_Q]]>(%[[VALUE_x_81]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1Q:[0-9]+]] @fn1Q(%[[VALUE_x_82:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_33:[0-9]+]] y: @type[[TYPE_Q]] [storage=automatic] = copy<@type[[TYPE_Q]], reason=assign>(read<@type[[TYPE_Q]]>(%[[VALUE_sQ]]));
// DEFAULT-NEXT:         let %[[VALUE213:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_33]]));
// DEFAULT-NEXT:         let %[[VALUE214:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE213]]))), read<u32>(%[[VALUE_x_82]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_33]]), read<u32>(%[[VALUE214]]));
// DEFAULT-NEXT:         write<@type[[TYPE_Q]]>(%[[VALUE_y_33]], copy<@type[[TYPE_Q]], reason=assign>(call<@type[[TYPE_Q]], signature=fn(@type[[TYPE_Q]]) -> @type[[TYPE_Q]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeQ]], copy<@type[[TYPE_Q]], reason=arg>(read<@type[[TYPE_Q]]>(%[[VALUE_y_33]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_33]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2Q:[0-9]+]] @fn2Q(%[[VALUE_x_83:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_34:[0-9]+]] y: @type[[TYPE_Q]] [storage=automatic] = copy<@type[[TYPE_Q]], reason=assign>(read<@type[[TYPE_Q]]>(%[[VALUE_sQ]]));
// DEFAULT-NEXT:         let %[[VALUE215:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_34]]));
// DEFAULT-NEXT:         let %[[VALUE216:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE215]]))), read<u32>(%[[VALUE_x_83]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_34]]), read<u32>(%[[VALUE216]]));
// DEFAULT-NEXT:         let %[[VALUE217:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_34]]));
// DEFAULT-NEXT:         let %[[VALUE218:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE217]])), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_34]]), read<u32>(%[[VALUE218]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_34]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitQ:[0-9]+]] @retitQ() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sQ]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3Q:[0-9]+]] @fn3Q(%[[VALUE_x_84:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE219:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sQ]]));
// DEFAULT-NEXT:         let %[[VALUE220:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE219]]))), read<u32>(%[[VALUE_x_84]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sQ]]), read<u32>(%[[VALUE220]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitQ]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testQ:[0-9]+]] @testQ() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_17:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_17:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_17:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_17:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_17:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_85:[0-9]+]] x: @type[[TYPE_Q]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_17:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_Q]]>>(%[[VALUE_sQ]]));
// DEFAULT-NEXT:         for %[[VALUE221:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_17]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_17]]))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE222:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_17]]);
// DEFAULT-NEXT:                 let %[[VALUE223:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE222]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_17]], read<i32>(%[[VALUE223]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE224:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_17]]);
// DEFAULT-NEXT:                 let %[[VALUE225:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE224]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_17]], read<ptr<i8>>(%[[VALUE225]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE224]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%[[VALUE_sQ]]), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sQ]]), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_17]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sQ]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_17]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_17]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sQ]]), read<u32>(%[[VALUE_v_17]]));
// DEFAULT-NEXT:         write<@type[[TYPE_Q]]>(%[[VALUE_x_85]], copy<@type[[TYPE_Q]], reason=assign>(read<@type[[TYPE_Q]]>(%[[VALUE_sQ]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_17]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1Q]], read<u32>(%[[VALUE_a_17]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_x_85]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sQ]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_x_85]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_sQ]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_x_85]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sQ]]))))), ne<u64>(read<u64>(field3(%[[VALUE_x_85]])), read<u64>(field3(%[[VALUE_sQ]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_17]]), read<u32>(%[[VALUE_a_17]])), read<u32>(%[[VALUE_mask_17]])), read<u32>(%[[VALUE_r_17]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_17]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_17]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sQ]]), read<u32>(%[[VALUE_v_17]]));
// DEFAULT-NEXT:         write<@type[[TYPE_Q]]>(%[[VALUE_x_85]], copy<@type[[TYPE_Q]], reason=assign>(read<@type[[TYPE_Q]]>(%[[VALUE_sQ]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_17]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2Q]], read<u32>(%[[VALUE_a_17]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_x_85]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sQ]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_x_85]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_sQ]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_x_85]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sQ]]))))), ne<u64>(read<u64>(field3(%[[VALUE_x_85]])), read<u64>(field3(%[[VALUE_sQ]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_17]]), read<u32>(%[[VALUE_a_17]])), read<u32>(%[[VALUE_mask_17]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_17]])), read<u32>(%[[VALUE_r_17]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_17]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_17]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sQ]]), read<u32>(%[[VALUE_v_17]]));
// DEFAULT-NEXT:         write<@type[[TYPE_Q]]>(%[[VALUE_x_85]], copy<@type[[TYPE_Q]], reason=assign>(read<@type[[TYPE_Q]]>(%[[VALUE_sQ]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_17]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3Q]], read<u32>(%[[VALUE_a_17]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_x_85]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sQ]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_x_85]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_sQ]]))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sQ]])))), read<u32>(%[[VALUE_r_17]]))), ne<u64>(read<u64>(field3(%[[VALUE_x_85]])), read<u64>(field3(%[[VALUE_sQ]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_17]]), read<u32>(%[[VALUE_a_17]])), read<u32>(%[[VALUE_mask_17]])), read<u32>(%[[VALUE_r_17]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeR:[0-9]+]] @retmeR(%[[VALUE_x_86:[0-9]+]] x: @type[[TYPE_R]]) -> @type[[TYPE_R]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_R]], reason=return>(read<@type[[TYPE_R]]>(%[[VALUE_x_86]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1R:[0-9]+]] @fn1R(%[[VALUE_x_87:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_35:[0-9]+]] y: @type[[TYPE_R]] [storage=automatic] = copy<@type[[TYPE_R]], reason=assign>(read<@type[[TYPE_R]]>(%[[VALUE_sR]]));
// DEFAULT-NEXT:         let %[[VALUE226:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_35]]));
// DEFAULT-NEXT:         let %[[VALUE227:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE226]]))), read<u32>(%[[VALUE_x_87]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_35]]), read<u32>(%[[VALUE227]]));
// DEFAULT-NEXT:         write<@type[[TYPE_R]]>(%[[VALUE_y_35]], copy<@type[[TYPE_R]], reason=assign>(call<@type[[TYPE_R]], signature=fn(@type[[TYPE_R]]) -> @type[[TYPE_R]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeR]], copy<@type[[TYPE_R]], reason=arg>(read<@type[[TYPE_R]]>(%[[VALUE_y_35]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_35]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2R:[0-9]+]] @fn2R(%[[VALUE_x_88:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_36:[0-9]+]] y: @type[[TYPE_R]] [storage=automatic] = copy<@type[[TYPE_R]], reason=assign>(read<@type[[TYPE_R]]>(%[[VALUE_sR]]));
// DEFAULT-NEXT:         let %[[VALUE228:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_36]]));
// DEFAULT-NEXT:         let %[[VALUE229:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE228]]))), read<u32>(%[[VALUE_x_88]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_36]]), read<u32>(%[[VALUE229]]));
// DEFAULT-NEXT:         let %[[VALUE230:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_36]]));
// DEFAULT-NEXT:         let %[[VALUE231:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE230]])), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_36]]), read<u32>(%[[VALUE231]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_36]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitR:[0-9]+]] @retitR() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sR]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3R:[0-9]+]] @fn3R(%[[VALUE_x_89:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE232:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sR]]));
// DEFAULT-NEXT:         let %[[VALUE233:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE232]]))), read<u32>(%[[VALUE_x_89]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sR]]), read<u32>(%[[VALUE233]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitR]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testR:[0-9]+]] @testR() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_18:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_18:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_18:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_18:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_18:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_90:[0-9]+]] x: @type[[TYPE_R]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_18:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_R]]>>(%[[VALUE_sR]]));
// DEFAULT-NEXT:         for %[[VALUE234:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_18]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_18]]))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE235:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_18]]);
// DEFAULT-NEXT:                 let %[[VALUE236:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE235]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_18]], read<i32>(%[[VALUE236]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE237:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_18]]);
// DEFAULT-NEXT:                 let %[[VALUE238:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE237]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_18]], read<ptr<i8>>(%[[VALUE238]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE237]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%[[VALUE_sR]]), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sR]]), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_18]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sR]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_18]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_18]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sR]]), read<u32>(%[[VALUE_v_18]]));
// DEFAULT-NEXT:         write<@type[[TYPE_R]]>(%[[VALUE_x_90]], copy<@type[[TYPE_R]], reason=assign>(read<@type[[TYPE_R]]>(%[[VALUE_sR]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_18]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1R]], read<u32>(%[[VALUE_a_18]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_x_90]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sR]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_x_90]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_sR]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_x_90]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sR]]))))), ne<u64>(read<u64>(field3(%[[VALUE_x_90]])), read<u64>(field3(%[[VALUE_sR]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_18]]), read<u32>(%[[VALUE_a_18]])), read<u32>(%[[VALUE_mask_18]])), read<u32>(%[[VALUE_r_18]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_18]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_18]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sR]]), read<u32>(%[[VALUE_v_18]]));
// DEFAULT-NEXT:         write<@type[[TYPE_R]]>(%[[VALUE_x_90]], copy<@type[[TYPE_R]], reason=assign>(read<@type[[TYPE_R]]>(%[[VALUE_sR]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_18]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2R]], read<u32>(%[[VALUE_a_18]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_x_90]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sR]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_x_90]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_sR]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_x_90]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sR]]))))), ne<u64>(read<u64>(field3(%[[VALUE_x_90]])), read<u64>(field3(%[[VALUE_sR]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_18]]), read<u32>(%[[VALUE_a_18]])), read<u32>(%[[VALUE_mask_18]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_18]])), read<u32>(%[[VALUE_r_18]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_18]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_18]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sR]]), read<u32>(%[[VALUE_v_18]]));
// DEFAULT-NEXT:         write<@type[[TYPE_R]]>(%[[VALUE_x_90]], copy<@type[[TYPE_R]], reason=assign>(read<@type[[TYPE_R]]>(%[[VALUE_sR]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_18]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3R]], read<u32>(%[[VALUE_a_18]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_x_90]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sR]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_x_90]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_sR]]))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sR]])))), read<u32>(%[[VALUE_r_18]]))), ne<u64>(read<u64>(field3(%[[VALUE_x_90]])), read<u64>(field3(%[[VALUE_sR]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_18]]), read<u32>(%[[VALUE_a_18]])), read<u32>(%[[VALUE_mask_18]])), read<u32>(%[[VALUE_r_18]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeS:[0-9]+]] @retmeS(%[[VALUE_x_91:[0-9]+]] x: @type[[TYPE_S]]) -> @type[[TYPE_S]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_S]], reason=return>(read<@type[[TYPE_S]]>(%[[VALUE_x_91]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1S:[0-9]+]] @fn1S(%[[VALUE_x_92:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_37:[0-9]+]] y: @type[[TYPE_S]] [storage=automatic] = copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_sS]]));
// DEFAULT-NEXT:         let %[[VALUE239:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_37]]));
// DEFAULT-NEXT:         let %[[VALUE240:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE239]])))), read<u32>(%[[VALUE_x_92]])));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_37]]), read<u16>(%[[VALUE240]]));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(%[[VALUE_y_37]], copy<@type[[TYPE_S]], reason=assign>(call<@type[[TYPE_S]], signature=fn(@type[[TYPE_S]]) -> @type[[TYPE_S]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeS]], copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_y_37]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_37]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2S:[0-9]+]] @fn2S(%[[VALUE_x_93:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_38:[0-9]+]] y: @type[[TYPE_S]] [storage=automatic] = copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_sS]]));
// DEFAULT-NEXT:         let %[[VALUE241:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_38]]));
// DEFAULT-NEXT:         let %[[VALUE242:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE241]])))), read<u32>(%[[VALUE_x_93]])));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_38]]), read<u16>(%[[VALUE242]]));
// DEFAULT-NEXT:         let %[[VALUE243:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_38]]));
// DEFAULT-NEXT:         let %[[VALUE244:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE243]]))), const<i32>(15))));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_38]]), read<u16>(%[[VALUE244]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_38]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitS:[0-9]+]] @retitS() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sS]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3S:[0-9]+]] @fn3S(%[[VALUE_x_94:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE245:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sS]]));
// DEFAULT-NEXT:         let %[[VALUE246:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE245]])))), read<u32>(%[[VALUE_x_94]])));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sS]]), read<u16>(%[[VALUE246]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitS]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testS:[0-9]+]] @testS() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_19:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_19:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_19:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_19:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_19:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_95:[0-9]+]] x: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_19:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_sS]]));
// DEFAULT-NEXT:         for %[[VALUE247:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_19]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_19]]))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE248:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_19]]);
// DEFAULT-NEXT:                 let %[[VALUE249:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE248]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_19]], read<i32>(%[[VALUE249]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE250:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_19]]);
// DEFAULT-NEXT:                 let %[[VALUE251:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE250]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_19]], read<ptr<i8>>(%[[VALUE251]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE250]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%[[VALUE_sS]]), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sS]]), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_19]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sS]]))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_19]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_19]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sS]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_19]])));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(%[[VALUE_x_95]], copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_sS]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_19]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1S]], read<u32>(%[[VALUE_a_19]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_x_95]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sS]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%[[VALUE_x_95]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%[[VALUE_sS]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_x_95]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sS]])))))), ne<u64>(read<u64>(field3(%[[VALUE_x_95]])), read<u64>(field3(%[[VALUE_sS]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_19]]), read<u32>(%[[VALUE_a_19]])), read<u32>(%[[VALUE_mask_19]])), read<u32>(%[[VALUE_r_19]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_19]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_19]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sS]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_19]])));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(%[[VALUE_x_95]], copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_sS]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_19]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2S]], read<u32>(%[[VALUE_a_19]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_x_95]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sS]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%[[VALUE_x_95]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%[[VALUE_sS]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_x_95]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sS]])))))), ne<u64>(read<u64>(field3(%[[VALUE_x_95]])), read<u64>(field3(%[[VALUE_sS]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_19]]), read<u32>(%[[VALUE_a_19]])), read<u32>(%[[VALUE_mask_19]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_19]])), read<u32>(%[[VALUE_r_19]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_19]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_19]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sS]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_19]])));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(%[[VALUE_x_95]], copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_sS]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_19]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3S]], read<u32>(%[[VALUE_a_19]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_x_95]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sS]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%[[VALUE_x_95]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..7>(%[[VALUE_sS]])))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sS]]))))), read<u32>(%[[VALUE_r_19]]))), ne<u64>(read<u64>(field3(%[[VALUE_x_95]])), read<u64>(field3(%[[VALUE_sS]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_19]]), read<u32>(%[[VALUE_a_19]])), read<u32>(%[[VALUE_mask_19]])), read<u32>(%[[VALUE_r_19]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeT:[0-9]+]] @retmeT(%[[VALUE_x_96:[0-9]+]] x: @type[[TYPE_T]]) -> @type[[TYPE_T]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_T]], reason=return>(read<@type[[TYPE_T]]>(%[[VALUE_x_96]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1T:[0-9]+]] @fn1T(%[[VALUE_x_97:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_39:[0-9]+]] y: @type[[TYPE_T]] [storage=automatic] = copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_sT]]));
// DEFAULT-NEXT:         let %[[VALUE252:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_39]]));
// DEFAULT-NEXT:         let %[[VALUE253:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE252]])))), read<u32>(%[[VALUE_x_97]])));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_39]]), read<u16>(%[[VALUE253]]));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(%[[VALUE_y_39]], copy<@type[[TYPE_T]], reason=assign>(call<@type[[TYPE_T]], signature=fn(@type[[TYPE_T]]) -> @type[[TYPE_T]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeT]], copy<@type[[TYPE_T]], reason=arg>(read<@type[[TYPE_T]]>(%[[VALUE_y_39]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_39]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2T:[0-9]+]] @fn2T(%[[VALUE_x_98:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_40:[0-9]+]] y: @type[[TYPE_T]] [storage=automatic] = copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_sT]]));
// DEFAULT-NEXT:         let %[[VALUE254:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_40]]));
// DEFAULT-NEXT:         let %[[VALUE255:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE254]])))), read<u32>(%[[VALUE_x_98]])));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_40]]), read<u16>(%[[VALUE255]]));
// DEFAULT-NEXT:         let %[[VALUE256:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_40]]));
// DEFAULT-NEXT:         let %[[VALUE257:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE256]]))), const<i32>(15))));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_40]]), read<u16>(%[[VALUE257]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_y_40]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitT:[0-9]+]] @retitT() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sT]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3T:[0-9]+]] @fn3T(%[[VALUE_x_99:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE258:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sT]]));
// DEFAULT-NEXT:         let %[[VALUE259:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE258]])))), read<u32>(%[[VALUE_x_99]])));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sT]]), read<u16>(%[[VALUE259]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitT]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testT:[0-9]+]] @testT() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_20:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_20:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_20:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_20:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_20:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_100:[0-9]+]] x: @type[[TYPE_T]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_20:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_T]]>>(%[[VALUE_sT]]));
// DEFAULT-NEXT:         for %[[VALUE260:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_20]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_20]]))), const<u64>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE261:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_20]]);
// DEFAULT-NEXT:                 let %[[VALUE262:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE261]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_20]], read<i32>(%[[VALUE262]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE263:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_20]]);
// DEFAULT-NEXT:                 let %[[VALUE264:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE263]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_20]], read<ptr<i8>>(%[[VALUE264]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE263]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u16>(field3(%[[VALUE_sT]]), float_to_int<u16, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sT]]), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_20]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sT]]))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_20]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_20]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sT]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_20]])));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(%[[VALUE_x_100]], copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_sT]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_20]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1T]], read<u32>(%[[VALUE_a_20]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_x_100]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sT]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%[[VALUE_x_100]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%[[VALUE_sT]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_x_100]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sT]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_x_100]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_sT]])))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_20]]), read<u32>(%[[VALUE_a_20]])), read<u32>(%[[VALUE_mask_20]])), read<u32>(%[[VALUE_r_20]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_20]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_20]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sT]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_20]])));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(%[[VALUE_x_100]], copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_sT]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_20]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2T]], read<u32>(%[[VALUE_a_20]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_x_100]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sT]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%[[VALUE_x_100]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%[[VALUE_sT]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_x_100]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sT]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_x_100]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_sT]])))))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_20]]), read<u32>(%[[VALUE_a_20]])), read<u32>(%[[VALUE_mask_20]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_20]])), read<u32>(%[[VALUE_r_20]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_20]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_20]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sT]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_20]])));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(%[[VALUE_x_100]], copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_sT]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_20]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3T]], read<u32>(%[[VALUE_a_20]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_x_100]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sT]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%[[VALUE_x_100]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=1..9>(%[[VALUE_sT]])))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..1>(%[[VALUE_sT]]))))), read<u32>(%[[VALUE_r_20]]))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_x_100]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_sT]])))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_20]]), read<u32>(%[[VALUE_a_20]])), read<u32>(%[[VALUE_mask_20]])), read<u32>(%[[VALUE_r_20]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeU:[0-9]+]] @retmeU(%[[VALUE_x_101:[0-9]+]] x: @type[[TYPE_U]]) -> @type[[TYPE_U]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_U]], reason=return>(read<@type[[TYPE_U]]>(%[[VALUE_x_101]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1U:[0-9]+]] @fn1U(%[[VALUE_x_102:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_41:[0-9]+]] y: @type[[TYPE_U]] [storage=automatic] = copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(%[[VALUE_sU]]));
// DEFAULT-NEXT:         let %[[VALUE265:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_y_41]]));
// DEFAULT-NEXT:         let %[[VALUE266:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE265]])))), read<u32>(%[[VALUE_x_102]])));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_y_41]]), read<u16>(%[[VALUE266]]));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(%[[VALUE_y_41]], copy<@type[[TYPE_U]], reason=assign>(call<@type[[TYPE_U]], signature=fn(@type[[TYPE_U]]) -> @type[[TYPE_U]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeU]], copy<@type[[TYPE_U]], reason=arg>(read<@type[[TYPE_U]]>(%[[VALUE_y_41]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_y_41]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2U:[0-9]+]] @fn2U(%[[VALUE_x_103:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_42:[0-9]+]] y: @type[[TYPE_U]] [storage=automatic] = copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(%[[VALUE_sU]]));
// DEFAULT-NEXT:         let %[[VALUE267:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_y_42]]));
// DEFAULT-NEXT:         let %[[VALUE268:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE267]])))), read<u32>(%[[VALUE_x_103]])));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_y_42]]), read<u16>(%[[VALUE268]]));
// DEFAULT-NEXT:         let %[[VALUE269:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_y_42]]));
// DEFAULT-NEXT:         let %[[VALUE270:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE269]]))), const<i32>(15))));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_y_42]]), read<u16>(%[[VALUE270]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_y_42]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitU:[0-9]+]] @retitU() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_sU]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3U:[0-9]+]] @fn3U(%[[VALUE_x_104:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE271:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_sU]]));
// DEFAULT-NEXT:         let %[[VALUE272:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE271]])))), read<u32>(%[[VALUE_x_104]])));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_sU]]), read<u16>(%[[VALUE272]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitU]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testU:[0-9]+]] @testU() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_21:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_21:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_21:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_21:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_21:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_105:[0-9]+]] x: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_21:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_U]]>>(%[[VALUE_sU]]));
// DEFAULT-NEXT:         for %[[VALUE273:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_21]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_21]]))), const<u64>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE274:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_21]]);
// DEFAULT-NEXT:                 let %[[VALUE275:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE274]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_21]], read<i32>(%[[VALUE275]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE276:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_21]]);
// DEFAULT-NEXT:                 let %[[VALUE277:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE276]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_21]], read<ptr<i8>>(%[[VALUE277]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE276]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u64>(field3(%[[VALUE_sU]]), float_to_int<u64, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_sU]]), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_21]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_sU]]))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_21]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_21]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_sU]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_21]])));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(%[[VALUE_x_105]], copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(%[[VALUE_sU]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_21]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1U]], read<u32>(%[[VALUE_a_21]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_x_105]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sU]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..6>(%[[VALUE_x_105]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..6>(%[[VALUE_sU]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_x_105]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_sU]])))))), ne<u64>(read<u64>(field3(%[[VALUE_x_105]])), read<u64>(field3(%[[VALUE_sU]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_21]]), read<u32>(%[[VALUE_a_21]])), read<u32>(%[[VALUE_mask_21]])), read<u32>(%[[VALUE_r_21]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_21]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_21]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_sU]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_21]])));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(%[[VALUE_x_105]], copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(%[[VALUE_sU]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_21]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2U]], read<u32>(%[[VALUE_a_21]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_x_105]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sU]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..6>(%[[VALUE_x_105]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..6>(%[[VALUE_sU]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_x_105]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_sU]])))))), ne<u64>(read<u64>(field3(%[[VALUE_x_105]])), read<u64>(field3(%[[VALUE_sU]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_21]]), read<u32>(%[[VALUE_a_21]])), read<u32>(%[[VALUE_mask_21]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_21]])), read<u32>(%[[VALUE_r_21]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_21]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_21]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_sU]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_21]])));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(%[[VALUE_x_105]], copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(%[[VALUE_sU]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_21]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3U]], read<u32>(%[[VALUE_a_21]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_x_105]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=7..16>(%[[VALUE_sU]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..6>(%[[VALUE_x_105]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..6>(%[[VALUE_sU]])))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=6..7>(%[[VALUE_sU]]))))), read<u32>(%[[VALUE_r_21]]))), ne<u64>(read<u64>(field3(%[[VALUE_x_105]])), read<u64>(field3(%[[VALUE_sU]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_21]]), read<u32>(%[[VALUE_a_21]])), read<u32>(%[[VALUE_mask_21]])), read<u32>(%[[VALUE_r_21]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeV:[0-9]+]] @retmeV(%[[VALUE_x_106:[0-9]+]] x: @type[[TYPE_V]]) -> @type[[TYPE_V]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_V]], reason=return>(read<@type[[TYPE_V]]>(%[[VALUE_x_106]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1V:[0-9]+]] @fn1V(%[[VALUE_x_107:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_43:[0-9]+]] y: @type[[TYPE_V]] [storage=automatic] = copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(%[[VALUE_sV]]));
// DEFAULT-NEXT:         let %[[VALUE278:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_y_43]]));
// DEFAULT-NEXT:         let %[[VALUE279:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE278]])))), read<u32>(%[[VALUE_x_107]])));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_y_43]]), read<u16>(%[[VALUE279]]));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(%[[VALUE_y_43]], copy<@type[[TYPE_V]], reason=assign>(call<@type[[TYPE_V]], signature=fn(@type[[TYPE_V]]) -> @type[[TYPE_V]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeV]], copy<@type[[TYPE_V]], reason=arg>(read<@type[[TYPE_V]]>(%[[VALUE_y_43]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_y_43]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2V:[0-9]+]] @fn2V(%[[VALUE_x_108:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_44:[0-9]+]] y: @type[[TYPE_V]] [storage=automatic] = copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(%[[VALUE_sV]]));
// DEFAULT-NEXT:         let %[[VALUE280:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_y_44]]));
// DEFAULT-NEXT:         let %[[VALUE281:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE280]])))), read<u32>(%[[VALUE_x_108]])));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_y_44]]), read<u16>(%[[VALUE281]]));
// DEFAULT-NEXT:         let %[[VALUE282:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_y_44]]));
// DEFAULT-NEXT:         let %[[VALUE283:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE282]]))), const<i32>(15))));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_y_44]]), read<u16>(%[[VALUE283]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_y_44]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitV:[0-9]+]] @retitV() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_sV]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3V:[0-9]+]] @fn3V(%[[VALUE_x_109:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE284:[0-9]+]]: u16 [synthetic] = read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_sV]]));
// DEFAULT-NEXT:         let %[[VALUE285:[0-9]+]]: u16 [synthetic] = truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE284]])))), read<u32>(%[[VALUE_x_109]])));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_sV]]), read<u16>(%[[VALUE285]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitV]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testV:[0-9]+]] @testV() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_22:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_22:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_22:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_22:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_22:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_110:[0-9]+]] x: @type[[TYPE_V]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_22:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_V]]>>(%[[VALUE_sV]]));
// DEFAULT-NEXT:         for %[[VALUE286:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_22]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_22]]))), const<u64>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE287:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_22]]);
// DEFAULT-NEXT:                 let %[[VALUE288:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE287]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_22]], read<i32>(%[[VALUE288]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE289:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_22]]);
// DEFAULT-NEXT:                 let %[[VALUE290:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE289]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_22]], read<ptr<i8>>(%[[VALUE290]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE289]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(1), const<i32>(8))
// DEFAULT-NEXT:             write<u16>(field3(%[[VALUE_sV]]), float_to_int<u16, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_sV]]), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_22]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_sV]]))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_22]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_22]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_sV]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_22]])));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(%[[VALUE_x_110]], copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(%[[VALUE_sV]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_22]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1V]], read<u32>(%[[VALUE_a_22]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_x_110]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sV]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_x_110]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_sV]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_x_110]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_sV]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_x_110]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_sV]])))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_22]]), read<u32>(%[[VALUE_a_22]])), read<u32>(%[[VALUE_mask_22]])), read<u32>(%[[VALUE_r_22]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_22]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_22]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_sV]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_22]])));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(%[[VALUE_x_110]], copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(%[[VALUE_sV]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_22]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2V]], read<u32>(%[[VALUE_a_22]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_x_110]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sV]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_x_110]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_sV]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_x_110]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_sV]])))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_x_110]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_sV]])))))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_22]]), read<u32>(%[[VALUE_a_22]])), read<u32>(%[[VALUE_mask_22]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_22]])), read<u32>(%[[VALUE_r_22]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_22]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_22]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_sV]]), truncate<u16, reason=assign, fits=unknown>(read<u32>(%[[VALUE_v_22]])));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(%[[VALUE_x_110]], copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(%[[VALUE_sV]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_22]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3V]], read<u32>(%[[VALUE_a_22]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_x_110]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield2<unit=0, bytes=0..2, bits=9..16>(%[[VALUE_sV]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_x_110]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield0<unit=0, bytes=0..2, bits=0..8>(%[[VALUE_sV]])))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(bitfield1<unit=0, bytes=0..2, bits=8..9>(%[[VALUE_sV]]))))), read<u32>(%[[VALUE_r_22]]))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_x_110]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field3(%[[VALUE_sV]])))))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_22]]), read<u32>(%[[VALUE_a_22]])), read<u32>(%[[VALUE_mask_22]])), read<u32>(%[[VALUE_r_22]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeW:[0-9]+]] @retmeW(%[[VALUE_x_111:[0-9]+]] x: @type[[TYPE_W]]) -> @type[[TYPE_W]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_W]], reason=return>(read<@type[[TYPE_W]]>(%[[VALUE_x_111]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1W:[0-9]+]] @fn1W(%[[VALUE_x_112:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_45:[0-9]+]] y: @type[[TYPE_W]] [storage=automatic] = copy<@type[[TYPE_W]], reason=assign>(read<@type[[TYPE_W]]>(%[[VALUE_sW]]));
// DEFAULT-NEXT:         let %[[VALUE291:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_y_45]]));
// DEFAULT-NEXT:         let %[[VALUE292:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE291]]))), read<u32>(%[[VALUE_x_112]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_y_45]]), read<u32>(%[[VALUE292]]));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(%[[VALUE_y_45]], copy<@type[[TYPE_W]], reason=assign>(call<@type[[TYPE_W]], signature=fn(@type[[TYPE_W]]) -> @type[[TYPE_W]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeW]], copy<@type[[TYPE_W]], reason=arg>(read<@type[[TYPE_W]]>(%[[VALUE_y_45]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_y_45]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2W:[0-9]+]] @fn2W(%[[VALUE_x_113:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_46:[0-9]+]] y: @type[[TYPE_W]] [storage=automatic] = copy<@type[[TYPE_W]], reason=assign>(read<@type[[TYPE_W]]>(%[[VALUE_sW]]));
// DEFAULT-NEXT:         let %[[VALUE293:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_y_46]]));
// DEFAULT-NEXT:         let %[[VALUE294:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE293]]))), read<u32>(%[[VALUE_x_113]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_y_46]]), read<u32>(%[[VALUE294]]));
// DEFAULT-NEXT:         let %[[VALUE295:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_y_46]]));
// DEFAULT-NEXT:         let %[[VALUE296:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE295]])), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_y_46]]), read<u32>(%[[VALUE296]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_y_46]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitW:[0-9]+]] @retitW() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_sW]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3W:[0-9]+]] @fn3W(%[[VALUE_x_114:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE297:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_sW]]));
// DEFAULT-NEXT:         let %[[VALUE298:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE297]]))), read<u32>(%[[VALUE_x_114]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_sW]]), read<u32>(%[[VALUE298]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitW]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testW:[0-9]+]] @testW() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_23:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_23:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_23:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_23:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_23:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_115:[0-9]+]] x: @type[[TYPE_W]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_23:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_W]]>>(%[[VALUE_sW]]));
// DEFAULT-NEXT:         for %[[VALUE299:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_23]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_23]]))), const<u64>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE300:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_23]]);
// DEFAULT-NEXT:                 let %[[VALUE301:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE300]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_23]], read<i32>(%[[VALUE301]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE302:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_23]]);
// DEFAULT-NEXT:                 let %[[VALUE303:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE302]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_23]], read<ptr<i8>>(%[[VALUE303]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE302]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(8), const<i32>(8))
// DEFAULT-NEXT:             write<f80>(field0(%[[VALUE_sW]]), float_widen<f80, reason=assign>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_sW]]), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_23]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_sW]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_23]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_23]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_sW]]), read<u32>(%[[VALUE_v_23]]));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(%[[VALUE_x_115]], copy<@type[[TYPE_W]], reason=assign>(read<@type[[TYPE_W]]>(%[[VALUE_sW]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_23]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1W]], read<u32>(%[[VALUE_a_23]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=25..32>(%[[VALUE_x_115]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=25..32>(%[[VALUE_sW]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=12..25>(%[[VALUE_x_115]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=12..25>(%[[VALUE_sW]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_x_115]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_sW]]))))), ne<f80, exceptions=observable>(read<f80>(field0(%[[VALUE_x_115]])), read<f80>(field0(%[[VALUE_sW]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_23]]), read<u32>(%[[VALUE_a_23]])), read<u32>(%[[VALUE_mask_23]])), read<u32>(%[[VALUE_r_23]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_23]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_23]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_sW]]), read<u32>(%[[VALUE_v_23]]));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(%[[VALUE_x_115]], copy<@type[[TYPE_W]], reason=assign>(read<@type[[TYPE_W]]>(%[[VALUE_sW]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_23]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2W]], read<u32>(%[[VALUE_a_23]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=25..32>(%[[VALUE_x_115]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=25..32>(%[[VALUE_sW]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=12..25>(%[[VALUE_x_115]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=12..25>(%[[VALUE_sW]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_x_115]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_sW]]))))), ne<f80, exceptions=observable>(read<f80>(field0(%[[VALUE_x_115]])), read<f80>(field0(%[[VALUE_sW]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_23]]), read<u32>(%[[VALUE_a_23]])), read<u32>(%[[VALUE_mask_23]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_23]])), read<u32>(%[[VALUE_r_23]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_23]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_23]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_sW]]), read<u32>(%[[VALUE_v_23]]));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(%[[VALUE_x_115]], copy<@type[[TYPE_W]], reason=assign>(read<@type[[TYPE_W]]>(%[[VALUE_sW]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_23]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3W]], read<u32>(%[[VALUE_a_23]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=25..32>(%[[VALUE_x_115]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=25..32>(%[[VALUE_sW]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=12..25>(%[[VALUE_x_115]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=12..25>(%[[VALUE_sW]]))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..12>(%[[VALUE_sW]])))), read<u32>(%[[VALUE_r_23]]))), ne<f80, exceptions=observable>(read<f80>(field0(%[[VALUE_x_115]])), read<f80>(field0(%[[VALUE_sW]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_23]]), read<u32>(%[[VALUE_a_23]])), read<u32>(%[[VALUE_mask_23]])), read<u32>(%[[VALUE_r_23]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeX:[0-9]+]] @retmeX(%[[VALUE_x_116:[0-9]+]] x: @type[[TYPE_X]]) -> @type[[TYPE_X]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_X]], reason=return>(read<@type[[TYPE_X]]>(%[[VALUE_x_116]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1X:[0-9]+]] @fn1X(%[[VALUE_x_117:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_47:[0-9]+]] y: @type[[TYPE_X]] [storage=automatic] = copy<@type[[TYPE_X]], reason=assign>(read<@type[[TYPE_X]]>(%[[VALUE_sX]]));
// DEFAULT-NEXT:         let %[[VALUE304:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_47]]));
// DEFAULT-NEXT:         let %[[VALUE305:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE304]]))), read<u32>(%[[VALUE_x_117]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_47]]), read<u32>(%[[VALUE305]]));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(%[[VALUE_y_47]], copy<@type[[TYPE_X]], reason=assign>(call<@type[[TYPE_X]], signature=fn(@type[[TYPE_X]]) -> @type[[TYPE_X]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeX]], copy<@type[[TYPE_X]], reason=arg>(read<@type[[TYPE_X]]>(%[[VALUE_y_47]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_47]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2X:[0-9]+]] @fn2X(%[[VALUE_x_118:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_48:[0-9]+]] y: @type[[TYPE_X]] [storage=automatic] = copy<@type[[TYPE_X]], reason=assign>(read<@type[[TYPE_X]]>(%[[VALUE_sX]]));
// DEFAULT-NEXT:         let %[[VALUE306:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_48]]));
// DEFAULT-NEXT:         let %[[VALUE307:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE306]]))), read<u32>(%[[VALUE_x_118]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_48]]), read<u32>(%[[VALUE307]]));
// DEFAULT-NEXT:         let %[[VALUE308:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_48]]));
// DEFAULT-NEXT:         let %[[VALUE309:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE308]])), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_48]]), read<u32>(%[[VALUE309]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_48]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitX:[0-9]+]] @retitX() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sX]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3X:[0-9]+]] @fn3X(%[[VALUE_x_119:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE310:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sX]]));
// DEFAULT-NEXT:         let %[[VALUE311:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE310]]))), read<u32>(%[[VALUE_x_119]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sX]]), read<u32>(%[[VALUE311]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitX]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testX:[0-9]+]] @testX() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_24:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_24:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_24:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_24:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_24:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_120:[0-9]+]] x: @type[[TYPE_X]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_24:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_X]]>>(%[[VALUE_sX]]));
// DEFAULT-NEXT:         for %[[VALUE312:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_24]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_24]]))), const<u64>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE313:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_24]]);
// DEFAULT-NEXT:                 let %[[VALUE314:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE313]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_24]], read<i32>(%[[VALUE314]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE315:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_24]]);
// DEFAULT-NEXT:                 let %[[VALUE316:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE315]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_24]], read<ptr<i8>>(%[[VALUE316]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE315]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(8), const<i32>(8))
// DEFAULT-NEXT:             write<f80>(field3(%[[VALUE_sX]]), float_widen<f80, reason=assign>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sX]]), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_24]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sX]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_24]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_24]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sX]]), read<u32>(%[[VALUE_v_24]]));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(%[[VALUE_x_120]], copy<@type[[TYPE_X]], reason=assign>(read<@type[[TYPE_X]]>(%[[VALUE_sX]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_24]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1X]], read<u32>(%[[VALUE_a_24]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_x_120]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sX]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_x_120]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_sX]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_x_120]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sX]]))))), ne<f80, exceptions=observable>(read<f80>(field3(%[[VALUE_x_120]])), read<f80>(field3(%[[VALUE_sX]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_24]]), read<u32>(%[[VALUE_a_24]])), read<u32>(%[[VALUE_mask_24]])), read<u32>(%[[VALUE_r_24]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_24]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_24]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sX]]), read<u32>(%[[VALUE_v_24]]));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(%[[VALUE_x_120]], copy<@type[[TYPE_X]], reason=assign>(read<@type[[TYPE_X]]>(%[[VALUE_sX]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_24]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2X]], read<u32>(%[[VALUE_a_24]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_x_120]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sX]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_x_120]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_sX]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_x_120]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sX]]))))), ne<f80, exceptions=observable>(read<f80>(field3(%[[VALUE_x_120]])), read<f80>(field3(%[[VALUE_sX]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_24]]), read<u32>(%[[VALUE_a_24]])), read<u32>(%[[VALUE_mask_24]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_24]])), read<u32>(%[[VALUE_r_24]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_24]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_24]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sX]]), read<u32>(%[[VALUE_v_24]]));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(%[[VALUE_x_120]], copy<@type[[TYPE_X]], reason=assign>(read<@type[[TYPE_X]]>(%[[VALUE_sX]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_24]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3X]], read<u32>(%[[VALUE_a_24]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_x_120]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=25..32>(%[[VALUE_sX]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_x_120]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..25>(%[[VALUE_sX]]))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sX]])))), read<u32>(%[[VALUE_r_24]]))), ne<f80, exceptions=observable>(read<f80>(field3(%[[VALUE_x_120]])), read<f80>(field3(%[[VALUE_sX]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_24]]), read<u32>(%[[VALUE_a_24]])), read<u32>(%[[VALUE_mask_24]])), read<u32>(%[[VALUE_r_24]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeY:[0-9]+]] @retmeY(%[[VALUE_x_121:[0-9]+]] x: @type[[TYPE_Y]]) -> @type[[TYPE_Y]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_Y]], reason=return>(read<@type[[TYPE_Y]]>(%[[VALUE_x_121]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1Y:[0-9]+]] @fn1Y(%[[VALUE_x_122:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_49:[0-9]+]] y: @type[[TYPE_Y]] [storage=automatic] = copy<@type[[TYPE_Y]], reason=assign>(read<@type[[TYPE_Y]]>(%[[VALUE_sY]]));
// DEFAULT-NEXT:         let %[[VALUE317:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_49]]));
// DEFAULT-NEXT:         let %[[VALUE318:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE317]]))), read<u32>(%[[VALUE_x_122]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_49]]), read<u32>(%[[VALUE318]]));
// DEFAULT-NEXT:         write<@type[[TYPE_Y]]>(%[[VALUE_y_49]], copy<@type[[TYPE_Y]], reason=assign>(call<@type[[TYPE_Y]], signature=fn(@type[[TYPE_Y]]) -> @type[[TYPE_Y]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeY]], copy<@type[[TYPE_Y]], reason=arg>(read<@type[[TYPE_Y]]>(%[[VALUE_y_49]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_49]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2Y:[0-9]+]] @fn2Y(%[[VALUE_x_123:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_50:[0-9]+]] y: @type[[TYPE_Y]] [storage=automatic] = copy<@type[[TYPE_Y]], reason=assign>(read<@type[[TYPE_Y]]>(%[[VALUE_sY]]));
// DEFAULT-NEXT:         let %[[VALUE319:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_50]]));
// DEFAULT-NEXT:         let %[[VALUE320:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE319]]))), read<u32>(%[[VALUE_x_123]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_50]]), read<u32>(%[[VALUE320]]));
// DEFAULT-NEXT:         let %[[VALUE321:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_50]]));
// DEFAULT-NEXT:         let %[[VALUE322:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE321]])), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_50]]), read<u32>(%[[VALUE322]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_y_50]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitY:[0-9]+]] @retitY() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sY]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3Y:[0-9]+]] @fn3Y(%[[VALUE_x_124:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE323:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sY]]));
// DEFAULT-NEXT:         let %[[VALUE324:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE323]]))), read<u32>(%[[VALUE_x_124]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sY]]), read<u32>(%[[VALUE324]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitY]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testY:[0-9]+]] @testY() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_25:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_25:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_25:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_25:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_25:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_125:[0-9]+]] x: @type[[TYPE_Y]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_25:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_Y]]>>(%[[VALUE_sY]]));
// DEFAULT-NEXT:         for %[[VALUE325:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_25]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_25]]))), const<u64>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE326:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_25]]);
// DEFAULT-NEXT:                 let %[[VALUE327:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE326]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_25]], read<i32>(%[[VALUE327]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE328:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_25]]);
// DEFAULT-NEXT:                 let %[[VALUE329:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE328]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_25]], read<ptr<i8>>(%[[VALUE329]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE328]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(8), const<i32>(8))
// DEFAULT-NEXT:             write<f80>(field3(%[[VALUE_sY]]), float_widen<f80, reason=assign>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sY]]), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_25]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sY]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_25]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_25]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sY]]), read<u32>(%[[VALUE_v_25]]));
// DEFAULT-NEXT:         write<@type[[TYPE_Y]]>(%[[VALUE_x_125]], copy<@type[[TYPE_Y]], reason=assign>(read<@type[[TYPE_Y]]>(%[[VALUE_sY]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_25]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1Y]], read<u32>(%[[VALUE_a_25]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_x_125]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sY]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_x_125]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_sY]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_x_125]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sY]]))))), ne<f80, exceptions=observable>(read<f80>(field3(%[[VALUE_x_125]])), read<f80>(field3(%[[VALUE_sY]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_25]]), read<u32>(%[[VALUE_a_25]])), read<u32>(%[[VALUE_mask_25]])), read<u32>(%[[VALUE_r_25]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_25]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_25]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sY]]), read<u32>(%[[VALUE_v_25]]));
// DEFAULT-NEXT:         write<@type[[TYPE_Y]]>(%[[VALUE_x_125]], copy<@type[[TYPE_Y]], reason=assign>(read<@type[[TYPE_Y]]>(%[[VALUE_sY]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_25]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2Y]], read<u32>(%[[VALUE_a_25]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_x_125]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sY]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_x_125]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_sY]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_x_125]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sY]]))))), ne<f80, exceptions=observable>(read<f80>(field3(%[[VALUE_x_125]])), read<f80>(field3(%[[VALUE_sY]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_25]]), read<u32>(%[[VALUE_a_25]])), read<u32>(%[[VALUE_mask_25]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_25]])), read<u32>(%[[VALUE_r_25]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_25]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_25]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sY]]), read<u32>(%[[VALUE_v_25]]));
// DEFAULT-NEXT:         write<@type[[TYPE_Y]]>(%[[VALUE_x_125]], copy<@type[[TYPE_Y]], reason=assign>(read<@type[[TYPE_Y]]>(%[[VALUE_sY]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_25]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3Y]], read<u32>(%[[VALUE_a_25]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_x_125]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=23..32>(%[[VALUE_sY]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_x_125]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=12..23>(%[[VALUE_sY]]))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..12>(%[[VALUE_sY]])))), read<u32>(%[[VALUE_r_25]]))), ne<f80, exceptions=observable>(read<f80>(field3(%[[VALUE_x_125]])), read<f80>(field3(%[[VALUE_sY]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_25]]), read<u32>(%[[VALUE_a_25]])), read<u32>(%[[VALUE_mask_25]])), read<u32>(%[[VALUE_r_25]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retmeZ:[0-9]+]] @retmeZ(%[[VALUE_x_126:[0-9]+]] x: @type[[TYPE_Z]]) -> @type[[TYPE_Z]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE_Z]], reason=return>(read<@type[[TYPE_Z]]>(%[[VALUE_x_126]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn1Z:[0-9]+]] @fn1Z(%[[VALUE_x_127:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_51:[0-9]+]] y: @type[[TYPE_Z]] [storage=automatic] = copy<@type[[TYPE_Z]], reason=assign>(read<@type[[TYPE_Z]]>(%[[VALUE_sZ]]));
// DEFAULT-NEXT:         let %[[VALUE330:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_y_51]]));
// DEFAULT-NEXT:         let %[[VALUE331:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE330]]))), read<u32>(%[[VALUE_x_127]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_y_51]]), read<u32>(%[[VALUE331]]));
// DEFAULT-NEXT:         write<@type[[TYPE_Z]]>(%[[VALUE_y_51]], copy<@type[[TYPE_Z]], reason=assign>(call<@type[[TYPE_Z]], signature=fn(@type[[TYPE_Z]]) -> @type[[TYPE_Z]], abi=sysv64(native_c) -> native_c>(%[[VALUE_retmeZ]], copy<@type[[TYPE_Z]], reason=arg>(read<@type[[TYPE_Z]]>(%[[VALUE_y_51]])))));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_y_51]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2Z:[0-9]+]] @fn2Z(%[[VALUE_x_128:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y_52:[0-9]+]] y: @type[[TYPE_Z]] [storage=automatic] = copy<@type[[TYPE_Z]], reason=assign>(read<@type[[TYPE_Z]]>(%[[VALUE_sZ]]));
// DEFAULT-NEXT:         let %[[VALUE332:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_y_52]]));
// DEFAULT-NEXT:         let %[[VALUE333:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE332]]))), read<u32>(%[[VALUE_x_128]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_y_52]]), read<u32>(%[[VALUE333]]));
// DEFAULT-NEXT:         let %[[VALUE334:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_y_52]]));
// DEFAULT-NEXT:         let %[[VALUE335:[0-9]+]]: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE334]])), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_y_52]]), read<u32>(%[[VALUE335]]));
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_y_52]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retitZ:[0-9]+]] @retitZ() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_sZ]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3Z:[0-9]+]] @fn3Z(%[[VALUE_x_129:[0-9]+]] x: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE336:[0-9]+]]: u32 [synthetic] = read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_sZ]]));
// DEFAULT-NEXT:         let %[[VALUE337:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%[[VALUE336]]))), read<u32>(%[[VALUE_x_129]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_sZ]]), read<u32>(%[[VALUE337]]));
// DEFAULT-NEXT:         return call<u32, signature=fn() -> u32>(%[[VALUE_retitZ]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testZ:[0-9]+]] @testZ() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_26:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_mask_26:[0-9]+]] mask: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v_26:[0-9]+]] v: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a_26:[0-9]+]] a: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r_26:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_130:[0-9]+]] x: @type[[TYPE_Z]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_p_26:[0-9]+]] p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<@type[[TYPE_Z]]>>(%[[VALUE_sZ]]));
// DEFAULT-NEXT:         for %[[VALUE338:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_26]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_26]]))), const<u64>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE339:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_26]]);
// DEFAULT-NEXT:                 let %[[VALUE340:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE339]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_26]], read<i32>(%[[VALUE340]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE341:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_p_26]]);
// DEFAULT-NEXT:                 let %[[VALUE342:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE341]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_p_26]], read<ptr<i8>>(%[[VALUE342]]));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%[[VALUE341]])), reinterpret<i8, reason=assign, fits=unknown>(truncate<u8, reason=assign, fits=unknown>(call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]))));
// DEFAULT-NEXT:         if eq<i32>(const<i32>(8), const<i32>(8))
// DEFAULT-NEXT:             write<f80>(field0(%[[VALUE_sZ]]), float_widen<f80, reason=assign>(const<f64>(5.25)));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_sZ]]), reinterpret<u32, reason=assign, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_mask_26]], reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_sZ]])))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_26]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_26]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_sZ]]), read<u32>(%[[VALUE_v_26]]));
// DEFAULT-NEXT:         write<@type[[TYPE_Z]]>(%[[VALUE_x_130]], copy<@type[[TYPE_Z]], reason=assign>(read<@type[[TYPE_Z]]>(%[[VALUE_sZ]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_26]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn1Z]], read<u32>(%[[VALUE_a_26]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=13..20>(%[[VALUE_x_130]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=13..20>(%[[VALUE_sZ]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..13>(%[[VALUE_x_130]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..13>(%[[VALUE_sZ]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_x_130]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_sZ]]))))), ne<f80, exceptions=observable>(read<f80>(field0(%[[VALUE_x_130]])), read<f80>(field0(%[[VALUE_sZ]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_26]]), read<u32>(%[[VALUE_a_26]])), read<u32>(%[[VALUE_mask_26]])), read<u32>(%[[VALUE_r_26]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_26]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_26]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_sZ]]), read<u32>(%[[VALUE_v_26]]));
// DEFAULT-NEXT:         write<@type[[TYPE_Z]]>(%[[VALUE_x_130]], copy<@type[[TYPE_Z]], reason=assign>(read<@type[[TYPE_Z]]>(%[[VALUE_sZ]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_26]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn2Z]], read<u32>(%[[VALUE_a_26]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=13..20>(%[[VALUE_x_130]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=13..20>(%[[VALUE_sZ]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..13>(%[[VALUE_x_130]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..13>(%[[VALUE_sZ]]))))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_x_130]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_sZ]]))))), ne<f80, exceptions=observable>(read<f80>(field0(%[[VALUE_x_130]])), read<f80>(field0(%[[VALUE_sZ]])))), ne<u32>(and<u32>(rem<u32, by_zero=ub>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_26]]), read<u32>(%[[VALUE_a_26]])), read<u32>(%[[VALUE_mask_26]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15))), read<u32>(%[[VALUE_mask_26]])), read<u32>(%[[VALUE_r_26]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<u32>(%[[VALUE_v_26]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_a_26]], call<u32, signature=fn() -> u32>(%[[VALUE_myrnd]]));
// DEFAULT-NEXT:         write<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_sZ]]), read<u32>(%[[VALUE_v_26]]));
// DEFAULT-NEXT:         write<@type[[TYPE_Z]]>(%[[VALUE_x_130]], copy<@type[[TYPE_Z]], reason=assign>(read<@type[[TYPE_Z]]>(%[[VALUE_sZ]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r_26]], call<u32, signature=fn(u32) -> u32>(%[[VALUE_fn3Z]], read<u32>(%[[VALUE_a_26]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=13..20>(%[[VALUE_x_130]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=16..20, bits=13..20>(%[[VALUE_sZ]])))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..13>(%[[VALUE_x_130]]))), reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=16..20, bits=0..13>(%[[VALUE_sZ]]))))), ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield3<unit=0, bytes=16..20, bits=20..32>(%[[VALUE_sZ]])))), read<u32>(%[[VALUE_r_26]]))), ne<f80, exceptions=observable>(read<f80>(field0(%[[VALUE_x_130]])), read<f80>(field0(%[[VALUE_sZ]])))), ne<u32>(and<u32>(add<u32, overflow=wrap>(read<u32>(%[[VALUE_v_26]]), read<u32>(%[[VALUE_a_26]])), read<u32>(%[[VALUE_mask_26]])), read<u32>(%[[VALUE_r_26]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testA]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testB]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testC]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testD]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testE]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testF]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testG]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testH]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testI]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testJ]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testK]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testL]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testM]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testN]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testO]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testP]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testQ]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testR]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testS]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testT]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testU]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testV]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testW]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testX]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testY]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testZ]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
