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
// DEFAULT-NEXT:     type @type[[TYPE_stuff:[0-9]+]] stuff = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: i32;
// DEFAULT-NEXT:         field4 e: i32;
// DEFAULT-NEXT:         field5 f: ptr<i8>;
// DEFAULT-NEXT:         field6 g: i32;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 4, 8, 12, 16, 24, 32]];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_stuff]]>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field6(deref(read<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_x]])))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: @type[[TYPE_stuff]] [storage=automatic] = aggregate<@type[[TYPE_stuff]], zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0), field2 = const<i32>(0), field3 = const<i32>(0), field4 = const<i32>(0), field5 = null<ptr<i8>>, field6 = const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_x_2]]), const<i32>(100));
// DEFAULT-NEXT:         write<i32>(field3(%[[VALUE_x_2]]), const<i32>(100));
// DEFAULT-NEXT:         write<i32>(field6(%[[VALUE_x_2]]), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_stuff]]>) -> void>(%[[VALUE_bar]], addr_of<ptr<@type[[TYPE_stuff]]>>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
