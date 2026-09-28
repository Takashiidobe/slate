#include <stdio.h>

__attribute__((nonnull(1, 3))) static int combine(int *left, int scale,
                                                  int *right, int *optional) {
  return *left + scale * *right + (optional ? *optional : 0);
}

__attribute__((nonnull)) static int difference(int *left, int scale,
                                               int *right) {
  return *left - scale * *right;
}

int main(void) {
  int left  = 11;
  int right = 3;
  printf("%d %d\n", combine(&left, 2, &right, NULL),
         difference(&left, 2, &right));
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
// DEFAULT-NEXT:     global %15 .str15: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%14 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @combine(%3 left: ptr<i32>, %4 scale: i32, %5 right: ptr<i32>, %6 optional: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%3))), mul<i32, overflow=ub>(read<i32>(%4), read<i32>(deref(read<ptr<i32>>(%5))))), conditional<i32>(ne<ptr<i32>>(read<ptr<i32>>(%6), null<ptr<i32>>), read<i32>(deref(read<ptr<i32>>(%6))), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @difference(%8 left: ptr<i32>, %9 scale: i32, %10 right: ptr<i32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%8))), mul<i32, overflow=ub>(read<i32>(%9), read<i32>(deref(read<ptr<i32>>(%10)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 left: i32 [storage=automatic] = const<i32>(11);
// DEFAULT-NEXT:         let %13 right: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%15)), call<i32, signature=fn(ptr<i32>, i32, ptr<i32>, ptr<i32>) -> i32>(%2, addr_of<ptr<i32>>(%12), const<i32>(2), addr_of<ptr<i32>>(%13), null<ptr<i32>>), call<i32, signature=fn(ptr<i32>, i32, ptr<i32>) -> i32>(%7, addr_of<ptr<i32>>(%12), const<i32>(2), addr_of<ptr<i32>>(%13)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
