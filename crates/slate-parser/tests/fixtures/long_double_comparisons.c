#include <float.h>
#include <stdio.h>

int main(void) {
  long double zero = 0.0L;
  long double tiny = LDBL_TRUE_MIN;
  long double one  = 1.0L;

  printf("%d %d %d %d\n", tiny > zero, zero < tiny, one >= tiny, tiny <= one);
  printf("%d %d %d %d\n", tiny<zero, zero> tiny, one <= tiny, tiny >= one);
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
// DEFAULT-NEXT:     global %6 .str6: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%5 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 zero: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %3 tiny: f80 [storage=automatic] = const<f80>(3.64519953188247460253E-4951);
// DEFAULT-NEXT:         let %4 one: f80 [storage=automatic] = const<f80>(1);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%6)), from_bool<i32, reason=vararg>(gt<f80, exceptions=ignore>(read<f80>(%3), read<f80>(%2))), from_bool<i32, reason=vararg>(lt<f80, exceptions=ignore>(read<f80>(%2), read<f80>(%3))), from_bool<i32, reason=vararg>(ge<f80, exceptions=ignore>(read<f80>(%4), read<f80>(%3))), from_bool<i32, reason=vararg>(le<f80, exceptions=ignore>(read<f80>(%3), read<f80>(%4))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%7)), from_bool<i32, reason=vararg>(lt<f80, exceptions=ignore>(read<f80>(%3), read<f80>(%2))), from_bool<i32, reason=vararg>(gt<f80, exceptions=ignore>(read<f80>(%2), read<f80>(%3))), from_bool<i32, reason=vararg>(le<f80, exceptions=ignore>(read<f80>(%4), read<f80>(%3))), from_bool<i32, reason=vararg>(ge<f80, exceptions=ignore>(read<f80>(%3), read<f80>(%4))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
