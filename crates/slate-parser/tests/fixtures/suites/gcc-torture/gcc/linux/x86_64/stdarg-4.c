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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_y:[0-9]+]] y: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f1i:[0-9]+]] @f1i(%[[VALUE_ap:[0-9]+]] ap: va_list) -> void [linkage=external] [inline=always] [definition=inline_only] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%[[VALUE_x]], float_to_int<i64, reason=assign, out_of_range=ub, exceptions=observable>(va_arg<f64>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE0]]), va_arg<i64>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_x]], read<i64>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i64 [synthetic] = float_to_int<i64, reason=assign, out_of_range=ub, exceptions=observable>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i64>(%[[VALUE2]])), va_arg<f64>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_x]], read<i64>(%[[VALUE3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_i:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         call<void, signature=fn(va_list) -> void>(%[[VALUE_f1i]], read<va_list>(%[[VALUE_ap_2]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2i:[0-9]+]] @f2i(%[[VALUE_ap_3:[0-9]+]] ap: va_list) -> void [linkage=external] [inline=always] [definition=inline_only] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%[[VALUE_y]], widen<i64, reason=assign>(va_arg<i32>(%[[VALUE_ap_3]])));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_y]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE4]]), va_arg<i64>(%[[VALUE_ap_3]]));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_y]], read<i64>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_y]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i64 [synthetic] = float_to_int<i64, reason=assign, out_of_range=ub, exceptions=observable>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i64>(%[[VALUE6]])), va_arg<f64>(%[[VALUE_ap_3]])));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_y]], read<i64>(%[[VALUE7]]));
// DEFAULT-NEXT:         call<void, signature=fn(va_list) -> void>(%[[VALUE_f1i]], read<va_list>(%[[VALUE_ap_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_i_2:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_4:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         call<void, signature=fn(va_list) -> void>(%[[VALUE_f2i]], read<va_list>(%[[VALUE_ap_4]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3h:[0-9]+]] @f3h(%[[VALUE_i_3:[0-9]+]] i: i32, %[[VALUE_arg0:[0-9]+]] arg0: i64, %[[VALUE_arg1:[0-9]+]] arg1: i64, %[[VALUE_arg2:[0-9]+]] arg2: i64, %[[VALUE_arg3:[0-9]+]] arg3: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(add<i64, overflow=ub>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i_3]])), read<i64>(%[[VALUE_arg0]])), read<i64>(%[[VALUE_arg1]])), read<i64>(%[[VALUE_arg2]])), read<i64>(%[[VALUE_arg3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_i_4:[0-9]+]] i: i32, ...) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_arg0_2:[0-9]+]] arg0: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_arg1_2:[0-9]+]] arg1: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_arg2_2:[0-9]+]] arg2: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_arg3_2:[0-9]+]] arg3: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap_5:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         switch %[[VALUE8:[0-9]+]] read<i32>(%[[VALUE_i_4]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE8]] const<i32>(0):
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_t]], call<i64, signature=fn(i32, i64, i64, i64, i64) -> i64>(%[[VALUE_f3h]], read<i32>(%[[VALUE_i_4]]), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:                 break %[[VALUE8]];
// DEFAULT-NEXT:                 case %[[VALUE8]] const<i32>(1):
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_arg0_2]], va_arg<i64>(%[[VALUE_ap_5]]));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_t]], call<i64, signature=fn(i32, i64, i64, i64, i64) -> i64>(%[[VALUE_f3h]], read<i32>(%[[VALUE_i_4]]), read<i64>(%[[VALUE_arg0_2]]), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:                 break %[[VALUE8]];
// DEFAULT-NEXT:                 case %[[VALUE8]] const<i32>(2):
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_arg0_2]], va_arg<i64>(%[[VALUE_ap_5]]));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_arg1_2]], va_arg<i64>(%[[VALUE_ap_5]]));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_t]], call<i64, signature=fn(i32, i64, i64, i64, i64) -> i64>(%[[VALUE_f3h]], read<i32>(%[[VALUE_i_4]]), read<i64>(%[[VALUE_arg0_2]]), read<i64>(%[[VALUE_arg1_2]]), widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:                 break %[[VALUE8]];
// DEFAULT-NEXT:                 case %[[VALUE8]] const<i32>(3):
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_arg0_2]], va_arg<i64>(%[[VALUE_ap_5]]));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_arg1_2]], va_arg<i64>(%[[VALUE_ap_5]]));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_arg2_2]], va_arg<i64>(%[[VALUE_ap_5]]));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_t]], call<i64, signature=fn(i32, i64, i64, i64, i64) -> i64>(%[[VALUE_f3h]], read<i32>(%[[VALUE_i_4]]), read<i64>(%[[VALUE_arg0_2]]), read<i64>(%[[VALUE_arg1_2]]), read<i64>(%[[VALUE_arg2_2]]), widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:                 break %[[VALUE8]];
// DEFAULT-NEXT:                 case %[[VALUE8]] const<i32>(4):
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_arg0_2]], va_arg<i64>(%[[VALUE_ap_5]]));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_arg1_2]], va_arg<i64>(%[[VALUE_ap_5]]));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_arg2_2]], va_arg<i64>(%[[VALUE_ap_5]]));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_arg3_2]], va_arg<i64>(%[[VALUE_ap_5]]));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_t]], call<i64, signature=fn(i32, i64, i64, i64, i64) -> i64>(%[[VALUE_f3h]], read<i32>(%[[VALUE_i_4]]), read<i64>(%[[VALUE_arg0_2]]), read<i64>(%[[VALUE_arg1_2]]), read<i64>(%[[VALUE_arg2_2]]), read<i64>(%[[VALUE_arg3_2]])));
// DEFAULT-NEXT:                 break %[[VALUE8]];
// DEFAULT-NEXT:                 default %[[VALUE8]]:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_t]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_i_5:[0-9]+]] i: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_6:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         switch %[[VALUE9:[0-9]+]] read<i32>(%[[VALUE_i_5]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE9]] const<i32>(4):
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_y]], float_to_int<i64, reason=assign, out_of_range=ub, exceptions=observable>(va_arg<f64>(%[[VALUE_ap_6]])));
// DEFAULT-NEXT:                 break %[[VALUE9]];
// DEFAULT-NEXT:                 case %[[VALUE9]] const<i32>(5):
// DEFAULT-NEXT:                     write<i64>(%[[VALUE_y]], float_to_int<i64, reason=assign, out_of_range=ub, exceptions=observable>(va_arg<f64>(%[[VALUE_ap_6]])));
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_y]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i64 [synthetic] = float_to_int<i64, reason=assign, out_of_range=ub, exceptions=observable>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i64>(%[[VALUE10]])), va_arg<f64>(%[[VALUE_ap_6]])));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_y]], read<i64>(%[[VALUE11]]));
// DEFAULT-NEXT:                 break %[[VALUE9]];
// DEFAULT-NEXT:                 default %[[VALUE9]]:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(va_list) -> void>(%[[VALUE_f1i]], read<va_list>(%[[VALUE_ap_6]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f1]], const<i32>(3), const<f64>(16.0), const<i64>(128), const<f64>(32.0));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_x]]), const<i64>(176))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f2]], const<i32>(6), const<i32>(5), const<i64>(7), const<f64>(18.0), const<f64>(19.0), const<i64>(17), const<f64>(64.0));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(%[[VALUE_x]]), const<i64>(100)), ne<i64>(read<i64>(%[[VALUE_y]]), const<i64>(30)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i32, ...) -> i64>(%[[VALUE_f3]], const<i32>(0)), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i32, ...) -> i64>(%[[VALUE_f3]], const<i32>(1), const<i64>(18)), const<i64>(19))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i32, ...) -> i64>(%[[VALUE_f3]], const<i32>(2), const<i64>(18), const<i64>(100)), const<i64>(120))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i32, ...) -> i64>(%[[VALUE_f3]], const<i32>(3), const<i64>(18), const<i64>(100), const<i64>(300)), const<i64>(421))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i32, ...) -> i64>(%[[VALUE_f3]], const<i32>(4), const<i64>(18), const<i64>(71), const<i64>(64), const<i64>(86)), const<i64>(243))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f4]], const<i32>(4), const<f64>(6.0), const<f64>(9.0), const<i64>(16), const<f64>(18.0));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(%[[VALUE_x]]), const<i64>(43)), ne<i64>(read<i64>(%[[VALUE_y]]), const<i64>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f4]], const<i32>(5), const<f64>(7.0), const<f64>(21.0), const<f64>(1.0), const<i64>(17), const<f64>(126.0));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(read<i64>(%[[VALUE_x]]), const<i64>(144)), ne<i64>(read<i64>(%[[VALUE_y]]), const<i64>(28)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
