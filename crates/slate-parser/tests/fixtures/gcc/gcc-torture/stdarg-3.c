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
// DEFAULT-NEXT:     type @type0 va_list = va_list;
// DEFAULT-NEXT:     type @type1 S1 = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:         field2 j: i32;
// DEFAULT-NEXT:         field3 e: f64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type2 S2 = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 i: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %2 foo_arg: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 bar_arg: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 x: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 d: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 gap: va_list [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 s1: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 s2: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 y: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %12 @bar(%13 v: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @f1(%15 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %16 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%16);
// DEFAULT-NEXT:         while %47 {
// DEFAULT-NEXT:             let %55: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:             let %56: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%55), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%15, read<i32>(%56));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%55), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<i64>(%4, va_arg<i64>(%16));
// DEFAULT-NEXT:             va_arg<i64>(%16);
// DEFAULT-NEXT:         va_end(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @f2(%18 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %19 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%19);
// DEFAULT-NEXT:         while %48 {
// DEFAULT-NEXT:             let %57: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:             let %58: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%57), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%18, read<i32>(%58));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%57), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<f64>(%5, va_arg<f64>(%19));
// DEFAULT-NEXT:             va_arg<f64>(%19);
// DEFAULT-NEXT:         va_end(%19);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @f3(%21 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %22 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %23 j: i32 [storage=automatic] = read<i32>(%21);
// DEFAULT-NEXT:         while %49 {
// DEFAULT-NEXT:             let %59: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:             let %60: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%59), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%23, read<i32>(%60));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%59), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_start(%22);
// DEFAULT-NEXT:                 write<i64>(%4, va_arg<i64>(%22));
// DEFAULT-NEXT:                 va_arg<i64>(%22);
// DEFAULT-NEXT:                 va_end(%22);
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%12, truncate<i32, reason=arg, fits=unknown>(read<i64>(%4)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @f4(%25 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %26 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %27 j: i32 [storage=automatic] = read<i32>(%25);
// DEFAULT-NEXT:         while %50 {
// DEFAULT-NEXT:             let %61: i32 [synthetic] = read<i32>(%27);
// DEFAULT-NEXT:             let %62: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%61), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%27, read<i32>(%62));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%61), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_start(%26);
// DEFAULT-NEXT:                 write<f64>(%5, va_arg<f64>(%26));
// DEFAULT-NEXT:                 va_arg<f64>(%26);
// DEFAULT-NEXT:                 va_end(%26);
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%12, float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%5), const<f64>(4.0))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @f5(%29 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %30 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%30);
// DEFAULT-NEXT:         while %51 {
// DEFAULT-NEXT:             let %63: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:             let %64: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%63), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%29, read<i32>(%64));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%63), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<@type1>(%8, copy<@type1, reason=assign>(va_arg<@type1>(%30)));
// DEFAULT-NEXT:             copy<@type1, reason=assign>(va_arg<@type1>(%30));
// DEFAULT-NEXT:         va_end(%30);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @f6(%32 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %33 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%33);
// DEFAULT-NEXT:         while %52 {
// DEFAULT-NEXT:             let %65: i32 [synthetic] = read<i32>(%32);
// DEFAULT-NEXT:             let %66: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%65), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%32, read<i32>(%66));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%65), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<@type2>(%10, copy<@type2, reason=assign>(va_arg<@type2>(%33)));
// DEFAULT-NEXT:             copy<@type2, reason=assign>(va_arg<@type2>(%33));
// DEFAULT-NEXT:         va_end(%33);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @f7(%35 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %36 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %37 j: i32 [storage=automatic] = read<i32>(%35);
// DEFAULT-NEXT:         while %53 {
// DEFAULT-NEXT:             let %67: i32 [synthetic] = read<i32>(%37);
// DEFAULT-NEXT:             let %68: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%67), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%37, read<i32>(%68));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%67), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_start(%36);
// DEFAULT-NEXT:                 write<@type1>(%8, copy<@type1, reason=assign>(va_arg<@type1>(%36)));
// DEFAULT-NEXT:                 copy<@type1, reason=assign>(va_arg<@type1>(%36));
// DEFAULT-NEXT:                 va_end(%36);
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%12, read<i32>(field0(%8)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @f8(%39 i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %40 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %41 j: i32 [storage=automatic] = read<i32>(%39);
// DEFAULT-NEXT:         while %54 {
// DEFAULT-NEXT:             let %69: i32 [synthetic] = read<i32>(%41);
// DEFAULT-NEXT:             let %70: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%69), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%41, read<i32>(%70));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%69), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_start(%40);
// DEFAULT-NEXT:                 write<@type2>(%10, copy<@type2, reason=assign>(va_arg<@type2>(%40)));
// DEFAULT-NEXT:                 copy<@type2, reason=assign>(va_arg<@type2>(%40));
// DEFAULT-NEXT:                 write<i32>(%11, va_arg<i32>(%40));
// DEFAULT-NEXT:                 va_arg<i32>(%40);
// DEFAULT-NEXT:                 va_end(%40);
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%12, truncate<i32, reason=arg, fits=unknown>(read<i64>(field1(%10))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %43 a1: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %44 a3: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %45 a2: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %46 a4: @type2 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%14, const<i32>(7), const<i64>(1), const<i64>(2), const<i64>(3), const<i64>(5), const<i64>(7), const<i64>(9), const<i64>(11), const<i64>(13));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%4), const<i64>(11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%17, const<i32>(6), const<f64>(1.0), const<f64>(2.0), const<f64>(4.0), const<f64>(8.0), const<f64>(16.0), const<f64>(32.0), const<f64>(64.0));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(%5), const<f64>(32.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%20, const<i32>(2), const<i64>(1), const<i64>(3));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(widen<i64, reason=usual_arith>(read<i32>(%3)), const<i64>(1)), ne<i64>(read<i64>(%4), const<i64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%24, const<i32>(2), const<f64>(17.0), const<f64>(19.0));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%3), const<i32>(21)), ne<f64, exceptions=ignore>(read<f64>(%5), const<f64>(17.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         write<i32>(field0(%43), const<i32>(131));
// DEFAULT-NEXT:         write<i32>(field2(%43), const<i32>(251));
// DEFAULT-NEXT:         write<f64>(field1(%43), const<f64>(15.0));
// DEFAULT-NEXT:         write<f64>(field3(%43), const<f64>(191.0));
// DEFAULT-NEXT:         write<@type1>(%44, copy<@type1, reason=assign>(read<@type1>(%43)));
// DEFAULT-NEXT:         write<i32>(field2(%44), const<i32>(254));
// DEFAULT-NEXT:         write<f64>(field3(%44), const<f64>(178.0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, byval<align=8>, byval<align=8>, byval<align=8>) -> void>(%28, const<i32>(2), copy<@type1, reason=vararg>(read<@type1>(%43)), copy<@type1, reason=vararg>(read<@type1>(%44)), copy<@type1, reason=vararg>(read<@type1>(%43)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%8)), const<i32>(131)), ne<i32>(read<i32>(field2(%8)), const<i32>(254))), ne<f64, exceptions=ignore>(read<f64>(field1(%8)), const<f64>(15.0))), ne<f64, exceptions=ignore>(read<f64>(field3(%8)), const<f64>(178.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, byval<align=8>, byval<align=8>, byval<align=8>) -> void>(%28, const<i32>(3), copy<@type1, reason=vararg>(read<@type1>(%43)), copy<@type1, reason=vararg>(read<@type1>(%44)), copy<@type1, reason=vararg>(read<@type1>(%43)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%8)), const<i32>(131)), ne<i32>(read<i32>(field2(%8)), const<i32>(251))), ne<f64, exceptions=ignore>(read<f64>(field1(%8)), const<f64>(15.0))), ne<f64, exceptions=ignore>(read<f64>(field3(%8)), const<f64>(191.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         write<i64>(field1(%45), widen<i64, reason=assign>(const<i32>(138)));
// DEFAULT-NEXT:         write<f64>(field0(%45), const<f64>(16.0));
// DEFAULT-NEXT:         write<i64>(field1(%46), widen<i64, reason=assign>(const<i32>(257)));
// DEFAULT-NEXT:         write<f64>(field0(%46), const<f64>(176.0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, coerce<f64, i64>, coerce<f64, i64>, coerce<f64, i64>) -> void>(%31, const<i32>(2), copy<@type2, reason=vararg>(read<@type2>(%45)), copy<@type2, reason=vararg>(read<@type2>(%46)), copy<@type2, reason=vararg>(read<@type2>(%45)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(field1(%10)), widen<i64, reason=usual_arith>(const<i32>(257))), ne<f64, exceptions=ignore>(read<f64>(field0(%10)), const<f64>(176.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, coerce<f64, i64>, coerce<f64, i64>, coerce<f64, i64>) -> void>(%31, const<i32>(3), copy<@type2, reason=vararg>(read<@type2>(%45)), copy<@type2, reason=vararg>(read<@type2>(%46)), copy<@type2, reason=vararg>(read<@type2>(%45)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(field1(%10)), widen<i64, reason=usual_arith>(const<i32>(138))), ne<f64, exceptions=ignore>(read<f64>(field0(%10)), const<f64>(16.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, byval<align=8>, byval<align=8>, byval<align=8>) -> void>(%34, const<i32>(2), copy<@type1, reason=vararg>(read<@type1>(%44)), copy<@type1, reason=vararg>(read<@type1>(%43)), copy<@type1, reason=vararg>(read<@type1>(%43)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%8)), const<i32>(131)), ne<i32>(read<i32>(field2(%8)), const<i32>(254))), ne<f64, exceptions=ignore>(read<f64>(field1(%8)), const<f64>(15.0))), ne<f64, exceptions=ignore>(read<f64>(field3(%8)), const<f64>(178.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(131))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, coerce<f64, i64>, coerce<f64, i64>, coerce<f64, i64>) -> void>(%38, const<i32>(3), copy<@type2, reason=vararg>(read<@type2>(%46)), copy<@type2, reason=vararg>(read<@type2>(%45)), copy<@type2, reason=vararg>(read<@type2>(%45)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(field1(%10)), widen<i64, reason=usual_arith>(const<i32>(257))), ne<f64, exceptions=ignore>(read<f64>(field0(%10)), const<f64>(176.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
