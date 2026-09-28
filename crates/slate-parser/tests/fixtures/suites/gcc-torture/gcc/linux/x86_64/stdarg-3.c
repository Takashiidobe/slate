#include <stdarg.h>

extern void abort(void);

int     foo_arg, bar_arg;
long    x;
double  d;
va_list gap;
struct S1 {
  int    i;
  double d;
  int    j;
  double e;
} s1;
struct S2 {
  double d;
  long   i;
} s2;
int y;

void bar(int v) { bar_arg = v; }

void f1(int i, ...) {
  va_list ap;
  va_start(ap, i);
  while (i-- > 0)
    x = va_arg(ap, long);
  va_end(ap);
}

void f2(int i, ...) {
  va_list ap;
  va_start(ap, i);
  while (i-- > 0)
    d = va_arg(ap, double);
  va_end(ap);
}

void f3(int i, ...) {
  va_list ap;
  int     j = i;
  while (j-- > 0) {
    va_start(ap, i);
    x = va_arg(ap, long);
    va_end(ap);
    bar(x);
  }
}

void f4(int i, ...) {
  va_list ap;
  int     j = i;
  while (j-- > 0) {
    va_start(ap, i);
    d = va_arg(ap, double);
    va_end(ap);
    bar(d + 4.0);
  }
}

void f5(int i, ...) {
  va_list ap;
  va_start(ap, i);
  while (i-- > 0)
    s1 = va_arg(ap, struct S1);
  va_end(ap);
}

void f6(int i, ...) {
  va_list ap;
  va_start(ap, i);
  while (i-- > 0)
    s2 = va_arg(ap, struct S2);
  va_end(ap);
}

void f7(int i, ...) {
  va_list ap;
  int     j = i;
  while (j-- > 0) {
    va_start(ap, i);
    s1 = va_arg(ap, struct S1);
    va_end(ap);
    bar(s1.i);
  }
}

void f8(int i, ...) {
  va_list ap;
  int     j = i;
  while (j-- > 0) {
    va_start(ap, i);
    s2 = va_arg(ap, struct S2);
    y  = va_arg(ap, int);
    va_end(ap);
    bar(s2.i);
  }
}

int main(void) {
  struct S1 a1, a3;
  struct S2 a2, a4;

  f1(7, 1L, 2L, 3L, 5L, 7L, 9L, 11L, 13L);
  if (x != 11L)
    abort();
  f2(6, 1.0, 2.0, 4.0, 8.0, 16.0, 32.0, 64.0);
  if (d != 32.0)
    abort();
  f3(2, 1L, 3L);
  if (bar_arg != 1L || x != 1L)
    abort();
  f4(2, 17.0, 19.0);
  if (bar_arg != 21 || d != 17.0)
    abort();
  a1.i = 131;
  a1.j = 251;
  a1.d = 15.0;
  a1.e = 191.0;
  a3   = a1;
  a3.j = 254;
  a3.e = 178.0;
  f5(2, a1, a3, a1);
  if (s1.i != 131 || s1.j != 254 || s1.d != 15.0 || s1.e != 178.0)
    abort();
  f5(3, a1, a3, a1);
  if (s1.i != 131 || s1.j != 251 || s1.d != 15.0 || s1.e != 191.0)
    abort();
  a2.i = 138;
  a2.d = 16.0;
  a4.i = 257;
  a4.d = 176.0;
  f6(2, a2, a4, a2);
  if (s2.i != 257 || s2.d != 176.0)
    abort();
  f6(3, a2, a4, a2);
  if (s2.i != 138 || s2.d != 16.0)
    abort();
  f7(2, a3, a1, a1);
  if (s1.i != 131 || s1.j != 254 || s1.d != 15.0 || s1.e != 178.0)
    abort();
  if (bar_arg != 131)
    abort();
  f8(3, a4, a2, a2);
  if (s2.i != 257 || s2.d != 176.0)
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
// DEFAULT-NEXT:     type @type2 S1 = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:         field2 j: i32;
// DEFAULT-NEXT:         field3 e: f64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type3 S2 = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 i: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %3 foo_arg: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 bar_arg: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 x: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 d: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 gap: va_list [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 s1: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 s2: @type3 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 y: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %13 @bar(%14 v: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @f1(%16 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %17 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%17);
// DEFAULT-NEXT:         while %48 {
// DEFAULT-NEXT:             let %56: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:             let %57: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%56), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%16, read<i32>(%57));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%56), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<i64>(%5, va_arg<i64>(%17));
// DEFAULT-NEXT:             va_arg<i64>(%17);
// DEFAULT-NEXT:         va_end(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @f2(%19 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %20 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%20);
// DEFAULT-NEXT:         while %49 {
// DEFAULT-NEXT:             let %58: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:             let %59: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%58), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%19, read<i32>(%59));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%58), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<f64>(%6, va_arg<f64>(%20));
// DEFAULT-NEXT:             va_arg<f64>(%20);
// DEFAULT-NEXT:         va_end(%20);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @f3(%22 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %23 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %24 j: i32 [storage=automatic] = read<i32>(%22);
// DEFAULT-NEXT:         while %50 {
// DEFAULT-NEXT:             let %60: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:             let %61: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%60), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%24, read<i32>(%61));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%60), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_start(%23);
// DEFAULT-NEXT:                 write<i64>(%5, va_arg<i64>(%23));
// DEFAULT-NEXT:                 va_arg<i64>(%23);
// DEFAULT-NEXT:                 va_end(%23);
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%13, truncate<i32, reason=arg, fits=unknown>(read<i64>(%5)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @f4(%26 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %27 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %28 j: i32 [storage=automatic] = read<i32>(%26);
// DEFAULT-NEXT:         while %51 {
// DEFAULT-NEXT:             let %62: i32 [synthetic] = read<i32>(%28);
// DEFAULT-NEXT:             let %63: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%62), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%28, read<i32>(%63));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%62), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_start(%27);
// DEFAULT-NEXT:                 write<f64>(%6, va_arg<f64>(%27));
// DEFAULT-NEXT:                 va_arg<f64>(%27);
// DEFAULT-NEXT:                 va_end(%27);
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%13, float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%6), const<f64>(4.0))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @f5(%30 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %31 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%31);
// DEFAULT-NEXT:         while %52 {
// DEFAULT-NEXT:             let %64: i32 [synthetic] = read<i32>(%30);
// DEFAULT-NEXT:             let %65: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%64), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%30, read<i32>(%65));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%64), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<@type2>(%9, copy<@type2, reason=assign>(va_arg<@type2>(%31)));
// DEFAULT-NEXT:             copy<@type2, reason=assign>(va_arg<@type2>(%31));
// DEFAULT-NEXT:         va_end(%31);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @f6(%33 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %34 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%34);
// DEFAULT-NEXT:         while %53 {
// DEFAULT-NEXT:             let %66: i32 [synthetic] = read<i32>(%33);
// DEFAULT-NEXT:             let %67: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%66), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%33, read<i32>(%67));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%66), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<@type3>(%11, copy<@type3, reason=assign>(va_arg<@type3>(%34)));
// DEFAULT-NEXT:             copy<@type3, reason=assign>(va_arg<@type3>(%34));
// DEFAULT-NEXT:         va_end(%34);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @f7(%36 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %37 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %38 j: i32 [storage=automatic] = read<i32>(%36);
// DEFAULT-NEXT:         while %54 {
// DEFAULT-NEXT:             let %68: i32 [synthetic] = read<i32>(%38);
// DEFAULT-NEXT:             let %69: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%68), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%38, read<i32>(%69));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%68), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_start(%37);
// DEFAULT-NEXT:                 write<@type2>(%9, copy<@type2, reason=assign>(va_arg<@type2>(%37)));
// DEFAULT-NEXT:                 copy<@type2, reason=assign>(va_arg<@type2>(%37));
// DEFAULT-NEXT:                 va_end(%37);
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%13, read<i32>(field0(%9)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @f8(%40 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %41 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %42 j: i32 [storage=automatic] = read<i32>(%40);
// DEFAULT-NEXT:         while %55 {
// DEFAULT-NEXT:             let %70: i32 [synthetic] = read<i32>(%42);
// DEFAULT-NEXT:             let %71: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%70), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%42, read<i32>(%71));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%70), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_start(%41);
// DEFAULT-NEXT:                 write<@type3>(%11, copy<@type3, reason=assign>(va_arg<@type3>(%41)));
// DEFAULT-NEXT:                 copy<@type3, reason=assign>(va_arg<@type3>(%41));
// DEFAULT-NEXT:                 write<i32>(%12, va_arg<i32>(%41));
// DEFAULT-NEXT:                 va_arg<i32>(%41);
// DEFAULT-NEXT:                 va_end(%41);
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%13, truncate<i32, reason=arg, fits=unknown>(read<i64>(field1(%11))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %44 a1: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %45 a3: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %46 a2: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %47 a4: @type3 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%15, const<i32>(7), const<i64>(1), const<i64>(2), const<i64>(3), const<i64>(5), const<i64>(7), const<i64>(9), const<i64>(11), const<i64>(13));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%5), const<i64>(11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%18, const<i32>(6), const<f64>(1.0), const<f64>(2.0), const<f64>(4.0), const<f64>(8.0), const<f64>(16.0), const<f64>(32.0), const<f64>(64.0));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%6), const<f64>(32.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%21, const<i32>(2), const<i64>(1), const<i64>(3));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(widen<i64, reason=usual_arith>(read<i32>(%4)), const<i64>(1)), ne<i64>(read<i64>(%5), const<i64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%25, const<i32>(2), const<f64>(17.0), const<f64>(19.0));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%4), const<i32>(21)), ne<f64, exceptions=observable>(read<f64>(%6), const<f64>(17.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<i32>(field0(%44), const<i32>(131));
// DEFAULT-NEXT:         write<i32>(field2(%44), const<i32>(251));
// DEFAULT-NEXT:         write<f64>(field1(%44), const<f64>(15.0));
// DEFAULT-NEXT:         write<f64>(field3(%44), const<f64>(191.0));
// DEFAULT-NEXT:         write<@type2>(%45, copy<@type2, reason=assign>(read<@type2>(%44)));
// DEFAULT-NEXT:         write<i32>(field2(%45), const<i32>(254));
// DEFAULT-NEXT:         write<f64>(field3(%45), const<f64>(178.0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c) -> void>(%29, const<i32>(2), copy<@type2, reason=vararg>(read<@type2>(%44)), copy<@type2, reason=vararg>(read<@type2>(%45)), copy<@type2, reason=vararg>(read<@type2>(%44)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%9)), const<i32>(131)), ne<i32>(read<i32>(field2(%9)), const<i32>(254))), ne<f64, exceptions=observable>(read<f64>(field1(%9)), const<f64>(15.0))), ne<f64, exceptions=observable>(read<f64>(field3(%9)), const<f64>(178.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c) -> void>(%29, const<i32>(3), copy<@type2, reason=vararg>(read<@type2>(%44)), copy<@type2, reason=vararg>(read<@type2>(%45)), copy<@type2, reason=vararg>(read<@type2>(%44)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%9)), const<i32>(131)), ne<i32>(read<i32>(field2(%9)), const<i32>(251))), ne<f64, exceptions=observable>(read<f64>(field1(%9)), const<f64>(15.0))), ne<f64, exceptions=observable>(read<f64>(field3(%9)), const<f64>(191.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<i64>(field1(%46), widen<i64, reason=assign>(const<i32>(138)));
// DEFAULT-NEXT:         write<f64>(field0(%46), const<f64>(16.0));
// DEFAULT-NEXT:         write<i64>(field1(%47), widen<i64, reason=assign>(const<i32>(257)));
// DEFAULT-NEXT:         write<f64>(field0(%47), const<f64>(176.0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c) -> void>(%32, const<i32>(2), copy<@type3, reason=vararg>(read<@type3>(%46)), copy<@type3, reason=vararg>(read<@type3>(%47)), copy<@type3, reason=vararg>(read<@type3>(%46)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(field1(%11)), widen<i64, reason=usual_arith>(const<i32>(257))), ne<f64, exceptions=observable>(read<f64>(field0(%11)), const<f64>(176.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c) -> void>(%32, const<i32>(3), copy<@type3, reason=vararg>(read<@type3>(%46)), copy<@type3, reason=vararg>(read<@type3>(%47)), copy<@type3, reason=vararg>(read<@type3>(%46)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(field1(%11)), widen<i64, reason=usual_arith>(const<i32>(138))), ne<f64, exceptions=observable>(read<f64>(field0(%11)), const<f64>(16.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c) -> void>(%35, const<i32>(2), copy<@type2, reason=vararg>(read<@type2>(%45)), copy<@type2, reason=vararg>(read<@type2>(%44)), copy<@type2, reason=vararg>(read<@type2>(%44)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%9)), const<i32>(131)), ne<i32>(read<i32>(field2(%9)), const<i32>(254))), ne<f64, exceptions=observable>(read<f64>(field1(%9)), const<f64>(15.0))), ne<f64, exceptions=observable>(read<f64>(field3(%9)), const<f64>(178.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(131))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c) -> void>(%39, const<i32>(3), copy<@type3, reason=vararg>(read<@type3>(%47)), copy<@type3, reason=vararg>(read<@type3>(%46)), copy<@type3, reason=vararg>(read<@type3>(%46)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(field1(%11)), widen<i64, reason=usual_arith>(const<i32>(257))), ne<f64, exceptions=observable>(read<f64>(field0(%11)), const<f64>(176.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
