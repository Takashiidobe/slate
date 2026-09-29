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
// DEFAULT-NEXT:     type @type[[TYPE_Node:[0-9]+]] Node = struct {
// DEFAULT-NEXT:         field0 child: ptr<@type[[TYPE_Node]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_space:[0-9]+]] space: array<@type[[TYPE_Node]], 2> [storage=static] [align=16] = aggregate<array<@type[[TYPE_Node]], 2>, zero_fill=true>() [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([78, 111, 32, 99, 108, 101, 97, 110, 117, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_my_puts:[0-9]+]] @my_puts(%[[VALUE_str_2:[0-9]+]] str: ptr<const i8>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_free:[0-9]+]] @my_free(%[[VALUE_p:[0-9]+]] p: ptr<void>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_my_malloc:[0-9]+]] @my_malloc(%[[VALUE_bytes:[0-9]+]] bytes: i32) -> ptr<@type[[TYPE_Node]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<@type[[TYPE_Node]]>>(deref(ptr_offset<ptr<@type[[TYPE_Node]]>, subtract=false, element=@type[[TYPE_Node]], overflow=ub>(array_decay<ptr<@type[[TYPE_Node]]>, length=Some(2)>(%[[VALUE_space]]), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_walk:[0-9]+]] @walk(%[[VALUE_module:[0-9]+]] module: ptr<@type[[TYPE_Node]]>, %[[VALUE_cleanup:[0-9]+]] cleanup: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<ptr<@type[[TYPE_Node]]>>(read<ptr<@type[[TYPE_Node]]>>(%[[VALUE_module]]), null<ptr<@type[[TYPE_Node]]>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_cleanup]]), const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<const i8>) -> void>(%[[VALUE_my_puts]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_Node]]>, i32) -> void>(%[[VALUE_walk]], read<ptr<@type[[TYPE_Node]]>>(field0(deref(read<ptr<@type[[TYPE_Node]]>>(%[[VALUE_module]])))), read<i32>(%[[VALUE_cleanup]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_cleanup]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_my_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_Node]]>>(%[[VALUE_module]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_node:[0-9]+]] node: ptr<@type[[TYPE_Node]]> [storage=automatic] = call<ptr<@type[[TYPE_Node]]>, signature=fn(i32) -> ptr<@type[[TYPE_Node]]>>(%[[VALUE_my_malloc]], reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(8))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_Node]]>>(field0(deref(read<ptr<@type[[TYPE_Node]]>>(%[[VALUE_node]]))), null<ptr<@type[[TYPE_Node]]>>);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_Node]]>, i32) -> void>(%[[VALUE_walk]], read<ptr<@type[[TYPE_Node]]>>(%[[VALUE_node]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
