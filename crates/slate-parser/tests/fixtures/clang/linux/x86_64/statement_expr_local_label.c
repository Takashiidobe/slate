#include <stdio.h>

#define CHECK_VALUE(expression)                                                \
  ({                                                                           \
    __label__ failed, done;                                                    \
    int result;                                                                \
    if (!(expression))                                                         \
      goto failed;                                                             \
    result = 17;                                                               \
    goto done;                                                                 \
  failed:                                                                      \
    result = -5;                                                               \
  done:                                                                        \
    result;                                                                    \
  })

int main(void) {
  int value  = 0;
  int first  = CHECK_VALUE(++value == 1);
  int second = CHECK_VALUE(++value == 9);
  printf("%d %d %d\n", first, second, value);
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
// DEFAULT-NEXT:     global %13 .str13: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%12 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 value: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %8 first: i32 [storage=automatic];
// DEFAULT-NEXT:         let %14: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %9 result: i32 [storage=automatic];
// DEFAULT-NEXT:             let %15: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:             let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%7, read<i32>(%16));
// DEFAULT-NEXT:             if not<bool>(eq<i32>(read<i32>(%16), const<i32>(1)))
// DEFAULT-NEXT:                 goto %3;
// DEFAULT-NEXT:             write<i32>(%9, const<i32>(17));
// DEFAULT-NEXT:             goto %4;
// DEFAULT-NEXT:             label %3 failed:
// DEFAULT-NEXT:                 write<i32>(%9, neg<i32, overflow=ub>(const<i32>(5)));
// DEFAULT-NEXT:             label %4 done:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:             write<i32>(%14, read<i32>(%9));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%14));
// DEFAULT-NEXT:         let %10 second: i32 [storage=automatic];
// DEFAULT-NEXT:         let %17: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %11 result: i32 [storage=automatic];
// DEFAULT-NEXT:             let %18: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:             let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%7, read<i32>(%19));
// DEFAULT-NEXT:             if not<bool>(eq<i32>(read<i32>(%19), const<i32>(9)))
// DEFAULT-NEXT:                 goto %5;
// DEFAULT-NEXT:             write<i32>(%11, const<i32>(17));
// DEFAULT-NEXT:             goto %6;
// DEFAULT-NEXT:             label %5 failed:
// DEFAULT-NEXT:                 write<i32>(%11, neg<i32, overflow=ub>(const<i32>(5)));
// DEFAULT-NEXT:             label %6 done:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:             write<i32>(%17, read<i32>(%11));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%17));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%13)), read<i32>(%8), read<i32>(%10), read<i32>(%7));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
