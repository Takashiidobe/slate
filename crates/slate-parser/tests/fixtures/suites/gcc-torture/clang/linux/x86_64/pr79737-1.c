/* PR tree-optimization/79737 */

#if __SIZEOF_INT__ < 4
__extension__ typedef __INT32_TYPE__ int32_t;
#else
typedef int int32_t;
#endif

#pragma pack(1)
struct S {
  int32_t b : 18;
  int32_t c : 1;
  int32_t d : 24;
  int32_t e : 15;
  int32_t f : 14;
} i;
int             g, j, k;
static struct S h;

void foo() {
  for (j = 0; j < 6; j++)
    k = 0;
  for (; k < 3; k++) {
    struct S m = {5, 0, -5, 9, 5};
    h          = m;
    if (g)
      i = m;
    h.e = 0;
  }
}

int main() {
  foo();
  if (h.e != 0)
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
// DEFAULT-NEXT:     type @type[[TYPE_int32_t:[0-9]+]] int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 b: i32 : 18;
// DEFAULT-NEXT:         field1 c: i32 : 1;
// DEFAULT-NEXT:         field2 d: i32 : 24;
// DEFAULT-NEXT:         field3 e: i32 : 15;
// DEFAULT-NEXT:         field4 f: i32 : 14;
// DEFAULT-NEXT:     } [size=9, align=1, offsets=[0, 2, 2, 5, 7], bit_offsets=[Some(0), Some(18), Some(19), Some(43), Some(58)], bit_units=[(0, 9)], field_units=[Some(0), Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: @type[[TYPE_S]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: @type[[TYPE_S]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_k]], const<i32>(0));
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_k]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_m:[0-9]+]] m: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(5), field1 = const<i32>(0), field2 = neg<i32, overflow=ub>(const<i32>(5)), field3 = const<i32>(9), field4 = const<i32>(5));
// DEFAULT-NEXT:                     write<@type[[TYPE_S]]>(%[[VALUE_h]], copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_m]])));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_g]]), const<i32>(0))
// DEFAULT-NEXT:                         write<@type[[TYPE_S]]>(%[[VALUE_i]], copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_m]])));
// DEFAULT-NEXT:                     write<i32>(bitfield3<unit=0, bytes=0..9, bits=43..58>(%[[VALUE_h]]), const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_foo]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(bitfield3<unit=0, bytes=0..9, bits=43..58>(%[[VALUE_h]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
