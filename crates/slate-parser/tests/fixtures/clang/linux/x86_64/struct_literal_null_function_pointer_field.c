#include <stdio.h>

typedef int (*Callback)(int);

struct Handlers {
  const char *label;
  Callback    onEvent;
  int        *counter;
};

static int report(struct Handlers h) {
  int total = 0;
  if (h.onEvent != NULL) {
    total += h.onEvent(1);
  }
  if (h.counter != NULL) {
    total += *h.counter;
  }
  return total;
}

int main(void) {
  struct Handlers h = {"none", NULL, NULL};
  printf("%d\n", report(h));
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
// DEFAULT-NEXT:     type @type0 Callback = ptr<fn(i32) -> i32>;
// DEFAULT-NEXT:     type @type1 Handlers = struct {
// DEFAULT-NEXT:         field0 label: ptr<const i8>;
// DEFAULT-NEXT:         field1 onEvent: ptr<fn(i32) -> i32>;
// DEFAULT-NEXT:         field2 counter: ptr<i32>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([110, 111, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%8 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @report(%4 h: @type1) -> i32 [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if ne<ptr<fn(i32) -> i32>>(read<ptr<fn(i32) -> i32>>(field1(%4)), null<ptr<fn(i32) -> i32>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(field1(%4)), const<i32>(1)));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%12));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(field2(%4)), null<ptr<i32>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), read<i32>(deref(read<ptr<i32>>(field2(%4)))));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%14));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 h: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%9)), field1 = null<ptr<fn(i32) -> i32>>, field2 = null<ptr<i32>>);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%10)), call<i32, signature=fn(@type1) -> i32, abi=sysv64(native_c) -> scalar>(%3, copy<@type1, reason=arg>(read<@type1>(%7))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
