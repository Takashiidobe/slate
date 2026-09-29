#include "pruning-attribute-roots/pruning-attribute-roots.h"

int main(void) {
  int value __attribute__((cleanup(cleanup_fn))) = 0;
  alias_entry();
  return value;
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
// DEFAULT-NEXT:     fn %[[VALUE_alias_target:[0-9]+]] @alias_target() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_alias_entry:[0-9]+]] @alias_entry() -> void [linkage=external] [alias="alias_target"];
// DEFAULT-NEXT:     fn %[[VALUE_cleanup_fn:[0-9]+]] @cleanup_fn(%[[VALUE_value:[0-9]+]] value: ptr<i32>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         read<ptr<i32>>(%[[VALUE_value]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_startup_fn:[0-9]+]] @startup_fn() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_shutdown_fn:[0-9]+]] @shutdown_fn() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_used_fn:[0-9]+]] @used_fn() -> void [linkage=internal] [used] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retained_fn:[0-9]+]] @retained_fn() -> void [linkage=internal] [retain] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_value_2:[0-9]+]] value: i32 [storage=automatic] [cleanup=cleanup_fn] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_alias_entry]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_value_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
