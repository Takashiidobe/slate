/* Test C23 auto.  Valid code, compilation tests.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

auto i = 1;
extern int i;
static auto l = 0L;
extern long l;
extern auto const d = 0.0; /* { dg-warning "initialized and declared 'extern'" } */
extern const double d;
double dx;
auto ((i2)) = 3;
extern int i2;
const auto i3 [[]] = 4;
extern int i4;
thread_local auto f = 1.0f;
float ff;
extern typeof (f) ff;
auto h = (short) 0;
extern short h;

struct s { int a; };
struct s sv;
struct s2;
enum e : int;

extern const volatile long double cvld;
extern void (*tfp) (void);

int a[10];
int *ap;

typedef int ti;

void
tf ()
{
  auto asv = (struct s) { 0 };
  extern typeof (asv) sv;
  auto s2p = (struct s2 *) 0;
  struct s3;
  auto s3p = (struct s3 *) 0;
  auto ev = (enum e) 0;
  static const auto volatile acvld = 0.5L;
  extern typeof (acvld) cvld;
  /* lvalue conversion occurs on the initializer, so losing qualifiers.  */
  auto ncd = d;
  extern typeof (ncd) dx;
  _Atomic double ad = 0.0;
  auto nad = ad;
  extern typeof (nad) dx;
  /* Function-to-pointer conversion occurs on the initializer.  */
  auto fp = tf;
  extern typeof (fp) tfp;
  /* Array-to-pointer conversion occurs on the initializer.  */
  auto aap = a;
  extern typeof (aap) ap;
  /* Shadowing a declaration from a containing scope is OK.  */
  auto i = 2L;
  extern typeof (i) l;
  /* auto can be used in for loops.  */
  for (auto ix = 2; ix < 10; ix++)
    {
      extern typeof (ix) i2;
    }
  /* auto is valid with bit-field initializers; the choice of type those have
     in expressions is unspecified but should match how _Generic handles such
     expressions.  */
  struct b { int a : 2; unsigned b : 3; } bv = { };
  auto bfa = bv.a;
  auto bfb = bv.b;
  static_assert (_Generic (bv.a, typeof (bfa) : 1, default : 2) == 1);
  static_assert (_Generic (bv.b, typeof (bfb) : 1, default : 2) == 1);
  /* The traditional meaning of auto with a type specifier is OK.  */
  auto short s;
  char auto c;
  auto struct t { int x; } t;
  /* That includes the case where the type comes from a typedef name.  */
  auto ti int_from_typedef = 3.0;
  extern typeof (int_from_typedef) i2;
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_s2:[0-9]+]] s2 = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_e:[0-9]+]] e = enum : i32 incomplete [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_ti:[0-9]+]] ti = i32;
// DEFAULT-NEXT:     type @type[[TYPE_s3:[0-9]+]] s3 = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_b:[0-9]+]] b = struct {
// DEFAULT-NEXT:         field0 a: i32 : 2;
// DEFAULT-NEXT:         field1 b: u32 : 3;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0], bit_offsets=[Some(0), Some(2)], bit_units=[(0, 1)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_t:[0-9]+]] t = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: i64 [storage=static] = const<i64>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: f64 [storage=static] [const] = const<f64>(0.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_dx:[0-9]+]] dx: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i2:[0-9]+]] i2: i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i3:[0-9]+]] i3: i32 [storage=static] [const] = const<i32>(4) [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_i4:[0-9]+]] i4: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: f32 [storage=thread] = const<f32>(1.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ff:[0-9]+]] ff: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: i16 [storage=static] = truncate<i16, reason=explicit, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sv:[0-9]+]] sv: @type[[TYPE_s]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_cvld:[0-9]+]] cvld: volatile f80 [storage=static] [const] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_tfp:[0-9]+]] tfp: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<i32, 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ap:[0-9]+]] ap: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_acvld:[0-9]+]] acvld: volatile f80 [storage=static] [const] = const<f80>(0.5) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_tf:[0-9]+]] @tf() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_asv:[0-9]+]] asv: @type[[TYPE_s]] [storage=automatic] = copy<@type[[TYPE_s]], reason=assign>(read<@type[[TYPE_s]]>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=false>(field0 = const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE_s2p:[0-9]+]] s2p: ptr<@type[[TYPE_s2]]> [storage=automatic] = null<ptr<@type[[TYPE_s2]]>>;
// DEFAULT-NEXT:         let %[[VALUE_s3p:[0-9]+]] s3p: ptr<@type[[TYPE_s3]]> [storage=automatic] = null<ptr<@type[[TYPE_s3]]>>;
// DEFAULT-NEXT:         let %[[VALUE_ev:[0-9]+]] ev: @type[[TYPE_e]] [storage=automatic] = int_to_enum<@type[[TYPE_e]], reason=explicit>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_ncd:[0-9]+]] ncd: f64 [storage=automatic] = read<f64>(%[[VALUE_d]]);
// DEFAULT-NEXT:         let %[[VALUE_ad:[0-9]+]] ad: atomic f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %[[VALUE_nad:[0-9]+]] nad: f64 [storage=automatic] = read<f64, atomic=seq_cst>(%[[VALUE_ad]]);
// DEFAULT-NEXT:         let %[[VALUE_fp:[0-9]+]] fp: ptr<fn() -> void> [storage=automatic] = function_decay<ptr<fn() -> void>>(%[[VALUE_tf]]);
// DEFAULT-NEXT:         let %[[VALUE_aap:[0-9]+]] aap: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i64 [storage=automatic] = const<i64>(2);
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_ix:[0-9]+]] ix: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_ix]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_ix]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_ix]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             counted: %[[VALUE_ix]] in const<i32>(2)..const<i32>(10)
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %[[VALUE_bv:[0-9]+]] bv: @type[[TYPE_b]] [storage=automatic] = aggregate<@type[[TYPE_b]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_bfa:[0-9]+]] bfa: i32 [storage=automatic] = read<i32>(bitfield0<unit=0, bytes=0..1, bits=0..2>(%[[VALUE_bv]]));
// DEFAULT-NEXT:         let %[[VALUE_bfb:[0-9]+]] bfb: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..1, bits=2..5>(%[[VALUE_bv]]))));
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: i16 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE_t]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_int_from_typedef:[0-9]+]] int_from_typedef: i32 [storage=automatic] = float_to_int<i32, reason=assign, out_of_range=ub, exceptions=observable>(const<f64>(3.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
