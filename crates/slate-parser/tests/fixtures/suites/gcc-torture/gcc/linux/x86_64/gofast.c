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
// DEFAULT-NEXT:     type @type[[TYPE___uint64_t:[0-9]+]] __uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___off_t:[0-9]+]] __off_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___off64_t:[0-9]+]] __off64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE__IO_FILE:[0-9]+]] _IO_FILE = struct {
// DEFAULT-NEXT:         field0 _flags: i32;
// DEFAULT-NEXT:         field1 _IO_read_ptr: ptr<i8>;
// DEFAULT-NEXT:         field2 _IO_read_end: ptr<i8>;
// DEFAULT-NEXT:         field3 _IO_read_base: ptr<i8>;
// DEFAULT-NEXT:         field4 _IO_write_base: ptr<i8>;
// DEFAULT-NEXT:         field5 _IO_write_ptr: ptr<i8>;
// DEFAULT-NEXT:         field6 _IO_write_end: ptr<i8>;
// DEFAULT-NEXT:         field7 _IO_buf_base: ptr<i8>;
// DEFAULT-NEXT:         field8 _IO_buf_end: ptr<i8>;
// DEFAULT-NEXT:         field9 _IO_save_base: ptr<i8>;
// DEFAULT-NEXT:         field10 _IO_backup_base: ptr<i8>;
// DEFAULT-NEXT:         field11 _IO_save_end: ptr<i8>;
// DEFAULT-NEXT:         field12 _markers: ptr<@type[[TYPE__IO_marker:[0-9]+]]>;
// DEFAULT-NEXT:         field13 _chain: ptr<@type[[TYPE__IO_FILE]]>;
// DEFAULT-NEXT:         field14 _fileno: i32;
// DEFAULT-NEXT:         field15 _flags2: i32 : 24;
// DEFAULT-NEXT:         field16 _short_backupbuf: array<i8, 1>;
// DEFAULT-NEXT:         field17 _old_offset: i64;
// DEFAULT-NEXT:         field18 _cur_column: u16;
// DEFAULT-NEXT:         field19 _vtable_offset: i8;
// DEFAULT-NEXT:         field20 _shortbuf: array<i8, 1>;
// DEFAULT-NEXT:         field21 _lock: ptr<void>;
// DEFAULT-NEXT:         field22 _offset: i64;
// DEFAULT-NEXT:         field23 _codecvt: ptr<@type[[TYPE__IO_codecvt:[0-9]+]]>;
// DEFAULT-NEXT:         field24 _wide_data: ptr<@type[[TYPE__IO_wide_data:[0-9]+]]>;
// DEFAULT-NEXT:         field25 _freeres_list: ptr<@type[[TYPE__IO_FILE]]>;
// DEFAULT-NEXT:         field26 _freeres_buf: ptr<void>;
// DEFAULT-NEXT:         field27 _prevchain: ptr<ptr<@type[[TYPE__IO_FILE]]>>;
// DEFAULT-NEXT:         field28 _mode: i32;
// DEFAULT-NEXT:         field29 _unused3: i32;
// DEFAULT-NEXT:         field30 _total_written: u64;
// DEFAULT-NEXT:         field31 _unused2: array<i8, 8>;
// DEFAULT-NEXT:     } [size=216, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 116, 119, 120, 128, 130, 131, 136, 144, 152, 160, 168, 176, 184, 192, 196, 200, 208], bit_offsets=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(928), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None], bit_units=[(116, 3)], field_units=[None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, Some(0), None, None, None, None, None, None, None, None, None, None, None, None, None, None, None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_FILE:[0-9]+]] FILE = @type[[TYPE__IO_FILE]];
// DEFAULT-NEXT:     type @type[[TYPE__IO_marker]] _IO_marker = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE__IO_codecvt]] _IO_codecvt = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE__IO_wide_data]] _IO_wide_data = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE__IO_lock_t:[0-9]+]] _IO_lock_t = void;
// DEFAULT-NEXT:     extern %[[VALUE_stderr:[0-9]+]] stderr: ptr<@type[[TYPE__IO_FILE]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_fail_count:[0-9]+]] fail_count: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([84, 101, 115, 116, 32, 102, 97, 105, 108, 101, 100, 58, 32, 37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([102, 112, 95, 97, 100, 100, 32, 49, 43, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([102, 112, 95, 115, 117, 98, 32, 51, 45, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([102, 112, 95, 109, 117, 108, 32, 50, 42, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([102, 112, 95, 100, 105, 118, 32, 51, 47, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([102, 112, 95, 110, 101, 103, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 112, 95, 97, 100, 100, 32, 49, 43, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 112, 95, 115, 117, 98, 32, 51, 45, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 112, 95, 109, 117, 108, 32, 50, 42, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([100, 112, 95, 100, 105, 118, 32, 51, 47, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([100, 112, 95, 110, 101, 103, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([102, 112, 95, 116, 111, 95, 100, 112, 32, 49, 46, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([100, 112, 95, 116, 111, 95, 102, 112, 32, 49, 46, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([102, 108, 111, 97, 116, 115, 105, 115, 102, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([102, 108, 111, 97, 116, 115, 105, 100, 102, 32, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([102, 105, 120, 115, 102, 115, 105, 32, 49, 46, 52, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([102, 105, 120, 117, 110, 115, 115, 102, 115, 105, 32, 49, 46, 52, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([102, 105, 120, 100, 102, 115, 105, 32, 49, 46, 52, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([102, 105, 120, 117, 110, 115, 100, 102, 115, 105, 32, 49, 46, 52, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([101, 113, 115, 102, 50, 32, 49, 61, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([101, 113, 115, 102, 50, 32, 49, 61, 61, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([110, 101, 115, 102, 50, 32, 49, 33, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([110, 101, 115, 102, 50, 32, 49, 33, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([103, 116, 115, 102, 50, 32, 50, 62, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([103, 116, 115, 102, 50, 32, 49, 62, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([103, 116, 115, 102, 50, 32, 48, 62, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([103, 101, 115, 102, 50, 32, 50, 62, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_28:[0-9]+]] .str[[VALUE_str_28]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([103, 101, 115, 102, 50, 32, 49, 62, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_29:[0-9]+]] .str[[VALUE_str_29]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([103, 101, 115, 102, 50, 32, 48, 62, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_30:[0-9]+]] .str[[VALUE_str_30]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 116, 115, 102, 50, 32, 49, 60, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_31:[0-9]+]] .str[[VALUE_str_31]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 116, 115, 102, 50, 32, 49, 60, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_32:[0-9]+]] .str[[VALUE_str_32]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([108, 116, 115, 102, 50, 32, 49, 60, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_33:[0-9]+]] .str[[VALUE_str_33]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([108, 101, 115, 102, 50, 32, 49, 60, 61, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_34:[0-9]+]] .str[[VALUE_str_34]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([108, 101, 115, 102, 50, 32, 49, 60, 61, 49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_35:[0-9]+]] .str[[VALUE_str_35]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([108, 101, 115, 102, 50, 32, 49, 60, 61, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_fprintf:[0-9]+]] @fprintf(%[[VALUE___stream:[0-9]+]] __stream: ptr<@type[[TYPE__IO_FILE]]> [restrict], %[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fp_add:[0-9]+]] @fp_add(%[[VALUE_a:[0-9]+]] a: f32, %[[VALUE_b:[0-9]+]] b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_a]]), read<f32>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fp_sub:[0-9]+]] @fp_sub(%[[VALUE_a_2:[0-9]+]] a: f32, %[[VALUE_b_2:[0-9]+]] b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_a_2]]), read<f32>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fp_mul:[0-9]+]] @fp_mul(%[[VALUE_a_3:[0-9]+]] a: f32, %[[VALUE_b_3:[0-9]+]] b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_a_3]]), read<f32>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fp_div:[0-9]+]] @fp_div(%[[VALUE_a_4:[0-9]+]] a: f32, %[[VALUE_b_4:[0-9]+]] b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_a_4]]), read<f32>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fp_neg:[0-9]+]] @fp_neg(%[[VALUE_a_5:[0-9]+]] a: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f32>(read<f32>(%[[VALUE_a_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dp_add:[0-9]+]] @dp_add(%[[VALUE_a_6:[0-9]+]] a: f64, %[[VALUE_b_5:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_a_6]]), read<f64>(%[[VALUE_b_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dp_sub:[0-9]+]] @dp_sub(%[[VALUE_a_7:[0-9]+]] a: f64, %[[VALUE_b_6:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_a_7]]), read<f64>(%[[VALUE_b_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dp_mul:[0-9]+]] @dp_mul(%[[VALUE_a_8:[0-9]+]] a: f64, %[[VALUE_b_7:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_a_8]]), read<f64>(%[[VALUE_b_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dp_div:[0-9]+]] @dp_div(%[[VALUE_a_9:[0-9]+]] a: f64, %[[VALUE_b_8:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_a_9]]), read<f64>(%[[VALUE_b_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dp_neg:[0-9]+]] @dp_neg(%[[VALUE_a_10:[0-9]+]] a: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f64>(read<f64>(%[[VALUE_a_10]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fp_to_dp:[0-9]+]] @fp_to_dp(%[[VALUE_f:[0-9]+]] f: f32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_widen<f64, reason=return>(read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dp_to_fp:[0-9]+]] @dp_to_fp(%[[VALUE_d:[0-9]+]] d: f64) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_narrow<f32, reason=return, rounding=nearest_even, exceptions=observable>(read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_eqsf2:[0-9]+]] @eqsf2(%[[VALUE_a_11:[0-9]+]] a: f32, %[[VALUE_b_9:[0-9]+]] b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<f32, exceptions=observable>(read<f32>(%[[VALUE_a_11]]), read<f32>(%[[VALUE_b_9]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nesf2:[0-9]+]] @nesf2(%[[VALUE_a_12:[0-9]+]] a: f32, %[[VALUE_b_10:[0-9]+]] b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<f32, exceptions=observable>(read<f32>(%[[VALUE_a_12]]), read<f32>(%[[VALUE_b_10]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gtsf2:[0-9]+]] @gtsf2(%[[VALUE_a_13:[0-9]+]] a: f32, %[[VALUE_b_11:[0-9]+]] b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<f32, exceptions=observable>(read<f32>(%[[VALUE_a_13]]), read<f32>(%[[VALUE_b_11]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gesf2:[0-9]+]] @gesf2(%[[VALUE_a_14:[0-9]+]] a: f32, %[[VALUE_b_12:[0-9]+]] b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<f32, exceptions=observable>(read<f32>(%[[VALUE_a_14]]), read<f32>(%[[VALUE_b_12]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ltsf2:[0-9]+]] @ltsf2(%[[VALUE_a_15:[0-9]+]] a: f32, %[[VALUE_b_13:[0-9]+]] b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_a_15]]), read<f32>(%[[VALUE_b_13]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lesf2:[0-9]+]] @lesf2(%[[VALUE_a_16:[0-9]+]] a: f32, %[[VALUE_b_14:[0-9]+]] b: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<f32, exceptions=observable>(read<f32>(%[[VALUE_a_16]]), read<f32>(%[[VALUE_b_14]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_eqdf2:[0-9]+]] @eqdf2(%[[VALUE_a_17:[0-9]+]] a: f64, %[[VALUE_b_15:[0-9]+]] b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<f64, exceptions=observable>(read<f64>(%[[VALUE_a_17]]), read<f64>(%[[VALUE_b_15]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nedf2:[0-9]+]] @nedf2(%[[VALUE_a_18:[0-9]+]] a: f64, %[[VALUE_b_16:[0-9]+]] b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ne<f64, exceptions=observable>(read<f64>(%[[VALUE_a_18]]), read<f64>(%[[VALUE_b_16]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gtdf2:[0-9]+]] @gtdf2(%[[VALUE_a_19:[0-9]+]] a: f64, %[[VALUE_b_17:[0-9]+]] b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<f64, exceptions=observable>(read<f64>(%[[VALUE_a_19]]), read<f64>(%[[VALUE_b_17]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gedf2:[0-9]+]] @gedf2(%[[VALUE_a_20:[0-9]+]] a: f64, %[[VALUE_b_18:[0-9]+]] b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(ge<f64, exceptions=observable>(read<f64>(%[[VALUE_a_20]]), read<f64>(%[[VALUE_b_18]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ltdf2:[0-9]+]] @ltdf2(%[[VALUE_a_21:[0-9]+]] a: f64, %[[VALUE_b_19:[0-9]+]] b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(lt<f64, exceptions=observable>(read<f64>(%[[VALUE_a_21]]), read<f64>(%[[VALUE_b_19]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ledf2:[0-9]+]] @ledf2(%[[VALUE_a_22:[0-9]+]] a: f64, %[[VALUE_b_20:[0-9]+]] b: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(le<f64, exceptions=observable>(read<f64>(%[[VALUE_a_22]]), read<f64>(%[[VALUE_b_20]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_floatsisf:[0-9]+]] @floatsisf(%[[VALUE_i:[0-9]+]] i: i32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<f32, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_floatsidf:[0-9]+]] @floatsidf(%[[VALUE_i_2:[0-9]+]] i: i32) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return int_to_float<f64, reason=return, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fixsfsi:[0-9]+]] @fixsfsi(%[[VALUE_f_2:[0-9]+]] f: f32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i32, reason=return, out_of_range=ub, exceptions=observable>(read<f32>(%[[VALUE_f_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fixdfsi:[0-9]+]] @fixdfsi(%[[VALUE_d_2:[0-9]+]] d: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i32, reason=return, out_of_range=ub, exceptions=observable>(read<f64>(%[[VALUE_d_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fixunssfsi:[0-9]+]] @fixunssfsi(%[[VALUE_f_3:[0-9]+]] f: f32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u32, reason=return, out_of_range=ub, exceptions=observable>(read<f32>(%[[VALUE_f_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fixunsdfsi:[0-9]+]] @fixunsdfsi(%[[VALUE_d_3:[0-9]+]] d: f64) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<u32, reason=return, out_of_range=ub, exceptions=observable>(read<f64>(%[[VALUE_d_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fail:[0-9]+]] @fail(%[[VALUE_msg:[0-9]+]] msg: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_fail_count]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_fail_count]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE__IO_FILE]]>, ptr<const i8>, ...) -> i32>(%[[VALUE_fprintf]], read<ptr<@type[[TYPE__IO_FILE]]>>(%[[VALUE_stderr]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(17)>(%[[VALUE_str]])), read<ptr<i8>>(%[[VALUE_msg]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fp_add]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_2]]));
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fp_sub]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_3]]));
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fp_mul]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(6)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_4]]));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fp_div]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), const<f64>(1.5))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_5]]));
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fp_neg]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_6]]));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_dp_add]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_7]]));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_dp_sub]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_8]]));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_dp_mul]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(3))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(6)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_9]]));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_dp_div]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2))), const<f64>(1.5))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_10]]));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_dp_neg]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_11]]));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f32) -> f64>(%[[VALUE_fp_to_dp]], float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=observable>(const<f64>(1.5))), const<f64>(1.5))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_12]]));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(float_widen<f64, reason=usual_arith>(call<f32, signature=fn(f64) -> f32>(%[[VALUE_dp_to_fp]], const<f64>(1.5))), const<f64>(1.5))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_13]]));
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(i32) -> f32>(%[[VALUE_floatsisf]], const<i32>(1)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_14]]));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(i32) -> f64>(%[[VALUE_floatsidf]], const<i32>(1)), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_15]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32) -> i32>(%[[VALUE_fixsfsi]], float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=observable>(const<f64>(1.42))), const<i32>(1))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_16]]));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(f32) -> u32>(%[[VALUE_fixunssfsi]], float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=observable>(const<f64>(1.42))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_17]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64) -> i32>(%[[VALUE_fixdfsi]], const<f64>(1.42)), const<i32>(1))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_18]]));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(f64) -> u32>(%[[VALUE_fixunsdfsi]], const<f64>(1.42)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_19]]));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_eqsf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_20]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_eqsf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_21]]));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_nesf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_22]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_nesf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_23]]));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_gtsf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_24]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_gtsf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_25]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_gtsf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_26]]));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_gesf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_27]]));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_gesf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_28]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_gesf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_29]]));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_ltsf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_30]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_ltsf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_31]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_ltsf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_32]]));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_lesf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_33]]));
// DEFAULT-NEXT:         if eq<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_lesf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_34]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32, f32) -> i32>(%[[VALUE_lesf2]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))), const<i32>(0))
// DEFAULT-NEXT:             call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_fail]], array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_35]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_fail_count]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
