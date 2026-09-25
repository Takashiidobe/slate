#include <stdio.h>

int add(int a, int b) { return a + b; }
int side_effect(void) {
  printf("effect\n");
  return 1;
}

int safe_forward(int a, int b, int c, int d) {
  int r    = add(a, b);
  int kept = c + d;
  printf("%d\n", r);
  return kept + kept;
}

void blocked_forward(int a, int b) {
  int r = add(a, b);
  printf("%d %d\n", r, side_effect());
}

int main(void) {
  printf("%d\n", safe_forward(2, 3, 4, 5));
  blocked_forward(6, 7);
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
// DEFAULT-NEXT:     global %18 .str18: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([101, 102, 102, 101, 99, 116, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%17 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @add(%2 a: i32, %3 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%2), read<i32>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @side_effect() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%18)));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @safe_forward(%6 a: i32, %7 b: i32, %8 c: i32, %9 d: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 r: i32 [storage=automatic] = call<i32, signature=fn(i32, i32) -> i32>(%1, read<i32>(%6), read<i32>(%7));
// DEFAULT-NEXT:         let %11 kept: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%8), read<i32>(%9));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%19)), read<i32>(%10));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%11), read<i32>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @blocked_forward(%13 a: i32, %14 b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %15 r: i32 [storage=automatic] = call<i32, signature=fn(i32, i32) -> i32>(%1, read<i32>(%13), read<i32>(%14));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%20)), read<i32>(%15), call<i32, signature=fn() -> i32>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%21)), call<i32, signature=fn(i32, i32, i32, i32) -> i32>(%5, const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%12, const<i32>(6), const<i32>(7));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
