/* PR target/64979 */

#include <stdarg.h>

void __attribute__((noinline, noclone)) bar(int x, va_list *ap) {
  if (ap) {
    int i;
    for (i = 0; i < 10; i++)
      if (i != va_arg(*ap, int))
        __builtin_abort();
    if (va_arg(*ap, double) != 0.5)
      __builtin_abort();
  }
}

void __attribute__((noinline, noclone)) foo(int x, ...) {
  va_list ap;
  int     n;

  va_start(ap, x);
  n = va_arg(ap, int);
  bar(x, (va_list *)((n == 0) ? ((void *)0) : &ap));
  va_end(ap);
}

int main() {
  foo(100, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0.5);
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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     fn %12 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @bar(%3 x: i32, %4 ap: ptr<va_list>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<ptr<va_list>>(read<ptr<va_list>>(%4), null<ptr<va_list>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %5 i: i32 [storage=automatic];
// DEFAULT-NEXT:                 for %11
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%5), const<i32>(10))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %13: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                         let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%5, read<i32>(%14));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         if ne<i32>(read<i32>(%5), va_arg<i32>(deref(read<ptr<va_list>>(%4))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:                 if ne<f64, exceptions=observable>(va_arg<f64>(deref(read<ptr<va_list>>(%4))), const<f64>(0.5))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @foo(%7 x: i32, ...) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %9 n: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%8);
// DEFAULT-NEXT:         write<i32>(%9, va_arg<i32>(%8));
// DEFAULT-NEXT:         va_arg<i32>(%8);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<va_list>) -> void>(%2, read<i32>(%7), conditional<ptr<va_list>>(eq<i32>(read<i32>(%9), const<i32>(0)), null<ptr<va_list>>, addr_of<ptr<va_list>>(%8)));
// DEFAULT-NEXT:         va_end(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%6, const<i32>(100), const<i32>(1), const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<f64>(0.5));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
