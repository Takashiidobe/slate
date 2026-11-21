int  array[1000];
void test(int a) {
  if (__builtin_expect(a > 3, 1))
    return;
  for (int i = 0; i < a; i++)
    array[i] = i;
}
void test2(int a) {
  if (__builtin_expect(a > 10, 1))
    return;
  for (int i = 0; i < a; i++)
    array[i] = i;
}
int main() {
  test(1);
  test(2);
  test(3);
  test2(10);
  if (array[9] != 9)
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
// DEFAULT-NEXT:     global %[[VALUE_array:[0-9]+]] array: array<i32, 1000> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expect:[0-9]+]] @__builtin_expect(%[[VALUE0:[0-9]+]] <unnamed>: i64, %[[VALUE1:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_a:[0-9]+]] a: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE___builtin_expect]], from_bool<i64, reason=arg>(gt<i32>(read<i32>(%[[VALUE_a]]), const<i32>(3))), widen<i64, reason=arg>(const<i32>(1))), const<i64>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_a]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1000)>(%[[VALUE_array]]), read<i32>(%[[VALUE_i]]))), read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_a_2:[0-9]+]] a: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE___builtin_expect]], from_bool<i64, reason=arg>(gt<i32>(read<i32>(%[[VALUE_a_2]]), const<i32>(10))), widen<i64, reason=arg>(const<i32>(1))), const<i64>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), read<i32>(%[[VALUE_a_2]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1000)>(%[[VALUE_array]]), read<i32>(%[[VALUE_i_2]]))), read<i32>(%[[VALUE_i_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test]], const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test]], const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test]], const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_test2]], const<i32>(10));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1000)>(%[[VALUE_array]]), const<i32>(9)))), const<i32>(9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
