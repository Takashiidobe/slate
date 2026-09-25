#include <stdio.h>

int main(void) {
  int a[5] = {1, 2, 3, 4, 5};
  __builtin_bzero(a, sizeof(a));
  int sum = 0;
  for (int i = 0; i < 5; i++)
    sum += a[i];
  printf("%d\n", sum);
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
// DEFAULT-NEXT:     global %7 .str7: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%5 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 a: array<i32, 5> [storage=automatic] = aggregate<array<i32, 5>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4), index4 = const<i32>(5));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64) -> void>(__builtin_bzero, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i32>, length=Some(5)>(%2)), const<u64>(20));
// DEFAULT-NEXT:         let %3 sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %4 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %8: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%9));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%2), read<i32>(%4)))));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%11));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%7)), read<i32>(%3));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
