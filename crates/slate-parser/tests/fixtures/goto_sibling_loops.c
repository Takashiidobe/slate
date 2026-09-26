#include <stdio.h>

int main() {
  int i    = 0;
  int k    = 0;
  int sum  = 0;
  int prod = 1;
first:
  sum = sum + i;
  i   = i + 1;
  if (i < 5)
    goto first;
  k = 1;
second:
  prod = prod * k;
  k    = k + 1;
  if (k < 5)
    goto second;
  printf("%d %d\n", sum, prod);
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
// DEFAULT-NEXT:     global %9 .str9: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%8 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %5 k: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %6 sum: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %7 prod: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         label %2 first:
// DEFAULT-NEXT:             write<i32>(%6, add<i32, overflow=ub>(read<i32>(%6), read<i32>(%4)));
// DEFAULT-NEXT:         write<i32>(%4, add<i32, overflow=ub>(read<i32>(%4), const<i32>(1)));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%4), const<i32>(5))
// DEFAULT-NEXT:             goto %2;
// DEFAULT-NEXT:         write<i32>(%5, const<i32>(1));
// DEFAULT-NEXT:         label %3 second:
// DEFAULT-NEXT:             write<i32>(%7, mul<i32, overflow=ub>(read<i32>(%7), read<i32>(%5)));
// DEFAULT-NEXT:         write<i32>(%5, add<i32, overflow=ub>(read<i32>(%5), const<i32>(1)));
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%5), const<i32>(5))
// DEFAULT-NEXT:             goto %3;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%9)), read<i32>(%6), read<i32>(%7));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
