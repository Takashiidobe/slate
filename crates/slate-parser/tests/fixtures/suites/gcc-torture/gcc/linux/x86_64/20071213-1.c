/* PR target/34281 */

#include <stdarg.h>

extern void abort(void);

void h(int x, va_list ap) {
  switch (x) {
  case 1:
    if (va_arg(ap, int) != 3 || va_arg(ap, int) != 4)
      abort();
    return;
  case 5:
    if (va_arg(ap, int) != 9 || va_arg(ap, int) != 10)
      abort();
    return;
  default:
    abort();
  }
}

void f1(int i, long long int j, ...) {
  va_list ap;
  va_start(ap, j);
  h(i, ap);
  if (i != 1 || j != 2)
    abort();
  va_end(ap);
}

void f2(int i, int j, int k, long long int l, ...) {
  va_list ap;
  va_start(ap, l);
  h(i, ap);
  if (i != 5 || j != 6 || k != 7 || l != 8)
    abort();
  va_end(ap);
}

int main() {
  f1(1, 2, 3, 4);
  f2(5, 6, 7, 8, 9, 10);
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
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @h(%4 x: i32, %5 ap: va_list) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %17 read<i32>(%4)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %17 const<i32>(1):
// DEFAULT-NEXT:                     let %18: bool [synthetic];
// DEFAULT-NEXT:                     if ne<i32>(va_arg<i32>(%5), const<i32>(3))
// DEFAULT-NEXT:                         write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%18, ne<i32>(va_arg<i32>(%5), const<i32>(4)));
// DEFAULT-NEXT:                     if read<bool>(%18)
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                 return;
// DEFAULT-NEXT:                 case %17 const<i32>(5):
// DEFAULT-NEXT:                     let %19: bool [synthetic];
// DEFAULT-NEXT:                     if ne<i32>(va_arg<i32>(%5), const<i32>(9))
// DEFAULT-NEXT:                         write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%19, ne<i32>(va_arg<i32>(%5), const<i32>(10)));
// DEFAULT-NEXT:                     if read<bool>(%19)
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                 return;
// DEFAULT-NEXT:                 default %17:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f1(%7 i: i32, %8 j: i64, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%9);
// DEFAULT-NEXT:         call<void, signature=fn(i32, va_list) -> void>(%3, read<i32>(%7), read<va_list>(%9));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%7), const<i32>(1)), ne<i64>(read<i64>(%8), widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         va_end(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f2(%11 i: i32, %12 j: i32, %13 k: i32, %14 l: i64, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %15 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%15);
// DEFAULT-NEXT:         call<void, signature=fn(i32, va_list) -> void>(%3, read<i32>(%11), read<va_list>(%15));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%11), const<i32>(5)), ne<i32>(read<i32>(%12), const<i32>(6))), ne<i32>(read<i32>(%13), const<i32>(7))), ne<i64>(read<i64>(%14), widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         va_end(%15);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i64, ...) -> void>(%6, const<i32>(1), widen<i64, reason=arg>(const<i32>(2)), const<i32>(3), const<i32>(4));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i64, ...) -> void>(%10, const<i32>(5), const<i32>(6), const<i32>(7), widen<i64, reason=arg>(const<i32>(8)), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
