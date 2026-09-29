#include <stdio.h>

enum Err { ERR_NONE = 0, ERR_BAD = 5 };

static int first(void) {
  struct Rec {
    const char *text;
    enum Err    code;
  };
  struct Rec items[] = {{"a", ERR_NONE}, {"bb", ERR_BAD}};
  int        total   = 0;
  for (unsigned i = 0; i < sizeof(items) / sizeof(struct Rec); i++)
    total += (int)items[i].code + (int)items[i].text[0];
  return total;
}

static int second(void) {
  struct Rec {
    unsigned long n;
    const char   *text;
    enum Err      code;
  };
  struct Rec items[] = {{5, "x", ERR_NONE}, {6, "y", ERR_BAD}};
  int        total   = 0;
  for (unsigned i = 0; i < sizeof(items) / sizeof(struct Rec); i++)
    total += (int)items[i].n + (int)items[i].code + (int)items[i].text[0];
  return total;
}

int main(void) {
  printf("%d %d\n", first(), second());
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
// DEFAULT-NEXT:     type @type[[TYPE_Err:[0-9]+]] Err = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_ERR_NONE:[0-9]+]] ERR_NONE = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_ERR_BAD:[0-9]+]] ERR_BAD = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_Rec:[0-9]+]] Rec = struct {
// DEFAULT-NEXT:         field0 text: ptr<const i8>;
// DEFAULT-NEXT:         field1 code: @type[[TYPE_Err]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_Rec_2:[0-9]+]] Rec = struct {
// DEFAULT-NEXT:         field0 n: u64;
// DEFAULT-NEXT:         field1 text: ptr<const i8>;
// DEFAULT-NEXT:         field2 code: @type[[TYPE_Err]];
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([98, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_ERR_BAD]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_first:[0-9]+]] @first() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_items:[0-9]+]] items: array<@type[[TYPE_Rec]], 2> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_Rec]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_Rec]], zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]])), field1 = int_to_enum<@type[[TYPE_Err]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0)))), index1 = aggregate<@type[[TYPE_Rec]], zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str_2]])), field1 = int_to_enum<@type[[TYPE_Err]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(5)))));
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_i]])), div<u64, by_zero=ub>(const<u64>(32), const<u64>(16)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE1]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i]], read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_Err]]>(field1(deref(ptr_offset<ptr<@type[[TYPE_Rec]]>, subtract=false, element=@type[[TYPE_Rec]], overflow=ub>(array_decay<ptr<@type[[TYPE_Rec]]>, length=Some(2)>(%[[VALUE_items]]), read<u32>(%[[VALUE_i]]))))))), widen<i32, reason=explicit>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(field0(deref(ptr_offset<ptr<@type[[TYPE_Rec]]>, subtract=false, element=@type[[TYPE_Rec]], overflow=ub>(array_decay<ptr<@type[[TYPE_Rec]]>, length=Some(2)>(%[[VALUE_items]]), read<u32>(%[[VALUE_i]]))))), const<i32>(0)))))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_second:[0-9]+]] @second() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_items_2:[0-9]+]] items: array<@type[[TYPE_Rec_2]], 2> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_Rec_2]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_Rec_2]], zero_fill=false>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(5))), field1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_3]])), field2 = int_to_enum<@type[[TYPE_Err]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0)))), index1 = aggregate<@type[[TYPE_Rec_2]], zero_fill=false>(field0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(6))), field1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_4]])), field2 = int_to_enum<@type[[TYPE_Err]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(5)))));
// DEFAULT-NEXT:         let %[[VALUE_total_2:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_i_2]])), div<u64, by_zero=ub>(const<u64>(48), const<u64>(24)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE6]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], read<u32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32,
// DEFAULT-SAME: overflow=ub>(read<i32>(%[[VALUE8]]), add<i32, overflow=ub>(add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit,
// DEFAULT-SAME: fits=unknown>(read<u64>(field0(deref(ptr_offset<ptr<@type[[TYPE_Rec_2]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_Rec_2]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_Rec_2]]>,
// DEFAULT-SAME: length=Some(2)>(%[[VALUE_items_2]]),
// DEFAULT-SAME: read<u32>(%[[VALUE_i_2]]))))))), reinterpret<i32, reason=explicit, fits=unknown>(enum_to_int<u32,
// DEFAULT-SAME: reason=promotion>(read<@type[[TYPE_Err]]>(field2(deref(ptr_offset<ptr<@type[[TYPE_Rec_2]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_Rec_2]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_Rec_2]]>,
// DEFAULT-SAME: length=Some(2)>(%[[VALUE_items_2]]),
// DEFAULT-SAME: read<u32>(%[[VALUE_i_2]])))))))), widen<i32, reason=explicit>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const
// DEFAULT-SAME: i8>>(field1(deref(ptr_offset<ptr<@type[[TYPE_Rec_2]]>, subtract=false,
// DEFAULT-SAME: element=@type[[TYPE_Rec_2]],
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<@type[[TYPE_Rec_2]]>,
// DEFAULT-SAME: length=Some(2)>(%[[VALUE_items_2]]),
// DEFAULT-SAME: read<u32>(%[[VALUE_i_2]]))))), const<i32>(0)))))));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_ERR_BAD]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_5]])), call<i32, signature=fn() -> i32>(%[[VALUE_first]]), call<i32, signature=fn() -> i32>(%[[VALUE_second]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
