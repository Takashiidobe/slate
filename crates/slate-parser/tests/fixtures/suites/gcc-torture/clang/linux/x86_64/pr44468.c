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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_R:[0-9]+]] R = struct {
// DEFAULT-NEXT:         field0 k: i32;
// DEFAULT-NEXT:         field1 a: @type[[TYPE_S]];
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_Q:[0-9]+]] Q = struct {
// DEFAULT-NEXT:         field0 k: f32;
// DEFAULT-NEXT:         field1 a: @type[[TYPE_S]];
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: @type[[TYPE_Q]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_q:[0-9]+]] q: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE_S]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<void>>(%[[VALUE_q]])), const<u64>(4)));
// DEFAULT-NEXT:         write<i32>(field0(field1(%[[VALUE_s]])), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_b]]))), const<i32>(3));
// DEFAULT-NEXT:         return read<i32>(field0(field1(%[[VALUE_s]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_q_2:[0-9]+]] q: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: ptr<@type[[TYPE_S]]> [storage=automatic] = addr_of<ptr<@type[[TYPE_S]]>>(field1(deref(pointer_cast<ptr<@type[[TYPE_R]]>, reason=explicit>(read<ptr<void>>(%[[VALUE_q_2]])))));
// DEFAULT-NEXT:         write<i32>(field0(field1(%[[VALUE_s]])), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_b_2]]))), const<i32>(3));
// DEFAULT-NEXT:         return read<i32>(field0(field1(%[[VALUE_s]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_q_3:[0-9]+]] q: ptr<void>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field0(field1(%[[VALUE_s]])), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field0(deref(pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<void>>(%[[VALUE_q_3]])), const<u64>(4))))), const<i32>(3));
// DEFAULT-NEXT:         return read<i32>(field0(field1(%[[VALUE_s]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<u64>(const<u64>(4), const<u64>(4)), ne<u64>(const<u64>(4), const<u64>(4))), ne<u64>(const<u64>(4), const<u64>(4)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         write<i32>(field0(field1(%[[VALUE_s]])), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field1(field1(%[[VALUE_s]])), const<i32>(2));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>) -> i32>(%[[VALUE_test1]], pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<@type[[TYPE_Q]]>>(%[[VALUE_s]]))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(field0(field1(%[[VALUE_s]])), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field1(field1(%[[VALUE_s]])), const<i32>(2));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>) -> i32>(%[[VALUE_test2]], pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<@type[[TYPE_Q]]>>(%[[VALUE_s]]))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(field0(field1(%[[VALUE_s]])), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field1(field1(%[[VALUE_s]])), const<i32>(2));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>) -> i32>(%[[VALUE_test3]], pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<@type[[TYPE_Q]]>>(%[[VALUE_s]]))), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
