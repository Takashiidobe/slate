#include <stdarg.h>

void abort(void);
void exit(int);

va_list global;

void vat(va_list param, ...) {
  va_list local;

  va_start(local, param);
  va_copy(global, local);
  va_copy(param, local);
  if (va_arg(local, int) != 1)
    abort();
  va_end(local);
  if (va_arg(global, int) != 1)
    abort();
  va_end(global);
  if (va_arg(param, int) != 1)
    abort();
  va_end(param);

  va_start(param, param);
  va_start(global, param);
  va_copy(local, param);
  if (va_arg(local, int) != 1)
    abort();
  va_end(local);
  va_copy(local, global);
  if (va_arg(local, int) != 1)
    abort();
  va_end(local);
  if (va_arg(global, int) != 1)
    abort();
  va_end(global);
  if (va_arg(param, int) != 1)
    abort();
  va_end(param);
}

int main(void) {
  va_list t;
  vat(t, 1);
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
// DEFAULT-NEXT:     global %[[VALUE_global:[0-9]+]] global: va_list [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_vat:[0-9]+]] @vat(%[[VALUE_param:[0-9]+]] param: va_list, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_local:[0-9]+]] local: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_local]]);
// DEFAULT-NEXT:         va_copy(%[[VALUE_global]], %[[VALUE_local]]);
// DEFAULT-NEXT:         va_copy(%[[VALUE_param]], %[[VALUE_local]]);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%[[VALUE_local]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_local]]);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%[[VALUE_global]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_global]]);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%[[VALUE_param]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_param]]);
// DEFAULT-NEXT:         va_start(%[[VALUE_param]]);
// DEFAULT-NEXT:         va_start(%[[VALUE_global]]);
// DEFAULT-NEXT:         va_copy(%[[VALUE_local]], %[[VALUE_param]]);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%[[VALUE_local]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_local]]);
// DEFAULT-NEXT:         va_copy(%[[VALUE_local]], %[[VALUE_global]]);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%[[VALUE_local]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_local]]);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%[[VALUE_global]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_global]]);
// DEFAULT-NEXT:         if ne<i32>(va_arg<i32>(%[[VALUE_param]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_param]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: va_list [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(va_list, ...) -> void>(%[[VALUE_vat]], read<va_list>(%[[VALUE_t]]), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
