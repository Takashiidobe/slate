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
// DEFAULT-NEXT:     type @type[[TYPE_Callback:[0-9]+]] Callback = ptr<fn(i32) -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE_Handlers:[0-9]+]] Handlers = struct {
// DEFAULT-NEXT:         field0 label: ptr<const i8>;
// DEFAULT-NEXT:         field1 onEvent: ptr<fn(i32) -> i32>;
// DEFAULT-NEXT:         field2 counter: ptr<i32>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([110, 111, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_report:[0-9]+]] @report(%[[VALUE_h:[0-9]+]] h: @type[[TYPE_Handlers]]) -> i32 [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if ne<ptr<fn(i32) -> i32>>(read<ptr<fn(i32) -> i32>>(field1(%[[VALUE_h]])), null<ptr<fn(i32) -> i32>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(field1(%[[VALUE_h]])), const<i32>(1)));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(field2(%[[VALUE_h]])), null<ptr<i32>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), read<i32>(deref(read<ptr<i32>>(field2(%[[VALUE_h]])))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_h_2:[0-9]+]] h: @type[[TYPE_Handlers]] [storage=automatic] = aggregate<@type[[TYPE_Handlers]], zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str]])), field1 = null<ptr<fn(i32) -> i32>>, field2 = null<ptr<i32>>);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), call<i32, signature=fn(@type[[TYPE_Handlers]]) -> i32, abi=sysv64(native_c) -> scalar>(%[[VALUE_report]], copy<@type[[TYPE_Handlers]], reason=arg>(read<@type[[TYPE_Handlers]]>(%[[VALUE_h_2]]))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
