#include <stdarg.h>

void abort(void);
void exit(int);

typedef double L;
void           f(L p0, L p1, L p2, L p3, L p4, L p5, L p6, L p7, L p8, ...) {
  va_list select;

  va_start(select, p8);

  if (va_arg(select, int) != 10)
    abort();
  if (va_arg(select, int) != 11)
    abort();
  if (va_arg(select, int) != 12)
    abort();

  va_end(select);
}

int main() {
  f(1., 2., 3., 4., 5., 6., 7., 8., 9., 10, 11, 12);
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
// DEFAULT-NEXT:     type @type1 L = f64;
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @exit(%16 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @f(%5 p0: f64, %6 p1: f64, %7 p2: f64, %8 p3: f64, %9 p4: f64, %10 p5: f64, %11 p6: f64, %12 p7: f64, %13 p8: f64, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %14 select: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%14);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%14), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%14), const<i32>(11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%14), const<i32>(12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         va_end(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64, f64, f64, f64, f64, f64, f64, f64, ...) -> void>(%4, const<f64>(1.0), const<f64>(2.0), const<f64>(3.0), const<f64>(4.0), const<f64>(5.0), const<f64>(6.0), const<f64>(7.0), const<f64>(8.0), const<f64>(9.0), const<i32>(10), const<i32>(11), const<i32>(12));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
