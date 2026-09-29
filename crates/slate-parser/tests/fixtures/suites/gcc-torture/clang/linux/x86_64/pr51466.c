/* PR tree-optimization/51466 */

extern void abort(void);

__attribute__((noinline, noclone)) int foo(int i) {
  volatile int v[4];
  int         *p;
  v[i] = 6;
  p    = (int *)&v[i];
  return *p;
}

__attribute__((noinline, noclone)) int bar(int i) {
  volatile int v[4];
  int         *p;
  v[i] = 6;
  p    = (int *)&v[i];
  *p   = 8;
  return v[i];
}

__attribute__((noinline, noclone)) int baz(int i) {
  volatile int v[4];
  int         *p;
  v[i] = 6;
  p    = (int *)&v[0];
  *p   = 8;
  return v[i];
}

int main() {
  if (foo(3) != 6 || bar(2) != 8 || baz(0) != 8 || baz(1) != 6)
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: volatile array<i32, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%[[VALUE_v]]), read<i32>(%[[VALUE_i]]))), const<i32>(6));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_p]], pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<volatile i32>>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%[[VALUE_v]]), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(%[[VALUE_p]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_i_2:[0-9]+]] i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v_2:[0-9]+]] v: volatile array<i32, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%[[VALUE_v_2]]), read<i32>(%[[VALUE_i_2]]))), const<i32>(6));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_p_2]], pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<volatile i32>>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%[[VALUE_v_2]]), read<i32>(%[[VALUE_i_2]]))))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_p_2]])), const<i32>(8));
// DEFAULT-NEXT:         return read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%[[VALUE_v_2]]), read<i32>(%[[VALUE_i_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_i_3:[0-9]+]] i: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v_3:[0-9]+]] v: volatile array<i32, 4> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_p_3:[0-9]+]] p: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         write<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%[[VALUE_v_3]]), read<i32>(%[[VALUE_i_3]]))), const<i32>(6));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_p_3]], pointer_cast<ptr<i32>, reason=explicit>(addr_of<ptr<volatile i32>>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%[[VALUE_v_3]]), const<i32>(0))))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_p_3]])), const<i32>(8));
// DEFAULT-NEXT:         return read<i32, volatile>(deref(ptr_offset<ptr<volatile i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<volatile i32>, length=Some(4)>(%[[VALUE_v_3]]), read<i32>(%[[VALUE_i_3]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(3)), const<i32>(6))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_bar]], const<i32>(2)), const<i32>(8)));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_baz]], const<i32>(0)), const<i32>(8)));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_baz]], const<i32>(1)), const<i32>(6)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
