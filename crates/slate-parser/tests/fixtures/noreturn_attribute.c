#include <stdio.h>
#include <stdlib.h>

__attribute__((noreturn)) void die(int code) {
  printf("dying with %d\n", code);
  exit(code);
}

int main(void) {
  if (0) {
    die(1);
  }
  printf("main\n");
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
// DEFAULT-NEXT:     global %7 .str7: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([100, 121, 105, 110, 103, 32, 119, 105, 116, 104, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([109, 97, 105, 110, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%5 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%6 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @die(%3 code: i32) -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%7)), read<i32>(%3));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, read<i32>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(const<i32>(0), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%2, const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%8)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
