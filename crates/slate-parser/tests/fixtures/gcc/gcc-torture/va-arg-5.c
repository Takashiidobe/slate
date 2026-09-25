#include <stdarg.h>

void abort(void);
void exit(int);

void va_double(int n, ...) {
  va_list args;

  va_start(args, n);

  if (va_arg(args, double) != 3.141592)
    abort();
  if (va_arg(args, double) != 2.71827)
    abort();
  if (va_arg(args, double) != 2.2360679)
    abort();
  if (va_arg(args, double) != 2.1474836)
    abort();

  va_end(args);
}

void va_long_double(int n, ...) {
  va_list args;

  va_start(args, n);

  if (va_arg(args, long double) != 3.141592L)
    abort();
  if (va_arg(args, long double) != 2.71827L)
    abort();
  if (va_arg(args, long double) != 2.2360679L)
    abort();
  if (va_arg(args, long double) != 2.1474836L)
    abort();

  va_end(args);
}

int main(void) {
  va_double(4, 3.141592, 2.71827, 2.2360679, 2.1474836);
  va_long_double(4, 3.141592L, 2.71827L, 2.2360679L, 2.1474836L);
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
// DEFAULT-NEXT:     type @type0 va_list = va_list;
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @exit(%10 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @va_double(%4 n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 args: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%5);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(va_arg<f64>(%5), const<f64>(3.141592))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(va_arg<f64>(%5), const<f64>(2.71827))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(va_arg<f64>(%5), const<f64>(2.2360679))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(va_arg<f64>(%5), const<f64>(2.1474836))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         va_end(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @va_long_double(%7 n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 args: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%8);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(va_arg<f80>(%8), const<f80>(3.14159199999999999998))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(va_arg<f80>(%8), const<f80>(2.71827000000000000004))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(va_arg<f80>(%8), const<f80>(2.2360679))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(va_arg<f80>(%8), const<f80>(2.14748360000000000007))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         va_end(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%3, const<i32>(4), const<f64>(3.141592), const<f64>(2.71827), const<f64>(2.2360679), const<f64>(2.1474836));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%6, const<i32>(4), const<f80>(3.14159199999999999998), const<f80>(2.71827000000000000004), const<f80>(2.2360679), const<f80>(2.14748360000000000007));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
