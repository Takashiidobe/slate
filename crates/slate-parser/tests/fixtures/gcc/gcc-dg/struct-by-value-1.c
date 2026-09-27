/* Test structure passing by value.  */
/* { dg-do run } */
/* { dg-options "-O2" } */

#define T(N)					\
struct S##N { unsigned char i[N]; };		\
struct S##N g1s##N, g2s##N, g3s##N;		\
						\
void						\
init##N (struct S##N *p, int i)			\
{						\
  int j;					\
  for (j = 0; j < N; j++)			\
    p->i[j] = i + j;				\
}						\
						\
void						\
check##N (struct S##N *p, int i)		\
{						\
  int j;					\
  for (j = 0; j < N; j++)			\
    if (p->i[j] != i + j) abort ();		\
}						\
						\
void						\
test##N (struct S##N s1, struct S##N s2,	\
	 struct S##N s3)			\
{						\
  check##N (&s1, 64);				\
  check##N (&s2, 128);				\
  check##N (&s3, 192);				\
}						\
						\
void						\
test2_##N (struct S##N s1, struct S##N s2)	\
{						\
  test##N (s1, g2s##N, s2);			\
}						\
						\
void						\
testit##N (void)				\
{						\
  init##N (&g1s##N, 64);			\
  check##N (&g1s##N, 64);			\
  init##N (&g2s##N, 128);			\
  check##N (&g2s##N, 128);			\
  init##N (&g3s##N, 192);			\
  check##N (&g3s##N, 192);			\
  test##N (g1s##N, g2s##N, g3s##N);		\
  test2_##N (g1s##N, g3s##N);			\
}

extern void abort (void);
extern void exit (int);

T(0) T(1) T(2) T(3) T(4) T(5) T(6) T(7)
T(8) T(9) T(10) T(11) T(12) T(13) T(14) T(15)
T(16) T(17) T(18) T(19) T(20) T(21) T(22) T(23)
T(24) T(25) T(26) T(27) T(28) T(29) T(30) T(31)
T(32) T(33) T(34) T(35) T(36) T(37) T(38) T(39)
T(40) T(41) T(42) T(43) T(44) T(45) T(46) T(47)
T(48) T(49) T(50) T(51) T(52) T(53) T(54) T(55)
T(56) T(57) T(58) T(59) T(60) T(61) T(62) T(63)

#undef T

int
main ()
{
#define T(N) testit##N ();

T(0) T(1) T(2) T(3) T(4) T(5) T(6) T(7)
T(8) T(9) T(10) T(11) T(12) T(13) T(14) T(15)
T(16) T(17) T(18) T(19) T(20) T(21) T(22) T(23)
T(24) T(25) T(26) T(27) T(28) T(29) T(30) T(31)
T(32) T(33) T(34) T(35) T(36) T(37) T(38) T(39)
T(40) T(41) T(42) T(43) T(44) T(45) T(46) T(47)
T(48) T(49) T(50) T(51) T(52) T(53) T(54) T(55)
T(56) T(57) T(58) T(59) T(60) T(61) T(62) T(63)

#undef T
  exit (0);
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type0 S0 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 0>;
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 S1 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 1>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type2 S2 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 2>;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type3 S3 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 3>;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type4 S4 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 4>;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type5 S5 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 5>;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type6 S6 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 6>;
// DEFAULT-NEXT:     } [size=6, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type7 S7 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 7>;
// DEFAULT-NEXT:     } [size=7, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type8 S8 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 8>;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type9 S9 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 9>;
// DEFAULT-NEXT:     } [size=9, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type10 S10 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 10>;
// DEFAULT-NEXT:     } [size=10, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type11 S11 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 11>;
// DEFAULT-NEXT:     } [size=11, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type12 S12 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 12>;
// DEFAULT-NEXT:     } [size=12, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type13 S13 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 13>;
// DEFAULT-NEXT:     } [size=13, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type14 S14 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 14>;
// DEFAULT-NEXT:     } [size=14, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type15 S15 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 15>;
// DEFAULT-NEXT:     } [size=15, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type16 S16 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 16>;
// DEFAULT-NEXT:     } [size=16, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type17 S17 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 17>;
// DEFAULT-NEXT:     } [size=17, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type18 S18 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 18>;
// DEFAULT-NEXT:     } [size=18, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type19 S19 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 19>;
// DEFAULT-NEXT:     } [size=19, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type20 S20 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 20>;
// DEFAULT-NEXT:     } [size=20, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type21 S21 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 21>;
// DEFAULT-NEXT:     } [size=21, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type22 S22 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 22>;
// DEFAULT-NEXT:     } [size=22, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type23 S23 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 23>;
// DEFAULT-NEXT:     } [size=23, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type24 S24 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 24>;
// DEFAULT-NEXT:     } [size=24, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type25 S25 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 25>;
// DEFAULT-NEXT:     } [size=25, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type26 S26 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 26>;
// DEFAULT-NEXT:     } [size=26, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type27 S27 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 27>;
// DEFAULT-NEXT:     } [size=27, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type28 S28 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 28>;
// DEFAULT-NEXT:     } [size=28, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type29 S29 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 29>;
// DEFAULT-NEXT:     } [size=29, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type30 S30 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 30>;
// DEFAULT-NEXT:     } [size=30, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type31 S31 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 31>;
// DEFAULT-NEXT:     } [size=31, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type32 S32 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 32>;
// DEFAULT-NEXT:     } [size=32, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type33 S33 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 33>;
// DEFAULT-NEXT:     } [size=33, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type34 S34 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 34>;
// DEFAULT-NEXT:     } [size=34, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type35 S35 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 35>;
// DEFAULT-NEXT:     } [size=35, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type36 S36 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 36>;
// DEFAULT-NEXT:     } [size=36, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type37 S37 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 37>;
// DEFAULT-NEXT:     } [size=37, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type38 S38 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 38>;
// DEFAULT-NEXT:     } [size=38, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type39 S39 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 39>;
// DEFAULT-NEXT:     } [size=39, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type40 S40 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 40>;
// DEFAULT-NEXT:     } [size=40, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type41 S41 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 41>;
// DEFAULT-NEXT:     } [size=41, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type42 S42 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 42>;
// DEFAULT-NEXT:     } [size=42, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type43 S43 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 43>;
// DEFAULT-NEXT:     } [size=43, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type44 S44 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 44>;
// DEFAULT-NEXT:     } [size=44, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type45 S45 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 45>;
// DEFAULT-NEXT:     } [size=45, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type46 S46 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 46>;
// DEFAULT-NEXT:     } [size=46, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type47 S47 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 47>;
// DEFAULT-NEXT:     } [size=47, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type48 S48 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 48>;
// DEFAULT-NEXT:     } [size=48, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type49 S49 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 49>;
// DEFAULT-NEXT:     } [size=49, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type50 S50 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 50>;
// DEFAULT-NEXT:     } [size=50, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type51 S51 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 51>;
// DEFAULT-NEXT:     } [size=51, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type52 S52 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 52>;
// DEFAULT-NEXT:     } [size=52, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type53 S53 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 53>;
// DEFAULT-NEXT:     } [size=53, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type54 S54 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 54>;
// DEFAULT-NEXT:     } [size=54, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type55 S55 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 55>;
// DEFAULT-NEXT:     } [size=55, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type56 S56 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 56>;
// DEFAULT-NEXT:     } [size=56, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type57 S57 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 57>;
// DEFAULT-NEXT:     } [size=57, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type58 S58 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 58>;
// DEFAULT-NEXT:     } [size=58, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type59 S59 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 59>;
// DEFAULT-NEXT:     } [size=59, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type60 S60 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 60>;
// DEFAULT-NEXT:     } [size=60, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type61 S61 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 61>;
// DEFAULT-NEXT:     } [size=61, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type62 S62 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 62>;
// DEFAULT-NEXT:     } [size=62, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type63 S63 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 63>;
// DEFAULT-NEXT:     } [size=63, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %3 g1s0: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 g2s0: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 g3s0: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %23 g1s1: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %24 g2s1: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %25 g3s1: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %43 g1s2: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %44 g2s2: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %45 g3s2: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %63 g1s3: @type3 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %64 g2s3: @type3 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %65 g3s3: @type3 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %83 g1s4: @type4 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %84 g2s4: @type4 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %85 g3s4: @type4 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %103 g1s5: @type5 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %104 g2s5: @type5 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %105 g3s5: @type5 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %123 g1s6: @type6 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %124 g2s6: @type6 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %125 g3s6: @type6 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %143 g1s7: @type7 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %144 g2s7: @type7 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %145 g3s7: @type7 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %163 g1s8: @type8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %164 g2s8: @type8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %165 g3s8: @type8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %183 g1s9: @type9 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %184 g2s9: @type9 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %185 g3s9: @type9 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %203 g1s10: @type10 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %204 g2s10: @type10 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %205 g3s10: @type10 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %223 g1s11: @type11 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %224 g2s11: @type11 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %225 g3s11: @type11 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %243 g1s12: @type12 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %244 g2s12: @type12 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %245 g3s12: @type12 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %263 g1s13: @type13 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %264 g2s13: @type13 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %265 g3s13: @type13 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %283 g1s14: @type14 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %284 g2s14: @type14 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %285 g3s14: @type14 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %303 g1s15: @type15 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %304 g2s15: @type15 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %305 g3s15: @type15 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %323 g1s16: @type16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %324 g2s16: @type16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %325 g3s16: @type16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %343 g1s17: @type17 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %344 g2s17: @type17 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %345 g3s17: @type17 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %363 g1s18: @type18 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %364 g2s18: @type18 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %365 g3s18: @type18 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %383 g1s19: @type19 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %384 g2s19: @type19 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %385 g3s19: @type19 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %403 g1s20: @type20 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %404 g2s20: @type20 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %405 g3s20: @type20 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %423 g1s21: @type21 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %424 g2s21: @type21 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %425 g3s21: @type21 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %443 g1s22: @type22 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %444 g2s22: @type22 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %445 g3s22: @type22 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %463 g1s23: @type23 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %464 g2s23: @type23 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %465 g3s23: @type23 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %483 g1s24: @type24 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %484 g2s24: @type24 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %485 g3s24: @type24 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %503 g1s25: @type25 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %504 g2s25: @type25 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %505 g3s25: @type25 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %523 g1s26: @type26 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %524 g2s26: @type26 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %525 g3s26: @type26 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %543 g1s27: @type27 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %544 g2s27: @type27 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %545 g3s27: @type27 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %563 g1s28: @type28 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %564 g2s28: @type28 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %565 g3s28: @type28 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %583 g1s29: @type29 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %584 g2s29: @type29 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %585 g3s29: @type29 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %603 g1s30: @type30 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %604 g2s30: @type30 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %605 g3s30: @type30 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %623 g1s31: @type31 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %624 g2s31: @type31 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %625 g3s31: @type31 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %643 g1s32: @type32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %644 g2s32: @type32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %645 g3s32: @type32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %663 g1s33: @type33 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %664 g2s33: @type33 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %665 g3s33: @type33 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %683 g1s34: @type34 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %684 g2s34: @type34 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %685 g3s34: @type34 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %703 g1s35: @type35 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %704 g2s35: @type35 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %705 g3s35: @type35 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %723 g1s36: @type36 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %724 g2s36: @type36 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %725 g3s36: @type36 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %743 g1s37: @type37 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %744 g2s37: @type37 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %745 g3s37: @type37 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %763 g1s38: @type38 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %764 g2s38: @type38 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %765 g3s38: @type38 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %783 g1s39: @type39 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %784 g2s39: @type39 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %785 g3s39: @type39 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %803 g1s40: @type40 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %804 g2s40: @type40 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %805 g3s40: @type40 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %823 g1s41: @type41 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %824 g2s41: @type41 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %825 g3s41: @type41 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %843 g1s42: @type42 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %844 g2s42: @type42 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %845 g3s42: @type42 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %863 g1s43: @type43 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %864 g2s43: @type43 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %865 g3s43: @type43 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %883 g1s44: @type44 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %884 g2s44: @type44 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %885 g3s44: @type44 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %903 g1s45: @type45 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %904 g2s45: @type45 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %905 g3s45: @type45 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %923 g1s46: @type46 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %924 g2s46: @type46 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %925 g3s46: @type46 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %943 g1s47: @type47 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %944 g2s47: @type47 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %945 g3s47: @type47 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %963 g1s48: @type48 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %964 g2s48: @type48 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %965 g3s48: @type48 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %983 g1s49: @type49 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %984 g2s49: @type49 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %985 g3s49: @type49 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1003 g1s50: @type50 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1004 g2s50: @type50 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1005 g3s50: @type50 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1023 g1s51: @type51 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1024 g2s51: @type51 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1025 g3s51: @type51 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1043 g1s52: @type52 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1044 g2s52: @type52 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1045 g3s52: @type52 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1063 g1s53: @type53 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1064 g2s53: @type53 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1065 g3s53: @type53 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1083 g1s54: @type54 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1084 g2s54: @type54 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1085 g3s54: @type54 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1103 g1s55: @type55 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1104 g2s55: @type55 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1105 g3s55: @type55 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1123 g1s56: @type56 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1124 g2s56: @type56 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1125 g3s56: @type56 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1143 g1s57: @type57 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1144 g2s57: @type57 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1145 g3s57: @type57 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1163 g1s58: @type58 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1164 g2s58: @type58 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1165 g3s58: @type58 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1183 g1s59: @type59 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1184 g2s59: @type59 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1185 g3s59: @type59 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1203 g1s60: @type60 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1204 g2s60: @type60 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1205 g3s60: @type60 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1223 g1s61: @type61 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1224 g2s61: @type61 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1225 g3s61: @type61 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1243 g1s62: @type62 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1244 g2s62: @type62 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1245 g3s62: @type62 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1263 g1s63: @type63 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1264 g2s63: @type63 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1265 g3s63: @type63 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%1283 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @init0(%7 p: ptr<@type0>, %8 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1284
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%9), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1412: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %1413: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1412), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%1413));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(0)>(field0(deref(read<ptr<@type0>>(%7)))), read<i32>(%9))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%8), read<i32>(%9)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @check0(%11 p: ptr<@type0>, %12 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1285
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%13, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%13), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1414: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %1415: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1414), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%1415));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(0)>(field0(deref(read<ptr<@type0>>(%11)))), read<i32>(%13)))))), add<i32, overflow=ub>(read<i32>(%12), read<i32>(%13)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test0(%15 s1: @type0, %16 s2: @type0, %17 s3: @type0) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%10, addr_of<ptr<@type0>>(%15), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%10, addr_of<ptr<@type0>>(%16), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%10, addr_of<ptr<@type0>>(%17), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test2_0(%19 s1: @type0, %20 s2: @type0) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type0, @type0, @type0) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%14, copy<@type0, reason=arg>(read<@type0>(%19)), copy<@type0, reason=arg>(read<@type0>(%4)), copy<@type0, reason=arg>(read<@type0>(%20)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @testit0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%6, addr_of<ptr<@type0>>(%3), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%10, addr_of<ptr<@type0>>(%3), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%6, addr_of<ptr<@type0>>(%4), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%10, addr_of<ptr<@type0>>(%4), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%6, addr_of<ptr<@type0>>(%5), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%10, addr_of<ptr<@type0>>(%5), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type0, @type0, @type0) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%14, copy<@type0, reason=arg>(read<@type0>(%3)), copy<@type0, reason=arg>(read<@type0>(%4)), copy<@type0, reason=arg>(read<@type0>(%5)));
// DEFAULT-NEXT:         call<void, signature=fn(@type0, @type0) -> void, abi=sysv64(native_c, native_c) -> void>(%18, copy<@type0, reason=arg>(read<@type0>(%3)), copy<@type0, reason=arg>(read<@type0>(%5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @init1(%27 p: ptr<@type1>, %28 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %29 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1286
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%29, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%29), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1416: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:                 let %1417: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1416), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%29, read<i32>(%1417));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1)>(field0(deref(read<ptr<@type1>>(%27)))), read<i32>(%29))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%28), read<i32>(%29)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @check1(%31 p: ptr<@type1>, %32 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %33 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1287
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%33, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%33), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1418: i32 [synthetic] = read<i32>(%33);
// DEFAULT-NEXT:                 let %1419: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1418), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%33, read<i32>(%1419));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1)>(field0(deref(read<ptr<@type1>>(%31)))), read<i32>(%33)))))), add<i32, overflow=ub>(read<i32>(%32), read<i32>(%33)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @test1(%35 s1: @type1, %36 s2: @type1, %37 s3: @type1) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>, i32) -> void>(%30, addr_of<ptr<@type1>>(%35), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>, i32) -> void>(%30, addr_of<ptr<@type1>>(%36), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>, i32) -> void>(%30, addr_of<ptr<@type1>>(%37), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @test2_1(%39 s1: @type1, %40 s2: @type1) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type1, @type1, @type1) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%34, copy<@type1, reason=arg>(read<@type1>(%39)), copy<@type1, reason=arg>(read<@type1>(%24)), copy<@type1, reason=arg>(read<@type1>(%40)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @testit1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>, i32) -> void>(%26, addr_of<ptr<@type1>>(%23), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>, i32) -> void>(%30, addr_of<ptr<@type1>>(%23), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>, i32) -> void>(%26, addr_of<ptr<@type1>>(%24), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>, i32) -> void>(%30, addr_of<ptr<@type1>>(%24), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>, i32) -> void>(%26, addr_of<ptr<@type1>>(%25), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>, i32) -> void>(%30, addr_of<ptr<@type1>>(%25), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type1, @type1, @type1) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%34, copy<@type1, reason=arg>(read<@type1>(%23)), copy<@type1, reason=arg>(read<@type1>(%24)), copy<@type1, reason=arg>(read<@type1>(%25)));
// DEFAULT-NEXT:         call<void, signature=fn(@type1, @type1) -> void, abi=sysv64(native_c, native_c) -> void>(%38, copy<@type1, reason=arg>(read<@type1>(%23)), copy<@type1, reason=arg>(read<@type1>(%25)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @init2(%47 p: ptr<@type2>, %48 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %49 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1288
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%49, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%49), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1420: i32 [synthetic] = read<i32>(%49);
// DEFAULT-NEXT:                 let %1421: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1420), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%49, read<i32>(%1421));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(2)>(field0(deref(read<ptr<@type2>>(%47)))), read<i32>(%49))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%48), read<i32>(%49)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @check2(%51 p: ptr<@type2>, %52 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %53 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1289
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%53, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%53), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1422: i32 [synthetic] = read<i32>(%53);
// DEFAULT-NEXT:                 let %1423: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1422), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%53, read<i32>(%1423));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(2)>(field0(deref(read<ptr<@type2>>(%51)))), read<i32>(%53)))))), add<i32, overflow=ub>(read<i32>(%52), read<i32>(%53)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @test2(%55 s1: @type2, %56 s2: @type2, %57 s3: @type2) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>, i32) -> void>(%50, addr_of<ptr<@type2>>(%55), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>, i32) -> void>(%50, addr_of<ptr<@type2>>(%56), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>, i32) -> void>(%50, addr_of<ptr<@type2>>(%57), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @test2_2(%59 s1: @type2, %60 s2: @type2) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type2, @type2, @type2) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%54, copy<@type2, reason=arg>(read<@type2>(%59)), copy<@type2, reason=arg>(read<@type2>(%44)), copy<@type2, reason=arg>(read<@type2>(%60)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %61 @testit2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>, i32) -> void>(%46, addr_of<ptr<@type2>>(%43), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>, i32) -> void>(%50, addr_of<ptr<@type2>>(%43), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>, i32) -> void>(%46, addr_of<ptr<@type2>>(%44), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>, i32) -> void>(%50, addr_of<ptr<@type2>>(%44), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>, i32) -> void>(%46, addr_of<ptr<@type2>>(%45), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>, i32) -> void>(%50, addr_of<ptr<@type2>>(%45), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type2, @type2, @type2) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%54, copy<@type2, reason=arg>(read<@type2>(%43)), copy<@type2, reason=arg>(read<@type2>(%44)), copy<@type2, reason=arg>(read<@type2>(%45)));
// DEFAULT-NEXT:         call<void, signature=fn(@type2, @type2) -> void, abi=sysv64(native_c, native_c) -> void>(%58, copy<@type2, reason=arg>(read<@type2>(%43)), copy<@type2, reason=arg>(read<@type2>(%45)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @init3(%67 p: ptr<@type3>, %68 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %69 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1290
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%69, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%69), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1424: i32 [synthetic] = read<i32>(%69);
// DEFAULT-NEXT:                 let %1425: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1424), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%69, read<i32>(%1425));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(3)>(field0(deref(read<ptr<@type3>>(%67)))), read<i32>(%69))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%68), read<i32>(%69)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %70 @check3(%71 p: ptr<@type3>, %72 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %73 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1291
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%73, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%73), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1426: i32 [synthetic] = read<i32>(%73);
// DEFAULT-NEXT:                 let %1427: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1426), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%73, read<i32>(%1427));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(3)>(field0(deref(read<ptr<@type3>>(%71)))), read<i32>(%73)))))), add<i32, overflow=ub>(read<i32>(%72), read<i32>(%73)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %74 @test3(%75 s1: @type3, %76 s2: @type3, %77 s3: @type3) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type3>, i32) -> void>(%70, addr_of<ptr<@type3>>(%75), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type3>, i32) -> void>(%70, addr_of<ptr<@type3>>(%76), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type3>, i32) -> void>(%70, addr_of<ptr<@type3>>(%77), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %78 @test2_3(%79 s1: @type3, %80 s2: @type3) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type3, @type3, @type3) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%74, copy<@type3, reason=arg>(read<@type3>(%79)), copy<@type3, reason=arg>(read<@type3>(%64)), copy<@type3, reason=arg>(read<@type3>(%80)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %81 @testit3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type3>, i32) -> void>(%66, addr_of<ptr<@type3>>(%63), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type3>, i32) -> void>(%70, addr_of<ptr<@type3>>(%63), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type3>, i32) -> void>(%66, addr_of<ptr<@type3>>(%64), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type3>, i32) -> void>(%70, addr_of<ptr<@type3>>(%64), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type3>, i32) -> void>(%66, addr_of<ptr<@type3>>(%65), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type3>, i32) -> void>(%70, addr_of<ptr<@type3>>(%65), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type3, @type3, @type3) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%74, copy<@type3, reason=arg>(read<@type3>(%63)), copy<@type3, reason=arg>(read<@type3>(%64)), copy<@type3, reason=arg>(read<@type3>(%65)));
// DEFAULT-NEXT:         call<void, signature=fn(@type3, @type3) -> void, abi=sysv64(native_c, native_c) -> void>(%78, copy<@type3, reason=arg>(read<@type3>(%63)), copy<@type3, reason=arg>(read<@type3>(%65)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %86 @init4(%87 p: ptr<@type4>, %88 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %89 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1292
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%89, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%89), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1428: i32 [synthetic] = read<i32>(%89);
// DEFAULT-NEXT:                 let %1429: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1428), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%89, read<i32>(%1429));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(field0(deref(read<ptr<@type4>>(%87)))), read<i32>(%89))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%88), read<i32>(%89)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %90 @check4(%91 p: ptr<@type4>, %92 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %93 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1293
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%93, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%93), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1430: i32 [synthetic] = read<i32>(%93);
// DEFAULT-NEXT:                 let %1431: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1430), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%93, read<i32>(%1431));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(field0(deref(read<ptr<@type4>>(%91)))), read<i32>(%93)))))), add<i32, overflow=ub>(read<i32>(%92), read<i32>(%93)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %94 @test4(%95 s1: @type4, %96 s2: @type4, %97 s3: @type4) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>, i32) -> void>(%90, addr_of<ptr<@type4>>(%95), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>, i32) -> void>(%90, addr_of<ptr<@type4>>(%96), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>, i32) -> void>(%90, addr_of<ptr<@type4>>(%97), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %98 @test2_4(%99 s1: @type4, %100 s2: @type4) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type4, @type4, @type4) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%94, copy<@type4, reason=arg>(read<@type4>(%99)), copy<@type4, reason=arg>(read<@type4>(%84)), copy<@type4, reason=arg>(read<@type4>(%100)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %101 @testit4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>, i32) -> void>(%86, addr_of<ptr<@type4>>(%83), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>, i32) -> void>(%90, addr_of<ptr<@type4>>(%83), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>, i32) -> void>(%86, addr_of<ptr<@type4>>(%84), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>, i32) -> void>(%90, addr_of<ptr<@type4>>(%84), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>, i32) -> void>(%86, addr_of<ptr<@type4>>(%85), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type4>, i32) -> void>(%90, addr_of<ptr<@type4>>(%85), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type4, @type4, @type4) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%94, copy<@type4, reason=arg>(read<@type4>(%83)), copy<@type4, reason=arg>(read<@type4>(%84)), copy<@type4, reason=arg>(read<@type4>(%85)));
// DEFAULT-NEXT:         call<void, signature=fn(@type4, @type4) -> void, abi=sysv64(native_c, native_c) -> void>(%98, copy<@type4, reason=arg>(read<@type4>(%83)), copy<@type4, reason=arg>(read<@type4>(%85)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %106 @init5(%107 p: ptr<@type5>, %108 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %109 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1294
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%109, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%109), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1432: i32 [synthetic] = read<i32>(%109);
// DEFAULT-NEXT:                 let %1433: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1432), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%109, read<i32>(%1433));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(5)>(field0(deref(read<ptr<@type5>>(%107)))), read<i32>(%109))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%108), read<i32>(%109)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %110 @check5(%111 p: ptr<@type5>, %112 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %113 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1295
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%113, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%113), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1434: i32 [synthetic] = read<i32>(%113);
// DEFAULT-NEXT:                 let %1435: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1434), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%113, read<i32>(%1435));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(5)>(field0(deref(read<ptr<@type5>>(%111)))), read<i32>(%113)))))), add<i32, overflow=ub>(read<i32>(%112), read<i32>(%113)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %114 @test5(%115 s1: @type5, %116 s2: @type5, %117 s3: @type5) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type5>, i32) -> void>(%110, addr_of<ptr<@type5>>(%115), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type5>, i32) -> void>(%110, addr_of<ptr<@type5>>(%116), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type5>, i32) -> void>(%110, addr_of<ptr<@type5>>(%117), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %118 @test2_5(%119 s1: @type5, %120 s2: @type5) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type5, @type5, @type5) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%114, copy<@type5, reason=arg>(read<@type5>(%119)), copy<@type5, reason=arg>(read<@type5>(%104)), copy<@type5, reason=arg>(read<@type5>(%120)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %121 @testit5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type5>, i32) -> void>(%106, addr_of<ptr<@type5>>(%103), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type5>, i32) -> void>(%110, addr_of<ptr<@type5>>(%103), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type5>, i32) -> void>(%106, addr_of<ptr<@type5>>(%104), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type5>, i32) -> void>(%110, addr_of<ptr<@type5>>(%104), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type5>, i32) -> void>(%106, addr_of<ptr<@type5>>(%105), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type5>, i32) -> void>(%110, addr_of<ptr<@type5>>(%105), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type5, @type5, @type5) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%114, copy<@type5, reason=arg>(read<@type5>(%103)), copy<@type5, reason=arg>(read<@type5>(%104)), copy<@type5, reason=arg>(read<@type5>(%105)));
// DEFAULT-NEXT:         call<void, signature=fn(@type5, @type5) -> void, abi=sysv64(native_c, native_c) -> void>(%118, copy<@type5, reason=arg>(read<@type5>(%103)), copy<@type5, reason=arg>(read<@type5>(%105)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %126 @init6(%127 p: ptr<@type6>, %128 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %129 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1296
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%129, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%129), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1436: i32 [synthetic] = read<i32>(%129);
// DEFAULT-NEXT:                 let %1437: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1436), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%129, read<i32>(%1437));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(6)>(field0(deref(read<ptr<@type6>>(%127)))), read<i32>(%129))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%128), read<i32>(%129)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %130 @check6(%131 p: ptr<@type6>, %132 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %133 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1297
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%133, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%133), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1438: i32 [synthetic] = read<i32>(%133);
// DEFAULT-NEXT:                 let %1439: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1438), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%133, read<i32>(%1439));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(6)>(field0(deref(read<ptr<@type6>>(%131)))), read<i32>(%133)))))), add<i32, overflow=ub>(read<i32>(%132), read<i32>(%133)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %134 @test6(%135 s1: @type6, %136 s2: @type6, %137 s3: @type6) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type6>, i32) -> void>(%130, addr_of<ptr<@type6>>(%135), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type6>, i32) -> void>(%130, addr_of<ptr<@type6>>(%136), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type6>, i32) -> void>(%130, addr_of<ptr<@type6>>(%137), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %138 @test2_6(%139 s1: @type6, %140 s2: @type6) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type6, @type6, @type6) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%134, copy<@type6, reason=arg>(read<@type6>(%139)), copy<@type6, reason=arg>(read<@type6>(%124)), copy<@type6, reason=arg>(read<@type6>(%140)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %141 @testit6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type6>, i32) -> void>(%126, addr_of<ptr<@type6>>(%123), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type6>, i32) -> void>(%130, addr_of<ptr<@type6>>(%123), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type6>, i32) -> void>(%126, addr_of<ptr<@type6>>(%124), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type6>, i32) -> void>(%130, addr_of<ptr<@type6>>(%124), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type6>, i32) -> void>(%126, addr_of<ptr<@type6>>(%125), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type6>, i32) -> void>(%130, addr_of<ptr<@type6>>(%125), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type6, @type6, @type6) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%134, copy<@type6, reason=arg>(read<@type6>(%123)), copy<@type6, reason=arg>(read<@type6>(%124)), copy<@type6, reason=arg>(read<@type6>(%125)));
// DEFAULT-NEXT:         call<void, signature=fn(@type6, @type6) -> void, abi=sysv64(native_c, native_c) -> void>(%138, copy<@type6, reason=arg>(read<@type6>(%123)), copy<@type6, reason=arg>(read<@type6>(%125)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %146 @init7(%147 p: ptr<@type7>, %148 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %149 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1298
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%149, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%149), const<i32>(7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1440: i32 [synthetic] = read<i32>(%149);
// DEFAULT-NEXT:                 let %1441: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1440), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%149, read<i32>(%1441));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(7)>(field0(deref(read<ptr<@type7>>(%147)))), read<i32>(%149))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%148), read<i32>(%149)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %150 @check7(%151 p: ptr<@type7>, %152 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %153 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1299
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%153, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%153), const<i32>(7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1442: i32 [synthetic] = read<i32>(%153);
// DEFAULT-NEXT:                 let %1443: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1442), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%153, read<i32>(%1443));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(7)>(field0(deref(read<ptr<@type7>>(%151)))), read<i32>(%153)))))), add<i32, overflow=ub>(read<i32>(%152), read<i32>(%153)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %154 @test7(%155 s1: @type7, %156 s2: @type7, %157 s3: @type7) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type7>, i32) -> void>(%150, addr_of<ptr<@type7>>(%155), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type7>, i32) -> void>(%150, addr_of<ptr<@type7>>(%156), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type7>, i32) -> void>(%150, addr_of<ptr<@type7>>(%157), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %158 @test2_7(%159 s1: @type7, %160 s2: @type7) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type7, @type7, @type7) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%154, copy<@type7, reason=arg>(read<@type7>(%159)), copy<@type7, reason=arg>(read<@type7>(%144)), copy<@type7, reason=arg>(read<@type7>(%160)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %161 @testit7() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type7>, i32) -> void>(%146, addr_of<ptr<@type7>>(%143), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type7>, i32) -> void>(%150, addr_of<ptr<@type7>>(%143), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type7>, i32) -> void>(%146, addr_of<ptr<@type7>>(%144), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type7>, i32) -> void>(%150, addr_of<ptr<@type7>>(%144), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type7>, i32) -> void>(%146, addr_of<ptr<@type7>>(%145), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type7>, i32) -> void>(%150, addr_of<ptr<@type7>>(%145), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type7, @type7, @type7) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%154, copy<@type7, reason=arg>(read<@type7>(%143)), copy<@type7, reason=arg>(read<@type7>(%144)), copy<@type7, reason=arg>(read<@type7>(%145)));
// DEFAULT-NEXT:         call<void, signature=fn(@type7, @type7) -> void, abi=sysv64(native_c, native_c) -> void>(%158, copy<@type7, reason=arg>(read<@type7>(%143)), copy<@type7, reason=arg>(read<@type7>(%145)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %166 @init8(%167 p: ptr<@type8>, %168 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %169 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1300
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%169, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%169), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1444: i32 [synthetic] = read<i32>(%169);
// DEFAULT-NEXT:                 let %1445: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1444), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%169, read<i32>(%1445));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field0(deref(read<ptr<@type8>>(%167)))), read<i32>(%169))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%168), read<i32>(%169)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %170 @check8(%171 p: ptr<@type8>, %172 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %173 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1301
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%173, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%173), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1446: i32 [synthetic] = read<i32>(%173);
// DEFAULT-NEXT:                 let %1447: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1446), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%173, read<i32>(%1447));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field0(deref(read<ptr<@type8>>(%171)))), read<i32>(%173)))))), add<i32, overflow=ub>(read<i32>(%172), read<i32>(%173)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %174 @test8(%175 s1: @type8, %176 s2: @type8, %177 s3: @type8) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type8>, i32) -> void>(%170, addr_of<ptr<@type8>>(%175), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type8>, i32) -> void>(%170, addr_of<ptr<@type8>>(%176), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type8>, i32) -> void>(%170, addr_of<ptr<@type8>>(%177), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %178 @test2_8(%179 s1: @type8, %180 s2: @type8) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type8, @type8, @type8) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%174, copy<@type8, reason=arg>(read<@type8>(%179)), copy<@type8, reason=arg>(read<@type8>(%164)), copy<@type8, reason=arg>(read<@type8>(%180)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %181 @testit8() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type8>, i32) -> void>(%166, addr_of<ptr<@type8>>(%163), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type8>, i32) -> void>(%170, addr_of<ptr<@type8>>(%163), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type8>, i32) -> void>(%166, addr_of<ptr<@type8>>(%164), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type8>, i32) -> void>(%170, addr_of<ptr<@type8>>(%164), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type8>, i32) -> void>(%166, addr_of<ptr<@type8>>(%165), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type8>, i32) -> void>(%170, addr_of<ptr<@type8>>(%165), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type8, @type8, @type8) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%174, copy<@type8, reason=arg>(read<@type8>(%163)), copy<@type8, reason=arg>(read<@type8>(%164)), copy<@type8, reason=arg>(read<@type8>(%165)));
// DEFAULT-NEXT:         call<void, signature=fn(@type8, @type8) -> void, abi=sysv64(native_c, native_c) -> void>(%178, copy<@type8, reason=arg>(read<@type8>(%163)), copy<@type8, reason=arg>(read<@type8>(%165)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %186 @init9(%187 p: ptr<@type9>, %188 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %189 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1302
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%189, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%189), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1448: i32 [synthetic] = read<i32>(%189);
// DEFAULT-NEXT:                 let %1449: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1448), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%189, read<i32>(%1449));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(9)>(field0(deref(read<ptr<@type9>>(%187)))), read<i32>(%189))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%188), read<i32>(%189)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %190 @check9(%191 p: ptr<@type9>, %192 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %193 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1303
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%193, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%193), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1450: i32 [synthetic] = read<i32>(%193);
// DEFAULT-NEXT:                 let %1451: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1450), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%193, read<i32>(%1451));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(9)>(field0(deref(read<ptr<@type9>>(%191)))), read<i32>(%193)))))), add<i32, overflow=ub>(read<i32>(%192), read<i32>(%193)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %194 @test9(%195 s1: @type9, %196 s2: @type9, %197 s3: @type9) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type9>, i32) -> void>(%190, addr_of<ptr<@type9>>(%195), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type9>, i32) -> void>(%190, addr_of<ptr<@type9>>(%196), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type9>, i32) -> void>(%190, addr_of<ptr<@type9>>(%197), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %198 @test2_9(%199 s1: @type9, %200 s2: @type9) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type9, @type9, @type9) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%194, copy<@type9, reason=arg>(read<@type9>(%199)), copy<@type9, reason=arg>(read<@type9>(%184)), copy<@type9, reason=arg>(read<@type9>(%200)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %201 @testit9() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type9>, i32) -> void>(%186, addr_of<ptr<@type9>>(%183), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type9>, i32) -> void>(%190, addr_of<ptr<@type9>>(%183), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type9>, i32) -> void>(%186, addr_of<ptr<@type9>>(%184), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type9>, i32) -> void>(%190, addr_of<ptr<@type9>>(%184), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type9>, i32) -> void>(%186, addr_of<ptr<@type9>>(%185), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type9>, i32) -> void>(%190, addr_of<ptr<@type9>>(%185), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type9, @type9, @type9) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%194, copy<@type9, reason=arg>(read<@type9>(%183)), copy<@type9, reason=arg>(read<@type9>(%184)), copy<@type9, reason=arg>(read<@type9>(%185)));
// DEFAULT-NEXT:         call<void, signature=fn(@type9, @type9) -> void, abi=sysv64(native_c, native_c) -> void>(%198, copy<@type9, reason=arg>(read<@type9>(%183)), copy<@type9, reason=arg>(read<@type9>(%185)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %206 @init10(%207 p: ptr<@type10>, %208 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %209 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1304
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%209, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%209), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1452: i32 [synthetic] = read<i32>(%209);
// DEFAULT-NEXT:                 let %1453: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1452), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%209, read<i32>(%1453));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(10)>(field0(deref(read<ptr<@type10>>(%207)))), read<i32>(%209))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%208), read<i32>(%209)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %210 @check10(%211 p: ptr<@type10>, %212 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %213 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1305
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%213, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%213), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1454: i32 [synthetic] = read<i32>(%213);
// DEFAULT-NEXT:                 let %1455: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1454), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%213, read<i32>(%1455));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(10)>(field0(deref(read<ptr<@type10>>(%211)))), read<i32>(%213)))))), add<i32, overflow=ub>(read<i32>(%212), read<i32>(%213)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %214 @test10(%215 s1: @type10, %216 s2: @type10, %217 s3: @type10) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type10>, i32) -> void>(%210, addr_of<ptr<@type10>>(%215), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type10>, i32) -> void>(%210, addr_of<ptr<@type10>>(%216), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type10>, i32) -> void>(%210, addr_of<ptr<@type10>>(%217), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %218 @test2_10(%219 s1: @type10, %220 s2: @type10) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type10, @type10, @type10) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%214, copy<@type10, reason=arg>(read<@type10>(%219)), copy<@type10, reason=arg>(read<@type10>(%204)), copy<@type10, reason=arg>(read<@type10>(%220)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %221 @testit10() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type10>, i32) -> void>(%206, addr_of<ptr<@type10>>(%203), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type10>, i32) -> void>(%210, addr_of<ptr<@type10>>(%203), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type10>, i32) -> void>(%206, addr_of<ptr<@type10>>(%204), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type10>, i32) -> void>(%210, addr_of<ptr<@type10>>(%204), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type10>, i32) -> void>(%206, addr_of<ptr<@type10>>(%205), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type10>, i32) -> void>(%210, addr_of<ptr<@type10>>(%205), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type10, @type10, @type10) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%214, copy<@type10, reason=arg>(read<@type10>(%203)), copy<@type10, reason=arg>(read<@type10>(%204)), copy<@type10, reason=arg>(read<@type10>(%205)));
// DEFAULT-NEXT:         call<void, signature=fn(@type10, @type10) -> void, abi=sysv64(native_c, native_c) -> void>(%218, copy<@type10, reason=arg>(read<@type10>(%203)), copy<@type10, reason=arg>(read<@type10>(%205)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %226 @init11(%227 p: ptr<@type11>, %228 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %229 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1306
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%229, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%229), const<i32>(11))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1456: i32 [synthetic] = read<i32>(%229);
// DEFAULT-NEXT:                 let %1457: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1456), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%229, read<i32>(%1457));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(11)>(field0(deref(read<ptr<@type11>>(%227)))), read<i32>(%229))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%228), read<i32>(%229)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %230 @check11(%231 p: ptr<@type11>, %232 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %233 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1307
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%233, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%233), const<i32>(11))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1458: i32 [synthetic] = read<i32>(%233);
// DEFAULT-NEXT:                 let %1459: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1458), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%233, read<i32>(%1459));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(11)>(field0(deref(read<ptr<@type11>>(%231)))), read<i32>(%233)))))), add<i32, overflow=ub>(read<i32>(%232), read<i32>(%233)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %234 @test11(%235 s1: @type11, %236 s2: @type11, %237 s3: @type11) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type11>, i32) -> void>(%230, addr_of<ptr<@type11>>(%235), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type11>, i32) -> void>(%230, addr_of<ptr<@type11>>(%236), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type11>, i32) -> void>(%230, addr_of<ptr<@type11>>(%237), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %238 @test2_11(%239 s1: @type11, %240 s2: @type11) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type11, @type11, @type11) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%234, copy<@type11, reason=arg>(read<@type11>(%239)), copy<@type11, reason=arg>(read<@type11>(%224)), copy<@type11, reason=arg>(read<@type11>(%240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %241 @testit11() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type11>, i32) -> void>(%226, addr_of<ptr<@type11>>(%223), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type11>, i32) -> void>(%230, addr_of<ptr<@type11>>(%223), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type11>, i32) -> void>(%226, addr_of<ptr<@type11>>(%224), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type11>, i32) -> void>(%230, addr_of<ptr<@type11>>(%224), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type11>, i32) -> void>(%226, addr_of<ptr<@type11>>(%225), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type11>, i32) -> void>(%230, addr_of<ptr<@type11>>(%225), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type11, @type11, @type11) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%234, copy<@type11, reason=arg>(read<@type11>(%223)), copy<@type11, reason=arg>(read<@type11>(%224)), copy<@type11, reason=arg>(read<@type11>(%225)));
// DEFAULT-NEXT:         call<void, signature=fn(@type11, @type11) -> void, abi=sysv64(native_c, native_c) -> void>(%238, copy<@type11, reason=arg>(read<@type11>(%223)), copy<@type11, reason=arg>(read<@type11>(%225)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %246 @init12(%247 p: ptr<@type12>, %248 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %249 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1308
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%249, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%249), const<i32>(12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1460: i32 [synthetic] = read<i32>(%249);
// DEFAULT-NEXT:                 let %1461: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1460), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%249, read<i32>(%1461));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(12)>(field0(deref(read<ptr<@type12>>(%247)))), read<i32>(%249))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%248), read<i32>(%249)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %250 @check12(%251 p: ptr<@type12>, %252 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %253 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1309
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%253, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%253), const<i32>(12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1462: i32 [synthetic] = read<i32>(%253);
// DEFAULT-NEXT:                 let %1463: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1462), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%253, read<i32>(%1463));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(12)>(field0(deref(read<ptr<@type12>>(%251)))), read<i32>(%253)))))), add<i32, overflow=ub>(read<i32>(%252), read<i32>(%253)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %254 @test12(%255 s1: @type12, %256 s2: @type12, %257 s3: @type12) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type12>, i32) -> void>(%250, addr_of<ptr<@type12>>(%255), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type12>, i32) -> void>(%250, addr_of<ptr<@type12>>(%256), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type12>, i32) -> void>(%250, addr_of<ptr<@type12>>(%257), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %258 @test2_12(%259 s1: @type12, %260 s2: @type12) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type12, @type12, @type12) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%254, copy<@type12, reason=arg>(read<@type12>(%259)), copy<@type12, reason=arg>(read<@type12>(%244)), copy<@type12, reason=arg>(read<@type12>(%260)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %261 @testit12() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type12>, i32) -> void>(%246, addr_of<ptr<@type12>>(%243), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type12>, i32) -> void>(%250, addr_of<ptr<@type12>>(%243), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type12>, i32) -> void>(%246, addr_of<ptr<@type12>>(%244), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type12>, i32) -> void>(%250, addr_of<ptr<@type12>>(%244), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type12>, i32) -> void>(%246, addr_of<ptr<@type12>>(%245), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type12>, i32) -> void>(%250, addr_of<ptr<@type12>>(%245), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type12, @type12, @type12) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%254, copy<@type12, reason=arg>(read<@type12>(%243)), copy<@type12, reason=arg>(read<@type12>(%244)), copy<@type12, reason=arg>(read<@type12>(%245)));
// DEFAULT-NEXT:         call<void, signature=fn(@type12, @type12) -> void, abi=sysv64(native_c, native_c) -> void>(%258, copy<@type12, reason=arg>(read<@type12>(%243)), copy<@type12, reason=arg>(read<@type12>(%245)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %266 @init13(%267 p: ptr<@type13>, %268 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %269 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1310
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%269, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%269), const<i32>(13))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1464: i32 [synthetic] = read<i32>(%269);
// DEFAULT-NEXT:                 let %1465: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1464), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%269, read<i32>(%1465));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(13)>(field0(deref(read<ptr<@type13>>(%267)))), read<i32>(%269))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%268), read<i32>(%269)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %270 @check13(%271 p: ptr<@type13>, %272 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %273 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1311
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%273, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%273), const<i32>(13))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1466: i32 [synthetic] = read<i32>(%273);
// DEFAULT-NEXT:                 let %1467: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1466), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%273, read<i32>(%1467));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(13)>(field0(deref(read<ptr<@type13>>(%271)))), read<i32>(%273)))))), add<i32, overflow=ub>(read<i32>(%272), read<i32>(%273)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %274 @test13(%275 s1: @type13, %276 s2: @type13, %277 s3: @type13) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type13>, i32) -> void>(%270, addr_of<ptr<@type13>>(%275), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type13>, i32) -> void>(%270, addr_of<ptr<@type13>>(%276), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type13>, i32) -> void>(%270, addr_of<ptr<@type13>>(%277), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %278 @test2_13(%279 s1: @type13, %280 s2: @type13) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type13, @type13, @type13) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%274, copy<@type13, reason=arg>(read<@type13>(%279)), copy<@type13, reason=arg>(read<@type13>(%264)), copy<@type13, reason=arg>(read<@type13>(%280)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %281 @testit13() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type13>, i32) -> void>(%266, addr_of<ptr<@type13>>(%263), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type13>, i32) -> void>(%270, addr_of<ptr<@type13>>(%263), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type13>, i32) -> void>(%266, addr_of<ptr<@type13>>(%264), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type13>, i32) -> void>(%270, addr_of<ptr<@type13>>(%264), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type13>, i32) -> void>(%266, addr_of<ptr<@type13>>(%265), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type13>, i32) -> void>(%270, addr_of<ptr<@type13>>(%265), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type13, @type13, @type13) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%274, copy<@type13, reason=arg>(read<@type13>(%263)), copy<@type13, reason=arg>(read<@type13>(%264)), copy<@type13, reason=arg>(read<@type13>(%265)));
// DEFAULT-NEXT:         call<void, signature=fn(@type13, @type13) -> void, abi=sysv64(native_c, native_c) -> void>(%278, copy<@type13, reason=arg>(read<@type13>(%263)), copy<@type13, reason=arg>(read<@type13>(%265)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %286 @init14(%287 p: ptr<@type14>, %288 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %289 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1312
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%289, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%289), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1468: i32 [synthetic] = read<i32>(%289);
// DEFAULT-NEXT:                 let %1469: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1468), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%289, read<i32>(%1469));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(14)>(field0(deref(read<ptr<@type14>>(%287)))), read<i32>(%289))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%288), read<i32>(%289)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %290 @check14(%291 p: ptr<@type14>, %292 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %293 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1313
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%293, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%293), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1470: i32 [synthetic] = read<i32>(%293);
// DEFAULT-NEXT:                 let %1471: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1470), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%293, read<i32>(%1471));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(14)>(field0(deref(read<ptr<@type14>>(%291)))), read<i32>(%293)))))), add<i32, overflow=ub>(read<i32>(%292), read<i32>(%293)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %294 @test14(%295 s1: @type14, %296 s2: @type14, %297 s3: @type14) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type14>, i32) -> void>(%290, addr_of<ptr<@type14>>(%295), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type14>, i32) -> void>(%290, addr_of<ptr<@type14>>(%296), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type14>, i32) -> void>(%290, addr_of<ptr<@type14>>(%297), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %298 @test2_14(%299 s1: @type14, %300 s2: @type14) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type14, @type14, @type14) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%294, copy<@type14, reason=arg>(read<@type14>(%299)), copy<@type14, reason=arg>(read<@type14>(%284)), copy<@type14, reason=arg>(read<@type14>(%300)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %301 @testit14() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type14>, i32) -> void>(%286, addr_of<ptr<@type14>>(%283), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type14>, i32) -> void>(%290, addr_of<ptr<@type14>>(%283), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type14>, i32) -> void>(%286, addr_of<ptr<@type14>>(%284), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type14>, i32) -> void>(%290, addr_of<ptr<@type14>>(%284), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type14>, i32) -> void>(%286, addr_of<ptr<@type14>>(%285), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type14>, i32) -> void>(%290, addr_of<ptr<@type14>>(%285), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type14, @type14, @type14) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%294, copy<@type14, reason=arg>(read<@type14>(%283)), copy<@type14, reason=arg>(read<@type14>(%284)), copy<@type14, reason=arg>(read<@type14>(%285)));
// DEFAULT-NEXT:         call<void, signature=fn(@type14, @type14) -> void, abi=sysv64(native_c, native_c) -> void>(%298, copy<@type14, reason=arg>(read<@type14>(%283)), copy<@type14, reason=arg>(read<@type14>(%285)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %306 @init15(%307 p: ptr<@type15>, %308 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %309 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1314
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%309, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%309), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1472: i32 [synthetic] = read<i32>(%309);
// DEFAULT-NEXT:                 let %1473: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1472), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%309, read<i32>(%1473));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(15)>(field0(deref(read<ptr<@type15>>(%307)))), read<i32>(%309))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%308), read<i32>(%309)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %310 @check15(%311 p: ptr<@type15>, %312 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %313 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1315
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%313, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%313), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1474: i32 [synthetic] = read<i32>(%313);
// DEFAULT-NEXT:                 let %1475: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1474), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%313, read<i32>(%1475));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(15)>(field0(deref(read<ptr<@type15>>(%311)))), read<i32>(%313)))))), add<i32, overflow=ub>(read<i32>(%312), read<i32>(%313)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %314 @test15(%315 s1: @type15, %316 s2: @type15, %317 s3: @type15) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type15>, i32) -> void>(%310, addr_of<ptr<@type15>>(%315), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type15>, i32) -> void>(%310, addr_of<ptr<@type15>>(%316), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type15>, i32) -> void>(%310, addr_of<ptr<@type15>>(%317), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %318 @test2_15(%319 s1: @type15, %320 s2: @type15) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type15, @type15, @type15) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%314, copy<@type15, reason=arg>(read<@type15>(%319)), copy<@type15, reason=arg>(read<@type15>(%304)), copy<@type15, reason=arg>(read<@type15>(%320)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %321 @testit15() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type15>, i32) -> void>(%306, addr_of<ptr<@type15>>(%303), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type15>, i32) -> void>(%310, addr_of<ptr<@type15>>(%303), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type15>, i32) -> void>(%306, addr_of<ptr<@type15>>(%304), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type15>, i32) -> void>(%310, addr_of<ptr<@type15>>(%304), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type15>, i32) -> void>(%306, addr_of<ptr<@type15>>(%305), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type15>, i32) -> void>(%310, addr_of<ptr<@type15>>(%305), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type15, @type15, @type15) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%314, copy<@type15, reason=arg>(read<@type15>(%303)), copy<@type15, reason=arg>(read<@type15>(%304)), copy<@type15, reason=arg>(read<@type15>(%305)));
// DEFAULT-NEXT:         call<void, signature=fn(@type15, @type15) -> void, abi=sysv64(native_c, native_c) -> void>(%318, copy<@type15, reason=arg>(read<@type15>(%303)), copy<@type15, reason=arg>(read<@type15>(%305)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %326 @init16(%327 p: ptr<@type16>, %328 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %329 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1316
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%329, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%329), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1476: i32 [synthetic] = read<i32>(%329);
// DEFAULT-NEXT:                 let %1477: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1476), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%329, read<i32>(%1477));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(16)>(field0(deref(read<ptr<@type16>>(%327)))), read<i32>(%329))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%328), read<i32>(%329)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %330 @check16(%331 p: ptr<@type16>, %332 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %333 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1317
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%333, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%333), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1478: i32 [synthetic] = read<i32>(%333);
// DEFAULT-NEXT:                 let %1479: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1478), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%333, read<i32>(%1479));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(16)>(field0(deref(read<ptr<@type16>>(%331)))), read<i32>(%333)))))), add<i32, overflow=ub>(read<i32>(%332), read<i32>(%333)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %334 @test16(%335 s1: @type16, %336 s2: @type16, %337 s3: @type16) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type16>, i32) -> void>(%330, addr_of<ptr<@type16>>(%335), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type16>, i32) -> void>(%330, addr_of<ptr<@type16>>(%336), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type16>, i32) -> void>(%330, addr_of<ptr<@type16>>(%337), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %338 @test2_16(%339 s1: @type16, %340 s2: @type16) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type16, @type16, @type16) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%334, copy<@type16, reason=arg>(read<@type16>(%339)), copy<@type16, reason=arg>(read<@type16>(%324)), copy<@type16, reason=arg>(read<@type16>(%340)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %341 @testit16() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type16>, i32) -> void>(%326, addr_of<ptr<@type16>>(%323), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type16>, i32) -> void>(%330, addr_of<ptr<@type16>>(%323), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type16>, i32) -> void>(%326, addr_of<ptr<@type16>>(%324), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type16>, i32) -> void>(%330, addr_of<ptr<@type16>>(%324), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type16>, i32) -> void>(%326, addr_of<ptr<@type16>>(%325), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type16>, i32) -> void>(%330, addr_of<ptr<@type16>>(%325), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type16, @type16, @type16) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%334, copy<@type16, reason=arg>(read<@type16>(%323)), copy<@type16, reason=arg>(read<@type16>(%324)), copy<@type16, reason=arg>(read<@type16>(%325)));
// DEFAULT-NEXT:         call<void, signature=fn(@type16, @type16) -> void, abi=sysv64(native_c, native_c) -> void>(%338, copy<@type16, reason=arg>(read<@type16>(%323)), copy<@type16, reason=arg>(read<@type16>(%325)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %346 @init17(%347 p: ptr<@type17>, %348 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %349 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1318
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%349, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%349), const<i32>(17))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1480: i32 [synthetic] = read<i32>(%349);
// DEFAULT-NEXT:                 let %1481: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1480), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%349, read<i32>(%1481));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(17)>(field0(deref(read<ptr<@type17>>(%347)))), read<i32>(%349))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%348), read<i32>(%349)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %350 @check17(%351 p: ptr<@type17>, %352 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %353 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1319
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%353, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%353), const<i32>(17))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1482: i32 [synthetic] = read<i32>(%353);
// DEFAULT-NEXT:                 let %1483: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1482), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%353, read<i32>(%1483));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(17)>(field0(deref(read<ptr<@type17>>(%351)))), read<i32>(%353)))))), add<i32, overflow=ub>(read<i32>(%352), read<i32>(%353)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %354 @test17(%355 s1: @type17, %356 s2: @type17, %357 s3: @type17) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type17>, i32) -> void>(%350, addr_of<ptr<@type17>>(%355), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type17>, i32) -> void>(%350, addr_of<ptr<@type17>>(%356), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type17>, i32) -> void>(%350, addr_of<ptr<@type17>>(%357), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %358 @test2_17(%359 s1: @type17, %360 s2: @type17) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type17, @type17, @type17) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%354, copy<@type17, reason=arg>(read<@type17>(%359)), copy<@type17, reason=arg>(read<@type17>(%344)), copy<@type17, reason=arg>(read<@type17>(%360)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %361 @testit17() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type17>, i32) -> void>(%346, addr_of<ptr<@type17>>(%343), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type17>, i32) -> void>(%350, addr_of<ptr<@type17>>(%343), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type17>, i32) -> void>(%346, addr_of<ptr<@type17>>(%344), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type17>, i32) -> void>(%350, addr_of<ptr<@type17>>(%344), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type17>, i32) -> void>(%346, addr_of<ptr<@type17>>(%345), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type17>, i32) -> void>(%350, addr_of<ptr<@type17>>(%345), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type17, @type17, @type17) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%354, copy<@type17, reason=arg>(read<@type17>(%343)), copy<@type17, reason=arg>(read<@type17>(%344)), copy<@type17, reason=arg>(read<@type17>(%345)));
// DEFAULT-NEXT:         call<void, signature=fn(@type17, @type17) -> void, abi=sysv64(native_c, native_c) -> void>(%358, copy<@type17, reason=arg>(read<@type17>(%343)), copy<@type17, reason=arg>(read<@type17>(%345)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %366 @init18(%367 p: ptr<@type18>, %368 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %369 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1320
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%369, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%369), const<i32>(18))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1484: i32 [synthetic] = read<i32>(%369);
// DEFAULT-NEXT:                 let %1485: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1484), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%369, read<i32>(%1485));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(18)>(field0(deref(read<ptr<@type18>>(%367)))), read<i32>(%369))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%368), read<i32>(%369)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %370 @check18(%371 p: ptr<@type18>, %372 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %373 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1321
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%373, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%373), const<i32>(18))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1486: i32 [synthetic] = read<i32>(%373);
// DEFAULT-NEXT:                 let %1487: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1486), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%373, read<i32>(%1487));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(18)>(field0(deref(read<ptr<@type18>>(%371)))), read<i32>(%373)))))), add<i32, overflow=ub>(read<i32>(%372), read<i32>(%373)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %374 @test18(%375 s1: @type18, %376 s2: @type18, %377 s3: @type18) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type18>, i32) -> void>(%370, addr_of<ptr<@type18>>(%375), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type18>, i32) -> void>(%370, addr_of<ptr<@type18>>(%376), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type18>, i32) -> void>(%370, addr_of<ptr<@type18>>(%377), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %378 @test2_18(%379 s1: @type18, %380 s2: @type18) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type18, @type18, @type18) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%374, copy<@type18, reason=arg>(read<@type18>(%379)), copy<@type18, reason=arg>(read<@type18>(%364)), copy<@type18, reason=arg>(read<@type18>(%380)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %381 @testit18() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type18>, i32) -> void>(%366, addr_of<ptr<@type18>>(%363), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type18>, i32) -> void>(%370, addr_of<ptr<@type18>>(%363), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type18>, i32) -> void>(%366, addr_of<ptr<@type18>>(%364), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type18>, i32) -> void>(%370, addr_of<ptr<@type18>>(%364), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type18>, i32) -> void>(%366, addr_of<ptr<@type18>>(%365), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type18>, i32) -> void>(%370, addr_of<ptr<@type18>>(%365), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type18, @type18, @type18) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%374, copy<@type18, reason=arg>(read<@type18>(%363)), copy<@type18, reason=arg>(read<@type18>(%364)), copy<@type18, reason=arg>(read<@type18>(%365)));
// DEFAULT-NEXT:         call<void, signature=fn(@type18, @type18) -> void, abi=sysv64(native_c, native_c) -> void>(%378, copy<@type18, reason=arg>(read<@type18>(%363)), copy<@type18, reason=arg>(read<@type18>(%365)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %386 @init19(%387 p: ptr<@type19>, %388 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %389 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1322
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%389, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%389), const<i32>(19))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1488: i32 [synthetic] = read<i32>(%389);
// DEFAULT-NEXT:                 let %1489: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1488), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%389, read<i32>(%1489));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(19)>(field0(deref(read<ptr<@type19>>(%387)))), read<i32>(%389))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%388), read<i32>(%389)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %390 @check19(%391 p: ptr<@type19>, %392 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %393 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1323
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%393, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%393), const<i32>(19))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1490: i32 [synthetic] = read<i32>(%393);
// DEFAULT-NEXT:                 let %1491: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1490), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%393, read<i32>(%1491));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(19)>(field0(deref(read<ptr<@type19>>(%391)))), read<i32>(%393)))))), add<i32, overflow=ub>(read<i32>(%392), read<i32>(%393)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %394 @test19(%395 s1: @type19, %396 s2: @type19, %397 s3: @type19) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type19>, i32) -> void>(%390, addr_of<ptr<@type19>>(%395), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type19>, i32) -> void>(%390, addr_of<ptr<@type19>>(%396), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type19>, i32) -> void>(%390, addr_of<ptr<@type19>>(%397), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %398 @test2_19(%399 s1: @type19, %400 s2: @type19) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type19, @type19, @type19) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%394, copy<@type19, reason=arg>(read<@type19>(%399)), copy<@type19, reason=arg>(read<@type19>(%384)), copy<@type19, reason=arg>(read<@type19>(%400)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %401 @testit19() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type19>, i32) -> void>(%386, addr_of<ptr<@type19>>(%383), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type19>, i32) -> void>(%390, addr_of<ptr<@type19>>(%383), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type19>, i32) -> void>(%386, addr_of<ptr<@type19>>(%384), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type19>, i32) -> void>(%390, addr_of<ptr<@type19>>(%384), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type19>, i32) -> void>(%386, addr_of<ptr<@type19>>(%385), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type19>, i32) -> void>(%390, addr_of<ptr<@type19>>(%385), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type19, @type19, @type19) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%394, copy<@type19, reason=arg>(read<@type19>(%383)), copy<@type19, reason=arg>(read<@type19>(%384)), copy<@type19, reason=arg>(read<@type19>(%385)));
// DEFAULT-NEXT:         call<void, signature=fn(@type19, @type19) -> void, abi=sysv64(native_c, native_c) -> void>(%398, copy<@type19, reason=arg>(read<@type19>(%383)), copy<@type19, reason=arg>(read<@type19>(%385)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %406 @init20(%407 p: ptr<@type20>, %408 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %409 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1324
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%409, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%409), const<i32>(20))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1492: i32 [synthetic] = read<i32>(%409);
// DEFAULT-NEXT:                 let %1493: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1492), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%409, read<i32>(%1493));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(20)>(field0(deref(read<ptr<@type20>>(%407)))), read<i32>(%409))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%408), read<i32>(%409)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %410 @check20(%411 p: ptr<@type20>, %412 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %413 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1325
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%413, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%413), const<i32>(20))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1494: i32 [synthetic] = read<i32>(%413);
// DEFAULT-NEXT:                 let %1495: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1494), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%413, read<i32>(%1495));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(20)>(field0(deref(read<ptr<@type20>>(%411)))), read<i32>(%413)))))), add<i32, overflow=ub>(read<i32>(%412), read<i32>(%413)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %414 @test20(%415 s1: @type20, %416 s2: @type20, %417 s3: @type20) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type20>, i32) -> void>(%410, addr_of<ptr<@type20>>(%415), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type20>, i32) -> void>(%410, addr_of<ptr<@type20>>(%416), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type20>, i32) -> void>(%410, addr_of<ptr<@type20>>(%417), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %418 @test2_20(%419 s1: @type20, %420 s2: @type20) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type20, @type20, @type20) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%414, copy<@type20, reason=arg>(read<@type20>(%419)), copy<@type20, reason=arg>(read<@type20>(%404)), copy<@type20, reason=arg>(read<@type20>(%420)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %421 @testit20() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type20>, i32) -> void>(%406, addr_of<ptr<@type20>>(%403), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type20>, i32) -> void>(%410, addr_of<ptr<@type20>>(%403), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type20>, i32) -> void>(%406, addr_of<ptr<@type20>>(%404), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type20>, i32) -> void>(%410, addr_of<ptr<@type20>>(%404), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type20>, i32) -> void>(%406, addr_of<ptr<@type20>>(%405), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type20>, i32) -> void>(%410, addr_of<ptr<@type20>>(%405), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type20, @type20, @type20) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%414, copy<@type20, reason=arg>(read<@type20>(%403)), copy<@type20, reason=arg>(read<@type20>(%404)), copy<@type20, reason=arg>(read<@type20>(%405)));
// DEFAULT-NEXT:         call<void, signature=fn(@type20, @type20) -> void, abi=sysv64(native_c, native_c) -> void>(%418, copy<@type20, reason=arg>(read<@type20>(%403)), copy<@type20, reason=arg>(read<@type20>(%405)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %426 @init21(%427 p: ptr<@type21>, %428 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %429 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1326
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%429, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%429), const<i32>(21))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1496: i32 [synthetic] = read<i32>(%429);
// DEFAULT-NEXT:                 let %1497: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1496), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%429, read<i32>(%1497));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(21)>(field0(deref(read<ptr<@type21>>(%427)))), read<i32>(%429))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%428), read<i32>(%429)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %430 @check21(%431 p: ptr<@type21>, %432 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %433 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1327
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%433, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%433), const<i32>(21))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1498: i32 [synthetic] = read<i32>(%433);
// DEFAULT-NEXT:                 let %1499: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1498), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%433, read<i32>(%1499));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(21)>(field0(deref(read<ptr<@type21>>(%431)))), read<i32>(%433)))))), add<i32, overflow=ub>(read<i32>(%432), read<i32>(%433)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %434 @test21(%435 s1: @type21, %436 s2: @type21, %437 s3: @type21) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type21>, i32) -> void>(%430, addr_of<ptr<@type21>>(%435), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type21>, i32) -> void>(%430, addr_of<ptr<@type21>>(%436), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type21>, i32) -> void>(%430, addr_of<ptr<@type21>>(%437), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %438 @test2_21(%439 s1: @type21, %440 s2: @type21) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type21, @type21, @type21) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%434, copy<@type21, reason=arg>(read<@type21>(%439)), copy<@type21, reason=arg>(read<@type21>(%424)), copy<@type21, reason=arg>(read<@type21>(%440)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %441 @testit21() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type21>, i32) -> void>(%426, addr_of<ptr<@type21>>(%423), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type21>, i32) -> void>(%430, addr_of<ptr<@type21>>(%423), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type21>, i32) -> void>(%426, addr_of<ptr<@type21>>(%424), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type21>, i32) -> void>(%430, addr_of<ptr<@type21>>(%424), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type21>, i32) -> void>(%426, addr_of<ptr<@type21>>(%425), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type21>, i32) -> void>(%430, addr_of<ptr<@type21>>(%425), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type21, @type21, @type21) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%434, copy<@type21, reason=arg>(read<@type21>(%423)), copy<@type21, reason=arg>(read<@type21>(%424)), copy<@type21, reason=arg>(read<@type21>(%425)));
// DEFAULT-NEXT:         call<void, signature=fn(@type21, @type21) -> void, abi=sysv64(native_c, native_c) -> void>(%438, copy<@type21, reason=arg>(read<@type21>(%423)), copy<@type21, reason=arg>(read<@type21>(%425)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %446 @init22(%447 p: ptr<@type22>, %448 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %449 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1328
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%449, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%449), const<i32>(22))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1500: i32 [synthetic] = read<i32>(%449);
// DEFAULT-NEXT:                 let %1501: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1500), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%449, read<i32>(%1501));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(22)>(field0(deref(read<ptr<@type22>>(%447)))), read<i32>(%449))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%448), read<i32>(%449)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %450 @check22(%451 p: ptr<@type22>, %452 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %453 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1329
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%453, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%453), const<i32>(22))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1502: i32 [synthetic] = read<i32>(%453);
// DEFAULT-NEXT:                 let %1503: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1502), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%453, read<i32>(%1503));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(22)>(field0(deref(read<ptr<@type22>>(%451)))), read<i32>(%453)))))), add<i32, overflow=ub>(read<i32>(%452), read<i32>(%453)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %454 @test22(%455 s1: @type22, %456 s2: @type22, %457 s3: @type22) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type22>, i32) -> void>(%450, addr_of<ptr<@type22>>(%455), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type22>, i32) -> void>(%450, addr_of<ptr<@type22>>(%456), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type22>, i32) -> void>(%450, addr_of<ptr<@type22>>(%457), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %458 @test2_22(%459 s1: @type22, %460 s2: @type22) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type22, @type22, @type22) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%454, copy<@type22, reason=arg>(read<@type22>(%459)), copy<@type22, reason=arg>(read<@type22>(%444)), copy<@type22, reason=arg>(read<@type22>(%460)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %461 @testit22() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type22>, i32) -> void>(%446, addr_of<ptr<@type22>>(%443), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type22>, i32) -> void>(%450, addr_of<ptr<@type22>>(%443), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type22>, i32) -> void>(%446, addr_of<ptr<@type22>>(%444), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type22>, i32) -> void>(%450, addr_of<ptr<@type22>>(%444), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type22>, i32) -> void>(%446, addr_of<ptr<@type22>>(%445), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type22>, i32) -> void>(%450, addr_of<ptr<@type22>>(%445), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type22, @type22, @type22) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%454, copy<@type22, reason=arg>(read<@type22>(%443)), copy<@type22, reason=arg>(read<@type22>(%444)), copy<@type22, reason=arg>(read<@type22>(%445)));
// DEFAULT-NEXT:         call<void, signature=fn(@type22, @type22) -> void, abi=sysv64(native_c, native_c) -> void>(%458, copy<@type22, reason=arg>(read<@type22>(%443)), copy<@type22, reason=arg>(read<@type22>(%445)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %466 @init23(%467 p: ptr<@type23>, %468 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %469 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1330
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%469, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%469), const<i32>(23))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1504: i32 [synthetic] = read<i32>(%469);
// DEFAULT-NEXT:                 let %1505: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1504), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%469, read<i32>(%1505));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(23)>(field0(deref(read<ptr<@type23>>(%467)))), read<i32>(%469))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%468), read<i32>(%469)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %470 @check23(%471 p: ptr<@type23>, %472 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %473 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1331
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%473, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%473), const<i32>(23))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1506: i32 [synthetic] = read<i32>(%473);
// DEFAULT-NEXT:                 let %1507: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1506), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%473, read<i32>(%1507));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(23)>(field0(deref(read<ptr<@type23>>(%471)))), read<i32>(%473)))))), add<i32, overflow=ub>(read<i32>(%472), read<i32>(%473)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %474 @test23(%475 s1: @type23, %476 s2: @type23, %477 s3: @type23) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type23>, i32) -> void>(%470, addr_of<ptr<@type23>>(%475), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type23>, i32) -> void>(%470, addr_of<ptr<@type23>>(%476), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type23>, i32) -> void>(%470, addr_of<ptr<@type23>>(%477), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %478 @test2_23(%479 s1: @type23, %480 s2: @type23) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type23, @type23, @type23) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%474, copy<@type23, reason=arg>(read<@type23>(%479)), copy<@type23, reason=arg>(read<@type23>(%464)), copy<@type23, reason=arg>(read<@type23>(%480)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %481 @testit23() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type23>, i32) -> void>(%466, addr_of<ptr<@type23>>(%463), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type23>, i32) -> void>(%470, addr_of<ptr<@type23>>(%463), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type23>, i32) -> void>(%466, addr_of<ptr<@type23>>(%464), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type23>, i32) -> void>(%470, addr_of<ptr<@type23>>(%464), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type23>, i32) -> void>(%466, addr_of<ptr<@type23>>(%465), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type23>, i32) -> void>(%470, addr_of<ptr<@type23>>(%465), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type23, @type23, @type23) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%474, copy<@type23, reason=arg>(read<@type23>(%463)), copy<@type23, reason=arg>(read<@type23>(%464)), copy<@type23, reason=arg>(read<@type23>(%465)));
// DEFAULT-NEXT:         call<void, signature=fn(@type23, @type23) -> void, abi=sysv64(native_c, native_c) -> void>(%478, copy<@type23, reason=arg>(read<@type23>(%463)), copy<@type23, reason=arg>(read<@type23>(%465)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %486 @init24(%487 p: ptr<@type24>, %488 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %489 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1332
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%489, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%489), const<i32>(24))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1508: i32 [synthetic] = read<i32>(%489);
// DEFAULT-NEXT:                 let %1509: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1508), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%489, read<i32>(%1509));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(24)>(field0(deref(read<ptr<@type24>>(%487)))), read<i32>(%489))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%488), read<i32>(%489)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %490 @check24(%491 p: ptr<@type24>, %492 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %493 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1333
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%493, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%493), const<i32>(24))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1510: i32 [synthetic] = read<i32>(%493);
// DEFAULT-NEXT:                 let %1511: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1510), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%493, read<i32>(%1511));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(24)>(field0(deref(read<ptr<@type24>>(%491)))), read<i32>(%493)))))), add<i32, overflow=ub>(read<i32>(%492), read<i32>(%493)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %494 @test24(%495 s1: @type24, %496 s2: @type24, %497 s3: @type24) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type24>, i32) -> void>(%490, addr_of<ptr<@type24>>(%495), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type24>, i32) -> void>(%490, addr_of<ptr<@type24>>(%496), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type24>, i32) -> void>(%490, addr_of<ptr<@type24>>(%497), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %498 @test2_24(%499 s1: @type24, %500 s2: @type24) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type24, @type24, @type24) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%494, copy<@type24, reason=arg>(read<@type24>(%499)), copy<@type24, reason=arg>(read<@type24>(%484)), copy<@type24, reason=arg>(read<@type24>(%500)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %501 @testit24() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type24>, i32) -> void>(%486, addr_of<ptr<@type24>>(%483), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type24>, i32) -> void>(%490, addr_of<ptr<@type24>>(%483), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type24>, i32) -> void>(%486, addr_of<ptr<@type24>>(%484), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type24>, i32) -> void>(%490, addr_of<ptr<@type24>>(%484), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type24>, i32) -> void>(%486, addr_of<ptr<@type24>>(%485), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type24>, i32) -> void>(%490, addr_of<ptr<@type24>>(%485), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type24, @type24, @type24) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%494, copy<@type24, reason=arg>(read<@type24>(%483)), copy<@type24, reason=arg>(read<@type24>(%484)), copy<@type24, reason=arg>(read<@type24>(%485)));
// DEFAULT-NEXT:         call<void, signature=fn(@type24, @type24) -> void, abi=sysv64(native_c, native_c) -> void>(%498, copy<@type24, reason=arg>(read<@type24>(%483)), copy<@type24, reason=arg>(read<@type24>(%485)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %506 @init25(%507 p: ptr<@type25>, %508 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %509 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1334
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%509, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%509), const<i32>(25))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1512: i32 [synthetic] = read<i32>(%509);
// DEFAULT-NEXT:                 let %1513: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1512), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%509, read<i32>(%1513));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(25)>(field0(deref(read<ptr<@type25>>(%507)))), read<i32>(%509))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%508), read<i32>(%509)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %510 @check25(%511 p: ptr<@type25>, %512 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %513 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1335
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%513, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%513), const<i32>(25))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1514: i32 [synthetic] = read<i32>(%513);
// DEFAULT-NEXT:                 let %1515: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1514), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%513, read<i32>(%1515));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(25)>(field0(deref(read<ptr<@type25>>(%511)))), read<i32>(%513)))))), add<i32, overflow=ub>(read<i32>(%512), read<i32>(%513)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %514 @test25(%515 s1: @type25, %516 s2: @type25, %517 s3: @type25) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type25>, i32) -> void>(%510, addr_of<ptr<@type25>>(%515), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type25>, i32) -> void>(%510, addr_of<ptr<@type25>>(%516), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type25>, i32) -> void>(%510, addr_of<ptr<@type25>>(%517), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %518 @test2_25(%519 s1: @type25, %520 s2: @type25) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type25, @type25, @type25) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%514, copy<@type25, reason=arg>(read<@type25>(%519)), copy<@type25, reason=arg>(read<@type25>(%504)), copy<@type25, reason=arg>(read<@type25>(%520)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %521 @testit25() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type25>, i32) -> void>(%506, addr_of<ptr<@type25>>(%503), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type25>, i32) -> void>(%510, addr_of<ptr<@type25>>(%503), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type25>, i32) -> void>(%506, addr_of<ptr<@type25>>(%504), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type25>, i32) -> void>(%510, addr_of<ptr<@type25>>(%504), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type25>, i32) -> void>(%506, addr_of<ptr<@type25>>(%505), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type25>, i32) -> void>(%510, addr_of<ptr<@type25>>(%505), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type25, @type25, @type25) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%514, copy<@type25, reason=arg>(read<@type25>(%503)), copy<@type25, reason=arg>(read<@type25>(%504)), copy<@type25, reason=arg>(read<@type25>(%505)));
// DEFAULT-NEXT:         call<void, signature=fn(@type25, @type25) -> void, abi=sysv64(native_c, native_c) -> void>(%518, copy<@type25, reason=arg>(read<@type25>(%503)), copy<@type25, reason=arg>(read<@type25>(%505)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %526 @init26(%527 p: ptr<@type26>, %528 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %529 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1336
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%529, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%529), const<i32>(26))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1516: i32 [synthetic] = read<i32>(%529);
// DEFAULT-NEXT:                 let %1517: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1516), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%529, read<i32>(%1517));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(26)>(field0(deref(read<ptr<@type26>>(%527)))), read<i32>(%529))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%528), read<i32>(%529)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %530 @check26(%531 p: ptr<@type26>, %532 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %533 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1337
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%533, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%533), const<i32>(26))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1518: i32 [synthetic] = read<i32>(%533);
// DEFAULT-NEXT:                 let %1519: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1518), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%533, read<i32>(%1519));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(26)>(field0(deref(read<ptr<@type26>>(%531)))), read<i32>(%533)))))), add<i32, overflow=ub>(read<i32>(%532), read<i32>(%533)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %534 @test26(%535 s1: @type26, %536 s2: @type26, %537 s3: @type26) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type26>, i32) -> void>(%530, addr_of<ptr<@type26>>(%535), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type26>, i32) -> void>(%530, addr_of<ptr<@type26>>(%536), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type26>, i32) -> void>(%530, addr_of<ptr<@type26>>(%537), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %538 @test2_26(%539 s1: @type26, %540 s2: @type26) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type26, @type26, @type26) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%534, copy<@type26, reason=arg>(read<@type26>(%539)), copy<@type26, reason=arg>(read<@type26>(%524)), copy<@type26, reason=arg>(read<@type26>(%540)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %541 @testit26() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type26>, i32) -> void>(%526, addr_of<ptr<@type26>>(%523), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type26>, i32) -> void>(%530, addr_of<ptr<@type26>>(%523), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type26>, i32) -> void>(%526, addr_of<ptr<@type26>>(%524), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type26>, i32) -> void>(%530, addr_of<ptr<@type26>>(%524), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type26>, i32) -> void>(%526, addr_of<ptr<@type26>>(%525), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type26>, i32) -> void>(%530, addr_of<ptr<@type26>>(%525), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type26, @type26, @type26) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%534, copy<@type26, reason=arg>(read<@type26>(%523)), copy<@type26, reason=arg>(read<@type26>(%524)), copy<@type26, reason=arg>(read<@type26>(%525)));
// DEFAULT-NEXT:         call<void, signature=fn(@type26, @type26) -> void, abi=sysv64(native_c, native_c) -> void>(%538, copy<@type26, reason=arg>(read<@type26>(%523)), copy<@type26, reason=arg>(read<@type26>(%525)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %546 @init27(%547 p: ptr<@type27>, %548 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %549 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1338
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%549, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%549), const<i32>(27))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1520: i32 [synthetic] = read<i32>(%549);
// DEFAULT-NEXT:                 let %1521: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1520), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%549, read<i32>(%1521));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(27)>(field0(deref(read<ptr<@type27>>(%547)))), read<i32>(%549))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%548), read<i32>(%549)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %550 @check27(%551 p: ptr<@type27>, %552 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %553 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1339
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%553, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%553), const<i32>(27))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1522: i32 [synthetic] = read<i32>(%553);
// DEFAULT-NEXT:                 let %1523: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1522), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%553, read<i32>(%1523));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(27)>(field0(deref(read<ptr<@type27>>(%551)))), read<i32>(%553)))))), add<i32, overflow=ub>(read<i32>(%552), read<i32>(%553)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %554 @test27(%555 s1: @type27, %556 s2: @type27, %557 s3: @type27) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type27>, i32) -> void>(%550, addr_of<ptr<@type27>>(%555), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type27>, i32) -> void>(%550, addr_of<ptr<@type27>>(%556), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type27>, i32) -> void>(%550, addr_of<ptr<@type27>>(%557), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %558 @test2_27(%559 s1: @type27, %560 s2: @type27) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type27, @type27, @type27) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%554, copy<@type27, reason=arg>(read<@type27>(%559)), copy<@type27, reason=arg>(read<@type27>(%544)), copy<@type27, reason=arg>(read<@type27>(%560)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %561 @testit27() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type27>, i32) -> void>(%546, addr_of<ptr<@type27>>(%543), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type27>, i32) -> void>(%550, addr_of<ptr<@type27>>(%543), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type27>, i32) -> void>(%546, addr_of<ptr<@type27>>(%544), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type27>, i32) -> void>(%550, addr_of<ptr<@type27>>(%544), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type27>, i32) -> void>(%546, addr_of<ptr<@type27>>(%545), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type27>, i32) -> void>(%550, addr_of<ptr<@type27>>(%545), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type27, @type27, @type27) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%554, copy<@type27, reason=arg>(read<@type27>(%543)), copy<@type27, reason=arg>(read<@type27>(%544)), copy<@type27, reason=arg>(read<@type27>(%545)));
// DEFAULT-NEXT:         call<void, signature=fn(@type27, @type27) -> void, abi=sysv64(native_c, native_c) -> void>(%558, copy<@type27, reason=arg>(read<@type27>(%543)), copy<@type27, reason=arg>(read<@type27>(%545)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %566 @init28(%567 p: ptr<@type28>, %568 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %569 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1340
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%569, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%569), const<i32>(28))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1524: i32 [synthetic] = read<i32>(%569);
// DEFAULT-NEXT:                 let %1525: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1524), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%569, read<i32>(%1525));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(28)>(field0(deref(read<ptr<@type28>>(%567)))), read<i32>(%569))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%568), read<i32>(%569)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %570 @check28(%571 p: ptr<@type28>, %572 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %573 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1341
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%573, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%573), const<i32>(28))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1526: i32 [synthetic] = read<i32>(%573);
// DEFAULT-NEXT:                 let %1527: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1526), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%573, read<i32>(%1527));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(28)>(field0(deref(read<ptr<@type28>>(%571)))), read<i32>(%573)))))), add<i32, overflow=ub>(read<i32>(%572), read<i32>(%573)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %574 @test28(%575 s1: @type28, %576 s2: @type28, %577 s3: @type28) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type28>, i32) -> void>(%570, addr_of<ptr<@type28>>(%575), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type28>, i32) -> void>(%570, addr_of<ptr<@type28>>(%576), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type28>, i32) -> void>(%570, addr_of<ptr<@type28>>(%577), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %578 @test2_28(%579 s1: @type28, %580 s2: @type28) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type28, @type28, @type28) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%574, copy<@type28, reason=arg>(read<@type28>(%579)), copy<@type28, reason=arg>(read<@type28>(%564)), copy<@type28, reason=arg>(read<@type28>(%580)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %581 @testit28() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type28>, i32) -> void>(%566, addr_of<ptr<@type28>>(%563), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type28>, i32) -> void>(%570, addr_of<ptr<@type28>>(%563), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type28>, i32) -> void>(%566, addr_of<ptr<@type28>>(%564), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type28>, i32) -> void>(%570, addr_of<ptr<@type28>>(%564), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type28>, i32) -> void>(%566, addr_of<ptr<@type28>>(%565), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type28>, i32) -> void>(%570, addr_of<ptr<@type28>>(%565), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type28, @type28, @type28) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%574, copy<@type28, reason=arg>(read<@type28>(%563)), copy<@type28, reason=arg>(read<@type28>(%564)), copy<@type28, reason=arg>(read<@type28>(%565)));
// DEFAULT-NEXT:         call<void, signature=fn(@type28, @type28) -> void, abi=sysv64(native_c, native_c) -> void>(%578, copy<@type28, reason=arg>(read<@type28>(%563)), copy<@type28, reason=arg>(read<@type28>(%565)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %586 @init29(%587 p: ptr<@type29>, %588 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %589 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1342
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%589, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%589), const<i32>(29))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1528: i32 [synthetic] = read<i32>(%589);
// DEFAULT-NEXT:                 let %1529: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1528), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%589, read<i32>(%1529));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(29)>(field0(deref(read<ptr<@type29>>(%587)))), read<i32>(%589))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%588), read<i32>(%589)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %590 @check29(%591 p: ptr<@type29>, %592 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %593 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1343
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%593, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%593), const<i32>(29))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1530: i32 [synthetic] = read<i32>(%593);
// DEFAULT-NEXT:                 let %1531: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1530), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%593, read<i32>(%1531));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(29)>(field0(deref(read<ptr<@type29>>(%591)))), read<i32>(%593)))))), add<i32, overflow=ub>(read<i32>(%592), read<i32>(%593)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %594 @test29(%595 s1: @type29, %596 s2: @type29, %597 s3: @type29) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type29>, i32) -> void>(%590, addr_of<ptr<@type29>>(%595), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type29>, i32) -> void>(%590, addr_of<ptr<@type29>>(%596), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type29>, i32) -> void>(%590, addr_of<ptr<@type29>>(%597), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %598 @test2_29(%599 s1: @type29, %600 s2: @type29) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type29, @type29, @type29) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%594, copy<@type29, reason=arg>(read<@type29>(%599)), copy<@type29, reason=arg>(read<@type29>(%584)), copy<@type29, reason=arg>(read<@type29>(%600)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %601 @testit29() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type29>, i32) -> void>(%586, addr_of<ptr<@type29>>(%583), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type29>, i32) -> void>(%590, addr_of<ptr<@type29>>(%583), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type29>, i32) -> void>(%586, addr_of<ptr<@type29>>(%584), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type29>, i32) -> void>(%590, addr_of<ptr<@type29>>(%584), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type29>, i32) -> void>(%586, addr_of<ptr<@type29>>(%585), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type29>, i32) -> void>(%590, addr_of<ptr<@type29>>(%585), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type29, @type29, @type29) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%594, copy<@type29, reason=arg>(read<@type29>(%583)), copy<@type29, reason=arg>(read<@type29>(%584)), copy<@type29, reason=arg>(read<@type29>(%585)));
// DEFAULT-NEXT:         call<void, signature=fn(@type29, @type29) -> void, abi=sysv64(native_c, native_c) -> void>(%598, copy<@type29, reason=arg>(read<@type29>(%583)), copy<@type29, reason=arg>(read<@type29>(%585)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %606 @init30(%607 p: ptr<@type30>, %608 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %609 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1344
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%609, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%609), const<i32>(30))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1532: i32 [synthetic] = read<i32>(%609);
// DEFAULT-NEXT:                 let %1533: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1532), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%609, read<i32>(%1533));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(30)>(field0(deref(read<ptr<@type30>>(%607)))), read<i32>(%609))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%608), read<i32>(%609)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %610 @check30(%611 p: ptr<@type30>, %612 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %613 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1345
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%613, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%613), const<i32>(30))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1534: i32 [synthetic] = read<i32>(%613);
// DEFAULT-NEXT:                 let %1535: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1534), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%613, read<i32>(%1535));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(30)>(field0(deref(read<ptr<@type30>>(%611)))), read<i32>(%613)))))), add<i32, overflow=ub>(read<i32>(%612), read<i32>(%613)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %614 @test30(%615 s1: @type30, %616 s2: @type30, %617 s3: @type30) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type30>, i32) -> void>(%610, addr_of<ptr<@type30>>(%615), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type30>, i32) -> void>(%610, addr_of<ptr<@type30>>(%616), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type30>, i32) -> void>(%610, addr_of<ptr<@type30>>(%617), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %618 @test2_30(%619 s1: @type30, %620 s2: @type30) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type30, @type30, @type30) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%614, copy<@type30, reason=arg>(read<@type30>(%619)), copy<@type30, reason=arg>(read<@type30>(%604)), copy<@type30, reason=arg>(read<@type30>(%620)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %621 @testit30() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type30>, i32) -> void>(%606, addr_of<ptr<@type30>>(%603), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type30>, i32) -> void>(%610, addr_of<ptr<@type30>>(%603), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type30>, i32) -> void>(%606, addr_of<ptr<@type30>>(%604), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type30>, i32) -> void>(%610, addr_of<ptr<@type30>>(%604), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type30>, i32) -> void>(%606, addr_of<ptr<@type30>>(%605), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type30>, i32) -> void>(%610, addr_of<ptr<@type30>>(%605), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type30, @type30, @type30) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%614, copy<@type30, reason=arg>(read<@type30>(%603)), copy<@type30, reason=arg>(read<@type30>(%604)), copy<@type30, reason=arg>(read<@type30>(%605)));
// DEFAULT-NEXT:         call<void, signature=fn(@type30, @type30) -> void, abi=sysv64(native_c, native_c) -> void>(%618, copy<@type30, reason=arg>(read<@type30>(%603)), copy<@type30, reason=arg>(read<@type30>(%605)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %626 @init31(%627 p: ptr<@type31>, %628 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %629 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1346
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%629, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%629), const<i32>(31))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1536: i32 [synthetic] = read<i32>(%629);
// DEFAULT-NEXT:                 let %1537: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1536), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%629, read<i32>(%1537));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(31)>(field0(deref(read<ptr<@type31>>(%627)))), read<i32>(%629))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%628), read<i32>(%629)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %630 @check31(%631 p: ptr<@type31>, %632 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %633 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1347
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%633, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%633), const<i32>(31))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1538: i32 [synthetic] = read<i32>(%633);
// DEFAULT-NEXT:                 let %1539: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1538), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%633, read<i32>(%1539));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(31)>(field0(deref(read<ptr<@type31>>(%631)))), read<i32>(%633)))))), add<i32, overflow=ub>(read<i32>(%632), read<i32>(%633)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %634 @test31(%635 s1: @type31, %636 s2: @type31, %637 s3: @type31) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type31>, i32) -> void>(%630, addr_of<ptr<@type31>>(%635), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type31>, i32) -> void>(%630, addr_of<ptr<@type31>>(%636), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type31>, i32) -> void>(%630, addr_of<ptr<@type31>>(%637), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %638 @test2_31(%639 s1: @type31, %640 s2: @type31) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type31, @type31, @type31) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%634, copy<@type31, reason=arg>(read<@type31>(%639)), copy<@type31, reason=arg>(read<@type31>(%624)), copy<@type31, reason=arg>(read<@type31>(%640)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %641 @testit31() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type31>, i32) -> void>(%626, addr_of<ptr<@type31>>(%623), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type31>, i32) -> void>(%630, addr_of<ptr<@type31>>(%623), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type31>, i32) -> void>(%626, addr_of<ptr<@type31>>(%624), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type31>, i32) -> void>(%630, addr_of<ptr<@type31>>(%624), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type31>, i32) -> void>(%626, addr_of<ptr<@type31>>(%625), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type31>, i32) -> void>(%630, addr_of<ptr<@type31>>(%625), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type31, @type31, @type31) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%634, copy<@type31, reason=arg>(read<@type31>(%623)), copy<@type31, reason=arg>(read<@type31>(%624)), copy<@type31, reason=arg>(read<@type31>(%625)));
// DEFAULT-NEXT:         call<void, signature=fn(@type31, @type31) -> void, abi=sysv64(native_c, native_c) -> void>(%638, copy<@type31, reason=arg>(read<@type31>(%623)), copy<@type31, reason=arg>(read<@type31>(%625)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %646 @init32(%647 p: ptr<@type32>, %648 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %649 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1348
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%649, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%649), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1540: i32 [synthetic] = read<i32>(%649);
// DEFAULT-NEXT:                 let %1541: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1540), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%649, read<i32>(%1541));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(field0(deref(read<ptr<@type32>>(%647)))), read<i32>(%649))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%648), read<i32>(%649)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %650 @check32(%651 p: ptr<@type32>, %652 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %653 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1349
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%653, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%653), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1542: i32 [synthetic] = read<i32>(%653);
// DEFAULT-NEXT:                 let %1543: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1542), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%653, read<i32>(%1543));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(field0(deref(read<ptr<@type32>>(%651)))), read<i32>(%653)))))), add<i32, overflow=ub>(read<i32>(%652), read<i32>(%653)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %654 @test32(%655 s1: @type32, %656 s2: @type32, %657 s3: @type32) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type32>, i32) -> void>(%650, addr_of<ptr<@type32>>(%655), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type32>, i32) -> void>(%650, addr_of<ptr<@type32>>(%656), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type32>, i32) -> void>(%650, addr_of<ptr<@type32>>(%657), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %658 @test2_32(%659 s1: @type32, %660 s2: @type32) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type32, @type32, @type32) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%654, copy<@type32, reason=arg>(read<@type32>(%659)), copy<@type32, reason=arg>(read<@type32>(%644)), copy<@type32, reason=arg>(read<@type32>(%660)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %661 @testit32() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type32>, i32) -> void>(%646, addr_of<ptr<@type32>>(%643), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type32>, i32) -> void>(%650, addr_of<ptr<@type32>>(%643), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type32>, i32) -> void>(%646, addr_of<ptr<@type32>>(%644), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type32>, i32) -> void>(%650, addr_of<ptr<@type32>>(%644), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type32>, i32) -> void>(%646, addr_of<ptr<@type32>>(%645), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type32>, i32) -> void>(%650, addr_of<ptr<@type32>>(%645), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type32, @type32, @type32) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%654, copy<@type32, reason=arg>(read<@type32>(%643)), copy<@type32, reason=arg>(read<@type32>(%644)), copy<@type32, reason=arg>(read<@type32>(%645)));
// DEFAULT-NEXT:         call<void, signature=fn(@type32, @type32) -> void, abi=sysv64(native_c, native_c) -> void>(%658, copy<@type32, reason=arg>(read<@type32>(%643)), copy<@type32, reason=arg>(read<@type32>(%645)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %666 @init33(%667 p: ptr<@type33>, %668 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %669 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1350
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%669, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%669), const<i32>(33))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1544: i32 [synthetic] = read<i32>(%669);
// DEFAULT-NEXT:                 let %1545: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1544), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%669, read<i32>(%1545));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(33)>(field0(deref(read<ptr<@type33>>(%667)))), read<i32>(%669))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%668), read<i32>(%669)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %670 @check33(%671 p: ptr<@type33>, %672 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %673 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1351
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%673, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%673), const<i32>(33))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1546: i32 [synthetic] = read<i32>(%673);
// DEFAULT-NEXT:                 let %1547: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1546), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%673, read<i32>(%1547));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(33)>(field0(deref(read<ptr<@type33>>(%671)))), read<i32>(%673)))))), add<i32, overflow=ub>(read<i32>(%672), read<i32>(%673)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %674 @test33(%675 s1: @type33, %676 s2: @type33, %677 s3: @type33) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type33>, i32) -> void>(%670, addr_of<ptr<@type33>>(%675), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type33>, i32) -> void>(%670, addr_of<ptr<@type33>>(%676), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type33>, i32) -> void>(%670, addr_of<ptr<@type33>>(%677), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %678 @test2_33(%679 s1: @type33, %680 s2: @type33) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type33, @type33, @type33) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%674, copy<@type33, reason=arg>(read<@type33>(%679)), copy<@type33, reason=arg>(read<@type33>(%664)), copy<@type33, reason=arg>(read<@type33>(%680)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %681 @testit33() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type33>, i32) -> void>(%666, addr_of<ptr<@type33>>(%663), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type33>, i32) -> void>(%670, addr_of<ptr<@type33>>(%663), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type33>, i32) -> void>(%666, addr_of<ptr<@type33>>(%664), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type33>, i32) -> void>(%670, addr_of<ptr<@type33>>(%664), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type33>, i32) -> void>(%666, addr_of<ptr<@type33>>(%665), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type33>, i32) -> void>(%670, addr_of<ptr<@type33>>(%665), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type33, @type33, @type33) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%674, copy<@type33, reason=arg>(read<@type33>(%663)), copy<@type33, reason=arg>(read<@type33>(%664)), copy<@type33, reason=arg>(read<@type33>(%665)));
// DEFAULT-NEXT:         call<void, signature=fn(@type33, @type33) -> void, abi=sysv64(native_c, native_c) -> void>(%678, copy<@type33, reason=arg>(read<@type33>(%663)), copy<@type33, reason=arg>(read<@type33>(%665)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %686 @init34(%687 p: ptr<@type34>, %688 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %689 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1352
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%689, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%689), const<i32>(34))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1548: i32 [synthetic] = read<i32>(%689);
// DEFAULT-NEXT:                 let %1549: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1548), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%689, read<i32>(%1549));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(34)>(field0(deref(read<ptr<@type34>>(%687)))), read<i32>(%689))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%688), read<i32>(%689)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %690 @check34(%691 p: ptr<@type34>, %692 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %693 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1353
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%693, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%693), const<i32>(34))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1550: i32 [synthetic] = read<i32>(%693);
// DEFAULT-NEXT:                 let %1551: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1550), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%693, read<i32>(%1551));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(34)>(field0(deref(read<ptr<@type34>>(%691)))), read<i32>(%693)))))), add<i32, overflow=ub>(read<i32>(%692), read<i32>(%693)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %694 @test34(%695 s1: @type34, %696 s2: @type34, %697 s3: @type34) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type34>, i32) -> void>(%690, addr_of<ptr<@type34>>(%695), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type34>, i32) -> void>(%690, addr_of<ptr<@type34>>(%696), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type34>, i32) -> void>(%690, addr_of<ptr<@type34>>(%697), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %698 @test2_34(%699 s1: @type34, %700 s2: @type34) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type34, @type34, @type34) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%694, copy<@type34, reason=arg>(read<@type34>(%699)), copy<@type34, reason=arg>(read<@type34>(%684)), copy<@type34, reason=arg>(read<@type34>(%700)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %701 @testit34() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type34>, i32) -> void>(%686, addr_of<ptr<@type34>>(%683), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type34>, i32) -> void>(%690, addr_of<ptr<@type34>>(%683), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type34>, i32) -> void>(%686, addr_of<ptr<@type34>>(%684), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type34>, i32) -> void>(%690, addr_of<ptr<@type34>>(%684), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type34>, i32) -> void>(%686, addr_of<ptr<@type34>>(%685), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type34>, i32) -> void>(%690, addr_of<ptr<@type34>>(%685), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type34, @type34, @type34) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%694, copy<@type34, reason=arg>(read<@type34>(%683)), copy<@type34, reason=arg>(read<@type34>(%684)), copy<@type34, reason=arg>(read<@type34>(%685)));
// DEFAULT-NEXT:         call<void, signature=fn(@type34, @type34) -> void, abi=sysv64(native_c, native_c) -> void>(%698, copy<@type34, reason=arg>(read<@type34>(%683)), copy<@type34, reason=arg>(read<@type34>(%685)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %706 @init35(%707 p: ptr<@type35>, %708 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %709 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1354
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%709, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%709), const<i32>(35))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1552: i32 [synthetic] = read<i32>(%709);
// DEFAULT-NEXT:                 let %1553: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1552), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%709, read<i32>(%1553));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(35)>(field0(deref(read<ptr<@type35>>(%707)))), read<i32>(%709))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%708), read<i32>(%709)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %710 @check35(%711 p: ptr<@type35>, %712 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %713 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1355
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%713, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%713), const<i32>(35))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1554: i32 [synthetic] = read<i32>(%713);
// DEFAULT-NEXT:                 let %1555: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1554), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%713, read<i32>(%1555));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(35)>(field0(deref(read<ptr<@type35>>(%711)))), read<i32>(%713)))))), add<i32, overflow=ub>(read<i32>(%712), read<i32>(%713)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %714 @test35(%715 s1: @type35, %716 s2: @type35, %717 s3: @type35) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type35>, i32) -> void>(%710, addr_of<ptr<@type35>>(%715), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type35>, i32) -> void>(%710, addr_of<ptr<@type35>>(%716), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type35>, i32) -> void>(%710, addr_of<ptr<@type35>>(%717), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %718 @test2_35(%719 s1: @type35, %720 s2: @type35) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type35, @type35, @type35) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%714, copy<@type35, reason=arg>(read<@type35>(%719)), copy<@type35, reason=arg>(read<@type35>(%704)), copy<@type35, reason=arg>(read<@type35>(%720)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %721 @testit35() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type35>, i32) -> void>(%706, addr_of<ptr<@type35>>(%703), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type35>, i32) -> void>(%710, addr_of<ptr<@type35>>(%703), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type35>, i32) -> void>(%706, addr_of<ptr<@type35>>(%704), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type35>, i32) -> void>(%710, addr_of<ptr<@type35>>(%704), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type35>, i32) -> void>(%706, addr_of<ptr<@type35>>(%705), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type35>, i32) -> void>(%710, addr_of<ptr<@type35>>(%705), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type35, @type35, @type35) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%714, copy<@type35, reason=arg>(read<@type35>(%703)), copy<@type35, reason=arg>(read<@type35>(%704)), copy<@type35, reason=arg>(read<@type35>(%705)));
// DEFAULT-NEXT:         call<void, signature=fn(@type35, @type35) -> void, abi=sysv64(native_c, native_c) -> void>(%718, copy<@type35, reason=arg>(read<@type35>(%703)), copy<@type35, reason=arg>(read<@type35>(%705)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %726 @init36(%727 p: ptr<@type36>, %728 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %729 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1356
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%729, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%729), const<i32>(36))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1556: i32 [synthetic] = read<i32>(%729);
// DEFAULT-NEXT:                 let %1557: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1556), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%729, read<i32>(%1557));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(36)>(field0(deref(read<ptr<@type36>>(%727)))), read<i32>(%729))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%728), read<i32>(%729)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %730 @check36(%731 p: ptr<@type36>, %732 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %733 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1357
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%733, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%733), const<i32>(36))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1558: i32 [synthetic] = read<i32>(%733);
// DEFAULT-NEXT:                 let %1559: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1558), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%733, read<i32>(%1559));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(36)>(field0(deref(read<ptr<@type36>>(%731)))), read<i32>(%733)))))), add<i32, overflow=ub>(read<i32>(%732), read<i32>(%733)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %734 @test36(%735 s1: @type36, %736 s2: @type36, %737 s3: @type36) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type36>, i32) -> void>(%730, addr_of<ptr<@type36>>(%735), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type36>, i32) -> void>(%730, addr_of<ptr<@type36>>(%736), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type36>, i32) -> void>(%730, addr_of<ptr<@type36>>(%737), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %738 @test2_36(%739 s1: @type36, %740 s2: @type36) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type36, @type36, @type36) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%734, copy<@type36, reason=arg>(read<@type36>(%739)), copy<@type36, reason=arg>(read<@type36>(%724)), copy<@type36, reason=arg>(read<@type36>(%740)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %741 @testit36() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type36>, i32) -> void>(%726, addr_of<ptr<@type36>>(%723), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type36>, i32) -> void>(%730, addr_of<ptr<@type36>>(%723), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type36>, i32) -> void>(%726, addr_of<ptr<@type36>>(%724), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type36>, i32) -> void>(%730, addr_of<ptr<@type36>>(%724), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type36>, i32) -> void>(%726, addr_of<ptr<@type36>>(%725), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type36>, i32) -> void>(%730, addr_of<ptr<@type36>>(%725), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type36, @type36, @type36) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%734, copy<@type36, reason=arg>(read<@type36>(%723)), copy<@type36, reason=arg>(read<@type36>(%724)), copy<@type36, reason=arg>(read<@type36>(%725)));
// DEFAULT-NEXT:         call<void, signature=fn(@type36, @type36) -> void, abi=sysv64(native_c, native_c) -> void>(%738, copy<@type36, reason=arg>(read<@type36>(%723)), copy<@type36, reason=arg>(read<@type36>(%725)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %746 @init37(%747 p: ptr<@type37>, %748 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %749 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1358
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%749, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%749), const<i32>(37))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1560: i32 [synthetic] = read<i32>(%749);
// DEFAULT-NEXT:                 let %1561: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1560), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%749, read<i32>(%1561));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(37)>(field0(deref(read<ptr<@type37>>(%747)))), read<i32>(%749))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%748), read<i32>(%749)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %750 @check37(%751 p: ptr<@type37>, %752 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %753 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1359
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%753, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%753), const<i32>(37))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1562: i32 [synthetic] = read<i32>(%753);
// DEFAULT-NEXT:                 let %1563: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1562), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%753, read<i32>(%1563));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(37)>(field0(deref(read<ptr<@type37>>(%751)))), read<i32>(%753)))))), add<i32, overflow=ub>(read<i32>(%752), read<i32>(%753)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %754 @test37(%755 s1: @type37, %756 s2: @type37, %757 s3: @type37) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type37>, i32) -> void>(%750, addr_of<ptr<@type37>>(%755), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type37>, i32) -> void>(%750, addr_of<ptr<@type37>>(%756), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type37>, i32) -> void>(%750, addr_of<ptr<@type37>>(%757), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %758 @test2_37(%759 s1: @type37, %760 s2: @type37) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type37, @type37, @type37) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%754, copy<@type37, reason=arg>(read<@type37>(%759)), copy<@type37, reason=arg>(read<@type37>(%744)), copy<@type37, reason=arg>(read<@type37>(%760)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %761 @testit37() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type37>, i32) -> void>(%746, addr_of<ptr<@type37>>(%743), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type37>, i32) -> void>(%750, addr_of<ptr<@type37>>(%743), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type37>, i32) -> void>(%746, addr_of<ptr<@type37>>(%744), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type37>, i32) -> void>(%750, addr_of<ptr<@type37>>(%744), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type37>, i32) -> void>(%746, addr_of<ptr<@type37>>(%745), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type37>, i32) -> void>(%750, addr_of<ptr<@type37>>(%745), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type37, @type37, @type37) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%754, copy<@type37, reason=arg>(read<@type37>(%743)), copy<@type37, reason=arg>(read<@type37>(%744)), copy<@type37, reason=arg>(read<@type37>(%745)));
// DEFAULT-NEXT:         call<void, signature=fn(@type37, @type37) -> void, abi=sysv64(native_c, native_c) -> void>(%758, copy<@type37, reason=arg>(read<@type37>(%743)), copy<@type37, reason=arg>(read<@type37>(%745)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %766 @init38(%767 p: ptr<@type38>, %768 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %769 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1360
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%769, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%769), const<i32>(38))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1564: i32 [synthetic] = read<i32>(%769);
// DEFAULT-NEXT:                 let %1565: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1564), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%769, read<i32>(%1565));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(38)>(field0(deref(read<ptr<@type38>>(%767)))), read<i32>(%769))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%768), read<i32>(%769)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %770 @check38(%771 p: ptr<@type38>, %772 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %773 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1361
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%773, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%773), const<i32>(38))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1566: i32 [synthetic] = read<i32>(%773);
// DEFAULT-NEXT:                 let %1567: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1566), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%773, read<i32>(%1567));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(38)>(field0(deref(read<ptr<@type38>>(%771)))), read<i32>(%773)))))), add<i32, overflow=ub>(read<i32>(%772), read<i32>(%773)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %774 @test38(%775 s1: @type38, %776 s2: @type38, %777 s3: @type38) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type38>, i32) -> void>(%770, addr_of<ptr<@type38>>(%775), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type38>, i32) -> void>(%770, addr_of<ptr<@type38>>(%776), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type38>, i32) -> void>(%770, addr_of<ptr<@type38>>(%777), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %778 @test2_38(%779 s1: @type38, %780 s2: @type38) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type38, @type38, @type38) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%774, copy<@type38, reason=arg>(read<@type38>(%779)), copy<@type38, reason=arg>(read<@type38>(%764)), copy<@type38, reason=arg>(read<@type38>(%780)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %781 @testit38() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type38>, i32) -> void>(%766, addr_of<ptr<@type38>>(%763), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type38>, i32) -> void>(%770, addr_of<ptr<@type38>>(%763), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type38>, i32) -> void>(%766, addr_of<ptr<@type38>>(%764), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type38>, i32) -> void>(%770, addr_of<ptr<@type38>>(%764), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type38>, i32) -> void>(%766, addr_of<ptr<@type38>>(%765), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type38>, i32) -> void>(%770, addr_of<ptr<@type38>>(%765), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type38, @type38, @type38) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%774, copy<@type38, reason=arg>(read<@type38>(%763)), copy<@type38, reason=arg>(read<@type38>(%764)), copy<@type38, reason=arg>(read<@type38>(%765)));
// DEFAULT-NEXT:         call<void, signature=fn(@type38, @type38) -> void, abi=sysv64(native_c, native_c) -> void>(%778, copy<@type38, reason=arg>(read<@type38>(%763)), copy<@type38, reason=arg>(read<@type38>(%765)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %786 @init39(%787 p: ptr<@type39>, %788 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %789 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1362
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%789, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%789), const<i32>(39))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1568: i32 [synthetic] = read<i32>(%789);
// DEFAULT-NEXT:                 let %1569: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1568), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%789, read<i32>(%1569));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(39)>(field0(deref(read<ptr<@type39>>(%787)))), read<i32>(%789))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%788), read<i32>(%789)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %790 @check39(%791 p: ptr<@type39>, %792 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %793 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1363
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%793, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%793), const<i32>(39))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1570: i32 [synthetic] = read<i32>(%793);
// DEFAULT-NEXT:                 let %1571: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1570), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%793, read<i32>(%1571));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(39)>(field0(deref(read<ptr<@type39>>(%791)))), read<i32>(%793)))))), add<i32, overflow=ub>(read<i32>(%792), read<i32>(%793)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %794 @test39(%795 s1: @type39, %796 s2: @type39, %797 s3: @type39) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type39>, i32) -> void>(%790, addr_of<ptr<@type39>>(%795), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type39>, i32) -> void>(%790, addr_of<ptr<@type39>>(%796), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type39>, i32) -> void>(%790, addr_of<ptr<@type39>>(%797), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %798 @test2_39(%799 s1: @type39, %800 s2: @type39) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type39, @type39, @type39) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%794, copy<@type39, reason=arg>(read<@type39>(%799)), copy<@type39, reason=arg>(read<@type39>(%784)), copy<@type39, reason=arg>(read<@type39>(%800)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %801 @testit39() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type39>, i32) -> void>(%786, addr_of<ptr<@type39>>(%783), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type39>, i32) -> void>(%790, addr_of<ptr<@type39>>(%783), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type39>, i32) -> void>(%786, addr_of<ptr<@type39>>(%784), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type39>, i32) -> void>(%790, addr_of<ptr<@type39>>(%784), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type39>, i32) -> void>(%786, addr_of<ptr<@type39>>(%785), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type39>, i32) -> void>(%790, addr_of<ptr<@type39>>(%785), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type39, @type39, @type39) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%794, copy<@type39, reason=arg>(read<@type39>(%783)), copy<@type39, reason=arg>(read<@type39>(%784)), copy<@type39, reason=arg>(read<@type39>(%785)));
// DEFAULT-NEXT:         call<void, signature=fn(@type39, @type39) -> void, abi=sysv64(native_c, native_c) -> void>(%798, copy<@type39, reason=arg>(read<@type39>(%783)), copy<@type39, reason=arg>(read<@type39>(%785)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %806 @init40(%807 p: ptr<@type40>, %808 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %809 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1364
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%809, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%809), const<i32>(40))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1572: i32 [synthetic] = read<i32>(%809);
// DEFAULT-NEXT:                 let %1573: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1572), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%809, read<i32>(%1573));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(40)>(field0(deref(read<ptr<@type40>>(%807)))), read<i32>(%809))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%808), read<i32>(%809)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %810 @check40(%811 p: ptr<@type40>, %812 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %813 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1365
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%813, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%813), const<i32>(40))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1574: i32 [synthetic] = read<i32>(%813);
// DEFAULT-NEXT:                 let %1575: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1574), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%813, read<i32>(%1575));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(40)>(field0(deref(read<ptr<@type40>>(%811)))), read<i32>(%813)))))), add<i32, overflow=ub>(read<i32>(%812), read<i32>(%813)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %814 @test40(%815 s1: @type40, %816 s2: @type40, %817 s3: @type40) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type40>, i32) -> void>(%810, addr_of<ptr<@type40>>(%815), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type40>, i32) -> void>(%810, addr_of<ptr<@type40>>(%816), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type40>, i32) -> void>(%810, addr_of<ptr<@type40>>(%817), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %818 @test2_40(%819 s1: @type40, %820 s2: @type40) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type40, @type40, @type40) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%814, copy<@type40, reason=arg>(read<@type40>(%819)), copy<@type40, reason=arg>(read<@type40>(%804)), copy<@type40, reason=arg>(read<@type40>(%820)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %821 @testit40() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type40>, i32) -> void>(%806, addr_of<ptr<@type40>>(%803), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type40>, i32) -> void>(%810, addr_of<ptr<@type40>>(%803), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type40>, i32) -> void>(%806, addr_of<ptr<@type40>>(%804), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type40>, i32) -> void>(%810, addr_of<ptr<@type40>>(%804), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type40>, i32) -> void>(%806, addr_of<ptr<@type40>>(%805), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type40>, i32) -> void>(%810, addr_of<ptr<@type40>>(%805), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type40, @type40, @type40) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%814, copy<@type40, reason=arg>(read<@type40>(%803)), copy<@type40, reason=arg>(read<@type40>(%804)), copy<@type40, reason=arg>(read<@type40>(%805)));
// DEFAULT-NEXT:         call<void, signature=fn(@type40, @type40) -> void, abi=sysv64(native_c, native_c) -> void>(%818, copy<@type40, reason=arg>(read<@type40>(%803)), copy<@type40, reason=arg>(read<@type40>(%805)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %826 @init41(%827 p: ptr<@type41>, %828 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %829 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1366
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%829, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%829), const<i32>(41))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1576: i32 [synthetic] = read<i32>(%829);
// DEFAULT-NEXT:                 let %1577: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1576), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%829, read<i32>(%1577));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(41)>(field0(deref(read<ptr<@type41>>(%827)))), read<i32>(%829))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%828), read<i32>(%829)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %830 @check41(%831 p: ptr<@type41>, %832 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %833 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1367
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%833, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%833), const<i32>(41))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1578: i32 [synthetic] = read<i32>(%833);
// DEFAULT-NEXT:                 let %1579: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1578), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%833, read<i32>(%1579));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(41)>(field0(deref(read<ptr<@type41>>(%831)))), read<i32>(%833)))))), add<i32, overflow=ub>(read<i32>(%832), read<i32>(%833)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %834 @test41(%835 s1: @type41, %836 s2: @type41, %837 s3: @type41) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type41>, i32) -> void>(%830, addr_of<ptr<@type41>>(%835), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type41>, i32) -> void>(%830, addr_of<ptr<@type41>>(%836), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type41>, i32) -> void>(%830, addr_of<ptr<@type41>>(%837), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %838 @test2_41(%839 s1: @type41, %840 s2: @type41) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type41, @type41, @type41) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%834, copy<@type41, reason=arg>(read<@type41>(%839)), copy<@type41, reason=arg>(read<@type41>(%824)), copy<@type41, reason=arg>(read<@type41>(%840)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %841 @testit41() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type41>, i32) -> void>(%826, addr_of<ptr<@type41>>(%823), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type41>, i32) -> void>(%830, addr_of<ptr<@type41>>(%823), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type41>, i32) -> void>(%826, addr_of<ptr<@type41>>(%824), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type41>, i32) -> void>(%830, addr_of<ptr<@type41>>(%824), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type41>, i32) -> void>(%826, addr_of<ptr<@type41>>(%825), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type41>, i32) -> void>(%830, addr_of<ptr<@type41>>(%825), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type41, @type41, @type41) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%834, copy<@type41, reason=arg>(read<@type41>(%823)), copy<@type41, reason=arg>(read<@type41>(%824)), copy<@type41, reason=arg>(read<@type41>(%825)));
// DEFAULT-NEXT:         call<void, signature=fn(@type41, @type41) -> void, abi=sysv64(native_c, native_c) -> void>(%838, copy<@type41, reason=arg>(read<@type41>(%823)), copy<@type41, reason=arg>(read<@type41>(%825)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %846 @init42(%847 p: ptr<@type42>, %848 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %849 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1368
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%849, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%849), const<i32>(42))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1580: i32 [synthetic] = read<i32>(%849);
// DEFAULT-NEXT:                 let %1581: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1580), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%849, read<i32>(%1581));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(42)>(field0(deref(read<ptr<@type42>>(%847)))), read<i32>(%849))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%848), read<i32>(%849)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %850 @check42(%851 p: ptr<@type42>, %852 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %853 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1369
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%853, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%853), const<i32>(42))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1582: i32 [synthetic] = read<i32>(%853);
// DEFAULT-NEXT:                 let %1583: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1582), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%853, read<i32>(%1583));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(42)>(field0(deref(read<ptr<@type42>>(%851)))), read<i32>(%853)))))), add<i32, overflow=ub>(read<i32>(%852), read<i32>(%853)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %854 @test42(%855 s1: @type42, %856 s2: @type42, %857 s3: @type42) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type42>, i32) -> void>(%850, addr_of<ptr<@type42>>(%855), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type42>, i32) -> void>(%850, addr_of<ptr<@type42>>(%856), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type42>, i32) -> void>(%850, addr_of<ptr<@type42>>(%857), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %858 @test2_42(%859 s1: @type42, %860 s2: @type42) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type42, @type42, @type42) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%854, copy<@type42, reason=arg>(read<@type42>(%859)), copy<@type42, reason=arg>(read<@type42>(%844)), copy<@type42, reason=arg>(read<@type42>(%860)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %861 @testit42() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type42>, i32) -> void>(%846, addr_of<ptr<@type42>>(%843), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type42>, i32) -> void>(%850, addr_of<ptr<@type42>>(%843), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type42>, i32) -> void>(%846, addr_of<ptr<@type42>>(%844), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type42>, i32) -> void>(%850, addr_of<ptr<@type42>>(%844), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type42>, i32) -> void>(%846, addr_of<ptr<@type42>>(%845), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type42>, i32) -> void>(%850, addr_of<ptr<@type42>>(%845), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type42, @type42, @type42) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%854, copy<@type42, reason=arg>(read<@type42>(%843)), copy<@type42, reason=arg>(read<@type42>(%844)), copy<@type42, reason=arg>(read<@type42>(%845)));
// DEFAULT-NEXT:         call<void, signature=fn(@type42, @type42) -> void, abi=sysv64(native_c, native_c) -> void>(%858, copy<@type42, reason=arg>(read<@type42>(%843)), copy<@type42, reason=arg>(read<@type42>(%845)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %866 @init43(%867 p: ptr<@type43>, %868 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %869 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1370
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%869, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%869), const<i32>(43))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1584: i32 [synthetic] = read<i32>(%869);
// DEFAULT-NEXT:                 let %1585: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1584), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%869, read<i32>(%1585));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(43)>(field0(deref(read<ptr<@type43>>(%867)))), read<i32>(%869))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%868), read<i32>(%869)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %870 @check43(%871 p: ptr<@type43>, %872 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %873 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1371
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%873, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%873), const<i32>(43))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1586: i32 [synthetic] = read<i32>(%873);
// DEFAULT-NEXT:                 let %1587: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1586), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%873, read<i32>(%1587));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(43)>(field0(deref(read<ptr<@type43>>(%871)))), read<i32>(%873)))))), add<i32, overflow=ub>(read<i32>(%872), read<i32>(%873)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %874 @test43(%875 s1: @type43, %876 s2: @type43, %877 s3: @type43) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type43>, i32) -> void>(%870, addr_of<ptr<@type43>>(%875), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type43>, i32) -> void>(%870, addr_of<ptr<@type43>>(%876), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type43>, i32) -> void>(%870, addr_of<ptr<@type43>>(%877), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %878 @test2_43(%879 s1: @type43, %880 s2: @type43) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type43, @type43, @type43) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%874, copy<@type43, reason=arg>(read<@type43>(%879)), copy<@type43, reason=arg>(read<@type43>(%864)), copy<@type43, reason=arg>(read<@type43>(%880)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %881 @testit43() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type43>, i32) -> void>(%866, addr_of<ptr<@type43>>(%863), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type43>, i32) -> void>(%870, addr_of<ptr<@type43>>(%863), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type43>, i32) -> void>(%866, addr_of<ptr<@type43>>(%864), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type43>, i32) -> void>(%870, addr_of<ptr<@type43>>(%864), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type43>, i32) -> void>(%866, addr_of<ptr<@type43>>(%865), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type43>, i32) -> void>(%870, addr_of<ptr<@type43>>(%865), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type43, @type43, @type43) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%874, copy<@type43, reason=arg>(read<@type43>(%863)), copy<@type43, reason=arg>(read<@type43>(%864)), copy<@type43, reason=arg>(read<@type43>(%865)));
// DEFAULT-NEXT:         call<void, signature=fn(@type43, @type43) -> void, abi=sysv64(native_c, native_c) -> void>(%878, copy<@type43, reason=arg>(read<@type43>(%863)), copy<@type43, reason=arg>(read<@type43>(%865)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %886 @init44(%887 p: ptr<@type44>, %888 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %889 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1372
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%889, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%889), const<i32>(44))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1588: i32 [synthetic] = read<i32>(%889);
// DEFAULT-NEXT:                 let %1589: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1588), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%889, read<i32>(%1589));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(44)>(field0(deref(read<ptr<@type44>>(%887)))), read<i32>(%889))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%888), read<i32>(%889)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %890 @check44(%891 p: ptr<@type44>, %892 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %893 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1373
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%893, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%893), const<i32>(44))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1590: i32 [synthetic] = read<i32>(%893);
// DEFAULT-NEXT:                 let %1591: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1590), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%893, read<i32>(%1591));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(44)>(field0(deref(read<ptr<@type44>>(%891)))), read<i32>(%893)))))), add<i32, overflow=ub>(read<i32>(%892), read<i32>(%893)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %894 @test44(%895 s1: @type44, %896 s2: @type44, %897 s3: @type44) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type44>, i32) -> void>(%890, addr_of<ptr<@type44>>(%895), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type44>, i32) -> void>(%890, addr_of<ptr<@type44>>(%896), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type44>, i32) -> void>(%890, addr_of<ptr<@type44>>(%897), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %898 @test2_44(%899 s1: @type44, %900 s2: @type44) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type44, @type44, @type44) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%894, copy<@type44, reason=arg>(read<@type44>(%899)), copy<@type44, reason=arg>(read<@type44>(%884)), copy<@type44, reason=arg>(read<@type44>(%900)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %901 @testit44() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type44>, i32) -> void>(%886, addr_of<ptr<@type44>>(%883), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type44>, i32) -> void>(%890, addr_of<ptr<@type44>>(%883), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type44>, i32) -> void>(%886, addr_of<ptr<@type44>>(%884), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type44>, i32) -> void>(%890, addr_of<ptr<@type44>>(%884), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type44>, i32) -> void>(%886, addr_of<ptr<@type44>>(%885), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type44>, i32) -> void>(%890, addr_of<ptr<@type44>>(%885), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type44, @type44, @type44) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%894, copy<@type44, reason=arg>(read<@type44>(%883)), copy<@type44, reason=arg>(read<@type44>(%884)), copy<@type44, reason=arg>(read<@type44>(%885)));
// DEFAULT-NEXT:         call<void, signature=fn(@type44, @type44) -> void, abi=sysv64(native_c, native_c) -> void>(%898, copy<@type44, reason=arg>(read<@type44>(%883)), copy<@type44, reason=arg>(read<@type44>(%885)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %906 @init45(%907 p: ptr<@type45>, %908 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %909 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1374
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%909, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%909), const<i32>(45))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1592: i32 [synthetic] = read<i32>(%909);
// DEFAULT-NEXT:                 let %1593: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1592), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%909, read<i32>(%1593));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(45)>(field0(deref(read<ptr<@type45>>(%907)))), read<i32>(%909))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%908), read<i32>(%909)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %910 @check45(%911 p: ptr<@type45>, %912 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %913 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1375
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%913, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%913), const<i32>(45))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1594: i32 [synthetic] = read<i32>(%913);
// DEFAULT-NEXT:                 let %1595: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1594), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%913, read<i32>(%1595));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(45)>(field0(deref(read<ptr<@type45>>(%911)))), read<i32>(%913)))))), add<i32, overflow=ub>(read<i32>(%912), read<i32>(%913)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %914 @test45(%915 s1: @type45, %916 s2: @type45, %917 s3: @type45) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type45>, i32) -> void>(%910, addr_of<ptr<@type45>>(%915), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type45>, i32) -> void>(%910, addr_of<ptr<@type45>>(%916), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type45>, i32) -> void>(%910, addr_of<ptr<@type45>>(%917), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %918 @test2_45(%919 s1: @type45, %920 s2: @type45) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type45, @type45, @type45) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%914, copy<@type45, reason=arg>(read<@type45>(%919)), copy<@type45, reason=arg>(read<@type45>(%904)), copy<@type45, reason=arg>(read<@type45>(%920)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %921 @testit45() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type45>, i32) -> void>(%906, addr_of<ptr<@type45>>(%903), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type45>, i32) -> void>(%910, addr_of<ptr<@type45>>(%903), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type45>, i32) -> void>(%906, addr_of<ptr<@type45>>(%904), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type45>, i32) -> void>(%910, addr_of<ptr<@type45>>(%904), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type45>, i32) -> void>(%906, addr_of<ptr<@type45>>(%905), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type45>, i32) -> void>(%910, addr_of<ptr<@type45>>(%905), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type45, @type45, @type45) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%914, copy<@type45, reason=arg>(read<@type45>(%903)), copy<@type45, reason=arg>(read<@type45>(%904)), copy<@type45, reason=arg>(read<@type45>(%905)));
// DEFAULT-NEXT:         call<void, signature=fn(@type45, @type45) -> void, abi=sysv64(native_c, native_c) -> void>(%918, copy<@type45, reason=arg>(read<@type45>(%903)), copy<@type45, reason=arg>(read<@type45>(%905)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %926 @init46(%927 p: ptr<@type46>, %928 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %929 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1376
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%929, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%929), const<i32>(46))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1596: i32 [synthetic] = read<i32>(%929);
// DEFAULT-NEXT:                 let %1597: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1596), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%929, read<i32>(%1597));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(46)>(field0(deref(read<ptr<@type46>>(%927)))), read<i32>(%929))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%928), read<i32>(%929)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %930 @check46(%931 p: ptr<@type46>, %932 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %933 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1377
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%933, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%933), const<i32>(46))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1598: i32 [synthetic] = read<i32>(%933);
// DEFAULT-NEXT:                 let %1599: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1598), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%933, read<i32>(%1599));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(46)>(field0(deref(read<ptr<@type46>>(%931)))), read<i32>(%933)))))), add<i32, overflow=ub>(read<i32>(%932), read<i32>(%933)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %934 @test46(%935 s1: @type46, %936 s2: @type46, %937 s3: @type46) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type46>, i32) -> void>(%930, addr_of<ptr<@type46>>(%935), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type46>, i32) -> void>(%930, addr_of<ptr<@type46>>(%936), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type46>, i32) -> void>(%930, addr_of<ptr<@type46>>(%937), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %938 @test2_46(%939 s1: @type46, %940 s2: @type46) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type46, @type46, @type46) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%934, copy<@type46, reason=arg>(read<@type46>(%939)), copy<@type46, reason=arg>(read<@type46>(%924)), copy<@type46, reason=arg>(read<@type46>(%940)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %941 @testit46() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type46>, i32) -> void>(%926, addr_of<ptr<@type46>>(%923), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type46>, i32) -> void>(%930, addr_of<ptr<@type46>>(%923), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type46>, i32) -> void>(%926, addr_of<ptr<@type46>>(%924), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type46>, i32) -> void>(%930, addr_of<ptr<@type46>>(%924), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type46>, i32) -> void>(%926, addr_of<ptr<@type46>>(%925), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type46>, i32) -> void>(%930, addr_of<ptr<@type46>>(%925), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type46, @type46, @type46) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%934, copy<@type46, reason=arg>(read<@type46>(%923)), copy<@type46, reason=arg>(read<@type46>(%924)), copy<@type46, reason=arg>(read<@type46>(%925)));
// DEFAULT-NEXT:         call<void, signature=fn(@type46, @type46) -> void, abi=sysv64(native_c, native_c) -> void>(%938, copy<@type46, reason=arg>(read<@type46>(%923)), copy<@type46, reason=arg>(read<@type46>(%925)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %946 @init47(%947 p: ptr<@type47>, %948 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %949 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1378
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%949, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%949), const<i32>(47))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1600: i32 [synthetic] = read<i32>(%949);
// DEFAULT-NEXT:                 let %1601: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1600), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%949, read<i32>(%1601));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(47)>(field0(deref(read<ptr<@type47>>(%947)))), read<i32>(%949))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%948), read<i32>(%949)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %950 @check47(%951 p: ptr<@type47>, %952 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %953 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1379
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%953, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%953), const<i32>(47))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1602: i32 [synthetic] = read<i32>(%953);
// DEFAULT-NEXT:                 let %1603: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1602), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%953, read<i32>(%1603));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(47)>(field0(deref(read<ptr<@type47>>(%951)))), read<i32>(%953)))))), add<i32, overflow=ub>(read<i32>(%952), read<i32>(%953)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %954 @test47(%955 s1: @type47, %956 s2: @type47, %957 s3: @type47) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type47>, i32) -> void>(%950, addr_of<ptr<@type47>>(%955), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type47>, i32) -> void>(%950, addr_of<ptr<@type47>>(%956), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type47>, i32) -> void>(%950, addr_of<ptr<@type47>>(%957), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %958 @test2_47(%959 s1: @type47, %960 s2: @type47) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type47, @type47, @type47) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%954, copy<@type47, reason=arg>(read<@type47>(%959)), copy<@type47, reason=arg>(read<@type47>(%944)), copy<@type47, reason=arg>(read<@type47>(%960)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %961 @testit47() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type47>, i32) -> void>(%946, addr_of<ptr<@type47>>(%943), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type47>, i32) -> void>(%950, addr_of<ptr<@type47>>(%943), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type47>, i32) -> void>(%946, addr_of<ptr<@type47>>(%944), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type47>, i32) -> void>(%950, addr_of<ptr<@type47>>(%944), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type47>, i32) -> void>(%946, addr_of<ptr<@type47>>(%945), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type47>, i32) -> void>(%950, addr_of<ptr<@type47>>(%945), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type47, @type47, @type47) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%954, copy<@type47, reason=arg>(read<@type47>(%943)), copy<@type47, reason=arg>(read<@type47>(%944)), copy<@type47, reason=arg>(read<@type47>(%945)));
// DEFAULT-NEXT:         call<void, signature=fn(@type47, @type47) -> void, abi=sysv64(native_c, native_c) -> void>(%958, copy<@type47, reason=arg>(read<@type47>(%943)), copy<@type47, reason=arg>(read<@type47>(%945)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %966 @init48(%967 p: ptr<@type48>, %968 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %969 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1380
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%969, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%969), const<i32>(48))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1604: i32 [synthetic] = read<i32>(%969);
// DEFAULT-NEXT:                 let %1605: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1604), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%969, read<i32>(%1605));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(48)>(field0(deref(read<ptr<@type48>>(%967)))), read<i32>(%969))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%968), read<i32>(%969)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %970 @check48(%971 p: ptr<@type48>, %972 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %973 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1381
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%973, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%973), const<i32>(48))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1606: i32 [synthetic] = read<i32>(%973);
// DEFAULT-NEXT:                 let %1607: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1606), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%973, read<i32>(%1607));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(48)>(field0(deref(read<ptr<@type48>>(%971)))), read<i32>(%973)))))), add<i32, overflow=ub>(read<i32>(%972), read<i32>(%973)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %974 @test48(%975 s1: @type48, %976 s2: @type48, %977 s3: @type48) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type48>, i32) -> void>(%970, addr_of<ptr<@type48>>(%975), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type48>, i32) -> void>(%970, addr_of<ptr<@type48>>(%976), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type48>, i32) -> void>(%970, addr_of<ptr<@type48>>(%977), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %978 @test2_48(%979 s1: @type48, %980 s2: @type48) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type48, @type48, @type48) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%974, copy<@type48, reason=arg>(read<@type48>(%979)), copy<@type48, reason=arg>(read<@type48>(%964)), copy<@type48, reason=arg>(read<@type48>(%980)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %981 @testit48() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type48>, i32) -> void>(%966, addr_of<ptr<@type48>>(%963), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type48>, i32) -> void>(%970, addr_of<ptr<@type48>>(%963), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type48>, i32) -> void>(%966, addr_of<ptr<@type48>>(%964), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type48>, i32) -> void>(%970, addr_of<ptr<@type48>>(%964), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type48>, i32) -> void>(%966, addr_of<ptr<@type48>>(%965), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type48>, i32) -> void>(%970, addr_of<ptr<@type48>>(%965), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type48, @type48, @type48) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%974, copy<@type48, reason=arg>(read<@type48>(%963)), copy<@type48, reason=arg>(read<@type48>(%964)), copy<@type48, reason=arg>(read<@type48>(%965)));
// DEFAULT-NEXT:         call<void, signature=fn(@type48, @type48) -> void, abi=sysv64(native_c, native_c) -> void>(%978, copy<@type48, reason=arg>(read<@type48>(%963)), copy<@type48, reason=arg>(read<@type48>(%965)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %986 @init49(%987 p: ptr<@type49>, %988 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %989 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1382
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%989, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%989), const<i32>(49))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1608: i32 [synthetic] = read<i32>(%989);
// DEFAULT-NEXT:                 let %1609: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1608), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%989, read<i32>(%1609));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(49)>(field0(deref(read<ptr<@type49>>(%987)))), read<i32>(%989))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%988), read<i32>(%989)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %990 @check49(%991 p: ptr<@type49>, %992 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %993 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1383
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%993, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%993), const<i32>(49))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1610: i32 [synthetic] = read<i32>(%993);
// DEFAULT-NEXT:                 let %1611: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1610), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%993, read<i32>(%1611));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(49)>(field0(deref(read<ptr<@type49>>(%991)))), read<i32>(%993)))))), add<i32, overflow=ub>(read<i32>(%992), read<i32>(%993)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %994 @test49(%995 s1: @type49, %996 s2: @type49, %997 s3: @type49) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type49>, i32) -> void>(%990, addr_of<ptr<@type49>>(%995), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type49>, i32) -> void>(%990, addr_of<ptr<@type49>>(%996), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type49>, i32) -> void>(%990, addr_of<ptr<@type49>>(%997), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %998 @test2_49(%999 s1: @type49, %1000 s2: @type49) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type49, @type49, @type49) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%994, copy<@type49, reason=arg>(read<@type49>(%999)), copy<@type49, reason=arg>(read<@type49>(%984)), copy<@type49, reason=arg>(read<@type49>(%1000)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1001 @testit49() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type49>, i32) -> void>(%986, addr_of<ptr<@type49>>(%983), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type49>, i32) -> void>(%990, addr_of<ptr<@type49>>(%983), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type49>, i32) -> void>(%986, addr_of<ptr<@type49>>(%984), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type49>, i32) -> void>(%990, addr_of<ptr<@type49>>(%984), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type49>, i32) -> void>(%986, addr_of<ptr<@type49>>(%985), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type49>, i32) -> void>(%990, addr_of<ptr<@type49>>(%985), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type49, @type49, @type49) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%994, copy<@type49, reason=arg>(read<@type49>(%983)), copy<@type49, reason=arg>(read<@type49>(%984)), copy<@type49, reason=arg>(read<@type49>(%985)));
// DEFAULT-NEXT:         call<void, signature=fn(@type49, @type49) -> void, abi=sysv64(native_c, native_c) -> void>(%998, copy<@type49, reason=arg>(read<@type49>(%983)), copy<@type49, reason=arg>(read<@type49>(%985)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1006 @init50(%1007 p: ptr<@type50>, %1008 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1009 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1384
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1009, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1009), const<i32>(50))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1612: i32 [synthetic] = read<i32>(%1009);
// DEFAULT-NEXT:                 let %1613: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1612), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1009, read<i32>(%1613));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(50)>(field0(deref(read<ptr<@type50>>(%1007)))), read<i32>(%1009))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%1008), read<i32>(%1009)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1010 @check50(%1011 p: ptr<@type50>, %1012 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1013 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1385
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1013, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1013), const<i32>(50))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1614: i32 [synthetic] = read<i32>(%1013);
// DEFAULT-NEXT:                 let %1615: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1614), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1013, read<i32>(%1615));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(50)>(field0(deref(read<ptr<@type50>>(%1011)))), read<i32>(%1013)))))), add<i32, overflow=ub>(read<i32>(%1012), read<i32>(%1013)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1014 @test50(%1015 s1: @type50, %1016 s2: @type50, %1017 s3: @type50) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type50>, i32) -> void>(%1010, addr_of<ptr<@type50>>(%1015), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type50>, i32) -> void>(%1010, addr_of<ptr<@type50>>(%1016), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type50>, i32) -> void>(%1010, addr_of<ptr<@type50>>(%1017), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1018 @test2_50(%1019 s1: @type50, %1020 s2: @type50) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type50, @type50, @type50) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1014, copy<@type50, reason=arg>(read<@type50>(%1019)), copy<@type50, reason=arg>(read<@type50>(%1004)), copy<@type50, reason=arg>(read<@type50>(%1020)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1021 @testit50() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type50>, i32) -> void>(%1006, addr_of<ptr<@type50>>(%1003), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type50>, i32) -> void>(%1010, addr_of<ptr<@type50>>(%1003), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type50>, i32) -> void>(%1006, addr_of<ptr<@type50>>(%1004), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type50>, i32) -> void>(%1010, addr_of<ptr<@type50>>(%1004), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type50>, i32) -> void>(%1006, addr_of<ptr<@type50>>(%1005), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type50>, i32) -> void>(%1010, addr_of<ptr<@type50>>(%1005), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type50, @type50, @type50) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1014, copy<@type50, reason=arg>(read<@type50>(%1003)), copy<@type50, reason=arg>(read<@type50>(%1004)), copy<@type50, reason=arg>(read<@type50>(%1005)));
// DEFAULT-NEXT:         call<void, signature=fn(@type50, @type50) -> void, abi=sysv64(native_c, native_c) -> void>(%1018, copy<@type50, reason=arg>(read<@type50>(%1003)), copy<@type50, reason=arg>(read<@type50>(%1005)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1026 @init51(%1027 p: ptr<@type51>, %1028 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1029 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1386
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1029, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1029), const<i32>(51))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1616: i32 [synthetic] = read<i32>(%1029);
// DEFAULT-NEXT:                 let %1617: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1616), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1029, read<i32>(%1617));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(51)>(field0(deref(read<ptr<@type51>>(%1027)))), read<i32>(%1029))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%1028), read<i32>(%1029)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1030 @check51(%1031 p: ptr<@type51>, %1032 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1033 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1387
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1033, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1033), const<i32>(51))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1618: i32 [synthetic] = read<i32>(%1033);
// DEFAULT-NEXT:                 let %1619: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1618), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1033, read<i32>(%1619));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(51)>(field0(deref(read<ptr<@type51>>(%1031)))), read<i32>(%1033)))))), add<i32, overflow=ub>(read<i32>(%1032), read<i32>(%1033)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1034 @test51(%1035 s1: @type51, %1036 s2: @type51, %1037 s3: @type51) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type51>, i32) -> void>(%1030, addr_of<ptr<@type51>>(%1035), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type51>, i32) -> void>(%1030, addr_of<ptr<@type51>>(%1036), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type51>, i32) -> void>(%1030, addr_of<ptr<@type51>>(%1037), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1038 @test2_51(%1039 s1: @type51, %1040 s2: @type51) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type51, @type51, @type51) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1034, copy<@type51, reason=arg>(read<@type51>(%1039)), copy<@type51, reason=arg>(read<@type51>(%1024)), copy<@type51, reason=arg>(read<@type51>(%1040)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1041 @testit51() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type51>, i32) -> void>(%1026, addr_of<ptr<@type51>>(%1023), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type51>, i32) -> void>(%1030, addr_of<ptr<@type51>>(%1023), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type51>, i32) -> void>(%1026, addr_of<ptr<@type51>>(%1024), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type51>, i32) -> void>(%1030, addr_of<ptr<@type51>>(%1024), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type51>, i32) -> void>(%1026, addr_of<ptr<@type51>>(%1025), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type51>, i32) -> void>(%1030, addr_of<ptr<@type51>>(%1025), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type51, @type51, @type51) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1034, copy<@type51, reason=arg>(read<@type51>(%1023)), copy<@type51, reason=arg>(read<@type51>(%1024)), copy<@type51, reason=arg>(read<@type51>(%1025)));
// DEFAULT-NEXT:         call<void, signature=fn(@type51, @type51) -> void, abi=sysv64(native_c, native_c) -> void>(%1038, copy<@type51, reason=arg>(read<@type51>(%1023)), copy<@type51, reason=arg>(read<@type51>(%1025)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1046 @init52(%1047 p: ptr<@type52>, %1048 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1049 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1388
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1049, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1049), const<i32>(52))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1620: i32 [synthetic] = read<i32>(%1049);
// DEFAULT-NEXT:                 let %1621: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1620), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1049, read<i32>(%1621));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(52)>(field0(deref(read<ptr<@type52>>(%1047)))), read<i32>(%1049))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%1048), read<i32>(%1049)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1050 @check52(%1051 p: ptr<@type52>, %1052 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1053 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1389
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1053, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1053), const<i32>(52))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1622: i32 [synthetic] = read<i32>(%1053);
// DEFAULT-NEXT:                 let %1623: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1622), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1053, read<i32>(%1623));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(52)>(field0(deref(read<ptr<@type52>>(%1051)))), read<i32>(%1053)))))), add<i32, overflow=ub>(read<i32>(%1052), read<i32>(%1053)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1054 @test52(%1055 s1: @type52, %1056 s2: @type52, %1057 s3: @type52) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type52>, i32) -> void>(%1050, addr_of<ptr<@type52>>(%1055), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type52>, i32) -> void>(%1050, addr_of<ptr<@type52>>(%1056), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type52>, i32) -> void>(%1050, addr_of<ptr<@type52>>(%1057), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1058 @test2_52(%1059 s1: @type52, %1060 s2: @type52) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type52, @type52, @type52) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1054, copy<@type52, reason=arg>(read<@type52>(%1059)), copy<@type52, reason=arg>(read<@type52>(%1044)), copy<@type52, reason=arg>(read<@type52>(%1060)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1061 @testit52() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type52>, i32) -> void>(%1046, addr_of<ptr<@type52>>(%1043), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type52>, i32) -> void>(%1050, addr_of<ptr<@type52>>(%1043), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type52>, i32) -> void>(%1046, addr_of<ptr<@type52>>(%1044), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type52>, i32) -> void>(%1050, addr_of<ptr<@type52>>(%1044), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type52>, i32) -> void>(%1046, addr_of<ptr<@type52>>(%1045), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type52>, i32) -> void>(%1050, addr_of<ptr<@type52>>(%1045), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type52, @type52, @type52) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1054, copy<@type52, reason=arg>(read<@type52>(%1043)), copy<@type52, reason=arg>(read<@type52>(%1044)), copy<@type52, reason=arg>(read<@type52>(%1045)));
// DEFAULT-NEXT:         call<void, signature=fn(@type52, @type52) -> void, abi=sysv64(native_c, native_c) -> void>(%1058, copy<@type52, reason=arg>(read<@type52>(%1043)), copy<@type52, reason=arg>(read<@type52>(%1045)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1066 @init53(%1067 p: ptr<@type53>, %1068 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1069 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1390
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1069, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1069), const<i32>(53))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1624: i32 [synthetic] = read<i32>(%1069);
// DEFAULT-NEXT:                 let %1625: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1624), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1069, read<i32>(%1625));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(53)>(field0(deref(read<ptr<@type53>>(%1067)))), read<i32>(%1069))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%1068), read<i32>(%1069)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1070 @check53(%1071 p: ptr<@type53>, %1072 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1073 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1391
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1073, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1073), const<i32>(53))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1626: i32 [synthetic] = read<i32>(%1073);
// DEFAULT-NEXT:                 let %1627: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1626), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1073, read<i32>(%1627));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(53)>(field0(deref(read<ptr<@type53>>(%1071)))), read<i32>(%1073)))))), add<i32, overflow=ub>(read<i32>(%1072), read<i32>(%1073)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1074 @test53(%1075 s1: @type53, %1076 s2: @type53, %1077 s3: @type53) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type53>, i32) -> void>(%1070, addr_of<ptr<@type53>>(%1075), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type53>, i32) -> void>(%1070, addr_of<ptr<@type53>>(%1076), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type53>, i32) -> void>(%1070, addr_of<ptr<@type53>>(%1077), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1078 @test2_53(%1079 s1: @type53, %1080 s2: @type53) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type53, @type53, @type53) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1074, copy<@type53, reason=arg>(read<@type53>(%1079)), copy<@type53, reason=arg>(read<@type53>(%1064)), copy<@type53, reason=arg>(read<@type53>(%1080)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1081 @testit53() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type53>, i32) -> void>(%1066, addr_of<ptr<@type53>>(%1063), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type53>, i32) -> void>(%1070, addr_of<ptr<@type53>>(%1063), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type53>, i32) -> void>(%1066, addr_of<ptr<@type53>>(%1064), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type53>, i32) -> void>(%1070, addr_of<ptr<@type53>>(%1064), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type53>, i32) -> void>(%1066, addr_of<ptr<@type53>>(%1065), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type53>, i32) -> void>(%1070, addr_of<ptr<@type53>>(%1065), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type53, @type53, @type53) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1074, copy<@type53, reason=arg>(read<@type53>(%1063)), copy<@type53, reason=arg>(read<@type53>(%1064)), copy<@type53, reason=arg>(read<@type53>(%1065)));
// DEFAULT-NEXT:         call<void, signature=fn(@type53, @type53) -> void, abi=sysv64(native_c, native_c) -> void>(%1078, copy<@type53, reason=arg>(read<@type53>(%1063)), copy<@type53, reason=arg>(read<@type53>(%1065)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1086 @init54(%1087 p: ptr<@type54>, %1088 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1089 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1392
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1089, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1089), const<i32>(54))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1628: i32 [synthetic] = read<i32>(%1089);
// DEFAULT-NEXT:                 let %1629: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1628), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1089, read<i32>(%1629));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(54)>(field0(deref(read<ptr<@type54>>(%1087)))), read<i32>(%1089))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%1088), read<i32>(%1089)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1090 @check54(%1091 p: ptr<@type54>, %1092 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1093 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1393
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1093, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1093), const<i32>(54))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1630: i32 [synthetic] = read<i32>(%1093);
// DEFAULT-NEXT:                 let %1631: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1630), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1093, read<i32>(%1631));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(54)>(field0(deref(read<ptr<@type54>>(%1091)))), read<i32>(%1093)))))), add<i32, overflow=ub>(read<i32>(%1092), read<i32>(%1093)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1094 @test54(%1095 s1: @type54, %1096 s2: @type54, %1097 s3: @type54) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type54>, i32) -> void>(%1090, addr_of<ptr<@type54>>(%1095), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type54>, i32) -> void>(%1090, addr_of<ptr<@type54>>(%1096), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type54>, i32) -> void>(%1090, addr_of<ptr<@type54>>(%1097), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1098 @test2_54(%1099 s1: @type54, %1100 s2: @type54) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type54, @type54, @type54) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1094, copy<@type54, reason=arg>(read<@type54>(%1099)), copy<@type54, reason=arg>(read<@type54>(%1084)), copy<@type54, reason=arg>(read<@type54>(%1100)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1101 @testit54() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type54>, i32) -> void>(%1086, addr_of<ptr<@type54>>(%1083), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type54>, i32) -> void>(%1090, addr_of<ptr<@type54>>(%1083), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type54>, i32) -> void>(%1086, addr_of<ptr<@type54>>(%1084), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type54>, i32) -> void>(%1090, addr_of<ptr<@type54>>(%1084), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type54>, i32) -> void>(%1086, addr_of<ptr<@type54>>(%1085), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type54>, i32) -> void>(%1090, addr_of<ptr<@type54>>(%1085), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type54, @type54, @type54) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1094, copy<@type54, reason=arg>(read<@type54>(%1083)), copy<@type54, reason=arg>(read<@type54>(%1084)), copy<@type54, reason=arg>(read<@type54>(%1085)));
// DEFAULT-NEXT:         call<void, signature=fn(@type54, @type54) -> void, abi=sysv64(native_c, native_c) -> void>(%1098, copy<@type54, reason=arg>(read<@type54>(%1083)), copy<@type54, reason=arg>(read<@type54>(%1085)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1106 @init55(%1107 p: ptr<@type55>, %1108 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1109 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1394
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1109, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1109), const<i32>(55))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1632: i32 [synthetic] = read<i32>(%1109);
// DEFAULT-NEXT:                 let %1633: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1632), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1109, read<i32>(%1633));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(55)>(field0(deref(read<ptr<@type55>>(%1107)))), read<i32>(%1109))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%1108), read<i32>(%1109)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1110 @check55(%1111 p: ptr<@type55>, %1112 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1113 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1395
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1113, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1113), const<i32>(55))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1634: i32 [synthetic] = read<i32>(%1113);
// DEFAULT-NEXT:                 let %1635: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1634), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1113, read<i32>(%1635));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(55)>(field0(deref(read<ptr<@type55>>(%1111)))), read<i32>(%1113)))))), add<i32, overflow=ub>(read<i32>(%1112), read<i32>(%1113)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1114 @test55(%1115 s1: @type55, %1116 s2: @type55, %1117 s3: @type55) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type55>, i32) -> void>(%1110, addr_of<ptr<@type55>>(%1115), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type55>, i32) -> void>(%1110, addr_of<ptr<@type55>>(%1116), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type55>, i32) -> void>(%1110, addr_of<ptr<@type55>>(%1117), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1118 @test2_55(%1119 s1: @type55, %1120 s2: @type55) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type55, @type55, @type55) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1114, copy<@type55, reason=arg>(read<@type55>(%1119)), copy<@type55, reason=arg>(read<@type55>(%1104)), copy<@type55, reason=arg>(read<@type55>(%1120)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1121 @testit55() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type55>, i32) -> void>(%1106, addr_of<ptr<@type55>>(%1103), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type55>, i32) -> void>(%1110, addr_of<ptr<@type55>>(%1103), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type55>, i32) -> void>(%1106, addr_of<ptr<@type55>>(%1104), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type55>, i32) -> void>(%1110, addr_of<ptr<@type55>>(%1104), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type55>, i32) -> void>(%1106, addr_of<ptr<@type55>>(%1105), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type55>, i32) -> void>(%1110, addr_of<ptr<@type55>>(%1105), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type55, @type55, @type55) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1114, copy<@type55, reason=arg>(read<@type55>(%1103)), copy<@type55, reason=arg>(read<@type55>(%1104)), copy<@type55, reason=arg>(read<@type55>(%1105)));
// DEFAULT-NEXT:         call<void, signature=fn(@type55, @type55) -> void, abi=sysv64(native_c, native_c) -> void>(%1118, copy<@type55, reason=arg>(read<@type55>(%1103)), copy<@type55, reason=arg>(read<@type55>(%1105)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1126 @init56(%1127 p: ptr<@type56>, %1128 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1129 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1396
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1129, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1129), const<i32>(56))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1636: i32 [synthetic] = read<i32>(%1129);
// DEFAULT-NEXT:                 let %1637: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1636), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1129, read<i32>(%1637));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(56)>(field0(deref(read<ptr<@type56>>(%1127)))), read<i32>(%1129))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%1128), read<i32>(%1129)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1130 @check56(%1131 p: ptr<@type56>, %1132 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1133 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1397
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1133, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1133), const<i32>(56))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1638: i32 [synthetic] = read<i32>(%1133);
// DEFAULT-NEXT:                 let %1639: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1638), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1133, read<i32>(%1639));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(56)>(field0(deref(read<ptr<@type56>>(%1131)))), read<i32>(%1133)))))), add<i32, overflow=ub>(read<i32>(%1132), read<i32>(%1133)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1134 @test56(%1135 s1: @type56, %1136 s2: @type56, %1137 s3: @type56) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type56>, i32) -> void>(%1130, addr_of<ptr<@type56>>(%1135), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type56>, i32) -> void>(%1130, addr_of<ptr<@type56>>(%1136), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type56>, i32) -> void>(%1130, addr_of<ptr<@type56>>(%1137), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1138 @test2_56(%1139 s1: @type56, %1140 s2: @type56) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type56, @type56, @type56) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1134, copy<@type56, reason=arg>(read<@type56>(%1139)), copy<@type56, reason=arg>(read<@type56>(%1124)), copy<@type56, reason=arg>(read<@type56>(%1140)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1141 @testit56() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type56>, i32) -> void>(%1126, addr_of<ptr<@type56>>(%1123), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type56>, i32) -> void>(%1130, addr_of<ptr<@type56>>(%1123), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type56>, i32) -> void>(%1126, addr_of<ptr<@type56>>(%1124), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type56>, i32) -> void>(%1130, addr_of<ptr<@type56>>(%1124), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type56>, i32) -> void>(%1126, addr_of<ptr<@type56>>(%1125), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type56>, i32) -> void>(%1130, addr_of<ptr<@type56>>(%1125), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type56, @type56, @type56) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1134, copy<@type56, reason=arg>(read<@type56>(%1123)), copy<@type56, reason=arg>(read<@type56>(%1124)), copy<@type56, reason=arg>(read<@type56>(%1125)));
// DEFAULT-NEXT:         call<void, signature=fn(@type56, @type56) -> void, abi=sysv64(native_c, native_c) -> void>(%1138, copy<@type56, reason=arg>(read<@type56>(%1123)), copy<@type56, reason=arg>(read<@type56>(%1125)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1146 @init57(%1147 p: ptr<@type57>, %1148 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1149 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1398
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1149, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1149), const<i32>(57))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1640: i32 [synthetic] = read<i32>(%1149);
// DEFAULT-NEXT:                 let %1641: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1640), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1149, read<i32>(%1641));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(57)>(field0(deref(read<ptr<@type57>>(%1147)))), read<i32>(%1149))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%1148), read<i32>(%1149)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1150 @check57(%1151 p: ptr<@type57>, %1152 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1153 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1399
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1153, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1153), const<i32>(57))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1642: i32 [synthetic] = read<i32>(%1153);
// DEFAULT-NEXT:                 let %1643: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1642), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1153, read<i32>(%1643));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(57)>(field0(deref(read<ptr<@type57>>(%1151)))), read<i32>(%1153)))))), add<i32, overflow=ub>(read<i32>(%1152), read<i32>(%1153)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1154 @test57(%1155 s1: @type57, %1156 s2: @type57, %1157 s3: @type57) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type57>, i32) -> void>(%1150, addr_of<ptr<@type57>>(%1155), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type57>, i32) -> void>(%1150, addr_of<ptr<@type57>>(%1156), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type57>, i32) -> void>(%1150, addr_of<ptr<@type57>>(%1157), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1158 @test2_57(%1159 s1: @type57, %1160 s2: @type57) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type57, @type57, @type57) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1154, copy<@type57, reason=arg>(read<@type57>(%1159)), copy<@type57, reason=arg>(read<@type57>(%1144)), copy<@type57, reason=arg>(read<@type57>(%1160)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1161 @testit57() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type57>, i32) -> void>(%1146, addr_of<ptr<@type57>>(%1143), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type57>, i32) -> void>(%1150, addr_of<ptr<@type57>>(%1143), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type57>, i32) -> void>(%1146, addr_of<ptr<@type57>>(%1144), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type57>, i32) -> void>(%1150, addr_of<ptr<@type57>>(%1144), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type57>, i32) -> void>(%1146, addr_of<ptr<@type57>>(%1145), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type57>, i32) -> void>(%1150, addr_of<ptr<@type57>>(%1145), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type57, @type57, @type57) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1154, copy<@type57, reason=arg>(read<@type57>(%1143)), copy<@type57, reason=arg>(read<@type57>(%1144)), copy<@type57, reason=arg>(read<@type57>(%1145)));
// DEFAULT-NEXT:         call<void, signature=fn(@type57, @type57) -> void, abi=sysv64(native_c, native_c) -> void>(%1158, copy<@type57, reason=arg>(read<@type57>(%1143)), copy<@type57, reason=arg>(read<@type57>(%1145)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1166 @init58(%1167 p: ptr<@type58>, %1168 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1169 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1400
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1169, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1169), const<i32>(58))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1644: i32 [synthetic] = read<i32>(%1169);
// DEFAULT-NEXT:                 let %1645: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1644), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1169, read<i32>(%1645));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(58)>(field0(deref(read<ptr<@type58>>(%1167)))), read<i32>(%1169))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%1168), read<i32>(%1169)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1170 @check58(%1171 p: ptr<@type58>, %1172 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1173 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1401
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1173, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1173), const<i32>(58))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1646: i32 [synthetic] = read<i32>(%1173);
// DEFAULT-NEXT:                 let %1647: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1646), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1173, read<i32>(%1647));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(58)>(field0(deref(read<ptr<@type58>>(%1171)))), read<i32>(%1173)))))), add<i32, overflow=ub>(read<i32>(%1172), read<i32>(%1173)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1174 @test58(%1175 s1: @type58, %1176 s2: @type58, %1177 s3: @type58) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type58>, i32) -> void>(%1170, addr_of<ptr<@type58>>(%1175), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type58>, i32) -> void>(%1170, addr_of<ptr<@type58>>(%1176), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type58>, i32) -> void>(%1170, addr_of<ptr<@type58>>(%1177), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1178 @test2_58(%1179 s1: @type58, %1180 s2: @type58) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type58, @type58, @type58) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1174, copy<@type58, reason=arg>(read<@type58>(%1179)), copy<@type58, reason=arg>(read<@type58>(%1164)), copy<@type58, reason=arg>(read<@type58>(%1180)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1181 @testit58() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type58>, i32) -> void>(%1166, addr_of<ptr<@type58>>(%1163), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type58>, i32) -> void>(%1170, addr_of<ptr<@type58>>(%1163), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type58>, i32) -> void>(%1166, addr_of<ptr<@type58>>(%1164), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type58>, i32) -> void>(%1170, addr_of<ptr<@type58>>(%1164), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type58>, i32) -> void>(%1166, addr_of<ptr<@type58>>(%1165), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type58>, i32) -> void>(%1170, addr_of<ptr<@type58>>(%1165), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type58, @type58, @type58) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1174, copy<@type58, reason=arg>(read<@type58>(%1163)), copy<@type58, reason=arg>(read<@type58>(%1164)), copy<@type58, reason=arg>(read<@type58>(%1165)));
// DEFAULT-NEXT:         call<void, signature=fn(@type58, @type58) -> void, abi=sysv64(native_c, native_c) -> void>(%1178, copy<@type58, reason=arg>(read<@type58>(%1163)), copy<@type58, reason=arg>(read<@type58>(%1165)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1186 @init59(%1187 p: ptr<@type59>, %1188 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1189 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1402
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1189, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1189), const<i32>(59))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1648: i32 [synthetic] = read<i32>(%1189);
// DEFAULT-NEXT:                 let %1649: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1648), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1189, read<i32>(%1649));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(59)>(field0(deref(read<ptr<@type59>>(%1187)))), read<i32>(%1189))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%1188), read<i32>(%1189)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1190 @check59(%1191 p: ptr<@type59>, %1192 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1193 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1403
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1193, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1193), const<i32>(59))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1650: i32 [synthetic] = read<i32>(%1193);
// DEFAULT-NEXT:                 let %1651: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1650), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1193, read<i32>(%1651));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(59)>(field0(deref(read<ptr<@type59>>(%1191)))), read<i32>(%1193)))))), add<i32, overflow=ub>(read<i32>(%1192), read<i32>(%1193)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1194 @test59(%1195 s1: @type59, %1196 s2: @type59, %1197 s3: @type59) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type59>, i32) -> void>(%1190, addr_of<ptr<@type59>>(%1195), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type59>, i32) -> void>(%1190, addr_of<ptr<@type59>>(%1196), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type59>, i32) -> void>(%1190, addr_of<ptr<@type59>>(%1197), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1198 @test2_59(%1199 s1: @type59, %1200 s2: @type59) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type59, @type59, @type59) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1194, copy<@type59, reason=arg>(read<@type59>(%1199)), copy<@type59, reason=arg>(read<@type59>(%1184)), copy<@type59, reason=arg>(read<@type59>(%1200)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1201 @testit59() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type59>, i32) -> void>(%1186, addr_of<ptr<@type59>>(%1183), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type59>, i32) -> void>(%1190, addr_of<ptr<@type59>>(%1183), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type59>, i32) -> void>(%1186, addr_of<ptr<@type59>>(%1184), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type59>, i32) -> void>(%1190, addr_of<ptr<@type59>>(%1184), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type59>, i32) -> void>(%1186, addr_of<ptr<@type59>>(%1185), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type59>, i32) -> void>(%1190, addr_of<ptr<@type59>>(%1185), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type59, @type59, @type59) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1194, copy<@type59, reason=arg>(read<@type59>(%1183)), copy<@type59, reason=arg>(read<@type59>(%1184)), copy<@type59, reason=arg>(read<@type59>(%1185)));
// DEFAULT-NEXT:         call<void, signature=fn(@type59, @type59) -> void, abi=sysv64(native_c, native_c) -> void>(%1198, copy<@type59, reason=arg>(read<@type59>(%1183)), copy<@type59, reason=arg>(read<@type59>(%1185)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1206 @init60(%1207 p: ptr<@type60>, %1208 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1209 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1404
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1209, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1209), const<i32>(60))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1652: i32 [synthetic] = read<i32>(%1209);
// DEFAULT-NEXT:                 let %1653: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1652), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1209, read<i32>(%1653));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(60)>(field0(deref(read<ptr<@type60>>(%1207)))), read<i32>(%1209))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%1208), read<i32>(%1209)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1210 @check60(%1211 p: ptr<@type60>, %1212 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1213 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1405
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1213, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1213), const<i32>(60))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1654: i32 [synthetic] = read<i32>(%1213);
// DEFAULT-NEXT:                 let %1655: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1654), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1213, read<i32>(%1655));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(60)>(field0(deref(read<ptr<@type60>>(%1211)))), read<i32>(%1213)))))), add<i32, overflow=ub>(read<i32>(%1212), read<i32>(%1213)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1214 @test60(%1215 s1: @type60, %1216 s2: @type60, %1217 s3: @type60) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type60>, i32) -> void>(%1210, addr_of<ptr<@type60>>(%1215), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type60>, i32) -> void>(%1210, addr_of<ptr<@type60>>(%1216), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type60>, i32) -> void>(%1210, addr_of<ptr<@type60>>(%1217), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1218 @test2_60(%1219 s1: @type60, %1220 s2: @type60) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type60, @type60, @type60) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1214, copy<@type60, reason=arg>(read<@type60>(%1219)), copy<@type60, reason=arg>(read<@type60>(%1204)), copy<@type60, reason=arg>(read<@type60>(%1220)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1221 @testit60() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type60>, i32) -> void>(%1206, addr_of<ptr<@type60>>(%1203), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type60>, i32) -> void>(%1210, addr_of<ptr<@type60>>(%1203), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type60>, i32) -> void>(%1206, addr_of<ptr<@type60>>(%1204), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type60>, i32) -> void>(%1210, addr_of<ptr<@type60>>(%1204), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type60>, i32) -> void>(%1206, addr_of<ptr<@type60>>(%1205), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type60>, i32) -> void>(%1210, addr_of<ptr<@type60>>(%1205), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type60, @type60, @type60) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1214, copy<@type60, reason=arg>(read<@type60>(%1203)), copy<@type60, reason=arg>(read<@type60>(%1204)), copy<@type60, reason=arg>(read<@type60>(%1205)));
// DEFAULT-NEXT:         call<void, signature=fn(@type60, @type60) -> void, abi=sysv64(native_c, native_c) -> void>(%1218, copy<@type60, reason=arg>(read<@type60>(%1203)), copy<@type60, reason=arg>(read<@type60>(%1205)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1226 @init61(%1227 p: ptr<@type61>, %1228 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1229 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1406
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1229, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1229), const<i32>(61))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1656: i32 [synthetic] = read<i32>(%1229);
// DEFAULT-NEXT:                 let %1657: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1656), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1229, read<i32>(%1657));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(61)>(field0(deref(read<ptr<@type61>>(%1227)))), read<i32>(%1229))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%1228), read<i32>(%1229)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1230 @check61(%1231 p: ptr<@type61>, %1232 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1233 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1407
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1233, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1233), const<i32>(61))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1658: i32 [synthetic] = read<i32>(%1233);
// DEFAULT-NEXT:                 let %1659: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1658), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1233, read<i32>(%1659));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(61)>(field0(deref(read<ptr<@type61>>(%1231)))), read<i32>(%1233)))))), add<i32, overflow=ub>(read<i32>(%1232), read<i32>(%1233)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1234 @test61(%1235 s1: @type61, %1236 s2: @type61, %1237 s3: @type61) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type61>, i32) -> void>(%1230, addr_of<ptr<@type61>>(%1235), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type61>, i32) -> void>(%1230, addr_of<ptr<@type61>>(%1236), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type61>, i32) -> void>(%1230, addr_of<ptr<@type61>>(%1237), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1238 @test2_61(%1239 s1: @type61, %1240 s2: @type61) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type61, @type61, @type61) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1234, copy<@type61, reason=arg>(read<@type61>(%1239)), copy<@type61, reason=arg>(read<@type61>(%1224)), copy<@type61, reason=arg>(read<@type61>(%1240)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1241 @testit61() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type61>, i32) -> void>(%1226, addr_of<ptr<@type61>>(%1223), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type61>, i32) -> void>(%1230, addr_of<ptr<@type61>>(%1223), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type61>, i32) -> void>(%1226, addr_of<ptr<@type61>>(%1224), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type61>, i32) -> void>(%1230, addr_of<ptr<@type61>>(%1224), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type61>, i32) -> void>(%1226, addr_of<ptr<@type61>>(%1225), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type61>, i32) -> void>(%1230, addr_of<ptr<@type61>>(%1225), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type61, @type61, @type61) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1234, copy<@type61, reason=arg>(read<@type61>(%1223)), copy<@type61, reason=arg>(read<@type61>(%1224)), copy<@type61, reason=arg>(read<@type61>(%1225)));
// DEFAULT-NEXT:         call<void, signature=fn(@type61, @type61) -> void, abi=sysv64(native_c, native_c) -> void>(%1238, copy<@type61, reason=arg>(read<@type61>(%1223)), copy<@type61, reason=arg>(read<@type61>(%1225)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1246 @init62(%1247 p: ptr<@type62>, %1248 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1249 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1408
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1249, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1249), const<i32>(62))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1660: i32 [synthetic] = read<i32>(%1249);
// DEFAULT-NEXT:                 let %1661: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1660), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1249, read<i32>(%1661));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(62)>(field0(deref(read<ptr<@type62>>(%1247)))), read<i32>(%1249))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%1248), read<i32>(%1249)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1250 @check62(%1251 p: ptr<@type62>, %1252 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1253 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1409
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1253, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1253), const<i32>(62))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1662: i32 [synthetic] = read<i32>(%1253);
// DEFAULT-NEXT:                 let %1663: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1662), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1253, read<i32>(%1663));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(62)>(field0(deref(read<ptr<@type62>>(%1251)))), read<i32>(%1253)))))), add<i32, overflow=ub>(read<i32>(%1252), read<i32>(%1253)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1254 @test62(%1255 s1: @type62, %1256 s2: @type62, %1257 s3: @type62) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type62>, i32) -> void>(%1250, addr_of<ptr<@type62>>(%1255), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type62>, i32) -> void>(%1250, addr_of<ptr<@type62>>(%1256), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type62>, i32) -> void>(%1250, addr_of<ptr<@type62>>(%1257), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1258 @test2_62(%1259 s1: @type62, %1260 s2: @type62) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type62, @type62, @type62) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1254, copy<@type62, reason=arg>(read<@type62>(%1259)), copy<@type62, reason=arg>(read<@type62>(%1244)), copy<@type62, reason=arg>(read<@type62>(%1260)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1261 @testit62() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type62>, i32) -> void>(%1246, addr_of<ptr<@type62>>(%1243), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type62>, i32) -> void>(%1250, addr_of<ptr<@type62>>(%1243), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type62>, i32) -> void>(%1246, addr_of<ptr<@type62>>(%1244), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type62>, i32) -> void>(%1250, addr_of<ptr<@type62>>(%1244), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type62>, i32) -> void>(%1246, addr_of<ptr<@type62>>(%1245), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type62>, i32) -> void>(%1250, addr_of<ptr<@type62>>(%1245), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type62, @type62, @type62) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1254, copy<@type62, reason=arg>(read<@type62>(%1243)), copy<@type62, reason=arg>(read<@type62>(%1244)), copy<@type62, reason=arg>(read<@type62>(%1245)));
// DEFAULT-NEXT:         call<void, signature=fn(@type62, @type62) -> void, abi=sysv64(native_c, native_c) -> void>(%1258, copy<@type62, reason=arg>(read<@type62>(%1243)), copy<@type62, reason=arg>(read<@type62>(%1245)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1266 @init63(%1267 p: ptr<@type63>, %1268 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1269 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1410
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1269, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1269), const<i32>(63))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1664: i32 [synthetic] = read<i32>(%1269);
// DEFAULT-NEXT:                 let %1665: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1664), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1269, read<i32>(%1665));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(63)>(field0(deref(read<ptr<@type63>>(%1267)))), read<i32>(%1269))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%1268), read<i32>(%1269)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1270 @check63(%1271 p: ptr<@type63>, %1272 i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %1273 j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %1411
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%1273, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1273), const<i32>(63))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %1666: i32 [synthetic] = read<i32>(%1273);
// DEFAULT-NEXT:                 let %1667: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%1666), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1273, read<i32>(%1667));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(63)>(field0(deref(read<ptr<@type63>>(%1271)))), read<i32>(%1273)))))), add<i32, overflow=ub>(read<i32>(%1272), read<i32>(%1273)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1274 @test63(%1275 s1: @type63, %1276 s2: @type63, %1277 s3: @type63) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type63>, i32) -> void>(%1270, addr_of<ptr<@type63>>(%1275), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type63>, i32) -> void>(%1270, addr_of<ptr<@type63>>(%1276), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type63>, i32) -> void>(%1270, addr_of<ptr<@type63>>(%1277), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1278 @test2_63(%1279 s1: @type63, %1280 s2: @type63) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type63, @type63, @type63) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1274, copy<@type63, reason=arg>(read<@type63>(%1279)), copy<@type63, reason=arg>(read<@type63>(%1264)), copy<@type63, reason=arg>(read<@type63>(%1280)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1281 @testit63() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type63>, i32) -> void>(%1266, addr_of<ptr<@type63>>(%1263), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type63>, i32) -> void>(%1270, addr_of<ptr<@type63>>(%1263), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type63>, i32) -> void>(%1266, addr_of<ptr<@type63>>(%1264), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type63>, i32) -> void>(%1270, addr_of<ptr<@type63>>(%1264), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type63>, i32) -> void>(%1266, addr_of<ptr<@type63>>(%1265), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type63>, i32) -> void>(%1270, addr_of<ptr<@type63>>(%1265), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type63, @type63, @type63) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%1274, copy<@type63, reason=arg>(read<@type63>(%1263)), copy<@type63, reason=arg>(read<@type63>(%1264)), copy<@type63, reason=arg>(read<@type63>(%1265)));
// DEFAULT-NEXT:         call<void, signature=fn(@type63, @type63) -> void, abi=sysv64(native_c, native_c) -> void>(%1278, copy<@type63, reason=arg>(read<@type63>(%1263)), copy<@type63, reason=arg>(read<@type63>(%1265)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %1282 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%21);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%41);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%61);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%81);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%101);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%121);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%141);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%161);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%181);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%201);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%221);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%241);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%261);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%281);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%301);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%321);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%341);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%361);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%381);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%401);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%421);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%441);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%461);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%481);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%501);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%521);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%541);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%561);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%581);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%601);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%621);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%641);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%661);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%681);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%701);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%721);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%741);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%761);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%781);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%801);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%821);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%841);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%861);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%881);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%901);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%921);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%941);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%961);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%981);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1001);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1021);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1041);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1061);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1081);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1101);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1121);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1141);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1161);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1181);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1201);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1221);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1241);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1261);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1281);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
