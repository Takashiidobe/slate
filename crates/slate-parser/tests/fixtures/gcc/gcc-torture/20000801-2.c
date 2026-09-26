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
// DEFAULT-NEXT:     type @type0 foo = struct {
// DEFAULT-NEXT:         field0 next: ptr<@type0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @bar() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @baz() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test(%6 node: ptr<@type0>) -> ptr<@type0> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         while %12 ne<ptr<@type0>>(read<ptr<@type0>>(%6), null<ptr<@type0>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %13: bool [synthetic];
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn() -> i32>(%2), const<i32>(0))
// DEFAULT-NEXT:                     write<bool>(%13, not<bool>(ne<i32>(call<i32, signature=fn() -> i32>(%3), const<i32>(0))));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%13, const<bool>(false));
// DEFAULT-NEXT:                 if read<bool>(%13)
// DEFAULT-NEXT:                     break %12;
// DEFAULT-NEXT:                 write<ptr<@type0>>(%6, read<ptr<@type0>>(field0(deref(read<ptr<@type0>>(%6)))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<ptr<@type0>>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %9 b: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %10 c: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type0>>(field0(%8), addr_of<ptr<@type0>>(%9));
// DEFAULT-NEXT:         write<ptr<@type0>>(field0(%9), null<ptr<@type0>>);
// DEFAULT-NEXT:         write<ptr<@type0>>(%10, call<ptr<@type0>, signature=fn(ptr<@type0>) -> ptr<@type0>>(%5, addr_of<ptr<@type0>>(%8)));
// DEFAULT-NEXT:         call<ptr<@type0>, signature=fn(ptr<@type0>) -> ptr<@type0>>(%5, addr_of<ptr<@type0>>(%8));
// DEFAULT-NEXT:         if ne<ptr<@type0>>(read<ptr<@type0>>(%10), null<ptr<@type0>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
