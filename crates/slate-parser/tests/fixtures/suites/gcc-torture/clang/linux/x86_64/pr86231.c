/* PR tree-optimization/86231 */

#define ONE ((void *)1)
#define TWO ((void *)2)

__attribute__((noipa)) int foo(void *p, int x) {
  if (p == ONE)
    return 0;
  if (!p)
    p = x ? TWO : ONE;
  return p == ONE ? 0 : 1;
}

int v[8];

int main() {
  if (foo((void *)0, 0) != 0 || foo((void *)0, 1) != 1 || foo(ONE, 0) != 0 ||
      foo(ONE, 1) != 0 || foo(TWO, 0) != 1 || foo(TWO, 1) != 1 ||
      foo(&v[7], 0) != 1 || foo(&v[7], 1) != 1)
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
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: array<i32, 8> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: ptr<void>, %[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<ptr<void>>(read<ptr<void>>(%[[VALUE_p]]), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(1)))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         if not<bool>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_p]]), null<ptr<void>>))
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p]], conditional<ptr<void>>(ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(2)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:         return conditional<i32>(eq<ptr<void>>(read<ptr<void>>(%[[VALUE_p]]), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(1))), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%[[VALUE_foo]], null<ptr<void>>, const<i32>(0)), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%[[VALUE_foo]], null<ptr<void>>, const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%[[VALUE_foo]], int_to_ptr<ptr<void>, reason=explicit>(const<i32>(1)), const<i32>(0)), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%[[VALUE_foo]], int_to_ptr<ptr<void>, reason=explicit>(const<i32>(1)), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%[[VALUE_foo]], int_to_ptr<ptr<void>, reason=explicit>(const<i32>(2)), const<i32>(0)), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%[[VALUE_foo]], int_to_ptr<ptr<void>, reason=explicit>(const<i32>(2)), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%[[VALUE_foo]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(%[[VALUE_v]]), const<i32>(7))))), const<i32>(0)), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE5]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], ne<i32>(call<i32, signature=fn(ptr<void>, i32) -> i32>(%[[VALUE_foo]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(%[[VALUE_v]]), const<i32>(7))))), const<i32>(1)), const<i32>(1)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
