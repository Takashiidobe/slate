#include <stdio.h>

static int jump_probe(int x) {
  static void *targets[] = {&&zero, &&one, &&two};
  if (x < 0 || x > 2) {
    return -1;
  }
  goto *targets[x];

zero:
  return 10;
one:
  return 20;
two:
  return 30;
}

int main(void) {
  volatile int input = 2;
  printf("%d\n", jump_probe(input));
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
// DEFAULT-NEXT:     global %7 targets: array<ptr<void>, 3> [storage=static] [align=16] = aggregate<array<ptr<void>, 3>, zero_fill=false>(index0 = label_addr<ptr<void>>(%3), index1 = label_addr<ptr<void>>(%4), index2 = label_addr<ptr<void>>(%5)) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%10 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @jump_probe(%6 x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(lt<i32>(read<i32>(%6), const<i32>(0)), gt<i32>(read<i32>(%6), const<i32>(2)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         goto *read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(3)>(%7), read<i32>(%6))));
// DEFAULT-NEXT:         label %3 zero:
// DEFAULT-NEXT:             return const<i32>(10);
// DEFAULT-NEXT:         label %4 one:
// DEFAULT-NEXT:             return const<i32>(20);
// DEFAULT-NEXT:         label %5 two:
// DEFAULT-NEXT:             return const<i32>(30);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 input: volatile i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%11)), call<i32, signature=fn(i32) -> i32>(%2, read<i32, volatile>(%9)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
