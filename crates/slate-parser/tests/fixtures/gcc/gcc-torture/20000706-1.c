extern void abort(void);
extern void exit(int);

struct baz {
  int a, b, c, d, e;
};

void bar(struct baz *x, int f, int g, int h, int i, int j) {
  if (x->a != 1 || x->b != 2 || x->c != 3 || x->d != 4 || x->e != 5 || f != 6 ||
      g != 7 || h != 8 || i != 9 || j != 10)
    abort();
}

void foo(struct baz x, char **y) { bar(&x, 6, 7, 8, 9, 10); }

int main() {
  struct baz x;

  x.a = 1;
  x.b = 2;
  x.c = 3;
  x.d = 4;
  x.e = 5;
  foo(x, (char **)0);
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
// DEFAULT-NEXT:     type @type0 baz = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: i32;
// DEFAULT-NEXT:         field4 e: i32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%15 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @bar(%4 x: ptr<@type0>, %5 f: i32, %6 g: i32, %7 h: i32, %8 i: i32, %9 j: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type0>>(%4)))), const<i32>(1)), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(%4)))), const<i32>(2))), ne<i32>(read<i32>(field2(deref(read<ptr<@type0>>(%4)))), const<i32>(3))), ne<i32>(read<i32>(field3(deref(read<ptr<@type0>>(%4)))), const<i32>(4))), ne<i32>(read<i32>(field4(deref(read<ptr<@type0>>(%4)))), const<i32>(5))), ne<i32>(read<i32>(%5), const<i32>(6))), ne<i32>(read<i32>(%6), const<i32>(7))), ne<i32>(read<i32>(%7), const<i32>(8))), ne<i32>(read<i32>(%8), const<i32>(9))), ne<i32>(read<i32>(%9), const<i32>(10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @foo(%11 x: @type0, %12 y: ptr<ptr<i8>>) -> void [linkage=external] [abi=sysv64(byval<align=4>, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32, i32, i32, i32, i32) -> void>(%3, addr_of<ptr<@type0>>(%11), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 x: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%14), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field1(%14), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(field2(%14), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(field3(%14), const<i32>(4));
// DEFAULT-NEXT:         write<i32>(field4(%14), const<i32>(5));
// DEFAULT-NEXT:         call<void, signature=fn(@type0, ptr<ptr<i8>>) -> void, abi=sysv64(byval<align=4>, scalar) -> void>(%10, copy<@type0, reason=arg>(read<@type0>(%14)), null<ptr<ptr<i8>>>);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
