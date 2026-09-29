#include <stdio.h>

typedef int (*Callback)(int);

static int add_one(int x) { return x + 1; }

static void *store_fn(void *fn) { return fn; }

int main(void) {
  void    *slot = (void *)add_one;
  Callback cb   = (Callback)slot;

  void    *slot2 = store_fn((void *)add_one);
  Callback cb2   = (Callback)slot2;

  printf("%d %d\n", cb(41), cb2(99));
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
// DEFAULT-NEXT:     type @type[[TYPE_Callback:[0-9]+]] Callback = ptr<fn(i32) -> i32>;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_add_one:[0-9]+]] @add_one(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_x]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_store_fn:[0-9]+]] @store_fn(%[[VALUE_fn:[0-9]+]] fn: ptr<void>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<void>>(%[[VALUE_fn]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_slot:[0-9]+]] slot: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_add_one]]));
// DEFAULT-NEXT:         let %[[VALUE_cb:[0-9]+]] cb: ptr<fn(i32) -> i32> [storage=automatic] = pointer_cast<ptr<fn(i32) -> i32>, reason=explicit>(read<ptr<void>>(%[[VALUE_slot]]));
// DEFAULT-NEXT:         let %[[VALUE_slot2:[0-9]+]] slot2: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>) -> ptr<void>>(%[[VALUE_store_fn]], pointer_cast<ptr<void>, reason=explicit>(function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_add_one]])));
// DEFAULT-NEXT:         let %[[VALUE_cb2:[0-9]+]] cb2: ptr<fn(i32) -> i32> [storage=automatic] = pointer_cast<ptr<fn(i32) -> i32>, reason=explicit>(read<ptr<void>>(%[[VALUE_slot2]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]])), call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(%[[VALUE_cb]]), const<i32>(41)), call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(%[[VALUE_cb2]]), const<i32>(99)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
