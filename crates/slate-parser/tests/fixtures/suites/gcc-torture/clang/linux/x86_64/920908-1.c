/* REPRODUCED:RUN:SIGNAL MACHINE:mips OPTIONS: */

#include <stdarg.h>

void abort(void);
void exit(int);

typedef struct {
  int A;
} T;

T f(int x, ...) {
  va_list ap;
  T       X;
  va_start(ap, x);
  X = va_arg(ap, T);
  if (X.A != 10)
    abort();
  X = va_arg(ap, T);
  if (X.A != 20)
    abort();
  va_end(ap);
  return X;
}

int main(void) {
  T   X, Y;
  int i;
  X.A = 10;
  Y.A = 20;
  f(2, X, Y);
  exit(0);
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 A: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_x:[0-9]+]] x: i32, ...) -> @type[[TYPE0]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_X:[0-9]+]] X: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(%[[VALUE_X]], copy<@type[[TYPE0]], reason=assign>(va_arg<@type[[TYPE0]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         copy<@type[[TYPE0]], reason=assign>(va_arg<@type[[TYPE0]]>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(%[[VALUE_X]])), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(%[[VALUE_X]], copy<@type[[TYPE0]], reason=assign>(va_arg<@type[[TYPE0]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         copy<@type[[TYPE0]], reason=assign>(va_arg<@type[[TYPE0]]>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(%[[VALUE_X]])), const<i32>(20))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:         return copy<@type[[TYPE0]], reason=return>(read<@type[[TYPE0]]>(%[[VALUE_X]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_X_2:[0-9]+]] X: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_Y:[0-9]+]] Y: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_X_2]]), const<i32>(10));
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_Y]]), const<i32>(20));
// DEFAULT-NEXT:         call<@type[[TYPE0]], signature=fn(i32, ...) -> @type[[TYPE0]], abi=sysv64(scalar, native_c, native_c) -> native_c>(%[[VALUE_f]], const<i32>(2), copy<@type[[TYPE0]], reason=vararg>(read<@type[[TYPE0]]>(%[[VALUE_X_2]])), copy<@type[[TYPE0]], reason=vararg>(read<@type[[TYPE0]]>(%[[VALUE_Y]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
