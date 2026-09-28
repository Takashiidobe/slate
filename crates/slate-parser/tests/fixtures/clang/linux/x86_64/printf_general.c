#include <stdio.h>

int main(void) {
  printf("%g %.3g %-10.3g|\n", 1234.5678, 1234.5678, 1234.5678);
  printf("%G %.3G\n", 1234.5678, 1234.5678);
  printf("%#g %#.3g\n", 1234.5678, 1234.5678);
  printf("%+.3g %+.3g\n", 1234.5678, -1234.5678);
  printf("%15.3g|%015.3g\n", 1234.5678, 1234.5678);
  printf("%g %g %g\n", 100.0, 0.0, -0.0);
  printf("%g\n", 1e300);
  printf("%g\n", 1e-300);
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
// DEFAULT-NEXT:     global %4 .str4: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([37, 103, 32, 37, 46, 51, 103, 32, 37, 45, 49, 48, 46, 51, 103, 124, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %5 .str5: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([37, 71, 32, 37, 46, 51, 71, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %6 .str6: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([37, 35, 103, 32, 37, 35, 46, 51, 103, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 43, 46, 51, 103, 32, 37, 43, 46, 51, 103, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 49, 53, 46, 51, 103, 124, 37, 48, 49, 53, 46, 51, 103, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 103, 32, 37, 103, 32, 37, 103, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 103, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 103, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%3 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%4)), const<f64>(1234.5678), const<f64>(1234.5678), const<f64>(1234.5678));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%5)), const<f64>(1234.5678), const<f64>(1234.5678));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%6)), const<f64>(1234.5678), const<f64>(1234.5678));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%7)), const<f64>(1234.5678), neg<f64>(const<f64>(1234.5678)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%8)), const<f64>(1234.5678), const<f64>(1234.5678));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%9)), const<f64>(100.0), const<f64>(0.0), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%10)), const<f64>(1e300));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%11)), const<f64>(1e-300));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
