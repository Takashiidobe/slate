/* { dg-additional-options "-Wl,-u,_printf_float" { target newlib_nano_io } } */

void abort(void);
void exit(int);

#include <stdarg.h>
#include <stdio.h>

char buf[50];
int  va(int a, double b, int c, ...) {
  va_list ap;
  int     d, e, f, g, h, i, j, k, l, m, n, o, p;
  va_start(ap, c);

  d = va_arg(ap, int);
  e = va_arg(ap, int);
  f = va_arg(ap, int);
  g = va_arg(ap, int);
  h = va_arg(ap, int);
  i = va_arg(ap, int);
  j = va_arg(ap, int);
  k = va_arg(ap, int);
  l = va_arg(ap, int);
  m = va_arg(ap, int);
  n = va_arg(ap, int);
  o = va_arg(ap, int);
  p = va_arg(ap, int);

  sprintf(buf, "%d,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", a, b, c, d, e,
          f, g, h, i, j, k, l, m, n, o, p);
  va_end(ap);
}

int main(void) {
  va(1, 1.0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  if (__builtin_strcmp("1,1.000000,2,3,4,5,6,7,8,9,10,11,12,13,14,15", buf))
    abort();
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
// DEFAULT-NEXT:     type @type2 va_list = va_list;
// DEFAULT-NEXT:     global %5 buf: array<i8, 50> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %28 .str28: array<i8, 48> [storage=static] = code_units<array<i8, 48>>([37, 100, 44, 37, 102, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 44, 37, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 45> [storage=static] = code_units<array<i8, 45>>([49, 44, 49, 46, 48, 48, 48, 48, 48, 48, 44, 50, 44, 51, 44, 52, 44, 53, 44, 54, 44, 55, 44, 56, 44, 57, 44, 49, 48, 44, 49, 49, 44, 49, 50, 44, 49, 51, 44, 49, 52, 44, 49, 53, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%25 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @sprintf(%26 __s: ptr<i8> [restrict], %27 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @va(%7 a: i32, %8 b: f64, %9 c: i32, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %11 d: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12 e: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 f: i32 [storage=automatic];
// DEFAULT-NEXT:         let %14 g: i32 [storage=automatic];
// DEFAULT-NEXT:         let %15 h: i32 [storage=automatic];
// DEFAULT-NEXT:         let %16 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %17 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %18 k: i32 [storage=automatic];
// DEFAULT-NEXT:         let %19 l: i32 [storage=automatic];
// DEFAULT-NEXT:         let %20 m: i32 [storage=automatic];
// DEFAULT-NEXT:         let %21 n: i32 [storage=automatic];
// DEFAULT-NEXT:         let %22 o: i32 [storage=automatic];
// DEFAULT-NEXT:         let %23 p: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%10);
// DEFAULT-NEXT:         write<i32>(%11, va_arg<i32>(%10));
// DEFAULT-NEXT:         va_arg<i32>(%10);
// DEFAULT-NEXT:         write<i32>(%12, va_arg<i32>(%10));
// DEFAULT-NEXT:         va_arg<i32>(%10);
// DEFAULT-NEXT:         write<i32>(%13, va_arg<i32>(%10));
// DEFAULT-NEXT:         va_arg<i32>(%10);
// DEFAULT-NEXT:         write<i32>(%14, va_arg<i32>(%10));
// DEFAULT-NEXT:         va_arg<i32>(%10);
// DEFAULT-NEXT:         write<i32>(%15, va_arg<i32>(%10));
// DEFAULT-NEXT:         va_arg<i32>(%10);
// DEFAULT-NEXT:         write<i32>(%16, va_arg<i32>(%10));
// DEFAULT-NEXT:         va_arg<i32>(%10);
// DEFAULT-NEXT:         write<i32>(%17, va_arg<i32>(%10));
// DEFAULT-NEXT:         va_arg<i32>(%10);
// DEFAULT-NEXT:         write<i32>(%18, va_arg<i32>(%10));
// DEFAULT-NEXT:         va_arg<i32>(%10);
// DEFAULT-NEXT:         write<i32>(%19, va_arg<i32>(%10));
// DEFAULT-NEXT:         va_arg<i32>(%10);
// DEFAULT-NEXT:         write<i32>(%20, va_arg<i32>(%10));
// DEFAULT-NEXT:         va_arg<i32>(%10);
// DEFAULT-NEXT:         write<i32>(%21, va_arg<i32>(%10));
// DEFAULT-NEXT:         va_arg<i32>(%10);
// DEFAULT-NEXT:         write<i32>(%22, va_arg<i32>(%10));
// DEFAULT-NEXT:         va_arg<i32>(%10);
// DEFAULT-NEXT:         write<i32>(%23, va_arg<i32>(%10));
// DEFAULT-NEXT:         va_arg<i32>(%10);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, ptr<const i8>, ...) -> i32>(%4, array_decay<ptr<i8>, length=Some(50)>(%5), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(48)>(%28)), read<i32>(%7), read<f64>(%8), read<i32>(%9), read<i32>(%11), read<i32>(%12), read<i32>(%13), read<i32>(%14), read<i32>(%15), read<i32>(%16), read<i32>(%17), read<i32>(%18), read<i32>(%19), read<i32>(%20), read<i32>(%21), read<i32>(%22), read<i32>(%23));
// DEFAULT-NEXT:         va_end(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @__builtin_strcmp(%29 <unnamed>: ptr<const i8>, %30 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %24 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32, f64, i32, ...) -> i32>(%6, const<i32>(1), const<f64>(1.0), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%31, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(45)>(%32)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(50)>(%5))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
