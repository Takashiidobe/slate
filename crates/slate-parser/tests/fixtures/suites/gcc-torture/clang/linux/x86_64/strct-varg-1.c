#include <stdarg.h>

void abort(void);
void exit(int);

struct s {
  int x, y;
};

void f(int attr, ...) {
  struct s va_values;
  va_list  va;
  int      i;

  va_start(va, attr);

  if (attr != 2)
    abort();

  va_values = va_arg(va, struct s);
  if (va_values.x != 0xaaaa || va_values.y != 0x5555)
    abort();

  attr = va_arg(va, int);
  if (attr != 3)
    abort();

  va_values = va_arg(va, struct s);
  if (va_values.x != 0xffff || va_values.y != 0x1111)
    abort();

  va_end(va);
}

int main(void) {
  struct s a, b;

  a.x = 0xaaaa;
  a.y = 0x5555;
  b.x = 0xffff;
  b.y = 0x1111;

  f(2, a, 3, b);
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_attr:[0-9]+]] attr: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_va_values:[0-9]+]] va_values: @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_va:[0-9]+]] va: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_va]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_attr]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_va_values]], copy<@type[[TYPE_s]], reason=assign>(va_arg<@type[[TYPE_s]]>(%[[VALUE_va]])));
// DEFAULT-NEXT:         copy<@type[[TYPE_s]], reason=assign>(va_arg<@type[[TYPE_s]]>(%[[VALUE_va]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field0(%[[VALUE_va_values]])), const<i32>(43690)), ne<i32>(read<i32>(field1(%[[VALUE_va_values]])), const<i32>(21845)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_attr]], va_arg<i32>(%[[VALUE_va]]));
// DEFAULT-NEXT:         va_arg<i32>(%[[VALUE_va]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_attr]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_va_values]], copy<@type[[TYPE_s]], reason=assign>(va_arg<@type[[TYPE_s]]>(%[[VALUE_va]])));
// DEFAULT-NEXT:         copy<@type[[TYPE_s]], reason=assign>(va_arg<@type[[TYPE_s]]>(%[[VALUE_va]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(field0(%[[VALUE_va_values]])), const<i32>(65535)), ne<i32>(read<i32>(field1(%[[VALUE_va_values]])), const<i32>(4369)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_va]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_s]] [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_a]]), const<i32>(43690));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_a]]), const<i32>(21845));
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_b]]), const<i32>(65535));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_b]]), const<i32>(4369));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, scalar, native_c) -> void>(%[[VALUE_f]], const<i32>(2), copy<@type[[TYPE_s]], reason=vararg>(read<@type[[TYPE_s]]>(%[[VALUE_a]])), const<i32>(3), copy<@type[[TYPE_s]], reason=vararg>(read<@type[[TYPE_s]]>(%[[VALUE_b]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
