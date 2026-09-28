#include <stdarg.h>

void abort(void);
void exit(int);

va_list global;

void vat(va_list param, ...) {
  va_list local;

  va_start(local, param);
  va_copy(global, local);
  va_copy(param, local);
  if (va_arg(local, int) != 1)
    abort();
  va_end(local);
  if (va_arg(global, int) != 1)
    abort();
  va_end(global);
  if (va_arg(param, int) != 1)
    abort();
  va_end(param);

  va_start(param, param);
  va_start(global, param);
  va_copy(local, param);
  if (va_arg(local, int) != 1)
    abort();
  va_end(local);
  va_copy(local, global);
  if (va_arg(local, int) != 1)
    abort();
  va_end(local);
  if (va_arg(global, int) != 1)
    abort();
  va_end(global);
  if (va_arg(param, int) != 1)
    abort();
  va_end(param);
}

int main(void) {
  va_list t;
  vat(t, 1);
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
// DEFAULT-NEXT:     global %3 global: va_list [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @exit(%9 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @vat(%5 param: va_list, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 local: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%6);
// DEFAULT-NEXT:         va_copy(%3, %6);
// DEFAULT-NEXT:         va_copy(%5, %6);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%6), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         va_end(%6);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%3), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         va_end(%3);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%5), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         va_end(%5);
// DEFAULT-NEXT:         va_start(%5);
// DEFAULT-NEXT:         va_start(%3);
// DEFAULT-NEXT:         va_copy(%6, %5);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%6), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         va_end(%6);
// DEFAULT-NEXT:         va_copy(%6, %3);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%6), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         va_end(%6);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%3), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         va_end(%3);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%5), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         va_end(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 t: va_list [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(va_list, ...) -> void>(%4, read<va_list>(%8), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
