/* PR target/44942 */

#include <stdarg.h>

void test1(int a, int b, int c, int d, int e, int f, int g, long double h,
           ...) {
  int     i;
  va_list ap;

  va_start(ap, h);
  i = va_arg(ap, int);
  if (i != 1234)
    __builtin_abort();
  va_end(ap);
}

void test2(int a, int b, int c, int d, int e, int f, int g, long double h,
           int i, long double j, int k, long double l, int m, long double n,
           ...) {
  int     o;
  va_list ap;

  va_start(ap, n);
  o = va_arg(ap, int);
  if (o != 1234)
    __builtin_abort();
  va_end(ap);
}

void test3(double a, double b, double c, double d, double e, double f, double g,
           long double h, ...) {
  double  i;
  va_list ap;

  va_start(ap, h);
  i = va_arg(ap, double);
  if (i != 1234.0)
    __builtin_abort();
  va_end(ap);
}

void test4(double a, double b, double c, double d, double e, double f, double g,
           long double h, double i, long double j, double k, long double l,
           double m, long double n, ...) {
  double  o;
  va_list ap;

  va_start(ap, n);
  o = va_arg(ap, double);
  if (o != 1234.0)
    __builtin_abort();
  va_end(ap);
}

int main() {
  test1(0, 0, 0, 0, 0, 0, 0, 0.0L, 1234);
  test2(0, 0, 0, 0, 0, 0, 0, 0.0L, 0, 0.0L, 0, 0.0L, 0, 0.0L, 1234);
  test3(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0L, 1234.0);
  test4(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0L, 0.0, 0.0L, 0.0, 0.0L, 0.0,
        0.0L, 1234.0);
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
// DEFAULT-NEXT:     fn %1 @test1(%2 a: i32, %3 b: i32, %4 c: i32, %5 d: i32, %6 e: i32, %7 f: i32, %8 g: i32, %9 h: f80, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %11 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%11);
// DEFAULT-NEXT:         write<i32>(%10, va_arg<i32>(%11));
// DEFAULT-NEXT:         va_arg<i32>(%11);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(1234))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         va_end(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test2(%13 a: i32, %14 b: i32, %15 c: i32, %16 d: i32, %17 e: i32, %18 f: i32, %19 g: i32, %20 h: f80, %21 i: i32, %22 j: f80, %23 k: i32, %24 l: f80, %25 m: i32, %26 n: f80, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %27 o: i32 [storage=automatic];
// DEFAULT-NEXT:         let %28 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%28);
// DEFAULT-NEXT:         write<i32>(%27, va_arg<i32>(%28));
// DEFAULT-NEXT:         va_arg<i32>(%28);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%27), const<i32>(1234))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         va_end(%28);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @test3(%30 a: f64, %31 b: f64, %32 c: f64, %33 d: f64, %34 e: f64, %35 f: f64, %36 g: f64, %37 h: f80, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %38 i: f64 [storage=automatic];
// DEFAULT-NEXT:         let %39 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%39);
// DEFAULT-NEXT:         write<f64>(%38, va_arg<f64>(%39));
// DEFAULT-NEXT:         va_arg<f64>(%39);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(%38), const<f64>(1234.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         va_end(%39);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @test4(%41 a: f64, %42 b: f64, %43 c: f64, %44 d: f64, %45 e: f64, %46 f: f64, %47 g: f64, %48 h: f80, %49 i: f64, %50 j: f80, %51 k: f64, %52 l: f80, %53 m: f64, %54 n: f80, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %55 o: f64 [storage=automatic];
// DEFAULT-NEXT:         let %56 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%56);
// DEFAULT-NEXT:         write<f64>(%55, va_arg<f64>(%56));
// DEFAULT-NEXT:         va_arg<f64>(%56);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(%55), const<f64>(1234.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         va_end(%56);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, f80, ...) -> void>(%1, const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<f80>(0), const<i32>(1234));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, f80, i32, f80, i32, f80, i32, f80, ...) -> void>(%12, const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<f80>(0), const<i32>(0), const<f80>(0), const<i32>(0), const<f80>(0), const<i32>(0), const<f80>(0), const<i32>(1234));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64, f64, f64, f64, f64, f64, f80, ...) -> void>(%29, const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f80>(0), const<f64>(1234.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64, f64, f64, f64, f64, f64, f80, f64, f80, f64, f80, f64, f80, ...) -> void>(%40, const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f80>(0), const<f64>(0.0), const<f80>(0), const<f64>(0.0), const<f80>(0), const<f64>(0.0), const<f80>(0), const<f64>(1234.0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
