/* PR 9700 */
/* Alpha got the base address for the va_list incorrect when there was
   a structure that was passed partially in registers and partially on
   the stack.  */

#include <stdarg.h>

void abort(void);

struct two {
  long x, y;
};

void foo(int a, int b, int c, int d, int e, struct two f, int g, ...) {
  va_list args;
  int     h;

  va_start(args, g);
  h = va_arg(args, int);
  if (g != 1 || h != 2)
    abort();
}

int main() {
  struct two t = {0, 0};
  foo(0, 0, 0, 0, 0, t, 1, 2);
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
// DEFAULT-NEXT:     type @type1 two = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:         field1 y: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @foo(%4 a: i32, %5 b: i32, %6 c: i32, %7 d: i32, %8 e: i32, %9 f: @type1, %10 g: i32, ...) -> void [linkage=external] [abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 args: va_list [storage=automatic];
// DEFAULT-NEXT:         let %12 h: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%11);
// DEFAULT-NEXT:         write<i32>(%12, va_arg<i32>(%11));
// DEFAULT-NEXT:         va_arg<i32>(%11);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%10), const<i32>(1)), ne<i32>(read<i32>(%12), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 t: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(0)), field1 = widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, @type1, i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c, scalar, scalar) -> void>(%3, const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=arg>(read<@type1>(%14)), const<i32>(1), const<i32>(2));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
