#include <stdio.h>

static volatile char   marker = 65;
static volatile double gain   = 1.5;

struct VolatileFields {
  volatile int    count;
  volatile double ratio;
};

static volatile int bump_return(int value) { return value + 1; }

static double read_volatile_param(volatile double value) { return value + 0.5; }

static double use_volatile_fields(double input) {
  struct VolatileFields fields;
  fields.count = bump_return(4);
  fields.ratio = input + gain;
  return fields.ratio + fields.count;
}

int main(void) {
  marker = marker + 1;
  gain   = read_volatile_param(gain);
  printf("%c\n", marker);
  printf("%f\n", gain);
  printf("%f\n", use_volatile_fields(2.0));
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
// DEFAULT-NEXT:     type @type[[TYPE_VolatileFields:[0-9]+]] VolatileFields = struct {
// DEFAULT-NEXT:         field0 count: volatile i32;
// DEFAULT-NEXT:         field1 ratio: volatile f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_marker:[0-9]+]] marker: volatile i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(65)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_gain:[0-9]+]] gain: volatile f64 [storage=static] = const<f64>(1.5) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 99, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bump_return:[0-9]+]] @bump_return(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_value]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_read_volatile_param:[0-9]+]] @read_volatile_param(%[[VALUE_value_2:[0-9]+]] value: volatile f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64, volatile>(%[[VALUE_value_2]]), const<f64>(0.5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use_volatile_fields:[0-9]+]] @use_volatile_fields(%[[VALUE_input:[0-9]+]] input: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_fields:[0-9]+]] fields: @type[[TYPE_VolatileFields]] [storage=automatic];
// DEFAULT-NEXT:         write<i32, volatile>(field0(%[[VALUE_fields]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE_bump_return]], const<i32>(4)));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_bump_return]], const<i32>(4));
// DEFAULT-NEXT:         write<f64, volatile>(field1(%[[VALUE_fields]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_input]]), read<f64, volatile>(%[[VALUE_gain]])));
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64, volatile>(field1(%[[VALUE_fields]])), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32, volatile>(field0(%[[VALUE_fields]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i8, volatile>(%[[VALUE_marker]], truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8, volatile>(%[[VALUE_marker]])), const<i32>(1))));
// DEFAULT-NEXT:         write<f64, volatile>(%[[VALUE_gain]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_read_volatile_param]], read<f64, volatile>(%[[VALUE_gain]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_read_volatile_param]], read<f64, volatile>(%[[VALUE_gain]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), widen<i32, reason=vararg>(read<i8, volatile>(%[[VALUE_marker]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), read<f64, volatile>(%[[VALUE_gain]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE_use_volatile_fields]], const<f64>(2.0)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
