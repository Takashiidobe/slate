#define NULL 0

struct stuff {
  int   a;
  int   b;
  int   c;
  int   d;
  int   e;
  char *f;
  int   g;
};

void __attribute__((noinline)) bar(struct stuff *x) {
  if (x->g != 2)
    __builtin_abort();
}

int main(int argc, char **argv) {
  struct stuff x = {0, 0, 0, 0, 0, NULL, 0};
  x.a            = 100;
  x.d            = 100;
  x.g            = 2;
  /* Struct should now look like {100, 0, 0, 100, 0, 0, 0, 2}.  */
  bar(&x);
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
// DEFAULT-NEXT:     type @type0 stuff = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: i32;
// DEFAULT-NEXT:         field4 e: i32;
// DEFAULT-NEXT:         field5 f: ptr<i8>;
// DEFAULT-NEXT:         field6 g: i32;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 4, 8, 12, 16, 24, 32]];
// DEFAULT-NEXT:     fn %1 @bar(%2 x: ptr<@type0>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field6(deref(read<ptr<@type0>>(%2)))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main(%4 argc: i32, %5 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 x: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0), field2 = const<i32>(0), field3 = const<i32>(0), field4 = const<i32>(0), field5 = null<ptr<i8>>, field6 = const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field0(%6), const<i32>(100));
// DEFAULT-NEXT:         write<i32>(field3(%6), const<i32>(100));
// DEFAULT-NEXT:         write<i32>(field6(%6), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%1, addr_of<ptr<@type0>>(%6));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
