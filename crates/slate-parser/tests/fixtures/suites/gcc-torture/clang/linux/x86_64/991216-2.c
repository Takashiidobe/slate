#include <stdarg.h>

void abort(void);
void exit(int);

#define VALUE 0x123456789abcdefLL
#define AFTER 0x55

void test(int n, ...) {
  va_list ap;
  int     i;

  va_start(ap, n);
  for (i = 2; i <= n; i++) {
    if (va_arg(ap, int) != i)
      abort();
  }

  if (va_arg(ap, long long) != VALUE)
    abort();

  if (va_arg(ap, int) != AFTER)
    abort();

  va_end(ap);
}

int main() {
  test(1, VALUE, AFTER);
  test(2, 2, VALUE, AFTER);
  test(3, 2, 3, VALUE, AFTER);
  test(4, 2, 3, 4, VALUE, AFTER);
  test(5, 2, 3, 4, 5, VALUE, AFTER);
  test(6, 2, 3, 4, 5, 6, VALUE, AFTER);
  test(7, 2, 3, 4, 5, 6, 7, VALUE, AFTER);
  test(8, 2, 3, 4, 5, 6, 7, 8, VALUE, AFTER);
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
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @exit(%8 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @test(%4 n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %6 i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%5);
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%6, const<i32>(2));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%6), read<i32>(%4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%11));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(va_arg<i32>(%5), read<i32>(%6))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i64>(va_arg<i64>(%5), const<i64>(81985529216486895))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%5), const<i32>(85))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         va_end(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%3, const<i32>(1), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%3, const<i32>(2), const<i32>(2), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%3, const<i32>(3), const<i32>(2), const<i32>(3), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%3, const<i32>(4), const<i32>(2), const<i32>(3), const<i32>(4), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%3, const<i32>(5), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%3, const<i32>(6), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%3, const<i32>(7), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%3, const<i32>(8), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i64>(81985529216486895), const<i32>(85));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
