typedef long aligned_to;
struct incomplete;

char *global = &(_Alignas(_Alignof(aligned_to)) char){1};

void f(struct incomplete *list[]) {
  char *leading = &(_Alignas(16) char){2};
  char *trailing = &(char _Alignas(int)){3};
  int *array = (_Alignas(8) int[]){1, 2, 3};
  char *strictest = &(_Alignas(16) _Alignas(32) char){4};
  char *natural = &(char){5};
  struct incomplete *first = list[0];
}

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     type @type[[TYPE_aligned_to:[0-9]+]] aligned_to = i64;
// DEFAULT-NEXT:     type @type[[TYPE_incomplete:[0-9]+]] incomplete = struct incomplete;
// DEFAULT-NEXT:     global %[[VALUE_global:[0-9]+]] global: ptr<i8> [storage=static] = addr_of<ptr<i8>>(compound_literal %[[VALUE0:[0-9]+]] [storage=static] [align=8] = truncate<i8, reason=assign, fits=always>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_list:[0-9]+]] list: ptr<ptr<@type[[TYPE_incomplete]]>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_leading:[0-9]+]] leading: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] [align=16] = truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         let %[[VALUE_trailing:[0-9]+]] trailing: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(compound_literal %[[VALUE2:[0-9]+]] [storage=automatic] [align=4] = truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         let %[[VALUE_array:[0-9]+]] array: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(3)>(compound_literal %[[VALUE3:[0-9]+]] [storage=automatic] [align=8] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3)));
// DEFAULT-NEXT:         let %[[VALUE_strictest:[0-9]+]] strictest: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(compound_literal %[[VALUE4:[0-9]+]] [storage=automatic] [align=32] = truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         let %[[VALUE_natural:[0-9]+]] natural: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(compound_literal %[[VALUE5:[0-9]+]] [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(5)));
// DEFAULT-NEXT:         let %[[VALUE_first:[0-9]+]] first: ptr<@type[[TYPE_incomplete]]> [storage=automatic] = read<ptr<@type[[TYPE_incomplete]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_incomplete]]>>, subtract=false, element=ptr<@type[[TYPE_incomplete]]>, overflow=ub>(read<ptr<ptr<@type[[TYPE_incomplete]]>>>(%[[VALUE_list]]), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
