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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:         field3 d: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12]];
// DEFAULT-NEXT:     global %1 s: array<@type0, 2> [storage=static] [align=16] = aggregate<array<@type0, 2>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(6), field1 = const<i32>(8), field2 = neg<i32, overflow=ub>(const<i32>(8)), field3 = neg<i32, overflow=ub>(const<i32>(5))), index1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(2), field2 = neg<i32, overflow=ub>(const<i32>(1)), field3 = const<i32>(2))) [linkage=external];
// DEFAULT-NEXT:     global %4 cnt: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %10 @__builtin_memcmp(%7 <unnamed>: ptr<const void>, %8 <unnamed>: ptr<const void>, %9 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %11 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @foo(%3 r: @type0) -> void [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(coerce<i64, i64>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:         let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%4, read<i32>(%14));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%10, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type0>>(%3)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%1), read<i32>(%13))))), const<u64>(16)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 r: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(6), field1 = const<i32>(8), field2 = neg<i32, overflow=ub>(const<i32>(8)), field3 = neg<i32, overflow=ub>(const<i32>(5)));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void, abi=sysv64(coerce<i64, i64>) -> void>(%2, copy<@type0, reason=arg>(read<@type0>(%6)));
// DEFAULT-NEXT:         write<@type0>(%6, copy<@type0, reason=assign>(read<@type0>(compound_literal %12 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(2), field2 = neg<i32, overflow=ub>(const<i32>(1)), field3 = const<i32>(2)))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void, abi=sysv64(coerce<i64, i64>) -> void>(%2, copy<@type0, reason=arg>(read<@type0>(%6)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
