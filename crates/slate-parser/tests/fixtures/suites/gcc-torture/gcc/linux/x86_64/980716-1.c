#include <stdarg.h>

void abort(void);
void exit(int);

void stub(int num, ...) {
  va_list ap;
  char   *end;
  int     i;

  for (i = 0; i < 2; i++) {
    va_start(ap, num);
    while (1) {
      end = va_arg(ap, char *);
      if (!end)
        break;
    }
    va_end(ap);
  }
}

int main() {
  stub(1, "ab", "bc", "cx", (char *)0);
  exit(0);
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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     global %13 .str13: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([97, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([99, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @exit(%10 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @stub(%5 num: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %7 end: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     va_start(%6);
// DEFAULT-NEXT:                     while %12 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<ptr<i8>>(%7, va_arg<ptr<i8>>(%6));
// DEFAULT-NEXT:                             va_arg<ptr<i8>>(%6);
// DEFAULT-NEXT:                             if not<bool>(ne<ptr<i8>>(read<ptr<i8>>(%7), null<ptr<i8>>))
// DEFAULT-NEXT:                                 break %12;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     va_end(%6);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%4, const<i32>(1), array_decay<ptr<i8>, length=Some(3)>(%13), array_decay<ptr<i8>, length=Some(3)>(%14), array_decay<ptr<i8>, length=Some(3)>(%15), null<ptr<i8>>);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
