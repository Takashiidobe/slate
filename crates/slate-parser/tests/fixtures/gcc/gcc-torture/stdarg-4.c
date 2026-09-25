#include <stdarg.h>

extern void abort(void);
long        x, y;

inline void __attribute__((always_inline)) f1i(va_list ap) {
  x  = va_arg(ap, double);
  x += va_arg(ap, long);
  x += va_arg(ap, double);
}

void f1(int i, ...) {
  va_list ap;
  va_start(ap, i);
  f1i(ap);
  va_end(ap);
}

inline void __attribute__((always_inline)) f2i(va_list ap) {
  y  = va_arg(ap, int);
  y += va_arg(ap, long);
  y += va_arg(ap, double);
  f1i(ap);
}

void f2(int i, ...) {
  va_list ap;
  va_start(ap, i);
  f2i(ap);
  va_end(ap);
}

long f3h(int i, long arg0, long arg1, long arg2, long arg3) {
  return i + arg0 + arg1 + arg2 + arg3;
}

long f3(int i, ...) {
  long    t, arg0, arg1, arg2, arg3;
  va_list ap;

  va_start(ap, i);
  switch (i) {
  case 0:
    t = f3h(i, 0, 0, 0, 0);
    break;
  case 1:
    arg0 = va_arg(ap, long);
    t    = f3h(i, arg0, 0, 0, 0);
    break;
  case 2:
    arg0 = va_arg(ap, long);
    arg1 = va_arg(ap, long);
    t    = f3h(i, arg0, arg1, 0, 0);
    break;
  case 3:
    arg0 = va_arg(ap, long);
    arg1 = va_arg(ap, long);
    arg2 = va_arg(ap, long);
    t    = f3h(i, arg0, arg1, arg2, 0);
    break;
  case 4:
    arg0 = va_arg(ap, long);
    arg1 = va_arg(ap, long);
    arg2 = va_arg(ap, long);
    arg3 = va_arg(ap, long);
    t    = f3h(i, arg0, arg1, arg2, arg3);
    break;
  default:
    abort();
  }
  va_end(ap);

  return t;
}

void f4(int i, ...) {
  va_list ap;

  va_start(ap, i);
  switch (i) {
  case 4:
    y = va_arg(ap, double);
    break;
  case 5:
    y  = va_arg(ap, double);
    y += va_arg(ap, double);
    break;
  default:
    abort();
  }
  f1i(ap);
  va_end(ap);
}

int main(void) {
  f1(3, 16.0, 128L, 32.0);
  if (x != 176L)
    abort();
  f2(6, 5, 7L, 18.0, 19.0, 17L, 64.0);
  if (x != 100L || y != 30L)
    abort();
  if (f3(0) != 0)
    abort();
  if (f3(1, 18L) != 19L)
    abort();
  if (f3(2, 18L, 100L) != 120L)
    abort();
  if (f3(3, 18L, 100L, 300L) != 421L)
    abort();
  if (f3(4, 18L, 71L, 64L, 86L) != 243L)
    abort();
  f4(4, 6.0, 9.0, 16L, 18.0);
  if (x != 43L || y != 6L)
    abort();
  f4(5, 7.0, 21.0, 1.0, 17L, 126.0);
  if (x != 144L || y != 28L)
    abort();
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
// DEFAULT-NEXT:     global %2 x: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 y: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @f1i(%5 ap: va_list) -> void [linkage=external] [inline=always] [definition=inline_only] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%2, float_to_int<i64, reason=assign, out_of_range=ub, exceptions=ignore>(va_arg<f64>(%5)));
// DEFAULT-NEXT:         float_to_int<i64, reason=assign, out_of_range=ub, exceptions=ignore>(va_arg<f64>(%5));
// DEFAULT-NEXT:         let %34: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %35: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%34), va_arg<i64>(%5));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%35));
// DEFAULT-NEXT:         let %36: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %37: i64 [synthetic] = float_to_int<i64, reason=assign, out_of_range=ub, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i64>(%36)), va_arg<f64>(%5)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%37));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f1(%7 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%8);
// DEFAULT-NEXT:         call<void, signature=fn(va_list) -> void>(%4, read<va_list>(%8));
// DEFAULT-NEXT:         va_end(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @f2i(%10 ap: va_list) -> void [linkage=external] [inline=always] [definition=inline_only] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%3, widen<i64, reason=assign>(va_arg<i32>(%10)));
// DEFAULT-NEXT:         widen<i64, reason=assign>(va_arg<i32>(%10));
// DEFAULT-NEXT:         let %38: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %39: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%38), va_arg<i64>(%10));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%39));
// DEFAULT-NEXT:         let %40: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %41: i64 [synthetic] = float_to_int<i64, reason=assign, out_of_range=ub, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i64>(%40)), va_arg<f64>(%10)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%41));
// DEFAULT-NEXT:         call<void, signature=fn(va_list) -> void>(%4, read<va_list>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @f2(%12 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%13);
// DEFAULT-NEXT:         call<void, signature=fn(va_list) -> void>(%9, read<va_list>(%13));
// DEFAULT-NEXT:         va_end(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @f3h(%15 i: i32, %16 arg0: i64, %17 arg1: i64, %18 arg2: i64, %19 arg3: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%15)), read<i64>(%16)), read<i64>(%17)), read<i64>(%18)), read<i64>(%19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @f3(%21 i: i32, ...) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %22 t: i64 [storage=automatic];
// DEFAULT-NEXT:         let %23 arg0: i64 [storage=automatic];
// DEFAULT-NEXT:         let %24 arg1: i64 [storage=automatic];
// DEFAULT-NEXT:         let %25 arg2: i64 [storage=automatic];
// DEFAULT-NEXT:         let %26 arg3: i64 [storage=automatic];
// DEFAULT-NEXT:         let %27 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%27);
// DEFAULT-NEXT:         switch %32 read<i32>(%21)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %32 const<i32>(0):
// DEFAULT-NEXT:                     write<i64>(%22, call<i64, signature=fn(i32, i64, i64, i64, i64) -> i64>(%14, read<i32>(%21), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:                     call<i64, signature=fn(i32, i64, i64, i64, i64) -> i64>(%14, read<i32>(%21), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:                 break %32;
// DEFAULT-NEXT:                 case %32 const<i32>(1):
// DEFAULT-NEXT:                     write<i64>(%23, va_arg<i64>(%27));
// DEFAULT-NEXT:                     va_arg<i64>(%27);
// DEFAULT-NEXT:                 write<i64>(%22, call<i64, signature=fn(i32, i64, i64, i64, i64) -> i64>(%14, read<i32>(%21), read<i64>(%23), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:                 call<i64, signature=fn(i32, i64, i64, i64, i64) -> i64>(%14, read<i32>(%21), read<i64>(%23), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:                 break %32;
// DEFAULT-NEXT:                 case %32 const<i32>(2):
// DEFAULT-NEXT:                     write<i64>(%23, va_arg<i64>(%27));
// DEFAULT-NEXT:                     va_arg<i64>(%27);
// DEFAULT-NEXT:                 write<i64>(%24, va_arg<i64>(%27));
// DEFAULT-NEXT:                 va_arg<i64>(%27);
// DEFAULT-NEXT:                 write<i64>(%22, call<i64, signature=fn(i32, i64, i64, i64, i64) -> i64>(%14, read<i32>(%21), read<i64>(%23), read<i64>(%24), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:                 call<i64, signature=fn(i32, i64, i64, i64, i64) -> i64>(%14, read<i32>(%21), read<i64>(%23), read<i64>(%24), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:                 break %32;
// DEFAULT-NEXT:                 case %32 const<i32>(3):
// DEFAULT-NEXT:                     write<i64>(%23, va_arg<i64>(%27));
// DEFAULT-NEXT:                     va_arg<i64>(%27);
// DEFAULT-NEXT:                 write<i64>(%24, va_arg<i64>(%27));
// DEFAULT-NEXT:                 va_arg<i64>(%27);
// DEFAULT-NEXT:                 write<i64>(%25, va_arg<i64>(%27));
// DEFAULT-NEXT:                 va_arg<i64>(%27);
// DEFAULT-NEXT:                 write<i64>(%22, call<i64, signature=fn(i32, i64, i64, i64, i64) -> i64>(%14, read<i32>(%21), read<i64>(%23), read<i64>(%24), read<i64>(%25), widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:                 call<i64, signature=fn(i32, i64, i64, i64, i64) -> i64>(%14, read<i32>(%21), read<i64>(%23), read<i64>(%24), read<i64>(%25), widen<i64, reason=arg>(const<i32>(0)));
// DEFAULT-NEXT:                 break %32;
// DEFAULT-NEXT:                 case %32 const<i32>(4):
// DEFAULT-NEXT:                     write<i64>(%23, va_arg<i64>(%27));
// DEFAULT-NEXT:                     va_arg<i64>(%27);
// DEFAULT-NEXT:                 write<i64>(%24, va_arg<i64>(%27));
// DEFAULT-NEXT:                 va_arg<i64>(%27);
// DEFAULT-NEXT:                 write<i64>(%25, va_arg<i64>(%27));
// DEFAULT-NEXT:                 va_arg<i64>(%27);
// DEFAULT-NEXT:                 write<i64>(%26, va_arg<i64>(%27));
// DEFAULT-NEXT:                 va_arg<i64>(%27);
// DEFAULT-NEXT:                 write<i64>(%22, call<i64, signature=fn(i32, i64, i64, i64, i64) -> i64>(%14, read<i32>(%21), read<i64>(%23), read<i64>(%24), read<i64>(%25), read<i64>(%26)));
// DEFAULT-NEXT:                 call<i64, signature=fn(i32, i64, i64, i64, i64) -> i64>(%14, read<i32>(%21), read<i64>(%23), read<i64>(%24), read<i64>(%25), read<i64>(%26));
// DEFAULT-NEXT:                 break %32;
// DEFAULT-NEXT:                 default %32:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         va_end(%27);
// DEFAULT-NEXT:         return read<i64>(%22);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @f4(%29 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %30 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%30);
// DEFAULT-NEXT:         switch %33 read<i32>(%29)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %33 const<i32>(4):
// DEFAULT-NEXT:                     write<i64>(%3, float_to_int<i64, reason=assign, out_of_range=ub, exceptions=ignore>(va_arg<f64>(%30)));
// DEFAULT-NEXT:                     float_to_int<i64, reason=assign, out_of_range=ub, exceptions=ignore>(va_arg<f64>(%30));
// DEFAULT-NEXT:                 break %33;
// DEFAULT-NEXT:                 case %33 const<i32>(5):
// DEFAULT-NEXT:                     write<i64>(%3, float_to_int<i64, reason=assign, out_of_range=ub, exceptions=ignore>(va_arg<f64>(%30)));
// DEFAULT-NEXT:                     float_to_int<i64, reason=assign, out_of_range=ub, exceptions=ignore>(va_arg<f64>(%30));
// DEFAULT-NEXT:                 let %42: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:                 let %43: i64 [synthetic] = float_to_int<i64, reason=assign, out_of_range=ub, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i64>(%42)), va_arg<f64>(%30)));
// DEFAULT-NEXT:                 write<i64>(%3, read<i64>(%43));
// DEFAULT-NEXT:                 break %33;
// DEFAULT-NEXT:                 default %33:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(va_list) -> void>(%4, read<va_list>(%30));
// DEFAULT-NEXT:         va_end(%30);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%6, const<i32>(3), const<f64>(16.0), const<i64>(128), const<f64>(32.0));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%2), const<i64>(176))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%11, const<i32>(6), const<i32>(5), const<i64>(7), const<f64>(18.0), const<f64>(19.0), const<i64>(17), const<f64>(64.0));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(%2), const<i64>(100)), ne<i64>(read<i64>(%3), const<i64>(30)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i32, ...) -> i64>(%20, const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i32, ...) -> i64>(%20, const<i32>(1), const<i64>(18)), const<i64>(19))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i32, ...) -> i64>(%20, const<i32>(2), const<i64>(18), const<i64>(100)), const<i64>(120))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i32, ...) -> i64>(%20, const<i32>(3), const<i64>(18), const<i64>(100), const<i64>(300)), const<i64>(421))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i32, ...) -> i64>(%20, const<i32>(4), const<i64>(18), const<i64>(71), const<i64>(64), const<i64>(86)), const<i64>(243))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%28, const<i32>(4), const<f64>(6.0), const<f64>(9.0), const<i64>(16), const<f64>(18.0));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(%2), const<i64>(43)), ne<i64>(read<i64>(%3), const<i64>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%28, const<i32>(5), const<f64>(7.0), const<f64>(21.0), const<f64>(1.0), const<i64>(17), const<f64>(126.0));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(%2), const<i64>(144)), ne<i64>(read<i64>(%3), const<i64>(28)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
