#include <stdarg.h>

extern void abort(void);

int      foo_arg, bar_arg;
long     x;
double   d;
va_list  gap;
va_list *pap;

void foo(int v, va_list ap) {
  switch (v) {
  case 5:
    foo_arg = va_arg(ap, int);
    break;
  default:
    abort();
  }
}

void bar(int v) {
  if (v == 0x4006) {
    if (va_arg(gap, double) != 17.0 || va_arg(gap, long) != 129L)
      abort();
  } else if (v == 0x4008) {
    if (va_arg(*pap, long long) != 14LL ||
        va_arg(*pap, long double) != 131.0L || va_arg(*pap, int) != 17)
      abort();
  }
  bar_arg = v;
}

void f0(int i, ...) {}

void f1(int i, ...) {
  va_list ap;
  va_start(ap, i);
  va_end(ap);
}

void f2(int i, ...) {
  va_list ap;
  va_start(ap, i);
  bar(d);
  x = va_arg(ap, long);
  bar(x);
  va_end(ap);
}

void f3(int i, ...) {
  va_list ap;
  va_start(ap, i);
  d = va_arg(ap, double);
  va_end(ap);
}

void f4(int i, ...) {
  va_list ap;
  va_start(ap, i);
  x = va_arg(ap, double);
  foo(i, ap);
  va_end(ap);
}

void f5(int i, ...) {
  va_list ap;
  va_start(ap, i);
  va_copy(gap, ap);
  bar(i);
  va_end(ap);
  va_end(gap);
}

void f6(int i, ...) {
  va_list ap;
  va_start(ap, i);
  bar(d);
  va_arg(ap, long);
  va_arg(ap, long);
  x = va_arg(ap, long);
  bar(x);
  va_end(ap);
}

void f7(int i, ...) {
  va_list ap;
  va_start(ap, i);
  pap = &ap;
  bar(i);
  va_end(ap);
}

void f8(int i, ...) {
  va_list ap;
  va_start(ap, i);
  pap = &ap;
  bar(i);
  d = va_arg(ap, double);
  va_end(ap);
}

int main(void) {
  f0(1);
  f1(2);
  d = 31.0;
  f2(3, 28L);
  if (bar_arg != 28 || x != 28)
    abort();
  f3(4, 131.0);
  if (d != 131.0)
    abort();
  f4(5, 16.0, 128);
  if (x != 16 || foo_arg != 128)
    abort();
  f5(0x4006, 17.0, 129L);
  if (bar_arg != 0x4006)
    abort();
  f6(7, 12L, 14L, -31L);
  if (bar_arg != -31)
    abort();
  f7(0x4008, 14LL, 131.0L, 17, 26.0);
  if (bar_arg != 0x4008)
    abort();
  f8(0x4008, 14LL, 131.0L, 17, 27.0);
  if (bar_arg != 0x4008 || d != 27.0)
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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     global %3 foo_arg: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 bar_arg: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 x: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 d: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 gap: va_list [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 pap: ptr<va_list> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @foo(%10 v: i32, %11 ap: va_list) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %41 read<i32>(%10)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %41 const<i32>(5):
// DEFAULT-NEXT:                     write<i32>(%3, va_arg<i32>(%11));
// DEFAULT-NEXT:                     va_arg<i32>(%11);
// DEFAULT-NEXT:                 break %41;
// DEFAULT-NEXT:                 default %41:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @bar(%13 v: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%13), const<i32>(16390))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %42: bool [synthetic];
// DEFAULT-NEXT:                 if ne<f64, exceptions=observable>(va_arg<f64>(%7), const<f64>(17.0))
// DEFAULT-NEXT:                     write<bool>(%42, const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%42, ne<i64>(va_arg<i64>(%7), const<i64>(129)));
// DEFAULT-NEXT:                 if read<bool>(%42)
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%13), const<i32>(16392))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %43: bool [synthetic];
// DEFAULT-NEXT:                     if ne<i64>(va_arg<i64>(deref(read<ptr<va_list>>(%8))), const<i64>(14))
// DEFAULT-NEXT:                         write<bool>(%43, const<bool>(true));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%43, ne<f80, exceptions=observable>(va_arg<f80>(deref(read<ptr<va_list>>(%8))), const<f80>(131)));
// DEFAULT-NEXT:                     let %44: bool [synthetic];
// DEFAULT-NEXT:                     if read<bool>(%43)
// DEFAULT-NEXT:                         write<bool>(%44, const<bool>(true));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%44, ne<i32>(va_arg<i32>(deref(read<ptr<va_list>>(%8))), const<i32>(17)));
// DEFAULT-NEXT:                     if read<bool>(%44)
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @f0(%15 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @f1(%17 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %18 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%18);
// DEFAULT-NEXT:         va_end(%18);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @f2(%20 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %21 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%21);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%12, float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<f64>(%6)));
// DEFAULT-NEXT:         write<i64>(%5, va_arg<i64>(%21));
// DEFAULT-NEXT:         va_arg<i64>(%21);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%12, truncate<i32, reason=arg, fits=unknown>(read<i64>(%5)));
// DEFAULT-NEXT:         va_end(%21);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @f3(%23 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %24 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%24);
// DEFAULT-NEXT:         write<f64>(%6, va_arg<f64>(%24));
// DEFAULT-NEXT:         va_arg<f64>(%24);
// DEFAULT-NEXT:         va_end(%24);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @f4(%26 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %27 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%27);
// DEFAULT-NEXT:         write<i64>(%5, float_to_int<i64, reason=assign, out_of_range=ub, exceptions=observable>(va_arg<f64>(%27)));
// DEFAULT-NEXT:         float_to_int<i64, reason=assign, out_of_range=ub, exceptions=observable>(va_arg<f64>(%27));
// DEFAULT-NEXT:         call<void, signature=fn(i32, va_list) -> void>(%9, read<i32>(%26), read<va_list>(%27));
// DEFAULT-NEXT:         va_end(%27);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @f5(%29 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %30 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%30);
// DEFAULT-NEXT:         va_copy(%7, %30);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%12, read<i32>(%29));
// DEFAULT-NEXT:         va_end(%30);
// DEFAULT-NEXT:         va_end(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @f6(%32 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %33 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%33);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%12, float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<f64>(%6)));
// DEFAULT-NEXT:         va_arg<i64>(%33);
// DEFAULT-NEXT:         va_arg<i64>(%33);
// DEFAULT-NEXT:         write<i64>(%5, va_arg<i64>(%33));
// DEFAULT-NEXT:         va_arg<i64>(%33);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%12, truncate<i32, reason=arg, fits=unknown>(read<i64>(%5)));
// DEFAULT-NEXT:         va_end(%33);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @f7(%35 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %36 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%36);
// DEFAULT-NEXT:         write<ptr<va_list>>(%8, addr_of<ptr<va_list>>(%36));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%12, read<i32>(%35));
// DEFAULT-NEXT:         va_end(%36);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @f8(%38 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %39 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%39);
// DEFAULT-NEXT:         write<ptr<va_list>>(%8, addr_of<ptr<va_list>>(%39));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%12, read<i32>(%38));
// DEFAULT-NEXT:         write<f64>(%6, va_arg<f64>(%39));
// DEFAULT-NEXT:         va_arg<f64>(%39);
// DEFAULT-NEXT:         va_end(%39);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%14, const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%16, const<i32>(2));
// DEFAULT-NEXT:         write<f64>(%6, const<f64>(31.0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%19, const<i32>(3), const<i64>(28));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%4), const<i32>(28)), ne<i64>(read<i64>(%5), widen<i64, reason=usual_arith>(const<i32>(28))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%22, const<i32>(4), const<f64>(131.0));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%6), const<f64>(131.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%25, const<i32>(5), const<f64>(16.0), const<i32>(128));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(%5), widen<i64, reason=usual_arith>(const<i32>(16))), ne<i32>(read<i32>(%3), const<i32>(128)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%28, const<i32>(16390), const<f64>(17.0), const<i64>(129));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(16390))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%31, const<i32>(7), const<i64>(12), const<i64>(14), neg<i64, overflow=ub>(const<i64>(31)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), neg<i32, overflow=ub>(const<i32>(31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%34, const<i32>(16392), const<i64>(14), const<f80>(131), const<i32>(17), const<f64>(26.0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(16392))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%37, const<i32>(16392), const<i64>(14), const<f80>(131), const<i32>(17), const<f64>(27.0));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%4), const<i32>(16392)), ne<f64, exceptions=observable>(read<f64>(%6), const<f64>(27.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
