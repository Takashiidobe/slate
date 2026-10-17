#include <stdarg.h>

int i = 9;

void *take(int n, ...) {
  va_list ap;
  va_start(ap, n);
  void *p = va_arg(ap, typeof(int (*)[++i]));
  va_end(ap);
  return p;
}

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT

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
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] = const<i32>(9) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_take:[0-9]+]] @take(%[[VALUE_n:[0-9]+]] n: i32, ...) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = reinterpret<u64>(widen<i64>(read<i32>(%[[VALUE1]])));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_p]], pointer_cast<ptr<void>>(va_arg<ptr<vla<i32, %[[VALUE2]]>>>(%[[VALUE_ap]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:         return read<ptr<void>>(%[[VALUE_p]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
