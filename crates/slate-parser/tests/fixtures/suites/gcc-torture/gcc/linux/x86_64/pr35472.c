extern void  abort(void);
extern void *memset(void *s, int c, __SIZE_TYPE__ n);
struct S {
  int i[16];
};
struct S                               *p;
void __attribute__((noinline, noclone)) foo(struct S *a, struct S *b) {
  a->i[0] = -1;
  p       = b;
}
void test(void) {
  struct S a, b;
  memset(&a.i[0], '\0', sizeof(a.i));
  memset(&b.i[0], '\0', sizeof(b.i));
  foo(&a, &b);
  *p = a;
  *p = b;
  if (b.i[0] != -1)
    abort();
}
int main() {
  test();
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
// DEFAULT-NEXT:         field0 i: array<i32, 16>;
// DEFAULT-NEXT:     } [size=64, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_S]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE_s:[0-9]+]] s: ptr<void>, %[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_n:[0-9]+]] n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_S]]>, %[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE_S]]>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_a]])))), const<i32>(0))), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]], read<ptr<@type[[TYPE_S]]>>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(field0(%[[VALUE_a_2]])), const<i32>(0))))), const<i32>(0), const<u64>(64));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(field0(%[[VALUE_b_2]])), const<i32>(0))))), const<i32>(0), const<u64>(64));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>, ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_a_2]]), addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_a_2]])));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_b_2]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(16)>(field0(%[[VALUE_b_2]])), const<i32>(0)))), neg<i32, overflow=ub>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
