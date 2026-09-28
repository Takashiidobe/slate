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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 A: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 T = @type2;
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @exit(%14 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @f(%7 x: i32, ...) -> @type2 [linkage=external] [abi=sysv64(scalar) -> coerce<i32>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %9 X: @type2 [storage=automatic];
// DEFAULT-NEXT:         va_start(%8);
// DEFAULT-NEXT:         write<@type2>(%9, copy<@type2, reason=assign>(va_arg<@type2>(%8)));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(va_arg<@type2>(%8));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(%9)), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         write<@type2>(%9, copy<@type2, reason=assign>(va_arg<@type2>(%8)));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(va_arg<@type2>(%8));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(%9)), const<i32>(20))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         va_end(%8);
// DEFAULT-NEXT:         return copy<@type2, reason=return>(read<@type2>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 X: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %12 Y: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %13 i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%11), const<i32>(10));
// DEFAULT-NEXT:         write<i32>(field0(%12), const<i32>(20));
// DEFAULT-NEXT:         call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, coerce<i32>, coerce<i32>) -> coerce<i32>>(%6, const<i32>(2), copy<@type2, reason=vararg>(read<@type2>(%11)), copy<@type2, reason=vararg>(read<@type2>(%12)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
