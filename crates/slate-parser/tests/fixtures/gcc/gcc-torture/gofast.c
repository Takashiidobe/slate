/* { dg-skip-if "requires io" { freestanding } }  */

/* Program to test gcc's usage of the gofast library.  */

/* The main guiding themes are to make it trivial to add test cases over time
   and to make it easy for a program to parse the output to see if the right
   libcalls are being made.  */

#include <stdio.h>

void abort(void);
void exit(int);

float fp_add(float a, float b) { return a + b; }
float fp_sub(float a, float b) { return a - b; }
float fp_mul(float a, float b) { return a * b; }
float fp_div(float a, float b) { return a / b; }
float fp_neg(float a) { return -a; }

double dp_add(double a, double b) { return a + b; }
double dp_sub(double a, double b) { return a - b; }
double dp_mul(double a, double b) { return a * b; }
double dp_div(double a, double b) { return a / b; }
double dp_neg(double a) { return -a; }

double fp_to_dp(float f) { return f; }
float  dp_to_fp(double d) { return d; }

int eqsf2(float a, float b) { return a == b; }
int nesf2(float a, float b) { return a != b; }
int gtsf2(float a, float b) { return a > b; }
int gesf2(float a, float b) { return a >= b; }
int ltsf2(float a, float b) { return a < b; }
int lesf2(float a, float b) { return a <= b; }

int eqdf2(double a, double b) { return a == b; }
int nedf2(double a, double b) { return a != b; }
int gtdf2(double a, double b) { return a > b; }
int gedf2(double a, double b) { return a >= b; }
int ltdf2(double a, double b) { return a < b; }
int ledf2(double a, double b) { return a <= b; }

float        floatsisf(int i) { return i; }
double       floatsidf(int i) { return i; }
int          fixsfsi(float f) { return f; }
int          fixdfsi(double d) { return d; }
unsigned int fixunssfsi(float f) { return f; }
unsigned int fixunsdfsi(double d) { return d; }

int fail_count = 0;

int fail(char *msg) {
  fail_count++;
  fprintf(stderr, "Test failed: %s\n", msg);
}

int main() {
  if (fp_add(1, 1) != 2)
    fail("fp_add 1+1");
  if (fp_sub(3, 2) != 1)
    fail("fp_sub 3-2");
  if (fp_mul(2, 3) != 6)
    fail("fp_mul 2*3");
  if (fp_div(3, 2) != 1.5)
    fail("fp_div 3/2");
  if (fp_neg(1) != -1)
    fail("fp_neg 1");

  if (dp_add(1, 1) != 2)
    fail("dp_add 1+1");
  if (dp_sub(3, 2) != 1)
    fail("dp_sub 3-2");
  if (dp_mul(2, 3) != 6)
    fail("dp_mul 2*3");
  if (dp_div(3, 2) != 1.5)
    fail("dp_div 3/2");
  if (dp_neg(1) != -1)
    fail("dp_neg 1");

  if (fp_to_dp(1.5) != 1.5)
    fail("fp_to_dp 1.5");
  if (dp_to_fp(1.5) != 1.5)
    fail("dp_to_fp 1.5");

  if (floatsisf(1) != 1)
    fail("floatsisf 1");
  if (floatsidf(1) != 1)
    fail("floatsidf 1");
  if (fixsfsi(1.42) != 1)
    fail("fixsfsi 1.42");
  if (fixunssfsi(1.42) != 1)
    fail("fixunssfsi 1.42");
  if (fixdfsi(1.42) != 1)
    fail("fixdfsi 1.42");
  if (fixunsdfsi(1.42) != 1)
    fail("fixunsdfsi 1.42");

  if (eqsf2(1, 1) == 0)
    fail("eqsf2 1==1");
  if (eqsf2(1, 2) != 0)
    fail("eqsf2 1==2");
  if (nesf2(1, 2) == 0)
    fail("nesf2 1!=1");
  if (nesf2(1, 1) != 0)
    fail("nesf2 1!=1");
  if (gtsf2(2, 1) == 0)
    fail("gtsf2 2>1");
  if (gtsf2(1, 1) != 0)
    fail("gtsf2 1>1");
  if (gtsf2(0, 1) != 0)
    fail("gtsf2 0>1");
  if (gesf2(2, 1) == 0)
    fail("gesf2 2>=1");
  if (gesf2(1, 1) == 0)
    fail("gesf2 1>=1");
  if (gesf2(0, 1) != 0)
    fail("gesf2 0>=1");
  if (ltsf2(1, 2) == 0)
    fail("ltsf2 1<2");
  if (ltsf2(1, 1) != 0)
    fail("ltsf2 1<1");
  if (ltsf2(1, 0) != 0)
    fail("ltsf2 1<0");
  if (lesf2(1, 2) == 0)
    fail("lesf2 1<=2");
  if (lesf2(1, 1) == 0)
    fail("lesf2 1<=1");
  if (lesf2(1, 0) != 0)
    fail("lesf2 1<=0");

  if (fail_count != 0)
    abort();
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
// DEFAULT-NEXT:     type @type0 __uint64_t = u64;
// DEFAULT-NEXT:     type @type1 __off_t = i64;
// DEFAULT-NEXT:     type @type2 __off64_t = i64;
// DEFAULT-NEXT:     type @type3 _IO_FILE = struct incomplete;
// DEFAULT-NEXT:     type @type4 FILE = @type3;
// DEFAULT-NEXT:     type @type5 _IO_lock_t = void;
// DEFAULT-NEXT:     type @type6 _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type7 _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type8 _IO_wide_data = struct incomplete;
// DEFAULT-NEXT:     extern %9 stderr: ptr<@type3> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %93 fail_count: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %100 .str100: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([84, 101, 115, 116, 32, 102, 97, 105, 108, 101, 100, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %101 .str101: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([102, 112, 95, 97, 100, 100, 32, 49, 43, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %102 .str102: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([102, 112, 95, 115, 117, 98, 32, 51, 45, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %103 .str103: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([102, 112, 95, 109, 117, 108, 32, 50, 42, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %104 .str104: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([102, 112, 95, 100, 105, 118, 32, 51, 47, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %105 .str105: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([102, 112, 95, 110, 101, 103, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %106 .str106: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 112, 95, 97, 100, 100, 32, 49, 43, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %107 .str107: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 112, 95, 115, 117, 98, 32, 51, 45, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %108 .str108: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 112, 95, 109, 117, 108, 32, 50, 42, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %109 .str109: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 112, 95, 100, 105, 118, 32, 51, 47, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %110 .str110: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([100, 112, 95, 110, 101, 103, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %111 .str111: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([102, 112, 95, 116, 111, 95, 100, 112, 32, 49, 46, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %112 .str112: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([100, 112, 95, 116, 111, 95, 102, 112, 32, 49, 46, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %113 .str113: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([102, 108, 111, 97, 116, 115, 105, 115, 102, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %114 .str114: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([102, 108, 111, 97, 116, 115, 105, 100, 102, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %115 .str115: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([102, 105, 120, 115, 102, 115, 105, 32, 49, 46, 52, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %116 .str116: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([102, 105, 120, 117, 110, 115, 115, 102, 115, 105, 32, 49, 46, 52, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %117 .str117: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([102, 105, 120, 100, 102, 115, 105, 32, 49, 46, 52, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %118 .str118: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([102, 105, 120, 117, 110, 115, 100, 102, 115, 105, 32, 49, 46, 52, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %119 .str119: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([101, 113, 115, 102, 50, 32, 49, 61, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %120 .str120: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([101, 113, 115, 102, 50, 32, 49, 61, 61, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %121 .str121: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([110, 101, 115, 102, 50, 32, 49, 33, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %122 .str122: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([110, 101, 115, 102, 50, 32, 49, 33, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %123 .str123: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([103, 116, 115, 102, 50, 32, 50, 62, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %124 .str124: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([103, 116, 115, 102, 50, 32, 49, 62, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %125 .str125: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([103, 116, 115, 102, 50, 32, 48, 62, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %126 .str126: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([103, 101, 115, 102, 50, 32, 50, 62, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %127 .str127: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([103, 101, 115, 102, 50, 32, 49, 62, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %128 .str128: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([103, 101, 115, 102, 50, 32, 48, 62, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %129 .str129: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 116, 115, 102, 50, 32, 49, 60, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %130 .str130: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 116, 115, 102, 50, 32, 49, 60, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %131 .str131: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 116, 115, 102, 50, 32, 49, 60, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %132 .str132: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([108, 101, 115, 102, 50, 32, 49, 60, 61, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %133 .str133: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([108, 101, 115, 102, 50, 32, 49, 60, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %134 .str134: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([108, 101, 115, 102, 50, 32, 49, 60, 61, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %10 @fprintf(%97 __stream: ptr<@type3> [restrict], %98 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %11 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %12 @exit(%99 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %13 @fp_add(%14 a: f32, %15 b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%14), read<f32>(%15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @fp_sub(%17 a: f32, %18 b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%17), read<f32>(%18));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @fp_mul(%20 a: f32, %21 b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%20), read<f32>(%21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @fp_div(%23 a: f32, %24 b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%23), read<f32>(%24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @fp_neg(%26 a: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f32>(read<f32>(%26));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @dp_add(%28 a: f64, %29 b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%28), read<f64>(%29));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @dp_sub(%31 a: f64, %32 b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%31), read<f64>(%32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @dp_mul(%34 a: f64, %35 b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%34), read<f64>(%35));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @dp_div(%37 a: f64, %38 b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%37), read<f64>(%38));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @dp_neg(%40 a: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f64>(read<f64>(%40));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @fp_to_dp(%42 f: f32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_widen<f64, reason=return>(read<f32>(%42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @dp_to_fp(%44 d: f64) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_narrow<f32, reason=return, rounding=nearest_even, exceptions=ignore>(read<f64>(%44));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @eqsf2(%46 a: f32, %47 b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<f32, exceptions=ignore>(read<f32>(%46), read<f32>(%47)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @nesf2(%49 a: f32, %50 b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<f32, exceptions=ignore>(read<f32>(%49), read<f32>(%50)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %51 @gtsf2(%52 a: f32, %53 b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<f32, exceptions=ignore>(read<f32>(%52), read<f32>(%53)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @gesf2(%55 a: f32, %56 b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<f32, exceptions=ignore>(read<f32>(%55), read<f32>(%56)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @ltsf2(%58 a: f32, %59 b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<f32, exceptions=ignore>(read<f32>(%58), read<f32>(%59)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @lesf2(%61 a: f32, %62 b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<f32, exceptions=ignore>(read<f32>(%61), read<f32>(%62)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %63 @eqdf2(%64 a: f64, %65 b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<f64, exceptions=ignore>(read<f64>(%64), read<f64>(%65)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @nedf2(%67 a: f64, %68 b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<f64, exceptions=ignore>(read<f64>(%67), read<f64>(%68)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %69 @gtdf2(%70 a: f64, %71 b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<f64, exceptions=ignore>(read<f64>(%70), read<f64>(%71)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %72 @gedf2(%73 a: f64, %74 b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<f64, exceptions=ignore>(read<f64>(%73), read<f64>(%74)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %75 @ltdf2(%76 a: f64, %77 b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<f64, exceptions=ignore>(read<f64>(%76), read<f64>(%77)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %78 @ledf2(%79 a: f64, %80 b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<f64, exceptions=ignore>(read<f64>(%79), read<f64>(%80)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %81 @floatsisf(%82 i: i32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<f32, reason=return, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%82));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %83 @floatsidf(%84 i: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<f64, reason=return, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%84));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %85 @fixsfsi(%86 f: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i32, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%86));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %87 @fixdfsi(%88 d: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i32, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%88));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %89 @fixunssfsi(%90 f: f32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u32, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%90));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %91 @fixunsdfsi(%92 d: f64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u32, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%92));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %94 @fail(%95 msg: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %135: i32 [synthetic] = read<i32>(%93);
// DEFAULT-NEXT:         let %136: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%135), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%93, read<i32>(%136));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%10, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%100)), read<ptr<i8>>(%95));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %96 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%13, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%101));
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%16, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%102));
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%19, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(6)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%103));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn(f32, f32) -> f32>(%22, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))), const<f64>(1.5))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%104));
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%25, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(9)>(%105));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%27, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%106));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%30, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%107));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%33, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(3))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(6)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%108));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%36, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), const<f64>(1.5))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%109));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%39, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(9)>(%110));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f32) -> f64>(%41, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5))), const<f64>(1.5))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(13)>(%111));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn(f64) -> f32>(%43, const<f64>(1.5))), const<f64>(1.5))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(13)>(%112));
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(i32) -> f32>(%81, const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(12)>(%113));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(i32) -> f64>(%83, const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(12)>(%114));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32) -> i32>(%85, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.42))), const<i32>(1))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(13)>(%115));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(f32) -> u32>(%89, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.42))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(16)>(%116));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64) -> i32>(%87, const<f64>(1.42)), const<i32>(1))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(13)>(%117));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(f64) -> u32>(%91, const<f64>(1.42)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(16)>(%118));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%45, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%119));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%45, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%120));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%48, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%121));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%48, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%122));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%51, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(10)>(%123));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%51, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(10)>(%124));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%51, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(10)>(%125));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%54, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%126));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%54, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%127));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%54, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%128));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%57, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(10)>(%129));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%57, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(10)>(%130));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%57, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(10)>(%131));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%60, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%132));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%60, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%133));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%60, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%94, array_decay<ptr<i8>, length=Some(11)>(%134));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%93), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
