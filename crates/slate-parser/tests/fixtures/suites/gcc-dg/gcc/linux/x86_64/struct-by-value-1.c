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
// DEFAULT-NEXT:     type @type[[TYPE_S0:[0-9]+]] S0 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 0>;
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S1:[0-9]+]] S1 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 1>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S2:[0-9]+]] S2 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 2>;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S3:[0-9]+]] S3 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 3>;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S4:[0-9]+]] S4 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 4>;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S5:[0-9]+]] S5 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 5>;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S6:[0-9]+]] S6 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 6>;
// DEFAULT-NEXT:     } [size=6, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S7:[0-9]+]] S7 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 7>;
// DEFAULT-NEXT:     } [size=7, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S8:[0-9]+]] S8 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 8>;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S9:[0-9]+]] S9 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 9>;
// DEFAULT-NEXT:     } [size=9, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S10:[0-9]+]] S10 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 10>;
// DEFAULT-NEXT:     } [size=10, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S11:[0-9]+]] S11 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 11>;
// DEFAULT-NEXT:     } [size=11, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S12:[0-9]+]] S12 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 12>;
// DEFAULT-NEXT:     } [size=12, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S13:[0-9]+]] S13 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 13>;
// DEFAULT-NEXT:     } [size=13, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S14:[0-9]+]] S14 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 14>;
// DEFAULT-NEXT:     } [size=14, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S15:[0-9]+]] S15 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 15>;
// DEFAULT-NEXT:     } [size=15, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S16:[0-9]+]] S16 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 16>;
// DEFAULT-NEXT:     } [size=16, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S17:[0-9]+]] S17 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 17>;
// DEFAULT-NEXT:     } [size=17, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S18:[0-9]+]] S18 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 18>;
// DEFAULT-NEXT:     } [size=18, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S19:[0-9]+]] S19 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 19>;
// DEFAULT-NEXT:     } [size=19, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S20:[0-9]+]] S20 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 20>;
// DEFAULT-NEXT:     } [size=20, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S21:[0-9]+]] S21 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 21>;
// DEFAULT-NEXT:     } [size=21, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S22:[0-9]+]] S22 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 22>;
// DEFAULT-NEXT:     } [size=22, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S23:[0-9]+]] S23 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 23>;
// DEFAULT-NEXT:     } [size=23, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S24:[0-9]+]] S24 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 24>;
// DEFAULT-NEXT:     } [size=24, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S25:[0-9]+]] S25 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 25>;
// DEFAULT-NEXT:     } [size=25, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S26:[0-9]+]] S26 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 26>;
// DEFAULT-NEXT:     } [size=26, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S27:[0-9]+]] S27 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 27>;
// DEFAULT-NEXT:     } [size=27, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S28:[0-9]+]] S28 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 28>;
// DEFAULT-NEXT:     } [size=28, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S29:[0-9]+]] S29 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 29>;
// DEFAULT-NEXT:     } [size=29, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S30:[0-9]+]] S30 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 30>;
// DEFAULT-NEXT:     } [size=30, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S31:[0-9]+]] S31 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 31>;
// DEFAULT-NEXT:     } [size=31, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S32:[0-9]+]] S32 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 32>;
// DEFAULT-NEXT:     } [size=32, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S33:[0-9]+]] S33 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 33>;
// DEFAULT-NEXT:     } [size=33, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S34:[0-9]+]] S34 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 34>;
// DEFAULT-NEXT:     } [size=34, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S35:[0-9]+]] S35 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 35>;
// DEFAULT-NEXT:     } [size=35, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S36:[0-9]+]] S36 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 36>;
// DEFAULT-NEXT:     } [size=36, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S37:[0-9]+]] S37 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 37>;
// DEFAULT-NEXT:     } [size=37, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S38:[0-9]+]] S38 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 38>;
// DEFAULT-NEXT:     } [size=38, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S39:[0-9]+]] S39 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 39>;
// DEFAULT-NEXT:     } [size=39, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S40:[0-9]+]] S40 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 40>;
// DEFAULT-NEXT:     } [size=40, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S41:[0-9]+]] S41 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 41>;
// DEFAULT-NEXT:     } [size=41, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S42:[0-9]+]] S42 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 42>;
// DEFAULT-NEXT:     } [size=42, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S43:[0-9]+]] S43 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 43>;
// DEFAULT-NEXT:     } [size=43, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S44:[0-9]+]] S44 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 44>;
// DEFAULT-NEXT:     } [size=44, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S45:[0-9]+]] S45 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 45>;
// DEFAULT-NEXT:     } [size=45, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S46:[0-9]+]] S46 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 46>;
// DEFAULT-NEXT:     } [size=46, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S47:[0-9]+]] S47 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 47>;
// DEFAULT-NEXT:     } [size=47, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S48:[0-9]+]] S48 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 48>;
// DEFAULT-NEXT:     } [size=48, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S49:[0-9]+]] S49 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 49>;
// DEFAULT-NEXT:     } [size=49, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S50:[0-9]+]] S50 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 50>;
// DEFAULT-NEXT:     } [size=50, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S51:[0-9]+]] S51 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 51>;
// DEFAULT-NEXT:     } [size=51, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S52:[0-9]+]] S52 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 52>;
// DEFAULT-NEXT:     } [size=52, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S53:[0-9]+]] S53 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 53>;
// DEFAULT-NEXT:     } [size=53, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S54:[0-9]+]] S54 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 54>;
// DEFAULT-NEXT:     } [size=54, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S55:[0-9]+]] S55 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 55>;
// DEFAULT-NEXT:     } [size=55, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S56:[0-9]+]] S56 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 56>;
// DEFAULT-NEXT:     } [size=56, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S57:[0-9]+]] S57 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 57>;
// DEFAULT-NEXT:     } [size=57, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S58:[0-9]+]] S58 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 58>;
// DEFAULT-NEXT:     } [size=58, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S59:[0-9]+]] S59 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 59>;
// DEFAULT-NEXT:     } [size=59, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S60:[0-9]+]] S60 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 60>;
// DEFAULT-NEXT:     } [size=60, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S61:[0-9]+]] S61 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 61>;
// DEFAULT-NEXT:     } [size=61, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S62:[0-9]+]] S62 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 62>;
// DEFAULT-NEXT:     } [size=62, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_S63:[0-9]+]] S63 = struct {
// DEFAULT-NEXT:         field0 i: array<u8, 63>;
// DEFAULT-NEXT:     } [size=63, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_g1s0:[0-9]+]] g1s0: @type[[TYPE_S0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s0:[0-9]+]] g2s0: @type[[TYPE_S0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s0:[0-9]+]] g3s0: @type[[TYPE_S0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s1:[0-9]+]] g1s1: @type[[TYPE_S1]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s1:[0-9]+]] g2s1: @type[[TYPE_S1]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s1:[0-9]+]] g3s1: @type[[TYPE_S1]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s2:[0-9]+]] g1s2: @type[[TYPE_S2]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s2:[0-9]+]] g2s2: @type[[TYPE_S2]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s2:[0-9]+]] g3s2: @type[[TYPE_S2]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s3:[0-9]+]] g1s3: @type[[TYPE_S3]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s3:[0-9]+]] g2s3: @type[[TYPE_S3]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s3:[0-9]+]] g3s3: @type[[TYPE_S3]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s4:[0-9]+]] g1s4: @type[[TYPE_S4]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s4:[0-9]+]] g2s4: @type[[TYPE_S4]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s4:[0-9]+]] g3s4: @type[[TYPE_S4]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s5:[0-9]+]] g1s5: @type[[TYPE_S5]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s5:[0-9]+]] g2s5: @type[[TYPE_S5]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s5:[0-9]+]] g3s5: @type[[TYPE_S5]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s6:[0-9]+]] g1s6: @type[[TYPE_S6]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s6:[0-9]+]] g2s6: @type[[TYPE_S6]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s6:[0-9]+]] g3s6: @type[[TYPE_S6]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s7:[0-9]+]] g1s7: @type[[TYPE_S7]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s7:[0-9]+]] g2s7: @type[[TYPE_S7]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s7:[0-9]+]] g3s7: @type[[TYPE_S7]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s8:[0-9]+]] g1s8: @type[[TYPE_S8]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s8:[0-9]+]] g2s8: @type[[TYPE_S8]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s8:[0-9]+]] g3s8: @type[[TYPE_S8]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s9:[0-9]+]] g1s9: @type[[TYPE_S9]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s9:[0-9]+]] g2s9: @type[[TYPE_S9]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s9:[0-9]+]] g3s9: @type[[TYPE_S9]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s10:[0-9]+]] g1s10: @type[[TYPE_S10]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s10:[0-9]+]] g2s10: @type[[TYPE_S10]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s10:[0-9]+]] g3s10: @type[[TYPE_S10]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s11:[0-9]+]] g1s11: @type[[TYPE_S11]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s11:[0-9]+]] g2s11: @type[[TYPE_S11]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s11:[0-9]+]] g3s11: @type[[TYPE_S11]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s12:[0-9]+]] g1s12: @type[[TYPE_S12]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s12:[0-9]+]] g2s12: @type[[TYPE_S12]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s12:[0-9]+]] g3s12: @type[[TYPE_S12]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s13:[0-9]+]] g1s13: @type[[TYPE_S13]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s13:[0-9]+]] g2s13: @type[[TYPE_S13]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s13:[0-9]+]] g3s13: @type[[TYPE_S13]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s14:[0-9]+]] g1s14: @type[[TYPE_S14]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s14:[0-9]+]] g2s14: @type[[TYPE_S14]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s14:[0-9]+]] g3s14: @type[[TYPE_S14]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s15:[0-9]+]] g1s15: @type[[TYPE_S15]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s15:[0-9]+]] g2s15: @type[[TYPE_S15]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s15:[0-9]+]] g3s15: @type[[TYPE_S15]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s16:[0-9]+]] g1s16: @type[[TYPE_S16]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s16:[0-9]+]] g2s16: @type[[TYPE_S16]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s16:[0-9]+]] g3s16: @type[[TYPE_S16]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s17:[0-9]+]] g1s17: @type[[TYPE_S17]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s17:[0-9]+]] g2s17: @type[[TYPE_S17]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s17:[0-9]+]] g3s17: @type[[TYPE_S17]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s18:[0-9]+]] g1s18: @type[[TYPE_S18]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s18:[0-9]+]] g2s18: @type[[TYPE_S18]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s18:[0-9]+]] g3s18: @type[[TYPE_S18]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s19:[0-9]+]] g1s19: @type[[TYPE_S19]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s19:[0-9]+]] g2s19: @type[[TYPE_S19]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s19:[0-9]+]] g3s19: @type[[TYPE_S19]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s20:[0-9]+]] g1s20: @type[[TYPE_S20]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s20:[0-9]+]] g2s20: @type[[TYPE_S20]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s20:[0-9]+]] g3s20: @type[[TYPE_S20]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s21:[0-9]+]] g1s21: @type[[TYPE_S21]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s21:[0-9]+]] g2s21: @type[[TYPE_S21]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s21:[0-9]+]] g3s21: @type[[TYPE_S21]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s22:[0-9]+]] g1s22: @type[[TYPE_S22]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s22:[0-9]+]] g2s22: @type[[TYPE_S22]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s22:[0-9]+]] g3s22: @type[[TYPE_S22]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s23:[0-9]+]] g1s23: @type[[TYPE_S23]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s23:[0-9]+]] g2s23: @type[[TYPE_S23]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s23:[0-9]+]] g3s23: @type[[TYPE_S23]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s24:[0-9]+]] g1s24: @type[[TYPE_S24]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s24:[0-9]+]] g2s24: @type[[TYPE_S24]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s24:[0-9]+]] g3s24: @type[[TYPE_S24]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s25:[0-9]+]] g1s25: @type[[TYPE_S25]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s25:[0-9]+]] g2s25: @type[[TYPE_S25]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s25:[0-9]+]] g3s25: @type[[TYPE_S25]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s26:[0-9]+]] g1s26: @type[[TYPE_S26]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s26:[0-9]+]] g2s26: @type[[TYPE_S26]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s26:[0-9]+]] g3s26: @type[[TYPE_S26]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s27:[0-9]+]] g1s27: @type[[TYPE_S27]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s27:[0-9]+]] g2s27: @type[[TYPE_S27]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s27:[0-9]+]] g3s27: @type[[TYPE_S27]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s28:[0-9]+]] g1s28: @type[[TYPE_S28]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s28:[0-9]+]] g2s28: @type[[TYPE_S28]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s28:[0-9]+]] g3s28: @type[[TYPE_S28]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s29:[0-9]+]] g1s29: @type[[TYPE_S29]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s29:[0-9]+]] g2s29: @type[[TYPE_S29]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s29:[0-9]+]] g3s29: @type[[TYPE_S29]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s30:[0-9]+]] g1s30: @type[[TYPE_S30]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s30:[0-9]+]] g2s30: @type[[TYPE_S30]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s30:[0-9]+]] g3s30: @type[[TYPE_S30]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s31:[0-9]+]] g1s31: @type[[TYPE_S31]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s31:[0-9]+]] g2s31: @type[[TYPE_S31]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s31:[0-9]+]] g3s31: @type[[TYPE_S31]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s32:[0-9]+]] g1s32: @type[[TYPE_S32]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s32:[0-9]+]] g2s32: @type[[TYPE_S32]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s32:[0-9]+]] g3s32: @type[[TYPE_S32]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s33:[0-9]+]] g1s33: @type[[TYPE_S33]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s33:[0-9]+]] g2s33: @type[[TYPE_S33]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s33:[0-9]+]] g3s33: @type[[TYPE_S33]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s34:[0-9]+]] g1s34: @type[[TYPE_S34]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s34:[0-9]+]] g2s34: @type[[TYPE_S34]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s34:[0-9]+]] g3s34: @type[[TYPE_S34]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s35:[0-9]+]] g1s35: @type[[TYPE_S35]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s35:[0-9]+]] g2s35: @type[[TYPE_S35]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s35:[0-9]+]] g3s35: @type[[TYPE_S35]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s36:[0-9]+]] g1s36: @type[[TYPE_S36]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s36:[0-9]+]] g2s36: @type[[TYPE_S36]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s36:[0-9]+]] g3s36: @type[[TYPE_S36]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s37:[0-9]+]] g1s37: @type[[TYPE_S37]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s37:[0-9]+]] g2s37: @type[[TYPE_S37]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s37:[0-9]+]] g3s37: @type[[TYPE_S37]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s38:[0-9]+]] g1s38: @type[[TYPE_S38]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s38:[0-9]+]] g2s38: @type[[TYPE_S38]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s38:[0-9]+]] g3s38: @type[[TYPE_S38]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s39:[0-9]+]] g1s39: @type[[TYPE_S39]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s39:[0-9]+]] g2s39: @type[[TYPE_S39]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s39:[0-9]+]] g3s39: @type[[TYPE_S39]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s40:[0-9]+]] g1s40: @type[[TYPE_S40]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s40:[0-9]+]] g2s40: @type[[TYPE_S40]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s40:[0-9]+]] g3s40: @type[[TYPE_S40]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s41:[0-9]+]] g1s41: @type[[TYPE_S41]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s41:[0-9]+]] g2s41: @type[[TYPE_S41]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s41:[0-9]+]] g3s41: @type[[TYPE_S41]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s42:[0-9]+]] g1s42: @type[[TYPE_S42]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s42:[0-9]+]] g2s42: @type[[TYPE_S42]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s42:[0-9]+]] g3s42: @type[[TYPE_S42]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s43:[0-9]+]] g1s43: @type[[TYPE_S43]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s43:[0-9]+]] g2s43: @type[[TYPE_S43]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s43:[0-9]+]] g3s43: @type[[TYPE_S43]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s44:[0-9]+]] g1s44: @type[[TYPE_S44]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s44:[0-9]+]] g2s44: @type[[TYPE_S44]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s44:[0-9]+]] g3s44: @type[[TYPE_S44]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s45:[0-9]+]] g1s45: @type[[TYPE_S45]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s45:[0-9]+]] g2s45: @type[[TYPE_S45]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s45:[0-9]+]] g3s45: @type[[TYPE_S45]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s46:[0-9]+]] g1s46: @type[[TYPE_S46]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s46:[0-9]+]] g2s46: @type[[TYPE_S46]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s46:[0-9]+]] g3s46: @type[[TYPE_S46]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s47:[0-9]+]] g1s47: @type[[TYPE_S47]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s47:[0-9]+]] g2s47: @type[[TYPE_S47]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s47:[0-9]+]] g3s47: @type[[TYPE_S47]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s48:[0-9]+]] g1s48: @type[[TYPE_S48]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s48:[0-9]+]] g2s48: @type[[TYPE_S48]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s48:[0-9]+]] g3s48: @type[[TYPE_S48]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s49:[0-9]+]] g1s49: @type[[TYPE_S49]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s49:[0-9]+]] g2s49: @type[[TYPE_S49]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s49:[0-9]+]] g3s49: @type[[TYPE_S49]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s50:[0-9]+]] g1s50: @type[[TYPE_S50]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s50:[0-9]+]] g2s50: @type[[TYPE_S50]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s50:[0-9]+]] g3s50: @type[[TYPE_S50]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s51:[0-9]+]] g1s51: @type[[TYPE_S51]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s51:[0-9]+]] g2s51: @type[[TYPE_S51]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s51:[0-9]+]] g3s51: @type[[TYPE_S51]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s52:[0-9]+]] g1s52: @type[[TYPE_S52]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s52:[0-9]+]] g2s52: @type[[TYPE_S52]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s52:[0-9]+]] g3s52: @type[[TYPE_S52]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s53:[0-9]+]] g1s53: @type[[TYPE_S53]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s53:[0-9]+]] g2s53: @type[[TYPE_S53]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s53:[0-9]+]] g3s53: @type[[TYPE_S53]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s54:[0-9]+]] g1s54: @type[[TYPE_S54]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s54:[0-9]+]] g2s54: @type[[TYPE_S54]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s54:[0-9]+]] g3s54: @type[[TYPE_S54]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s55:[0-9]+]] g1s55: @type[[TYPE_S55]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s55:[0-9]+]] g2s55: @type[[TYPE_S55]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s55:[0-9]+]] g3s55: @type[[TYPE_S55]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s56:[0-9]+]] g1s56: @type[[TYPE_S56]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s56:[0-9]+]] g2s56: @type[[TYPE_S56]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s56:[0-9]+]] g3s56: @type[[TYPE_S56]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s57:[0-9]+]] g1s57: @type[[TYPE_S57]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s57:[0-9]+]] g2s57: @type[[TYPE_S57]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s57:[0-9]+]] g3s57: @type[[TYPE_S57]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s58:[0-9]+]] g1s58: @type[[TYPE_S58]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s58:[0-9]+]] g2s58: @type[[TYPE_S58]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s58:[0-9]+]] g3s58: @type[[TYPE_S58]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s59:[0-9]+]] g1s59: @type[[TYPE_S59]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s59:[0-9]+]] g2s59: @type[[TYPE_S59]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s59:[0-9]+]] g3s59: @type[[TYPE_S59]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s60:[0-9]+]] g1s60: @type[[TYPE_S60]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s60:[0-9]+]] g2s60: @type[[TYPE_S60]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s60:[0-9]+]] g3s60: @type[[TYPE_S60]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s61:[0-9]+]] g1s61: @type[[TYPE_S61]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s61:[0-9]+]] g2s61: @type[[TYPE_S61]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s61:[0-9]+]] g3s61: @type[[TYPE_S61]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s62:[0-9]+]] g1s62: @type[[TYPE_S62]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s62:[0-9]+]] g2s62: @type[[TYPE_S62]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s62:[0-9]+]] g3s62: @type[[TYPE_S62]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g1s63:[0-9]+]] g1s63: @type[[TYPE_S63]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g2s63:[0-9]+]] g2s63: @type[[TYPE_S63]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g3s63:[0-9]+]] g3s63: @type[[TYPE_S63]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_init0:[0-9]+]] @init0(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_S0]]>, %[[VALUE_i:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(0)>(field0(deref(read<ptr<@type[[TYPE_S0]]>>(%[[VALUE_p]])))), read<i32>(%[[VALUE_j]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_j]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check0:[0-9]+]] @check0(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_S0]]>, %[[VALUE_i_2:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_2:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_2]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_2]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_2]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(0)>(field0(deref(read<ptr<@type[[TYPE_S0]]>>(%[[VALUE_p_2]])))), read<i32>(%[[VALUE_j_2]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_j_2]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test0:[0-9]+]] @test0(%[[VALUE_s1:[0-9]+]] s1: @type[[TYPE_S0]], %[[VALUE_s2:[0-9]+]] s2: @type[[TYPE_S0]], %[[VALUE_s3:[0-9]+]] s3: @type[[TYPE_S0]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S0]]>, i32) -> void>(%[[VALUE_check0]], addr_of<ptr<@type[[TYPE_S0]]>>(%[[VALUE_s1]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S0]]>, i32) -> void>(%[[VALUE_check0]], addr_of<ptr<@type[[TYPE_S0]]>>(%[[VALUE_s2]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S0]]>, i32) -> void>(%[[VALUE_check0]], addr_of<ptr<@type[[TYPE_S0]]>>(%[[VALUE_s3]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_0:[0-9]+]] @test2_0(%[[VALUE_s1_2:[0-9]+]] s1: @type[[TYPE_S0]], %[[VALUE_s2_2:[0-9]+]] s2: @type[[TYPE_S0]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S0]], @type[[TYPE_S0]], @type[[TYPE_S0]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test0]], copy<@type[[TYPE_S0]], reason=arg>(read<@type[[TYPE_S0]]>(%[[VALUE_s1_2]])), copy<@type[[TYPE_S0]], reason=arg>(read<@type[[TYPE_S0]]>(%[[VALUE_g2s0]])), copy<@type[[TYPE_S0]], reason=arg>(read<@type[[TYPE_S0]]>(%[[VALUE_s2_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit0:[0-9]+]] @testit0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S0]]>, i32) -> void>(%[[VALUE_init0]], addr_of<ptr<@type[[TYPE_S0]]>>(%[[VALUE_g1s0]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S0]]>, i32) -> void>(%[[VALUE_check0]], addr_of<ptr<@type[[TYPE_S0]]>>(%[[VALUE_g1s0]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S0]]>, i32) -> void>(%[[VALUE_init0]], addr_of<ptr<@type[[TYPE_S0]]>>(%[[VALUE_g2s0]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S0]]>, i32) -> void>(%[[VALUE_check0]], addr_of<ptr<@type[[TYPE_S0]]>>(%[[VALUE_g2s0]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S0]]>, i32) -> void>(%[[VALUE_init0]], addr_of<ptr<@type[[TYPE_S0]]>>(%[[VALUE_g3s0]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S0]]>, i32) -> void>(%[[VALUE_check0]], addr_of<ptr<@type[[TYPE_S0]]>>(%[[VALUE_g3s0]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S0]], @type[[TYPE_S0]], @type[[TYPE_S0]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test0]], copy<@type[[TYPE_S0]], reason=arg>(read<@type[[TYPE_S0]]>(%[[VALUE_g1s0]])), copy<@type[[TYPE_S0]], reason=arg>(read<@type[[TYPE_S0]]>(%[[VALUE_g2s0]])), copy<@type[[TYPE_S0]], reason=arg>(read<@type[[TYPE_S0]]>(%[[VALUE_g3s0]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S0]], @type[[TYPE_S0]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_0]], copy<@type[[TYPE_S0]], reason=arg>(read<@type[[TYPE_S0]]>(%[[VALUE_g1s0]])), copy<@type[[TYPE_S0]], reason=arg>(read<@type[[TYPE_S0]]>(%[[VALUE_g3s0]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init1:[0-9]+]] @init1(%[[VALUE_p_3:[0-9]+]] p: ptr<@type[[TYPE_S1]]>, %[[VALUE_i_3:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_3:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_3]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_3]]), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_3]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_3]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1)>(field0(deref(read<ptr<@type[[TYPE_S1]]>>(%[[VALUE_p_3]])))), read<i32>(%[[VALUE_j_3]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_3]]), read<i32>(%[[VALUE_j_3]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check1:[0-9]+]] @check1(%[[VALUE_p_4:[0-9]+]] p: ptr<@type[[TYPE_S1]]>, %[[VALUE_i_4:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_4:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_4]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_4]]), const<i32>(1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_4]]);
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_4]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1)>(field0(deref(read<ptr<@type[[TYPE_S1]]>>(%[[VALUE_p_4]])))), read<i32>(%[[VALUE_j_4]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_4]]), read<i32>(%[[VALUE_j_4]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_s1_3:[0-9]+]] s1: @type[[TYPE_S1]], %[[VALUE_s2_3:[0-9]+]] s2: @type[[TYPE_S1]], %[[VALUE_s3_2:[0-9]+]] s3: @type[[TYPE_S1]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S1]]>, i32) -> void>(%[[VALUE_check1]], addr_of<ptr<@type[[TYPE_S1]]>>(%[[VALUE_s1_3]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S1]]>, i32) -> void>(%[[VALUE_check1]], addr_of<ptr<@type[[TYPE_S1]]>>(%[[VALUE_s2_3]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S1]]>, i32) -> void>(%[[VALUE_check1]], addr_of<ptr<@type[[TYPE_S1]]>>(%[[VALUE_s3_2]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_1:[0-9]+]] @test2_1(%[[VALUE_s1_4:[0-9]+]] s1: @type[[TYPE_S1]], %[[VALUE_s2_4:[0-9]+]] s2: @type[[TYPE_S1]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S1]], @type[[TYPE_S1]], @type[[TYPE_S1]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test1]], copy<@type[[TYPE_S1]], reason=arg>(read<@type[[TYPE_S1]]>(%[[VALUE_s1_4]])), copy<@type[[TYPE_S1]], reason=arg>(read<@type[[TYPE_S1]]>(%[[VALUE_g2s1]])), copy<@type[[TYPE_S1]], reason=arg>(read<@type[[TYPE_S1]]>(%[[VALUE_s2_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit1:[0-9]+]] @testit1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S1]]>, i32) -> void>(%[[VALUE_init1]], addr_of<ptr<@type[[TYPE_S1]]>>(%[[VALUE_g1s1]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S1]]>, i32) -> void>(%[[VALUE_check1]], addr_of<ptr<@type[[TYPE_S1]]>>(%[[VALUE_g1s1]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S1]]>, i32) -> void>(%[[VALUE_init1]], addr_of<ptr<@type[[TYPE_S1]]>>(%[[VALUE_g2s1]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S1]]>, i32) -> void>(%[[VALUE_check1]], addr_of<ptr<@type[[TYPE_S1]]>>(%[[VALUE_g2s1]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S1]]>, i32) -> void>(%[[VALUE_init1]], addr_of<ptr<@type[[TYPE_S1]]>>(%[[VALUE_g3s1]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S1]]>, i32) -> void>(%[[VALUE_check1]], addr_of<ptr<@type[[TYPE_S1]]>>(%[[VALUE_g3s1]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S1]], @type[[TYPE_S1]], @type[[TYPE_S1]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test1]], copy<@type[[TYPE_S1]], reason=arg>(read<@type[[TYPE_S1]]>(%[[VALUE_g1s1]])), copy<@type[[TYPE_S1]], reason=arg>(read<@type[[TYPE_S1]]>(%[[VALUE_g2s1]])), copy<@type[[TYPE_S1]], reason=arg>(read<@type[[TYPE_S1]]>(%[[VALUE_g3s1]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S1]], @type[[TYPE_S1]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_1]], copy<@type[[TYPE_S1]], reason=arg>(read<@type[[TYPE_S1]]>(%[[VALUE_g1s1]])), copy<@type[[TYPE_S1]], reason=arg>(read<@type[[TYPE_S1]]>(%[[VALUE_g3s1]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init2:[0-9]+]] @init2(%[[VALUE_p_5:[0-9]+]] p: ptr<@type[[TYPE_S2]]>, %[[VALUE_i_5:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_5:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_5]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_5]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_5]]);
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_5]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(2)>(field0(deref(read<ptr<@type[[TYPE_S2]]>>(%[[VALUE_p_5]])))), read<i32>(%[[VALUE_j_5]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_5]]), read<i32>(%[[VALUE_j_5]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check2:[0-9]+]] @check2(%[[VALUE_p_6:[0-9]+]] p: ptr<@type[[TYPE_S2]]>, %[[VALUE_i_6:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_6:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_6]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_6]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_6]]);
// DEFAULT-NEXT:                 let %[[VALUE18:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE17]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_6]], read<i32>(%[[VALUE18]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(2)>(field0(deref(read<ptr<@type[[TYPE_S2]]>>(%[[VALUE_p_6]])))), read<i32>(%[[VALUE_j_6]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_6]]), read<i32>(%[[VALUE_j_6]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_s1_5:[0-9]+]] s1: @type[[TYPE_S2]], %[[VALUE_s2_5:[0-9]+]] s2: @type[[TYPE_S2]], %[[VALUE_s3_3:[0-9]+]] s3: @type[[TYPE_S2]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S2]]>, i32) -> void>(%[[VALUE_check2]], addr_of<ptr<@type[[TYPE_S2]]>>(%[[VALUE_s1_5]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S2]]>, i32) -> void>(%[[VALUE_check2]], addr_of<ptr<@type[[TYPE_S2]]>>(%[[VALUE_s2_5]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S2]]>, i32) -> void>(%[[VALUE_check2]], addr_of<ptr<@type[[TYPE_S2]]>>(%[[VALUE_s3_3]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_2:[0-9]+]] @test2_2(%[[VALUE_s1_6:[0-9]+]] s1: @type[[TYPE_S2]], %[[VALUE_s2_6:[0-9]+]] s2: @type[[TYPE_S2]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S2]], @type[[TYPE_S2]], @type[[TYPE_S2]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test2]], copy<@type[[TYPE_S2]], reason=arg>(read<@type[[TYPE_S2]]>(%[[VALUE_s1_6]])), copy<@type[[TYPE_S2]], reason=arg>(read<@type[[TYPE_S2]]>(%[[VALUE_g2s2]])), copy<@type[[TYPE_S2]], reason=arg>(read<@type[[TYPE_S2]]>(%[[VALUE_s2_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit2:[0-9]+]] @testit2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S2]]>, i32) -> void>(%[[VALUE_init2]], addr_of<ptr<@type[[TYPE_S2]]>>(%[[VALUE_g1s2]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S2]]>, i32) -> void>(%[[VALUE_check2]], addr_of<ptr<@type[[TYPE_S2]]>>(%[[VALUE_g1s2]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S2]]>, i32) -> void>(%[[VALUE_init2]], addr_of<ptr<@type[[TYPE_S2]]>>(%[[VALUE_g2s2]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S2]]>, i32) -> void>(%[[VALUE_check2]], addr_of<ptr<@type[[TYPE_S2]]>>(%[[VALUE_g2s2]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S2]]>, i32) -> void>(%[[VALUE_init2]], addr_of<ptr<@type[[TYPE_S2]]>>(%[[VALUE_g3s2]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S2]]>, i32) -> void>(%[[VALUE_check2]], addr_of<ptr<@type[[TYPE_S2]]>>(%[[VALUE_g3s2]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S2]], @type[[TYPE_S2]], @type[[TYPE_S2]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test2]], copy<@type[[TYPE_S2]], reason=arg>(read<@type[[TYPE_S2]]>(%[[VALUE_g1s2]])), copy<@type[[TYPE_S2]], reason=arg>(read<@type[[TYPE_S2]]>(%[[VALUE_g2s2]])), copy<@type[[TYPE_S2]], reason=arg>(read<@type[[TYPE_S2]]>(%[[VALUE_g3s2]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S2]], @type[[TYPE_S2]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_2]], copy<@type[[TYPE_S2]], reason=arg>(read<@type[[TYPE_S2]]>(%[[VALUE_g1s2]])), copy<@type[[TYPE_S2]], reason=arg>(read<@type[[TYPE_S2]]>(%[[VALUE_g3s2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init3:[0-9]+]] @init3(%[[VALUE_p_7:[0-9]+]] p: ptr<@type[[TYPE_S3]]>, %[[VALUE_i_7:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_7:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_7]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_7]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_7]]);
// DEFAULT-NEXT:                 let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_7]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(3)>(field0(deref(read<ptr<@type[[TYPE_S3]]>>(%[[VALUE_p_7]])))), read<i32>(%[[VALUE_j_7]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_7]]), read<i32>(%[[VALUE_j_7]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check3:[0-9]+]] @check3(%[[VALUE_p_8:[0-9]+]] p: ptr<@type[[TYPE_S3]]>, %[[VALUE_i_8:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_8:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_8]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_8]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_8]]);
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE23]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_8]], read<i32>(%[[VALUE24]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(3)>(field0(deref(read<ptr<@type[[TYPE_S3]]>>(%[[VALUE_p_8]])))), read<i32>(%[[VALUE_j_8]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_8]]), read<i32>(%[[VALUE_j_8]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_s1_7:[0-9]+]] s1: @type[[TYPE_S3]], %[[VALUE_s2_7:[0-9]+]] s2: @type[[TYPE_S3]], %[[VALUE_s3_4:[0-9]+]] s3: @type[[TYPE_S3]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S3]]>, i32) -> void>(%[[VALUE_check3]], addr_of<ptr<@type[[TYPE_S3]]>>(%[[VALUE_s1_7]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S3]]>, i32) -> void>(%[[VALUE_check3]], addr_of<ptr<@type[[TYPE_S3]]>>(%[[VALUE_s2_7]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S3]]>, i32) -> void>(%[[VALUE_check3]], addr_of<ptr<@type[[TYPE_S3]]>>(%[[VALUE_s3_4]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_3:[0-9]+]] @test2_3(%[[VALUE_s1_8:[0-9]+]] s1: @type[[TYPE_S3]], %[[VALUE_s2_8:[0-9]+]] s2: @type[[TYPE_S3]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S3]], @type[[TYPE_S3]], @type[[TYPE_S3]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test3]], copy<@type[[TYPE_S3]], reason=arg>(read<@type[[TYPE_S3]]>(%[[VALUE_s1_8]])), copy<@type[[TYPE_S3]], reason=arg>(read<@type[[TYPE_S3]]>(%[[VALUE_g2s3]])), copy<@type[[TYPE_S3]], reason=arg>(read<@type[[TYPE_S3]]>(%[[VALUE_s2_8]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit3:[0-9]+]] @testit3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S3]]>, i32) -> void>(%[[VALUE_init3]], addr_of<ptr<@type[[TYPE_S3]]>>(%[[VALUE_g1s3]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S3]]>, i32) -> void>(%[[VALUE_check3]], addr_of<ptr<@type[[TYPE_S3]]>>(%[[VALUE_g1s3]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S3]]>, i32) -> void>(%[[VALUE_init3]], addr_of<ptr<@type[[TYPE_S3]]>>(%[[VALUE_g2s3]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S3]]>, i32) -> void>(%[[VALUE_check3]], addr_of<ptr<@type[[TYPE_S3]]>>(%[[VALUE_g2s3]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S3]]>, i32) -> void>(%[[VALUE_init3]], addr_of<ptr<@type[[TYPE_S3]]>>(%[[VALUE_g3s3]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S3]]>, i32) -> void>(%[[VALUE_check3]], addr_of<ptr<@type[[TYPE_S3]]>>(%[[VALUE_g3s3]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S3]], @type[[TYPE_S3]], @type[[TYPE_S3]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test3]], copy<@type[[TYPE_S3]], reason=arg>(read<@type[[TYPE_S3]]>(%[[VALUE_g1s3]])), copy<@type[[TYPE_S3]], reason=arg>(read<@type[[TYPE_S3]]>(%[[VALUE_g2s3]])), copy<@type[[TYPE_S3]], reason=arg>(read<@type[[TYPE_S3]]>(%[[VALUE_g3s3]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S3]], @type[[TYPE_S3]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_3]], copy<@type[[TYPE_S3]], reason=arg>(read<@type[[TYPE_S3]]>(%[[VALUE_g1s3]])), copy<@type[[TYPE_S3]], reason=arg>(read<@type[[TYPE_S3]]>(%[[VALUE_g3s3]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init4:[0-9]+]] @init4(%[[VALUE_p_9:[0-9]+]] p: ptr<@type[[TYPE_S4]]>, %[[VALUE_i_9:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_9:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_9]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_9]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_9]]);
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_9]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S4]]>>(%[[VALUE_p_9]])))), read<i32>(%[[VALUE_j_9]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_9]]), read<i32>(%[[VALUE_j_9]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check4:[0-9]+]] @check4(%[[VALUE_p_10:[0-9]+]] p: ptr<@type[[TYPE_S4]]>, %[[VALUE_i_10:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_10:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_10]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_10]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_10]]);
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE29]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_10]], read<i32>(%[[VALUE30]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_S4]]>>(%[[VALUE_p_10]])))), read<i32>(%[[VALUE_j_10]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_10]]), read<i32>(%[[VALUE_j_10]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_s1_9:[0-9]+]] s1: @type[[TYPE_S4]], %[[VALUE_s2_9:[0-9]+]] s2: @type[[TYPE_S4]], %[[VALUE_s3_5:[0-9]+]] s3: @type[[TYPE_S4]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S4]]>, i32) -> void>(%[[VALUE_check4]], addr_of<ptr<@type[[TYPE_S4]]>>(%[[VALUE_s1_9]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S4]]>, i32) -> void>(%[[VALUE_check4]], addr_of<ptr<@type[[TYPE_S4]]>>(%[[VALUE_s2_9]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S4]]>, i32) -> void>(%[[VALUE_check4]], addr_of<ptr<@type[[TYPE_S4]]>>(%[[VALUE_s3_5]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_4:[0-9]+]] @test2_4(%[[VALUE_s1_10:[0-9]+]] s1: @type[[TYPE_S4]], %[[VALUE_s2_10:[0-9]+]] s2: @type[[TYPE_S4]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S4]], @type[[TYPE_S4]], @type[[TYPE_S4]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test4]], copy<@type[[TYPE_S4]], reason=arg>(read<@type[[TYPE_S4]]>(%[[VALUE_s1_10]])), copy<@type[[TYPE_S4]], reason=arg>(read<@type[[TYPE_S4]]>(%[[VALUE_g2s4]])), copy<@type[[TYPE_S4]], reason=arg>(read<@type[[TYPE_S4]]>(%[[VALUE_s2_10]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit4:[0-9]+]] @testit4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S4]]>, i32) -> void>(%[[VALUE_init4]], addr_of<ptr<@type[[TYPE_S4]]>>(%[[VALUE_g1s4]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S4]]>, i32) -> void>(%[[VALUE_check4]], addr_of<ptr<@type[[TYPE_S4]]>>(%[[VALUE_g1s4]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S4]]>, i32) -> void>(%[[VALUE_init4]], addr_of<ptr<@type[[TYPE_S4]]>>(%[[VALUE_g2s4]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S4]]>, i32) -> void>(%[[VALUE_check4]], addr_of<ptr<@type[[TYPE_S4]]>>(%[[VALUE_g2s4]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S4]]>, i32) -> void>(%[[VALUE_init4]], addr_of<ptr<@type[[TYPE_S4]]>>(%[[VALUE_g3s4]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S4]]>, i32) -> void>(%[[VALUE_check4]], addr_of<ptr<@type[[TYPE_S4]]>>(%[[VALUE_g3s4]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S4]], @type[[TYPE_S4]], @type[[TYPE_S4]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test4]], copy<@type[[TYPE_S4]], reason=arg>(read<@type[[TYPE_S4]]>(%[[VALUE_g1s4]])), copy<@type[[TYPE_S4]], reason=arg>(read<@type[[TYPE_S4]]>(%[[VALUE_g2s4]])), copy<@type[[TYPE_S4]], reason=arg>(read<@type[[TYPE_S4]]>(%[[VALUE_g3s4]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S4]], @type[[TYPE_S4]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_4]], copy<@type[[TYPE_S4]], reason=arg>(read<@type[[TYPE_S4]]>(%[[VALUE_g1s4]])), copy<@type[[TYPE_S4]], reason=arg>(read<@type[[TYPE_S4]]>(%[[VALUE_g3s4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init5:[0-9]+]] @init5(%[[VALUE_p_11:[0-9]+]] p: ptr<@type[[TYPE_S5]]>, %[[VALUE_i_11:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_11:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE31:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_11]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_11]]), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE32:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_11]]);
// DEFAULT-NEXT:                 let %[[VALUE33:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE32]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_11]], read<i32>(%[[VALUE33]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S5]]>>(%[[VALUE_p_11]])))), read<i32>(%[[VALUE_j_11]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_11]]), read<i32>(%[[VALUE_j_11]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check5:[0-9]+]] @check5(%[[VALUE_p_12:[0-9]+]] p: ptr<@type[[TYPE_S5]]>, %[[VALUE_i_12:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_12:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE34:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_12]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_12]]), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE35:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_12]]);
// DEFAULT-NEXT:                 let %[[VALUE36:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE35]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_12]], read<i32>(%[[VALUE36]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(5)>(field0(deref(read<ptr<@type[[TYPE_S5]]>>(%[[VALUE_p_12]])))), read<i32>(%[[VALUE_j_12]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_12]]), read<i32>(%[[VALUE_j_12]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test5:[0-9]+]] @test5(%[[VALUE_s1_11:[0-9]+]] s1: @type[[TYPE_S5]], %[[VALUE_s2_11:[0-9]+]] s2: @type[[TYPE_S5]], %[[VALUE_s3_6:[0-9]+]] s3: @type[[TYPE_S5]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S5]]>, i32) -> void>(%[[VALUE_check5]], addr_of<ptr<@type[[TYPE_S5]]>>(%[[VALUE_s1_11]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S5]]>, i32) -> void>(%[[VALUE_check5]], addr_of<ptr<@type[[TYPE_S5]]>>(%[[VALUE_s2_11]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S5]]>, i32) -> void>(%[[VALUE_check5]], addr_of<ptr<@type[[TYPE_S5]]>>(%[[VALUE_s3_6]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_5:[0-9]+]] @test2_5(%[[VALUE_s1_12:[0-9]+]] s1: @type[[TYPE_S5]], %[[VALUE_s2_12:[0-9]+]] s2: @type[[TYPE_S5]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S5]], @type[[TYPE_S5]], @type[[TYPE_S5]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test5]], copy<@type[[TYPE_S5]], reason=arg>(read<@type[[TYPE_S5]]>(%[[VALUE_s1_12]])), copy<@type[[TYPE_S5]], reason=arg>(read<@type[[TYPE_S5]]>(%[[VALUE_g2s5]])), copy<@type[[TYPE_S5]], reason=arg>(read<@type[[TYPE_S5]]>(%[[VALUE_s2_12]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit5:[0-9]+]] @testit5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S5]]>, i32) -> void>(%[[VALUE_init5]], addr_of<ptr<@type[[TYPE_S5]]>>(%[[VALUE_g1s5]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S5]]>, i32) -> void>(%[[VALUE_check5]], addr_of<ptr<@type[[TYPE_S5]]>>(%[[VALUE_g1s5]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S5]]>, i32) -> void>(%[[VALUE_init5]], addr_of<ptr<@type[[TYPE_S5]]>>(%[[VALUE_g2s5]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S5]]>, i32) -> void>(%[[VALUE_check5]], addr_of<ptr<@type[[TYPE_S5]]>>(%[[VALUE_g2s5]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S5]]>, i32) -> void>(%[[VALUE_init5]], addr_of<ptr<@type[[TYPE_S5]]>>(%[[VALUE_g3s5]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S5]]>, i32) -> void>(%[[VALUE_check5]], addr_of<ptr<@type[[TYPE_S5]]>>(%[[VALUE_g3s5]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S5]], @type[[TYPE_S5]], @type[[TYPE_S5]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test5]], copy<@type[[TYPE_S5]], reason=arg>(read<@type[[TYPE_S5]]>(%[[VALUE_g1s5]])), copy<@type[[TYPE_S5]], reason=arg>(read<@type[[TYPE_S5]]>(%[[VALUE_g2s5]])), copy<@type[[TYPE_S5]], reason=arg>(read<@type[[TYPE_S5]]>(%[[VALUE_g3s5]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S5]], @type[[TYPE_S5]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_5]], copy<@type[[TYPE_S5]], reason=arg>(read<@type[[TYPE_S5]]>(%[[VALUE_g1s5]])), copy<@type[[TYPE_S5]], reason=arg>(read<@type[[TYPE_S5]]>(%[[VALUE_g3s5]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init6:[0-9]+]] @init6(%[[VALUE_p_13:[0-9]+]] p: ptr<@type[[TYPE_S6]]>, %[[VALUE_i_13:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_13:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_13]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_13]]), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE38:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_13]]);
// DEFAULT-NEXT:                 let %[[VALUE39:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE38]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_13]], read<i32>(%[[VALUE39]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(6)>(field0(deref(read<ptr<@type[[TYPE_S6]]>>(%[[VALUE_p_13]])))), read<i32>(%[[VALUE_j_13]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_13]]), read<i32>(%[[VALUE_j_13]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check6:[0-9]+]] @check6(%[[VALUE_p_14:[0-9]+]] p: ptr<@type[[TYPE_S6]]>, %[[VALUE_i_14:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_14:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE40:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_14]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_14]]), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE41:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_14]]);
// DEFAULT-NEXT:                 let %[[VALUE42:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE41]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_14]], read<i32>(%[[VALUE42]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(6)>(field0(deref(read<ptr<@type[[TYPE_S6]]>>(%[[VALUE_p_14]])))), read<i32>(%[[VALUE_j_14]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_14]]), read<i32>(%[[VALUE_j_14]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test6:[0-9]+]] @test6(%[[VALUE_s1_13:[0-9]+]] s1: @type[[TYPE_S6]], %[[VALUE_s2_13:[0-9]+]] s2: @type[[TYPE_S6]], %[[VALUE_s3_7:[0-9]+]] s3: @type[[TYPE_S6]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S6]]>, i32) -> void>(%[[VALUE_check6]], addr_of<ptr<@type[[TYPE_S6]]>>(%[[VALUE_s1_13]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S6]]>, i32) -> void>(%[[VALUE_check6]], addr_of<ptr<@type[[TYPE_S6]]>>(%[[VALUE_s2_13]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S6]]>, i32) -> void>(%[[VALUE_check6]], addr_of<ptr<@type[[TYPE_S6]]>>(%[[VALUE_s3_7]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_6:[0-9]+]] @test2_6(%[[VALUE_s1_14:[0-9]+]] s1: @type[[TYPE_S6]], %[[VALUE_s2_14:[0-9]+]] s2: @type[[TYPE_S6]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S6]], @type[[TYPE_S6]], @type[[TYPE_S6]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test6]], copy<@type[[TYPE_S6]], reason=arg>(read<@type[[TYPE_S6]]>(%[[VALUE_s1_14]])), copy<@type[[TYPE_S6]], reason=arg>(read<@type[[TYPE_S6]]>(%[[VALUE_g2s6]])), copy<@type[[TYPE_S6]], reason=arg>(read<@type[[TYPE_S6]]>(%[[VALUE_s2_14]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit6:[0-9]+]] @testit6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S6]]>, i32) -> void>(%[[VALUE_init6]], addr_of<ptr<@type[[TYPE_S6]]>>(%[[VALUE_g1s6]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S6]]>, i32) -> void>(%[[VALUE_check6]], addr_of<ptr<@type[[TYPE_S6]]>>(%[[VALUE_g1s6]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S6]]>, i32) -> void>(%[[VALUE_init6]], addr_of<ptr<@type[[TYPE_S6]]>>(%[[VALUE_g2s6]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S6]]>, i32) -> void>(%[[VALUE_check6]], addr_of<ptr<@type[[TYPE_S6]]>>(%[[VALUE_g2s6]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S6]]>, i32) -> void>(%[[VALUE_init6]], addr_of<ptr<@type[[TYPE_S6]]>>(%[[VALUE_g3s6]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S6]]>, i32) -> void>(%[[VALUE_check6]], addr_of<ptr<@type[[TYPE_S6]]>>(%[[VALUE_g3s6]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S6]], @type[[TYPE_S6]], @type[[TYPE_S6]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test6]], copy<@type[[TYPE_S6]], reason=arg>(read<@type[[TYPE_S6]]>(%[[VALUE_g1s6]])), copy<@type[[TYPE_S6]], reason=arg>(read<@type[[TYPE_S6]]>(%[[VALUE_g2s6]])), copy<@type[[TYPE_S6]], reason=arg>(read<@type[[TYPE_S6]]>(%[[VALUE_g3s6]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S6]], @type[[TYPE_S6]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_6]], copy<@type[[TYPE_S6]], reason=arg>(read<@type[[TYPE_S6]]>(%[[VALUE_g1s6]])), copy<@type[[TYPE_S6]], reason=arg>(read<@type[[TYPE_S6]]>(%[[VALUE_g3s6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init7:[0-9]+]] @init7(%[[VALUE_p_15:[0-9]+]] p: ptr<@type[[TYPE_S7]]>, %[[VALUE_i_15:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_15:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE43:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_15]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_15]]), const<i32>(7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE44:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_15]]);
// DEFAULT-NEXT:                 let %[[VALUE45:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE44]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_15]], read<i32>(%[[VALUE45]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(7)>(field0(deref(read<ptr<@type[[TYPE_S7]]>>(%[[VALUE_p_15]])))), read<i32>(%[[VALUE_j_15]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_15]]), read<i32>(%[[VALUE_j_15]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check7:[0-9]+]] @check7(%[[VALUE_p_16:[0-9]+]] p: ptr<@type[[TYPE_S7]]>, %[[VALUE_i_16:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_16:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE46:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_16]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_16]]), const<i32>(7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE47:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_16]]);
// DEFAULT-NEXT:                 let %[[VALUE48:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE47]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_16]], read<i32>(%[[VALUE48]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(7)>(field0(deref(read<ptr<@type[[TYPE_S7]]>>(%[[VALUE_p_16]])))), read<i32>(%[[VALUE_j_16]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_16]]), read<i32>(%[[VALUE_j_16]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test7:[0-9]+]] @test7(%[[VALUE_s1_15:[0-9]+]] s1: @type[[TYPE_S7]], %[[VALUE_s2_15:[0-9]+]] s2: @type[[TYPE_S7]], %[[VALUE_s3_8:[0-9]+]] s3: @type[[TYPE_S7]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S7]]>, i32) -> void>(%[[VALUE_check7]], addr_of<ptr<@type[[TYPE_S7]]>>(%[[VALUE_s1_15]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S7]]>, i32) -> void>(%[[VALUE_check7]], addr_of<ptr<@type[[TYPE_S7]]>>(%[[VALUE_s2_15]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S7]]>, i32) -> void>(%[[VALUE_check7]], addr_of<ptr<@type[[TYPE_S7]]>>(%[[VALUE_s3_8]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_7:[0-9]+]] @test2_7(%[[VALUE_s1_16:[0-9]+]] s1: @type[[TYPE_S7]], %[[VALUE_s2_16:[0-9]+]] s2: @type[[TYPE_S7]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S7]], @type[[TYPE_S7]], @type[[TYPE_S7]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test7]], copy<@type[[TYPE_S7]], reason=arg>(read<@type[[TYPE_S7]]>(%[[VALUE_s1_16]])), copy<@type[[TYPE_S7]], reason=arg>(read<@type[[TYPE_S7]]>(%[[VALUE_g2s7]])), copy<@type[[TYPE_S7]], reason=arg>(read<@type[[TYPE_S7]]>(%[[VALUE_s2_16]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit7:[0-9]+]] @testit7() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S7]]>, i32) -> void>(%[[VALUE_init7]], addr_of<ptr<@type[[TYPE_S7]]>>(%[[VALUE_g1s7]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S7]]>, i32) -> void>(%[[VALUE_check7]], addr_of<ptr<@type[[TYPE_S7]]>>(%[[VALUE_g1s7]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S7]]>, i32) -> void>(%[[VALUE_init7]], addr_of<ptr<@type[[TYPE_S7]]>>(%[[VALUE_g2s7]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S7]]>, i32) -> void>(%[[VALUE_check7]], addr_of<ptr<@type[[TYPE_S7]]>>(%[[VALUE_g2s7]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S7]]>, i32) -> void>(%[[VALUE_init7]], addr_of<ptr<@type[[TYPE_S7]]>>(%[[VALUE_g3s7]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S7]]>, i32) -> void>(%[[VALUE_check7]], addr_of<ptr<@type[[TYPE_S7]]>>(%[[VALUE_g3s7]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S7]], @type[[TYPE_S7]], @type[[TYPE_S7]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test7]], copy<@type[[TYPE_S7]], reason=arg>(read<@type[[TYPE_S7]]>(%[[VALUE_g1s7]])), copy<@type[[TYPE_S7]], reason=arg>(read<@type[[TYPE_S7]]>(%[[VALUE_g2s7]])), copy<@type[[TYPE_S7]], reason=arg>(read<@type[[TYPE_S7]]>(%[[VALUE_g3s7]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S7]], @type[[TYPE_S7]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_7]], copy<@type[[TYPE_S7]], reason=arg>(read<@type[[TYPE_S7]]>(%[[VALUE_g1s7]])), copy<@type[[TYPE_S7]], reason=arg>(read<@type[[TYPE_S7]]>(%[[VALUE_g3s7]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init8:[0-9]+]] @init8(%[[VALUE_p_17:[0-9]+]] p: ptr<@type[[TYPE_S8]]>, %[[VALUE_i_17:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_17:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE49:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_17]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_17]]), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE50:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_17]]);
// DEFAULT-NEXT:                 let %[[VALUE51:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE50]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_17]], read<i32>(%[[VALUE51]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field0(deref(read<ptr<@type[[TYPE_S8]]>>(%[[VALUE_p_17]])))), read<i32>(%[[VALUE_j_17]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_17]]), read<i32>(%[[VALUE_j_17]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check8:[0-9]+]] @check8(%[[VALUE_p_18:[0-9]+]] p: ptr<@type[[TYPE_S8]]>, %[[VALUE_i_18:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_18:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE52:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_18]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_18]]), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE53:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_18]]);
// DEFAULT-NEXT:                 let %[[VALUE54:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE53]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_18]], read<i32>(%[[VALUE54]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(8)>(field0(deref(read<ptr<@type[[TYPE_S8]]>>(%[[VALUE_p_18]])))), read<i32>(%[[VALUE_j_18]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_18]]), read<i32>(%[[VALUE_j_18]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test8:[0-9]+]] @test8(%[[VALUE_s1_17:[0-9]+]] s1: @type[[TYPE_S8]], %[[VALUE_s2_17:[0-9]+]] s2: @type[[TYPE_S8]], %[[VALUE_s3_9:[0-9]+]] s3: @type[[TYPE_S8]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S8]]>, i32) -> void>(%[[VALUE_check8]], addr_of<ptr<@type[[TYPE_S8]]>>(%[[VALUE_s1_17]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S8]]>, i32) -> void>(%[[VALUE_check8]], addr_of<ptr<@type[[TYPE_S8]]>>(%[[VALUE_s2_17]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S8]]>, i32) -> void>(%[[VALUE_check8]], addr_of<ptr<@type[[TYPE_S8]]>>(%[[VALUE_s3_9]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_8:[0-9]+]] @test2_8(%[[VALUE_s1_18:[0-9]+]] s1: @type[[TYPE_S8]], %[[VALUE_s2_18:[0-9]+]] s2: @type[[TYPE_S8]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S8]], @type[[TYPE_S8]], @type[[TYPE_S8]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test8]], copy<@type[[TYPE_S8]], reason=arg>(read<@type[[TYPE_S8]]>(%[[VALUE_s1_18]])), copy<@type[[TYPE_S8]], reason=arg>(read<@type[[TYPE_S8]]>(%[[VALUE_g2s8]])), copy<@type[[TYPE_S8]], reason=arg>(read<@type[[TYPE_S8]]>(%[[VALUE_s2_18]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit8:[0-9]+]] @testit8() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S8]]>, i32) -> void>(%[[VALUE_init8]], addr_of<ptr<@type[[TYPE_S8]]>>(%[[VALUE_g1s8]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S8]]>, i32) -> void>(%[[VALUE_check8]], addr_of<ptr<@type[[TYPE_S8]]>>(%[[VALUE_g1s8]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S8]]>, i32) -> void>(%[[VALUE_init8]], addr_of<ptr<@type[[TYPE_S8]]>>(%[[VALUE_g2s8]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S8]]>, i32) -> void>(%[[VALUE_check8]], addr_of<ptr<@type[[TYPE_S8]]>>(%[[VALUE_g2s8]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S8]]>, i32) -> void>(%[[VALUE_init8]], addr_of<ptr<@type[[TYPE_S8]]>>(%[[VALUE_g3s8]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S8]]>, i32) -> void>(%[[VALUE_check8]], addr_of<ptr<@type[[TYPE_S8]]>>(%[[VALUE_g3s8]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S8]], @type[[TYPE_S8]], @type[[TYPE_S8]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test8]], copy<@type[[TYPE_S8]], reason=arg>(read<@type[[TYPE_S8]]>(%[[VALUE_g1s8]])), copy<@type[[TYPE_S8]], reason=arg>(read<@type[[TYPE_S8]]>(%[[VALUE_g2s8]])), copy<@type[[TYPE_S8]], reason=arg>(read<@type[[TYPE_S8]]>(%[[VALUE_g3s8]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S8]], @type[[TYPE_S8]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_8]], copy<@type[[TYPE_S8]], reason=arg>(read<@type[[TYPE_S8]]>(%[[VALUE_g1s8]])), copy<@type[[TYPE_S8]], reason=arg>(read<@type[[TYPE_S8]]>(%[[VALUE_g3s8]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init9:[0-9]+]] @init9(%[[VALUE_p_19:[0-9]+]] p: ptr<@type[[TYPE_S9]]>, %[[VALUE_i_19:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_19:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE55:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_19]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_19]]), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE56:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_19]]);
// DEFAULT-NEXT:                 let %[[VALUE57:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE56]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_19]], read<i32>(%[[VALUE57]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(9)>(field0(deref(read<ptr<@type[[TYPE_S9]]>>(%[[VALUE_p_19]])))), read<i32>(%[[VALUE_j_19]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_19]]), read<i32>(%[[VALUE_j_19]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check9:[0-9]+]] @check9(%[[VALUE_p_20:[0-9]+]] p: ptr<@type[[TYPE_S9]]>, %[[VALUE_i_20:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_20:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE58:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_20]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_20]]), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE59:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_20]]);
// DEFAULT-NEXT:                 let %[[VALUE60:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE59]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_20]], read<i32>(%[[VALUE60]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(9)>(field0(deref(read<ptr<@type[[TYPE_S9]]>>(%[[VALUE_p_20]])))), read<i32>(%[[VALUE_j_20]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_20]]), read<i32>(%[[VALUE_j_20]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test9:[0-9]+]] @test9(%[[VALUE_s1_19:[0-9]+]] s1: @type[[TYPE_S9]], %[[VALUE_s2_19:[0-9]+]] s2: @type[[TYPE_S9]], %[[VALUE_s3_10:[0-9]+]] s3: @type[[TYPE_S9]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S9]]>, i32) -> void>(%[[VALUE_check9]], addr_of<ptr<@type[[TYPE_S9]]>>(%[[VALUE_s1_19]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S9]]>, i32) -> void>(%[[VALUE_check9]], addr_of<ptr<@type[[TYPE_S9]]>>(%[[VALUE_s2_19]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S9]]>, i32) -> void>(%[[VALUE_check9]], addr_of<ptr<@type[[TYPE_S9]]>>(%[[VALUE_s3_10]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_9:[0-9]+]] @test2_9(%[[VALUE_s1_20:[0-9]+]] s1: @type[[TYPE_S9]], %[[VALUE_s2_20:[0-9]+]] s2: @type[[TYPE_S9]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S9]], @type[[TYPE_S9]], @type[[TYPE_S9]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test9]], copy<@type[[TYPE_S9]], reason=arg>(read<@type[[TYPE_S9]]>(%[[VALUE_s1_20]])), copy<@type[[TYPE_S9]], reason=arg>(read<@type[[TYPE_S9]]>(%[[VALUE_g2s9]])), copy<@type[[TYPE_S9]], reason=arg>(read<@type[[TYPE_S9]]>(%[[VALUE_s2_20]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit9:[0-9]+]] @testit9() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S9]]>, i32) -> void>(%[[VALUE_init9]], addr_of<ptr<@type[[TYPE_S9]]>>(%[[VALUE_g1s9]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S9]]>, i32) -> void>(%[[VALUE_check9]], addr_of<ptr<@type[[TYPE_S9]]>>(%[[VALUE_g1s9]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S9]]>, i32) -> void>(%[[VALUE_init9]], addr_of<ptr<@type[[TYPE_S9]]>>(%[[VALUE_g2s9]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S9]]>, i32) -> void>(%[[VALUE_check9]], addr_of<ptr<@type[[TYPE_S9]]>>(%[[VALUE_g2s9]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S9]]>, i32) -> void>(%[[VALUE_init9]], addr_of<ptr<@type[[TYPE_S9]]>>(%[[VALUE_g3s9]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S9]]>, i32) -> void>(%[[VALUE_check9]], addr_of<ptr<@type[[TYPE_S9]]>>(%[[VALUE_g3s9]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S9]], @type[[TYPE_S9]], @type[[TYPE_S9]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test9]], copy<@type[[TYPE_S9]], reason=arg>(read<@type[[TYPE_S9]]>(%[[VALUE_g1s9]])), copy<@type[[TYPE_S9]], reason=arg>(read<@type[[TYPE_S9]]>(%[[VALUE_g2s9]])), copy<@type[[TYPE_S9]], reason=arg>(read<@type[[TYPE_S9]]>(%[[VALUE_g3s9]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S9]], @type[[TYPE_S9]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_9]], copy<@type[[TYPE_S9]], reason=arg>(read<@type[[TYPE_S9]]>(%[[VALUE_g1s9]])), copy<@type[[TYPE_S9]], reason=arg>(read<@type[[TYPE_S9]]>(%[[VALUE_g3s9]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init10:[0-9]+]] @init10(%[[VALUE_p_21:[0-9]+]] p: ptr<@type[[TYPE_S10]]>, %[[VALUE_i_21:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_21:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE61:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_21]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_21]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE62:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_21]]);
// DEFAULT-NEXT:                 let %[[VALUE63:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE62]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_21]], read<i32>(%[[VALUE63]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(10)>(field0(deref(read<ptr<@type[[TYPE_S10]]>>(%[[VALUE_p_21]])))), read<i32>(%[[VALUE_j_21]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_21]]), read<i32>(%[[VALUE_j_21]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check10:[0-9]+]] @check10(%[[VALUE_p_22:[0-9]+]] p: ptr<@type[[TYPE_S10]]>, %[[VALUE_i_22:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_22:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE64:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_22]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_22]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE65:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_22]]);
// DEFAULT-NEXT:                 let %[[VALUE66:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE65]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_22]], read<i32>(%[[VALUE66]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(10)>(field0(deref(read<ptr<@type[[TYPE_S10]]>>(%[[VALUE_p_22]])))), read<i32>(%[[VALUE_j_22]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_22]]), read<i32>(%[[VALUE_j_22]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test10:[0-9]+]] @test10(%[[VALUE_s1_21:[0-9]+]] s1: @type[[TYPE_S10]], %[[VALUE_s2_21:[0-9]+]] s2: @type[[TYPE_S10]], %[[VALUE_s3_11:[0-9]+]] s3: @type[[TYPE_S10]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S10]]>, i32) -> void>(%[[VALUE_check10]], addr_of<ptr<@type[[TYPE_S10]]>>(%[[VALUE_s1_21]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S10]]>, i32) -> void>(%[[VALUE_check10]], addr_of<ptr<@type[[TYPE_S10]]>>(%[[VALUE_s2_21]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S10]]>, i32) -> void>(%[[VALUE_check10]], addr_of<ptr<@type[[TYPE_S10]]>>(%[[VALUE_s3_11]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_10:[0-9]+]] @test2_10(%[[VALUE_s1_22:[0-9]+]] s1: @type[[TYPE_S10]], %[[VALUE_s2_22:[0-9]+]] s2: @type[[TYPE_S10]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S10]], @type[[TYPE_S10]], @type[[TYPE_S10]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test10]], copy<@type[[TYPE_S10]], reason=arg>(read<@type[[TYPE_S10]]>(%[[VALUE_s1_22]])), copy<@type[[TYPE_S10]], reason=arg>(read<@type[[TYPE_S10]]>(%[[VALUE_g2s10]])), copy<@type[[TYPE_S10]], reason=arg>(read<@type[[TYPE_S10]]>(%[[VALUE_s2_22]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit10:[0-9]+]] @testit10() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S10]]>, i32) -> void>(%[[VALUE_init10]], addr_of<ptr<@type[[TYPE_S10]]>>(%[[VALUE_g1s10]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S10]]>, i32) -> void>(%[[VALUE_check10]], addr_of<ptr<@type[[TYPE_S10]]>>(%[[VALUE_g1s10]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S10]]>, i32) -> void>(%[[VALUE_init10]], addr_of<ptr<@type[[TYPE_S10]]>>(%[[VALUE_g2s10]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S10]]>, i32) -> void>(%[[VALUE_check10]], addr_of<ptr<@type[[TYPE_S10]]>>(%[[VALUE_g2s10]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S10]]>, i32) -> void>(%[[VALUE_init10]], addr_of<ptr<@type[[TYPE_S10]]>>(%[[VALUE_g3s10]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S10]]>, i32) -> void>(%[[VALUE_check10]], addr_of<ptr<@type[[TYPE_S10]]>>(%[[VALUE_g3s10]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S10]], @type[[TYPE_S10]], @type[[TYPE_S10]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test10]], copy<@type[[TYPE_S10]], reason=arg>(read<@type[[TYPE_S10]]>(%[[VALUE_g1s10]])), copy<@type[[TYPE_S10]], reason=arg>(read<@type[[TYPE_S10]]>(%[[VALUE_g2s10]])), copy<@type[[TYPE_S10]], reason=arg>(read<@type[[TYPE_S10]]>(%[[VALUE_g3s10]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S10]], @type[[TYPE_S10]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_10]], copy<@type[[TYPE_S10]], reason=arg>(read<@type[[TYPE_S10]]>(%[[VALUE_g1s10]])), copy<@type[[TYPE_S10]], reason=arg>(read<@type[[TYPE_S10]]>(%[[VALUE_g3s10]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init11:[0-9]+]] @init11(%[[VALUE_p_23:[0-9]+]] p: ptr<@type[[TYPE_S11]]>, %[[VALUE_i_23:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_23:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE67:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_23]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_23]]), const<i32>(11))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE68:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_23]]);
// DEFAULT-NEXT:                 let %[[VALUE69:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE68]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_23]], read<i32>(%[[VALUE69]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(11)>(field0(deref(read<ptr<@type[[TYPE_S11]]>>(%[[VALUE_p_23]])))), read<i32>(%[[VALUE_j_23]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_23]]), read<i32>(%[[VALUE_j_23]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check11:[0-9]+]] @check11(%[[VALUE_p_24:[0-9]+]] p: ptr<@type[[TYPE_S11]]>, %[[VALUE_i_24:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_24:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE70:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_24]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_24]]), const<i32>(11))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE71:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_24]]);
// DEFAULT-NEXT:                 let %[[VALUE72:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE71]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_24]], read<i32>(%[[VALUE72]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(11)>(field0(deref(read<ptr<@type[[TYPE_S11]]>>(%[[VALUE_p_24]])))), read<i32>(%[[VALUE_j_24]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_24]]), read<i32>(%[[VALUE_j_24]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test11:[0-9]+]] @test11(%[[VALUE_s1_23:[0-9]+]] s1: @type[[TYPE_S11]], %[[VALUE_s2_23:[0-9]+]] s2: @type[[TYPE_S11]], %[[VALUE_s3_12:[0-9]+]] s3: @type[[TYPE_S11]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S11]]>, i32) -> void>(%[[VALUE_check11]], addr_of<ptr<@type[[TYPE_S11]]>>(%[[VALUE_s1_23]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S11]]>, i32) -> void>(%[[VALUE_check11]], addr_of<ptr<@type[[TYPE_S11]]>>(%[[VALUE_s2_23]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S11]]>, i32) -> void>(%[[VALUE_check11]], addr_of<ptr<@type[[TYPE_S11]]>>(%[[VALUE_s3_12]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_11:[0-9]+]] @test2_11(%[[VALUE_s1_24:[0-9]+]] s1: @type[[TYPE_S11]], %[[VALUE_s2_24:[0-9]+]] s2: @type[[TYPE_S11]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S11]], @type[[TYPE_S11]], @type[[TYPE_S11]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test11]], copy<@type[[TYPE_S11]], reason=arg>(read<@type[[TYPE_S11]]>(%[[VALUE_s1_24]])), copy<@type[[TYPE_S11]], reason=arg>(read<@type[[TYPE_S11]]>(%[[VALUE_g2s11]])), copy<@type[[TYPE_S11]], reason=arg>(read<@type[[TYPE_S11]]>(%[[VALUE_s2_24]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit11:[0-9]+]] @testit11() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S11]]>, i32) -> void>(%[[VALUE_init11]], addr_of<ptr<@type[[TYPE_S11]]>>(%[[VALUE_g1s11]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S11]]>, i32) -> void>(%[[VALUE_check11]], addr_of<ptr<@type[[TYPE_S11]]>>(%[[VALUE_g1s11]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S11]]>, i32) -> void>(%[[VALUE_init11]], addr_of<ptr<@type[[TYPE_S11]]>>(%[[VALUE_g2s11]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S11]]>, i32) -> void>(%[[VALUE_check11]], addr_of<ptr<@type[[TYPE_S11]]>>(%[[VALUE_g2s11]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S11]]>, i32) -> void>(%[[VALUE_init11]], addr_of<ptr<@type[[TYPE_S11]]>>(%[[VALUE_g3s11]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S11]]>, i32) -> void>(%[[VALUE_check11]], addr_of<ptr<@type[[TYPE_S11]]>>(%[[VALUE_g3s11]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S11]], @type[[TYPE_S11]], @type[[TYPE_S11]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test11]], copy<@type[[TYPE_S11]], reason=arg>(read<@type[[TYPE_S11]]>(%[[VALUE_g1s11]])), copy<@type[[TYPE_S11]], reason=arg>(read<@type[[TYPE_S11]]>(%[[VALUE_g2s11]])), copy<@type[[TYPE_S11]], reason=arg>(read<@type[[TYPE_S11]]>(%[[VALUE_g3s11]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S11]], @type[[TYPE_S11]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_11]], copy<@type[[TYPE_S11]], reason=arg>(read<@type[[TYPE_S11]]>(%[[VALUE_g1s11]])), copy<@type[[TYPE_S11]], reason=arg>(read<@type[[TYPE_S11]]>(%[[VALUE_g3s11]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init12:[0-9]+]] @init12(%[[VALUE_p_25:[0-9]+]] p: ptr<@type[[TYPE_S12]]>, %[[VALUE_i_25:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_25:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE73:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_25]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_25]]), const<i32>(12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE74:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_25]]);
// DEFAULT-NEXT:                 let %[[VALUE75:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE74]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_25]], read<i32>(%[[VALUE75]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(12)>(field0(deref(read<ptr<@type[[TYPE_S12]]>>(%[[VALUE_p_25]])))), read<i32>(%[[VALUE_j_25]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_25]]), read<i32>(%[[VALUE_j_25]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check12:[0-9]+]] @check12(%[[VALUE_p_26:[0-9]+]] p: ptr<@type[[TYPE_S12]]>, %[[VALUE_i_26:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_26:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE76:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_26]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_26]]), const<i32>(12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE77:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_26]]);
// DEFAULT-NEXT:                 let %[[VALUE78:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE77]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_26]], read<i32>(%[[VALUE78]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(12)>(field0(deref(read<ptr<@type[[TYPE_S12]]>>(%[[VALUE_p_26]])))), read<i32>(%[[VALUE_j_26]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_26]]), read<i32>(%[[VALUE_j_26]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test12:[0-9]+]] @test12(%[[VALUE_s1_25:[0-9]+]] s1: @type[[TYPE_S12]], %[[VALUE_s2_25:[0-9]+]] s2: @type[[TYPE_S12]], %[[VALUE_s3_13:[0-9]+]] s3: @type[[TYPE_S12]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S12]]>, i32) -> void>(%[[VALUE_check12]], addr_of<ptr<@type[[TYPE_S12]]>>(%[[VALUE_s1_25]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S12]]>, i32) -> void>(%[[VALUE_check12]], addr_of<ptr<@type[[TYPE_S12]]>>(%[[VALUE_s2_25]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S12]]>, i32) -> void>(%[[VALUE_check12]], addr_of<ptr<@type[[TYPE_S12]]>>(%[[VALUE_s3_13]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_12:[0-9]+]] @test2_12(%[[VALUE_s1_26:[0-9]+]] s1: @type[[TYPE_S12]], %[[VALUE_s2_26:[0-9]+]] s2: @type[[TYPE_S12]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S12]], @type[[TYPE_S12]], @type[[TYPE_S12]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test12]], copy<@type[[TYPE_S12]], reason=arg>(read<@type[[TYPE_S12]]>(%[[VALUE_s1_26]])), copy<@type[[TYPE_S12]], reason=arg>(read<@type[[TYPE_S12]]>(%[[VALUE_g2s12]])), copy<@type[[TYPE_S12]], reason=arg>(read<@type[[TYPE_S12]]>(%[[VALUE_s2_26]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit12:[0-9]+]] @testit12() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S12]]>, i32) -> void>(%[[VALUE_init12]], addr_of<ptr<@type[[TYPE_S12]]>>(%[[VALUE_g1s12]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S12]]>, i32) -> void>(%[[VALUE_check12]], addr_of<ptr<@type[[TYPE_S12]]>>(%[[VALUE_g1s12]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S12]]>, i32) -> void>(%[[VALUE_init12]], addr_of<ptr<@type[[TYPE_S12]]>>(%[[VALUE_g2s12]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S12]]>, i32) -> void>(%[[VALUE_check12]], addr_of<ptr<@type[[TYPE_S12]]>>(%[[VALUE_g2s12]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S12]]>, i32) -> void>(%[[VALUE_init12]], addr_of<ptr<@type[[TYPE_S12]]>>(%[[VALUE_g3s12]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S12]]>, i32) -> void>(%[[VALUE_check12]], addr_of<ptr<@type[[TYPE_S12]]>>(%[[VALUE_g3s12]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S12]], @type[[TYPE_S12]], @type[[TYPE_S12]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test12]], copy<@type[[TYPE_S12]], reason=arg>(read<@type[[TYPE_S12]]>(%[[VALUE_g1s12]])), copy<@type[[TYPE_S12]], reason=arg>(read<@type[[TYPE_S12]]>(%[[VALUE_g2s12]])), copy<@type[[TYPE_S12]], reason=arg>(read<@type[[TYPE_S12]]>(%[[VALUE_g3s12]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S12]], @type[[TYPE_S12]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_12]], copy<@type[[TYPE_S12]], reason=arg>(read<@type[[TYPE_S12]]>(%[[VALUE_g1s12]])), copy<@type[[TYPE_S12]], reason=arg>(read<@type[[TYPE_S12]]>(%[[VALUE_g3s12]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init13:[0-9]+]] @init13(%[[VALUE_p_27:[0-9]+]] p: ptr<@type[[TYPE_S13]]>, %[[VALUE_i_27:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_27:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE79:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_27]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_27]]), const<i32>(13))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE80:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_27]]);
// DEFAULT-NEXT:                 let %[[VALUE81:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE80]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_27]], read<i32>(%[[VALUE81]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(13)>(field0(deref(read<ptr<@type[[TYPE_S13]]>>(%[[VALUE_p_27]])))), read<i32>(%[[VALUE_j_27]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_27]]), read<i32>(%[[VALUE_j_27]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check13:[0-9]+]] @check13(%[[VALUE_p_28:[0-9]+]] p: ptr<@type[[TYPE_S13]]>, %[[VALUE_i_28:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_28:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE82:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_28]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_28]]), const<i32>(13))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE83:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_28]]);
// DEFAULT-NEXT:                 let %[[VALUE84:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE83]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_28]], read<i32>(%[[VALUE84]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(13)>(field0(deref(read<ptr<@type[[TYPE_S13]]>>(%[[VALUE_p_28]])))), read<i32>(%[[VALUE_j_28]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_28]]), read<i32>(%[[VALUE_j_28]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test13:[0-9]+]] @test13(%[[VALUE_s1_27:[0-9]+]] s1: @type[[TYPE_S13]], %[[VALUE_s2_27:[0-9]+]] s2: @type[[TYPE_S13]], %[[VALUE_s3_14:[0-9]+]] s3: @type[[TYPE_S13]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S13]]>, i32) -> void>(%[[VALUE_check13]], addr_of<ptr<@type[[TYPE_S13]]>>(%[[VALUE_s1_27]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S13]]>, i32) -> void>(%[[VALUE_check13]], addr_of<ptr<@type[[TYPE_S13]]>>(%[[VALUE_s2_27]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S13]]>, i32) -> void>(%[[VALUE_check13]], addr_of<ptr<@type[[TYPE_S13]]>>(%[[VALUE_s3_14]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_13:[0-9]+]] @test2_13(%[[VALUE_s1_28:[0-9]+]] s1: @type[[TYPE_S13]], %[[VALUE_s2_28:[0-9]+]] s2: @type[[TYPE_S13]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S13]], @type[[TYPE_S13]], @type[[TYPE_S13]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test13]], copy<@type[[TYPE_S13]], reason=arg>(read<@type[[TYPE_S13]]>(%[[VALUE_s1_28]])), copy<@type[[TYPE_S13]], reason=arg>(read<@type[[TYPE_S13]]>(%[[VALUE_g2s13]])), copy<@type[[TYPE_S13]], reason=arg>(read<@type[[TYPE_S13]]>(%[[VALUE_s2_28]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit13:[0-9]+]] @testit13() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S13]]>, i32) -> void>(%[[VALUE_init13]], addr_of<ptr<@type[[TYPE_S13]]>>(%[[VALUE_g1s13]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S13]]>, i32) -> void>(%[[VALUE_check13]], addr_of<ptr<@type[[TYPE_S13]]>>(%[[VALUE_g1s13]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S13]]>, i32) -> void>(%[[VALUE_init13]], addr_of<ptr<@type[[TYPE_S13]]>>(%[[VALUE_g2s13]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S13]]>, i32) -> void>(%[[VALUE_check13]], addr_of<ptr<@type[[TYPE_S13]]>>(%[[VALUE_g2s13]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S13]]>, i32) -> void>(%[[VALUE_init13]], addr_of<ptr<@type[[TYPE_S13]]>>(%[[VALUE_g3s13]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S13]]>, i32) -> void>(%[[VALUE_check13]], addr_of<ptr<@type[[TYPE_S13]]>>(%[[VALUE_g3s13]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S13]], @type[[TYPE_S13]], @type[[TYPE_S13]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test13]], copy<@type[[TYPE_S13]], reason=arg>(read<@type[[TYPE_S13]]>(%[[VALUE_g1s13]])), copy<@type[[TYPE_S13]], reason=arg>(read<@type[[TYPE_S13]]>(%[[VALUE_g2s13]])), copy<@type[[TYPE_S13]], reason=arg>(read<@type[[TYPE_S13]]>(%[[VALUE_g3s13]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S13]], @type[[TYPE_S13]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_13]], copy<@type[[TYPE_S13]], reason=arg>(read<@type[[TYPE_S13]]>(%[[VALUE_g1s13]])), copy<@type[[TYPE_S13]], reason=arg>(read<@type[[TYPE_S13]]>(%[[VALUE_g3s13]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init14:[0-9]+]] @init14(%[[VALUE_p_29:[0-9]+]] p: ptr<@type[[TYPE_S14]]>, %[[VALUE_i_29:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_29:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE85:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_29]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_29]]), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE86:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_29]]);
// DEFAULT-NEXT:                 let %[[VALUE87:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE86]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_29]], read<i32>(%[[VALUE87]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(14)>(field0(deref(read<ptr<@type[[TYPE_S14]]>>(%[[VALUE_p_29]])))), read<i32>(%[[VALUE_j_29]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_29]]), read<i32>(%[[VALUE_j_29]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check14:[0-9]+]] @check14(%[[VALUE_p_30:[0-9]+]] p: ptr<@type[[TYPE_S14]]>, %[[VALUE_i_30:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_30:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE88:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_30]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_30]]), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE89:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_30]]);
// DEFAULT-NEXT:                 let %[[VALUE90:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE89]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_30]], read<i32>(%[[VALUE90]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(14)>(field0(deref(read<ptr<@type[[TYPE_S14]]>>(%[[VALUE_p_30]])))), read<i32>(%[[VALUE_j_30]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_30]]), read<i32>(%[[VALUE_j_30]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test14:[0-9]+]] @test14(%[[VALUE_s1_29:[0-9]+]] s1: @type[[TYPE_S14]], %[[VALUE_s2_29:[0-9]+]] s2: @type[[TYPE_S14]], %[[VALUE_s3_15:[0-9]+]] s3: @type[[TYPE_S14]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S14]]>, i32) -> void>(%[[VALUE_check14]], addr_of<ptr<@type[[TYPE_S14]]>>(%[[VALUE_s1_29]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S14]]>, i32) -> void>(%[[VALUE_check14]], addr_of<ptr<@type[[TYPE_S14]]>>(%[[VALUE_s2_29]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S14]]>, i32) -> void>(%[[VALUE_check14]], addr_of<ptr<@type[[TYPE_S14]]>>(%[[VALUE_s3_15]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_14:[0-9]+]] @test2_14(%[[VALUE_s1_30:[0-9]+]] s1: @type[[TYPE_S14]], %[[VALUE_s2_30:[0-9]+]] s2: @type[[TYPE_S14]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S14]], @type[[TYPE_S14]], @type[[TYPE_S14]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test14]], copy<@type[[TYPE_S14]], reason=arg>(read<@type[[TYPE_S14]]>(%[[VALUE_s1_30]])), copy<@type[[TYPE_S14]], reason=arg>(read<@type[[TYPE_S14]]>(%[[VALUE_g2s14]])), copy<@type[[TYPE_S14]], reason=arg>(read<@type[[TYPE_S14]]>(%[[VALUE_s2_30]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit14:[0-9]+]] @testit14() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S14]]>, i32) -> void>(%[[VALUE_init14]], addr_of<ptr<@type[[TYPE_S14]]>>(%[[VALUE_g1s14]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S14]]>, i32) -> void>(%[[VALUE_check14]], addr_of<ptr<@type[[TYPE_S14]]>>(%[[VALUE_g1s14]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S14]]>, i32) -> void>(%[[VALUE_init14]], addr_of<ptr<@type[[TYPE_S14]]>>(%[[VALUE_g2s14]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S14]]>, i32) -> void>(%[[VALUE_check14]], addr_of<ptr<@type[[TYPE_S14]]>>(%[[VALUE_g2s14]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S14]]>, i32) -> void>(%[[VALUE_init14]], addr_of<ptr<@type[[TYPE_S14]]>>(%[[VALUE_g3s14]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S14]]>, i32) -> void>(%[[VALUE_check14]], addr_of<ptr<@type[[TYPE_S14]]>>(%[[VALUE_g3s14]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S14]], @type[[TYPE_S14]], @type[[TYPE_S14]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test14]], copy<@type[[TYPE_S14]], reason=arg>(read<@type[[TYPE_S14]]>(%[[VALUE_g1s14]])), copy<@type[[TYPE_S14]], reason=arg>(read<@type[[TYPE_S14]]>(%[[VALUE_g2s14]])), copy<@type[[TYPE_S14]], reason=arg>(read<@type[[TYPE_S14]]>(%[[VALUE_g3s14]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S14]], @type[[TYPE_S14]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_14]], copy<@type[[TYPE_S14]], reason=arg>(read<@type[[TYPE_S14]]>(%[[VALUE_g1s14]])), copy<@type[[TYPE_S14]], reason=arg>(read<@type[[TYPE_S14]]>(%[[VALUE_g3s14]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init15:[0-9]+]] @init15(%[[VALUE_p_31:[0-9]+]] p: ptr<@type[[TYPE_S15]]>, %[[VALUE_i_31:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_31:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE91:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_31]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_31]]), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE92:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_31]]);
// DEFAULT-NEXT:                 let %[[VALUE93:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE92]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_31]], read<i32>(%[[VALUE93]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(15)>(field0(deref(read<ptr<@type[[TYPE_S15]]>>(%[[VALUE_p_31]])))), read<i32>(%[[VALUE_j_31]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_31]]), read<i32>(%[[VALUE_j_31]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check15:[0-9]+]] @check15(%[[VALUE_p_32:[0-9]+]] p: ptr<@type[[TYPE_S15]]>, %[[VALUE_i_32:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_32:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE94:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_32]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_32]]), const<i32>(15))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE95:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_32]]);
// DEFAULT-NEXT:                 let %[[VALUE96:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE95]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_32]], read<i32>(%[[VALUE96]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(15)>(field0(deref(read<ptr<@type[[TYPE_S15]]>>(%[[VALUE_p_32]])))), read<i32>(%[[VALUE_j_32]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_32]]), read<i32>(%[[VALUE_j_32]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test15:[0-9]+]] @test15(%[[VALUE_s1_31:[0-9]+]] s1: @type[[TYPE_S15]], %[[VALUE_s2_31:[0-9]+]] s2: @type[[TYPE_S15]], %[[VALUE_s3_16:[0-9]+]] s3: @type[[TYPE_S15]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S15]]>, i32) -> void>(%[[VALUE_check15]], addr_of<ptr<@type[[TYPE_S15]]>>(%[[VALUE_s1_31]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S15]]>, i32) -> void>(%[[VALUE_check15]], addr_of<ptr<@type[[TYPE_S15]]>>(%[[VALUE_s2_31]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S15]]>, i32) -> void>(%[[VALUE_check15]], addr_of<ptr<@type[[TYPE_S15]]>>(%[[VALUE_s3_16]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_15:[0-9]+]] @test2_15(%[[VALUE_s1_32:[0-9]+]] s1: @type[[TYPE_S15]], %[[VALUE_s2_32:[0-9]+]] s2: @type[[TYPE_S15]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S15]], @type[[TYPE_S15]], @type[[TYPE_S15]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test15]], copy<@type[[TYPE_S15]], reason=arg>(read<@type[[TYPE_S15]]>(%[[VALUE_s1_32]])), copy<@type[[TYPE_S15]], reason=arg>(read<@type[[TYPE_S15]]>(%[[VALUE_g2s15]])), copy<@type[[TYPE_S15]], reason=arg>(read<@type[[TYPE_S15]]>(%[[VALUE_s2_32]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit15:[0-9]+]] @testit15() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S15]]>, i32) -> void>(%[[VALUE_init15]], addr_of<ptr<@type[[TYPE_S15]]>>(%[[VALUE_g1s15]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S15]]>, i32) -> void>(%[[VALUE_check15]], addr_of<ptr<@type[[TYPE_S15]]>>(%[[VALUE_g1s15]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S15]]>, i32) -> void>(%[[VALUE_init15]], addr_of<ptr<@type[[TYPE_S15]]>>(%[[VALUE_g2s15]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S15]]>, i32) -> void>(%[[VALUE_check15]], addr_of<ptr<@type[[TYPE_S15]]>>(%[[VALUE_g2s15]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S15]]>, i32) -> void>(%[[VALUE_init15]], addr_of<ptr<@type[[TYPE_S15]]>>(%[[VALUE_g3s15]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S15]]>, i32) -> void>(%[[VALUE_check15]], addr_of<ptr<@type[[TYPE_S15]]>>(%[[VALUE_g3s15]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S15]], @type[[TYPE_S15]], @type[[TYPE_S15]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test15]], copy<@type[[TYPE_S15]], reason=arg>(read<@type[[TYPE_S15]]>(%[[VALUE_g1s15]])), copy<@type[[TYPE_S15]], reason=arg>(read<@type[[TYPE_S15]]>(%[[VALUE_g2s15]])), copy<@type[[TYPE_S15]], reason=arg>(read<@type[[TYPE_S15]]>(%[[VALUE_g3s15]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S15]], @type[[TYPE_S15]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_15]], copy<@type[[TYPE_S15]], reason=arg>(read<@type[[TYPE_S15]]>(%[[VALUE_g1s15]])), copy<@type[[TYPE_S15]], reason=arg>(read<@type[[TYPE_S15]]>(%[[VALUE_g3s15]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init16:[0-9]+]] @init16(%[[VALUE_p_33:[0-9]+]] p: ptr<@type[[TYPE_S16]]>, %[[VALUE_i_33:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_33:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE97:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_33]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_33]]), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE98:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_33]]);
// DEFAULT-NEXT:                 let %[[VALUE99:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE98]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_33]], read<i32>(%[[VALUE99]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(16)>(field0(deref(read<ptr<@type[[TYPE_S16]]>>(%[[VALUE_p_33]])))), read<i32>(%[[VALUE_j_33]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_33]]), read<i32>(%[[VALUE_j_33]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check16:[0-9]+]] @check16(%[[VALUE_p_34:[0-9]+]] p: ptr<@type[[TYPE_S16]]>, %[[VALUE_i_34:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_34:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE100:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_34]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_34]]), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE101:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_34]]);
// DEFAULT-NEXT:                 let %[[VALUE102:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE101]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_34]], read<i32>(%[[VALUE102]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(16)>(field0(deref(read<ptr<@type[[TYPE_S16]]>>(%[[VALUE_p_34]])))), read<i32>(%[[VALUE_j_34]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_34]]), read<i32>(%[[VALUE_j_34]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test16:[0-9]+]] @test16(%[[VALUE_s1_33:[0-9]+]] s1: @type[[TYPE_S16]], %[[VALUE_s2_33:[0-9]+]] s2: @type[[TYPE_S16]], %[[VALUE_s3_17:[0-9]+]] s3: @type[[TYPE_S16]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S16]]>, i32) -> void>(%[[VALUE_check16]], addr_of<ptr<@type[[TYPE_S16]]>>(%[[VALUE_s1_33]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S16]]>, i32) -> void>(%[[VALUE_check16]], addr_of<ptr<@type[[TYPE_S16]]>>(%[[VALUE_s2_33]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S16]]>, i32) -> void>(%[[VALUE_check16]], addr_of<ptr<@type[[TYPE_S16]]>>(%[[VALUE_s3_17]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_16:[0-9]+]] @test2_16(%[[VALUE_s1_34:[0-9]+]] s1: @type[[TYPE_S16]], %[[VALUE_s2_34:[0-9]+]] s2: @type[[TYPE_S16]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S16]], @type[[TYPE_S16]], @type[[TYPE_S16]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test16]], copy<@type[[TYPE_S16]], reason=arg>(read<@type[[TYPE_S16]]>(%[[VALUE_s1_34]])), copy<@type[[TYPE_S16]], reason=arg>(read<@type[[TYPE_S16]]>(%[[VALUE_g2s16]])), copy<@type[[TYPE_S16]], reason=arg>(read<@type[[TYPE_S16]]>(%[[VALUE_s2_34]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit16:[0-9]+]] @testit16() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S16]]>, i32) -> void>(%[[VALUE_init16]], addr_of<ptr<@type[[TYPE_S16]]>>(%[[VALUE_g1s16]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S16]]>, i32) -> void>(%[[VALUE_check16]], addr_of<ptr<@type[[TYPE_S16]]>>(%[[VALUE_g1s16]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S16]]>, i32) -> void>(%[[VALUE_init16]], addr_of<ptr<@type[[TYPE_S16]]>>(%[[VALUE_g2s16]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S16]]>, i32) -> void>(%[[VALUE_check16]], addr_of<ptr<@type[[TYPE_S16]]>>(%[[VALUE_g2s16]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S16]]>, i32) -> void>(%[[VALUE_init16]], addr_of<ptr<@type[[TYPE_S16]]>>(%[[VALUE_g3s16]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S16]]>, i32) -> void>(%[[VALUE_check16]], addr_of<ptr<@type[[TYPE_S16]]>>(%[[VALUE_g3s16]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S16]], @type[[TYPE_S16]], @type[[TYPE_S16]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test16]], copy<@type[[TYPE_S16]], reason=arg>(read<@type[[TYPE_S16]]>(%[[VALUE_g1s16]])), copy<@type[[TYPE_S16]], reason=arg>(read<@type[[TYPE_S16]]>(%[[VALUE_g2s16]])), copy<@type[[TYPE_S16]], reason=arg>(read<@type[[TYPE_S16]]>(%[[VALUE_g3s16]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S16]], @type[[TYPE_S16]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_16]], copy<@type[[TYPE_S16]], reason=arg>(read<@type[[TYPE_S16]]>(%[[VALUE_g1s16]])), copy<@type[[TYPE_S16]], reason=arg>(read<@type[[TYPE_S16]]>(%[[VALUE_g3s16]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init17:[0-9]+]] @init17(%[[VALUE_p_35:[0-9]+]] p: ptr<@type[[TYPE_S17]]>, %[[VALUE_i_35:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_35:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE103:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_35]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_35]]), const<i32>(17))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE104:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_35]]);
// DEFAULT-NEXT:                 let %[[VALUE105:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE104]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_35]], read<i32>(%[[VALUE105]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(17)>(field0(deref(read<ptr<@type[[TYPE_S17]]>>(%[[VALUE_p_35]])))), read<i32>(%[[VALUE_j_35]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_35]]), read<i32>(%[[VALUE_j_35]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check17:[0-9]+]] @check17(%[[VALUE_p_36:[0-9]+]] p: ptr<@type[[TYPE_S17]]>, %[[VALUE_i_36:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_36:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE106:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_36]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_36]]), const<i32>(17))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE107:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_36]]);
// DEFAULT-NEXT:                 let %[[VALUE108:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE107]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_36]], read<i32>(%[[VALUE108]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(17)>(field0(deref(read<ptr<@type[[TYPE_S17]]>>(%[[VALUE_p_36]])))), read<i32>(%[[VALUE_j_36]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_36]]), read<i32>(%[[VALUE_j_36]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test17:[0-9]+]] @test17(%[[VALUE_s1_35:[0-9]+]] s1: @type[[TYPE_S17]], %[[VALUE_s2_35:[0-9]+]] s2: @type[[TYPE_S17]], %[[VALUE_s3_18:[0-9]+]] s3: @type[[TYPE_S17]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S17]]>, i32) -> void>(%[[VALUE_check17]], addr_of<ptr<@type[[TYPE_S17]]>>(%[[VALUE_s1_35]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S17]]>, i32) -> void>(%[[VALUE_check17]], addr_of<ptr<@type[[TYPE_S17]]>>(%[[VALUE_s2_35]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S17]]>, i32) -> void>(%[[VALUE_check17]], addr_of<ptr<@type[[TYPE_S17]]>>(%[[VALUE_s3_18]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_17:[0-9]+]] @test2_17(%[[VALUE_s1_36:[0-9]+]] s1: @type[[TYPE_S17]], %[[VALUE_s2_36:[0-9]+]] s2: @type[[TYPE_S17]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S17]], @type[[TYPE_S17]], @type[[TYPE_S17]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test17]], copy<@type[[TYPE_S17]], reason=arg>(read<@type[[TYPE_S17]]>(%[[VALUE_s1_36]])), copy<@type[[TYPE_S17]], reason=arg>(read<@type[[TYPE_S17]]>(%[[VALUE_g2s17]])), copy<@type[[TYPE_S17]], reason=arg>(read<@type[[TYPE_S17]]>(%[[VALUE_s2_36]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit17:[0-9]+]] @testit17() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S17]]>, i32) -> void>(%[[VALUE_init17]], addr_of<ptr<@type[[TYPE_S17]]>>(%[[VALUE_g1s17]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S17]]>, i32) -> void>(%[[VALUE_check17]], addr_of<ptr<@type[[TYPE_S17]]>>(%[[VALUE_g1s17]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S17]]>, i32) -> void>(%[[VALUE_init17]], addr_of<ptr<@type[[TYPE_S17]]>>(%[[VALUE_g2s17]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S17]]>, i32) -> void>(%[[VALUE_check17]], addr_of<ptr<@type[[TYPE_S17]]>>(%[[VALUE_g2s17]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S17]]>, i32) -> void>(%[[VALUE_init17]], addr_of<ptr<@type[[TYPE_S17]]>>(%[[VALUE_g3s17]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S17]]>, i32) -> void>(%[[VALUE_check17]], addr_of<ptr<@type[[TYPE_S17]]>>(%[[VALUE_g3s17]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S17]], @type[[TYPE_S17]], @type[[TYPE_S17]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test17]], copy<@type[[TYPE_S17]], reason=arg>(read<@type[[TYPE_S17]]>(%[[VALUE_g1s17]])), copy<@type[[TYPE_S17]], reason=arg>(read<@type[[TYPE_S17]]>(%[[VALUE_g2s17]])), copy<@type[[TYPE_S17]], reason=arg>(read<@type[[TYPE_S17]]>(%[[VALUE_g3s17]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S17]], @type[[TYPE_S17]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_17]], copy<@type[[TYPE_S17]], reason=arg>(read<@type[[TYPE_S17]]>(%[[VALUE_g1s17]])), copy<@type[[TYPE_S17]], reason=arg>(read<@type[[TYPE_S17]]>(%[[VALUE_g3s17]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init18:[0-9]+]] @init18(%[[VALUE_p_37:[0-9]+]] p: ptr<@type[[TYPE_S18]]>, %[[VALUE_i_37:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_37:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE109:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_37]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_37]]), const<i32>(18))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE110:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_37]]);
// DEFAULT-NEXT:                 let %[[VALUE111:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE110]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_37]], read<i32>(%[[VALUE111]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(18)>(field0(deref(read<ptr<@type[[TYPE_S18]]>>(%[[VALUE_p_37]])))), read<i32>(%[[VALUE_j_37]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_37]]), read<i32>(%[[VALUE_j_37]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check18:[0-9]+]] @check18(%[[VALUE_p_38:[0-9]+]] p: ptr<@type[[TYPE_S18]]>, %[[VALUE_i_38:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_38:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE112:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_38]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_38]]), const<i32>(18))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE113:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_38]]);
// DEFAULT-NEXT:                 let %[[VALUE114:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE113]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_38]], read<i32>(%[[VALUE114]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(18)>(field0(deref(read<ptr<@type[[TYPE_S18]]>>(%[[VALUE_p_38]])))), read<i32>(%[[VALUE_j_38]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_38]]), read<i32>(%[[VALUE_j_38]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test18:[0-9]+]] @test18(%[[VALUE_s1_37:[0-9]+]] s1: @type[[TYPE_S18]], %[[VALUE_s2_37:[0-9]+]] s2: @type[[TYPE_S18]], %[[VALUE_s3_19:[0-9]+]] s3: @type[[TYPE_S18]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S18]]>, i32) -> void>(%[[VALUE_check18]], addr_of<ptr<@type[[TYPE_S18]]>>(%[[VALUE_s1_37]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S18]]>, i32) -> void>(%[[VALUE_check18]], addr_of<ptr<@type[[TYPE_S18]]>>(%[[VALUE_s2_37]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S18]]>, i32) -> void>(%[[VALUE_check18]], addr_of<ptr<@type[[TYPE_S18]]>>(%[[VALUE_s3_19]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_18:[0-9]+]] @test2_18(%[[VALUE_s1_38:[0-9]+]] s1: @type[[TYPE_S18]], %[[VALUE_s2_38:[0-9]+]] s2: @type[[TYPE_S18]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S18]], @type[[TYPE_S18]], @type[[TYPE_S18]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test18]], copy<@type[[TYPE_S18]], reason=arg>(read<@type[[TYPE_S18]]>(%[[VALUE_s1_38]])), copy<@type[[TYPE_S18]], reason=arg>(read<@type[[TYPE_S18]]>(%[[VALUE_g2s18]])), copy<@type[[TYPE_S18]], reason=arg>(read<@type[[TYPE_S18]]>(%[[VALUE_s2_38]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit18:[0-9]+]] @testit18() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S18]]>, i32) -> void>(%[[VALUE_init18]], addr_of<ptr<@type[[TYPE_S18]]>>(%[[VALUE_g1s18]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S18]]>, i32) -> void>(%[[VALUE_check18]], addr_of<ptr<@type[[TYPE_S18]]>>(%[[VALUE_g1s18]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S18]]>, i32) -> void>(%[[VALUE_init18]], addr_of<ptr<@type[[TYPE_S18]]>>(%[[VALUE_g2s18]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S18]]>, i32) -> void>(%[[VALUE_check18]], addr_of<ptr<@type[[TYPE_S18]]>>(%[[VALUE_g2s18]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S18]]>, i32) -> void>(%[[VALUE_init18]], addr_of<ptr<@type[[TYPE_S18]]>>(%[[VALUE_g3s18]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S18]]>, i32) -> void>(%[[VALUE_check18]], addr_of<ptr<@type[[TYPE_S18]]>>(%[[VALUE_g3s18]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S18]], @type[[TYPE_S18]], @type[[TYPE_S18]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test18]], copy<@type[[TYPE_S18]], reason=arg>(read<@type[[TYPE_S18]]>(%[[VALUE_g1s18]])), copy<@type[[TYPE_S18]], reason=arg>(read<@type[[TYPE_S18]]>(%[[VALUE_g2s18]])), copy<@type[[TYPE_S18]], reason=arg>(read<@type[[TYPE_S18]]>(%[[VALUE_g3s18]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S18]], @type[[TYPE_S18]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_18]], copy<@type[[TYPE_S18]], reason=arg>(read<@type[[TYPE_S18]]>(%[[VALUE_g1s18]])), copy<@type[[TYPE_S18]], reason=arg>(read<@type[[TYPE_S18]]>(%[[VALUE_g3s18]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init19:[0-9]+]] @init19(%[[VALUE_p_39:[0-9]+]] p: ptr<@type[[TYPE_S19]]>, %[[VALUE_i_39:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_39:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE115:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_39]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_39]]), const<i32>(19))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE116:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_39]]);
// DEFAULT-NEXT:                 let %[[VALUE117:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE116]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_39]], read<i32>(%[[VALUE117]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(19)>(field0(deref(read<ptr<@type[[TYPE_S19]]>>(%[[VALUE_p_39]])))), read<i32>(%[[VALUE_j_39]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_39]]), read<i32>(%[[VALUE_j_39]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check19:[0-9]+]] @check19(%[[VALUE_p_40:[0-9]+]] p: ptr<@type[[TYPE_S19]]>, %[[VALUE_i_40:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_40:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE118:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_40]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_40]]), const<i32>(19))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE119:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_40]]);
// DEFAULT-NEXT:                 let %[[VALUE120:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE119]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_40]], read<i32>(%[[VALUE120]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(19)>(field0(deref(read<ptr<@type[[TYPE_S19]]>>(%[[VALUE_p_40]])))), read<i32>(%[[VALUE_j_40]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_40]]), read<i32>(%[[VALUE_j_40]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test19:[0-9]+]] @test19(%[[VALUE_s1_39:[0-9]+]] s1: @type[[TYPE_S19]], %[[VALUE_s2_39:[0-9]+]] s2: @type[[TYPE_S19]], %[[VALUE_s3_20:[0-9]+]] s3: @type[[TYPE_S19]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S19]]>, i32) -> void>(%[[VALUE_check19]], addr_of<ptr<@type[[TYPE_S19]]>>(%[[VALUE_s1_39]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S19]]>, i32) -> void>(%[[VALUE_check19]], addr_of<ptr<@type[[TYPE_S19]]>>(%[[VALUE_s2_39]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S19]]>, i32) -> void>(%[[VALUE_check19]], addr_of<ptr<@type[[TYPE_S19]]>>(%[[VALUE_s3_20]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_19:[0-9]+]] @test2_19(%[[VALUE_s1_40:[0-9]+]] s1: @type[[TYPE_S19]], %[[VALUE_s2_40:[0-9]+]] s2: @type[[TYPE_S19]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S19]], @type[[TYPE_S19]], @type[[TYPE_S19]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test19]], copy<@type[[TYPE_S19]], reason=arg>(read<@type[[TYPE_S19]]>(%[[VALUE_s1_40]])), copy<@type[[TYPE_S19]], reason=arg>(read<@type[[TYPE_S19]]>(%[[VALUE_g2s19]])), copy<@type[[TYPE_S19]], reason=arg>(read<@type[[TYPE_S19]]>(%[[VALUE_s2_40]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit19:[0-9]+]] @testit19() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S19]]>, i32) -> void>(%[[VALUE_init19]], addr_of<ptr<@type[[TYPE_S19]]>>(%[[VALUE_g1s19]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S19]]>, i32) -> void>(%[[VALUE_check19]], addr_of<ptr<@type[[TYPE_S19]]>>(%[[VALUE_g1s19]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S19]]>, i32) -> void>(%[[VALUE_init19]], addr_of<ptr<@type[[TYPE_S19]]>>(%[[VALUE_g2s19]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S19]]>, i32) -> void>(%[[VALUE_check19]], addr_of<ptr<@type[[TYPE_S19]]>>(%[[VALUE_g2s19]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S19]]>, i32) -> void>(%[[VALUE_init19]], addr_of<ptr<@type[[TYPE_S19]]>>(%[[VALUE_g3s19]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S19]]>, i32) -> void>(%[[VALUE_check19]], addr_of<ptr<@type[[TYPE_S19]]>>(%[[VALUE_g3s19]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S19]], @type[[TYPE_S19]], @type[[TYPE_S19]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test19]], copy<@type[[TYPE_S19]], reason=arg>(read<@type[[TYPE_S19]]>(%[[VALUE_g1s19]])), copy<@type[[TYPE_S19]], reason=arg>(read<@type[[TYPE_S19]]>(%[[VALUE_g2s19]])), copy<@type[[TYPE_S19]], reason=arg>(read<@type[[TYPE_S19]]>(%[[VALUE_g3s19]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S19]], @type[[TYPE_S19]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_19]], copy<@type[[TYPE_S19]], reason=arg>(read<@type[[TYPE_S19]]>(%[[VALUE_g1s19]])), copy<@type[[TYPE_S19]], reason=arg>(read<@type[[TYPE_S19]]>(%[[VALUE_g3s19]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init20:[0-9]+]] @init20(%[[VALUE_p_41:[0-9]+]] p: ptr<@type[[TYPE_S20]]>, %[[VALUE_i_41:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_41:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE121:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_41]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_41]]), const<i32>(20))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE122:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_41]]);
// DEFAULT-NEXT:                 let %[[VALUE123:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE122]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_41]], read<i32>(%[[VALUE123]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(20)>(field0(deref(read<ptr<@type[[TYPE_S20]]>>(%[[VALUE_p_41]])))), read<i32>(%[[VALUE_j_41]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_41]]), read<i32>(%[[VALUE_j_41]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check20:[0-9]+]] @check20(%[[VALUE_p_42:[0-9]+]] p: ptr<@type[[TYPE_S20]]>, %[[VALUE_i_42:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_42:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE124:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_42]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_42]]), const<i32>(20))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE125:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_42]]);
// DEFAULT-NEXT:                 let %[[VALUE126:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE125]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_42]], read<i32>(%[[VALUE126]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(20)>(field0(deref(read<ptr<@type[[TYPE_S20]]>>(%[[VALUE_p_42]])))), read<i32>(%[[VALUE_j_42]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_42]]), read<i32>(%[[VALUE_j_42]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test20:[0-9]+]] @test20(%[[VALUE_s1_41:[0-9]+]] s1: @type[[TYPE_S20]], %[[VALUE_s2_41:[0-9]+]] s2: @type[[TYPE_S20]], %[[VALUE_s3_21:[0-9]+]] s3: @type[[TYPE_S20]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S20]]>, i32) -> void>(%[[VALUE_check20]], addr_of<ptr<@type[[TYPE_S20]]>>(%[[VALUE_s1_41]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S20]]>, i32) -> void>(%[[VALUE_check20]], addr_of<ptr<@type[[TYPE_S20]]>>(%[[VALUE_s2_41]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S20]]>, i32) -> void>(%[[VALUE_check20]], addr_of<ptr<@type[[TYPE_S20]]>>(%[[VALUE_s3_21]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_20:[0-9]+]] @test2_20(%[[VALUE_s1_42:[0-9]+]] s1: @type[[TYPE_S20]], %[[VALUE_s2_42:[0-9]+]] s2: @type[[TYPE_S20]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S20]], @type[[TYPE_S20]], @type[[TYPE_S20]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test20]], copy<@type[[TYPE_S20]], reason=arg>(read<@type[[TYPE_S20]]>(%[[VALUE_s1_42]])), copy<@type[[TYPE_S20]], reason=arg>(read<@type[[TYPE_S20]]>(%[[VALUE_g2s20]])), copy<@type[[TYPE_S20]], reason=arg>(read<@type[[TYPE_S20]]>(%[[VALUE_s2_42]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit20:[0-9]+]] @testit20() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S20]]>, i32) -> void>(%[[VALUE_init20]], addr_of<ptr<@type[[TYPE_S20]]>>(%[[VALUE_g1s20]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S20]]>, i32) -> void>(%[[VALUE_check20]], addr_of<ptr<@type[[TYPE_S20]]>>(%[[VALUE_g1s20]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S20]]>, i32) -> void>(%[[VALUE_init20]], addr_of<ptr<@type[[TYPE_S20]]>>(%[[VALUE_g2s20]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S20]]>, i32) -> void>(%[[VALUE_check20]], addr_of<ptr<@type[[TYPE_S20]]>>(%[[VALUE_g2s20]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S20]]>, i32) -> void>(%[[VALUE_init20]], addr_of<ptr<@type[[TYPE_S20]]>>(%[[VALUE_g3s20]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S20]]>, i32) -> void>(%[[VALUE_check20]], addr_of<ptr<@type[[TYPE_S20]]>>(%[[VALUE_g3s20]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S20]], @type[[TYPE_S20]], @type[[TYPE_S20]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test20]], copy<@type[[TYPE_S20]], reason=arg>(read<@type[[TYPE_S20]]>(%[[VALUE_g1s20]])), copy<@type[[TYPE_S20]], reason=arg>(read<@type[[TYPE_S20]]>(%[[VALUE_g2s20]])), copy<@type[[TYPE_S20]], reason=arg>(read<@type[[TYPE_S20]]>(%[[VALUE_g3s20]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S20]], @type[[TYPE_S20]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_20]], copy<@type[[TYPE_S20]], reason=arg>(read<@type[[TYPE_S20]]>(%[[VALUE_g1s20]])), copy<@type[[TYPE_S20]], reason=arg>(read<@type[[TYPE_S20]]>(%[[VALUE_g3s20]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init21:[0-9]+]] @init21(%[[VALUE_p_43:[0-9]+]] p: ptr<@type[[TYPE_S21]]>, %[[VALUE_i_43:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_43:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE127:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_43]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_43]]), const<i32>(21))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE128:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_43]]);
// DEFAULT-NEXT:                 let %[[VALUE129:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE128]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_43]], read<i32>(%[[VALUE129]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(21)>(field0(deref(read<ptr<@type[[TYPE_S21]]>>(%[[VALUE_p_43]])))), read<i32>(%[[VALUE_j_43]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_43]]), read<i32>(%[[VALUE_j_43]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check21:[0-9]+]] @check21(%[[VALUE_p_44:[0-9]+]] p: ptr<@type[[TYPE_S21]]>, %[[VALUE_i_44:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_44:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE130:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_44]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_44]]), const<i32>(21))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE131:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_44]]);
// DEFAULT-NEXT:                 let %[[VALUE132:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE131]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_44]], read<i32>(%[[VALUE132]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(21)>(field0(deref(read<ptr<@type[[TYPE_S21]]>>(%[[VALUE_p_44]])))), read<i32>(%[[VALUE_j_44]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_44]]), read<i32>(%[[VALUE_j_44]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test21:[0-9]+]] @test21(%[[VALUE_s1_43:[0-9]+]] s1: @type[[TYPE_S21]], %[[VALUE_s2_43:[0-9]+]] s2: @type[[TYPE_S21]], %[[VALUE_s3_22:[0-9]+]] s3: @type[[TYPE_S21]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S21]]>, i32) -> void>(%[[VALUE_check21]], addr_of<ptr<@type[[TYPE_S21]]>>(%[[VALUE_s1_43]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S21]]>, i32) -> void>(%[[VALUE_check21]], addr_of<ptr<@type[[TYPE_S21]]>>(%[[VALUE_s2_43]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S21]]>, i32) -> void>(%[[VALUE_check21]], addr_of<ptr<@type[[TYPE_S21]]>>(%[[VALUE_s3_22]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_21:[0-9]+]] @test2_21(%[[VALUE_s1_44:[0-9]+]] s1: @type[[TYPE_S21]], %[[VALUE_s2_44:[0-9]+]] s2: @type[[TYPE_S21]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S21]], @type[[TYPE_S21]], @type[[TYPE_S21]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test21]], copy<@type[[TYPE_S21]], reason=arg>(read<@type[[TYPE_S21]]>(%[[VALUE_s1_44]])), copy<@type[[TYPE_S21]], reason=arg>(read<@type[[TYPE_S21]]>(%[[VALUE_g2s21]])), copy<@type[[TYPE_S21]], reason=arg>(read<@type[[TYPE_S21]]>(%[[VALUE_s2_44]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit21:[0-9]+]] @testit21() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S21]]>, i32) -> void>(%[[VALUE_init21]], addr_of<ptr<@type[[TYPE_S21]]>>(%[[VALUE_g1s21]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S21]]>, i32) -> void>(%[[VALUE_check21]], addr_of<ptr<@type[[TYPE_S21]]>>(%[[VALUE_g1s21]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S21]]>, i32) -> void>(%[[VALUE_init21]], addr_of<ptr<@type[[TYPE_S21]]>>(%[[VALUE_g2s21]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S21]]>, i32) -> void>(%[[VALUE_check21]], addr_of<ptr<@type[[TYPE_S21]]>>(%[[VALUE_g2s21]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S21]]>, i32) -> void>(%[[VALUE_init21]], addr_of<ptr<@type[[TYPE_S21]]>>(%[[VALUE_g3s21]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S21]]>, i32) -> void>(%[[VALUE_check21]], addr_of<ptr<@type[[TYPE_S21]]>>(%[[VALUE_g3s21]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S21]], @type[[TYPE_S21]], @type[[TYPE_S21]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test21]], copy<@type[[TYPE_S21]], reason=arg>(read<@type[[TYPE_S21]]>(%[[VALUE_g1s21]])), copy<@type[[TYPE_S21]], reason=arg>(read<@type[[TYPE_S21]]>(%[[VALUE_g2s21]])), copy<@type[[TYPE_S21]], reason=arg>(read<@type[[TYPE_S21]]>(%[[VALUE_g3s21]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S21]], @type[[TYPE_S21]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_21]], copy<@type[[TYPE_S21]], reason=arg>(read<@type[[TYPE_S21]]>(%[[VALUE_g1s21]])), copy<@type[[TYPE_S21]], reason=arg>(read<@type[[TYPE_S21]]>(%[[VALUE_g3s21]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init22:[0-9]+]] @init22(%[[VALUE_p_45:[0-9]+]] p: ptr<@type[[TYPE_S22]]>, %[[VALUE_i_45:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_45:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE133:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_45]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_45]]), const<i32>(22))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE134:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_45]]);
// DEFAULT-NEXT:                 let %[[VALUE135:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE134]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_45]], read<i32>(%[[VALUE135]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(22)>(field0(deref(read<ptr<@type[[TYPE_S22]]>>(%[[VALUE_p_45]])))), read<i32>(%[[VALUE_j_45]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_45]]), read<i32>(%[[VALUE_j_45]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check22:[0-9]+]] @check22(%[[VALUE_p_46:[0-9]+]] p: ptr<@type[[TYPE_S22]]>, %[[VALUE_i_46:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_46:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE136:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_46]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_46]]), const<i32>(22))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE137:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_46]]);
// DEFAULT-NEXT:                 let %[[VALUE138:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE137]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_46]], read<i32>(%[[VALUE138]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(22)>(field0(deref(read<ptr<@type[[TYPE_S22]]>>(%[[VALUE_p_46]])))), read<i32>(%[[VALUE_j_46]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_46]]), read<i32>(%[[VALUE_j_46]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test22:[0-9]+]] @test22(%[[VALUE_s1_45:[0-9]+]] s1: @type[[TYPE_S22]], %[[VALUE_s2_45:[0-9]+]] s2: @type[[TYPE_S22]], %[[VALUE_s3_23:[0-9]+]] s3: @type[[TYPE_S22]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S22]]>, i32) -> void>(%[[VALUE_check22]], addr_of<ptr<@type[[TYPE_S22]]>>(%[[VALUE_s1_45]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S22]]>, i32) -> void>(%[[VALUE_check22]], addr_of<ptr<@type[[TYPE_S22]]>>(%[[VALUE_s2_45]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S22]]>, i32) -> void>(%[[VALUE_check22]], addr_of<ptr<@type[[TYPE_S22]]>>(%[[VALUE_s3_23]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_22:[0-9]+]] @test2_22(%[[VALUE_s1_46:[0-9]+]] s1: @type[[TYPE_S22]], %[[VALUE_s2_46:[0-9]+]] s2: @type[[TYPE_S22]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S22]], @type[[TYPE_S22]], @type[[TYPE_S22]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test22]], copy<@type[[TYPE_S22]], reason=arg>(read<@type[[TYPE_S22]]>(%[[VALUE_s1_46]])), copy<@type[[TYPE_S22]], reason=arg>(read<@type[[TYPE_S22]]>(%[[VALUE_g2s22]])), copy<@type[[TYPE_S22]], reason=arg>(read<@type[[TYPE_S22]]>(%[[VALUE_s2_46]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit22:[0-9]+]] @testit22() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S22]]>, i32) -> void>(%[[VALUE_init22]], addr_of<ptr<@type[[TYPE_S22]]>>(%[[VALUE_g1s22]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S22]]>, i32) -> void>(%[[VALUE_check22]], addr_of<ptr<@type[[TYPE_S22]]>>(%[[VALUE_g1s22]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S22]]>, i32) -> void>(%[[VALUE_init22]], addr_of<ptr<@type[[TYPE_S22]]>>(%[[VALUE_g2s22]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S22]]>, i32) -> void>(%[[VALUE_check22]], addr_of<ptr<@type[[TYPE_S22]]>>(%[[VALUE_g2s22]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S22]]>, i32) -> void>(%[[VALUE_init22]], addr_of<ptr<@type[[TYPE_S22]]>>(%[[VALUE_g3s22]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S22]]>, i32) -> void>(%[[VALUE_check22]], addr_of<ptr<@type[[TYPE_S22]]>>(%[[VALUE_g3s22]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S22]], @type[[TYPE_S22]], @type[[TYPE_S22]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test22]], copy<@type[[TYPE_S22]], reason=arg>(read<@type[[TYPE_S22]]>(%[[VALUE_g1s22]])), copy<@type[[TYPE_S22]], reason=arg>(read<@type[[TYPE_S22]]>(%[[VALUE_g2s22]])), copy<@type[[TYPE_S22]], reason=arg>(read<@type[[TYPE_S22]]>(%[[VALUE_g3s22]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S22]], @type[[TYPE_S22]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_22]], copy<@type[[TYPE_S22]], reason=arg>(read<@type[[TYPE_S22]]>(%[[VALUE_g1s22]])), copy<@type[[TYPE_S22]], reason=arg>(read<@type[[TYPE_S22]]>(%[[VALUE_g3s22]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init23:[0-9]+]] @init23(%[[VALUE_p_47:[0-9]+]] p: ptr<@type[[TYPE_S23]]>, %[[VALUE_i_47:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_47:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE139:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_47]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_47]]), const<i32>(23))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE140:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_47]]);
// DEFAULT-NEXT:                 let %[[VALUE141:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE140]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_47]], read<i32>(%[[VALUE141]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(23)>(field0(deref(read<ptr<@type[[TYPE_S23]]>>(%[[VALUE_p_47]])))), read<i32>(%[[VALUE_j_47]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_47]]), read<i32>(%[[VALUE_j_47]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check23:[0-9]+]] @check23(%[[VALUE_p_48:[0-9]+]] p: ptr<@type[[TYPE_S23]]>, %[[VALUE_i_48:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_48:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE142:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_48]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_48]]), const<i32>(23))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE143:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_48]]);
// DEFAULT-NEXT:                 let %[[VALUE144:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE143]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_48]], read<i32>(%[[VALUE144]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(23)>(field0(deref(read<ptr<@type[[TYPE_S23]]>>(%[[VALUE_p_48]])))), read<i32>(%[[VALUE_j_48]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_48]]), read<i32>(%[[VALUE_j_48]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test23:[0-9]+]] @test23(%[[VALUE_s1_47:[0-9]+]] s1: @type[[TYPE_S23]], %[[VALUE_s2_47:[0-9]+]] s2: @type[[TYPE_S23]], %[[VALUE_s3_24:[0-9]+]] s3: @type[[TYPE_S23]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S23]]>, i32) -> void>(%[[VALUE_check23]], addr_of<ptr<@type[[TYPE_S23]]>>(%[[VALUE_s1_47]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S23]]>, i32) -> void>(%[[VALUE_check23]], addr_of<ptr<@type[[TYPE_S23]]>>(%[[VALUE_s2_47]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S23]]>, i32) -> void>(%[[VALUE_check23]], addr_of<ptr<@type[[TYPE_S23]]>>(%[[VALUE_s3_24]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_23:[0-9]+]] @test2_23(%[[VALUE_s1_48:[0-9]+]] s1: @type[[TYPE_S23]], %[[VALUE_s2_48:[0-9]+]] s2: @type[[TYPE_S23]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S23]], @type[[TYPE_S23]], @type[[TYPE_S23]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test23]], copy<@type[[TYPE_S23]], reason=arg>(read<@type[[TYPE_S23]]>(%[[VALUE_s1_48]])), copy<@type[[TYPE_S23]], reason=arg>(read<@type[[TYPE_S23]]>(%[[VALUE_g2s23]])), copy<@type[[TYPE_S23]], reason=arg>(read<@type[[TYPE_S23]]>(%[[VALUE_s2_48]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit23:[0-9]+]] @testit23() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S23]]>, i32) -> void>(%[[VALUE_init23]], addr_of<ptr<@type[[TYPE_S23]]>>(%[[VALUE_g1s23]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S23]]>, i32) -> void>(%[[VALUE_check23]], addr_of<ptr<@type[[TYPE_S23]]>>(%[[VALUE_g1s23]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S23]]>, i32) -> void>(%[[VALUE_init23]], addr_of<ptr<@type[[TYPE_S23]]>>(%[[VALUE_g2s23]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S23]]>, i32) -> void>(%[[VALUE_check23]], addr_of<ptr<@type[[TYPE_S23]]>>(%[[VALUE_g2s23]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S23]]>, i32) -> void>(%[[VALUE_init23]], addr_of<ptr<@type[[TYPE_S23]]>>(%[[VALUE_g3s23]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S23]]>, i32) -> void>(%[[VALUE_check23]], addr_of<ptr<@type[[TYPE_S23]]>>(%[[VALUE_g3s23]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S23]], @type[[TYPE_S23]], @type[[TYPE_S23]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test23]], copy<@type[[TYPE_S23]], reason=arg>(read<@type[[TYPE_S23]]>(%[[VALUE_g1s23]])), copy<@type[[TYPE_S23]], reason=arg>(read<@type[[TYPE_S23]]>(%[[VALUE_g2s23]])), copy<@type[[TYPE_S23]], reason=arg>(read<@type[[TYPE_S23]]>(%[[VALUE_g3s23]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S23]], @type[[TYPE_S23]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_23]], copy<@type[[TYPE_S23]], reason=arg>(read<@type[[TYPE_S23]]>(%[[VALUE_g1s23]])), copy<@type[[TYPE_S23]], reason=arg>(read<@type[[TYPE_S23]]>(%[[VALUE_g3s23]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init24:[0-9]+]] @init24(%[[VALUE_p_49:[0-9]+]] p: ptr<@type[[TYPE_S24]]>, %[[VALUE_i_49:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_49:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE145:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_49]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_49]]), const<i32>(24))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE146:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_49]]);
// DEFAULT-NEXT:                 let %[[VALUE147:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE146]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_49]], read<i32>(%[[VALUE147]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(24)>(field0(deref(read<ptr<@type[[TYPE_S24]]>>(%[[VALUE_p_49]])))), read<i32>(%[[VALUE_j_49]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_49]]), read<i32>(%[[VALUE_j_49]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check24:[0-9]+]] @check24(%[[VALUE_p_50:[0-9]+]] p: ptr<@type[[TYPE_S24]]>, %[[VALUE_i_50:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_50:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE148:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_50]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_50]]), const<i32>(24))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE149:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_50]]);
// DEFAULT-NEXT:                 let %[[VALUE150:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE149]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_50]], read<i32>(%[[VALUE150]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(24)>(field0(deref(read<ptr<@type[[TYPE_S24]]>>(%[[VALUE_p_50]])))), read<i32>(%[[VALUE_j_50]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_50]]), read<i32>(%[[VALUE_j_50]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test24:[0-9]+]] @test24(%[[VALUE_s1_49:[0-9]+]] s1: @type[[TYPE_S24]], %[[VALUE_s2_49:[0-9]+]] s2: @type[[TYPE_S24]], %[[VALUE_s3_25:[0-9]+]] s3: @type[[TYPE_S24]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S24]]>, i32) -> void>(%[[VALUE_check24]], addr_of<ptr<@type[[TYPE_S24]]>>(%[[VALUE_s1_49]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S24]]>, i32) -> void>(%[[VALUE_check24]], addr_of<ptr<@type[[TYPE_S24]]>>(%[[VALUE_s2_49]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S24]]>, i32) -> void>(%[[VALUE_check24]], addr_of<ptr<@type[[TYPE_S24]]>>(%[[VALUE_s3_25]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_24:[0-9]+]] @test2_24(%[[VALUE_s1_50:[0-9]+]] s1: @type[[TYPE_S24]], %[[VALUE_s2_50:[0-9]+]] s2: @type[[TYPE_S24]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S24]], @type[[TYPE_S24]], @type[[TYPE_S24]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test24]], copy<@type[[TYPE_S24]], reason=arg>(read<@type[[TYPE_S24]]>(%[[VALUE_s1_50]])), copy<@type[[TYPE_S24]], reason=arg>(read<@type[[TYPE_S24]]>(%[[VALUE_g2s24]])), copy<@type[[TYPE_S24]], reason=arg>(read<@type[[TYPE_S24]]>(%[[VALUE_s2_50]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit24:[0-9]+]] @testit24() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S24]]>, i32) -> void>(%[[VALUE_init24]], addr_of<ptr<@type[[TYPE_S24]]>>(%[[VALUE_g1s24]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S24]]>, i32) -> void>(%[[VALUE_check24]], addr_of<ptr<@type[[TYPE_S24]]>>(%[[VALUE_g1s24]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S24]]>, i32) -> void>(%[[VALUE_init24]], addr_of<ptr<@type[[TYPE_S24]]>>(%[[VALUE_g2s24]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S24]]>, i32) -> void>(%[[VALUE_check24]], addr_of<ptr<@type[[TYPE_S24]]>>(%[[VALUE_g2s24]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S24]]>, i32) -> void>(%[[VALUE_init24]], addr_of<ptr<@type[[TYPE_S24]]>>(%[[VALUE_g3s24]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S24]]>, i32) -> void>(%[[VALUE_check24]], addr_of<ptr<@type[[TYPE_S24]]>>(%[[VALUE_g3s24]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S24]], @type[[TYPE_S24]], @type[[TYPE_S24]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test24]], copy<@type[[TYPE_S24]], reason=arg>(read<@type[[TYPE_S24]]>(%[[VALUE_g1s24]])), copy<@type[[TYPE_S24]], reason=arg>(read<@type[[TYPE_S24]]>(%[[VALUE_g2s24]])), copy<@type[[TYPE_S24]], reason=arg>(read<@type[[TYPE_S24]]>(%[[VALUE_g3s24]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S24]], @type[[TYPE_S24]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_24]], copy<@type[[TYPE_S24]], reason=arg>(read<@type[[TYPE_S24]]>(%[[VALUE_g1s24]])), copy<@type[[TYPE_S24]], reason=arg>(read<@type[[TYPE_S24]]>(%[[VALUE_g3s24]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init25:[0-9]+]] @init25(%[[VALUE_p_51:[0-9]+]] p: ptr<@type[[TYPE_S25]]>, %[[VALUE_i_51:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_51:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE151:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_51]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_51]]), const<i32>(25))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE152:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_51]]);
// DEFAULT-NEXT:                 let %[[VALUE153:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE152]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_51]], read<i32>(%[[VALUE153]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(25)>(field0(deref(read<ptr<@type[[TYPE_S25]]>>(%[[VALUE_p_51]])))), read<i32>(%[[VALUE_j_51]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_51]]), read<i32>(%[[VALUE_j_51]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check25:[0-9]+]] @check25(%[[VALUE_p_52:[0-9]+]] p: ptr<@type[[TYPE_S25]]>, %[[VALUE_i_52:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_52:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE154:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_52]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_52]]), const<i32>(25))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE155:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_52]]);
// DEFAULT-NEXT:                 let %[[VALUE156:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE155]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_52]], read<i32>(%[[VALUE156]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(25)>(field0(deref(read<ptr<@type[[TYPE_S25]]>>(%[[VALUE_p_52]])))), read<i32>(%[[VALUE_j_52]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_52]]), read<i32>(%[[VALUE_j_52]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test25:[0-9]+]] @test25(%[[VALUE_s1_51:[0-9]+]] s1: @type[[TYPE_S25]], %[[VALUE_s2_51:[0-9]+]] s2: @type[[TYPE_S25]], %[[VALUE_s3_26:[0-9]+]] s3: @type[[TYPE_S25]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S25]]>, i32) -> void>(%[[VALUE_check25]], addr_of<ptr<@type[[TYPE_S25]]>>(%[[VALUE_s1_51]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S25]]>, i32) -> void>(%[[VALUE_check25]], addr_of<ptr<@type[[TYPE_S25]]>>(%[[VALUE_s2_51]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S25]]>, i32) -> void>(%[[VALUE_check25]], addr_of<ptr<@type[[TYPE_S25]]>>(%[[VALUE_s3_26]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_25:[0-9]+]] @test2_25(%[[VALUE_s1_52:[0-9]+]] s1: @type[[TYPE_S25]], %[[VALUE_s2_52:[0-9]+]] s2: @type[[TYPE_S25]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S25]], @type[[TYPE_S25]], @type[[TYPE_S25]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test25]], copy<@type[[TYPE_S25]], reason=arg>(read<@type[[TYPE_S25]]>(%[[VALUE_s1_52]])), copy<@type[[TYPE_S25]], reason=arg>(read<@type[[TYPE_S25]]>(%[[VALUE_g2s25]])), copy<@type[[TYPE_S25]], reason=arg>(read<@type[[TYPE_S25]]>(%[[VALUE_s2_52]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit25:[0-9]+]] @testit25() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S25]]>, i32) -> void>(%[[VALUE_init25]], addr_of<ptr<@type[[TYPE_S25]]>>(%[[VALUE_g1s25]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S25]]>, i32) -> void>(%[[VALUE_check25]], addr_of<ptr<@type[[TYPE_S25]]>>(%[[VALUE_g1s25]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S25]]>, i32) -> void>(%[[VALUE_init25]], addr_of<ptr<@type[[TYPE_S25]]>>(%[[VALUE_g2s25]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S25]]>, i32) -> void>(%[[VALUE_check25]], addr_of<ptr<@type[[TYPE_S25]]>>(%[[VALUE_g2s25]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S25]]>, i32) -> void>(%[[VALUE_init25]], addr_of<ptr<@type[[TYPE_S25]]>>(%[[VALUE_g3s25]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S25]]>, i32) -> void>(%[[VALUE_check25]], addr_of<ptr<@type[[TYPE_S25]]>>(%[[VALUE_g3s25]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S25]], @type[[TYPE_S25]], @type[[TYPE_S25]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test25]], copy<@type[[TYPE_S25]], reason=arg>(read<@type[[TYPE_S25]]>(%[[VALUE_g1s25]])), copy<@type[[TYPE_S25]], reason=arg>(read<@type[[TYPE_S25]]>(%[[VALUE_g2s25]])), copy<@type[[TYPE_S25]], reason=arg>(read<@type[[TYPE_S25]]>(%[[VALUE_g3s25]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S25]], @type[[TYPE_S25]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_25]], copy<@type[[TYPE_S25]], reason=arg>(read<@type[[TYPE_S25]]>(%[[VALUE_g1s25]])), copy<@type[[TYPE_S25]], reason=arg>(read<@type[[TYPE_S25]]>(%[[VALUE_g3s25]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init26:[0-9]+]] @init26(%[[VALUE_p_53:[0-9]+]] p: ptr<@type[[TYPE_S26]]>, %[[VALUE_i_53:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_53:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE157:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_53]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_53]]), const<i32>(26))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE158:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_53]]);
// DEFAULT-NEXT:                 let %[[VALUE159:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE158]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_53]], read<i32>(%[[VALUE159]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(26)>(field0(deref(read<ptr<@type[[TYPE_S26]]>>(%[[VALUE_p_53]])))), read<i32>(%[[VALUE_j_53]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_53]]), read<i32>(%[[VALUE_j_53]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check26:[0-9]+]] @check26(%[[VALUE_p_54:[0-9]+]] p: ptr<@type[[TYPE_S26]]>, %[[VALUE_i_54:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_54:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE160:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_54]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_54]]), const<i32>(26))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE161:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_54]]);
// DEFAULT-NEXT:                 let %[[VALUE162:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE161]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_54]], read<i32>(%[[VALUE162]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(26)>(field0(deref(read<ptr<@type[[TYPE_S26]]>>(%[[VALUE_p_54]])))), read<i32>(%[[VALUE_j_54]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_54]]), read<i32>(%[[VALUE_j_54]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test26:[0-9]+]] @test26(%[[VALUE_s1_53:[0-9]+]] s1: @type[[TYPE_S26]], %[[VALUE_s2_53:[0-9]+]] s2: @type[[TYPE_S26]], %[[VALUE_s3_27:[0-9]+]] s3: @type[[TYPE_S26]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S26]]>, i32) -> void>(%[[VALUE_check26]], addr_of<ptr<@type[[TYPE_S26]]>>(%[[VALUE_s1_53]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S26]]>, i32) -> void>(%[[VALUE_check26]], addr_of<ptr<@type[[TYPE_S26]]>>(%[[VALUE_s2_53]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S26]]>, i32) -> void>(%[[VALUE_check26]], addr_of<ptr<@type[[TYPE_S26]]>>(%[[VALUE_s3_27]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_26:[0-9]+]] @test2_26(%[[VALUE_s1_54:[0-9]+]] s1: @type[[TYPE_S26]], %[[VALUE_s2_54:[0-9]+]] s2: @type[[TYPE_S26]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S26]], @type[[TYPE_S26]], @type[[TYPE_S26]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test26]], copy<@type[[TYPE_S26]], reason=arg>(read<@type[[TYPE_S26]]>(%[[VALUE_s1_54]])), copy<@type[[TYPE_S26]], reason=arg>(read<@type[[TYPE_S26]]>(%[[VALUE_g2s26]])), copy<@type[[TYPE_S26]], reason=arg>(read<@type[[TYPE_S26]]>(%[[VALUE_s2_54]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit26:[0-9]+]] @testit26() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S26]]>, i32) -> void>(%[[VALUE_init26]], addr_of<ptr<@type[[TYPE_S26]]>>(%[[VALUE_g1s26]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S26]]>, i32) -> void>(%[[VALUE_check26]], addr_of<ptr<@type[[TYPE_S26]]>>(%[[VALUE_g1s26]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S26]]>, i32) -> void>(%[[VALUE_init26]], addr_of<ptr<@type[[TYPE_S26]]>>(%[[VALUE_g2s26]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S26]]>, i32) -> void>(%[[VALUE_check26]], addr_of<ptr<@type[[TYPE_S26]]>>(%[[VALUE_g2s26]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S26]]>, i32) -> void>(%[[VALUE_init26]], addr_of<ptr<@type[[TYPE_S26]]>>(%[[VALUE_g3s26]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S26]]>, i32) -> void>(%[[VALUE_check26]], addr_of<ptr<@type[[TYPE_S26]]>>(%[[VALUE_g3s26]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S26]], @type[[TYPE_S26]], @type[[TYPE_S26]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test26]], copy<@type[[TYPE_S26]], reason=arg>(read<@type[[TYPE_S26]]>(%[[VALUE_g1s26]])), copy<@type[[TYPE_S26]], reason=arg>(read<@type[[TYPE_S26]]>(%[[VALUE_g2s26]])), copy<@type[[TYPE_S26]], reason=arg>(read<@type[[TYPE_S26]]>(%[[VALUE_g3s26]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S26]], @type[[TYPE_S26]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_26]], copy<@type[[TYPE_S26]], reason=arg>(read<@type[[TYPE_S26]]>(%[[VALUE_g1s26]])), copy<@type[[TYPE_S26]], reason=arg>(read<@type[[TYPE_S26]]>(%[[VALUE_g3s26]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init27:[0-9]+]] @init27(%[[VALUE_p_55:[0-9]+]] p: ptr<@type[[TYPE_S27]]>, %[[VALUE_i_55:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_55:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE163:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_55]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_55]]), const<i32>(27))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE164:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_55]]);
// DEFAULT-NEXT:                 let %[[VALUE165:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE164]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_55]], read<i32>(%[[VALUE165]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(27)>(field0(deref(read<ptr<@type[[TYPE_S27]]>>(%[[VALUE_p_55]])))), read<i32>(%[[VALUE_j_55]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_55]]), read<i32>(%[[VALUE_j_55]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check27:[0-9]+]] @check27(%[[VALUE_p_56:[0-9]+]] p: ptr<@type[[TYPE_S27]]>, %[[VALUE_i_56:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_56:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE166:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_56]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_56]]), const<i32>(27))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE167:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_56]]);
// DEFAULT-NEXT:                 let %[[VALUE168:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE167]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_56]], read<i32>(%[[VALUE168]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(27)>(field0(deref(read<ptr<@type[[TYPE_S27]]>>(%[[VALUE_p_56]])))), read<i32>(%[[VALUE_j_56]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_56]]), read<i32>(%[[VALUE_j_56]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test27:[0-9]+]] @test27(%[[VALUE_s1_55:[0-9]+]] s1: @type[[TYPE_S27]], %[[VALUE_s2_55:[0-9]+]] s2: @type[[TYPE_S27]], %[[VALUE_s3_28:[0-9]+]] s3: @type[[TYPE_S27]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S27]]>, i32) -> void>(%[[VALUE_check27]], addr_of<ptr<@type[[TYPE_S27]]>>(%[[VALUE_s1_55]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S27]]>, i32) -> void>(%[[VALUE_check27]], addr_of<ptr<@type[[TYPE_S27]]>>(%[[VALUE_s2_55]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S27]]>, i32) -> void>(%[[VALUE_check27]], addr_of<ptr<@type[[TYPE_S27]]>>(%[[VALUE_s3_28]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_27:[0-9]+]] @test2_27(%[[VALUE_s1_56:[0-9]+]] s1: @type[[TYPE_S27]], %[[VALUE_s2_56:[0-9]+]] s2: @type[[TYPE_S27]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S27]], @type[[TYPE_S27]], @type[[TYPE_S27]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test27]], copy<@type[[TYPE_S27]], reason=arg>(read<@type[[TYPE_S27]]>(%[[VALUE_s1_56]])), copy<@type[[TYPE_S27]], reason=arg>(read<@type[[TYPE_S27]]>(%[[VALUE_g2s27]])), copy<@type[[TYPE_S27]], reason=arg>(read<@type[[TYPE_S27]]>(%[[VALUE_s2_56]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit27:[0-9]+]] @testit27() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S27]]>, i32) -> void>(%[[VALUE_init27]], addr_of<ptr<@type[[TYPE_S27]]>>(%[[VALUE_g1s27]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S27]]>, i32) -> void>(%[[VALUE_check27]], addr_of<ptr<@type[[TYPE_S27]]>>(%[[VALUE_g1s27]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S27]]>, i32) -> void>(%[[VALUE_init27]], addr_of<ptr<@type[[TYPE_S27]]>>(%[[VALUE_g2s27]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S27]]>, i32) -> void>(%[[VALUE_check27]], addr_of<ptr<@type[[TYPE_S27]]>>(%[[VALUE_g2s27]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S27]]>, i32) -> void>(%[[VALUE_init27]], addr_of<ptr<@type[[TYPE_S27]]>>(%[[VALUE_g3s27]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S27]]>, i32) -> void>(%[[VALUE_check27]], addr_of<ptr<@type[[TYPE_S27]]>>(%[[VALUE_g3s27]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S27]], @type[[TYPE_S27]], @type[[TYPE_S27]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test27]], copy<@type[[TYPE_S27]], reason=arg>(read<@type[[TYPE_S27]]>(%[[VALUE_g1s27]])), copy<@type[[TYPE_S27]], reason=arg>(read<@type[[TYPE_S27]]>(%[[VALUE_g2s27]])), copy<@type[[TYPE_S27]], reason=arg>(read<@type[[TYPE_S27]]>(%[[VALUE_g3s27]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S27]], @type[[TYPE_S27]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_27]], copy<@type[[TYPE_S27]], reason=arg>(read<@type[[TYPE_S27]]>(%[[VALUE_g1s27]])), copy<@type[[TYPE_S27]], reason=arg>(read<@type[[TYPE_S27]]>(%[[VALUE_g3s27]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init28:[0-9]+]] @init28(%[[VALUE_p_57:[0-9]+]] p: ptr<@type[[TYPE_S28]]>, %[[VALUE_i_57:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_57:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE169:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_57]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_57]]), const<i32>(28))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE170:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_57]]);
// DEFAULT-NEXT:                 let %[[VALUE171:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE170]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_57]], read<i32>(%[[VALUE171]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(28)>(field0(deref(read<ptr<@type[[TYPE_S28]]>>(%[[VALUE_p_57]])))), read<i32>(%[[VALUE_j_57]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_57]]), read<i32>(%[[VALUE_j_57]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check28:[0-9]+]] @check28(%[[VALUE_p_58:[0-9]+]] p: ptr<@type[[TYPE_S28]]>, %[[VALUE_i_58:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_58:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE172:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_58]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_58]]), const<i32>(28))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE173:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_58]]);
// DEFAULT-NEXT:                 let %[[VALUE174:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE173]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_58]], read<i32>(%[[VALUE174]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(28)>(field0(deref(read<ptr<@type[[TYPE_S28]]>>(%[[VALUE_p_58]])))), read<i32>(%[[VALUE_j_58]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_58]]), read<i32>(%[[VALUE_j_58]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test28:[0-9]+]] @test28(%[[VALUE_s1_57:[0-9]+]] s1: @type[[TYPE_S28]], %[[VALUE_s2_57:[0-9]+]] s2: @type[[TYPE_S28]], %[[VALUE_s3_29:[0-9]+]] s3: @type[[TYPE_S28]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S28]]>, i32) -> void>(%[[VALUE_check28]], addr_of<ptr<@type[[TYPE_S28]]>>(%[[VALUE_s1_57]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S28]]>, i32) -> void>(%[[VALUE_check28]], addr_of<ptr<@type[[TYPE_S28]]>>(%[[VALUE_s2_57]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S28]]>, i32) -> void>(%[[VALUE_check28]], addr_of<ptr<@type[[TYPE_S28]]>>(%[[VALUE_s3_29]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_28:[0-9]+]] @test2_28(%[[VALUE_s1_58:[0-9]+]] s1: @type[[TYPE_S28]], %[[VALUE_s2_58:[0-9]+]] s2: @type[[TYPE_S28]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S28]], @type[[TYPE_S28]], @type[[TYPE_S28]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test28]], copy<@type[[TYPE_S28]], reason=arg>(read<@type[[TYPE_S28]]>(%[[VALUE_s1_58]])), copy<@type[[TYPE_S28]], reason=arg>(read<@type[[TYPE_S28]]>(%[[VALUE_g2s28]])), copy<@type[[TYPE_S28]], reason=arg>(read<@type[[TYPE_S28]]>(%[[VALUE_s2_58]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit28:[0-9]+]] @testit28() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S28]]>, i32) -> void>(%[[VALUE_init28]], addr_of<ptr<@type[[TYPE_S28]]>>(%[[VALUE_g1s28]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S28]]>, i32) -> void>(%[[VALUE_check28]], addr_of<ptr<@type[[TYPE_S28]]>>(%[[VALUE_g1s28]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S28]]>, i32) -> void>(%[[VALUE_init28]], addr_of<ptr<@type[[TYPE_S28]]>>(%[[VALUE_g2s28]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S28]]>, i32) -> void>(%[[VALUE_check28]], addr_of<ptr<@type[[TYPE_S28]]>>(%[[VALUE_g2s28]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S28]]>, i32) -> void>(%[[VALUE_init28]], addr_of<ptr<@type[[TYPE_S28]]>>(%[[VALUE_g3s28]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S28]]>, i32) -> void>(%[[VALUE_check28]], addr_of<ptr<@type[[TYPE_S28]]>>(%[[VALUE_g3s28]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S28]], @type[[TYPE_S28]], @type[[TYPE_S28]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test28]], copy<@type[[TYPE_S28]], reason=arg>(read<@type[[TYPE_S28]]>(%[[VALUE_g1s28]])), copy<@type[[TYPE_S28]], reason=arg>(read<@type[[TYPE_S28]]>(%[[VALUE_g2s28]])), copy<@type[[TYPE_S28]], reason=arg>(read<@type[[TYPE_S28]]>(%[[VALUE_g3s28]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S28]], @type[[TYPE_S28]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_28]], copy<@type[[TYPE_S28]], reason=arg>(read<@type[[TYPE_S28]]>(%[[VALUE_g1s28]])), copy<@type[[TYPE_S28]], reason=arg>(read<@type[[TYPE_S28]]>(%[[VALUE_g3s28]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init29:[0-9]+]] @init29(%[[VALUE_p_59:[0-9]+]] p: ptr<@type[[TYPE_S29]]>, %[[VALUE_i_59:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_59:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE175:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_59]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_59]]), const<i32>(29))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE176:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_59]]);
// DEFAULT-NEXT:                 let %[[VALUE177:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE176]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_59]], read<i32>(%[[VALUE177]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(29)>(field0(deref(read<ptr<@type[[TYPE_S29]]>>(%[[VALUE_p_59]])))), read<i32>(%[[VALUE_j_59]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_59]]), read<i32>(%[[VALUE_j_59]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check29:[0-9]+]] @check29(%[[VALUE_p_60:[0-9]+]] p: ptr<@type[[TYPE_S29]]>, %[[VALUE_i_60:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_60:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE178:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_60]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_60]]), const<i32>(29))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE179:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_60]]);
// DEFAULT-NEXT:                 let %[[VALUE180:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE179]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_60]], read<i32>(%[[VALUE180]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(29)>(field0(deref(read<ptr<@type[[TYPE_S29]]>>(%[[VALUE_p_60]])))), read<i32>(%[[VALUE_j_60]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_60]]), read<i32>(%[[VALUE_j_60]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test29:[0-9]+]] @test29(%[[VALUE_s1_59:[0-9]+]] s1: @type[[TYPE_S29]], %[[VALUE_s2_59:[0-9]+]] s2: @type[[TYPE_S29]], %[[VALUE_s3_30:[0-9]+]] s3: @type[[TYPE_S29]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S29]]>, i32) -> void>(%[[VALUE_check29]], addr_of<ptr<@type[[TYPE_S29]]>>(%[[VALUE_s1_59]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S29]]>, i32) -> void>(%[[VALUE_check29]], addr_of<ptr<@type[[TYPE_S29]]>>(%[[VALUE_s2_59]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S29]]>, i32) -> void>(%[[VALUE_check29]], addr_of<ptr<@type[[TYPE_S29]]>>(%[[VALUE_s3_30]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_29:[0-9]+]] @test2_29(%[[VALUE_s1_60:[0-9]+]] s1: @type[[TYPE_S29]], %[[VALUE_s2_60:[0-9]+]] s2: @type[[TYPE_S29]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S29]], @type[[TYPE_S29]], @type[[TYPE_S29]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test29]], copy<@type[[TYPE_S29]], reason=arg>(read<@type[[TYPE_S29]]>(%[[VALUE_s1_60]])), copy<@type[[TYPE_S29]], reason=arg>(read<@type[[TYPE_S29]]>(%[[VALUE_g2s29]])), copy<@type[[TYPE_S29]], reason=arg>(read<@type[[TYPE_S29]]>(%[[VALUE_s2_60]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit29:[0-9]+]] @testit29() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S29]]>, i32) -> void>(%[[VALUE_init29]], addr_of<ptr<@type[[TYPE_S29]]>>(%[[VALUE_g1s29]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S29]]>, i32) -> void>(%[[VALUE_check29]], addr_of<ptr<@type[[TYPE_S29]]>>(%[[VALUE_g1s29]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S29]]>, i32) -> void>(%[[VALUE_init29]], addr_of<ptr<@type[[TYPE_S29]]>>(%[[VALUE_g2s29]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S29]]>, i32) -> void>(%[[VALUE_check29]], addr_of<ptr<@type[[TYPE_S29]]>>(%[[VALUE_g2s29]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S29]]>, i32) -> void>(%[[VALUE_init29]], addr_of<ptr<@type[[TYPE_S29]]>>(%[[VALUE_g3s29]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S29]]>, i32) -> void>(%[[VALUE_check29]], addr_of<ptr<@type[[TYPE_S29]]>>(%[[VALUE_g3s29]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S29]], @type[[TYPE_S29]], @type[[TYPE_S29]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test29]], copy<@type[[TYPE_S29]], reason=arg>(read<@type[[TYPE_S29]]>(%[[VALUE_g1s29]])), copy<@type[[TYPE_S29]], reason=arg>(read<@type[[TYPE_S29]]>(%[[VALUE_g2s29]])), copy<@type[[TYPE_S29]], reason=arg>(read<@type[[TYPE_S29]]>(%[[VALUE_g3s29]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S29]], @type[[TYPE_S29]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_29]], copy<@type[[TYPE_S29]], reason=arg>(read<@type[[TYPE_S29]]>(%[[VALUE_g1s29]])), copy<@type[[TYPE_S29]], reason=arg>(read<@type[[TYPE_S29]]>(%[[VALUE_g3s29]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init30:[0-9]+]] @init30(%[[VALUE_p_61:[0-9]+]] p: ptr<@type[[TYPE_S30]]>, %[[VALUE_i_61:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_61:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE181:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_61]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_61]]), const<i32>(30))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE182:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_61]]);
// DEFAULT-NEXT:                 let %[[VALUE183:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE182]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_61]], read<i32>(%[[VALUE183]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(30)>(field0(deref(read<ptr<@type[[TYPE_S30]]>>(%[[VALUE_p_61]])))), read<i32>(%[[VALUE_j_61]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_61]]), read<i32>(%[[VALUE_j_61]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check30:[0-9]+]] @check30(%[[VALUE_p_62:[0-9]+]] p: ptr<@type[[TYPE_S30]]>, %[[VALUE_i_62:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_62:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE184:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_62]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_62]]), const<i32>(30))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE185:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_62]]);
// DEFAULT-NEXT:                 let %[[VALUE186:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE185]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_62]], read<i32>(%[[VALUE186]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(30)>(field0(deref(read<ptr<@type[[TYPE_S30]]>>(%[[VALUE_p_62]])))), read<i32>(%[[VALUE_j_62]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_62]]), read<i32>(%[[VALUE_j_62]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test30:[0-9]+]] @test30(%[[VALUE_s1_61:[0-9]+]] s1: @type[[TYPE_S30]], %[[VALUE_s2_61:[0-9]+]] s2: @type[[TYPE_S30]], %[[VALUE_s3_31:[0-9]+]] s3: @type[[TYPE_S30]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S30]]>, i32) -> void>(%[[VALUE_check30]], addr_of<ptr<@type[[TYPE_S30]]>>(%[[VALUE_s1_61]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S30]]>, i32) -> void>(%[[VALUE_check30]], addr_of<ptr<@type[[TYPE_S30]]>>(%[[VALUE_s2_61]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S30]]>, i32) -> void>(%[[VALUE_check30]], addr_of<ptr<@type[[TYPE_S30]]>>(%[[VALUE_s3_31]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_30:[0-9]+]] @test2_30(%[[VALUE_s1_62:[0-9]+]] s1: @type[[TYPE_S30]], %[[VALUE_s2_62:[0-9]+]] s2: @type[[TYPE_S30]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S30]], @type[[TYPE_S30]], @type[[TYPE_S30]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test30]], copy<@type[[TYPE_S30]], reason=arg>(read<@type[[TYPE_S30]]>(%[[VALUE_s1_62]])), copy<@type[[TYPE_S30]], reason=arg>(read<@type[[TYPE_S30]]>(%[[VALUE_g2s30]])), copy<@type[[TYPE_S30]], reason=arg>(read<@type[[TYPE_S30]]>(%[[VALUE_s2_62]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit30:[0-9]+]] @testit30() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S30]]>, i32) -> void>(%[[VALUE_init30]], addr_of<ptr<@type[[TYPE_S30]]>>(%[[VALUE_g1s30]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S30]]>, i32) -> void>(%[[VALUE_check30]], addr_of<ptr<@type[[TYPE_S30]]>>(%[[VALUE_g1s30]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S30]]>, i32) -> void>(%[[VALUE_init30]], addr_of<ptr<@type[[TYPE_S30]]>>(%[[VALUE_g2s30]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S30]]>, i32) -> void>(%[[VALUE_check30]], addr_of<ptr<@type[[TYPE_S30]]>>(%[[VALUE_g2s30]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S30]]>, i32) -> void>(%[[VALUE_init30]], addr_of<ptr<@type[[TYPE_S30]]>>(%[[VALUE_g3s30]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S30]]>, i32) -> void>(%[[VALUE_check30]], addr_of<ptr<@type[[TYPE_S30]]>>(%[[VALUE_g3s30]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S30]], @type[[TYPE_S30]], @type[[TYPE_S30]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test30]], copy<@type[[TYPE_S30]], reason=arg>(read<@type[[TYPE_S30]]>(%[[VALUE_g1s30]])), copy<@type[[TYPE_S30]], reason=arg>(read<@type[[TYPE_S30]]>(%[[VALUE_g2s30]])), copy<@type[[TYPE_S30]], reason=arg>(read<@type[[TYPE_S30]]>(%[[VALUE_g3s30]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S30]], @type[[TYPE_S30]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_30]], copy<@type[[TYPE_S30]], reason=arg>(read<@type[[TYPE_S30]]>(%[[VALUE_g1s30]])), copy<@type[[TYPE_S30]], reason=arg>(read<@type[[TYPE_S30]]>(%[[VALUE_g3s30]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init31:[0-9]+]] @init31(%[[VALUE_p_63:[0-9]+]] p: ptr<@type[[TYPE_S31]]>, %[[VALUE_i_63:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_63:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE187:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_63]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_63]]), const<i32>(31))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE188:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_63]]);
// DEFAULT-NEXT:                 let %[[VALUE189:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE188]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_63]], read<i32>(%[[VALUE189]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(31)>(field0(deref(read<ptr<@type[[TYPE_S31]]>>(%[[VALUE_p_63]])))), read<i32>(%[[VALUE_j_63]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_63]]), read<i32>(%[[VALUE_j_63]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check31:[0-9]+]] @check31(%[[VALUE_p_64:[0-9]+]] p: ptr<@type[[TYPE_S31]]>, %[[VALUE_i_64:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_64:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE190:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_64]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_64]]), const<i32>(31))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE191:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_64]]);
// DEFAULT-NEXT:                 let %[[VALUE192:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE191]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_64]], read<i32>(%[[VALUE192]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(31)>(field0(deref(read<ptr<@type[[TYPE_S31]]>>(%[[VALUE_p_64]])))), read<i32>(%[[VALUE_j_64]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_64]]), read<i32>(%[[VALUE_j_64]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test31:[0-9]+]] @test31(%[[VALUE_s1_63:[0-9]+]] s1: @type[[TYPE_S31]], %[[VALUE_s2_63:[0-9]+]] s2: @type[[TYPE_S31]], %[[VALUE_s3_32:[0-9]+]] s3: @type[[TYPE_S31]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S31]]>, i32) -> void>(%[[VALUE_check31]], addr_of<ptr<@type[[TYPE_S31]]>>(%[[VALUE_s1_63]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S31]]>, i32) -> void>(%[[VALUE_check31]], addr_of<ptr<@type[[TYPE_S31]]>>(%[[VALUE_s2_63]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S31]]>, i32) -> void>(%[[VALUE_check31]], addr_of<ptr<@type[[TYPE_S31]]>>(%[[VALUE_s3_32]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_31:[0-9]+]] @test2_31(%[[VALUE_s1_64:[0-9]+]] s1: @type[[TYPE_S31]], %[[VALUE_s2_64:[0-9]+]] s2: @type[[TYPE_S31]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S31]], @type[[TYPE_S31]], @type[[TYPE_S31]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test31]], copy<@type[[TYPE_S31]], reason=arg>(read<@type[[TYPE_S31]]>(%[[VALUE_s1_64]])), copy<@type[[TYPE_S31]], reason=arg>(read<@type[[TYPE_S31]]>(%[[VALUE_g2s31]])), copy<@type[[TYPE_S31]], reason=arg>(read<@type[[TYPE_S31]]>(%[[VALUE_s2_64]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit31:[0-9]+]] @testit31() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S31]]>, i32) -> void>(%[[VALUE_init31]], addr_of<ptr<@type[[TYPE_S31]]>>(%[[VALUE_g1s31]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S31]]>, i32) -> void>(%[[VALUE_check31]], addr_of<ptr<@type[[TYPE_S31]]>>(%[[VALUE_g1s31]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S31]]>, i32) -> void>(%[[VALUE_init31]], addr_of<ptr<@type[[TYPE_S31]]>>(%[[VALUE_g2s31]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S31]]>, i32) -> void>(%[[VALUE_check31]], addr_of<ptr<@type[[TYPE_S31]]>>(%[[VALUE_g2s31]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S31]]>, i32) -> void>(%[[VALUE_init31]], addr_of<ptr<@type[[TYPE_S31]]>>(%[[VALUE_g3s31]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S31]]>, i32) -> void>(%[[VALUE_check31]], addr_of<ptr<@type[[TYPE_S31]]>>(%[[VALUE_g3s31]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S31]], @type[[TYPE_S31]], @type[[TYPE_S31]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test31]], copy<@type[[TYPE_S31]], reason=arg>(read<@type[[TYPE_S31]]>(%[[VALUE_g1s31]])), copy<@type[[TYPE_S31]], reason=arg>(read<@type[[TYPE_S31]]>(%[[VALUE_g2s31]])), copy<@type[[TYPE_S31]], reason=arg>(read<@type[[TYPE_S31]]>(%[[VALUE_g3s31]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S31]], @type[[TYPE_S31]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_31]], copy<@type[[TYPE_S31]], reason=arg>(read<@type[[TYPE_S31]]>(%[[VALUE_g1s31]])), copy<@type[[TYPE_S31]], reason=arg>(read<@type[[TYPE_S31]]>(%[[VALUE_g3s31]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init32:[0-9]+]] @init32(%[[VALUE_p_65:[0-9]+]] p: ptr<@type[[TYPE_S32]]>, %[[VALUE_i_65:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_65:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE193:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_65]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_65]]), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE194:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_65]]);
// DEFAULT-NEXT:                 let %[[VALUE195:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE194]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_65]], read<i32>(%[[VALUE195]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(field0(deref(read<ptr<@type[[TYPE_S32]]>>(%[[VALUE_p_65]])))), read<i32>(%[[VALUE_j_65]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_65]]), read<i32>(%[[VALUE_j_65]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check32:[0-9]+]] @check32(%[[VALUE_p_66:[0-9]+]] p: ptr<@type[[TYPE_S32]]>, %[[VALUE_i_66:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_66:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE196:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_66]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_66]]), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE197:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_66]]);
// DEFAULT-NEXT:                 let %[[VALUE198:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE197]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_66]], read<i32>(%[[VALUE198]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(32)>(field0(deref(read<ptr<@type[[TYPE_S32]]>>(%[[VALUE_p_66]])))), read<i32>(%[[VALUE_j_66]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_66]]), read<i32>(%[[VALUE_j_66]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test32:[0-9]+]] @test32(%[[VALUE_s1_65:[0-9]+]] s1: @type[[TYPE_S32]], %[[VALUE_s2_65:[0-9]+]] s2: @type[[TYPE_S32]], %[[VALUE_s3_33:[0-9]+]] s3: @type[[TYPE_S32]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S32]]>, i32) -> void>(%[[VALUE_check32]], addr_of<ptr<@type[[TYPE_S32]]>>(%[[VALUE_s1_65]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S32]]>, i32) -> void>(%[[VALUE_check32]], addr_of<ptr<@type[[TYPE_S32]]>>(%[[VALUE_s2_65]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S32]]>, i32) -> void>(%[[VALUE_check32]], addr_of<ptr<@type[[TYPE_S32]]>>(%[[VALUE_s3_33]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_32:[0-9]+]] @test2_32(%[[VALUE_s1_66:[0-9]+]] s1: @type[[TYPE_S32]], %[[VALUE_s2_66:[0-9]+]] s2: @type[[TYPE_S32]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S32]], @type[[TYPE_S32]], @type[[TYPE_S32]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test32]], copy<@type[[TYPE_S32]], reason=arg>(read<@type[[TYPE_S32]]>(%[[VALUE_s1_66]])), copy<@type[[TYPE_S32]], reason=arg>(read<@type[[TYPE_S32]]>(%[[VALUE_g2s32]])), copy<@type[[TYPE_S32]], reason=arg>(read<@type[[TYPE_S32]]>(%[[VALUE_s2_66]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit32:[0-9]+]] @testit32() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S32]]>, i32) -> void>(%[[VALUE_init32]], addr_of<ptr<@type[[TYPE_S32]]>>(%[[VALUE_g1s32]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S32]]>, i32) -> void>(%[[VALUE_check32]], addr_of<ptr<@type[[TYPE_S32]]>>(%[[VALUE_g1s32]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S32]]>, i32) -> void>(%[[VALUE_init32]], addr_of<ptr<@type[[TYPE_S32]]>>(%[[VALUE_g2s32]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S32]]>, i32) -> void>(%[[VALUE_check32]], addr_of<ptr<@type[[TYPE_S32]]>>(%[[VALUE_g2s32]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S32]]>, i32) -> void>(%[[VALUE_init32]], addr_of<ptr<@type[[TYPE_S32]]>>(%[[VALUE_g3s32]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S32]]>, i32) -> void>(%[[VALUE_check32]], addr_of<ptr<@type[[TYPE_S32]]>>(%[[VALUE_g3s32]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S32]], @type[[TYPE_S32]], @type[[TYPE_S32]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test32]], copy<@type[[TYPE_S32]], reason=arg>(read<@type[[TYPE_S32]]>(%[[VALUE_g1s32]])), copy<@type[[TYPE_S32]], reason=arg>(read<@type[[TYPE_S32]]>(%[[VALUE_g2s32]])), copy<@type[[TYPE_S32]], reason=arg>(read<@type[[TYPE_S32]]>(%[[VALUE_g3s32]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S32]], @type[[TYPE_S32]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_32]], copy<@type[[TYPE_S32]], reason=arg>(read<@type[[TYPE_S32]]>(%[[VALUE_g1s32]])), copy<@type[[TYPE_S32]], reason=arg>(read<@type[[TYPE_S32]]>(%[[VALUE_g3s32]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init33:[0-9]+]] @init33(%[[VALUE_p_67:[0-9]+]] p: ptr<@type[[TYPE_S33]]>, %[[VALUE_i_67:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_67:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE199:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_67]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_67]]), const<i32>(33))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE200:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_67]]);
// DEFAULT-NEXT:                 let %[[VALUE201:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE200]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_67]], read<i32>(%[[VALUE201]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(33)>(field0(deref(read<ptr<@type[[TYPE_S33]]>>(%[[VALUE_p_67]])))), read<i32>(%[[VALUE_j_67]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_67]]), read<i32>(%[[VALUE_j_67]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check33:[0-9]+]] @check33(%[[VALUE_p_68:[0-9]+]] p: ptr<@type[[TYPE_S33]]>, %[[VALUE_i_68:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_68:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE202:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_68]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_68]]), const<i32>(33))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE203:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_68]]);
// DEFAULT-NEXT:                 let %[[VALUE204:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE203]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_68]], read<i32>(%[[VALUE204]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(33)>(field0(deref(read<ptr<@type[[TYPE_S33]]>>(%[[VALUE_p_68]])))), read<i32>(%[[VALUE_j_68]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_68]]), read<i32>(%[[VALUE_j_68]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test33:[0-9]+]] @test33(%[[VALUE_s1_67:[0-9]+]] s1: @type[[TYPE_S33]], %[[VALUE_s2_67:[0-9]+]] s2: @type[[TYPE_S33]], %[[VALUE_s3_34:[0-9]+]] s3: @type[[TYPE_S33]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S33]]>, i32) -> void>(%[[VALUE_check33]], addr_of<ptr<@type[[TYPE_S33]]>>(%[[VALUE_s1_67]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S33]]>, i32) -> void>(%[[VALUE_check33]], addr_of<ptr<@type[[TYPE_S33]]>>(%[[VALUE_s2_67]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S33]]>, i32) -> void>(%[[VALUE_check33]], addr_of<ptr<@type[[TYPE_S33]]>>(%[[VALUE_s3_34]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_33:[0-9]+]] @test2_33(%[[VALUE_s1_68:[0-9]+]] s1: @type[[TYPE_S33]], %[[VALUE_s2_68:[0-9]+]] s2: @type[[TYPE_S33]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S33]], @type[[TYPE_S33]], @type[[TYPE_S33]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test33]], copy<@type[[TYPE_S33]], reason=arg>(read<@type[[TYPE_S33]]>(%[[VALUE_s1_68]])), copy<@type[[TYPE_S33]], reason=arg>(read<@type[[TYPE_S33]]>(%[[VALUE_g2s33]])), copy<@type[[TYPE_S33]], reason=arg>(read<@type[[TYPE_S33]]>(%[[VALUE_s2_68]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit33:[0-9]+]] @testit33() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S33]]>, i32) -> void>(%[[VALUE_init33]], addr_of<ptr<@type[[TYPE_S33]]>>(%[[VALUE_g1s33]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S33]]>, i32) -> void>(%[[VALUE_check33]], addr_of<ptr<@type[[TYPE_S33]]>>(%[[VALUE_g1s33]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S33]]>, i32) -> void>(%[[VALUE_init33]], addr_of<ptr<@type[[TYPE_S33]]>>(%[[VALUE_g2s33]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S33]]>, i32) -> void>(%[[VALUE_check33]], addr_of<ptr<@type[[TYPE_S33]]>>(%[[VALUE_g2s33]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S33]]>, i32) -> void>(%[[VALUE_init33]], addr_of<ptr<@type[[TYPE_S33]]>>(%[[VALUE_g3s33]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S33]]>, i32) -> void>(%[[VALUE_check33]], addr_of<ptr<@type[[TYPE_S33]]>>(%[[VALUE_g3s33]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S33]], @type[[TYPE_S33]], @type[[TYPE_S33]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test33]], copy<@type[[TYPE_S33]], reason=arg>(read<@type[[TYPE_S33]]>(%[[VALUE_g1s33]])), copy<@type[[TYPE_S33]], reason=arg>(read<@type[[TYPE_S33]]>(%[[VALUE_g2s33]])), copy<@type[[TYPE_S33]], reason=arg>(read<@type[[TYPE_S33]]>(%[[VALUE_g3s33]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S33]], @type[[TYPE_S33]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_33]], copy<@type[[TYPE_S33]], reason=arg>(read<@type[[TYPE_S33]]>(%[[VALUE_g1s33]])), copy<@type[[TYPE_S33]], reason=arg>(read<@type[[TYPE_S33]]>(%[[VALUE_g3s33]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init34:[0-9]+]] @init34(%[[VALUE_p_69:[0-9]+]] p: ptr<@type[[TYPE_S34]]>, %[[VALUE_i_69:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_69:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE205:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_69]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_69]]), const<i32>(34))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE206:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_69]]);
// DEFAULT-NEXT:                 let %[[VALUE207:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE206]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_69]], read<i32>(%[[VALUE207]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(34)>(field0(deref(read<ptr<@type[[TYPE_S34]]>>(%[[VALUE_p_69]])))), read<i32>(%[[VALUE_j_69]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_69]]), read<i32>(%[[VALUE_j_69]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check34:[0-9]+]] @check34(%[[VALUE_p_70:[0-9]+]] p: ptr<@type[[TYPE_S34]]>, %[[VALUE_i_70:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_70:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE208:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_70]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_70]]), const<i32>(34))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE209:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_70]]);
// DEFAULT-NEXT:                 let %[[VALUE210:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE209]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_70]], read<i32>(%[[VALUE210]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(34)>(field0(deref(read<ptr<@type[[TYPE_S34]]>>(%[[VALUE_p_70]])))), read<i32>(%[[VALUE_j_70]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_70]]), read<i32>(%[[VALUE_j_70]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test34:[0-9]+]] @test34(%[[VALUE_s1_69:[0-9]+]] s1: @type[[TYPE_S34]], %[[VALUE_s2_69:[0-9]+]] s2: @type[[TYPE_S34]], %[[VALUE_s3_35:[0-9]+]] s3: @type[[TYPE_S34]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S34]]>, i32) -> void>(%[[VALUE_check34]], addr_of<ptr<@type[[TYPE_S34]]>>(%[[VALUE_s1_69]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S34]]>, i32) -> void>(%[[VALUE_check34]], addr_of<ptr<@type[[TYPE_S34]]>>(%[[VALUE_s2_69]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S34]]>, i32) -> void>(%[[VALUE_check34]], addr_of<ptr<@type[[TYPE_S34]]>>(%[[VALUE_s3_35]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_34:[0-9]+]] @test2_34(%[[VALUE_s1_70:[0-9]+]] s1: @type[[TYPE_S34]], %[[VALUE_s2_70:[0-9]+]] s2: @type[[TYPE_S34]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S34]], @type[[TYPE_S34]], @type[[TYPE_S34]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test34]], copy<@type[[TYPE_S34]], reason=arg>(read<@type[[TYPE_S34]]>(%[[VALUE_s1_70]])), copy<@type[[TYPE_S34]], reason=arg>(read<@type[[TYPE_S34]]>(%[[VALUE_g2s34]])), copy<@type[[TYPE_S34]], reason=arg>(read<@type[[TYPE_S34]]>(%[[VALUE_s2_70]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit34:[0-9]+]] @testit34() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S34]]>, i32) -> void>(%[[VALUE_init34]], addr_of<ptr<@type[[TYPE_S34]]>>(%[[VALUE_g1s34]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S34]]>, i32) -> void>(%[[VALUE_check34]], addr_of<ptr<@type[[TYPE_S34]]>>(%[[VALUE_g1s34]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S34]]>, i32) -> void>(%[[VALUE_init34]], addr_of<ptr<@type[[TYPE_S34]]>>(%[[VALUE_g2s34]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S34]]>, i32) -> void>(%[[VALUE_check34]], addr_of<ptr<@type[[TYPE_S34]]>>(%[[VALUE_g2s34]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S34]]>, i32) -> void>(%[[VALUE_init34]], addr_of<ptr<@type[[TYPE_S34]]>>(%[[VALUE_g3s34]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S34]]>, i32) -> void>(%[[VALUE_check34]], addr_of<ptr<@type[[TYPE_S34]]>>(%[[VALUE_g3s34]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S34]], @type[[TYPE_S34]], @type[[TYPE_S34]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test34]], copy<@type[[TYPE_S34]], reason=arg>(read<@type[[TYPE_S34]]>(%[[VALUE_g1s34]])), copy<@type[[TYPE_S34]], reason=arg>(read<@type[[TYPE_S34]]>(%[[VALUE_g2s34]])), copy<@type[[TYPE_S34]], reason=arg>(read<@type[[TYPE_S34]]>(%[[VALUE_g3s34]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S34]], @type[[TYPE_S34]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_34]], copy<@type[[TYPE_S34]], reason=arg>(read<@type[[TYPE_S34]]>(%[[VALUE_g1s34]])), copy<@type[[TYPE_S34]], reason=arg>(read<@type[[TYPE_S34]]>(%[[VALUE_g3s34]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init35:[0-9]+]] @init35(%[[VALUE_p_71:[0-9]+]] p: ptr<@type[[TYPE_S35]]>, %[[VALUE_i_71:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_71:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE211:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_71]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_71]]), const<i32>(35))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE212:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_71]]);
// DEFAULT-NEXT:                 let %[[VALUE213:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE212]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_71]], read<i32>(%[[VALUE213]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(35)>(field0(deref(read<ptr<@type[[TYPE_S35]]>>(%[[VALUE_p_71]])))), read<i32>(%[[VALUE_j_71]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_71]]), read<i32>(%[[VALUE_j_71]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check35:[0-9]+]] @check35(%[[VALUE_p_72:[0-9]+]] p: ptr<@type[[TYPE_S35]]>, %[[VALUE_i_72:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_72:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE214:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_72]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_72]]), const<i32>(35))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE215:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_72]]);
// DEFAULT-NEXT:                 let %[[VALUE216:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE215]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_72]], read<i32>(%[[VALUE216]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(35)>(field0(deref(read<ptr<@type[[TYPE_S35]]>>(%[[VALUE_p_72]])))), read<i32>(%[[VALUE_j_72]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_72]]), read<i32>(%[[VALUE_j_72]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test35:[0-9]+]] @test35(%[[VALUE_s1_71:[0-9]+]] s1: @type[[TYPE_S35]], %[[VALUE_s2_71:[0-9]+]] s2: @type[[TYPE_S35]], %[[VALUE_s3_36:[0-9]+]] s3: @type[[TYPE_S35]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S35]]>, i32) -> void>(%[[VALUE_check35]], addr_of<ptr<@type[[TYPE_S35]]>>(%[[VALUE_s1_71]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S35]]>, i32) -> void>(%[[VALUE_check35]], addr_of<ptr<@type[[TYPE_S35]]>>(%[[VALUE_s2_71]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S35]]>, i32) -> void>(%[[VALUE_check35]], addr_of<ptr<@type[[TYPE_S35]]>>(%[[VALUE_s3_36]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_35:[0-9]+]] @test2_35(%[[VALUE_s1_72:[0-9]+]] s1: @type[[TYPE_S35]], %[[VALUE_s2_72:[0-9]+]] s2: @type[[TYPE_S35]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S35]], @type[[TYPE_S35]], @type[[TYPE_S35]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test35]], copy<@type[[TYPE_S35]], reason=arg>(read<@type[[TYPE_S35]]>(%[[VALUE_s1_72]])), copy<@type[[TYPE_S35]], reason=arg>(read<@type[[TYPE_S35]]>(%[[VALUE_g2s35]])), copy<@type[[TYPE_S35]], reason=arg>(read<@type[[TYPE_S35]]>(%[[VALUE_s2_72]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit35:[0-9]+]] @testit35() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S35]]>, i32) -> void>(%[[VALUE_init35]], addr_of<ptr<@type[[TYPE_S35]]>>(%[[VALUE_g1s35]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S35]]>, i32) -> void>(%[[VALUE_check35]], addr_of<ptr<@type[[TYPE_S35]]>>(%[[VALUE_g1s35]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S35]]>, i32) -> void>(%[[VALUE_init35]], addr_of<ptr<@type[[TYPE_S35]]>>(%[[VALUE_g2s35]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S35]]>, i32) -> void>(%[[VALUE_check35]], addr_of<ptr<@type[[TYPE_S35]]>>(%[[VALUE_g2s35]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S35]]>, i32) -> void>(%[[VALUE_init35]], addr_of<ptr<@type[[TYPE_S35]]>>(%[[VALUE_g3s35]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S35]]>, i32) -> void>(%[[VALUE_check35]], addr_of<ptr<@type[[TYPE_S35]]>>(%[[VALUE_g3s35]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S35]], @type[[TYPE_S35]], @type[[TYPE_S35]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test35]], copy<@type[[TYPE_S35]], reason=arg>(read<@type[[TYPE_S35]]>(%[[VALUE_g1s35]])), copy<@type[[TYPE_S35]], reason=arg>(read<@type[[TYPE_S35]]>(%[[VALUE_g2s35]])), copy<@type[[TYPE_S35]], reason=arg>(read<@type[[TYPE_S35]]>(%[[VALUE_g3s35]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S35]], @type[[TYPE_S35]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_35]], copy<@type[[TYPE_S35]], reason=arg>(read<@type[[TYPE_S35]]>(%[[VALUE_g1s35]])), copy<@type[[TYPE_S35]], reason=arg>(read<@type[[TYPE_S35]]>(%[[VALUE_g3s35]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init36:[0-9]+]] @init36(%[[VALUE_p_73:[0-9]+]] p: ptr<@type[[TYPE_S36]]>, %[[VALUE_i_73:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_73:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE217:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_73]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_73]]), const<i32>(36))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE218:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_73]]);
// DEFAULT-NEXT:                 let %[[VALUE219:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE218]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_73]], read<i32>(%[[VALUE219]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(36)>(field0(deref(read<ptr<@type[[TYPE_S36]]>>(%[[VALUE_p_73]])))), read<i32>(%[[VALUE_j_73]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_73]]), read<i32>(%[[VALUE_j_73]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check36:[0-9]+]] @check36(%[[VALUE_p_74:[0-9]+]] p: ptr<@type[[TYPE_S36]]>, %[[VALUE_i_74:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_74:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE220:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_74]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_74]]), const<i32>(36))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE221:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_74]]);
// DEFAULT-NEXT:                 let %[[VALUE222:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE221]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_74]], read<i32>(%[[VALUE222]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(36)>(field0(deref(read<ptr<@type[[TYPE_S36]]>>(%[[VALUE_p_74]])))), read<i32>(%[[VALUE_j_74]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_74]]), read<i32>(%[[VALUE_j_74]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test36:[0-9]+]] @test36(%[[VALUE_s1_73:[0-9]+]] s1: @type[[TYPE_S36]], %[[VALUE_s2_73:[0-9]+]] s2: @type[[TYPE_S36]], %[[VALUE_s3_37:[0-9]+]] s3: @type[[TYPE_S36]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S36]]>, i32) -> void>(%[[VALUE_check36]], addr_of<ptr<@type[[TYPE_S36]]>>(%[[VALUE_s1_73]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S36]]>, i32) -> void>(%[[VALUE_check36]], addr_of<ptr<@type[[TYPE_S36]]>>(%[[VALUE_s2_73]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S36]]>, i32) -> void>(%[[VALUE_check36]], addr_of<ptr<@type[[TYPE_S36]]>>(%[[VALUE_s3_37]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_36:[0-9]+]] @test2_36(%[[VALUE_s1_74:[0-9]+]] s1: @type[[TYPE_S36]], %[[VALUE_s2_74:[0-9]+]] s2: @type[[TYPE_S36]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S36]], @type[[TYPE_S36]], @type[[TYPE_S36]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test36]], copy<@type[[TYPE_S36]], reason=arg>(read<@type[[TYPE_S36]]>(%[[VALUE_s1_74]])), copy<@type[[TYPE_S36]], reason=arg>(read<@type[[TYPE_S36]]>(%[[VALUE_g2s36]])), copy<@type[[TYPE_S36]], reason=arg>(read<@type[[TYPE_S36]]>(%[[VALUE_s2_74]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit36:[0-9]+]] @testit36() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S36]]>, i32) -> void>(%[[VALUE_init36]], addr_of<ptr<@type[[TYPE_S36]]>>(%[[VALUE_g1s36]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S36]]>, i32) -> void>(%[[VALUE_check36]], addr_of<ptr<@type[[TYPE_S36]]>>(%[[VALUE_g1s36]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S36]]>, i32) -> void>(%[[VALUE_init36]], addr_of<ptr<@type[[TYPE_S36]]>>(%[[VALUE_g2s36]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S36]]>, i32) -> void>(%[[VALUE_check36]], addr_of<ptr<@type[[TYPE_S36]]>>(%[[VALUE_g2s36]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S36]]>, i32) -> void>(%[[VALUE_init36]], addr_of<ptr<@type[[TYPE_S36]]>>(%[[VALUE_g3s36]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S36]]>, i32) -> void>(%[[VALUE_check36]], addr_of<ptr<@type[[TYPE_S36]]>>(%[[VALUE_g3s36]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S36]], @type[[TYPE_S36]], @type[[TYPE_S36]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test36]], copy<@type[[TYPE_S36]], reason=arg>(read<@type[[TYPE_S36]]>(%[[VALUE_g1s36]])), copy<@type[[TYPE_S36]], reason=arg>(read<@type[[TYPE_S36]]>(%[[VALUE_g2s36]])), copy<@type[[TYPE_S36]], reason=arg>(read<@type[[TYPE_S36]]>(%[[VALUE_g3s36]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S36]], @type[[TYPE_S36]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_36]], copy<@type[[TYPE_S36]], reason=arg>(read<@type[[TYPE_S36]]>(%[[VALUE_g1s36]])), copy<@type[[TYPE_S36]], reason=arg>(read<@type[[TYPE_S36]]>(%[[VALUE_g3s36]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init37:[0-9]+]] @init37(%[[VALUE_p_75:[0-9]+]] p: ptr<@type[[TYPE_S37]]>, %[[VALUE_i_75:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_75:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE223:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_75]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_75]]), const<i32>(37))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE224:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_75]]);
// DEFAULT-NEXT:                 let %[[VALUE225:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE224]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_75]], read<i32>(%[[VALUE225]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(37)>(field0(deref(read<ptr<@type[[TYPE_S37]]>>(%[[VALUE_p_75]])))), read<i32>(%[[VALUE_j_75]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_75]]), read<i32>(%[[VALUE_j_75]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check37:[0-9]+]] @check37(%[[VALUE_p_76:[0-9]+]] p: ptr<@type[[TYPE_S37]]>, %[[VALUE_i_76:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_76:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE226:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_76]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_76]]), const<i32>(37))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE227:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_76]]);
// DEFAULT-NEXT:                 let %[[VALUE228:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE227]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_76]], read<i32>(%[[VALUE228]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(37)>(field0(deref(read<ptr<@type[[TYPE_S37]]>>(%[[VALUE_p_76]])))), read<i32>(%[[VALUE_j_76]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_76]]), read<i32>(%[[VALUE_j_76]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test37:[0-9]+]] @test37(%[[VALUE_s1_75:[0-9]+]] s1: @type[[TYPE_S37]], %[[VALUE_s2_75:[0-9]+]] s2: @type[[TYPE_S37]], %[[VALUE_s3_38:[0-9]+]] s3: @type[[TYPE_S37]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S37]]>, i32) -> void>(%[[VALUE_check37]], addr_of<ptr<@type[[TYPE_S37]]>>(%[[VALUE_s1_75]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S37]]>, i32) -> void>(%[[VALUE_check37]], addr_of<ptr<@type[[TYPE_S37]]>>(%[[VALUE_s2_75]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S37]]>, i32) -> void>(%[[VALUE_check37]], addr_of<ptr<@type[[TYPE_S37]]>>(%[[VALUE_s3_38]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_37:[0-9]+]] @test2_37(%[[VALUE_s1_76:[0-9]+]] s1: @type[[TYPE_S37]], %[[VALUE_s2_76:[0-9]+]] s2: @type[[TYPE_S37]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S37]], @type[[TYPE_S37]], @type[[TYPE_S37]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test37]], copy<@type[[TYPE_S37]], reason=arg>(read<@type[[TYPE_S37]]>(%[[VALUE_s1_76]])), copy<@type[[TYPE_S37]], reason=arg>(read<@type[[TYPE_S37]]>(%[[VALUE_g2s37]])), copy<@type[[TYPE_S37]], reason=arg>(read<@type[[TYPE_S37]]>(%[[VALUE_s2_76]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit37:[0-9]+]] @testit37() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S37]]>, i32) -> void>(%[[VALUE_init37]], addr_of<ptr<@type[[TYPE_S37]]>>(%[[VALUE_g1s37]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S37]]>, i32) -> void>(%[[VALUE_check37]], addr_of<ptr<@type[[TYPE_S37]]>>(%[[VALUE_g1s37]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S37]]>, i32) -> void>(%[[VALUE_init37]], addr_of<ptr<@type[[TYPE_S37]]>>(%[[VALUE_g2s37]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S37]]>, i32) -> void>(%[[VALUE_check37]], addr_of<ptr<@type[[TYPE_S37]]>>(%[[VALUE_g2s37]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S37]]>, i32) -> void>(%[[VALUE_init37]], addr_of<ptr<@type[[TYPE_S37]]>>(%[[VALUE_g3s37]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S37]]>, i32) -> void>(%[[VALUE_check37]], addr_of<ptr<@type[[TYPE_S37]]>>(%[[VALUE_g3s37]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S37]], @type[[TYPE_S37]], @type[[TYPE_S37]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test37]], copy<@type[[TYPE_S37]], reason=arg>(read<@type[[TYPE_S37]]>(%[[VALUE_g1s37]])), copy<@type[[TYPE_S37]], reason=arg>(read<@type[[TYPE_S37]]>(%[[VALUE_g2s37]])), copy<@type[[TYPE_S37]], reason=arg>(read<@type[[TYPE_S37]]>(%[[VALUE_g3s37]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S37]], @type[[TYPE_S37]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_37]], copy<@type[[TYPE_S37]], reason=arg>(read<@type[[TYPE_S37]]>(%[[VALUE_g1s37]])), copy<@type[[TYPE_S37]], reason=arg>(read<@type[[TYPE_S37]]>(%[[VALUE_g3s37]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init38:[0-9]+]] @init38(%[[VALUE_p_77:[0-9]+]] p: ptr<@type[[TYPE_S38]]>, %[[VALUE_i_77:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_77:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE229:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_77]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_77]]), const<i32>(38))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE230:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_77]]);
// DEFAULT-NEXT:                 let %[[VALUE231:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE230]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_77]], read<i32>(%[[VALUE231]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(38)>(field0(deref(read<ptr<@type[[TYPE_S38]]>>(%[[VALUE_p_77]])))), read<i32>(%[[VALUE_j_77]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_77]]), read<i32>(%[[VALUE_j_77]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check38:[0-9]+]] @check38(%[[VALUE_p_78:[0-9]+]] p: ptr<@type[[TYPE_S38]]>, %[[VALUE_i_78:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_78:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE232:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_78]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_78]]), const<i32>(38))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE233:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_78]]);
// DEFAULT-NEXT:                 let %[[VALUE234:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE233]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_78]], read<i32>(%[[VALUE234]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(38)>(field0(deref(read<ptr<@type[[TYPE_S38]]>>(%[[VALUE_p_78]])))), read<i32>(%[[VALUE_j_78]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_78]]), read<i32>(%[[VALUE_j_78]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test38:[0-9]+]] @test38(%[[VALUE_s1_77:[0-9]+]] s1: @type[[TYPE_S38]], %[[VALUE_s2_77:[0-9]+]] s2: @type[[TYPE_S38]], %[[VALUE_s3_39:[0-9]+]] s3: @type[[TYPE_S38]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S38]]>, i32) -> void>(%[[VALUE_check38]], addr_of<ptr<@type[[TYPE_S38]]>>(%[[VALUE_s1_77]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S38]]>, i32) -> void>(%[[VALUE_check38]], addr_of<ptr<@type[[TYPE_S38]]>>(%[[VALUE_s2_77]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S38]]>, i32) -> void>(%[[VALUE_check38]], addr_of<ptr<@type[[TYPE_S38]]>>(%[[VALUE_s3_39]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_38:[0-9]+]] @test2_38(%[[VALUE_s1_78:[0-9]+]] s1: @type[[TYPE_S38]], %[[VALUE_s2_78:[0-9]+]] s2: @type[[TYPE_S38]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S38]], @type[[TYPE_S38]], @type[[TYPE_S38]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test38]], copy<@type[[TYPE_S38]], reason=arg>(read<@type[[TYPE_S38]]>(%[[VALUE_s1_78]])), copy<@type[[TYPE_S38]], reason=arg>(read<@type[[TYPE_S38]]>(%[[VALUE_g2s38]])), copy<@type[[TYPE_S38]], reason=arg>(read<@type[[TYPE_S38]]>(%[[VALUE_s2_78]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit38:[0-9]+]] @testit38() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S38]]>, i32) -> void>(%[[VALUE_init38]], addr_of<ptr<@type[[TYPE_S38]]>>(%[[VALUE_g1s38]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S38]]>, i32) -> void>(%[[VALUE_check38]], addr_of<ptr<@type[[TYPE_S38]]>>(%[[VALUE_g1s38]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S38]]>, i32) -> void>(%[[VALUE_init38]], addr_of<ptr<@type[[TYPE_S38]]>>(%[[VALUE_g2s38]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S38]]>, i32) -> void>(%[[VALUE_check38]], addr_of<ptr<@type[[TYPE_S38]]>>(%[[VALUE_g2s38]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S38]]>, i32) -> void>(%[[VALUE_init38]], addr_of<ptr<@type[[TYPE_S38]]>>(%[[VALUE_g3s38]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S38]]>, i32) -> void>(%[[VALUE_check38]], addr_of<ptr<@type[[TYPE_S38]]>>(%[[VALUE_g3s38]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S38]], @type[[TYPE_S38]], @type[[TYPE_S38]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test38]], copy<@type[[TYPE_S38]], reason=arg>(read<@type[[TYPE_S38]]>(%[[VALUE_g1s38]])), copy<@type[[TYPE_S38]], reason=arg>(read<@type[[TYPE_S38]]>(%[[VALUE_g2s38]])), copy<@type[[TYPE_S38]], reason=arg>(read<@type[[TYPE_S38]]>(%[[VALUE_g3s38]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S38]], @type[[TYPE_S38]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_38]], copy<@type[[TYPE_S38]], reason=arg>(read<@type[[TYPE_S38]]>(%[[VALUE_g1s38]])), copy<@type[[TYPE_S38]], reason=arg>(read<@type[[TYPE_S38]]>(%[[VALUE_g3s38]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init39:[0-9]+]] @init39(%[[VALUE_p_79:[0-9]+]] p: ptr<@type[[TYPE_S39]]>, %[[VALUE_i_79:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_79:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE235:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_79]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_79]]), const<i32>(39))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE236:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_79]]);
// DEFAULT-NEXT:                 let %[[VALUE237:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE236]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_79]], read<i32>(%[[VALUE237]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(39)>(field0(deref(read<ptr<@type[[TYPE_S39]]>>(%[[VALUE_p_79]])))), read<i32>(%[[VALUE_j_79]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_79]]), read<i32>(%[[VALUE_j_79]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check39:[0-9]+]] @check39(%[[VALUE_p_80:[0-9]+]] p: ptr<@type[[TYPE_S39]]>, %[[VALUE_i_80:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_80:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE238:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_80]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_80]]), const<i32>(39))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE239:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_80]]);
// DEFAULT-NEXT:                 let %[[VALUE240:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE239]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_80]], read<i32>(%[[VALUE240]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(39)>(field0(deref(read<ptr<@type[[TYPE_S39]]>>(%[[VALUE_p_80]])))), read<i32>(%[[VALUE_j_80]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_80]]), read<i32>(%[[VALUE_j_80]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test39:[0-9]+]] @test39(%[[VALUE_s1_79:[0-9]+]] s1: @type[[TYPE_S39]], %[[VALUE_s2_79:[0-9]+]] s2: @type[[TYPE_S39]], %[[VALUE_s3_40:[0-9]+]] s3: @type[[TYPE_S39]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S39]]>, i32) -> void>(%[[VALUE_check39]], addr_of<ptr<@type[[TYPE_S39]]>>(%[[VALUE_s1_79]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S39]]>, i32) -> void>(%[[VALUE_check39]], addr_of<ptr<@type[[TYPE_S39]]>>(%[[VALUE_s2_79]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S39]]>, i32) -> void>(%[[VALUE_check39]], addr_of<ptr<@type[[TYPE_S39]]>>(%[[VALUE_s3_40]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_39:[0-9]+]] @test2_39(%[[VALUE_s1_80:[0-9]+]] s1: @type[[TYPE_S39]], %[[VALUE_s2_80:[0-9]+]] s2: @type[[TYPE_S39]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S39]], @type[[TYPE_S39]], @type[[TYPE_S39]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test39]], copy<@type[[TYPE_S39]], reason=arg>(read<@type[[TYPE_S39]]>(%[[VALUE_s1_80]])), copy<@type[[TYPE_S39]], reason=arg>(read<@type[[TYPE_S39]]>(%[[VALUE_g2s39]])), copy<@type[[TYPE_S39]], reason=arg>(read<@type[[TYPE_S39]]>(%[[VALUE_s2_80]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit39:[0-9]+]] @testit39() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S39]]>, i32) -> void>(%[[VALUE_init39]], addr_of<ptr<@type[[TYPE_S39]]>>(%[[VALUE_g1s39]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S39]]>, i32) -> void>(%[[VALUE_check39]], addr_of<ptr<@type[[TYPE_S39]]>>(%[[VALUE_g1s39]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S39]]>, i32) -> void>(%[[VALUE_init39]], addr_of<ptr<@type[[TYPE_S39]]>>(%[[VALUE_g2s39]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S39]]>, i32) -> void>(%[[VALUE_check39]], addr_of<ptr<@type[[TYPE_S39]]>>(%[[VALUE_g2s39]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S39]]>, i32) -> void>(%[[VALUE_init39]], addr_of<ptr<@type[[TYPE_S39]]>>(%[[VALUE_g3s39]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S39]]>, i32) -> void>(%[[VALUE_check39]], addr_of<ptr<@type[[TYPE_S39]]>>(%[[VALUE_g3s39]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S39]], @type[[TYPE_S39]], @type[[TYPE_S39]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test39]], copy<@type[[TYPE_S39]], reason=arg>(read<@type[[TYPE_S39]]>(%[[VALUE_g1s39]])), copy<@type[[TYPE_S39]], reason=arg>(read<@type[[TYPE_S39]]>(%[[VALUE_g2s39]])), copy<@type[[TYPE_S39]], reason=arg>(read<@type[[TYPE_S39]]>(%[[VALUE_g3s39]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S39]], @type[[TYPE_S39]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_39]], copy<@type[[TYPE_S39]], reason=arg>(read<@type[[TYPE_S39]]>(%[[VALUE_g1s39]])), copy<@type[[TYPE_S39]], reason=arg>(read<@type[[TYPE_S39]]>(%[[VALUE_g3s39]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init40:[0-9]+]] @init40(%[[VALUE_p_81:[0-9]+]] p: ptr<@type[[TYPE_S40]]>, %[[VALUE_i_81:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_81:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE241:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_81]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_81]]), const<i32>(40))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE242:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_81]]);
// DEFAULT-NEXT:                 let %[[VALUE243:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE242]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_81]], read<i32>(%[[VALUE243]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(40)>(field0(deref(read<ptr<@type[[TYPE_S40]]>>(%[[VALUE_p_81]])))), read<i32>(%[[VALUE_j_81]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_81]]), read<i32>(%[[VALUE_j_81]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check40:[0-9]+]] @check40(%[[VALUE_p_82:[0-9]+]] p: ptr<@type[[TYPE_S40]]>, %[[VALUE_i_82:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_82:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE244:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_82]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_82]]), const<i32>(40))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE245:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_82]]);
// DEFAULT-NEXT:                 let %[[VALUE246:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE245]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_82]], read<i32>(%[[VALUE246]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(40)>(field0(deref(read<ptr<@type[[TYPE_S40]]>>(%[[VALUE_p_82]])))), read<i32>(%[[VALUE_j_82]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_82]]), read<i32>(%[[VALUE_j_82]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test40:[0-9]+]] @test40(%[[VALUE_s1_81:[0-9]+]] s1: @type[[TYPE_S40]], %[[VALUE_s2_81:[0-9]+]] s2: @type[[TYPE_S40]], %[[VALUE_s3_41:[0-9]+]] s3: @type[[TYPE_S40]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S40]]>, i32) -> void>(%[[VALUE_check40]], addr_of<ptr<@type[[TYPE_S40]]>>(%[[VALUE_s1_81]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S40]]>, i32) -> void>(%[[VALUE_check40]], addr_of<ptr<@type[[TYPE_S40]]>>(%[[VALUE_s2_81]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S40]]>, i32) -> void>(%[[VALUE_check40]], addr_of<ptr<@type[[TYPE_S40]]>>(%[[VALUE_s3_41]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_40:[0-9]+]] @test2_40(%[[VALUE_s1_82:[0-9]+]] s1: @type[[TYPE_S40]], %[[VALUE_s2_82:[0-9]+]] s2: @type[[TYPE_S40]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S40]], @type[[TYPE_S40]], @type[[TYPE_S40]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test40]], copy<@type[[TYPE_S40]], reason=arg>(read<@type[[TYPE_S40]]>(%[[VALUE_s1_82]])), copy<@type[[TYPE_S40]], reason=arg>(read<@type[[TYPE_S40]]>(%[[VALUE_g2s40]])), copy<@type[[TYPE_S40]], reason=arg>(read<@type[[TYPE_S40]]>(%[[VALUE_s2_82]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit40:[0-9]+]] @testit40() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S40]]>, i32) -> void>(%[[VALUE_init40]], addr_of<ptr<@type[[TYPE_S40]]>>(%[[VALUE_g1s40]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S40]]>, i32) -> void>(%[[VALUE_check40]], addr_of<ptr<@type[[TYPE_S40]]>>(%[[VALUE_g1s40]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S40]]>, i32) -> void>(%[[VALUE_init40]], addr_of<ptr<@type[[TYPE_S40]]>>(%[[VALUE_g2s40]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S40]]>, i32) -> void>(%[[VALUE_check40]], addr_of<ptr<@type[[TYPE_S40]]>>(%[[VALUE_g2s40]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S40]]>, i32) -> void>(%[[VALUE_init40]], addr_of<ptr<@type[[TYPE_S40]]>>(%[[VALUE_g3s40]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S40]]>, i32) -> void>(%[[VALUE_check40]], addr_of<ptr<@type[[TYPE_S40]]>>(%[[VALUE_g3s40]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S40]], @type[[TYPE_S40]], @type[[TYPE_S40]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test40]], copy<@type[[TYPE_S40]], reason=arg>(read<@type[[TYPE_S40]]>(%[[VALUE_g1s40]])), copy<@type[[TYPE_S40]], reason=arg>(read<@type[[TYPE_S40]]>(%[[VALUE_g2s40]])), copy<@type[[TYPE_S40]], reason=arg>(read<@type[[TYPE_S40]]>(%[[VALUE_g3s40]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S40]], @type[[TYPE_S40]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_40]], copy<@type[[TYPE_S40]], reason=arg>(read<@type[[TYPE_S40]]>(%[[VALUE_g1s40]])), copy<@type[[TYPE_S40]], reason=arg>(read<@type[[TYPE_S40]]>(%[[VALUE_g3s40]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init41:[0-9]+]] @init41(%[[VALUE_p_83:[0-9]+]] p: ptr<@type[[TYPE_S41]]>, %[[VALUE_i_83:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_83:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE247:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_83]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_83]]), const<i32>(41))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE248:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_83]]);
// DEFAULT-NEXT:                 let %[[VALUE249:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE248]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_83]], read<i32>(%[[VALUE249]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(41)>(field0(deref(read<ptr<@type[[TYPE_S41]]>>(%[[VALUE_p_83]])))), read<i32>(%[[VALUE_j_83]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_83]]), read<i32>(%[[VALUE_j_83]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check41:[0-9]+]] @check41(%[[VALUE_p_84:[0-9]+]] p: ptr<@type[[TYPE_S41]]>, %[[VALUE_i_84:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_84:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE250:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_84]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_84]]), const<i32>(41))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE251:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_84]]);
// DEFAULT-NEXT:                 let %[[VALUE252:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE251]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_84]], read<i32>(%[[VALUE252]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(41)>(field0(deref(read<ptr<@type[[TYPE_S41]]>>(%[[VALUE_p_84]])))), read<i32>(%[[VALUE_j_84]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_84]]), read<i32>(%[[VALUE_j_84]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test41:[0-9]+]] @test41(%[[VALUE_s1_83:[0-9]+]] s1: @type[[TYPE_S41]], %[[VALUE_s2_83:[0-9]+]] s2: @type[[TYPE_S41]], %[[VALUE_s3_42:[0-9]+]] s3: @type[[TYPE_S41]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S41]]>, i32) -> void>(%[[VALUE_check41]], addr_of<ptr<@type[[TYPE_S41]]>>(%[[VALUE_s1_83]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S41]]>, i32) -> void>(%[[VALUE_check41]], addr_of<ptr<@type[[TYPE_S41]]>>(%[[VALUE_s2_83]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S41]]>, i32) -> void>(%[[VALUE_check41]], addr_of<ptr<@type[[TYPE_S41]]>>(%[[VALUE_s3_42]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_41:[0-9]+]] @test2_41(%[[VALUE_s1_84:[0-9]+]] s1: @type[[TYPE_S41]], %[[VALUE_s2_84:[0-9]+]] s2: @type[[TYPE_S41]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S41]], @type[[TYPE_S41]], @type[[TYPE_S41]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test41]], copy<@type[[TYPE_S41]], reason=arg>(read<@type[[TYPE_S41]]>(%[[VALUE_s1_84]])), copy<@type[[TYPE_S41]], reason=arg>(read<@type[[TYPE_S41]]>(%[[VALUE_g2s41]])), copy<@type[[TYPE_S41]], reason=arg>(read<@type[[TYPE_S41]]>(%[[VALUE_s2_84]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit41:[0-9]+]] @testit41() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S41]]>, i32) -> void>(%[[VALUE_init41]], addr_of<ptr<@type[[TYPE_S41]]>>(%[[VALUE_g1s41]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S41]]>, i32) -> void>(%[[VALUE_check41]], addr_of<ptr<@type[[TYPE_S41]]>>(%[[VALUE_g1s41]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S41]]>, i32) -> void>(%[[VALUE_init41]], addr_of<ptr<@type[[TYPE_S41]]>>(%[[VALUE_g2s41]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S41]]>, i32) -> void>(%[[VALUE_check41]], addr_of<ptr<@type[[TYPE_S41]]>>(%[[VALUE_g2s41]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S41]]>, i32) -> void>(%[[VALUE_init41]], addr_of<ptr<@type[[TYPE_S41]]>>(%[[VALUE_g3s41]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S41]]>, i32) -> void>(%[[VALUE_check41]], addr_of<ptr<@type[[TYPE_S41]]>>(%[[VALUE_g3s41]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S41]], @type[[TYPE_S41]], @type[[TYPE_S41]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test41]], copy<@type[[TYPE_S41]], reason=arg>(read<@type[[TYPE_S41]]>(%[[VALUE_g1s41]])), copy<@type[[TYPE_S41]], reason=arg>(read<@type[[TYPE_S41]]>(%[[VALUE_g2s41]])), copy<@type[[TYPE_S41]], reason=arg>(read<@type[[TYPE_S41]]>(%[[VALUE_g3s41]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S41]], @type[[TYPE_S41]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_41]], copy<@type[[TYPE_S41]], reason=arg>(read<@type[[TYPE_S41]]>(%[[VALUE_g1s41]])), copy<@type[[TYPE_S41]], reason=arg>(read<@type[[TYPE_S41]]>(%[[VALUE_g3s41]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init42:[0-9]+]] @init42(%[[VALUE_p_85:[0-9]+]] p: ptr<@type[[TYPE_S42]]>, %[[VALUE_i_85:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_85:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE253:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_85]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_85]]), const<i32>(42))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE254:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_85]]);
// DEFAULT-NEXT:                 let %[[VALUE255:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE254]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_85]], read<i32>(%[[VALUE255]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(42)>(field0(deref(read<ptr<@type[[TYPE_S42]]>>(%[[VALUE_p_85]])))), read<i32>(%[[VALUE_j_85]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_85]]), read<i32>(%[[VALUE_j_85]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check42:[0-9]+]] @check42(%[[VALUE_p_86:[0-9]+]] p: ptr<@type[[TYPE_S42]]>, %[[VALUE_i_86:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_86:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE256:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_86]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_86]]), const<i32>(42))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE257:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_86]]);
// DEFAULT-NEXT:                 let %[[VALUE258:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE257]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_86]], read<i32>(%[[VALUE258]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(42)>(field0(deref(read<ptr<@type[[TYPE_S42]]>>(%[[VALUE_p_86]])))), read<i32>(%[[VALUE_j_86]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_86]]), read<i32>(%[[VALUE_j_86]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test42:[0-9]+]] @test42(%[[VALUE_s1_85:[0-9]+]] s1: @type[[TYPE_S42]], %[[VALUE_s2_85:[0-9]+]] s2: @type[[TYPE_S42]], %[[VALUE_s3_43:[0-9]+]] s3: @type[[TYPE_S42]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S42]]>, i32) -> void>(%[[VALUE_check42]], addr_of<ptr<@type[[TYPE_S42]]>>(%[[VALUE_s1_85]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S42]]>, i32) -> void>(%[[VALUE_check42]], addr_of<ptr<@type[[TYPE_S42]]>>(%[[VALUE_s2_85]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S42]]>, i32) -> void>(%[[VALUE_check42]], addr_of<ptr<@type[[TYPE_S42]]>>(%[[VALUE_s3_43]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_42:[0-9]+]] @test2_42(%[[VALUE_s1_86:[0-9]+]] s1: @type[[TYPE_S42]], %[[VALUE_s2_86:[0-9]+]] s2: @type[[TYPE_S42]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S42]], @type[[TYPE_S42]], @type[[TYPE_S42]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test42]], copy<@type[[TYPE_S42]], reason=arg>(read<@type[[TYPE_S42]]>(%[[VALUE_s1_86]])), copy<@type[[TYPE_S42]], reason=arg>(read<@type[[TYPE_S42]]>(%[[VALUE_g2s42]])), copy<@type[[TYPE_S42]], reason=arg>(read<@type[[TYPE_S42]]>(%[[VALUE_s2_86]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit42:[0-9]+]] @testit42() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S42]]>, i32) -> void>(%[[VALUE_init42]], addr_of<ptr<@type[[TYPE_S42]]>>(%[[VALUE_g1s42]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S42]]>, i32) -> void>(%[[VALUE_check42]], addr_of<ptr<@type[[TYPE_S42]]>>(%[[VALUE_g1s42]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S42]]>, i32) -> void>(%[[VALUE_init42]], addr_of<ptr<@type[[TYPE_S42]]>>(%[[VALUE_g2s42]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S42]]>, i32) -> void>(%[[VALUE_check42]], addr_of<ptr<@type[[TYPE_S42]]>>(%[[VALUE_g2s42]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S42]]>, i32) -> void>(%[[VALUE_init42]], addr_of<ptr<@type[[TYPE_S42]]>>(%[[VALUE_g3s42]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S42]]>, i32) -> void>(%[[VALUE_check42]], addr_of<ptr<@type[[TYPE_S42]]>>(%[[VALUE_g3s42]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S42]], @type[[TYPE_S42]], @type[[TYPE_S42]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test42]], copy<@type[[TYPE_S42]], reason=arg>(read<@type[[TYPE_S42]]>(%[[VALUE_g1s42]])), copy<@type[[TYPE_S42]], reason=arg>(read<@type[[TYPE_S42]]>(%[[VALUE_g2s42]])), copy<@type[[TYPE_S42]], reason=arg>(read<@type[[TYPE_S42]]>(%[[VALUE_g3s42]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S42]], @type[[TYPE_S42]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_42]], copy<@type[[TYPE_S42]], reason=arg>(read<@type[[TYPE_S42]]>(%[[VALUE_g1s42]])), copy<@type[[TYPE_S42]], reason=arg>(read<@type[[TYPE_S42]]>(%[[VALUE_g3s42]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init43:[0-9]+]] @init43(%[[VALUE_p_87:[0-9]+]] p: ptr<@type[[TYPE_S43]]>, %[[VALUE_i_87:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_87:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE259:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_87]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_87]]), const<i32>(43))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE260:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_87]]);
// DEFAULT-NEXT:                 let %[[VALUE261:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE260]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_87]], read<i32>(%[[VALUE261]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(43)>(field0(deref(read<ptr<@type[[TYPE_S43]]>>(%[[VALUE_p_87]])))), read<i32>(%[[VALUE_j_87]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_87]]), read<i32>(%[[VALUE_j_87]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check43:[0-9]+]] @check43(%[[VALUE_p_88:[0-9]+]] p: ptr<@type[[TYPE_S43]]>, %[[VALUE_i_88:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_88:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE262:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_88]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_88]]), const<i32>(43))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE263:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_88]]);
// DEFAULT-NEXT:                 let %[[VALUE264:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE263]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_88]], read<i32>(%[[VALUE264]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(43)>(field0(deref(read<ptr<@type[[TYPE_S43]]>>(%[[VALUE_p_88]])))), read<i32>(%[[VALUE_j_88]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_88]]), read<i32>(%[[VALUE_j_88]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test43:[0-9]+]] @test43(%[[VALUE_s1_87:[0-9]+]] s1: @type[[TYPE_S43]], %[[VALUE_s2_87:[0-9]+]] s2: @type[[TYPE_S43]], %[[VALUE_s3_44:[0-9]+]] s3: @type[[TYPE_S43]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S43]]>, i32) -> void>(%[[VALUE_check43]], addr_of<ptr<@type[[TYPE_S43]]>>(%[[VALUE_s1_87]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S43]]>, i32) -> void>(%[[VALUE_check43]], addr_of<ptr<@type[[TYPE_S43]]>>(%[[VALUE_s2_87]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S43]]>, i32) -> void>(%[[VALUE_check43]], addr_of<ptr<@type[[TYPE_S43]]>>(%[[VALUE_s3_44]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_43:[0-9]+]] @test2_43(%[[VALUE_s1_88:[0-9]+]] s1: @type[[TYPE_S43]], %[[VALUE_s2_88:[0-9]+]] s2: @type[[TYPE_S43]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S43]], @type[[TYPE_S43]], @type[[TYPE_S43]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test43]], copy<@type[[TYPE_S43]], reason=arg>(read<@type[[TYPE_S43]]>(%[[VALUE_s1_88]])), copy<@type[[TYPE_S43]], reason=arg>(read<@type[[TYPE_S43]]>(%[[VALUE_g2s43]])), copy<@type[[TYPE_S43]], reason=arg>(read<@type[[TYPE_S43]]>(%[[VALUE_s2_88]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit43:[0-9]+]] @testit43() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S43]]>, i32) -> void>(%[[VALUE_init43]], addr_of<ptr<@type[[TYPE_S43]]>>(%[[VALUE_g1s43]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S43]]>, i32) -> void>(%[[VALUE_check43]], addr_of<ptr<@type[[TYPE_S43]]>>(%[[VALUE_g1s43]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S43]]>, i32) -> void>(%[[VALUE_init43]], addr_of<ptr<@type[[TYPE_S43]]>>(%[[VALUE_g2s43]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S43]]>, i32) -> void>(%[[VALUE_check43]], addr_of<ptr<@type[[TYPE_S43]]>>(%[[VALUE_g2s43]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S43]]>, i32) -> void>(%[[VALUE_init43]], addr_of<ptr<@type[[TYPE_S43]]>>(%[[VALUE_g3s43]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S43]]>, i32) -> void>(%[[VALUE_check43]], addr_of<ptr<@type[[TYPE_S43]]>>(%[[VALUE_g3s43]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S43]], @type[[TYPE_S43]], @type[[TYPE_S43]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test43]], copy<@type[[TYPE_S43]], reason=arg>(read<@type[[TYPE_S43]]>(%[[VALUE_g1s43]])), copy<@type[[TYPE_S43]], reason=arg>(read<@type[[TYPE_S43]]>(%[[VALUE_g2s43]])), copy<@type[[TYPE_S43]], reason=arg>(read<@type[[TYPE_S43]]>(%[[VALUE_g3s43]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S43]], @type[[TYPE_S43]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_43]], copy<@type[[TYPE_S43]], reason=arg>(read<@type[[TYPE_S43]]>(%[[VALUE_g1s43]])), copy<@type[[TYPE_S43]], reason=arg>(read<@type[[TYPE_S43]]>(%[[VALUE_g3s43]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init44:[0-9]+]] @init44(%[[VALUE_p_89:[0-9]+]] p: ptr<@type[[TYPE_S44]]>, %[[VALUE_i_89:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_89:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE265:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_89]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_89]]), const<i32>(44))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE266:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_89]]);
// DEFAULT-NEXT:                 let %[[VALUE267:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE266]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_89]], read<i32>(%[[VALUE267]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(44)>(field0(deref(read<ptr<@type[[TYPE_S44]]>>(%[[VALUE_p_89]])))), read<i32>(%[[VALUE_j_89]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_89]]), read<i32>(%[[VALUE_j_89]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check44:[0-9]+]] @check44(%[[VALUE_p_90:[0-9]+]] p: ptr<@type[[TYPE_S44]]>, %[[VALUE_i_90:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_90:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE268:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_90]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_90]]), const<i32>(44))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE269:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_90]]);
// DEFAULT-NEXT:                 let %[[VALUE270:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE269]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_90]], read<i32>(%[[VALUE270]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(44)>(field0(deref(read<ptr<@type[[TYPE_S44]]>>(%[[VALUE_p_90]])))), read<i32>(%[[VALUE_j_90]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_90]]), read<i32>(%[[VALUE_j_90]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test44:[0-9]+]] @test44(%[[VALUE_s1_89:[0-9]+]] s1: @type[[TYPE_S44]], %[[VALUE_s2_89:[0-9]+]] s2: @type[[TYPE_S44]], %[[VALUE_s3_45:[0-9]+]] s3: @type[[TYPE_S44]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S44]]>, i32) -> void>(%[[VALUE_check44]], addr_of<ptr<@type[[TYPE_S44]]>>(%[[VALUE_s1_89]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S44]]>, i32) -> void>(%[[VALUE_check44]], addr_of<ptr<@type[[TYPE_S44]]>>(%[[VALUE_s2_89]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S44]]>, i32) -> void>(%[[VALUE_check44]], addr_of<ptr<@type[[TYPE_S44]]>>(%[[VALUE_s3_45]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_44:[0-9]+]] @test2_44(%[[VALUE_s1_90:[0-9]+]] s1: @type[[TYPE_S44]], %[[VALUE_s2_90:[0-9]+]] s2: @type[[TYPE_S44]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S44]], @type[[TYPE_S44]], @type[[TYPE_S44]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test44]], copy<@type[[TYPE_S44]], reason=arg>(read<@type[[TYPE_S44]]>(%[[VALUE_s1_90]])), copy<@type[[TYPE_S44]], reason=arg>(read<@type[[TYPE_S44]]>(%[[VALUE_g2s44]])), copy<@type[[TYPE_S44]], reason=arg>(read<@type[[TYPE_S44]]>(%[[VALUE_s2_90]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit44:[0-9]+]] @testit44() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S44]]>, i32) -> void>(%[[VALUE_init44]], addr_of<ptr<@type[[TYPE_S44]]>>(%[[VALUE_g1s44]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S44]]>, i32) -> void>(%[[VALUE_check44]], addr_of<ptr<@type[[TYPE_S44]]>>(%[[VALUE_g1s44]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S44]]>, i32) -> void>(%[[VALUE_init44]], addr_of<ptr<@type[[TYPE_S44]]>>(%[[VALUE_g2s44]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S44]]>, i32) -> void>(%[[VALUE_check44]], addr_of<ptr<@type[[TYPE_S44]]>>(%[[VALUE_g2s44]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S44]]>, i32) -> void>(%[[VALUE_init44]], addr_of<ptr<@type[[TYPE_S44]]>>(%[[VALUE_g3s44]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S44]]>, i32) -> void>(%[[VALUE_check44]], addr_of<ptr<@type[[TYPE_S44]]>>(%[[VALUE_g3s44]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S44]], @type[[TYPE_S44]], @type[[TYPE_S44]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test44]], copy<@type[[TYPE_S44]], reason=arg>(read<@type[[TYPE_S44]]>(%[[VALUE_g1s44]])), copy<@type[[TYPE_S44]], reason=arg>(read<@type[[TYPE_S44]]>(%[[VALUE_g2s44]])), copy<@type[[TYPE_S44]], reason=arg>(read<@type[[TYPE_S44]]>(%[[VALUE_g3s44]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S44]], @type[[TYPE_S44]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_44]], copy<@type[[TYPE_S44]], reason=arg>(read<@type[[TYPE_S44]]>(%[[VALUE_g1s44]])), copy<@type[[TYPE_S44]], reason=arg>(read<@type[[TYPE_S44]]>(%[[VALUE_g3s44]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init45:[0-9]+]] @init45(%[[VALUE_p_91:[0-9]+]] p: ptr<@type[[TYPE_S45]]>, %[[VALUE_i_91:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_91:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE271:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_91]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_91]]), const<i32>(45))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE272:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_91]]);
// DEFAULT-NEXT:                 let %[[VALUE273:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE272]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_91]], read<i32>(%[[VALUE273]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(45)>(field0(deref(read<ptr<@type[[TYPE_S45]]>>(%[[VALUE_p_91]])))), read<i32>(%[[VALUE_j_91]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_91]]), read<i32>(%[[VALUE_j_91]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check45:[0-9]+]] @check45(%[[VALUE_p_92:[0-9]+]] p: ptr<@type[[TYPE_S45]]>, %[[VALUE_i_92:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_92:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE274:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_92]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_92]]), const<i32>(45))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE275:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_92]]);
// DEFAULT-NEXT:                 let %[[VALUE276:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE275]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_92]], read<i32>(%[[VALUE276]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(45)>(field0(deref(read<ptr<@type[[TYPE_S45]]>>(%[[VALUE_p_92]])))), read<i32>(%[[VALUE_j_92]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_92]]), read<i32>(%[[VALUE_j_92]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test45:[0-9]+]] @test45(%[[VALUE_s1_91:[0-9]+]] s1: @type[[TYPE_S45]], %[[VALUE_s2_91:[0-9]+]] s2: @type[[TYPE_S45]], %[[VALUE_s3_46:[0-9]+]] s3: @type[[TYPE_S45]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S45]]>, i32) -> void>(%[[VALUE_check45]], addr_of<ptr<@type[[TYPE_S45]]>>(%[[VALUE_s1_91]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S45]]>, i32) -> void>(%[[VALUE_check45]], addr_of<ptr<@type[[TYPE_S45]]>>(%[[VALUE_s2_91]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S45]]>, i32) -> void>(%[[VALUE_check45]], addr_of<ptr<@type[[TYPE_S45]]>>(%[[VALUE_s3_46]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_45:[0-9]+]] @test2_45(%[[VALUE_s1_92:[0-9]+]] s1: @type[[TYPE_S45]], %[[VALUE_s2_92:[0-9]+]] s2: @type[[TYPE_S45]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S45]], @type[[TYPE_S45]], @type[[TYPE_S45]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test45]], copy<@type[[TYPE_S45]], reason=arg>(read<@type[[TYPE_S45]]>(%[[VALUE_s1_92]])), copy<@type[[TYPE_S45]], reason=arg>(read<@type[[TYPE_S45]]>(%[[VALUE_g2s45]])), copy<@type[[TYPE_S45]], reason=arg>(read<@type[[TYPE_S45]]>(%[[VALUE_s2_92]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit45:[0-9]+]] @testit45() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S45]]>, i32) -> void>(%[[VALUE_init45]], addr_of<ptr<@type[[TYPE_S45]]>>(%[[VALUE_g1s45]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S45]]>, i32) -> void>(%[[VALUE_check45]], addr_of<ptr<@type[[TYPE_S45]]>>(%[[VALUE_g1s45]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S45]]>, i32) -> void>(%[[VALUE_init45]], addr_of<ptr<@type[[TYPE_S45]]>>(%[[VALUE_g2s45]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S45]]>, i32) -> void>(%[[VALUE_check45]], addr_of<ptr<@type[[TYPE_S45]]>>(%[[VALUE_g2s45]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S45]]>, i32) -> void>(%[[VALUE_init45]], addr_of<ptr<@type[[TYPE_S45]]>>(%[[VALUE_g3s45]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S45]]>, i32) -> void>(%[[VALUE_check45]], addr_of<ptr<@type[[TYPE_S45]]>>(%[[VALUE_g3s45]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S45]], @type[[TYPE_S45]], @type[[TYPE_S45]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test45]], copy<@type[[TYPE_S45]], reason=arg>(read<@type[[TYPE_S45]]>(%[[VALUE_g1s45]])), copy<@type[[TYPE_S45]], reason=arg>(read<@type[[TYPE_S45]]>(%[[VALUE_g2s45]])), copy<@type[[TYPE_S45]], reason=arg>(read<@type[[TYPE_S45]]>(%[[VALUE_g3s45]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S45]], @type[[TYPE_S45]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_45]], copy<@type[[TYPE_S45]], reason=arg>(read<@type[[TYPE_S45]]>(%[[VALUE_g1s45]])), copy<@type[[TYPE_S45]], reason=arg>(read<@type[[TYPE_S45]]>(%[[VALUE_g3s45]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init46:[0-9]+]] @init46(%[[VALUE_p_93:[0-9]+]] p: ptr<@type[[TYPE_S46]]>, %[[VALUE_i_93:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_93:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE277:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_93]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_93]]), const<i32>(46))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE278:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_93]]);
// DEFAULT-NEXT:                 let %[[VALUE279:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE278]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_93]], read<i32>(%[[VALUE279]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(46)>(field0(deref(read<ptr<@type[[TYPE_S46]]>>(%[[VALUE_p_93]])))), read<i32>(%[[VALUE_j_93]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_93]]), read<i32>(%[[VALUE_j_93]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check46:[0-9]+]] @check46(%[[VALUE_p_94:[0-9]+]] p: ptr<@type[[TYPE_S46]]>, %[[VALUE_i_94:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_94:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE280:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_94]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_94]]), const<i32>(46))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE281:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_94]]);
// DEFAULT-NEXT:                 let %[[VALUE282:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE281]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_94]], read<i32>(%[[VALUE282]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(46)>(field0(deref(read<ptr<@type[[TYPE_S46]]>>(%[[VALUE_p_94]])))), read<i32>(%[[VALUE_j_94]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_94]]), read<i32>(%[[VALUE_j_94]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test46:[0-9]+]] @test46(%[[VALUE_s1_93:[0-9]+]] s1: @type[[TYPE_S46]], %[[VALUE_s2_93:[0-9]+]] s2: @type[[TYPE_S46]], %[[VALUE_s3_47:[0-9]+]] s3: @type[[TYPE_S46]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S46]]>, i32) -> void>(%[[VALUE_check46]], addr_of<ptr<@type[[TYPE_S46]]>>(%[[VALUE_s1_93]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S46]]>, i32) -> void>(%[[VALUE_check46]], addr_of<ptr<@type[[TYPE_S46]]>>(%[[VALUE_s2_93]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S46]]>, i32) -> void>(%[[VALUE_check46]], addr_of<ptr<@type[[TYPE_S46]]>>(%[[VALUE_s3_47]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_46:[0-9]+]] @test2_46(%[[VALUE_s1_94:[0-9]+]] s1: @type[[TYPE_S46]], %[[VALUE_s2_94:[0-9]+]] s2: @type[[TYPE_S46]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S46]], @type[[TYPE_S46]], @type[[TYPE_S46]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test46]], copy<@type[[TYPE_S46]], reason=arg>(read<@type[[TYPE_S46]]>(%[[VALUE_s1_94]])), copy<@type[[TYPE_S46]], reason=arg>(read<@type[[TYPE_S46]]>(%[[VALUE_g2s46]])), copy<@type[[TYPE_S46]], reason=arg>(read<@type[[TYPE_S46]]>(%[[VALUE_s2_94]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit46:[0-9]+]] @testit46() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S46]]>, i32) -> void>(%[[VALUE_init46]], addr_of<ptr<@type[[TYPE_S46]]>>(%[[VALUE_g1s46]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S46]]>, i32) -> void>(%[[VALUE_check46]], addr_of<ptr<@type[[TYPE_S46]]>>(%[[VALUE_g1s46]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S46]]>, i32) -> void>(%[[VALUE_init46]], addr_of<ptr<@type[[TYPE_S46]]>>(%[[VALUE_g2s46]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S46]]>, i32) -> void>(%[[VALUE_check46]], addr_of<ptr<@type[[TYPE_S46]]>>(%[[VALUE_g2s46]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S46]]>, i32) -> void>(%[[VALUE_init46]], addr_of<ptr<@type[[TYPE_S46]]>>(%[[VALUE_g3s46]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S46]]>, i32) -> void>(%[[VALUE_check46]], addr_of<ptr<@type[[TYPE_S46]]>>(%[[VALUE_g3s46]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S46]], @type[[TYPE_S46]], @type[[TYPE_S46]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test46]], copy<@type[[TYPE_S46]], reason=arg>(read<@type[[TYPE_S46]]>(%[[VALUE_g1s46]])), copy<@type[[TYPE_S46]], reason=arg>(read<@type[[TYPE_S46]]>(%[[VALUE_g2s46]])), copy<@type[[TYPE_S46]], reason=arg>(read<@type[[TYPE_S46]]>(%[[VALUE_g3s46]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S46]], @type[[TYPE_S46]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_46]], copy<@type[[TYPE_S46]], reason=arg>(read<@type[[TYPE_S46]]>(%[[VALUE_g1s46]])), copy<@type[[TYPE_S46]], reason=arg>(read<@type[[TYPE_S46]]>(%[[VALUE_g3s46]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init47:[0-9]+]] @init47(%[[VALUE_p_95:[0-9]+]] p: ptr<@type[[TYPE_S47]]>, %[[VALUE_i_95:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_95:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE283:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_95]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_95]]), const<i32>(47))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE284:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_95]]);
// DEFAULT-NEXT:                 let %[[VALUE285:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE284]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_95]], read<i32>(%[[VALUE285]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(47)>(field0(deref(read<ptr<@type[[TYPE_S47]]>>(%[[VALUE_p_95]])))), read<i32>(%[[VALUE_j_95]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_95]]), read<i32>(%[[VALUE_j_95]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check47:[0-9]+]] @check47(%[[VALUE_p_96:[0-9]+]] p: ptr<@type[[TYPE_S47]]>, %[[VALUE_i_96:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_96:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE286:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_96]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_96]]), const<i32>(47))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE287:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_96]]);
// DEFAULT-NEXT:                 let %[[VALUE288:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE287]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_96]], read<i32>(%[[VALUE288]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(47)>(field0(deref(read<ptr<@type[[TYPE_S47]]>>(%[[VALUE_p_96]])))), read<i32>(%[[VALUE_j_96]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_96]]), read<i32>(%[[VALUE_j_96]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test47:[0-9]+]] @test47(%[[VALUE_s1_95:[0-9]+]] s1: @type[[TYPE_S47]], %[[VALUE_s2_95:[0-9]+]] s2: @type[[TYPE_S47]], %[[VALUE_s3_48:[0-9]+]] s3: @type[[TYPE_S47]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S47]]>, i32) -> void>(%[[VALUE_check47]], addr_of<ptr<@type[[TYPE_S47]]>>(%[[VALUE_s1_95]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S47]]>, i32) -> void>(%[[VALUE_check47]], addr_of<ptr<@type[[TYPE_S47]]>>(%[[VALUE_s2_95]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S47]]>, i32) -> void>(%[[VALUE_check47]], addr_of<ptr<@type[[TYPE_S47]]>>(%[[VALUE_s3_48]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_47:[0-9]+]] @test2_47(%[[VALUE_s1_96:[0-9]+]] s1: @type[[TYPE_S47]], %[[VALUE_s2_96:[0-9]+]] s2: @type[[TYPE_S47]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S47]], @type[[TYPE_S47]], @type[[TYPE_S47]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test47]], copy<@type[[TYPE_S47]], reason=arg>(read<@type[[TYPE_S47]]>(%[[VALUE_s1_96]])), copy<@type[[TYPE_S47]], reason=arg>(read<@type[[TYPE_S47]]>(%[[VALUE_g2s47]])), copy<@type[[TYPE_S47]], reason=arg>(read<@type[[TYPE_S47]]>(%[[VALUE_s2_96]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit47:[0-9]+]] @testit47() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S47]]>, i32) -> void>(%[[VALUE_init47]], addr_of<ptr<@type[[TYPE_S47]]>>(%[[VALUE_g1s47]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S47]]>, i32) -> void>(%[[VALUE_check47]], addr_of<ptr<@type[[TYPE_S47]]>>(%[[VALUE_g1s47]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S47]]>, i32) -> void>(%[[VALUE_init47]], addr_of<ptr<@type[[TYPE_S47]]>>(%[[VALUE_g2s47]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S47]]>, i32) -> void>(%[[VALUE_check47]], addr_of<ptr<@type[[TYPE_S47]]>>(%[[VALUE_g2s47]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S47]]>, i32) -> void>(%[[VALUE_init47]], addr_of<ptr<@type[[TYPE_S47]]>>(%[[VALUE_g3s47]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S47]]>, i32) -> void>(%[[VALUE_check47]], addr_of<ptr<@type[[TYPE_S47]]>>(%[[VALUE_g3s47]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S47]], @type[[TYPE_S47]], @type[[TYPE_S47]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test47]], copy<@type[[TYPE_S47]], reason=arg>(read<@type[[TYPE_S47]]>(%[[VALUE_g1s47]])), copy<@type[[TYPE_S47]], reason=arg>(read<@type[[TYPE_S47]]>(%[[VALUE_g2s47]])), copy<@type[[TYPE_S47]], reason=arg>(read<@type[[TYPE_S47]]>(%[[VALUE_g3s47]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S47]], @type[[TYPE_S47]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_47]], copy<@type[[TYPE_S47]], reason=arg>(read<@type[[TYPE_S47]]>(%[[VALUE_g1s47]])), copy<@type[[TYPE_S47]], reason=arg>(read<@type[[TYPE_S47]]>(%[[VALUE_g3s47]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init48:[0-9]+]] @init48(%[[VALUE_p_97:[0-9]+]] p: ptr<@type[[TYPE_S48]]>, %[[VALUE_i_97:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_97:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE289:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_97]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_97]]), const<i32>(48))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE290:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_97]]);
// DEFAULT-NEXT:                 let %[[VALUE291:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE290]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_97]], read<i32>(%[[VALUE291]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(48)>(field0(deref(read<ptr<@type[[TYPE_S48]]>>(%[[VALUE_p_97]])))), read<i32>(%[[VALUE_j_97]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_97]]), read<i32>(%[[VALUE_j_97]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check48:[0-9]+]] @check48(%[[VALUE_p_98:[0-9]+]] p: ptr<@type[[TYPE_S48]]>, %[[VALUE_i_98:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_98:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE292:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_98]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_98]]), const<i32>(48))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE293:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_98]]);
// DEFAULT-NEXT:                 let %[[VALUE294:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE293]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_98]], read<i32>(%[[VALUE294]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(48)>(field0(deref(read<ptr<@type[[TYPE_S48]]>>(%[[VALUE_p_98]])))), read<i32>(%[[VALUE_j_98]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_98]]), read<i32>(%[[VALUE_j_98]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test48:[0-9]+]] @test48(%[[VALUE_s1_97:[0-9]+]] s1: @type[[TYPE_S48]], %[[VALUE_s2_97:[0-9]+]] s2: @type[[TYPE_S48]], %[[VALUE_s3_49:[0-9]+]] s3: @type[[TYPE_S48]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S48]]>, i32) -> void>(%[[VALUE_check48]], addr_of<ptr<@type[[TYPE_S48]]>>(%[[VALUE_s1_97]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S48]]>, i32) -> void>(%[[VALUE_check48]], addr_of<ptr<@type[[TYPE_S48]]>>(%[[VALUE_s2_97]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S48]]>, i32) -> void>(%[[VALUE_check48]], addr_of<ptr<@type[[TYPE_S48]]>>(%[[VALUE_s3_49]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_48:[0-9]+]] @test2_48(%[[VALUE_s1_98:[0-9]+]] s1: @type[[TYPE_S48]], %[[VALUE_s2_98:[0-9]+]] s2: @type[[TYPE_S48]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S48]], @type[[TYPE_S48]], @type[[TYPE_S48]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test48]], copy<@type[[TYPE_S48]], reason=arg>(read<@type[[TYPE_S48]]>(%[[VALUE_s1_98]])), copy<@type[[TYPE_S48]], reason=arg>(read<@type[[TYPE_S48]]>(%[[VALUE_g2s48]])), copy<@type[[TYPE_S48]], reason=arg>(read<@type[[TYPE_S48]]>(%[[VALUE_s2_98]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit48:[0-9]+]] @testit48() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S48]]>, i32) -> void>(%[[VALUE_init48]], addr_of<ptr<@type[[TYPE_S48]]>>(%[[VALUE_g1s48]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S48]]>, i32) -> void>(%[[VALUE_check48]], addr_of<ptr<@type[[TYPE_S48]]>>(%[[VALUE_g1s48]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S48]]>, i32) -> void>(%[[VALUE_init48]], addr_of<ptr<@type[[TYPE_S48]]>>(%[[VALUE_g2s48]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S48]]>, i32) -> void>(%[[VALUE_check48]], addr_of<ptr<@type[[TYPE_S48]]>>(%[[VALUE_g2s48]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S48]]>, i32) -> void>(%[[VALUE_init48]], addr_of<ptr<@type[[TYPE_S48]]>>(%[[VALUE_g3s48]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S48]]>, i32) -> void>(%[[VALUE_check48]], addr_of<ptr<@type[[TYPE_S48]]>>(%[[VALUE_g3s48]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S48]], @type[[TYPE_S48]], @type[[TYPE_S48]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test48]], copy<@type[[TYPE_S48]], reason=arg>(read<@type[[TYPE_S48]]>(%[[VALUE_g1s48]])), copy<@type[[TYPE_S48]], reason=arg>(read<@type[[TYPE_S48]]>(%[[VALUE_g2s48]])), copy<@type[[TYPE_S48]], reason=arg>(read<@type[[TYPE_S48]]>(%[[VALUE_g3s48]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S48]], @type[[TYPE_S48]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_48]], copy<@type[[TYPE_S48]], reason=arg>(read<@type[[TYPE_S48]]>(%[[VALUE_g1s48]])), copy<@type[[TYPE_S48]], reason=arg>(read<@type[[TYPE_S48]]>(%[[VALUE_g3s48]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init49:[0-9]+]] @init49(%[[VALUE_p_99:[0-9]+]] p: ptr<@type[[TYPE_S49]]>, %[[VALUE_i_99:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_99:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE295:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_99]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_99]]), const<i32>(49))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE296:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_99]]);
// DEFAULT-NEXT:                 let %[[VALUE297:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE296]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_99]], read<i32>(%[[VALUE297]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(49)>(field0(deref(read<ptr<@type[[TYPE_S49]]>>(%[[VALUE_p_99]])))), read<i32>(%[[VALUE_j_99]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_99]]), read<i32>(%[[VALUE_j_99]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check49:[0-9]+]] @check49(%[[VALUE_p_100:[0-9]+]] p: ptr<@type[[TYPE_S49]]>, %[[VALUE_i_100:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_100:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE298:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_100]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_100]]), const<i32>(49))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE299:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_100]]);
// DEFAULT-NEXT:                 let %[[VALUE300:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE299]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_100]], read<i32>(%[[VALUE300]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(49)>(field0(deref(read<ptr<@type[[TYPE_S49]]>>(%[[VALUE_p_100]])))), read<i32>(%[[VALUE_j_100]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_100]]), read<i32>(%[[VALUE_j_100]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test49:[0-9]+]] @test49(%[[VALUE_s1_99:[0-9]+]] s1: @type[[TYPE_S49]], %[[VALUE_s2_99:[0-9]+]] s2: @type[[TYPE_S49]], %[[VALUE_s3_50:[0-9]+]] s3: @type[[TYPE_S49]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S49]]>, i32) -> void>(%[[VALUE_check49]], addr_of<ptr<@type[[TYPE_S49]]>>(%[[VALUE_s1_99]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S49]]>, i32) -> void>(%[[VALUE_check49]], addr_of<ptr<@type[[TYPE_S49]]>>(%[[VALUE_s2_99]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S49]]>, i32) -> void>(%[[VALUE_check49]], addr_of<ptr<@type[[TYPE_S49]]>>(%[[VALUE_s3_50]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_49:[0-9]+]] @test2_49(%[[VALUE_s1_100:[0-9]+]] s1: @type[[TYPE_S49]], %[[VALUE_s2_100:[0-9]+]] s2: @type[[TYPE_S49]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S49]], @type[[TYPE_S49]], @type[[TYPE_S49]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test49]], copy<@type[[TYPE_S49]], reason=arg>(read<@type[[TYPE_S49]]>(%[[VALUE_s1_100]])), copy<@type[[TYPE_S49]], reason=arg>(read<@type[[TYPE_S49]]>(%[[VALUE_g2s49]])), copy<@type[[TYPE_S49]], reason=arg>(read<@type[[TYPE_S49]]>(%[[VALUE_s2_100]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit49:[0-9]+]] @testit49() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S49]]>, i32) -> void>(%[[VALUE_init49]], addr_of<ptr<@type[[TYPE_S49]]>>(%[[VALUE_g1s49]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S49]]>, i32) -> void>(%[[VALUE_check49]], addr_of<ptr<@type[[TYPE_S49]]>>(%[[VALUE_g1s49]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S49]]>, i32) -> void>(%[[VALUE_init49]], addr_of<ptr<@type[[TYPE_S49]]>>(%[[VALUE_g2s49]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S49]]>, i32) -> void>(%[[VALUE_check49]], addr_of<ptr<@type[[TYPE_S49]]>>(%[[VALUE_g2s49]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S49]]>, i32) -> void>(%[[VALUE_init49]], addr_of<ptr<@type[[TYPE_S49]]>>(%[[VALUE_g3s49]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S49]]>, i32) -> void>(%[[VALUE_check49]], addr_of<ptr<@type[[TYPE_S49]]>>(%[[VALUE_g3s49]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S49]], @type[[TYPE_S49]], @type[[TYPE_S49]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test49]], copy<@type[[TYPE_S49]], reason=arg>(read<@type[[TYPE_S49]]>(%[[VALUE_g1s49]])), copy<@type[[TYPE_S49]], reason=arg>(read<@type[[TYPE_S49]]>(%[[VALUE_g2s49]])), copy<@type[[TYPE_S49]], reason=arg>(read<@type[[TYPE_S49]]>(%[[VALUE_g3s49]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S49]], @type[[TYPE_S49]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_49]], copy<@type[[TYPE_S49]], reason=arg>(read<@type[[TYPE_S49]]>(%[[VALUE_g1s49]])), copy<@type[[TYPE_S49]], reason=arg>(read<@type[[TYPE_S49]]>(%[[VALUE_g3s49]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init50:[0-9]+]] @init50(%[[VALUE_p_101:[0-9]+]] p: ptr<@type[[TYPE_S50]]>, %[[VALUE_i_101:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_101:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE301:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_101]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_101]]), const<i32>(50))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE302:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_101]]);
// DEFAULT-NEXT:                 let %[[VALUE303:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE302]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_101]], read<i32>(%[[VALUE303]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(50)>(field0(deref(read<ptr<@type[[TYPE_S50]]>>(%[[VALUE_p_101]])))), read<i32>(%[[VALUE_j_101]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_101]]), read<i32>(%[[VALUE_j_101]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check50:[0-9]+]] @check50(%[[VALUE_p_102:[0-9]+]] p: ptr<@type[[TYPE_S50]]>, %[[VALUE_i_102:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_102:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE304:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_102]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_102]]), const<i32>(50))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE305:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_102]]);
// DEFAULT-NEXT:                 let %[[VALUE306:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE305]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_102]], read<i32>(%[[VALUE306]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(50)>(field0(deref(read<ptr<@type[[TYPE_S50]]>>(%[[VALUE_p_102]])))), read<i32>(%[[VALUE_j_102]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_102]]), read<i32>(%[[VALUE_j_102]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test50:[0-9]+]] @test50(%[[VALUE_s1_101:[0-9]+]] s1: @type[[TYPE_S50]], %[[VALUE_s2_101:[0-9]+]] s2: @type[[TYPE_S50]], %[[VALUE_s3_51:[0-9]+]] s3: @type[[TYPE_S50]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S50]]>, i32) -> void>(%[[VALUE_check50]], addr_of<ptr<@type[[TYPE_S50]]>>(%[[VALUE_s1_101]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S50]]>, i32) -> void>(%[[VALUE_check50]], addr_of<ptr<@type[[TYPE_S50]]>>(%[[VALUE_s2_101]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S50]]>, i32) -> void>(%[[VALUE_check50]], addr_of<ptr<@type[[TYPE_S50]]>>(%[[VALUE_s3_51]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_50:[0-9]+]] @test2_50(%[[VALUE_s1_102:[0-9]+]] s1: @type[[TYPE_S50]], %[[VALUE_s2_102:[0-9]+]] s2: @type[[TYPE_S50]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S50]], @type[[TYPE_S50]], @type[[TYPE_S50]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test50]], copy<@type[[TYPE_S50]], reason=arg>(read<@type[[TYPE_S50]]>(%[[VALUE_s1_102]])), copy<@type[[TYPE_S50]], reason=arg>(read<@type[[TYPE_S50]]>(%[[VALUE_g2s50]])), copy<@type[[TYPE_S50]], reason=arg>(read<@type[[TYPE_S50]]>(%[[VALUE_s2_102]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit50:[0-9]+]] @testit50() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S50]]>, i32) -> void>(%[[VALUE_init50]], addr_of<ptr<@type[[TYPE_S50]]>>(%[[VALUE_g1s50]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S50]]>, i32) -> void>(%[[VALUE_check50]], addr_of<ptr<@type[[TYPE_S50]]>>(%[[VALUE_g1s50]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S50]]>, i32) -> void>(%[[VALUE_init50]], addr_of<ptr<@type[[TYPE_S50]]>>(%[[VALUE_g2s50]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S50]]>, i32) -> void>(%[[VALUE_check50]], addr_of<ptr<@type[[TYPE_S50]]>>(%[[VALUE_g2s50]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S50]]>, i32) -> void>(%[[VALUE_init50]], addr_of<ptr<@type[[TYPE_S50]]>>(%[[VALUE_g3s50]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S50]]>, i32) -> void>(%[[VALUE_check50]], addr_of<ptr<@type[[TYPE_S50]]>>(%[[VALUE_g3s50]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S50]], @type[[TYPE_S50]], @type[[TYPE_S50]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test50]], copy<@type[[TYPE_S50]], reason=arg>(read<@type[[TYPE_S50]]>(%[[VALUE_g1s50]])), copy<@type[[TYPE_S50]], reason=arg>(read<@type[[TYPE_S50]]>(%[[VALUE_g2s50]])), copy<@type[[TYPE_S50]], reason=arg>(read<@type[[TYPE_S50]]>(%[[VALUE_g3s50]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S50]], @type[[TYPE_S50]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_50]], copy<@type[[TYPE_S50]], reason=arg>(read<@type[[TYPE_S50]]>(%[[VALUE_g1s50]])), copy<@type[[TYPE_S50]], reason=arg>(read<@type[[TYPE_S50]]>(%[[VALUE_g3s50]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init51:[0-9]+]] @init51(%[[VALUE_p_103:[0-9]+]] p: ptr<@type[[TYPE_S51]]>, %[[VALUE_i_103:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_103:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE307:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_103]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_103]]), const<i32>(51))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE308:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_103]]);
// DEFAULT-NEXT:                 let %[[VALUE309:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE308]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_103]], read<i32>(%[[VALUE309]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(51)>(field0(deref(read<ptr<@type[[TYPE_S51]]>>(%[[VALUE_p_103]])))), read<i32>(%[[VALUE_j_103]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_103]]), read<i32>(%[[VALUE_j_103]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check51:[0-9]+]] @check51(%[[VALUE_p_104:[0-9]+]] p: ptr<@type[[TYPE_S51]]>, %[[VALUE_i_104:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_104:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE310:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_104]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_104]]), const<i32>(51))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE311:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_104]]);
// DEFAULT-NEXT:                 let %[[VALUE312:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE311]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_104]], read<i32>(%[[VALUE312]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(51)>(field0(deref(read<ptr<@type[[TYPE_S51]]>>(%[[VALUE_p_104]])))), read<i32>(%[[VALUE_j_104]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_104]]), read<i32>(%[[VALUE_j_104]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test51:[0-9]+]] @test51(%[[VALUE_s1_103:[0-9]+]] s1: @type[[TYPE_S51]], %[[VALUE_s2_103:[0-9]+]] s2: @type[[TYPE_S51]], %[[VALUE_s3_52:[0-9]+]] s3: @type[[TYPE_S51]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S51]]>, i32) -> void>(%[[VALUE_check51]], addr_of<ptr<@type[[TYPE_S51]]>>(%[[VALUE_s1_103]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S51]]>, i32) -> void>(%[[VALUE_check51]], addr_of<ptr<@type[[TYPE_S51]]>>(%[[VALUE_s2_103]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S51]]>, i32) -> void>(%[[VALUE_check51]], addr_of<ptr<@type[[TYPE_S51]]>>(%[[VALUE_s3_52]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_51:[0-9]+]] @test2_51(%[[VALUE_s1_104:[0-9]+]] s1: @type[[TYPE_S51]], %[[VALUE_s2_104:[0-9]+]] s2: @type[[TYPE_S51]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S51]], @type[[TYPE_S51]], @type[[TYPE_S51]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test51]], copy<@type[[TYPE_S51]], reason=arg>(read<@type[[TYPE_S51]]>(%[[VALUE_s1_104]])), copy<@type[[TYPE_S51]], reason=arg>(read<@type[[TYPE_S51]]>(%[[VALUE_g2s51]])), copy<@type[[TYPE_S51]], reason=arg>(read<@type[[TYPE_S51]]>(%[[VALUE_s2_104]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit51:[0-9]+]] @testit51() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S51]]>, i32) -> void>(%[[VALUE_init51]], addr_of<ptr<@type[[TYPE_S51]]>>(%[[VALUE_g1s51]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S51]]>, i32) -> void>(%[[VALUE_check51]], addr_of<ptr<@type[[TYPE_S51]]>>(%[[VALUE_g1s51]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S51]]>, i32) -> void>(%[[VALUE_init51]], addr_of<ptr<@type[[TYPE_S51]]>>(%[[VALUE_g2s51]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S51]]>, i32) -> void>(%[[VALUE_check51]], addr_of<ptr<@type[[TYPE_S51]]>>(%[[VALUE_g2s51]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S51]]>, i32) -> void>(%[[VALUE_init51]], addr_of<ptr<@type[[TYPE_S51]]>>(%[[VALUE_g3s51]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S51]]>, i32) -> void>(%[[VALUE_check51]], addr_of<ptr<@type[[TYPE_S51]]>>(%[[VALUE_g3s51]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S51]], @type[[TYPE_S51]], @type[[TYPE_S51]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test51]], copy<@type[[TYPE_S51]], reason=arg>(read<@type[[TYPE_S51]]>(%[[VALUE_g1s51]])), copy<@type[[TYPE_S51]], reason=arg>(read<@type[[TYPE_S51]]>(%[[VALUE_g2s51]])), copy<@type[[TYPE_S51]], reason=arg>(read<@type[[TYPE_S51]]>(%[[VALUE_g3s51]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S51]], @type[[TYPE_S51]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_51]], copy<@type[[TYPE_S51]], reason=arg>(read<@type[[TYPE_S51]]>(%[[VALUE_g1s51]])), copy<@type[[TYPE_S51]], reason=arg>(read<@type[[TYPE_S51]]>(%[[VALUE_g3s51]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init52:[0-9]+]] @init52(%[[VALUE_p_105:[0-9]+]] p: ptr<@type[[TYPE_S52]]>, %[[VALUE_i_105:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_105:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE313:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_105]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_105]]), const<i32>(52))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE314:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_105]]);
// DEFAULT-NEXT:                 let %[[VALUE315:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE314]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_105]], read<i32>(%[[VALUE315]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(52)>(field0(deref(read<ptr<@type[[TYPE_S52]]>>(%[[VALUE_p_105]])))), read<i32>(%[[VALUE_j_105]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_105]]), read<i32>(%[[VALUE_j_105]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check52:[0-9]+]] @check52(%[[VALUE_p_106:[0-9]+]] p: ptr<@type[[TYPE_S52]]>, %[[VALUE_i_106:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_106:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE316:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_106]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_106]]), const<i32>(52))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE317:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_106]]);
// DEFAULT-NEXT:                 let %[[VALUE318:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE317]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_106]], read<i32>(%[[VALUE318]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(52)>(field0(deref(read<ptr<@type[[TYPE_S52]]>>(%[[VALUE_p_106]])))), read<i32>(%[[VALUE_j_106]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_106]]), read<i32>(%[[VALUE_j_106]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test52:[0-9]+]] @test52(%[[VALUE_s1_105:[0-9]+]] s1: @type[[TYPE_S52]], %[[VALUE_s2_105:[0-9]+]] s2: @type[[TYPE_S52]], %[[VALUE_s3_53:[0-9]+]] s3: @type[[TYPE_S52]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S52]]>, i32) -> void>(%[[VALUE_check52]], addr_of<ptr<@type[[TYPE_S52]]>>(%[[VALUE_s1_105]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S52]]>, i32) -> void>(%[[VALUE_check52]], addr_of<ptr<@type[[TYPE_S52]]>>(%[[VALUE_s2_105]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S52]]>, i32) -> void>(%[[VALUE_check52]], addr_of<ptr<@type[[TYPE_S52]]>>(%[[VALUE_s3_53]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_52:[0-9]+]] @test2_52(%[[VALUE_s1_106:[0-9]+]] s1: @type[[TYPE_S52]], %[[VALUE_s2_106:[0-9]+]] s2: @type[[TYPE_S52]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S52]], @type[[TYPE_S52]], @type[[TYPE_S52]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test52]], copy<@type[[TYPE_S52]], reason=arg>(read<@type[[TYPE_S52]]>(%[[VALUE_s1_106]])), copy<@type[[TYPE_S52]], reason=arg>(read<@type[[TYPE_S52]]>(%[[VALUE_g2s52]])), copy<@type[[TYPE_S52]], reason=arg>(read<@type[[TYPE_S52]]>(%[[VALUE_s2_106]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit52:[0-9]+]] @testit52() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S52]]>, i32) -> void>(%[[VALUE_init52]], addr_of<ptr<@type[[TYPE_S52]]>>(%[[VALUE_g1s52]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S52]]>, i32) -> void>(%[[VALUE_check52]], addr_of<ptr<@type[[TYPE_S52]]>>(%[[VALUE_g1s52]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S52]]>, i32) -> void>(%[[VALUE_init52]], addr_of<ptr<@type[[TYPE_S52]]>>(%[[VALUE_g2s52]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S52]]>, i32) -> void>(%[[VALUE_check52]], addr_of<ptr<@type[[TYPE_S52]]>>(%[[VALUE_g2s52]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S52]]>, i32) -> void>(%[[VALUE_init52]], addr_of<ptr<@type[[TYPE_S52]]>>(%[[VALUE_g3s52]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S52]]>, i32) -> void>(%[[VALUE_check52]], addr_of<ptr<@type[[TYPE_S52]]>>(%[[VALUE_g3s52]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S52]], @type[[TYPE_S52]], @type[[TYPE_S52]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test52]], copy<@type[[TYPE_S52]], reason=arg>(read<@type[[TYPE_S52]]>(%[[VALUE_g1s52]])), copy<@type[[TYPE_S52]], reason=arg>(read<@type[[TYPE_S52]]>(%[[VALUE_g2s52]])), copy<@type[[TYPE_S52]], reason=arg>(read<@type[[TYPE_S52]]>(%[[VALUE_g3s52]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S52]], @type[[TYPE_S52]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_52]], copy<@type[[TYPE_S52]], reason=arg>(read<@type[[TYPE_S52]]>(%[[VALUE_g1s52]])), copy<@type[[TYPE_S52]], reason=arg>(read<@type[[TYPE_S52]]>(%[[VALUE_g3s52]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init53:[0-9]+]] @init53(%[[VALUE_p_107:[0-9]+]] p: ptr<@type[[TYPE_S53]]>, %[[VALUE_i_107:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_107:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE319:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_107]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_107]]), const<i32>(53))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE320:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_107]]);
// DEFAULT-NEXT:                 let %[[VALUE321:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE320]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_107]], read<i32>(%[[VALUE321]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(53)>(field0(deref(read<ptr<@type[[TYPE_S53]]>>(%[[VALUE_p_107]])))), read<i32>(%[[VALUE_j_107]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_107]]), read<i32>(%[[VALUE_j_107]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check53:[0-9]+]] @check53(%[[VALUE_p_108:[0-9]+]] p: ptr<@type[[TYPE_S53]]>, %[[VALUE_i_108:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_108:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE322:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_108]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_108]]), const<i32>(53))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE323:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_108]]);
// DEFAULT-NEXT:                 let %[[VALUE324:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE323]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_108]], read<i32>(%[[VALUE324]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(53)>(field0(deref(read<ptr<@type[[TYPE_S53]]>>(%[[VALUE_p_108]])))), read<i32>(%[[VALUE_j_108]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_108]]), read<i32>(%[[VALUE_j_108]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test53:[0-9]+]] @test53(%[[VALUE_s1_107:[0-9]+]] s1: @type[[TYPE_S53]], %[[VALUE_s2_107:[0-9]+]] s2: @type[[TYPE_S53]], %[[VALUE_s3_54:[0-9]+]] s3: @type[[TYPE_S53]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S53]]>, i32) -> void>(%[[VALUE_check53]], addr_of<ptr<@type[[TYPE_S53]]>>(%[[VALUE_s1_107]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S53]]>, i32) -> void>(%[[VALUE_check53]], addr_of<ptr<@type[[TYPE_S53]]>>(%[[VALUE_s2_107]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S53]]>, i32) -> void>(%[[VALUE_check53]], addr_of<ptr<@type[[TYPE_S53]]>>(%[[VALUE_s3_54]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_53:[0-9]+]] @test2_53(%[[VALUE_s1_108:[0-9]+]] s1: @type[[TYPE_S53]], %[[VALUE_s2_108:[0-9]+]] s2: @type[[TYPE_S53]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S53]], @type[[TYPE_S53]], @type[[TYPE_S53]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test53]], copy<@type[[TYPE_S53]], reason=arg>(read<@type[[TYPE_S53]]>(%[[VALUE_s1_108]])), copy<@type[[TYPE_S53]], reason=arg>(read<@type[[TYPE_S53]]>(%[[VALUE_g2s53]])), copy<@type[[TYPE_S53]], reason=arg>(read<@type[[TYPE_S53]]>(%[[VALUE_s2_108]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit53:[0-9]+]] @testit53() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S53]]>, i32) -> void>(%[[VALUE_init53]], addr_of<ptr<@type[[TYPE_S53]]>>(%[[VALUE_g1s53]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S53]]>, i32) -> void>(%[[VALUE_check53]], addr_of<ptr<@type[[TYPE_S53]]>>(%[[VALUE_g1s53]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S53]]>, i32) -> void>(%[[VALUE_init53]], addr_of<ptr<@type[[TYPE_S53]]>>(%[[VALUE_g2s53]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S53]]>, i32) -> void>(%[[VALUE_check53]], addr_of<ptr<@type[[TYPE_S53]]>>(%[[VALUE_g2s53]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S53]]>, i32) -> void>(%[[VALUE_init53]], addr_of<ptr<@type[[TYPE_S53]]>>(%[[VALUE_g3s53]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S53]]>, i32) -> void>(%[[VALUE_check53]], addr_of<ptr<@type[[TYPE_S53]]>>(%[[VALUE_g3s53]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S53]], @type[[TYPE_S53]], @type[[TYPE_S53]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test53]], copy<@type[[TYPE_S53]], reason=arg>(read<@type[[TYPE_S53]]>(%[[VALUE_g1s53]])), copy<@type[[TYPE_S53]], reason=arg>(read<@type[[TYPE_S53]]>(%[[VALUE_g2s53]])), copy<@type[[TYPE_S53]], reason=arg>(read<@type[[TYPE_S53]]>(%[[VALUE_g3s53]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S53]], @type[[TYPE_S53]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_53]], copy<@type[[TYPE_S53]], reason=arg>(read<@type[[TYPE_S53]]>(%[[VALUE_g1s53]])), copy<@type[[TYPE_S53]], reason=arg>(read<@type[[TYPE_S53]]>(%[[VALUE_g3s53]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init54:[0-9]+]] @init54(%[[VALUE_p_109:[0-9]+]] p: ptr<@type[[TYPE_S54]]>, %[[VALUE_i_109:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_109:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE325:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_109]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_109]]), const<i32>(54))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE326:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_109]]);
// DEFAULT-NEXT:                 let %[[VALUE327:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE326]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_109]], read<i32>(%[[VALUE327]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(54)>(field0(deref(read<ptr<@type[[TYPE_S54]]>>(%[[VALUE_p_109]])))), read<i32>(%[[VALUE_j_109]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_109]]), read<i32>(%[[VALUE_j_109]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check54:[0-9]+]] @check54(%[[VALUE_p_110:[0-9]+]] p: ptr<@type[[TYPE_S54]]>, %[[VALUE_i_110:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_110:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE328:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_110]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_110]]), const<i32>(54))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE329:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_110]]);
// DEFAULT-NEXT:                 let %[[VALUE330:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE329]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_110]], read<i32>(%[[VALUE330]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(54)>(field0(deref(read<ptr<@type[[TYPE_S54]]>>(%[[VALUE_p_110]])))), read<i32>(%[[VALUE_j_110]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_110]]), read<i32>(%[[VALUE_j_110]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test54:[0-9]+]] @test54(%[[VALUE_s1_109:[0-9]+]] s1: @type[[TYPE_S54]], %[[VALUE_s2_109:[0-9]+]] s2: @type[[TYPE_S54]], %[[VALUE_s3_55:[0-9]+]] s3: @type[[TYPE_S54]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S54]]>, i32) -> void>(%[[VALUE_check54]], addr_of<ptr<@type[[TYPE_S54]]>>(%[[VALUE_s1_109]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S54]]>, i32) -> void>(%[[VALUE_check54]], addr_of<ptr<@type[[TYPE_S54]]>>(%[[VALUE_s2_109]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S54]]>, i32) -> void>(%[[VALUE_check54]], addr_of<ptr<@type[[TYPE_S54]]>>(%[[VALUE_s3_55]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_54:[0-9]+]] @test2_54(%[[VALUE_s1_110:[0-9]+]] s1: @type[[TYPE_S54]], %[[VALUE_s2_110:[0-9]+]] s2: @type[[TYPE_S54]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S54]], @type[[TYPE_S54]], @type[[TYPE_S54]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test54]], copy<@type[[TYPE_S54]], reason=arg>(read<@type[[TYPE_S54]]>(%[[VALUE_s1_110]])), copy<@type[[TYPE_S54]], reason=arg>(read<@type[[TYPE_S54]]>(%[[VALUE_g2s54]])), copy<@type[[TYPE_S54]], reason=arg>(read<@type[[TYPE_S54]]>(%[[VALUE_s2_110]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit54:[0-9]+]] @testit54() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S54]]>, i32) -> void>(%[[VALUE_init54]], addr_of<ptr<@type[[TYPE_S54]]>>(%[[VALUE_g1s54]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S54]]>, i32) -> void>(%[[VALUE_check54]], addr_of<ptr<@type[[TYPE_S54]]>>(%[[VALUE_g1s54]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S54]]>, i32) -> void>(%[[VALUE_init54]], addr_of<ptr<@type[[TYPE_S54]]>>(%[[VALUE_g2s54]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S54]]>, i32) -> void>(%[[VALUE_check54]], addr_of<ptr<@type[[TYPE_S54]]>>(%[[VALUE_g2s54]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S54]]>, i32) -> void>(%[[VALUE_init54]], addr_of<ptr<@type[[TYPE_S54]]>>(%[[VALUE_g3s54]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S54]]>, i32) -> void>(%[[VALUE_check54]], addr_of<ptr<@type[[TYPE_S54]]>>(%[[VALUE_g3s54]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S54]], @type[[TYPE_S54]], @type[[TYPE_S54]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test54]], copy<@type[[TYPE_S54]], reason=arg>(read<@type[[TYPE_S54]]>(%[[VALUE_g1s54]])), copy<@type[[TYPE_S54]], reason=arg>(read<@type[[TYPE_S54]]>(%[[VALUE_g2s54]])), copy<@type[[TYPE_S54]], reason=arg>(read<@type[[TYPE_S54]]>(%[[VALUE_g3s54]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S54]], @type[[TYPE_S54]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_54]], copy<@type[[TYPE_S54]], reason=arg>(read<@type[[TYPE_S54]]>(%[[VALUE_g1s54]])), copy<@type[[TYPE_S54]], reason=arg>(read<@type[[TYPE_S54]]>(%[[VALUE_g3s54]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init55:[0-9]+]] @init55(%[[VALUE_p_111:[0-9]+]] p: ptr<@type[[TYPE_S55]]>, %[[VALUE_i_111:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_111:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE331:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_111]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_111]]), const<i32>(55))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE332:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_111]]);
// DEFAULT-NEXT:                 let %[[VALUE333:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE332]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_111]], read<i32>(%[[VALUE333]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(55)>(field0(deref(read<ptr<@type[[TYPE_S55]]>>(%[[VALUE_p_111]])))), read<i32>(%[[VALUE_j_111]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_111]]), read<i32>(%[[VALUE_j_111]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check55:[0-9]+]] @check55(%[[VALUE_p_112:[0-9]+]] p: ptr<@type[[TYPE_S55]]>, %[[VALUE_i_112:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_112:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE334:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_112]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_112]]), const<i32>(55))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE335:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_112]]);
// DEFAULT-NEXT:                 let %[[VALUE336:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE335]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_112]], read<i32>(%[[VALUE336]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(55)>(field0(deref(read<ptr<@type[[TYPE_S55]]>>(%[[VALUE_p_112]])))), read<i32>(%[[VALUE_j_112]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_112]]), read<i32>(%[[VALUE_j_112]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test55:[0-9]+]] @test55(%[[VALUE_s1_111:[0-9]+]] s1: @type[[TYPE_S55]], %[[VALUE_s2_111:[0-9]+]] s2: @type[[TYPE_S55]], %[[VALUE_s3_56:[0-9]+]] s3: @type[[TYPE_S55]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S55]]>, i32) -> void>(%[[VALUE_check55]], addr_of<ptr<@type[[TYPE_S55]]>>(%[[VALUE_s1_111]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S55]]>, i32) -> void>(%[[VALUE_check55]], addr_of<ptr<@type[[TYPE_S55]]>>(%[[VALUE_s2_111]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S55]]>, i32) -> void>(%[[VALUE_check55]], addr_of<ptr<@type[[TYPE_S55]]>>(%[[VALUE_s3_56]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_55:[0-9]+]] @test2_55(%[[VALUE_s1_112:[0-9]+]] s1: @type[[TYPE_S55]], %[[VALUE_s2_112:[0-9]+]] s2: @type[[TYPE_S55]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S55]], @type[[TYPE_S55]], @type[[TYPE_S55]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test55]], copy<@type[[TYPE_S55]], reason=arg>(read<@type[[TYPE_S55]]>(%[[VALUE_s1_112]])), copy<@type[[TYPE_S55]], reason=arg>(read<@type[[TYPE_S55]]>(%[[VALUE_g2s55]])), copy<@type[[TYPE_S55]], reason=arg>(read<@type[[TYPE_S55]]>(%[[VALUE_s2_112]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit55:[0-9]+]] @testit55() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S55]]>, i32) -> void>(%[[VALUE_init55]], addr_of<ptr<@type[[TYPE_S55]]>>(%[[VALUE_g1s55]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S55]]>, i32) -> void>(%[[VALUE_check55]], addr_of<ptr<@type[[TYPE_S55]]>>(%[[VALUE_g1s55]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S55]]>, i32) -> void>(%[[VALUE_init55]], addr_of<ptr<@type[[TYPE_S55]]>>(%[[VALUE_g2s55]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S55]]>, i32) -> void>(%[[VALUE_check55]], addr_of<ptr<@type[[TYPE_S55]]>>(%[[VALUE_g2s55]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S55]]>, i32) -> void>(%[[VALUE_init55]], addr_of<ptr<@type[[TYPE_S55]]>>(%[[VALUE_g3s55]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S55]]>, i32) -> void>(%[[VALUE_check55]], addr_of<ptr<@type[[TYPE_S55]]>>(%[[VALUE_g3s55]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S55]], @type[[TYPE_S55]], @type[[TYPE_S55]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test55]], copy<@type[[TYPE_S55]], reason=arg>(read<@type[[TYPE_S55]]>(%[[VALUE_g1s55]])), copy<@type[[TYPE_S55]], reason=arg>(read<@type[[TYPE_S55]]>(%[[VALUE_g2s55]])), copy<@type[[TYPE_S55]], reason=arg>(read<@type[[TYPE_S55]]>(%[[VALUE_g3s55]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S55]], @type[[TYPE_S55]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_55]], copy<@type[[TYPE_S55]], reason=arg>(read<@type[[TYPE_S55]]>(%[[VALUE_g1s55]])), copy<@type[[TYPE_S55]], reason=arg>(read<@type[[TYPE_S55]]>(%[[VALUE_g3s55]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init56:[0-9]+]] @init56(%[[VALUE_p_113:[0-9]+]] p: ptr<@type[[TYPE_S56]]>, %[[VALUE_i_113:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_113:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE337:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_113]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_113]]), const<i32>(56))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE338:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_113]]);
// DEFAULT-NEXT:                 let %[[VALUE339:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE338]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_113]], read<i32>(%[[VALUE339]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(56)>(field0(deref(read<ptr<@type[[TYPE_S56]]>>(%[[VALUE_p_113]])))), read<i32>(%[[VALUE_j_113]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_113]]), read<i32>(%[[VALUE_j_113]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check56:[0-9]+]] @check56(%[[VALUE_p_114:[0-9]+]] p: ptr<@type[[TYPE_S56]]>, %[[VALUE_i_114:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_114:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE340:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_114]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_114]]), const<i32>(56))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE341:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_114]]);
// DEFAULT-NEXT:                 let %[[VALUE342:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE341]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_114]], read<i32>(%[[VALUE342]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(56)>(field0(deref(read<ptr<@type[[TYPE_S56]]>>(%[[VALUE_p_114]])))), read<i32>(%[[VALUE_j_114]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_114]]), read<i32>(%[[VALUE_j_114]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test56:[0-9]+]] @test56(%[[VALUE_s1_113:[0-9]+]] s1: @type[[TYPE_S56]], %[[VALUE_s2_113:[0-9]+]] s2: @type[[TYPE_S56]], %[[VALUE_s3_57:[0-9]+]] s3: @type[[TYPE_S56]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S56]]>, i32) -> void>(%[[VALUE_check56]], addr_of<ptr<@type[[TYPE_S56]]>>(%[[VALUE_s1_113]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S56]]>, i32) -> void>(%[[VALUE_check56]], addr_of<ptr<@type[[TYPE_S56]]>>(%[[VALUE_s2_113]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S56]]>, i32) -> void>(%[[VALUE_check56]], addr_of<ptr<@type[[TYPE_S56]]>>(%[[VALUE_s3_57]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_56:[0-9]+]] @test2_56(%[[VALUE_s1_114:[0-9]+]] s1: @type[[TYPE_S56]], %[[VALUE_s2_114:[0-9]+]] s2: @type[[TYPE_S56]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S56]], @type[[TYPE_S56]], @type[[TYPE_S56]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test56]], copy<@type[[TYPE_S56]], reason=arg>(read<@type[[TYPE_S56]]>(%[[VALUE_s1_114]])), copy<@type[[TYPE_S56]], reason=arg>(read<@type[[TYPE_S56]]>(%[[VALUE_g2s56]])), copy<@type[[TYPE_S56]], reason=arg>(read<@type[[TYPE_S56]]>(%[[VALUE_s2_114]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit56:[0-9]+]] @testit56() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S56]]>, i32) -> void>(%[[VALUE_init56]], addr_of<ptr<@type[[TYPE_S56]]>>(%[[VALUE_g1s56]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S56]]>, i32) -> void>(%[[VALUE_check56]], addr_of<ptr<@type[[TYPE_S56]]>>(%[[VALUE_g1s56]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S56]]>, i32) -> void>(%[[VALUE_init56]], addr_of<ptr<@type[[TYPE_S56]]>>(%[[VALUE_g2s56]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S56]]>, i32) -> void>(%[[VALUE_check56]], addr_of<ptr<@type[[TYPE_S56]]>>(%[[VALUE_g2s56]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S56]]>, i32) -> void>(%[[VALUE_init56]], addr_of<ptr<@type[[TYPE_S56]]>>(%[[VALUE_g3s56]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S56]]>, i32) -> void>(%[[VALUE_check56]], addr_of<ptr<@type[[TYPE_S56]]>>(%[[VALUE_g3s56]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S56]], @type[[TYPE_S56]], @type[[TYPE_S56]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test56]], copy<@type[[TYPE_S56]], reason=arg>(read<@type[[TYPE_S56]]>(%[[VALUE_g1s56]])), copy<@type[[TYPE_S56]], reason=arg>(read<@type[[TYPE_S56]]>(%[[VALUE_g2s56]])), copy<@type[[TYPE_S56]], reason=arg>(read<@type[[TYPE_S56]]>(%[[VALUE_g3s56]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S56]], @type[[TYPE_S56]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_56]], copy<@type[[TYPE_S56]], reason=arg>(read<@type[[TYPE_S56]]>(%[[VALUE_g1s56]])), copy<@type[[TYPE_S56]], reason=arg>(read<@type[[TYPE_S56]]>(%[[VALUE_g3s56]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init57:[0-9]+]] @init57(%[[VALUE_p_115:[0-9]+]] p: ptr<@type[[TYPE_S57]]>, %[[VALUE_i_115:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_115:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE343:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_115]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_115]]), const<i32>(57))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE344:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_115]]);
// DEFAULT-NEXT:                 let %[[VALUE345:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE344]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_115]], read<i32>(%[[VALUE345]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(57)>(field0(deref(read<ptr<@type[[TYPE_S57]]>>(%[[VALUE_p_115]])))), read<i32>(%[[VALUE_j_115]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_115]]), read<i32>(%[[VALUE_j_115]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check57:[0-9]+]] @check57(%[[VALUE_p_116:[0-9]+]] p: ptr<@type[[TYPE_S57]]>, %[[VALUE_i_116:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_116:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE346:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_116]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_116]]), const<i32>(57))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE347:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_116]]);
// DEFAULT-NEXT:                 let %[[VALUE348:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE347]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_116]], read<i32>(%[[VALUE348]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(57)>(field0(deref(read<ptr<@type[[TYPE_S57]]>>(%[[VALUE_p_116]])))), read<i32>(%[[VALUE_j_116]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_116]]), read<i32>(%[[VALUE_j_116]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test57:[0-9]+]] @test57(%[[VALUE_s1_115:[0-9]+]] s1: @type[[TYPE_S57]], %[[VALUE_s2_115:[0-9]+]] s2: @type[[TYPE_S57]], %[[VALUE_s3_58:[0-9]+]] s3: @type[[TYPE_S57]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S57]]>, i32) -> void>(%[[VALUE_check57]], addr_of<ptr<@type[[TYPE_S57]]>>(%[[VALUE_s1_115]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S57]]>, i32) -> void>(%[[VALUE_check57]], addr_of<ptr<@type[[TYPE_S57]]>>(%[[VALUE_s2_115]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S57]]>, i32) -> void>(%[[VALUE_check57]], addr_of<ptr<@type[[TYPE_S57]]>>(%[[VALUE_s3_58]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_57:[0-9]+]] @test2_57(%[[VALUE_s1_116:[0-9]+]] s1: @type[[TYPE_S57]], %[[VALUE_s2_116:[0-9]+]] s2: @type[[TYPE_S57]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S57]], @type[[TYPE_S57]], @type[[TYPE_S57]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test57]], copy<@type[[TYPE_S57]], reason=arg>(read<@type[[TYPE_S57]]>(%[[VALUE_s1_116]])), copy<@type[[TYPE_S57]], reason=arg>(read<@type[[TYPE_S57]]>(%[[VALUE_g2s57]])), copy<@type[[TYPE_S57]], reason=arg>(read<@type[[TYPE_S57]]>(%[[VALUE_s2_116]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit57:[0-9]+]] @testit57() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S57]]>, i32) -> void>(%[[VALUE_init57]], addr_of<ptr<@type[[TYPE_S57]]>>(%[[VALUE_g1s57]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S57]]>, i32) -> void>(%[[VALUE_check57]], addr_of<ptr<@type[[TYPE_S57]]>>(%[[VALUE_g1s57]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S57]]>, i32) -> void>(%[[VALUE_init57]], addr_of<ptr<@type[[TYPE_S57]]>>(%[[VALUE_g2s57]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S57]]>, i32) -> void>(%[[VALUE_check57]], addr_of<ptr<@type[[TYPE_S57]]>>(%[[VALUE_g2s57]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S57]]>, i32) -> void>(%[[VALUE_init57]], addr_of<ptr<@type[[TYPE_S57]]>>(%[[VALUE_g3s57]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S57]]>, i32) -> void>(%[[VALUE_check57]], addr_of<ptr<@type[[TYPE_S57]]>>(%[[VALUE_g3s57]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S57]], @type[[TYPE_S57]], @type[[TYPE_S57]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test57]], copy<@type[[TYPE_S57]], reason=arg>(read<@type[[TYPE_S57]]>(%[[VALUE_g1s57]])), copy<@type[[TYPE_S57]], reason=arg>(read<@type[[TYPE_S57]]>(%[[VALUE_g2s57]])), copy<@type[[TYPE_S57]], reason=arg>(read<@type[[TYPE_S57]]>(%[[VALUE_g3s57]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S57]], @type[[TYPE_S57]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_57]], copy<@type[[TYPE_S57]], reason=arg>(read<@type[[TYPE_S57]]>(%[[VALUE_g1s57]])), copy<@type[[TYPE_S57]], reason=arg>(read<@type[[TYPE_S57]]>(%[[VALUE_g3s57]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init58:[0-9]+]] @init58(%[[VALUE_p_117:[0-9]+]] p: ptr<@type[[TYPE_S58]]>, %[[VALUE_i_117:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_117:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE349:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_117]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_117]]), const<i32>(58))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE350:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_117]]);
// DEFAULT-NEXT:                 let %[[VALUE351:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE350]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_117]], read<i32>(%[[VALUE351]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(58)>(field0(deref(read<ptr<@type[[TYPE_S58]]>>(%[[VALUE_p_117]])))), read<i32>(%[[VALUE_j_117]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_117]]), read<i32>(%[[VALUE_j_117]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check58:[0-9]+]] @check58(%[[VALUE_p_118:[0-9]+]] p: ptr<@type[[TYPE_S58]]>, %[[VALUE_i_118:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_118:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE352:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_118]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_118]]), const<i32>(58))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE353:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_118]]);
// DEFAULT-NEXT:                 let %[[VALUE354:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE353]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_118]], read<i32>(%[[VALUE354]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(58)>(field0(deref(read<ptr<@type[[TYPE_S58]]>>(%[[VALUE_p_118]])))), read<i32>(%[[VALUE_j_118]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_118]]), read<i32>(%[[VALUE_j_118]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test58:[0-9]+]] @test58(%[[VALUE_s1_117:[0-9]+]] s1: @type[[TYPE_S58]], %[[VALUE_s2_117:[0-9]+]] s2: @type[[TYPE_S58]], %[[VALUE_s3_59:[0-9]+]] s3: @type[[TYPE_S58]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S58]]>, i32) -> void>(%[[VALUE_check58]], addr_of<ptr<@type[[TYPE_S58]]>>(%[[VALUE_s1_117]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S58]]>, i32) -> void>(%[[VALUE_check58]], addr_of<ptr<@type[[TYPE_S58]]>>(%[[VALUE_s2_117]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S58]]>, i32) -> void>(%[[VALUE_check58]], addr_of<ptr<@type[[TYPE_S58]]>>(%[[VALUE_s3_59]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_58:[0-9]+]] @test2_58(%[[VALUE_s1_118:[0-9]+]] s1: @type[[TYPE_S58]], %[[VALUE_s2_118:[0-9]+]] s2: @type[[TYPE_S58]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S58]], @type[[TYPE_S58]], @type[[TYPE_S58]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test58]], copy<@type[[TYPE_S58]], reason=arg>(read<@type[[TYPE_S58]]>(%[[VALUE_s1_118]])), copy<@type[[TYPE_S58]], reason=arg>(read<@type[[TYPE_S58]]>(%[[VALUE_g2s58]])), copy<@type[[TYPE_S58]], reason=arg>(read<@type[[TYPE_S58]]>(%[[VALUE_s2_118]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit58:[0-9]+]] @testit58() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S58]]>, i32) -> void>(%[[VALUE_init58]], addr_of<ptr<@type[[TYPE_S58]]>>(%[[VALUE_g1s58]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S58]]>, i32) -> void>(%[[VALUE_check58]], addr_of<ptr<@type[[TYPE_S58]]>>(%[[VALUE_g1s58]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S58]]>, i32) -> void>(%[[VALUE_init58]], addr_of<ptr<@type[[TYPE_S58]]>>(%[[VALUE_g2s58]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S58]]>, i32) -> void>(%[[VALUE_check58]], addr_of<ptr<@type[[TYPE_S58]]>>(%[[VALUE_g2s58]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S58]]>, i32) -> void>(%[[VALUE_init58]], addr_of<ptr<@type[[TYPE_S58]]>>(%[[VALUE_g3s58]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S58]]>, i32) -> void>(%[[VALUE_check58]], addr_of<ptr<@type[[TYPE_S58]]>>(%[[VALUE_g3s58]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S58]], @type[[TYPE_S58]], @type[[TYPE_S58]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test58]], copy<@type[[TYPE_S58]], reason=arg>(read<@type[[TYPE_S58]]>(%[[VALUE_g1s58]])), copy<@type[[TYPE_S58]], reason=arg>(read<@type[[TYPE_S58]]>(%[[VALUE_g2s58]])), copy<@type[[TYPE_S58]], reason=arg>(read<@type[[TYPE_S58]]>(%[[VALUE_g3s58]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S58]], @type[[TYPE_S58]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_58]], copy<@type[[TYPE_S58]], reason=arg>(read<@type[[TYPE_S58]]>(%[[VALUE_g1s58]])), copy<@type[[TYPE_S58]], reason=arg>(read<@type[[TYPE_S58]]>(%[[VALUE_g3s58]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init59:[0-9]+]] @init59(%[[VALUE_p_119:[0-9]+]] p: ptr<@type[[TYPE_S59]]>, %[[VALUE_i_119:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_119:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE355:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_119]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_119]]), const<i32>(59))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE356:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_119]]);
// DEFAULT-NEXT:                 let %[[VALUE357:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE356]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_119]], read<i32>(%[[VALUE357]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(59)>(field0(deref(read<ptr<@type[[TYPE_S59]]>>(%[[VALUE_p_119]])))), read<i32>(%[[VALUE_j_119]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_119]]), read<i32>(%[[VALUE_j_119]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check59:[0-9]+]] @check59(%[[VALUE_p_120:[0-9]+]] p: ptr<@type[[TYPE_S59]]>, %[[VALUE_i_120:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_120:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE358:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_120]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_120]]), const<i32>(59))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE359:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_120]]);
// DEFAULT-NEXT:                 let %[[VALUE360:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE359]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_120]], read<i32>(%[[VALUE360]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(59)>(field0(deref(read<ptr<@type[[TYPE_S59]]>>(%[[VALUE_p_120]])))), read<i32>(%[[VALUE_j_120]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_120]]), read<i32>(%[[VALUE_j_120]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test59:[0-9]+]] @test59(%[[VALUE_s1_119:[0-9]+]] s1: @type[[TYPE_S59]], %[[VALUE_s2_119:[0-9]+]] s2: @type[[TYPE_S59]], %[[VALUE_s3_60:[0-9]+]] s3: @type[[TYPE_S59]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S59]]>, i32) -> void>(%[[VALUE_check59]], addr_of<ptr<@type[[TYPE_S59]]>>(%[[VALUE_s1_119]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S59]]>, i32) -> void>(%[[VALUE_check59]], addr_of<ptr<@type[[TYPE_S59]]>>(%[[VALUE_s2_119]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S59]]>, i32) -> void>(%[[VALUE_check59]], addr_of<ptr<@type[[TYPE_S59]]>>(%[[VALUE_s3_60]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_59:[0-9]+]] @test2_59(%[[VALUE_s1_120:[0-9]+]] s1: @type[[TYPE_S59]], %[[VALUE_s2_120:[0-9]+]] s2: @type[[TYPE_S59]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S59]], @type[[TYPE_S59]], @type[[TYPE_S59]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test59]], copy<@type[[TYPE_S59]], reason=arg>(read<@type[[TYPE_S59]]>(%[[VALUE_s1_120]])), copy<@type[[TYPE_S59]], reason=arg>(read<@type[[TYPE_S59]]>(%[[VALUE_g2s59]])), copy<@type[[TYPE_S59]], reason=arg>(read<@type[[TYPE_S59]]>(%[[VALUE_s2_120]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit59:[0-9]+]] @testit59() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S59]]>, i32) -> void>(%[[VALUE_init59]], addr_of<ptr<@type[[TYPE_S59]]>>(%[[VALUE_g1s59]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S59]]>, i32) -> void>(%[[VALUE_check59]], addr_of<ptr<@type[[TYPE_S59]]>>(%[[VALUE_g1s59]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S59]]>, i32) -> void>(%[[VALUE_init59]], addr_of<ptr<@type[[TYPE_S59]]>>(%[[VALUE_g2s59]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S59]]>, i32) -> void>(%[[VALUE_check59]], addr_of<ptr<@type[[TYPE_S59]]>>(%[[VALUE_g2s59]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S59]]>, i32) -> void>(%[[VALUE_init59]], addr_of<ptr<@type[[TYPE_S59]]>>(%[[VALUE_g3s59]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S59]]>, i32) -> void>(%[[VALUE_check59]], addr_of<ptr<@type[[TYPE_S59]]>>(%[[VALUE_g3s59]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S59]], @type[[TYPE_S59]], @type[[TYPE_S59]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test59]], copy<@type[[TYPE_S59]], reason=arg>(read<@type[[TYPE_S59]]>(%[[VALUE_g1s59]])), copy<@type[[TYPE_S59]], reason=arg>(read<@type[[TYPE_S59]]>(%[[VALUE_g2s59]])), copy<@type[[TYPE_S59]], reason=arg>(read<@type[[TYPE_S59]]>(%[[VALUE_g3s59]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S59]], @type[[TYPE_S59]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_59]], copy<@type[[TYPE_S59]], reason=arg>(read<@type[[TYPE_S59]]>(%[[VALUE_g1s59]])), copy<@type[[TYPE_S59]], reason=arg>(read<@type[[TYPE_S59]]>(%[[VALUE_g3s59]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init60:[0-9]+]] @init60(%[[VALUE_p_121:[0-9]+]] p: ptr<@type[[TYPE_S60]]>, %[[VALUE_i_121:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_121:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE361:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_121]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_121]]), const<i32>(60))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE362:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_121]]);
// DEFAULT-NEXT:                 let %[[VALUE363:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE362]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_121]], read<i32>(%[[VALUE363]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(60)>(field0(deref(read<ptr<@type[[TYPE_S60]]>>(%[[VALUE_p_121]])))), read<i32>(%[[VALUE_j_121]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_121]]), read<i32>(%[[VALUE_j_121]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check60:[0-9]+]] @check60(%[[VALUE_p_122:[0-9]+]] p: ptr<@type[[TYPE_S60]]>, %[[VALUE_i_122:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_122:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE364:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_122]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_122]]), const<i32>(60))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE365:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_122]]);
// DEFAULT-NEXT:                 let %[[VALUE366:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE365]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_122]], read<i32>(%[[VALUE366]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(60)>(field0(deref(read<ptr<@type[[TYPE_S60]]>>(%[[VALUE_p_122]])))), read<i32>(%[[VALUE_j_122]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_122]]), read<i32>(%[[VALUE_j_122]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test60:[0-9]+]] @test60(%[[VALUE_s1_121:[0-9]+]] s1: @type[[TYPE_S60]], %[[VALUE_s2_121:[0-9]+]] s2: @type[[TYPE_S60]], %[[VALUE_s3_61:[0-9]+]] s3: @type[[TYPE_S60]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S60]]>, i32) -> void>(%[[VALUE_check60]], addr_of<ptr<@type[[TYPE_S60]]>>(%[[VALUE_s1_121]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S60]]>, i32) -> void>(%[[VALUE_check60]], addr_of<ptr<@type[[TYPE_S60]]>>(%[[VALUE_s2_121]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S60]]>, i32) -> void>(%[[VALUE_check60]], addr_of<ptr<@type[[TYPE_S60]]>>(%[[VALUE_s3_61]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_60:[0-9]+]] @test2_60(%[[VALUE_s1_122:[0-9]+]] s1: @type[[TYPE_S60]], %[[VALUE_s2_122:[0-9]+]] s2: @type[[TYPE_S60]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S60]], @type[[TYPE_S60]], @type[[TYPE_S60]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test60]], copy<@type[[TYPE_S60]], reason=arg>(read<@type[[TYPE_S60]]>(%[[VALUE_s1_122]])), copy<@type[[TYPE_S60]], reason=arg>(read<@type[[TYPE_S60]]>(%[[VALUE_g2s60]])), copy<@type[[TYPE_S60]], reason=arg>(read<@type[[TYPE_S60]]>(%[[VALUE_s2_122]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit60:[0-9]+]] @testit60() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S60]]>, i32) -> void>(%[[VALUE_init60]], addr_of<ptr<@type[[TYPE_S60]]>>(%[[VALUE_g1s60]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S60]]>, i32) -> void>(%[[VALUE_check60]], addr_of<ptr<@type[[TYPE_S60]]>>(%[[VALUE_g1s60]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S60]]>, i32) -> void>(%[[VALUE_init60]], addr_of<ptr<@type[[TYPE_S60]]>>(%[[VALUE_g2s60]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S60]]>, i32) -> void>(%[[VALUE_check60]], addr_of<ptr<@type[[TYPE_S60]]>>(%[[VALUE_g2s60]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S60]]>, i32) -> void>(%[[VALUE_init60]], addr_of<ptr<@type[[TYPE_S60]]>>(%[[VALUE_g3s60]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S60]]>, i32) -> void>(%[[VALUE_check60]], addr_of<ptr<@type[[TYPE_S60]]>>(%[[VALUE_g3s60]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S60]], @type[[TYPE_S60]], @type[[TYPE_S60]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test60]], copy<@type[[TYPE_S60]], reason=arg>(read<@type[[TYPE_S60]]>(%[[VALUE_g1s60]])), copy<@type[[TYPE_S60]], reason=arg>(read<@type[[TYPE_S60]]>(%[[VALUE_g2s60]])), copy<@type[[TYPE_S60]], reason=arg>(read<@type[[TYPE_S60]]>(%[[VALUE_g3s60]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S60]], @type[[TYPE_S60]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_60]], copy<@type[[TYPE_S60]], reason=arg>(read<@type[[TYPE_S60]]>(%[[VALUE_g1s60]])), copy<@type[[TYPE_S60]], reason=arg>(read<@type[[TYPE_S60]]>(%[[VALUE_g3s60]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init61:[0-9]+]] @init61(%[[VALUE_p_123:[0-9]+]] p: ptr<@type[[TYPE_S61]]>, %[[VALUE_i_123:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_123:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE367:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_123]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_123]]), const<i32>(61))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE368:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_123]]);
// DEFAULT-NEXT:                 let %[[VALUE369:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE368]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_123]], read<i32>(%[[VALUE369]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(61)>(field0(deref(read<ptr<@type[[TYPE_S61]]>>(%[[VALUE_p_123]])))), read<i32>(%[[VALUE_j_123]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_123]]), read<i32>(%[[VALUE_j_123]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check61:[0-9]+]] @check61(%[[VALUE_p_124:[0-9]+]] p: ptr<@type[[TYPE_S61]]>, %[[VALUE_i_124:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_124:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE370:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_124]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_124]]), const<i32>(61))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE371:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_124]]);
// DEFAULT-NEXT:                 let %[[VALUE372:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE371]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_124]], read<i32>(%[[VALUE372]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(61)>(field0(deref(read<ptr<@type[[TYPE_S61]]>>(%[[VALUE_p_124]])))), read<i32>(%[[VALUE_j_124]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_124]]), read<i32>(%[[VALUE_j_124]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test61:[0-9]+]] @test61(%[[VALUE_s1_123:[0-9]+]] s1: @type[[TYPE_S61]], %[[VALUE_s2_123:[0-9]+]] s2: @type[[TYPE_S61]], %[[VALUE_s3_62:[0-9]+]] s3: @type[[TYPE_S61]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S61]]>, i32) -> void>(%[[VALUE_check61]], addr_of<ptr<@type[[TYPE_S61]]>>(%[[VALUE_s1_123]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S61]]>, i32) -> void>(%[[VALUE_check61]], addr_of<ptr<@type[[TYPE_S61]]>>(%[[VALUE_s2_123]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S61]]>, i32) -> void>(%[[VALUE_check61]], addr_of<ptr<@type[[TYPE_S61]]>>(%[[VALUE_s3_62]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_61:[0-9]+]] @test2_61(%[[VALUE_s1_124:[0-9]+]] s1: @type[[TYPE_S61]], %[[VALUE_s2_124:[0-9]+]] s2: @type[[TYPE_S61]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S61]], @type[[TYPE_S61]], @type[[TYPE_S61]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test61]], copy<@type[[TYPE_S61]], reason=arg>(read<@type[[TYPE_S61]]>(%[[VALUE_s1_124]])), copy<@type[[TYPE_S61]], reason=arg>(read<@type[[TYPE_S61]]>(%[[VALUE_g2s61]])), copy<@type[[TYPE_S61]], reason=arg>(read<@type[[TYPE_S61]]>(%[[VALUE_s2_124]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit61:[0-9]+]] @testit61() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S61]]>, i32) -> void>(%[[VALUE_init61]], addr_of<ptr<@type[[TYPE_S61]]>>(%[[VALUE_g1s61]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S61]]>, i32) -> void>(%[[VALUE_check61]], addr_of<ptr<@type[[TYPE_S61]]>>(%[[VALUE_g1s61]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S61]]>, i32) -> void>(%[[VALUE_init61]], addr_of<ptr<@type[[TYPE_S61]]>>(%[[VALUE_g2s61]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S61]]>, i32) -> void>(%[[VALUE_check61]], addr_of<ptr<@type[[TYPE_S61]]>>(%[[VALUE_g2s61]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S61]]>, i32) -> void>(%[[VALUE_init61]], addr_of<ptr<@type[[TYPE_S61]]>>(%[[VALUE_g3s61]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S61]]>, i32) -> void>(%[[VALUE_check61]], addr_of<ptr<@type[[TYPE_S61]]>>(%[[VALUE_g3s61]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S61]], @type[[TYPE_S61]], @type[[TYPE_S61]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test61]], copy<@type[[TYPE_S61]], reason=arg>(read<@type[[TYPE_S61]]>(%[[VALUE_g1s61]])), copy<@type[[TYPE_S61]], reason=arg>(read<@type[[TYPE_S61]]>(%[[VALUE_g2s61]])), copy<@type[[TYPE_S61]], reason=arg>(read<@type[[TYPE_S61]]>(%[[VALUE_g3s61]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S61]], @type[[TYPE_S61]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_61]], copy<@type[[TYPE_S61]], reason=arg>(read<@type[[TYPE_S61]]>(%[[VALUE_g1s61]])), copy<@type[[TYPE_S61]], reason=arg>(read<@type[[TYPE_S61]]>(%[[VALUE_g3s61]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init62:[0-9]+]] @init62(%[[VALUE_p_125:[0-9]+]] p: ptr<@type[[TYPE_S62]]>, %[[VALUE_i_125:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_125:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE373:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_125]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_125]]), const<i32>(62))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE374:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_125]]);
// DEFAULT-NEXT:                 let %[[VALUE375:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE374]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_125]], read<i32>(%[[VALUE375]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(62)>(field0(deref(read<ptr<@type[[TYPE_S62]]>>(%[[VALUE_p_125]])))), read<i32>(%[[VALUE_j_125]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_125]]), read<i32>(%[[VALUE_j_125]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check62:[0-9]+]] @check62(%[[VALUE_p_126:[0-9]+]] p: ptr<@type[[TYPE_S62]]>, %[[VALUE_i_126:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_126:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE376:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_126]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_126]]), const<i32>(62))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE377:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_126]]);
// DEFAULT-NEXT:                 let %[[VALUE378:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE377]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_126]], read<i32>(%[[VALUE378]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(62)>(field0(deref(read<ptr<@type[[TYPE_S62]]>>(%[[VALUE_p_126]])))), read<i32>(%[[VALUE_j_126]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_126]]), read<i32>(%[[VALUE_j_126]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test62:[0-9]+]] @test62(%[[VALUE_s1_125:[0-9]+]] s1: @type[[TYPE_S62]], %[[VALUE_s2_125:[0-9]+]] s2: @type[[TYPE_S62]], %[[VALUE_s3_63:[0-9]+]] s3: @type[[TYPE_S62]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S62]]>, i32) -> void>(%[[VALUE_check62]], addr_of<ptr<@type[[TYPE_S62]]>>(%[[VALUE_s1_125]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S62]]>, i32) -> void>(%[[VALUE_check62]], addr_of<ptr<@type[[TYPE_S62]]>>(%[[VALUE_s2_125]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S62]]>, i32) -> void>(%[[VALUE_check62]], addr_of<ptr<@type[[TYPE_S62]]>>(%[[VALUE_s3_63]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_62:[0-9]+]] @test2_62(%[[VALUE_s1_126:[0-9]+]] s1: @type[[TYPE_S62]], %[[VALUE_s2_126:[0-9]+]] s2: @type[[TYPE_S62]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S62]], @type[[TYPE_S62]], @type[[TYPE_S62]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test62]], copy<@type[[TYPE_S62]], reason=arg>(read<@type[[TYPE_S62]]>(%[[VALUE_s1_126]])), copy<@type[[TYPE_S62]], reason=arg>(read<@type[[TYPE_S62]]>(%[[VALUE_g2s62]])), copy<@type[[TYPE_S62]], reason=arg>(read<@type[[TYPE_S62]]>(%[[VALUE_s2_126]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit62:[0-9]+]] @testit62() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S62]]>, i32) -> void>(%[[VALUE_init62]], addr_of<ptr<@type[[TYPE_S62]]>>(%[[VALUE_g1s62]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S62]]>, i32) -> void>(%[[VALUE_check62]], addr_of<ptr<@type[[TYPE_S62]]>>(%[[VALUE_g1s62]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S62]]>, i32) -> void>(%[[VALUE_init62]], addr_of<ptr<@type[[TYPE_S62]]>>(%[[VALUE_g2s62]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S62]]>, i32) -> void>(%[[VALUE_check62]], addr_of<ptr<@type[[TYPE_S62]]>>(%[[VALUE_g2s62]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S62]]>, i32) -> void>(%[[VALUE_init62]], addr_of<ptr<@type[[TYPE_S62]]>>(%[[VALUE_g3s62]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S62]]>, i32) -> void>(%[[VALUE_check62]], addr_of<ptr<@type[[TYPE_S62]]>>(%[[VALUE_g3s62]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S62]], @type[[TYPE_S62]], @type[[TYPE_S62]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test62]], copy<@type[[TYPE_S62]], reason=arg>(read<@type[[TYPE_S62]]>(%[[VALUE_g1s62]])), copy<@type[[TYPE_S62]], reason=arg>(read<@type[[TYPE_S62]]>(%[[VALUE_g2s62]])), copy<@type[[TYPE_S62]], reason=arg>(read<@type[[TYPE_S62]]>(%[[VALUE_g3s62]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S62]], @type[[TYPE_S62]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_62]], copy<@type[[TYPE_S62]], reason=arg>(read<@type[[TYPE_S62]]>(%[[VALUE_g1s62]])), copy<@type[[TYPE_S62]], reason=arg>(read<@type[[TYPE_S62]]>(%[[VALUE_g3s62]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_init63:[0-9]+]] @init63(%[[VALUE_p_127:[0-9]+]] p: ptr<@type[[TYPE_S63]]>, %[[VALUE_i_127:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_127:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE379:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_127]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_127]]), const<i32>(63))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE380:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_127]]);
// DEFAULT-NEXT:                 let %[[VALUE381:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE380]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_127]], read<i32>(%[[VALUE381]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(63)>(field0(deref(read<ptr<@type[[TYPE_S63]]>>(%[[VALUE_p_127]])))), read<i32>(%[[VALUE_j_127]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_i_127]]), read<i32>(%[[VALUE_j_127]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check63:[0-9]+]] @check63(%[[VALUE_p_128:[0-9]+]] p: ptr<@type[[TYPE_S63]]>, %[[VALUE_i_128:[0-9]+]] i: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j_128:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE382:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_128]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_128]]), const<i32>(63))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE383:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_128]]);
// DEFAULT-NEXT:                 let %[[VALUE384:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE383]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_128]], read<i32>(%[[VALUE384]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(63)>(field0(deref(read<ptr<@type[[TYPE_S63]]>>(%[[VALUE_p_128]])))), read<i32>(%[[VALUE_j_128]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_128]]), read<i32>(%[[VALUE_j_128]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test63:[0-9]+]] @test63(%[[VALUE_s1_127:[0-9]+]] s1: @type[[TYPE_S63]], %[[VALUE_s2_127:[0-9]+]] s2: @type[[TYPE_S63]], %[[VALUE_s3_64:[0-9]+]] s3: @type[[TYPE_S63]]) -> void [linkage=external] [abi=sysv64(native_c, native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S63]]>, i32) -> void>(%[[VALUE_check63]], addr_of<ptr<@type[[TYPE_S63]]>>(%[[VALUE_s1_127]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S63]]>, i32) -> void>(%[[VALUE_check63]], addr_of<ptr<@type[[TYPE_S63]]>>(%[[VALUE_s2_127]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S63]]>, i32) -> void>(%[[VALUE_check63]], addr_of<ptr<@type[[TYPE_S63]]>>(%[[VALUE_s3_64]]), const<i32>(192));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2_63:[0-9]+]] @test2_63(%[[VALUE_s1_128:[0-9]+]] s1: @type[[TYPE_S63]], %[[VALUE_s2_128:[0-9]+]] s2: @type[[TYPE_S63]]) -> void [linkage=external] [abi=sysv64(native_c, native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S63]], @type[[TYPE_S63]], @type[[TYPE_S63]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test63]], copy<@type[[TYPE_S63]], reason=arg>(read<@type[[TYPE_S63]]>(%[[VALUE_s1_128]])), copy<@type[[TYPE_S63]], reason=arg>(read<@type[[TYPE_S63]]>(%[[VALUE_g2s63]])), copy<@type[[TYPE_S63]], reason=arg>(read<@type[[TYPE_S63]]>(%[[VALUE_s2_128]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testit63:[0-9]+]] @testit63() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S63]]>, i32) -> void>(%[[VALUE_init63]], addr_of<ptr<@type[[TYPE_S63]]>>(%[[VALUE_g1s63]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S63]]>, i32) -> void>(%[[VALUE_check63]], addr_of<ptr<@type[[TYPE_S63]]>>(%[[VALUE_g1s63]]), const<i32>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S63]]>, i32) -> void>(%[[VALUE_init63]], addr_of<ptr<@type[[TYPE_S63]]>>(%[[VALUE_g2s63]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S63]]>, i32) -> void>(%[[VALUE_check63]], addr_of<ptr<@type[[TYPE_S63]]>>(%[[VALUE_g2s63]]), const<i32>(128));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S63]]>, i32) -> void>(%[[VALUE_init63]], addr_of<ptr<@type[[TYPE_S63]]>>(%[[VALUE_g3s63]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S63]]>, i32) -> void>(%[[VALUE_check63]], addr_of<ptr<@type[[TYPE_S63]]>>(%[[VALUE_g3s63]]), const<i32>(192));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S63]], @type[[TYPE_S63]], @type[[TYPE_S63]]) -> void, abi=sysv64(native_c, native_c, native_c) -> void>(%[[VALUE_test63]], copy<@type[[TYPE_S63]], reason=arg>(read<@type[[TYPE_S63]]>(%[[VALUE_g1s63]])), copy<@type[[TYPE_S63]], reason=arg>(read<@type[[TYPE_S63]]>(%[[VALUE_g2s63]])), copy<@type[[TYPE_S63]], reason=arg>(read<@type[[TYPE_S63]]>(%[[VALUE_g3s63]])));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S63]], @type[[TYPE_S63]]) -> void, abi=sysv64(native_c, native_c) -> void>(%[[VALUE_test2_63]], copy<@type[[TYPE_S63]], reason=arg>(read<@type[[TYPE_S63]]>(%[[VALUE_g1s63]])), copy<@type[[TYPE_S63]], reason=arg>(read<@type[[TYPE_S63]]>(%[[VALUE_g3s63]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit0]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit1]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit2]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit3]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit4]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit5]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit6]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit7]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit8]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit9]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit10]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit11]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit12]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit13]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit14]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit15]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit16]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit17]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit18]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit19]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit20]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit21]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit22]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit23]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit24]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit25]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit26]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit27]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit28]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit29]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit30]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit31]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit32]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit33]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit34]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit35]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit36]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit37]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit38]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit39]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit40]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit41]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit42]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit43]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit44]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit45]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit46]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit47]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit48]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit49]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit50]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit51]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit52]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit53]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit54]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit55]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit56]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit57]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit58]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit59]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit60]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit61]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit62]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_testit63]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
