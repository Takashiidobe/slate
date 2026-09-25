#include <stdarg.h>

void abort(void);
void exit(int);

typedef double TYPE;

void vafunction(char *dummy, ...) {
  va_list ap;

  va_start(ap, dummy);
  if (va_arg(ap, TYPE) != 1.)
    abort();
  if (va_arg(ap, TYPE) != 2.)
    abort();
  if (va_arg(ap, TYPE) != 3.)
    abort();
  if (va_arg(ap, TYPE) != 4.)
    abort();
  if (va_arg(ap, TYPE) != 5.)
    abort();
  if (va_arg(ap, TYPE) != 6.)
    abort();
  if (va_arg(ap, TYPE) != 7.)
    abort();
  if (va_arg(ap, TYPE) != 8.)
    abort();
  if (va_arg(ap, TYPE) != 9.)
    abort();
  va_end(ap);
}

int main(void) {
  vafunction("", 1., 2., 3., 4., 5., 6., 7., 8., 9.);
  exit(0);
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
// DEFAULT-NEXT:     type @type0 va_list = va_list;
// DEFAULT-NEXT:     type @type1 TYPE = f64;
// DEFAULT-NEXT:     global %9 .str9: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @vafunction(%5 dummy: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%6);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(va_arg<f64>(%6), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(va_arg<f64>(%6), const<f64>(2.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(va_arg<f64>(%6), const<f64>(3.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(va_arg<f64>(%6), const<f64>(4.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(va_arg<f64>(%6), const<f64>(5.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(va_arg<f64>(%6), const<f64>(6.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(va_arg<f64>(%6), const<f64>(7.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(va_arg<f64>(%6), const<f64>(8.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(va_arg<f64>(%6), const<f64>(9.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         va_end(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ...) -> void>(%4, array_decay<ptr<i8>, length=Some(1)>(%9), const<f64>(1.0), const<f64>(2.0), const<f64>(3.0), const<f64>(4.0), const<f64>(5.0), const<f64>(6.0), const<f64>(7.0), const<f64>(8.0), const<f64>(9.0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
