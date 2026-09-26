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
// DEFAULT-NEXT:     type @type0 a_short = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 s: i16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type1 a_int = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type2 b_int = struct {
// DEFAULT-NEXT:         field0 s: i16;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type3 a_float = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 f: f32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type4 b_float = struct {
// DEFAULT-NEXT:         field0 s: i16;
// DEFAULT-NEXT:         field1 f: f32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type5 a_double = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type6 b_double = struct {
// DEFAULT-NEXT:         field0 s: i16;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type7 c_double = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type8 d_double = struct {
// DEFAULT-NEXT:         field0 f: f32;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type9 a_ldouble = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 ld: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type10 b_ldouble = struct {
// DEFAULT-NEXT:         field0 s: i16;
// DEFAULT-NEXT:         field1 ld: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type11 c_ldouble = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 ld: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type12 d_ldouble = struct {
// DEFAULT-NEXT:         field0 f: f32;
// DEFAULT-NEXT:         field1 ld: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type13 e_ldouble = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 ld: f80;
// DEFAULT-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// DEFAULT-NEXT:     global %2 s_c_s: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(97)), field1 = truncate<i16, reason=assign, fits=always>(const<i32>(13))) [linkage=external];
// DEFAULT-NEXT:     global %4 s_c_i: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(98)), field1 = const<i32>(14)) [linkage=external];
// DEFAULT-NEXT:     global %6 s_s_i: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(15)), field1 = const<i32>(16)) [linkage=external];
// DEFAULT-NEXT:     global %8 s_c_f: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(99)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(17.0))) [linkage=external];
// DEFAULT-NEXT:     global %10 s_s_f: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(18)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(19.0))) [linkage=external];
// DEFAULT-NEXT:     global %12 s_c_d: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(100)), field1 = const<f64>(20.0)) [linkage=external];
// DEFAULT-NEXT:     global %14 s_s_d: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(21)), field1 = const<f64>(22.0)) [linkage=external];
// DEFAULT-NEXT:     global %16 s_i_d: @type7 [storage=static] = aggregate<@type7, zero_fill=false>(field0 = const<i32>(23), field1 = const<f64>(24.0)) [linkage=external];
// DEFAULT-NEXT:     global %18 s_f_d: @type8 [storage=static] = aggregate<@type8, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(25.0)), field1 = const<f64>(26.0)) [linkage=external];
// DEFAULT-NEXT:     global %20 s_c_ld: @type9 [storage=static] = aggregate<@type9, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(101)), field1 = float_widen<f80, reason=assign>(const<f64>(27.0))) [linkage=external];
// DEFAULT-NEXT:     global %22 s_s_ld: @type10 [storage=static] = aggregate<@type10, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(28)), field1 = float_widen<f80, reason=assign>(const<f64>(29.0))) [linkage=external];
// DEFAULT-NEXT:     global %24 s_i_ld: @type11 [storage=static] = aggregate<@type11, zero_fill=false>(field0 = const<i32>(30), field1 = float_widen<f80, reason=assign>(const<f64>(31.0))) [linkage=external];
// DEFAULT-NEXT:     global %26 s_f_ld: @type12 [storage=static] = aggregate<@type12, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(32.0)), field1 = float_widen<f80, reason=assign>(const<f64>(33.0))) [linkage=external];
// DEFAULT-NEXT:     global %28 s_d_ld: @type13 [storage=static] = aggregate<@type13, zero_fill=false>(field0 = const<f64>(34.0), field1 = float_widen<f80, reason=assign>(const<f64>(35.0))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %29 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%2))), const<i32>(97))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field1(%2))), const<i32>(13))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%4))), const<i32>(98))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field1(%4)), const<i32>(14))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(%6))), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field1(%6)), const<i32>(16))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%8))), const<i32>(99))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(field1(%8))), const<f64>(17.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(%10))), const<i32>(18))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(field1(%10))), const<f64>(19.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%12))), const<i32>(100))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(field1(%12)), const<f64>(20.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(%14))), const<i32>(21))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(field1(%14)), const<f64>(22.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(%16)), const<i32>(23))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(field1(%16)), const<f64>(24.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(field0(%18))), const<f64>(25.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(field1(%18)), const<f64>(26.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%20))), const<i32>(101))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(field1(%20)), float_widen<f80, reason=usual_arith>(const<f64>(27.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(%22))), const<i32>(28))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(field1(%22)), float_widen<f80, reason=usual_arith>(const<f64>(29.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(%24)), const<i32>(30))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(field1(%24)), float_widen<f80, reason=usual_arith>(const<f64>(31.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(field0(%26))), const<f64>(32.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(field1(%26)), float_widen<f80, reason=usual_arith>(const<f64>(33.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(field0(%28)), const<f64>(34.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(field1(%28)), float_widen<f80, reason=usual_arith>(const<f64>(35.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
