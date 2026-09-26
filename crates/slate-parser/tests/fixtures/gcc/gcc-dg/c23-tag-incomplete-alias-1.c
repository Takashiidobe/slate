/* { dg-do run } 
 * { dg-options "-std=c23 -O2" } */

[[gnu::noinline]]
void *alias(void *ap, void *bp, void *x, void *y) {
  struct foo {
    struct bar *f;
  } *a = ap;
  struct bar {
    long x;
  };

  a->f = x;

  {
    struct bar;
    struct foo {
      struct bar *f;
    } *b = bp;
    struct bar {
      long x;
    };

    // after completing bar, the two struct foo should be compatible

    b->f = y;
  }

  return a->f;
}

int main() {
  struct bar {
    long x;
  };
  struct foo {
    struct bar *f;
  } a;
  struct bar x, y;
  if (&y != alias(&a, &a, &x, &y))
    __builtin_abort();

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
// DEFAULT-NEXT:     type @type0 foo = struct {
// DEFAULT-NEXT:         field0 f: ptr<@type1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 bar = struct incomplete;
// DEFAULT-NEXT:     type @type2 bar = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type3 foo = struct {
// DEFAULT-NEXT:         field0 f: ptr<@type2>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type4 bar = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type5 foo = struct {
// DEFAULT-NEXT:         field0 f: ptr<@type4>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %0 @alias(%1 ap: ptr<void>, %2 bp: ptr<void>, %3 x: ptr<void>, %4 y: ptr<void>) -> ptr<void> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 a: ptr<@type0> [storage=automatic] = pointer_cast<ptr<@type0>, reason=assign>(read<ptr<void>>(%1));
// DEFAULT-NEXT:         write<ptr<@type1>>(field0(deref(read<ptr<@type0>>(%7))), pointer_cast<ptr<@type1>, reason=assign>(read<ptr<void>>(%3)));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %10 b: ptr<@type3> [storage=automatic] = pointer_cast<ptr<@type3>, reason=assign>(read<ptr<void>>(%2));
// DEFAULT-NEXT:             write<ptr<@type2>>(field0(deref(read<ptr<@type3>>(%10))), pointer_cast<ptr<@type2>, reason=assign>(read<ptr<void>>(%4)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(read<ptr<@type1>>(field0(deref(read<ptr<@type0>>(%7)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 a: @type5 [storage=automatic];
// DEFAULT-NEXT:         let %15 x: @type4 [storage=automatic];
// DEFAULT-NEXT:         let %16 y: @type4 [storage=automatic];
// DEFAULT-NEXT:         if ne<ptr<@type4>>(addr_of<ptr<@type4>>(%16), pointer_cast<ptr<@type4>, reason=usual_arith>(call<ptr<void>, signature=fn(ptr<void>, ptr<void>, ptr<void>, ptr<void>) -> ptr<void>>(%0, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type5>>(%14)), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type5>>(%14)), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type4>>(%15)), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type4>>(%16)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
