/* This used to segfault on SPARC 64-bit at runtime because
   the stack pointer was clobbered by the function call.   */

/* { dg-do run } */

#include <stdarg.h>

union U
{
  long l1[2];
};

union U u;

void foo (int z, ...)
{
  int i;
  va_list ap;
  va_start(ap,z);
  i = va_arg(ap, int);
  va_end(ap);
}

int main(void)
{
  foo (1, 1, 1, 1, 1, u);
  return 0;
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 l1: array<i64, 2>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: @type[[TYPE_U]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_z:[0-9]+]] z: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_foo]], const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), const<i32>(1), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(%[[VALUE_u]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
