#include <stddef.h>

struct S {
  int i;
  int j;
};
struct R {
  int      k;
  struct S a;
};
struct Q {
  float    k;
  struct S a;
};
struct Q                               s;
int __attribute__((noinline, noclone)) test1(void *q) {
  struct S *b = (struct S *)((char *)q + sizeof(int));
  s.a.i       = 0;
  b->i        = 3;
  return s.a.i;
}
int __attribute__((noinline, noclone)) test2(void *q) {
  struct S *b = &((struct R *)q)->a;
  s.a.i       = 0;
  b->i        = 3;
  return s.a.i;
}
int __attribute__((noinline, noclone)) test3(void *q) {
  s.a.i                                      = 0;
  ((struct S *)((char *)q + sizeof(int)))->i = 3;
  return s.a.i;
}
extern void abort(void);
int         main() {
  if (sizeof(float) != sizeof(int) || offsetof(struct R, a) != sizeof(int) ||
      offsetof(struct Q, a) != sizeof(int))
    return 0;
  s.a.i = 1;
  s.a.j = 2;
  if (test1((void *)&s) != 3)
    abort();
  s.a.i = 1;
  s.a.j = 2;
  if (test2((void *)&s) != 3)
    abort();
  s.a.i = 1;
  s.a.j = 2;
  if (test3((void *)&s) != 3)
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
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 R = struct {
// DEFAULT-NEXT:         field0 k: i32;
// DEFAULT-NEXT:         field1 a: @type0;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type2 Q = struct {
// DEFAULT-NEXT:         field0 k: f32;
// DEFAULT-NEXT:         field1 a: @type0;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %3 s: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @test1(%5 q: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 b: ptr<@type0> [storage=automatic] = pointer_cast<ptr<@type0>, reason=explicit>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<void>>(%5)), const<u64>(4)));
// DEFAULT-NEXT:         write<i32>(field0(field1(%3)), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type0>>(%6))), const<i32>(3));
// DEFAULT-NEXT:         return read<i32>(field0(field1(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test2(%8 q: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 b: ptr<@type0> [storage=automatic] = addr_of<ptr<@type0>>(field1(deref(pointer_cast<ptr<@type1>, reason=explicit>(read<ptr<void>>(%8)))));
// DEFAULT-NEXT:         write<i32>(field0(field1(%3)), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type0>>(%9))), const<i32>(3));
// DEFAULT-NEXT:         return read<i32>(field0(field1(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test3(%11 q: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(field1(%3)), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field0(deref(pointer_cast<ptr<@type0>, reason=explicit>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<void>>(%11)), const<u64>(4))))), const<i32>(3));
// DEFAULT-NEXT:         return read<i32>(field0(field1(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u64>(const<u64>(4), const<u64>(4)), ne<u64>(const<u64>(4), const<u64>(4))), ne<u64>(const<u64>(4), const<u64>(4)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<i32>(field0(field1(%3)), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field1(field1(%3)), const<i32>(2));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>) -> i32>(%4, pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<@type2>>(%3))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i32>(field0(field1(%3)), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field1(field1(%3)), const<i32>(2));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>) -> i32>(%7, pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<@type2>>(%3))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i32>(field0(field1(%3)), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field1(field1(%3)), const<i32>(2));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>) -> i32>(%10, pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<@type2>>(%3))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
