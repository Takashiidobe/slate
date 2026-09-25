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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 a: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 B = struct {
// DEFAULT-NEXT:         field0 buf: array<i8, 640>;
// DEFAULT-NEXT:         field1 a: @type0;
// DEFAULT-NEXT:     } [size=648, align=8, offsets=[0, 640]];
// DEFAULT-NEXT:     global %6 b: array<@type1, 32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: ptr<@type0>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             out 0 "+m" place<i64>(field0(deref(read<ptr<@type0>>(%2))));
// DEFAULT-NEXT:             in 1 "r" read<ptr<@type0>>(%2);
// DEFAULT-NEXT:             clobbers: memory, cc;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar(%4 x: ptr<@type0>) -> void [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%1, read<ptr<@type0>>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @baz() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9 j: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(1));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), const<i32>(32))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%12));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type1>>(%9, addr_of<ptr<@type1>>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(32)>(%6), read<i32>(%8)))));
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<@type0>) -> void>(%3, addr_of<ptr<@type0>>(field1(deref(read<ptr<@type1>>(%9)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
