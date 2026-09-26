struct X {
  struct Y {
    struct YY {
      struct Z {
        int i;
      } c;
    } bb;
  } b;
} a;
int __attribute__((noinline, noclone)) foo(struct Z *p) {
  int i = p->i;
  a.b   = (struct Y){};
  return p->i + i;
}
extern void abort(void);
int         main() {
  a.b.bb.c.i = 1;
  if (foo(&a.b.bb.c) != 1)
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
// DEFAULT-NEXT:         field0 b: @type1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 Y = struct {
// DEFAULT-NEXT:         field0 bb: @type2;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type2 YY = struct {
// DEFAULT-NEXT:         field0 c: @type3;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 Z = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %4 a: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %5 @foo(%6 p: ptr<@type3>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic] = read<i32>(field0(deref(read<ptr<@type3>>(%6))));
// DEFAULT-NEXT:         write<@type1>(field0(%4), copy<@type1, reason=assign>(read<@type1>(compound_literal %10 [storage=automatic] = aggregate<@type1, zero_fill=true>())));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(field0(deref(read<ptr<@type3>>(%6)))), read<i32>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(field0(field0(field0(field0(%4)))), const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type3>) -> i32>(%5, addr_of<ptr<@type3>>(field0(field0(field0(%4))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
