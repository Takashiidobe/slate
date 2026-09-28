#include <stdio.h>
#include <stdlib.h>

static int touch_and_maybe_free(int *q, int do_free) {
  *q    = *q + 1;
  int v = *q;
  if (do_free) {
    free(q);
  }
  return v;
}

int main(void) {
  int  stack_val  = 10;
  int *stack_ptr  = &stack_val;
  int  from_stack = touch_and_maybe_free(stack_ptr, 0);

  int *heap_ptr = malloc(sizeof(int));
  *heap_ptr     = 100;
  int from_heap = touch_and_maybe_free(heap_ptr, 1);

  printf("%d %d %d\n", stack_val, from_stack, from_heap);
  return stack_val + from_stack + from_heap;
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %17 .str17: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%14 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @malloc(%15 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @free(%16 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @touch_and_maybe_free(%5 q: ptr<i32>, %6 do_free: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%5)), add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%5))), const<i32>(1)));
// DEFAULT-NEXT:         let %7 v: i32 [storage=automatic] = read<i32>(deref(read<ptr<i32>>(%5)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%3, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%5)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 stack_val: i32 [storage=automatic] = const<i32>(10);
// DEFAULT-NEXT:         let %10 stack_ptr: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%9);
// DEFAULT-NEXT:         let %11 from_stack: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>, i32) -> i32>(%4, read<ptr<i32>>(%10), const<i32>(0));
// DEFAULT-NEXT:         let %12 heap_ptr: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%2, const<u64>(4)));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%12)), const<i32>(100));
// DEFAULT-NEXT:         let %13 from_heap: i32 [storage=automatic] = call<i32, signature=fn(ptr<i32>, i32) -> i32>(%4, read<ptr<i32>>(%12), const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%17)), read<i32>(%9), read<i32>(%11), read<i32>(%13));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%9), read<i32>(%11)), read<i32>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
