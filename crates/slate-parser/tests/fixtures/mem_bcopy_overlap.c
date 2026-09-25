#include <stdio.h>

int main(void) {
  char buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  __builtin_bcopy(buf, buf + 2, 4);
  for (int i = 0; i < 8; i++)
    printf("%d ", buf[i]);
  printf("\n");
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
// DEFAULT-NEXT:     global %6 .str6: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%4 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 buf: array<i8, 8> [storage=automatic] = aggregate<array<i8, 8>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(3)), index3 = truncate<i8, reason=assign, fits=always>(const<i32>(4)), index4 = truncate<i8, reason=assign, fits=always>(const<i32>(5)), index5 = truncate<i8, reason=assign, fits=always>(const<i32>(6)), index6 = truncate<i8, reason=assign, fits=always>(const<i32>(7)), index7 = truncate<i8, reason=assign, fits=always>(const<i32>(8)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ptr<void>, u64) -> void>(__builtin_bcopy, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%2)), pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%2), const<i32>(2))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         for %5
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %3 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%3), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %8: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%8), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%9));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%6)), widen<i32, reason=vararg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(8)>(%2), read<i32>(%3))))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(2)>(%7)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
