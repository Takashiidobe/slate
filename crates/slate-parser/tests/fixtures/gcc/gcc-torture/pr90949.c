void __attribute__((noipa, noinline)) my_puts(const char *str) {}

void __attribute__((noipa, noinline)) my_free(void *p) {}

struct Node {
  struct Node *child;
};

struct Node space[2] = {};

struct Node *__attribute__((noipa, noinline)) my_malloc(int bytes) {
  return &space[0];
}

void walk(struct Node *module, int cleanup) {
  if (module == 0) {
    return;
  }
  if (!cleanup) {
    my_puts("No cleanup");
  }
  walk(module->child, cleanup);
  if (cleanup) {
    my_free(module);
  }
}

int main() {
  struct Node *node = my_malloc(sizeof(struct Node));
  node->child       = 0;
  walk(node, 1);
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
// DEFAULT-NEXT:     type @type0 Node = struct {
// DEFAULT-NEXT:         field0 child: ptr<@type0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %5 space: array<@type0, 2> [storage=static] = aggregate<array<@type0, 2>, zero_fill=true>() [linkage=external];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([78, 111, 32, 99, 108, 101, 97, 110, 117, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @my_puts(%1 str: ptr<const i8>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @my_free(%3 p: ptr<void>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @my_malloc(%7 bytes: i32) -> ptr<@type0> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%5), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @walk(%9 module: ptr<@type0>, %10 cleanup: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<ptr<@type0>>(read<ptr<@type0>>(%9), null<ptr<@type0>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%10), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>) -> void>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%13)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%8, read<ptr<@type0>>(field0(deref(read<ptr<@type0>>(%9)))), read<i32>(%10));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%2, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type0>>(%9)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 node: ptr<@type0> [storage=automatic] = call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%6, reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         write<ptr<@type0>>(field0(deref(read<ptr<@type0>>(%12))), null<ptr<@type0>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%8, read<ptr<@type0>>(%12), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
