#include <stdio.h>

typedef _BitInt(256) big;

int classify(int seed) {
  big v = 170141183460469231731687303715884105727wb + (big)seed;
  switch (v) {
  case 170141183460469231731687303715884105727wb:
  case 170141183460469231731687303715884105728wb:
  case 170141183460469231731687303715884105729wb:
    return 1;
  case 170141183460469231731687303715884105736wb:
    return 2;
  case -170141183460469231731687303715884105727wb:
    return 3;
  default:
    return 0;
  }
}

int main(void) {
  printf("%d %d %d %d\n", classify(0), classify(2), classify(9), classify(5));
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
// DEFAULT-NEXT:     type @type[[TYPE_big:[0-9]+]] big = i256b;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_classify:[0-9]+]] @classify(%[[VALUE_seed:[0-9]+]] seed: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: i256b [storage=automatic] = add<i256b, overflow=ub>(widen<i256b, reason=usual_arith>(const<i128b>(170141183460469231731687303715884105727)), widen<i256b, reason=explicit>(read<i32>(%[[VALUE_seed]])));
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] read<i256b>(%[[VALUE_v]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i256b>(170141183460469231731687303715884105727):
// DEFAULT-NEXT:                     case %[[VALUE0]] const<i256b>(170141183460469231731687303715884105728):
// DEFAULT-NEXT:                         case %[[VALUE0]] const<i256b>(170141183460469231731687303715884105729):
// DEFAULT-NEXT:                             return const<i32>(1);
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i256b>(170141183460469231731687303715884105736):
// DEFAULT-NEXT:                     return const<i32>(2);
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i256b>(-170141183460469231731687303715884105727):
// DEFAULT-NEXT:                     return const<i32>(3);
// DEFAULT-NEXT:                 default %[[VALUE0]]:
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_classify]], const<i32>(0)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_classify]], const<i32>(2)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_classify]], const<i32>(9)), call<i32, signature=fn(i32) -> i32>(%[[VALUE_classify]], const<i32>(5)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
