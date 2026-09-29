// SLATE-FILECHECK-DEFINES DEFAULT

struct A {
  long a;
};

static inline void foo(struct A *x)
{
  __asm__ __volatile__("" : "+m"(x->a) : "r"(x) : "memory", "cc");
}

static inline void bar(struct A *x)
{
  foo(x);
}

struct B { char buf[640]; struct A a; };
struct B b[32];

int baz(void)
{
  int i;
  struct B *j;
  for (i = 1; i < 32; i++)
    {
      j = &b[i];
      bar(&j->a);
    }
  return 0;
}

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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 buf: array<i8, 640>;
// DEFAULT-NEXT:         field1 a: @type[[TYPE_A]];
// DEFAULT-NEXT:     } [size=648, align=8, offsets=[0, 640]];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: array<@type[[TYPE_B]], 32> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_A]]>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             inlateout 0 "m" [mem] width 64 place<i64>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_x]]))));
// DEFAULT-NEXT:             in 1 "r" [reg] width 64 read<ptr<@type[[TYPE_A]]>>(%[[VALUE_x]]);
// DEFAULT-NEXT:             clobbers: memory, cc;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: ptr<@type[[TYPE_A]]>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_A]]>) -> void>(%[[VALUE_foo]], read<ptr<@type[[TYPE_A]]>>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: ptr<@type[[TYPE_B]]> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(1));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_B]]>>(%[[VALUE_j]], addr_of<ptr<@type[[TYPE_B]]>>(deref(ptr_offset<ptr<@type[[TYPE_B]]>, subtract=false, element=@type[[TYPE_B]], overflow=ub>(array_decay<ptr<@type[[TYPE_B]]>, length=Some(32)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i]])))));
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type[[TYPE_A]]>) -> void>(%[[VALUE_bar]], addr_of<ptr<@type[[TYPE_A]]>>(field1(deref(read<ptr<@type[[TYPE_B]]>>(%[[VALUE_j]])))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
