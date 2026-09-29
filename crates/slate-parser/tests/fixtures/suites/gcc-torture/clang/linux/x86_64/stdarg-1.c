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
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     global %[[VALUE_foo_arg:[0-9]+]] foo_arg: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_bar_arg:[0-9]+]] bar_arg: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_gap:[0-9]+]] gap: va_list [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pap:[0-9]+]] pap: ptr<va_list> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_v:[0-9]+]] v: i32, %[[VALUE_ap:[0-9]+]] ap: va_list) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] read<i32>(%[[VALUE_v]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(5):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_foo_arg]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:                     va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 default %[[VALUE0]]:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_v_2:[0-9]+]] v: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_v_2]]), const<i32>(16390))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(va_arg<f64>(%[[VALUE_gap]]), const<f64>(17.0))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE1]], ne<i64>(va_arg<i64>(%[[VALUE_gap]]), const<i64>(129)));
// DEFAULT-NEXT:                 if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%[[VALUE_v_2]]), const<i32>(16392))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                     if ne<i64>(va_arg<i64>(deref(read<ptr<va_list>>(%[[VALUE_pap]]))), const<i64>(14))
// DEFAULT-NEXT:                         write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%[[VALUE2]], ne<f80, exceptions=ignore>(va_arg<f80>(deref(read<ptr<va_list>>(%[[VALUE_pap]]))), const<f80>(131)));
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                     if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:                         write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         write<bool>(%[[VALUE3]], ne<i32>(va_arg<i32>(deref(read<ptr<va_list>>(%[[VALUE_pap]]))), const<i32>(17)));
// DEFAULT-NEXT:                     if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<i32>(%[[VALUE_bar_arg]], read<i32>(%[[VALUE_v_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f0:[0-9]+]] @f0(%[[VALUE_i:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_i_2:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_i_3:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_3:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f64>(%[[VALUE_d]])));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_x]], va_arg<i64>(%[[VALUE_ap_3]]));
// DEFAULT-NEXT:         va_arg<i64>(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], truncate<i32, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_i_4:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_4:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         write<f64>(%[[VALUE_d]], va_arg<f64>(%[[VALUE_ap_4]]));
// DEFAULT-NEXT:         va_arg<f64>(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_i_5:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_5:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         write<i64>(%[[VALUE_x]], float_to_int<i64, reason=assign, out_of_range=ub, exceptions=ignore>(va_arg<f64>(%[[VALUE_ap_5]])));
// DEFAULT-NEXT:         float_to_int<i64, reason=assign, out_of_range=ub, exceptions=ignore>(va_arg<f64>(%[[VALUE_ap_5]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32, va_list) -> void>(%[[VALUE_foo]], read<i32>(%[[VALUE_i_5]]), read<va_list>(%[[VALUE_ap_5]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_i_6:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_6:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         va_copy(%[[VALUE_gap]], %[[VALUE_ap_6]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], read<i32>(%[[VALUE_i_6]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_gap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE_i_7:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_7:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f64>(%[[VALUE_d]])));
// DEFAULT-NEXT:         va_arg<i64>(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         va_arg<i64>(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         write<i64>(%[[VALUE_x]], va_arg<i64>(%[[VALUE_ap_7]]));
// DEFAULT-NEXT:         va_arg<i64>(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], truncate<i32, reason=arg, fits=unknown>(read<i64>(%[[VALUE_x]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_i_8:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_8:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         write<ptr<va_list>>(%[[VALUE_pap]], addr_of<ptr<va_list>>(%[[VALUE_ap_8]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], read<i32>(%[[VALUE_i_8]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(%[[VALUE_i_9:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_9:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:         write<ptr<va_list>>(%[[VALUE_pap]], addr_of<ptr<va_list>>(%[[VALUE_ap_9]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], read<i32>(%[[VALUE_i_9]]));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_d]], va_arg<f64>(%[[VALUE_ap_9]]));
// DEFAULT-NEXT:         va_arg<f64>(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f0]], const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f1]], const<i32>(2));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_d]], const<f64>(31.0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f2]], const<i32>(3), const<i64>(28));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_bar_arg]]), const<i32>(28)), ne<i64>(read<i64>(%[[VALUE_x]]), widen<i64, reason=usual_arith>(const<i32>(28))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f3]], const<i32>(4), const<f64>(131.0));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_d]]), const<f64>(131.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f4]], const<i32>(5), const<f64>(16.0), const<i32>(128));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(%[[VALUE_x]]), widen<i64, reason=usual_arith>(const<i32>(16))), ne<i32>(read<i32>(%[[VALUE_foo_arg]]), const<i32>(128)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f5]], const<i32>(16390), const<f64>(17.0), const<i64>(129));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_bar_arg]]), const<i32>(16390))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f6]], const<i32>(7), const<i64>(12), const<i64>(14), neg<i64, overflow=ub>(const<i64>(31)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_bar_arg]]), neg<i32, overflow=ub>(const<i32>(31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f7]], const<i32>(16392), const<i64>(14), const<f80>(131), const<i32>(17), const<f64>(26.0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_bar_arg]]), const<i32>(16392))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f8]], const<i32>(16392), const<i64>(14), const<f80>(131), const<i32>(17), const<f64>(27.0));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_bar_arg]]), const<i32>(16392)), ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_d]]), const<f64>(27.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
