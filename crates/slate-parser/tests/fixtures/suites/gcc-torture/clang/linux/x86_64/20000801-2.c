extern void abort(void);
extern void exit(int);
int         bar(void);
int         baz(void);

struct foo {
  struct foo *next;
};

struct foo *test(struct foo *node) {
  while (node) {
    if (bar() && !baz())
      break;
    node = node->next;
  }
  return node;
}

int bar(void) { return 0; }

int baz(void) { return 0; }

int main(void) {
  struct foo a, b, *c;

  a.next = &b;
  b.next = (struct foo *)0;
  c      = test(&a);
  if (c)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type[[TYPE_foo:[0-9]+]] foo = struct {
// DEFAULT-NEXT:         field0 next: ptr<@type[[TYPE_foo]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_node:[0-9]+]] node: ptr<@type[[TYPE_foo]]>) -> ptr<@type[[TYPE_foo]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %[[VALUE1:[0-9]+]] ne<ptr<@type[[TYPE_foo]]>>(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_node]]), null<ptr<@type[[TYPE_foo]]>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_bar]]), const<i32>(0))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE2]], not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_baz]]), const<i32>(0))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE2]], const<bool>(false));
// DEFAULT-NEXT:                 if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:                     break %[[VALUE1]];
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_foo]]>>(%[[VALUE_node]], read<ptr<@type[[TYPE_foo]]>>(field0(deref(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_node]])))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_node]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_foo]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_foo]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: ptr<@type[[TYPE_foo]]> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_foo]]>>(field0(%[[VALUE_a]]), addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_foo]]>>(field0(%[[VALUE_b]]), null<ptr<@type[[TYPE_foo]]>>);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_foo]]>>(%[[VALUE_c]], call<ptr<@type[[TYPE_foo]]>, signature=fn(ptr<@type[[TYPE_foo]]>) -> ptr<@type[[TYPE_foo]]>>(%[[VALUE_test]], addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_a]])));
// DEFAULT-NEXT:         call<ptr<@type[[TYPE_foo]]>, signature=fn(ptr<@type[[TYPE_foo]]>) -> ptr<@type[[TYPE_foo]]>>(%[[VALUE_test]], addr_of<ptr<@type[[TYPE_foo]]>>(%[[VALUE_a]]));
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_foo]]>>(read<ptr<@type[[TYPE_foo]]>>(%[[VALUE_c]]), null<ptr<@type[[TYPE_foo]]>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
