#include <stdlib.h>

typedef void *(*malloc_fn_t)(size_t);

struct hooks {
  malloc_fn_t malloc_fn;
};

int main(void) {
  struct hooks h;
  h.malloc_fn = malloc;
  int matches = (h.malloc_fn == malloc);
  return matches ? 0 : 1;
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_malloc_fn_t:[0-9]+]] malloc_fn_t = ptr<fn(u64) -> ptr<void>>;
// DEFAULT-NEXT:     type @type[[TYPE_hooks:[0-9]+]] hooks = struct {
// DEFAULT-NEXT:         field0 malloc_fn: ptr<fn(u64) -> ptr<void>>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: @type[[TYPE_hooks]] [storage=automatic];
// DEFAULT-NEXT:         write<ptr<fn(u64) -> ptr<void>>>(field0(%[[VALUE_h]]), function_decay<ptr<fn(u64) -> ptr<void>>>(%[[VALUE_malloc]]));
// DEFAULT-NEXT:         let %[[VALUE_matches:[0-9]+]] matches: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<ptr<fn(u64) -> ptr<void>>>(read<ptr<fn(u64) -> ptr<void>>>(field0(%[[VALUE_h]])), function_decay<ptr<fn(u64) -> ptr<void>>>(%[[VALUE_malloc]])));
// DEFAULT-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%[[VALUE_matches]]), const<i32>(0)), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
