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
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 g: va_list;
// DEFAULT-NEXT:         field2 h: array<va_list, 2>;
// DEFAULT-NEXT:     } [size=80, align=8, offsets=[0, 8, 32]];
// DEFAULT-NEXT:     global %[[VALUE_foo_arg:[0-9]+]] foo_arg: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_bar_arg:[0-9]+]] bar_arg: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_gap:[0-9]+]] gap: va_list [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_v:[0-9]+]] v: i32, %[[VALUE_ap:[0-9]+]] ap: va_list) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] read<i32>(%[[VALUE_v]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(5):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_foo_arg]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_foo_arg]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE1]])), va_arg<f64>(%[[VALUE_ap]])));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_foo_arg]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_foo_arg]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = truncate<i32, reason=assign, fits=unknown>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE3]])), va_arg<i64>(%[[VALUE_ap]])));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_foo_arg]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(8):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_foo_arg]], truncate<i32, reason=assign, fits=unknown>(va_arg<i64>(%[[VALUE_ap]])));
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_foo_arg]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE5]])), va_arg<f64>(%[[VALUE_ap]])));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_foo_arg]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(11):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_foo_arg]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_foo_arg]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE7]])), va_arg<f80>(%[[VALUE_ap]])));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_foo_arg]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 default %[[VALUE0]]:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_v_2:[0-9]+]] v: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_v_2]]), const<i32>(16386))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<i32>(va_arg<i32>(%[[VALUE_gap]]), const<i32>(13))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE9]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE9]], ne<f64, exceptions=ignore>(va_arg<f64>(%[[VALUE_gap]]), neg<f64>(const<f64>(14.0))));
// DEFAULT-NEXT:                 if read<bool>(%[[VALUE9]])
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i32>(%[[VALUE_bar_arg]], read<i32>(%[[VALUE_v_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_i:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         va_start(%[[VALUE_gap]]);
// DEFAULT-NEXT:         write<i64>(%[[VALUE_x]], va_arg<i64>(%[[VALUE_gap]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_gap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_i_2:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         va_start(%[[VALUE_gap]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], read<i32>(%[[VALUE_i_2]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_gap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_i_3:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_aps:[0-9]+]] aps: array<va_list, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         va_start(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%[[VALUE_aps]]), const<i32>(4))));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_x]], va_arg<i64>(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%[[VALUE_aps]]), const<i32>(4)))));
// DEFAULT-NEXT:         va_end(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%[[VALUE_aps]]), const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_i_4:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_aps_2:[0-9]+]] aps: array<va_list, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         va_start(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%[[VALUE_aps_2]]), const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], read<i32>(%[[VALUE_i_4]]));
// DEFAULT-NEXT:         va_end(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%[[VALUE_aps_2]]), const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_i_5:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_aps_3:[0-9]+]] aps: array<va_list, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         va_start(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%[[VALUE_aps_3]]), const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, va_list) -> void>(%[[VALUE_foo]], read<i32>(%[[VALUE_i_5]]), read<va_list>(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%[[VALUE_aps_3]]), const<i32>(4)))));
// DEFAULT-NEXT:         va_end(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(10)>(%[[VALUE_aps_3]]), const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE_i_6:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         va_start(field1(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_x]], va_arg<i64>(field1(%[[VALUE_a]])));
// DEFAULT-NEXT:         va_end(field1(%[[VALUE_a]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_i_7:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         va_start(field1(%[[VALUE_a_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], read<i32>(%[[VALUE_i_7]]));
// DEFAULT-NEXT:         va_end(field1(%[[VALUE_a_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(%[[VALUE_i_8:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_3:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         va_start(field1(%[[VALUE_a_3]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32, va_list) -> void>(%[[VALUE_foo]], read<i32>(%[[VALUE_i_8]]), read<va_list>(field1(%[[VALUE_a_3]])));
// DEFAULT-NEXT:         va_end(field1(%[[VALUE_a_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f10:[0-9]+]] @f10(%[[VALUE_i_9:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_4:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         va_start(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%[[VALUE_a_4]])), const<i32>(1))));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_x]], va_arg<i64>(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%[[VALUE_a_4]])), const<i32>(1)))));
// DEFAULT-NEXT:         va_end(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%[[VALUE_a_4]])), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f11:[0-9]+]] @f11(%[[VALUE_i_10:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_5:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         va_start(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%[[VALUE_a_5]])), const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], read<i32>(%[[VALUE_i_10]]));
// DEFAULT-NEXT:         va_end(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%[[VALUE_a_5]])), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f12:[0-9]+]] @f12(%[[VALUE_i_11:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_6:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         va_start(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%[[VALUE_a_6]])), const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, va_list) -> void>(%[[VALUE_foo]], read<i32>(%[[VALUE_i_11]]), read<va_list>(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%[[VALUE_a_6]])), const<i32>(1)))));
// DEFAULT-NEXT:         va_end(deref(ptr_offset<ptr<va_list>, subtract=false, element=va_list, overflow=ub>(array_decay<ptr<va_list>, length=Some(2)>(field2(%[[VALUE_a_6]])), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f1]], const<i32>(1), const<i64>(79));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_x]]), const<i64>(79))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f2]], const<i32>(16386), const<i32>(13), neg<f64>(const<f64>(14.0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_bar_arg]]), const<i32>(16386))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f3]], const<i32>(3), const<i64>(2031));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_x]]), widen<i64, reason=usual_arith>(const<i32>(2031)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f4]], const<i32>(4), const<i32>(18));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_bar_arg]]), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f5]], const<i32>(5), const<i32>(1), const<f64>(19.0), const<i64>(18));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_foo_arg]]), const<i32>(38))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f6]], const<i32>(6), const<i64>(18));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_x]]), const<i64>(18))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f7]], const<i32>(7));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_bar_arg]]), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f8]], const<i32>(8), const<i64>(2031), const<f64>(13.0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_foo_arg]]), const<i32>(2044))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f10]], const<i32>(9), const<i64>(180));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_x]]), const<i64>(180))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f11]], const<i32>(10));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_bar_arg]]), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f12]], const<i32>(11), const<i32>(2030), const<f80>(12));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_foo_arg]]), const<i32>(2042))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
