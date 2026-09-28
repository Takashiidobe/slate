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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     type @type2 two = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:         field1 y: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @foo(%5 a: i32, %6 b: i32, %7 c: i32, %8 d: i32, %9 e: i32, %10 f: @type2, %11 g: i32, ...) -> void [linkage=external] [abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 args: va_list [storage=automatic];
// DEFAULT-NEXT:         let %13 h: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%12);
// DEFAULT-NEXT:         write<i32>(%13, va_arg<i32>(%12));
// DEFAULT-NEXT:         va_arg<i32>(%12);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%11), const<i32>(1)), ne<i32>(read<i32>(%13), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15 t: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(0)), field1 = widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, @type2, i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c, scalar, scalar) -> void>(%4, const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=arg>(read<@type2>(%15)), const<i32>(1), const<i32>(2));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
