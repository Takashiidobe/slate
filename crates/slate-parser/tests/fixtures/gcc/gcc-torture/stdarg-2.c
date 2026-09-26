#include <stdarg.h>

extern void abort(void);

int     foo_arg, bar_arg;
long    x;
double  d;
va_list gap;

void foo(int v, va_list ap) {
  switch (v) {
  case 5:
    foo_arg  = va_arg(ap, int);
    foo_arg += va_arg(ap, double);
    foo_arg += va_arg(ap, long long);
    break;
  case 8:
    foo_arg  = va_arg(ap, long long);
    foo_arg += va_arg(ap, double);
    break;
  case 11:
    foo_arg  = va_arg(ap, int);
    foo_arg += va_arg(ap, long double);
    break;
  default:
    abort();
  }
}

void bar(int v) {
  if (v == 0x4002) {
    if (va_arg(gap, int) != 13 || va_arg(gap, double) != -14.0)
      abort();
  }
  bar_arg = v;
}

void f1(int i, ...) {
  va_start(gap, i);
  x = va_arg(gap, long);
  va_end(gap);
}

void f2(int i, ...) {
  va_start(gap, i);
  bar(i);
  va_end(gap);
}

void f3(int i, ...) {
  va_list aps[10];
  va_start(aps[4], i);
  x = va_arg(aps[4], long);
  va_end(aps[4]);
}

void f4(int i, ...) {
  va_list aps[10];
  va_start(aps[4], i);
  bar(i);
  va_end(aps[4]);
}

void f5(int i, ...) {
  va_list aps[10];
  va_start(aps[4], i);
  foo(i, aps[4]);
  va_end(aps[4]);
}

struct A {
  int     i;
  va_list g;
  va_list h[2];
};

void f6(int i, ...) {
  struct A a;
  va_start(a.g, i);
  x = va_arg(a.g, long);
  va_end(a.g);
}

void f7(int i, ...) {
  struct A a;
  va_start(a.g, i);
  bar(i);
  va_end(a.g);
}

void f8(int i, ...) {
  struct A a;
  va_start(a.g, i);
  foo(i, a.g);
  va_end(a.g);
}

void f10(int i, ...) {
  struct A a;
  va_start(a.h[1], i);
  x = va_arg(a.h[1], long);
  va_end(a.h[1]);
}

void f11(int i, ...) {
  struct A a;
  va_start(a.h[1], i);
  bar(i);
  va_end(a.h[1]);
}

void f12(int i, ...) {
  struct A a;
  va_start(a.h[1], i);
  foo(i, a.h[1]);
  va_end(a.h[1]);
}

int main(void) {
  f1(1, 79L);
  if (x != 79L)
    abort();
  f2(0x4002, 13, -14.0);
  if (bar_arg != 0x4002)
    abort();
  f3(3, 2031L);
  if (x != 2031)
    abort();
  f4(4, 18);
  if (bar_arg != 4)
    abort();
  f5(5, 1, 19.0, 18LL);
  if (foo_arg != 38)
    abort();
  f6(6, 18L);
  if (x != 18L)
    abort();
  f7(7);
  if (bar_arg != 7)
    abort();
  f8(8, 2031LL, 13.0);
  if (foo_arg != 2044)
    abort();
  f10(9, 180L);
  if (x != 180L)
    abort();
  f11(10);
  if (bar_arg != 10)
    abort();
  f12(11, 2030, 12.0L);
  if (foo_arg != 2042)
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
// DEFAULT-NEXT:     type @type1 A = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 g: va_list;
// DEFAULT-NEXT:         field2 h: array<va_list, 2>;
// DEFAULT-NEXT:     } [size=80, align=8, offsets=[0, 8, 32]];
// DEFAULT-NEXT:     global %2 foo_arg: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 bar_arg: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 x: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 d: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 gap: va_list [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @foo(%8 v: i32, %9 ap: va_list) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %45 read<i32>(%8)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %45 const<i32>(5):
// DEFAULT-NEXT:                     write<i32>(%2, va_arg<i32>(%9));
// DEFAULT-NEXT:                     va_arg<i32>(%9);
// DEFAULT-NEXT:                 let %46: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %47: i32 [synthetic] = float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%46)), va_arg<f64>(%9)));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%47));
// DEFAULT-NEXT:                 let %48: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %49: i32 [synthetic] = truncate<i32, reason=assign, fits=unknown>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%48)), va_arg<i64>(%9)));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%49));
// DEFAULT-NEXT:                 break %45;
// DEFAULT-NEXT:                 case %45 const<i32>(8):
// DEFAULT-NEXT:                     write<i32>(%2, truncate<i32, reason=assign, fits=unknown>(va_arg<i64>(%9)));
// DEFAULT-NEXT:                     truncate<i32, reason=assign, fits=unknown>(va_arg<i64>(%9));
// DEFAULT-NEXT:                 let %50: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %51: i32 [synthetic] = float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%50)), va_arg<f64>(%9)));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%51));
// DEFAULT-NEXT:                 break %45;
// DEFAULT-NEXT:                 case %45 const<i32>(11):
// DEFAULT-NEXT:                     write<i32>(%2, va_arg<i32>(%9));
// DEFAULT-NEXT:                     va_arg<i32>(%9);
// DEFAULT-NEXT:                 let %52: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                 let %53: i32 [synthetic] = float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%52)), va_arg<f80>(%9)));
// DEFAULT-NEXT:                 write<i32>(%2, read<i32>(%53));
// DEFAULT-NEXT:                 break %45;
// DEFAULT-NEXT:                 default %45:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @bar(%11 v: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%11), const<i32>(16386))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %54: bool [synthetic];
// DEFAULT-NEXT:                 if ne<i32>(va_arg<i32>(%6), const<i32>(13))
// DEFAULT-NEXT:                     write<bool>(%54, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%54, ne<f64, exceptions=ignore>(va_arg<f64>(%6), neg<f64>(const<f64>(14.0))));
// DEFAULT-NEXT:                 if read<bool>(%54)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @f1(%13 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         va_start(%6);
// DEFAULT-NEXT:         write<i64>(%4, va_arg<i64>(%6));
// DEFAULT-NEXT:         va_arg<i64>(%6);
// DEFAULT-NEXT:         va_end(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @f2(%15 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         va_start(%6);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%10, read<i32>(%15));
// DEFAULT-NEXT:         va_end(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @f3(%17 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %18 aps: array<va_list, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         va_start(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%18), const<i32>(4))));
// DEFAULT-NEXT:         write<i64>(%4, va_arg<i64>(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%18), const<i32>(4)))));
// DEFAULT-NEXT:         va_arg<i64>(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%18), const<i32>(4))));
// DEFAULT-NEXT:         va_end(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%18), const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @f4(%20 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %21 aps: array<va_list, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         va_start(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%21), const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%10, read<i32>(%20));
// DEFAULT-NEXT:         va_end(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%21), const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @f5(%23 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %24 aps: array<va_list, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         va_start(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%24), const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, va_list) -> void>(%7, read<i32>(%23), read<va_list>(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%24), const<i32>(4)))));
// DEFAULT-NEXT:         va_end(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%24), const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @f6(%27 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %28 a: @type1 [storage=automatic];
// DEFAULT-NEXT:         va_start(field1(%28));
// DEFAULT-NEXT:         write<i64>(%4, va_arg<i64>(field1(%28)));
// DEFAULT-NEXT:         va_arg<i64>(field1(%28));
// DEFAULT-NEXT:         va_end(field1(%28));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @f7(%30 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %31 a: @type1 [storage=automatic];
// DEFAULT-NEXT:         va_start(field1(%31));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%10, read<i32>(%30));
// DEFAULT-NEXT:         va_end(field1(%31));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @f8(%33 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %34 a: @type1 [storage=automatic];
// DEFAULT-NEXT:         va_start(field1(%34));
// DEFAULT-NEXT:         call<void, signature=fn(i32, va_list) -> void>(%7, read<i32>(%33), read<va_list>(field1(%34)));
// DEFAULT-NEXT:         va_end(field1(%34));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @f10(%36 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %37 a: @type1 [storage=automatic];
// DEFAULT-NEXT:         va_start(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%37)), const<i32>(1))));
// DEFAULT-NEXT:         write<i64>(%4, va_arg<i64>(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%37)), const<i32>(1)))));
// DEFAULT-NEXT:         va_arg<i64>(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%37)), const<i32>(1))));
// DEFAULT-NEXT:         va_end(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%37)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @f11(%39 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %40 a: @type1 [storage=automatic];
// DEFAULT-NEXT:         va_start(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%40)), const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%10, read<i32>(%39));
// DEFAULT-NEXT:         va_end(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%40)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @f12(%42 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %43 a: @type1 [storage=automatic];
// DEFAULT-NEXT:         va_start(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%43)), const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, va_list) -> void>(%7, read<i32>(%42), read<va_list>(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%43)), const<i32>(1)))));
// DEFAULT-NEXT:         va_end(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%43)), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%12, const<i32>(1), const<i64>(79));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%4), const<i64>(79))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%14, const<i32>(16386), const<i32>(13), neg<f64>(const<f64>(14.0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(16386))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%16, const<i32>(3), const<i64>(2031));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%4), widen<i64, reason=usual_arith>(const<i32>(2031)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%19, const<i32>(4), const<i32>(18));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%22, const<i32>(5), const<i32>(1), const<f64>(19.0), const<i64>(18));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(38))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%26, const<i32>(6), const<i64>(18));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%4), const<i64>(18))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%29, const<i32>(7));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%32, const<i32>(8), const<i64>(2031), const<f64>(13.0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(2044))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%35, const<i32>(9), const<i64>(180));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%4), const<i64>(180))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%38, const<i32>(10));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%41, const<i32>(11), const<i32>(2030), const<f80>(12));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(2042))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
