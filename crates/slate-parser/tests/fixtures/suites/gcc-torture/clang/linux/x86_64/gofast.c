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
// DEFAULT-NEXT:     global %95 fail_count: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %102 .str102: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([84, 101, 115, 116, 32, 102, 97, 105, 108, 101, 100, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %103 .str103: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([102, 112, 95, 97, 100, 100, 32, 49, 43, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %104 .str104: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([102, 112, 95, 115, 117, 98, 32, 51, 45, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %105 .str105: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([102, 112, 95, 109, 117, 108, 32, 50, 42, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %106 .str106: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([102, 112, 95, 100, 105, 118, 32, 51, 47, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %107 .str107: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([102, 112, 95, 110, 101, 103, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %108 .str108: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 112, 95, 97, 100, 100, 32, 49, 43, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %109 .str109: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 112, 95, 115, 117, 98, 32, 51, 45, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %110 .str110: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 112, 95, 109, 117, 108, 32, 50, 42, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %111 .str111: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 112, 95, 100, 105, 118, 32, 51, 47, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %112 .str112: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([100, 112, 95, 110, 101, 103, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %113 .str113: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([102, 112, 95, 116, 111, 95, 100, 112, 32, 49, 46, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %114 .str114: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([100, 112, 95, 116, 111, 95, 102, 112, 32, 49, 46, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %115 .str115: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([102, 108, 111, 97, 116, 115, 105, 115, 102, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %116 .str116: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([102, 108, 111, 97, 116, 115, 105, 100, 102, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %117 .str117: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([102, 105, 120, 115, 102, 115, 105, 32, 49, 46, 52, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %118 .str118: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([102, 105, 120, 117, 110, 115, 115, 102, 115, 105, 32, 49, 46, 52, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %119 .str119: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([102, 105, 120, 100, 102, 115, 105, 32, 49, 46, 52, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %120 .str120: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([102, 105, 120, 117, 110, 115, 100, 102, 115, 105, 32, 49, 46, 52, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %121 .str121: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([101, 113, 115, 102, 50, 32, 49, 61, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %122 .str122: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([101, 113, 115, 102, 50, 32, 49, 61, 61, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %123 .str123: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([110, 101, 115, 102, 50, 32, 49, 33, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %124 .str124: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([110, 101, 115, 102, 50, 32, 49, 33, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %125 .str125: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([103, 116, 115, 102, 50, 32, 50, 62, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %126 .str126: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([103, 116, 115, 102, 50, 32, 49, 62, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %127 .str127: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([103, 116, 115, 102, 50, 32, 48, 62, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %128 .str128: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([103, 101, 115, 102, 50, 32, 50, 62, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %129 .str129: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([103, 101, 115, 102, 50, 32, 49, 62, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %130 .str130: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([103, 101, 115, 102, 50, 32, 48, 62, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %131 .str131: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 116, 115, 102, 50, 32, 49, 60, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %132 .str132: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 116, 115, 102, 50, 32, 49, 60, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %133 .str133: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 116, 115, 102, 50, 32, 49, 60, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %134 .str134: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([108, 101, 115, 102, 50, 32, 49, 60, 61, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %135 .str135: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([108, 101, 115, 102, 50, 32, 49, 60, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %136 .str136: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([108, 101, 115, 102, 50, 32, 49, 60, 61, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %12 @fprintf(%99 __stream: ptr<@type3> [restrict], %100 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %14 @exit(%101 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %15 @fp_add(%16 a: f32, %17 b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%16), read<f32>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @fp_sub(%19 a: f32, %20 b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%19), read<f32>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @fp_mul(%22 a: f32, %23 b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%22), read<f32>(%23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @fp_div(%25 a: f32, %26 b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%25), read<f32>(%26));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @fp_neg(%28 a: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f32>(read<f32>(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @dp_add(%30 a: f64, %31 b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%30), read<f64>(%31));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @dp_sub(%33 a: f64, %34 b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%33), read<f64>(%34));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @dp_mul(%36 a: f64, %37 b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%36), read<f64>(%37));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @dp_div(%39 a: f64, %40 b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%39), read<f64>(%40));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @dp_neg(%42 a: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f64>(read<f64>(%42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @fp_to_dp(%44 f: f32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_widen<f64, reason=return>(read<f32>(%44));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @dp_to_fp(%46 d: f64) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_narrow<f32, reason=return, rounding=nearest_even, exceptions=ignore>(read<f64>(%46));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @eqsf2(%48 a: f32, %49 b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<f32, exceptions=ignore>(read<f32>(%48), read<f32>(%49)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @nesf2(%51 a: f32, %52 b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<f32, exceptions=ignore>(read<f32>(%51), read<f32>(%52)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %53 @gtsf2(%54 a: f32, %55 b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<f32, exceptions=ignore>(read<f32>(%54), read<f32>(%55)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @gesf2(%57 a: f32, %58 b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<f32, exceptions=ignore>(read<f32>(%57), read<f32>(%58)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %59 @ltsf2(%60 a: f32, %61 b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<f32, exceptions=ignore>(read<f32>(%60), read<f32>(%61)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %62 @lesf2(%63 a: f32, %64 b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<f32, exceptions=ignore>(read<f32>(%63), read<f32>(%64)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %65 @eqdf2(%66 a: f64, %67 b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<f64, exceptions=ignore>(read<f64>(%66), read<f64>(%67)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %68 @nedf2(%69 a: f64, %70 b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<f64, exceptions=ignore>(read<f64>(%69), read<f64>(%70)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %71 @gtdf2(%72 a: f64, %73 b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<f64, exceptions=ignore>(read<f64>(%72), read<f64>(%73)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %74 @gedf2(%75 a: f64, %76 b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<f64, exceptions=ignore>(read<f64>(%75), read<f64>(%76)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %77 @ltdf2(%78 a: f64, %79 b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<f64, exceptions=ignore>(read<f64>(%78), read<f64>(%79)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %80 @ledf2(%81 a: f64, %82 b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<f64, exceptions=ignore>(read<f64>(%81), read<f64>(%82)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %83 @floatsisf(%84 i: i32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<f32, reason=return, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%84));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %85 @floatsidf(%86 i: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<f64, reason=return, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%86));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %87 @fixsfsi(%88 f: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i32, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%88));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %89 @fixdfsi(%90 d: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i32, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%90));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %91 @fixunssfsi(%92 f: f32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u32, reason=return, out_of_range=ub, exceptions=ignore>(read<f32>(%92));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %93 @fixunsdfsi(%94 d: f64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u32, reason=return, out_of_range=ub, exceptions=ignore>(read<f64>(%94));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %96 @fail(%97 msg: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %137: i32 [synthetic] = read<i32>(%95);
// DEFAULT-NEXT:         let %138: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%137), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%95, read<i32>(%138));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type3>, ptr<const i8>, ...) -> i32>(%12, read<ptr<@type3>>(%9), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%102)), read<ptr<i8>>(%97));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %98 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%15, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%103));
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%18, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%104));
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%21, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(6)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%105));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn(f32, f32) -> f32>(%24, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))), const<f64>(1.5))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%106));
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%27, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(9)>(%107));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%29, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%108));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%32, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%109));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%35, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(3))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(6)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%110));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%38, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(3)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), const<f64>(1.5))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%111));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%41, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(9)>(%112));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f32) -> f64>(%43, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.5))), const<f64>(1.5))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(13)>(%113));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn(f64) -> f32>(%45, const<f64>(1.5))), const<f64>(1.5))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(13)>(%114));
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(i32) -> f32>(%83, const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(12)>(%115));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(i32) -> f64>(%85, const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(12)>(%116));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32) -> i32>(%87, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.42))), const<i32>(1))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(13)>(%117));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(f32) -> u32>(%91, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.42))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(16)>(%118));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64) -> i32>(%89, const<f64>(1.42)), const<i32>(1))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(13)>(%119));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(f64) -> u32>(%93, const<f64>(1.42)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(16)>(%120));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%47, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%121));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%47, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%122));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%50, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%123));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%50, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%124));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%53, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(10)>(%125));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%53, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(10)>(%126));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%53, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(10)>(%127));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%56, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%128));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%56, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%129));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%56, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%130));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%59, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(10)>(%131));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%59, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(10)>(%132));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%59, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(10)>(%133));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%62, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%134));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%62, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%135));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%62, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%96, array_decay<ptr<i8>, length=Some(11)>(%136));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%95), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%14, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
