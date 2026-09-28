#include <stdio.h>

int main(void) {
  char  greeting[] = "hé";
  char  mutate[]   = "abc";
  char *p          = mutate;
  *p               = 'Z';
  printf("%s\n", greeting);
  printf("%c\n", mutate[0]);
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
// DEFAULT-NEXT:     global %7 .str7: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 99, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%6 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 greeting: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([104, 195, 169, 0]);
// DEFAULT-NEXT:         let %4 mutate: array<i8, 4> [storage=automatic] = code_units<array<i8, 4>>([97, 98, 99, 0]);
// DEFAULT-NEXT:         let %5 p: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(4)>(%4);
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%5)), truncate<i8, reason=assign, fits=always>(const<i32>(90)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%7)), array_decay<ptr<i8>, length=Some(4)>(%3));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%8)), widen<i32, reason=vararg>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%4), const<i32>(0))))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
