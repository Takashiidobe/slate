#include <stdarg.h>

// SLATE-FILECHECK-DEFINES DEFAULT

void abort(void);
void exit(int);

struct tiny {
  int c;
};

void f(int n, ...) {
  struct tiny x;
  int         i;

  va_list ap;
  va_start(ap, n);
  for (i = 0; i < n; i++) {
    x = va_arg(ap, struct tiny);
    if (x.c != i + 10)
      abort();
  }
  {
    long x = va_arg(ap, long);
    if (x != 123)
      abort();
  }
  va_end(ap);
}

int main(void) {
  struct tiny x[3];
  x[0].c = 10;
  x[1].c = 11;
  x[2].c = 12;
  f(3, x[0], x[1], x[2], (long)123);
  exit(0);
}

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
// DEFAULT-NEXT:     type @type[[TYPE_tiny:[0-9]+]] tiny = struct {
// DEFAULT-NEXT:         field0 c: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_n:[0-9]+]] n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: @type[[TYPE_tiny]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_n]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<@type[[TYPE_tiny]]>(%[[VALUE_x]], copy<@type[[TYPE_tiny]], reason=assign>(va_arg<@type[[TYPE_tiny]]>(%[[VALUE_ap]])));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(field0(%[[VALUE_x]])), add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(10)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_x_2:[0-9]+]] x: i64 [storage=automatic] = va_arg<i64>(%[[VALUE_ap]]);
// DEFAULT-NEXT:             if ne<i64>(read<i64>(%[[VALUE_x_2]]), widen<i64, reason=usual_arith>(const<i32>(123)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_3:[0-9]+]] x: array<@type[[TYPE_tiny]], 3> [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE_tiny]]>, subtract=false, element=@type[[TYPE_tiny]], overflow=ub>(array_decay<ptr<@type[[TYPE_tiny]]>, length=Some(3)>(%[[VALUE_x_3]]), const<i32>(0)))), const<i32>(10));
// DEFAULT-NEXT:         write<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE_tiny]]>, subtract=false, element=@type[[TYPE_tiny]], overflow=ub>(array_decay<ptr<@type[[TYPE_tiny]]>, length=Some(3)>(%[[VALUE_x_3]]), const<i32>(1)))), const<i32>(11));
// DEFAULT-NEXT:         write<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE_tiny]]>, subtract=false, element=@type[[TYPE_tiny]], overflow=ub>(array_decay<ptr<@type[[TYPE_tiny]]>, length=Some(3)>(%[[VALUE_x_3]]), const<i32>(2)))), const<i32>(12));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c, scalar) -> void>(%[[VALUE_f]], const<i32>(3), copy<@type[[TYPE_tiny]], reason=vararg>(read<@type[[TYPE_tiny]]>(deref(ptr_offset<ptr<@type[[TYPE_tiny]]>, subtract=false, element=@type[[TYPE_tiny]], overflow=ub>(array_decay<ptr<@type[[TYPE_tiny]]>, length=Some(3)>(%[[VALUE_x_3]]), const<i32>(0))))), copy<@type[[TYPE_tiny]], reason=vararg>(read<@type[[TYPE_tiny]]>(deref(ptr_offset<ptr<@type[[TYPE_tiny]]>, subtract=false, element=@type[[TYPE_tiny]], overflow=ub>(array_decay<ptr<@type[[TYPE_tiny]]>, length=Some(3)>(%[[VALUE_x_3]]), const<i32>(1))))), copy<@type[[TYPE_tiny]], reason=vararg>(read<@type[[TYPE_tiny]]>(deref(ptr_offset<ptr<@type[[TYPE_tiny]]>, subtract=false, element=@type[[TYPE_tiny]], overflow=ub>(array_decay<ptr<@type[[TYPE_tiny]]>, length=Some(3)>(%[[VALUE_x_3]]), const<i32>(2))))), widen<i64, reason=explicit>(const<i32>(123)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
