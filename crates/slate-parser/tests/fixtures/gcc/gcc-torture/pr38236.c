struct X {
  int i;
};

int __attribute__((noinline)) foo(struct X *p, int *q, int a, int b) {
  struct X x, y;
  if (a)
    p = &x;
  if (b)
    q = &x.i;
  else
    q = &y.i;
  *q = 1;
  return p->i;
}
extern void abort(void);
int         main() {
  if (foo((void *)0, (void *)0, 1, 1) != 1)
    abort();
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
// DEFAULT-NEXT:     type @type0 X = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %1 @foo(%2 p: ptr<@type0>, %3 q: ptr<i32>, %4 a: i32, %5 b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 x: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %7 y: @type0 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<@type0>>(%2, addr_of<ptr<@type0>>(%6));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<i32>>(%3, addr_of<ptr<i32>>(field0(%6)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<ptr<i32>>(%3, addr_of<ptr<i32>>(field0(%7)));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%3)), const<i32>(1));
// DEFAULT-NEXT:         return read<i32>(field0(deref(read<ptr<@type0>>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type0>, ptr<i32>, i32, i32) -> i32>(%1, null<ptr<@type0>>, null<ptr<i32>>, const<i32>(1), const<i32>(1)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
