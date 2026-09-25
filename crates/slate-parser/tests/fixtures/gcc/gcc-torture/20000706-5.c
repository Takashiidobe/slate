extern void abort(void);
extern void exit(int);

struct baz {
  int a, b, c;
};

struct baz *c;

void bar(int b) {
  if (c->a != 1 || c->b != 2 || c->c != 3 || b != 4)
    abort();
}

void foo(struct baz a, int b) {
  c = &a;
  bar(b);
}

int main() {
  struct baz a;
  a.a = 1;
  a.b = 2;
  a.c = 3;
  foo(a, 4);
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
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     global %3 c: ptr<@type0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @bar(%5 b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(field0(deref(read<ptr<@type0>>(%3)))), const<i32>(1)), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(%3)))), const<i32>(2))), ne<i32>(read<i32>(field2(deref(read<ptr<@type0>>(%3)))), const<i32>(3))), ne<i32>(read<i32>(%5), const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @foo(%7 a: @type0, %8 b: i32) -> void [linkage=external] [abi=sysv64(coerce<i64, i32>, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<@type0>>(%3, addr_of<ptr<@type0>>(%7));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, read<i32>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%10), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field1(%10), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(field2(%10), const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(@type0, i32) -> void, abi=sysv64(coerce<i64, i32>, scalar) -> void>(%6, copy<@type0, reason=arg>(read<@type0>(%10)), const<i32>(4));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
