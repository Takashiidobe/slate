#include <stdio.h>

int main(void) {
  printf("%s %c %c %d\n", "tag", 'A', 10, 7);
  printf("literal=%s", "tail");
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
// DEFAULT-NEXT:     global %3 .str3: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 115, 32, 37, 99, 32, 37, 99, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %4 .str4: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([116, 97, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %5 .str5: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([108, 105, 116, 101, 114, 97, 108, 61, 37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %6 .str6: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 97, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%2 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%3)), array_decay<ptr<i8>, length=Some(4)>(%4), const<i32>(65), const<i32>(10), const<i32>(7));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%5)), array_decay<ptr<i8>, length=Some(5)>(%6));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
