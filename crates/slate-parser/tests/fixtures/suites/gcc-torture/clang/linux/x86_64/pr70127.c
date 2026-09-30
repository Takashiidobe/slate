/* PR tree-optimization/70127 */

struct S {
  int        f;
  signed int g : 2;
} a[1], c = {5, 1}, d;
short b;

__attribute__((noinline, noclone)) void foo(int x) {
  if (x != 1)
    __builtin_abort();
}

int main() {
  while (b++ <= 0) {
    struct S e = {1, 1};
    d = e = a[0] = c;
  }
  foo(a[0].g);
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
// DEFAULT-NEXT:         field0 f: i32;
// DEFAULT-NEXT:         field1 g: i32 : 2;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[None, Some(32)], bit_units=[(4, 1)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: array<@type[[TYPE_S]], 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: @type[[TYPE_S]] [storage=static] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(5), field1 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: @type[[TYPE_S]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_b]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE1]])), const<i32>(1)));
// DEFAULT-NEXT:             write<i16>(%[[VALUE_b]], read<i16>(%[[VALUE2]]));
// DEFAULT-NEXT:             yield le<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE1]])), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_e:[0-9]+]] e: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(1));
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: @type[[TYPE_S]] [synthetic] = copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_c]]));
// DEFAULT-NEXT:                 write<@type[[TYPE_S]]>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(1)>(%[[VALUE_a]]), const<i32>(0))), read<@type[[TYPE_S]]>(%[[VALUE3]]));
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: @type[[TYPE_S]] [synthetic] = copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE3]]));
// DEFAULT-NEXT:                 write<@type[[TYPE_S]]>(%[[VALUE_e]], read<@type[[TYPE_S]]>(%[[VALUE4]]));
// DEFAULT-NEXT:                 write<@type[[TYPE_S]]>(%[[VALUE_d]], copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE4]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo]], read<i32>(bitfield1<unit=0, bytes=4..5, bits=0..2>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(1)>(%[[VALUE_a]]), const<i32>(0))))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
