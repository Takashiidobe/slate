/* Simple alignment checks;
   looking for compiler/assembler alignment disagreements,
   agreement between struct initialization and access.  */
void abort(void);

struct a_short {
  char  c;
  short s;
} s_c_s = {'a', 13};
struct a_int {
  char c;
  int  i;
} s_c_i = {'b', 14};
struct b_int {
  short s;
  int   i;
} s_s_i = {15, 16};
struct a_float {
  char  c;
  float f;
} s_c_f = {'c', 17.0};
struct b_float {
  short s;
  float f;
} s_s_f = {18, 19.0};
struct a_double {
  char   c;
  double d;
} s_c_d = {'d', 20.0};
struct b_double {
  short  s;
  double d;
} s_s_d = {21, 22.0};
struct c_double {
  int    i;
  double d;
} s_i_d = {23, 24.0};
struct d_double {
  float  f;
  double d;
} s_f_d = {25.0, 26.0};
struct a_ldouble {
  char        c;
  long double ld;
} s_c_ld = {'e', 27.0};
struct b_ldouble {
  short       s;
  long double ld;
} s_s_ld = {28, 29.0};
struct c_ldouble {
  int         i;
  long double ld;
} s_i_ld = {30, 31.0};
struct d_ldouble {
  float       f;
  long double ld;
} s_f_ld = {32.0, 33.0};
struct e_ldouble {
  double      d;
  long double ld;
} s_d_ld = {34.0, 35.0};

int main() {
  if (s_c_s.c != 'a')
    abort();
  if (s_c_s.s != 13)
    abort();
  if (s_c_i.c != 'b')
    abort();
  if (s_c_i.i != 14)
    abort();
  if (s_s_i.s != 15)
    abort();
  if (s_s_i.i != 16)
    abort();
  if (s_c_f.c != 'c')
    abort();
  if (s_c_f.f != 17.0)
    abort();
  if (s_s_f.s != 18)
    abort();
  if (s_s_f.f != 19.0)
    abort();
  if (s_c_d.c != 'd')
    abort();
  if (s_c_d.d != 20.0)
    abort();
  if (s_s_d.s != 21)
    abort();
  if (s_s_d.d != 22.0)
    abort();
  if (s_i_d.i != 23)
    abort();
  if (s_i_d.d != 24.0)
    abort();
  if (s_f_d.f != 25.0)
    abort();
  if (s_f_d.d != 26.0)
    abort();
  if (s_c_ld.c != 'e')
    abort();
  if (s_c_ld.ld != 27.0)
    abort();
  if (s_s_ld.s != 28)
    abort();
  if (s_s_ld.ld != 29.0)
    abort();
  if (s_i_ld.i != 30)
    abort();
  if (s_i_ld.ld != 31.0)
    abort();
  if (s_f_ld.f != 32.0)
    abort();
  if (s_f_ld.ld != 33.0)
    abort();
  if (s_d_ld.d != 34.0)
    abort();
  if (s_d_ld.ld != 35.0)
    abort();
  return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_a_short:[0-9]+]] a_short = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 s: i16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type[[TYPE_a_int:[0-9]+]] a_int = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_b_int:[0-9]+]] b_int = struct {
// DEFAULT-NEXT:         field0 s: i16;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_a_float:[0-9]+]] a_float = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: f32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_b_float:[0-9]+]] b_float = struct {
// DEFAULT-NEXT:         field0 s: i16;
// DEFAULT-NEXT:         field1 f: f32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_a_double:[0-9]+]] a_double = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_b_double:[0-9]+]] b_double = struct {
// DEFAULT-NEXT:         field0 s: i16;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_c_double:[0-9]+]] c_double = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_d_double:[0-9]+]] d_double = struct {
// DEFAULT-NEXT:         field0 f: f32;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_a_ldouble:[0-9]+]] a_ldouble = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 ld: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_b_ldouble:[0-9]+]] b_ldouble = struct {
// DEFAULT-NEXT:         field0 s: i16;
// DEFAULT-NEXT:         field1 ld: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_c_ldouble:[0-9]+]] c_ldouble = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 ld: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_d_ldouble:[0-9]+]] d_ldouble = struct {
// DEFAULT-NEXT:         field0 f: f32;
// DEFAULT-NEXT:         field1 ld: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_e_ldouble:[0-9]+]] e_ldouble = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 ld: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     global %[[VALUE_s_c_s:[0-9]+]] s_c_s: @type[[TYPE_a_short]] [storage=static] = aggregate<@type[[TYPE_a_short]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(97)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(13))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s_c_i:[0-9]+]] s_c_i: @type[[TYPE_a_int]] [storage=static] = aggregate<@type[[TYPE_a_int]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(98)), field1 = const<i32>(14)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s_s_i:[0-9]+]] s_s_i: @type[[TYPE_b_int]] [storage=static] = aggregate<@type[[TYPE_b_int]], zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(15)), field1 = const<i32>(16)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s_c_f:[0-9]+]] s_c_f: @type[[TYPE_a_float]] [storage=static] = aggregate<@type[[TYPE_a_float]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(99)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(17.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s_s_f:[0-9]+]] s_s_f: @type[[TYPE_b_float]] [storage=static] = aggregate<@type[[TYPE_b_float]], zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(18)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(19.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s_c_d:[0-9]+]] s_c_d: @type[[TYPE_a_double]] [storage=static] = aggregate<@type[[TYPE_a_double]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(100)), field1 = const<f64>(20.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s_s_d:[0-9]+]] s_s_d: @type[[TYPE_b_double]] [storage=static] = aggregate<@type[[TYPE_b_double]], zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(21)), field1 = const<f64>(22.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s_i_d:[0-9]+]] s_i_d: @type[[TYPE_c_double]] [storage=static] = aggregate<@type[[TYPE_c_double]], zero_fill=false>(field0 = const<i32>(23), field1 = const<f64>(24.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s_f_d:[0-9]+]] s_f_d: @type[[TYPE_d_double]] [storage=static] = aggregate<@type[[TYPE_d_double]], zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(25.0)), field1 = const<f64>(26.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s_c_ld:[0-9]+]] s_c_ld: @type[[TYPE_a_ldouble]] [storage=static] = aggregate<@type[[TYPE_a_ldouble]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(101)), field1 = float_widen<f80, reason=assign>(const<f64>(27.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s_s_ld:[0-9]+]] s_s_ld: @type[[TYPE_b_ldouble]] [storage=static] = aggregate<@type[[TYPE_b_ldouble]], zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(28)), field1 = float_widen<f80, reason=assign>(const<f64>(29.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s_i_ld:[0-9]+]] s_i_ld: @type[[TYPE_c_ldouble]] [storage=static] = aggregate<@type[[TYPE_c_ldouble]], zero_fill=false>(field0 = const<i32>(30), field1 = float_widen<f80, reason=assign>(const<f64>(31.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s_f_ld:[0-9]+]] s_f_ld: @type[[TYPE_d_ldouble]] [storage=static] = aggregate<@type[[TYPE_d_ldouble]], zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(32.0)), field1 = float_widen<f80, reason=assign>(const<f64>(33.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s_d_ld:[0-9]+]] s_d_ld: @type[[TYPE_e_ldouble]] [storage=static] = aggregate<@type[[TYPE_e_ldouble]], zero_fill=false>(field0 = const<f64>(34.0), field1 = float_widen<f80, reason=assign>(const<f64>(35.0))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_s_c_s]]))), const<i32>(97))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field1(%[[VALUE_s_c_s]]))), const<i32>(13))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_s_c_i]]))), const<i32>(98))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field1(%[[VALUE_s_c_i]])), const<i32>(14))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(%[[VALUE_s_s_i]]))), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field1(%[[VALUE_s_s_i]])), const<i32>(16))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_s_c_f]]))), const<i32>(99))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(field1(%[[VALUE_s_c_f]]))), const<f64>(17.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(%[[VALUE_s_s_f]]))), const<i32>(18))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(field1(%[[VALUE_s_s_f]]))), const<f64>(19.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_s_c_d]]))), const<i32>(100))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(field1(%[[VALUE_s_c_d]])), const<f64>(20.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(%[[VALUE_s_s_d]]))), const<i32>(21))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(field1(%[[VALUE_s_s_d]])), const<f64>(22.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(%[[VALUE_s_i_d]])), const<i32>(23))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(field1(%[[VALUE_s_i_d]])), const<f64>(24.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(field0(%[[VALUE_s_f_d]]))), const<f64>(25.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(field1(%[[VALUE_s_f_d]])), const<f64>(26.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_s_c_ld]]))), const<i32>(101))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(field1(%[[VALUE_s_c_ld]])), float_widen<f80, reason=usual_arith>(const<f64>(27.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(%[[VALUE_s_s_ld]]))), const<i32>(28))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(field1(%[[VALUE_s_s_ld]])), float_widen<f80, reason=usual_arith>(const<f64>(29.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(%[[VALUE_s_i_ld]])), const<i32>(30))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(field1(%[[VALUE_s_i_ld]])), float_widen<f80, reason=usual_arith>(const<f64>(31.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(field0(%[[VALUE_s_f_ld]]))), const<f64>(32.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(field1(%[[VALUE_s_f_ld]])), float_widen<f80, reason=usual_arith>(const<f64>(33.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(field0(%[[VALUE_s_d_ld]])), const<f64>(34.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(field1(%[[VALUE_s_d_ld]])), float_widen<f80, reason=usual_arith>(const<f64>(35.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
