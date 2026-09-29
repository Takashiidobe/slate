/* PR c++/34459 */

extern void  abort(void);
extern void *memset(void *s, int c, __SIZE_TYPE__ n);

struct S {
  char s[25];
};

struct S *p;

void __attribute__((noinline, noclone)) foo(struct S *x, int set) {
  int i;
  for (i = 0; i < sizeof(x->s); ++i)
    if (x->s[i] != 0)
      abort();
    else if (set)
      x->s[i] = set;
  p = x;
}

void __attribute__((noinline, noclone)) test1(void) {
  struct S a;
  memset(&a.s, '\0', sizeof(a.s));
  foo(&a, 0);
  struct S b = a;
  foo(&b, 1);
  b = a;
  b = b;
  foo(&b, 0);
}

void __attribute__((noinline, noclone)) test2(void) {
  struct S a;
  memset(&a.s, '\0', sizeof(a.s));
  foo(&a, 0);
  struct S b = a;
  foo(&b, 1);
  b = a;
  b = *p;
  foo(&b, 0);
}

void __attribute__((noinline, noclone)) test3(void) {
  struct S a;
  memset(&a.s, '\0', sizeof(a.s));
  foo(&a, 0);
  struct S b = a;
  foo(&b, 1);
  *p = a;
  *p = b;
  foo(&b, 0);
}

int main(void) {
  test1();
  test2();
  test3();
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
// DEFAULT-NEXT:         field0 s: array<i8, 25>;
// DEFAULT-NEXT:     } [size=25, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_S]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE_s:[0-9]+]] s: ptr<void>, %[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_n:[0-9]+]] n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_S]]>, %[[VALUE_set:[0-9]+]] set: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_i]]))), const<u64>(25))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(25)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_x]])))), read<i32>(%[[VALUE_i]]))))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_set]]), const<i32>(0))
// DEFAULT-NEXT:                         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(25)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_x]])))), read<i32>(%[[VALUE_i]]))), truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_set]])));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]], read<ptr<@type[[TYPE_S]]>>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<array<i8, 25>>>(field0(%[[VALUE_a]]))), const<i32>(0), const<u64>(25));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>, i32) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_a]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_S]] [storage=automatic] = copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_a]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>, i32) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_b]]), const<i32>(1));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(%[[VALUE_b]], copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_a]])));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(%[[VALUE_b]], copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_b]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>, i32) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_b]]), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<array<i8, 25>>>(field0(%[[VALUE_a_2]]))), const<i32>(0), const<u64>(25));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>, i32) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_a_2]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: @type[[TYPE_S]] [storage=automatic] = copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>, i32) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_b_2]]), const<i32>(1));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(%[[VALUE_b_2]], copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_a_2]])));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(%[[VALUE_b_2]], copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>, i32) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_b_2]]), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_3:[0-9]+]] a: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<array<i8, 25>>>(field0(%[[VALUE_a_3]]))), const<i32>(0), const<u64>(25));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>, i32) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_a_3]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_b_3:[0-9]+]] b: @type[[TYPE_S]] [storage=automatic] = copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_a_3]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>, i32) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_b_3]]), const<i32>(1));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_a_3]])));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_b_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>, i32) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_b_3]]), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test1]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test2]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test3]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
