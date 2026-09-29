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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_S1:[0-9]+]] S1 = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:         field2 j: i32;
// DEFAULT-NEXT:         field3 e: f64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_S2:[0-9]+]] S2 = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:         field1 i: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_foo_arg:[0-9]+]] foo_arg: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_bar_arg:[0-9]+]] bar_arg: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_gap:[0-9]+]] gap: va_list [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s1:[0-9]+]] s1: @type[[TYPE_S1]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s2:[0-9]+]] s2: @type[[TYPE_S2]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_y:[0-9]+]] y: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_v:[0-9]+]] v: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_bar_arg]], read<i32>(%[[VALUE_v]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_i:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%[[VALUE1]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<i64>(%[[VALUE_x]], va_arg<i64>(%[[VALUE_ap]]));
// DEFAULT-NEXT:             va_arg<i64>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_i_2:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         while %[[VALUE3:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%[[VALUE4]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<f64>(%[[VALUE_d]], va_arg<f64>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:             va_arg<f64>(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_i_3:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_3:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:         while %[[VALUE6:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%[[VALUE7]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_start(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_x]], va_arg<i64>(%[[VALUE_ap_3]]));
// DEFAULT-NEXT:                 va_arg<i64>(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:                 va_end(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], truncate<i32, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_i_4:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_4:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j_2:[0-9]+]] j: i32 [storage=automatic] = read<i32>(%[[VALUE_i_4]]);
// DEFAULT-NEXT:         while %[[VALUE9:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_2]]);
// DEFAULT-NEXT:             let %[[VALUE11:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_j_2]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%[[VALUE10]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_start(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:                 write<f64>(%[[VALUE_d]], va_arg<f64>(%[[VALUE_ap_4]]));
// DEFAULT-NEXT:                 va_arg<f64>(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:                 va_end(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_d]]), const<f64>(4.0))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_i_5:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_5:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         while %[[VALUE12:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_5]]);
// DEFAULT-NEXT:             let %[[VALUE14:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_i_5]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%[[VALUE13]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<@type[[TYPE_S1]]>(%[[VALUE_s1]], copy<@type[[TYPE_S1]], reason=assign>(va_arg<@type[[TYPE_S1]]>(%[[VALUE_ap_5]])));
// DEFAULT-NEXT:             copy<@type[[TYPE_S1]], reason=assign>(va_arg<@type[[TYPE_S1]]>(%[[VALUE_ap_5]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE_i_6:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_6:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         while %[[VALUE15:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_6]]);
// DEFAULT-NEXT:             let %[[VALUE17:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_i_6]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%[[VALUE16]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<@type[[TYPE_S2]]>(%[[VALUE_s2]], copy<@type[[TYPE_S2]], reason=assign>(va_arg<@type[[TYPE_S2]]>(%[[VALUE_ap_6]])));
// DEFAULT-NEXT:             copy<@type[[TYPE_S2]], reason=assign>(va_arg<@type[[TYPE_S2]]>(%[[VALUE_ap_6]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_i_7:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_7:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j_3:[0-9]+]] j: i32 [storage=automatic] = read<i32>(%[[VALUE_i_7]]);
// DEFAULT-NEXT:         while %[[VALUE18:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_3]]);
// DEFAULT-NEXT:             let %[[VALUE20:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE19]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_j_3]], read<i32>(%[[VALUE20]]));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%[[VALUE19]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_start(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:                 write<@type[[TYPE_S1]]>(%[[VALUE_s1]], copy<@type[[TYPE_S1]], reason=assign>(va_arg<@type[[TYPE_S1]]>(%[[VALUE_ap_7]])));
// DEFAULT-NEXT:                 copy<@type[[TYPE_S1]], reason=assign>(va_arg<@type[[TYPE_S1]]>(%[[VALUE_ap_7]]));
// DEFAULT-NEXT:                 va_end(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], read<i32>(field0(%[[VALUE_s1]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(%[[VALUE_i_8:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_8:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j_4:[0-9]+]] j: i32 [storage=automatic] = read<i32>(%[[VALUE_i_8]]);
// DEFAULT-NEXT:         while %[[VALUE21:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_4]]);
// DEFAULT-NEXT:             let %[[VALUE23:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_j_4]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:             yield gt<i32>(read<i32>(%[[VALUE22]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_start(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:                 write<@type[[TYPE_S2]]>(%[[VALUE_s2]], copy<@type[[TYPE_S2]], reason=assign>(va_arg<@type[[TYPE_S2]]>(%[[VALUE_ap_8]])));
// DEFAULT-NEXT:                 copy<@type[[TYPE_S2]], reason=assign>(va_arg<@type[[TYPE_S2]]>(%[[VALUE_ap_8]]));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_y]], va_arg<i32>(%[[VALUE_ap_8]]));
// DEFAULT-NEXT:                 va_arg<i32>(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:                 va_end(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], truncate<i32, reason=arg, fits=unknown>(read<i64>(field1(%[[VALUE_s2]]))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a1:[0-9]+]] a1: @type[[TYPE_S1]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a3:[0-9]+]] a3: @type[[TYPE_S1]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a2:[0-9]+]] a2: @type[[TYPE_S2]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_a4:[0-9]+]] a4: @type[[TYPE_S2]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f1]], const<i32>(7), const<i64>(1), const<i64>(2), const<i64>(3), const<i64>(5), const<i64>(7), const<i64>(9), const<i64>(11), const<i64>(13));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_x]]), const<i64>(11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f2]], const<i32>(6), const<f64>(1.0), const<f64>(2.0), const<f64>(4.0), const<f64>(8.0), const<f64>(16.0), const<f64>(32.0), const<f64>(64.0));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE_d]]), const<f64>(32.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f3]], const<i32>(2), const<i64>(1), const<i64>(3));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_bar_arg]])), const<i64>(1)), ne<i64>(read<i64>(%[[VALUE_x]]), const<i64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f4]], const<i32>(2), const<f64>(17.0), const<f64>(19.0));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_bar_arg]]), const<i32>(21)), ne<f64, exceptions=observable>(read<f64>(%[[VALUE_d]]), const<f64>(17.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_a1]]), const<i32>(131));
// DEFAULT-NEXT:         write<i32>(field2(%[[VALUE_a1]]), const<i32>(251));
// DEFAULT-NEXT:         write<f64>(field1(%[[VALUE_a1]]), const<f64>(15.0));
// DEFAULT-NEXT:         write<f64>(field3(%[[VALUE_a1]]), const<f64>(191.0));
// DEFAULT-NEXT:         write<@type[[TYPE_S1]]>(%[[VALUE_a3]], copy<@type[[TYPE_S1]], reason=assign>(read<@type[[TYPE_S1]]>(%[[VALUE_a1]])));
// DEFAULT-NEXT:         write<i32>(field2(%[[VALUE_a3]]), const<i32>(254));
// DEFAULT-NEXT:         write<f64>(field3(%[[VALUE_a3]]), const<f64>(178.0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c) -> void>(%[[VALUE_f5]], const<i32>(2), copy<@type[[TYPE_S1]], reason=vararg>(read<@type[[TYPE_S1]]>(%[[VALUE_a1]])), copy<@type[[TYPE_S1]], reason=vararg>(read<@type[[TYPE_S1]]>(%[[VALUE_a3]])), copy<@type[[TYPE_S1]], reason=vararg>(read<@type[[TYPE_S1]]>(%[[VALUE_a1]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%[[VALUE_s1]])), const<i32>(131)), ne<i32>(read<i32>(field2(%[[VALUE_s1]])), const<i32>(254))), ne<f64, exceptions=observable>(read<f64>(field1(%[[VALUE_s1]])), const<f64>(15.0))), ne<f64, exceptions=observable>(read<f64>(field3(%[[VALUE_s1]])), const<f64>(178.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c) -> void>(%[[VALUE_f5]], const<i32>(3), copy<@type[[TYPE_S1]], reason=vararg>(read<@type[[TYPE_S1]]>(%[[VALUE_a1]])), copy<@type[[TYPE_S1]], reason=vararg>(read<@type[[TYPE_S1]]>(%[[VALUE_a3]])), copy<@type[[TYPE_S1]], reason=vararg>(read<@type[[TYPE_S1]]>(%[[VALUE_a1]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%[[VALUE_s1]])), const<i32>(131)), ne<i32>(read<i32>(field2(%[[VALUE_s1]])), const<i32>(251))), ne<f64, exceptions=observable>(read<f64>(field1(%[[VALUE_s1]])), const<f64>(15.0))), ne<f64, exceptions=observable>(read<f64>(field3(%[[VALUE_s1]])), const<f64>(191.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i64>(field1(%[[VALUE_a2]]), widen<i64, reason=assign>(const<i32>(138)));
// DEFAULT-NEXT:         write<f64>(field0(%[[VALUE_a2]]), const<f64>(16.0));
// DEFAULT-NEXT:         write<i64>(field1(%[[VALUE_a4]]), widen<i64, reason=assign>(const<i32>(257)));
// DEFAULT-NEXT:         write<f64>(field0(%[[VALUE_a4]]), const<f64>(176.0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c) -> void>(%[[VALUE_f6]], const<i32>(2), copy<@type[[TYPE_S2]], reason=vararg>(read<@type[[TYPE_S2]]>(%[[VALUE_a2]])), copy<@type[[TYPE_S2]], reason=vararg>(read<@type[[TYPE_S2]]>(%[[VALUE_a4]])), copy<@type[[TYPE_S2]], reason=vararg>(read<@type[[TYPE_S2]]>(%[[VALUE_a2]])));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(field1(%[[VALUE_s2]])), widen<i64, reason=usual_arith>(const<i32>(257))), ne<f64, exceptions=observable>(read<f64>(field0(%[[VALUE_s2]])), const<f64>(176.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c) -> void>(%[[VALUE_f6]], const<i32>(3), copy<@type[[TYPE_S2]], reason=vararg>(read<@type[[TYPE_S2]]>(%[[VALUE_a2]])), copy<@type[[TYPE_S2]], reason=vararg>(read<@type[[TYPE_S2]]>(%[[VALUE_a4]])), copy<@type[[TYPE_S2]], reason=vararg>(read<@type[[TYPE_S2]]>(%[[VALUE_a2]])));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(field1(%[[VALUE_s2]])), widen<i64, reason=usual_arith>(const<i32>(138))), ne<f64, exceptions=observable>(read<f64>(field0(%[[VALUE_s2]])), const<f64>(16.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c) -> void>(%[[VALUE_f7]], const<i32>(2), copy<@type[[TYPE_S1]], reason=vararg>(read<@type[[TYPE_S1]]>(%[[VALUE_a3]])), copy<@type[[TYPE_S1]], reason=vararg>(read<@type[[TYPE_S1]]>(%[[VALUE_a1]])), copy<@type[[TYPE_S1]], reason=vararg>(read<@type[[TYPE_S1]]>(%[[VALUE_a1]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(%[[VALUE_s1]])), const<i32>(131)), ne<i32>(read<i32>(field2(%[[VALUE_s1]])), const<i32>(254))), ne<f64, exceptions=observable>(read<f64>(field1(%[[VALUE_s1]])), const<f64>(15.0))), ne<f64, exceptions=observable>(read<f64>(field3(%[[VALUE_s1]])), const<f64>(178.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_bar_arg]]), const<i32>(131))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c) -> void>(%[[VALUE_f8]], const<i32>(3), copy<@type[[TYPE_S2]], reason=vararg>(read<@type[[TYPE_S2]]>(%[[VALUE_a4]])), copy<@type[[TYPE_S2]], reason=vararg>(read<@type[[TYPE_S2]]>(%[[VALUE_a2]])), copy<@type[[TYPE_S2]], reason=vararg>(read<@type[[TYPE_S2]]>(%[[VALUE_a2]])));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(field1(%[[VALUE_s2]])), widen<i64, reason=usual_arith>(const<i32>(257))), ne<f64, exceptions=observable>(read<f64>(field0(%[[VALUE_s2]])), const<f64>(176.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
