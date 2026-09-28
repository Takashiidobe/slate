#include <stdio.h>

typedef int (*BinaryOp)(int, int);

static int add(int a, int b) { return a + b; }
static int sub(int a, int b) { return a - b; }

static int apply(int useAdd, int a, int b) {
  return (useAdd ? add : sub)(a, b);
}

int main(void) {
  int      useAdd = 1;
  BinaryOp op     = useAdd ? add : sub;
  printf("%d %d %d %d\n", op(10, 3), apply(0, 10, 3), apply(1, 4, 4),
         (useAdd ? sub : add)(9, 2));
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
// DEFAULT-NEXT:     type @type0 BinaryOp = ptr<fn(i32, i32) -> i32>;
// DEFAULT-NEXT:     global %17 .str17: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%16 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @add(%4 a: i32, %5 b: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%4), read<i32>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @sub(%7 a: i32, %8 b: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(%7), read<i32>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @apply(%10 useAdd: i32, %11 a: i32, %12 b: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(conditional<ptr<fn(i32, i32) -> i32>>(ne<i32>(read<i32>(%10), const<i32>(0)), function_decay<ptr<fn(i32, i32) -> i32>>(%3), function_decay<ptr<fn(i32, i32) -> i32>>(%6)), read<i32>(%11), read<i32>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 useAdd: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %15 op: ptr<fn(i32, i32) -> i32> [storage=automatic] = conditional<ptr<fn(i32, i32) -> i32>>(ne<i32>(read<i32>(%14), const<i32>(0)), function_decay<ptr<fn(i32, i32) -> i32>>(%3), function_decay<ptr<fn(i32, i32) -> i32>>(%6));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%17)), call<i32, signature=fn(i32, i32) -> i32>(read<ptr<fn(i32, i32) -> i32>>(%15), const<i32>(10), const<i32>(3)), call<i32, signature=fn(i32, i32, i32) -> i32>(%9, const<i32>(0), const<i32>(10), const<i32>(3)), call<i32, signature=fn(i32, i32, i32) -> i32>(%9, const<i32>(1), const<i32>(4), const<i32>(4)), call<i32, signature=fn(i32, i32) -> i32>(conditional<ptr<fn(i32, i32) -> i32>>(ne<i32>(read<i32>(%14), const<i32>(0)), function_decay<ptr<fn(i32, i32) -> i32>>(%6), function_decay<ptr<fn(i32, i32) -> i32>>(%3)), const<i32>(9), const<i32>(2)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
