#include <stdarg.h>

void abort(void);
void exit(int);

void foo(va_list v) {
  unsigned long long x = va_arg(v, unsigned long long);
  if (x != 16LL)
    abort();
}

void bar(char c, char d, ...) {
  va_list v;
  va_start(v, d);
  foo(v);
  va_end(v);
}

int main(void) {
  bar(0, 0, 16LL);
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
// DEFAULT-NEXT:     fn %3 @exit(%12 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @foo(%5 v: va_list) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 x: u64 [storage=automatic] = va_arg<u64>(%5);
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%6), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(16)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @bar(%8 c: i8, %9 d: i8, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10 v: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%10);
// DEFAULT-NEXT:         call<void, signature=fn(va_list) -> void>(%4, read<va_list>(%10));
// DEFAULT-NEXT:         va_end(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i8, i8, ...) -> void>(%7, truncate<i8, reason=arg, fits=always>(const<i32>(0)), truncate<i8, reason=arg, fits=always>(const<i32>(0)), const<i64>(16));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
