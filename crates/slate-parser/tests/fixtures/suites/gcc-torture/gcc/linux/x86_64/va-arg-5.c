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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @exit(%11 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @va_double(%5 n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 args: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%6);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(va_arg<f64>(%6), const<f64>(3.141592))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(va_arg<f64>(%6), const<f64>(2.71827))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(va_arg<f64>(%6), const<f64>(2.2360679))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(va_arg<f64>(%6), const<f64>(2.1474836))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         va_end(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @va_long_double(%8 n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 args: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%9);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(va_arg<f80>(%9), const<f80>(3.14159199999999999998))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(va_arg<f80>(%9), const<f80>(2.71827000000000000004))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(va_arg<f80>(%9), const<f80>(2.2360679))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(va_arg<f80>(%9), const<f80>(2.14748360000000000007))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         va_end(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%4, const<i32>(4), const<f64>(3.141592), const<f64>(2.71827), const<f64>(2.2360679), const<f64>(2.1474836));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%7, const<i32>(4), const<f80>(3.14159199999999999998), const<f80>(2.71827000000000000004), const<f80>(2.2360679), const<f80>(2.14748360000000000007));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
