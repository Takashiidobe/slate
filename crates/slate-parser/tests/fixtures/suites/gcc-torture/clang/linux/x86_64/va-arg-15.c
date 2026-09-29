#include <stdarg.h>

void abort(void);
void exit(int);

void vafunction(char *dummy, ...) {
  double  darg;
  int     iarg;
  int     flag = 0;
  int     i;
  va_list ap;

  va_start(ap, dummy);
  for (i = 1; i <= 18; i++, flag++) {
    if (flag & 1) {
      darg = va_arg(ap, double);
      if (darg != (double)i)
        abort();
    } else {
      iarg = va_arg(ap, int);
      if (iarg != i)
        abort();
    }
  }
  va_end(ap);
}

int main(void) {
  vafunction("", 1, 2., 3, 4., 5, 6., 7, 8., 9, 10., 11, 12., 13, 14., 15, 16.,
             17, 18.);
  exit(0);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_vafunction:[0-9]+]] @vafunction(%[[VALUE_dummy:[0-9]+]] dummy: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_darg:[0-9]+]] darg: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_iarg:[0-9]+]] iarg: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_flag:[0-9]+]] flag: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(1));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(18))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_flag]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_flag]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(and<i32>(read<i32>(%[[VALUE_flag]]), const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<f64>(%[[VALUE_darg]], va_arg<f64>(%[[VALUE_ap]]));
// DEFAULT-NEXT:                             va_arg<f64>(%[[VALUE_ap]]);
// DEFAULT-NEXT:                             if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_darg]]), int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE_i]])))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_iarg]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:                             va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:                             if ne<i32>(read<i32>(%[[VALUE_iarg]]), read<i32>(%[[VALUE_i]]))
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ...) -> void>(%[[VALUE_vafunction]], array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]]), const<i32>(1), const<f64>(2.0), const<i32>(3), const<f64>(4.0), const<i32>(5), const<f64>(6.0), const<i32>(7), const<f64>(8.0), const<i32>(9), const<f64>(10.0), const<i32>(11), const<f64>(12.0), const<i32>(13), const<f64>(14.0), const<i32>(15), const<f64>(16.0), const<i32>(17), const<f64>(18.0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
