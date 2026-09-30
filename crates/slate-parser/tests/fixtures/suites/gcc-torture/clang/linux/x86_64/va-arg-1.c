#include <stdarg.h>

void abort(void);
void exit(int);

typedef unsigned long L;
void f(L p0, L p1, L p2, L p3, L p4, L p5, L p6, L p7, L p8, ...) {
  va_list select;

  va_start(select, p8);

  if (va_arg(select, L) != 10)
    abort();
  if (va_arg(select, L) != 11)
    abort();
  if (va_arg(select, L) != 0)
    abort();

  va_end(select);
}

int main(void) {
  f(1L, 2L, 3L, 4L, 5L, 6L, 7L, 8L, 9L, 10L, 11L, 0L);
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
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_L:[0-9]+]] L = u64;
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_p0:[0-9]+]] p0: u64, %[[VALUE_p1:[0-9]+]] p1: u64, %[[VALUE_p2:[0-9]+]] p2: u64, %[[VALUE_p3:[0-9]+]] p3: u64, %[[VALUE_p4:[0-9]+]] p4: u64, %[[VALUE_p5:[0-9]+]] p5: u64, %[[VALUE_p6:[0-9]+]] p6: u64, %[[VALUE_p7:[0-9]+]] p7: u64, %[[VALUE_p8:[0-9]+]] p8: u64, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_select:[0-9]+]] select: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_select]]);
// DEFAULT-NEXT:         if ne<u64>(va_arg<u64>(%[[VALUE_select]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(va_arg<u64>(%[[VALUE_select]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(11))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(va_arg<u64>(%[[VALUE_select]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_select]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(u64, u64, u64, u64, u64, u64, u64, u64, u64, ...) -> void>(%[[VALUE_f]], reinterpret<u64, reason=arg, fits=always>(const<i64>(1)), reinterpret<u64, reason=arg, fits=always>(const<i64>(2)), reinterpret<u64, reason=arg, fits=always>(const<i64>(3)), reinterpret<u64, reason=arg, fits=always>(const<i64>(4)), reinterpret<u64, reason=arg, fits=always>(const<i64>(5)), reinterpret<u64, reason=arg, fits=always>(const<i64>(6)), reinterpret<u64, reason=arg, fits=always>(const<i64>(7)), reinterpret<u64, reason=arg, fits=always>(const<i64>(8)), reinterpret<u64, reason=arg, fits=always>(const<i64>(9)), const<i64>(10), const<i64>(11), const<i64>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
