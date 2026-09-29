/* { dg-do run }

   # AVR doubles are floats
   { dg-skip-if "AVR doubles are floats" { avr-*-* } } */
extern void abort();

typedef union {
  struct {
    unsigned int hi;
    unsigned int lo;
  } i;
  double d;
} hexdouble;

static const double twoTo52 = 0x1.0p+52;

void func(double x) {
  hexdouble       argument;
  register double y, z;
  unsigned int    xHead;
  argument.d = x;
  xHead      = argument.i.hi & 0x7fffffff;
  if (__builtin_expect(!!(xHead < 0x43300000u), 1)) {
    y = (x - twoTo52) + twoTo52;
    if (y != x)
      abort();
    z = x - 0.5;
    y = (z - twoTo52) + twoTo52;
    if (y == ((x - twoTo52) + twoTo52))
      abort();
  }
  return;
}

int main() {
  if (sizeof(double) == 4)
    return 0;
  func((double)1.00);
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 i: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = struct {
// DEFAULT-NEXT:         field0 hi: u32;
// DEFAULT-NEXT:         field1 lo: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_hexdouble:[0-9]+]] hexdouble = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_twoTo52:[0-9]+]] twoTo52: f64 [storage=static] [const] = const<f64>(4503599627370496.0) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expect:[0-9]+]] @__builtin_expect(%[[VALUE0:[0-9]+]] <unnamed>: i64, %[[VALUE1:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_func:[0-9]+]] @func(%[[VALUE_x:[0-9]+]] x: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_argument:[0-9]+]] argument: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_xHead:[0-9]+]] xHead: u32 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(field1(%[[VALUE_argument]]), read<f64>(%[[VALUE_x]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_xHead]], and<u32>(read<u32>(field0(field0(%[[VALUE_argument]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647))));
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE___builtin_expect]], from_bool<i64, reason=arg>(not<bool>(not<bool>(lt<u32>(read<u32>(%[[VALUE_xHead]]), const<u32>(1127219200))))), widen<i64, reason=arg>(const<i32>(1))), const<i64>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64>(%[[VALUE_y]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_x]]), read<f64>(%[[VALUE_twoTo52]])), read<f64>(%[[VALUE_twoTo52]])));
// DEFAULT-NEXT:                 if ne<f64, exceptions=observable>(read<f64>(%[[VALUE_y]]), read<f64>(%[[VALUE_x]]))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 write<f64>(%[[VALUE_z]], sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_x]]), const<f64>(0.5)));
// DEFAULT-NEXT:                 write<f64>(%[[VALUE_y]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_z]]), read<f64>(%[[VALUE_twoTo52]])), read<f64>(%[[VALUE_twoTo52]])));
// DEFAULT-NEXT:                 if eq<f64, exceptions=observable>(read<f64>(%[[VALUE_y]]), add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_x]]), read<f64>(%[[VALUE_twoTo52]])), read<f64>(%[[VALUE_twoTo52]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if eq<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(%[[VALUE_func]], const<f64>(1.0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
