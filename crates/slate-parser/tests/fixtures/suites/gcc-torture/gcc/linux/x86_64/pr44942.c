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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     fn %59 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @test1(%3 a: i32, %4 b: i32, %5 c: i32, %6 d: i32, %7 e: i32, %8 f: i32, %9 g: i32, %10 h: f80, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%12);
// DEFAULT-NEXT:         write<i32>(%11, va_arg<i32>(%12));
// DEFAULT-NEXT:         va_arg<i32>(%12);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%11), const<i32>(1234))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%59);
// DEFAULT-NEXT:         va_end(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test2(%14 a: i32, %15 b: i32, %16 c: i32, %17 d: i32, %18 e: i32, %19 f: i32, %20 g: i32, %21 h: f80, %22 i: i32, %23 j: f80, %24 k: i32, %25 l: f80, %26 m: i32, %27 n: f80, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %28 o: i32 [storage=automatic];
// DEFAULT-NEXT:         let %29 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%29);
// DEFAULT-NEXT:         write<i32>(%28, va_arg<i32>(%29));
// DEFAULT-NEXT:         va_arg<i32>(%29);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%28), const<i32>(1234))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%59);
// DEFAULT-NEXT:         va_end(%29);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @test3(%31 a: f64, %32 b: f64, %33 c: f64, %34 d: f64, %35 e: f64, %36 f: f64, %37 g: f64, %38 h: f80, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %39 i: f64 [storage=automatic];
// DEFAULT-NEXT:         let %40 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%40);
// DEFAULT-NEXT:         write<f64>(%39, va_arg<f64>(%40));
// DEFAULT-NEXT:         va_arg<f64>(%40);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%39), const<f64>(1234.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%59);
// DEFAULT-NEXT:         va_end(%40);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @test4(%42 a: f64, %43 b: f64, %44 c: f64, %45 d: f64, %46 e: f64, %47 f: f64, %48 g: f64, %49 h: f80, %50 i: f64, %51 j: f80, %52 k: f64, %53 l: f80, %54 m: f64, %55 n: f80, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %56 o: f64 [storage=automatic];
// DEFAULT-NEXT:         let %57 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%57);
// DEFAULT-NEXT:         write<f64>(%56, va_arg<f64>(%57));
// DEFAULT-NEXT:         va_arg<f64>(%57);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%56), const<f64>(1234.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%59);
// DEFAULT-NEXT:         va_end(%57);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %58 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, f80, ...) -> void>(%2, const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<f80>(0), const<i32>(1234));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, f80, i32, f80, i32, f80, i32, f80, ...) -> void>(%13, const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<f80>(0), const<i32>(0), const<f80>(0), const<i32>(0), const<f80>(0), const<i32>(0), const<f80>(0), const<i32>(1234));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64, f64, f64, f64, f64, f64, f80, ...) -> void>(%30, const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f80>(0), const<f64>(1234.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64, f64, f64, f64, f64, f64, f80, f64, f80, f64, f80, f64, f80, ...) -> void>(%41, const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f80>(0), const<f64>(0.0), const<f80>(0), const<f64>(0.0), const<f80>(0), const<f64>(0.0), const<f80>(0), const<f64>(1234.0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
