#include <stdio.h>

typedef _BitInt(256) big;

int classify(int seed) {
  big s = 170141183460469231731687303715884105727wb + (big)seed;
  switch (s) {
  case 170141183460469231731687303715884105727wb ... 170141183460469231731687303715884105730wb:
    return 1;
  case 170141183460469231731687303715884105740wb:
  case 170141183460469231731687303715884105745wb ... 170141183460469231731687303715884105747wb:
    return 2;
  default:
    return 0;
  }
}

int probe(int seed) {
  big s = 170141183460469231731687303715884105727wb + (big)seed;
  int v = 42;
  switch (s) {
  case 170141183460469231731687303715884105727wb ... 170141183460469231731687303715884105730wb:
    return v;
  default:
    return 0;
  }
}

int main(void) {
  printf("%d %d %d %d %d %d\n", classify(0), classify(13), classify(19),
         classify(5), probe(2), probe(9));
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
// DEFAULT-NEXT:     type @type0 big = i256b;
// DEFAULT-NEXT:     global %13 .str13: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%10 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @classify(%3 seed: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 s: i256b [storage=automatic] = add<i256b, overflow=ub>(widen<i256b, reason=usual_arith>(const<i128b>(170141183460469231731687303715884105727)), widen<i256b, reason=explicit>(read<i32>(%3)));
// DEFAULT-NEXT:         switch %11 read<i256b>(%4)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %11 const<i256b>(170141183460469231731687303715884105727) ... const<i256b>(170141183460469231731687303715884105730):
// DEFAULT-NEXT:                     return const<i32>(1);
// DEFAULT-NEXT:                 case %11 const<i256b>(170141183460469231731687303715884105740):
// DEFAULT-NEXT:                     case %11 const<i256b>(170141183460469231731687303715884105745) ... const<i256b>(170141183460469231731687303715884105747):
// DEFAULT-NEXT:                         return const<i32>(2);
// DEFAULT-NEXT:                 default %11:
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @probe(%6 seed: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 s: i256b [storage=automatic] = add<i256b, overflow=ub>(widen<i256b, reason=usual_arith>(const<i128b>(170141183460469231731687303715884105727)), widen<i256b, reason=explicit>(read<i32>(%6)));
// DEFAULT-NEXT:         let %8 v: i32 [storage=automatic] = const<i32>(42);
// DEFAULT-NEXT:         switch %12 read<i256b>(%7)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %12 const<i256b>(170141183460469231731687303715884105727) ... const<i256b>(170141183460469231731687303715884105730):
// DEFAULT-NEXT:                     return read<i32>(%8);
// DEFAULT-NEXT:                 default %12:
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%13)), call<i32, signature=fn(i32) -> i32>(%2, const<i32>(0)), call<i32, signature=fn(i32) -> i32>(%2, const<i32>(13)), call<i32, signature=fn(i32) -> i32>(%2, const<i32>(19)), call<i32, signature=fn(i32) -> i32>(%2, const<i32>(5)), call<i32, signature=fn(i32) -> i32>(%5, const<i32>(2)), call<i32, signature=fn(i32) -> i32>(%5, const<i32>(9)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
