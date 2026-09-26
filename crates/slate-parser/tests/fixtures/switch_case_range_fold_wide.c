#include <stdio.h>

typedef _BitInt(128) big;

int classify_u64(unsigned long long v) {
  switch (v) {
  case 18446744073709551610ULL:
  case 18446744073709551611ULL:
  case 18446744073709551612ULL:
    return 1;
  case 18446744073709551615ULL:
    return 2;
  default:
    return 0;
  }
}

int classify_bitint(int seed) {
  big v = (big)170141183460469231731687303715884105720wb + (big)seed;
  switch (v) {
  case 170141183460469231731687303715884105720wb:
  case 170141183460469231731687303715884105721wb:
  case 170141183460469231731687303715884105722wb:
    return 1;
  case 170141183460469231731687303715884105727wb:
    return 2;
  default:
    return 0;
  }
}

int main(void) {
  printf("%d %d %d %d %d %d\n", classify_u64(18446744073709551611ULL),
         classify_u64(18446744073709551615ULL), classify_u64(3),
         classify_bitint(1), classify_bitint(7), classify_bitint(4));
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
// DEFAULT-NEXT:     type @type0 big = i128b;
// DEFAULT-NEXT:     global %11 .str11: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%8 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @classify_u64(%3 v: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %9 read<u64>(%3)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %9 const<u64>(18446744073709551610):
// DEFAULT-NEXT:                     case %9 const<u64>(18446744073709551611):
// DEFAULT-NEXT:                         case %9 const<u64>(18446744073709551612):
// DEFAULT-NEXT:                             return const<i32>(1);
// DEFAULT-NEXT:                 case %9 const<u64>(18446744073709551615):
// DEFAULT-NEXT:                     return const<i32>(2);
// DEFAULT-NEXT:                 default %9:
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @classify_bitint(%5 seed: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 v: i128b [storage=automatic] = add<i128b, overflow=ub>(const<i128b>(170141183460469231731687303715884105720), widen<i128b, reason=explicit>(read<i32>(%5)));
// DEFAULT-NEXT:         switch %10 read<i128b>(%6)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %10 const<i128b>(170141183460469231731687303715884105720):
// DEFAULT-NEXT:                     case %10 const<i128b>(170141183460469231731687303715884105721):
// DEFAULT-NEXT:                         case %10 const<i128b>(170141183460469231731687303715884105722):
// DEFAULT-NEXT:                             return const<i32>(1);
// DEFAULT-NEXT:                 case %10 const<i128b>(170141183460469231731687303715884105727):
// DEFAULT-NEXT:                     return const<i32>(2);
// DEFAULT-NEXT:                 default %10:
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%11)), call<i32, signature=fn(u64) -> i32>(%2, const<u64>(18446744073709551611)), call<i32, signature=fn(u64) -> i32>(%2, const<u64>(18446744073709551615)), call<i32, signature=fn(u64) -> i32>(%2, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))), call<i32, signature=fn(i32) -> i32>(%4, const<i32>(1)), call<i32, signature=fn(i32) -> i32>(%4, const<i32>(7)), call<i32, signature=fn(i32) -> i32>(%4, const<i32>(4)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
