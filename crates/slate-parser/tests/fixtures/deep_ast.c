#include <stdio.h>

#define STEP_1(value) ((value) + 1)
#define STEP_2(value) STEP_1(STEP_1(value))
#define STEP_3(value) STEP_2(STEP_2(value))
#define STEP_4(value) STEP_3(STEP_3(value))
#define STEP_5(value) STEP_4(STEP_4(value))
#define STEP_6(value) STEP_5(STEP_5(value))
#define STEP_7(value) STEP_6(STEP_6(value))
#define STEP_8(value) STEP_7(STEP_7(value))

int main(void) {
  printf("%d\n", STEP_8(0));
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
// DEFAULT-NEXT:     global %3 .str3: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%2 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%3)), add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(0), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
