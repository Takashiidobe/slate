/* PR middle-end/123978 */

struct A {
  unsigned b, c, *d;
};

[[gnu::noipa]] int foo(struct A *a) {
  __builtin_memset(a->d, 0, ((long long)sizeof(unsigned)) * a->b * a->c);
  return 0;
}

int main() {
  struct A a;
  unsigned b[256];
  __builtin_memset(b, 0x55, sizeof(b));
  a.b = 15;
  a.c = 15;
  a.d = b;
  foo(&a);
  for (int i = 0; i < 225; ++i)
    if (b[i] != 0)
      __builtin_abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 b: u32;
// DEFAULT-NEXT:         field1 c: u32;
// DEFAULT-NEXT:         field2 d: ptr<u32>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memset:[0-9]+]] @__builtin_memset(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_A]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<u32>>(field2(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]]))))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(mul<i64, overflow=ub>(mul<i64, overflow=ub>(reinterpret<i64, reason=explicit, fits=always>(const<u64>(4)), reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(read<u32>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]]))))))), reinterpret<i64, reason=usual_arith, fits=unknown>(widen<u64, reason=usual_arith>(read<u32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_a]])))))))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: array<u32, 256> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u32>, length=Some(256)>(%[[VALUE_b]])), const<i32>(85), const<u64>(1024));
// DEFAULT-NEXT:         write<u32>(field0(%[[VALUE_a_2]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(field1(%[[VALUE_a_2]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(15)));
// DEFAULT-NEXT:         write<ptr<u32>>(field2(%[[VALUE_a_2]]), array_decay<ptr<u32>, length=Some(256)>(%[[VALUE_b]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<@type[[TYPE_A]]>) -> i32>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(225))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(256)>(%[[VALUE_b]]), read<i32>(%[[VALUE_i]])))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
