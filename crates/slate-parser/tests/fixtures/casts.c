#include <stdio.h>

int main(void) {
  double        d  = 7.75;
  float         f  = (float)d;
  int           i  = (int)d;
  unsigned int  u  = (unsigned int)-1;
  long          l  = (long)i;
  unsigned char c  = (unsigned char)260;
  _Bool         b0 = (_Bool)0.0;
  _Bool         b1 = (_Bool)-3;
  printf("%f\n", f);
  printf("%d\n", i);
  printf("%u\n", u);
  printf("%ld\n", l);
  printf("%u\n", c);
  printf("%d %d\n", b0, b1);
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
// DEFAULT-NEXT:     global %11 .str11: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([37, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%10 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 d: f64 [storage=automatic] = const<f64>(7.75);
// DEFAULT-NEXT:         let %3 f: f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<f64>(%2));
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(%2));
// DEFAULT-NEXT:         let %5 u: u32 [storage=automatic] = reinterpret<u32, reason=explicit, fits=unknown>(neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         let %6 l: i64 [storage=automatic] = widen<i64, reason=explicit>(read<i32>(%4));
// DEFAULT-NEXT:         let %7 c: u8 [storage=automatic] = reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(260)));
// DEFAULT-NEXT:         let %8 b0: bool [storage=automatic] = ne<f64, reason=explicit, exceptions=ignore>(const<f64>(0.0), const<f64>(0.0));
// DEFAULT-NEXT:         let %9 b1: bool [storage=automatic] = ne<i32, reason=explicit>(neg<i32, overflow=ub>(const<i32>(3)), const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%11)), float_widen<f64, reason=vararg>(read<f32>(%3)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%12)), read<i32>(%4));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%13)), read<u32>(%5));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%14)), read<i64>(%6));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%15)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(%7))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%16)), from_bool<i32, reason=vararg>(read<bool>(%8)), from_bool<i32, reason=vararg>(read<bool>(%9)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
