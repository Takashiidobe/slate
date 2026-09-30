#include <stdio.h>

struct Data {
  int value;
};

static void process(int flag, void (*handler)(const void *, int),
                    struct Data *d) {
  if (flag) {
    static const char c = '\0';
    handler(&c, 0);
    return;
  }
  handler(d, 42);
}

static void print_handler(const void *p, int extra) {
  if (extra == 0) {
    const char *c = (const char *)p;
    printf("zero %d\n", *c);
    return;
  }
  const struct Data *d = (const struct Data *)p;
  printf("%d %d\n", d->value, extra);
}

int main(void) {
  struct Data d = {7};
  process(1, print_handler, &d);
  process(0, print_handler, &d);
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
// DEFAULT-NEXT:     type @type[[TYPE_Data:[0-9]+]] Data = struct {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i8 [storage=static] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([122, 101, 114, 111, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_process:[0-9]+]] @process(%[[VALUE_flag:[0-9]+]] flag: i32, %[[VALUE_handler:[0-9]+]] handler: ptr<fn(ptr<const void>, i32) -> void>, %[[VALUE_d:[0-9]+]] d: ptr<@type[[TYPE_Data]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_flag]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const void>, i32) -> void>(read<ptr<fn(ptr<const void>, i32) -> void>>(%[[VALUE_handler]]), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(%[[VALUE_c]])), const<i32>(0));
// DEFAULT-NEXT:                 return;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, i32) -> void>(read<ptr<fn(ptr<const void>, i32) -> void>>(%[[VALUE_handler]]), pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type[[TYPE_Data]]>>(%[[VALUE_d]])), const<i32>(42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_print_handler:[0-9]+]] @print_handler(%[[VALUE_p:[0-9]+]] p: ptr<const void>, %[[VALUE_extra:[0-9]+]] extra: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_extra]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_c_2:[0-9]+]] c: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=explicit>(read<ptr<const void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str]])), widen<i32, reason=vararg>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE_c_2]])))));
// DEFAULT-NEXT:                 return;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %[[VALUE_d_2:[0-9]+]] d: ptr<const @type[[TYPE_Data]]> [storage=automatic] = pointer_cast<ptr<const @type[[TYPE_Data]]>, reason=explicit>(read<ptr<const void>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_2]])), read<i32>(field0(deref(read<ptr<const @type[[TYPE_Data]]>>(%[[VALUE_d_2]])))), read<i32>(%[[VALUE_extra]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_d_3:[0-9]+]] d: @type[[TYPE_Data]] [storage=automatic] = aggregate<@type[[TYPE_Data]], zero_fill=false>(field0 = const<i32>(7));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<fn(ptr<const void>, i32) -> void>, ptr<@type[[TYPE_Data]]>) -> void>(%[[VALUE_process]], const<i32>(1), function_decay<ptr<fn(ptr<const void>, i32) -> void>>(%[[VALUE_print_handler]]), addr_of<ptr<@type[[TYPE_Data]]>>(%[[VALUE_d_3]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<fn(ptr<const void>, i32) -> void>, ptr<@type[[TYPE_Data]]>) -> void>(%[[VALUE_process]], const<i32>(0), function_decay<ptr<fn(ptr<const void>, i32) -> void>>(%[[VALUE_print_handler]]), addr_of<ptr<@type[[TYPE_Data]]>>(%[[VALUE_d_3]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
