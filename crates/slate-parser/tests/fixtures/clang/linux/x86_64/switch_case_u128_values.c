#include <stdio.h>

typedef unsigned _BitInt(128) u128;

int classify(int seed) {
  u128 v = (u128)340282366920938463463374607431768211440uwb + (u128)seed;
  switch (v) {
  case 340282366920938463463374607431768211441uwb:
  case 340282366920938463463374607431768211443uwb:
    return 1;
  case 340282366920938463463374607431768211450uwb ... 340282366920938463463374607431768211453uwb:
    return 2;
  default:
    return 0;
  }
}

int classify_run(int seed) {
  u128 v = (u128)340282366920938463463374607431768211440uwb + (u128)seed;
  switch (v) {
  case 340282366920938463463374607431768211441uwb:
  case 340282366920938463463374607431768211442uwb:
  case 340282366920938463463374607431768211443uwb:
    return 1;
  case 340282366920938463463374607431768211450uwb:
    return 2;
  default:
    return 0;
  }
}

int main(void) {
  printf("%d %d %d %d %d %d %d\n", classify(1), classify(3), classify(11),
         classify(5), classify_run(2), classify_run(10), classify_run(5));
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
// DEFAULT-NEXT:     type @type0 u128 = u128b;
// DEFAULT-NEXT:     global %13 .str13: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%10 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @classify(%4 seed: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 v: u128b [storage=automatic] = add<u128b, overflow=wrap>(const<u128b>(340282366920938463463374607431768211440), reinterpret<u128b, reason=explicit, fits=unknown>(widen<i128b, reason=explicit>(read<i32>(%4))));
// DEFAULT-NEXT:         switch %11 read<u128b>(%5)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %11 const<u128b>(340282366920938463463374607431768211441):
// DEFAULT-NEXT:                     case %11 const<u128b>(340282366920938463463374607431768211443):
// DEFAULT-NEXT:                         return const<i32>(1);
// DEFAULT-NEXT:                 case %11 const<u128b>(340282366920938463463374607431768211450) ... const<u128b>(340282366920938463463374607431768211453):
// DEFAULT-NEXT:                     return const<i32>(2);
// DEFAULT-NEXT:                 default %11:
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @classify_run(%7 seed: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 v: u128b [storage=automatic] = add<u128b, overflow=wrap>(const<u128b>(340282366920938463463374607431768211440), reinterpret<u128b, reason=explicit, fits=unknown>(widen<i128b, reason=explicit>(read<i32>(%7))));
// DEFAULT-NEXT:         switch %12 read<u128b>(%8)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %12 const<u128b>(340282366920938463463374607431768211441):
// DEFAULT-NEXT:                     case %12 const<u128b>(340282366920938463463374607431768211442):
// DEFAULT-NEXT:                         case %12 const<u128b>(340282366920938463463374607431768211443):
// DEFAULT-NEXT:                             return const<i32>(1);
// DEFAULT-NEXT:                 case %12 const<u128b>(340282366920938463463374607431768211450):
// DEFAULT-NEXT:                     return const<i32>(2);
// DEFAULT-NEXT:                 default %12:
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(22)>(%13)), call<i32, signature=fn(i32) -> i32>(%3, const<i32>(1)), call<i32, signature=fn(i32) -> i32>(%3, const<i32>(3)), call<i32, signature=fn(i32) -> i32>(%3, const<i32>(11)), call<i32, signature=fn(i32) -> i32>(%3, const<i32>(5)), call<i32, signature=fn(i32) -> i32>(%6, const<i32>(2)), call<i32, signature=fn(i32) -> i32>(%6, const<i32>(10)), call<i32, signature=fn(i32) -> i32>(%6, const<i32>(5)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
