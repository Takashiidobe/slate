/* PR rtl-optimization/57130 */

struct S {
  int a, b, c, d;
} s[2] = {{6, 8, -8, -5}, {0, 2, -1, 2}};

__attribute__((noinline, noclone)) void foo(struct S r) {
  static int cnt;
  if (__builtin_memcmp(&r, &s[cnt++], sizeof r) != 0)
    __builtin_abort();
}

int main() {
  struct S r = {6, 8, -8, -5};
  foo(r);
  r = (struct S){0, 2, -1, 2};
  foo(r);
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
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12]];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: array<@type[[TYPE_S]], 2> [storage=static] [align=16] = aggregate<array<@type[[TYPE_S]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(6), field1 = const<i32>(8), field2 = neg<i32, overflow=ub>(const<i32>(8)), field3 = neg<i32, overflow=ub>(const<i32>(5))), index1 = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(2), field2 = neg<i32, overflow=ub>(const<i32>(1)), field3 = const<i32>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cnt:[0-9]+]] cnt: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcmp:[0-9]+]] @__builtin_memcmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_r:[0-9]+]] r: @type[[TYPE_S]]) -> void [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_cnt]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_cnt]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_r]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_S]]>>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(2)>(%[[VALUE_s]]), read<i32>(%[[VALUE3]]))))), const<u64>(16)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(6), field1 = const<i32>(8), field2 = neg<i32, overflow=ub>(const<i32>(8)), field3 = neg<i32, overflow=ub>(const<i32>(5)));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S]]) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_foo]], copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_r_2]])));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(%[[VALUE_r_2]], copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(compound_literal %[[VALUE5:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(2), field2 = neg<i32, overflow=ub>(const<i32>(1)), field3 = const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(@type[[TYPE_S]]) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_foo]], copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_r_2]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
