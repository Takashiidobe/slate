#include <stdio.h>

typedef int *pointer_returning_fn(int *);

struct Callback {
  pointer_returning_fn *handler;
};

static int *add_one(int *value) {
  *value += 1;
  return value;
}

int main(void) {
  int             value    = 41;
  struct Callback callback = {0};
  callback.handler         = add_one;
  printf("%d\n", *callback.handler(&value));
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
// DEFAULT-NEXT:     type @type0 pointer_returning_fn = fn(ptr<i32>) -> ptr<i32>;
// DEFAULT-NEXT:     type @type1 Callback = struct {
// DEFAULT-NEXT:         field0 handler: ptr<fn(ptr<i32>) -> ptr<i32>>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%9 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @add_one(%5 value: ptr<i32>) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11: ptr<i32> [synthetic] = read<ptr<i32>>(%5);
// DEFAULT-NEXT:         let %12: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%11)));
// DEFAULT-NEXT:         let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%11)), read<i32>(%13));
// DEFAULT-NEXT:         return read<ptr<i32>>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 value: i32 [storage=automatic] = const<i32>(41);
// DEFAULT-NEXT:         let %8 callback: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = null<ptr<fn(ptr<i32>) -> ptr<i32>>>);
// DEFAULT-NEXT:         write<ptr<fn(ptr<i32>) -> ptr<i32>>>(field0(%8), function_decay<ptr<fn(ptr<i32>) -> ptr<i32>>>(%4));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%10)), read<i32>(deref(call<ptr<i32>, signature=fn(ptr<i32>) -> ptr<i32>>(read<ptr<fn(ptr<i32>) -> ptr<i32>>>(field0(%8)), addr_of<ptr<i32>>(%7)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
