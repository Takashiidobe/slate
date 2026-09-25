/* PR tree-optimization/32575 */

extern void abort(void);

struct S {
  void         *s1, *s2;
  unsigned char s3, s4, s5;
};

__attribute__((noinline)) void *foo(void) {
  static struct S s;
  return &s;
}

__attribute__((noinline)) void *bar() { return (void *)0; }

__attribute__((noinline)) struct S *test(void *a, void *b) {
  struct S *p, q;
  p = foo();
  if (p == 0) {
    p = &q;
    __builtin_memset(p, 0, sizeof(*p));
  }
  p->s1 = a;
  p->s2 = b;
  if (p == &q)
    p = 0;
  return p;
}

int main(void) {
  int       a;
  int       b;
  struct S *z = test((void *)&a, (void *)&b);
  if (z == 0 || z->s1 != (void *)&a || z->s2 != (void *)&b || z->s3 || z->s4)
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 s1: ptr<void>;
// DEFAULT-NEXT:         field1 s2: ptr<void>;
// DEFAULT-NEXT:         field2 s3: u8;
// DEFAULT-NEXT:         field3 s4: u8;
// DEFAULT-NEXT:         field4 s5: u8;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16, 17, 18]];
// DEFAULT-NEXT:     global %3 s: @type0 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo() -> ptr<void> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(addr_of<ptr<@type0>>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar() -> ptr<void> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<void>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @test(%6 a: ptr<void>, %7 b: ptr<void>) -> ptr<@type0> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 p: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %9 q: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type0>>(%8, pointer_cast<ptr<@type0>, reason=assign>(call<ptr<void>, signature=fn() -> ptr<void>>(%2)));
// DEFAULT-NEXT:         pointer_cast<ptr<@type0>, reason=assign>(call<ptr<void>, signature=fn() -> ptr<void>>(%2));
// DEFAULT-NEXT:         if eq<ptr<@type0>>(read<ptr<@type0>>(%8), null<ptr<@type0>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type0>>(%8, addr_of<ptr<@type0>>(%9));
// DEFAULT-NEXT:                 call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type0>>(%8)), const<i32>(0), const<u64>(24));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<ptr<void>>(field0(deref(read<ptr<@type0>>(%8))), read<ptr<void>>(%6));
// DEFAULT-NEXT:         write<ptr<void>>(field1(deref(read<ptr<@type0>>(%8))), read<ptr<void>>(%7));
// DEFAULT-NEXT:         if eq<ptr<@type0>>(read<ptr<@type0>>(%8), addr_of<ptr<@type0>>(%9))
// DEFAULT-NEXT:             write<ptr<@type0>>(%8, null<ptr<@type0>>);
// DEFAULT-NEXT:         return read<ptr<@type0>>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %12 b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 z: ptr<@type0> [storage=automatic] = call<ptr<@type0>, signature=fn(ptr<void>, ptr<void>) -> ptr<@type0>>(%5, pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%11)), pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%12)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(eq<ptr<@type0>>(read<ptr<@type0>>(%13), null<ptr<@type0>>), ne<ptr<void>>(read<ptr<void>>(field0(deref(read<ptr<@type0>>(%13)))), pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%11)))), ne<ptr<void>>(read<ptr<void>>(field1(deref(read<ptr<@type0>>(%13)))), pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<i32>>(%12)))), ne<u8>(read<u8>(field2(deref(read<ptr<@type0>>(%13)))), const<u8>(0))), ne<u8>(read<u8>(field3(deref(read<ptr<@type0>>(%13)))), const<u8>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
