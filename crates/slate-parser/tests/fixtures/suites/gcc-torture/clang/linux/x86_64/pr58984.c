/* PR tree-optimization/58984 */

struct S {
  int f0 : 8;
  int    : 6;
  int f1 : 5;
};
struct T {
  char f0;
  int    : 6;
  int f1 : 5;
};

int a, *c = &a, e, n, b, m;

static int foo(struct S p) {
  const unsigned short *f[36];
  for (; e < 2; e++) {
    const unsigned short **i  = &f[0];
    *c                       ^= 1;
    if (p.f1) {
      *i = 0;
      return b;
    }
  }
  return 0;
}

static int bar(struct T p) {
  const unsigned short *f[36];
  for (; e < 2; e++) {
    const unsigned short **i  = &f[0];
    *c                       ^= 1;
    if (p.f1) {
      *i = 0;
      return b;
    }
  }
  return 0;
}

int main() {
  struct S o = {1, 1};
  foo(o);
  m = n || o.f0;
  if (a != 1)
    __builtin_abort();
  e          = 0;
  struct T p = {1, 1};
  bar(p);
  m |= n || p.f0;
  if (a != 0)
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 f0: i32 : 8;
// DEFAULT-NEXT:         field1 <anonymous>: i32 : 6;
// DEFAULT-NEXT:         field2 f1: i32 : 5;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 1, 1], bit_offsets=[Some(0), Some(8), Some(14)], bit_units=[(0, 3)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = struct {
// DEFAULT-NEXT:         field0 f0: i8;
// DEFAULT-NEXT:         field1 <anonymous>: i32 : 6;
// DEFAULT-NEXT:         field2 f1: i32 : 5;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 1, 1], bit_offsets=[None, Some(8), Some(14)], bit_units=[(1, 2)], field_units=[None, Some(0), Some(0)]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%[[VALUE_a]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_m:[0-9]+]] m: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: @type[[TYPE_S]]) -> i32 [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: array<ptr<const u16>, 36> [storage=automatic] [align=16];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_e]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_i:[0-9]+]] i: ptr<ptr<const u16>> [storage=automatic] = addr_of<ptr<ptr<const u16>>>(deref(ptr_offset<ptr<ptr<const u16>>, subtract=false, element=ptr<const u16>, overflow=ub>(array_decay<ptr<ptr<const u16>>, length=Some(36)>(%[[VALUE_f]]), const<i32>(0))));
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_c]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE3]])));
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = xor<i32>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%[[VALUE3]])), read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(bitfield2<unit=0, bytes=0..3, bits=14..19>(%[[VALUE_p]])), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<ptr<const u16>>(deref(read<ptr<ptr<const u16>>>(%[[VALUE_i]])), null<ptr<const u16>>);
// DEFAULT-NEXT:                             return read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_p_2:[0-9]+]] p: @type[[TYPE_T]]) -> i32 [linkage=internal] [abi=sysv64(native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_f_2:[0-9]+]] f: array<ptr<const u16>, 36> [storage=automatic] [align=16];
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_e]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_i_2:[0-9]+]] i: ptr<ptr<const u16>> [storage=automatic] = addr_of<ptr<ptr<const u16>>>(deref(ptr_offset<ptr<ptr<const u16>>, subtract=false, element=ptr<const u16>, overflow=ub>(array_decay<ptr<ptr<const u16>>, length=Some(36)>(%[[VALUE_f_2]]), const<i32>(0))));
// DEFAULT-NEXT:                     let %[[VALUE9:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_c]]);
// DEFAULT-NEXT:                     let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE9]])));
// DEFAULT-NEXT:                     let %[[VALUE11:[0-9]+]]: i32 [synthetic] = xor<i32>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(deref(read<ptr<i32>>(%[[VALUE9]])), read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(bitfield2<unit=0, bytes=1..3, bits=6..11>(%[[VALUE_p_2]])), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<ptr<const u16>>(deref(read<ptr<ptr<const u16>>>(%[[VALUE_i_2]])), null<ptr<const u16>>);
// DEFAULT-NEXT:                             return read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_o:[0-9]+]] o: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=false>(field0 = const<i32>(1), field2 = const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(@type[[TYPE_S]]) -> i32, abi=sysv64(native_c) -> scalar>(%[[VALUE_foo]], copy<@type[[TYPE_S]], reason=arg>(read<@type[[TYPE_S]]>(%[[VALUE_o]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_m]], from_bool<i32, reason=assign>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0)), ne<i32>(read<i32>(bitfield0<unit=0, bytes=0..3, bits=0..8>(%[[VALUE_o]])), const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_e]], const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_p_3:[0-9]+]] p: @type[[TYPE_T]] [storage=automatic] = aggregate<@type[[TYPE_T]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field2 = const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(@type[[TYPE_T]]) -> i32, abi=sysv64(native_c) -> scalar>(%[[VALUE_bar]], copy<@type[[TYPE_T]], reason=arg>(read<@type[[TYPE_T]]>(%[[VALUE_p_3]])));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_m]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE12]]), from_bool<i32, reason=promotion>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0)), ne<i8>(read<i8>(field0(%[[VALUE_p_3]])), const<i8>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_m]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
