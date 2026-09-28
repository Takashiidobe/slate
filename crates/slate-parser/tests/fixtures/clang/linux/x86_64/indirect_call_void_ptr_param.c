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
// DEFAULT-NEXT:     type @type0 Data = struct {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %7 c: i8 [storage=static] [const] = truncate<i8, reason=assign, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %16 .str16: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([122, 101, 114, 111, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%15 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @process(%4 flag: i32, %5 handler: ptr<fn(ptr<const void>, i32) -> void>, %6 d: ptr<@type0>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const void>, i32) -> void>(read<ptr<fn(ptr<const void>, i32) -> void>>(%5), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<const i8>>(%7)), const<i32>(0));
// DEFAULT-NEXT:                 return;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, i32) -> void>(read<ptr<fn(ptr<const void>, i32) -> void>>(%5), pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type0>>(%6)), const<i32>(42));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @print_handler(%9 p: ptr<const void>, %10 extra: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%10), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %11 c: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=explicit>(read<ptr<const void>>(%9));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%16)), widen<i32, reason=vararg>(read<i8>(deref(read<ptr<const i8>>(%11)))));
// DEFAULT-NEXT:                 return;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %12 d: ptr<const @type0> [storage=automatic] = pointer_cast<ptr<const @type0>, reason=explicit>(read<ptr<const void>>(%9));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%17)), read<i32>(field0(deref(read<ptr<const @type0>>(%12)))), read<i32>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 d: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(7));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<fn(ptr<const void>, i32) -> void>, ptr<@type0>) -> void>(%3, const<i32>(1), function_decay<ptr<fn(ptr<const void>, i32) -> void>>(%8), addr_of<ptr<@type0>>(%14));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<fn(ptr<const void>, i32) -> void>, ptr<@type0>) -> void>(%3, const<i32>(0), function_decay<ptr<fn(ptr<const void>, i32) -> void>>(%8), addr_of<ptr<@type0>>(%14));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
