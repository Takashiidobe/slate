#include <stdarg.h>

void abort(void);
void exit(int);

void fdouble(double one, ...) {
  double  value;
  va_list ap;

  va_start(ap, one);
  value = va_arg(ap, double);
  va_end(ap);

  if (one != 1.0 || value != 2.0)
    abort();
}

int main() {
  fdouble(1.0, 2.0);
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
// DEFAULT-NEXT:     fn %2 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @fdouble(%4 one: f64, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 value: f64 [storage=automatic];
// DEFAULT-NEXT:         let %6 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%6);
// DEFAULT-NEXT:         write<f64>(%5, va_arg<f64>(%6));
// DEFAULT-NEXT:         va_arg<f64>(%6);
// DEFAULT-NEXT:         va_end(%6);
// DEFAULT-NEXT:         if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(%4), const<f64>(1.0)), ne<f64, exceptions=ignore>(read<f64>(%5), const<f64>(2.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64, ...) -> void>(%3, const<f64>(1.0), const<f64>(2.0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
