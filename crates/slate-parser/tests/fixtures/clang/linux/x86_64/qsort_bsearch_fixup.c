#include <stdio.h>
#include <stdlib.h>

struct Item {
  int key;
  int value;
};

static int cmp_int(const void *a, const void *b) {
  return *(const int *)a - *(const int *)b;
}

static int cmp_item(const void *a, const void *b) {
  const struct Item *ia = (const struct Item *)a;
  const struct Item *ib = (const struct Item *)b;
  return ia->key - ib->key;
}

int main(void) {
  int nums[5] = {4, 1, 5, 3, 2};
  int key     = 3;
  qsort(nums, 5, sizeof(int), cmp_int);
  int *hit = bsearch(&key, nums, 5, sizeof(int), cmp_int);

  struct Item items[4] = {{3, 30}, {1, 10}, {4, 40}, {2, 20}};
  struct Item needle   = {4, 0};
  qsort(items, 4, sizeof(struct Item), cmp_item);
  struct Item *found =
      bsearch(&needle, items, 4, sizeof(struct Item), cmp_item);

  printf("%d %d %d %d\n", nums[0], nums[4], hit ? *hit : -1,
         found ? found->value : -1);
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___compar_fn_t:[0-9]+]] __compar_fn_t = ptr<fn(ptr<const void>, ptr<const void>) -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE_Item:[0-9]+]] Item = struct {
// DEFAULT-NEXT:         field0 key: i32;
// DEFAULT-NEXT:         field1 value: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bsearch:[0-9]+]] @bsearch(%[[VALUE___key:[0-9]+]] __key: ptr<const void>, %[[VALUE___base:[0-9]+]] __base: ptr<const void>, %[[VALUE___nmemb:[0-9]+]] __nmemb: u64, %[[VALUE___size:[0-9]+]] __size: u64, %[[VALUE___compar:[0-9]+]] __compar: ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_qsort:[0-9]+]] @qsort(%[[VALUE___base_2:[0-9]+]] __base: ptr<void>, %[[VALUE___nmemb_2:[0-9]+]] __nmemb: u64, %[[VALUE___size_2:[0-9]+]] __size: u64, %[[VALUE___compar_2:[0-9]+]] __compar: ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cmp_int:[0-9]+]] @cmp_int(%[[VALUE_a:[0-9]+]] a: ptr<const void>, %[[VALUE_b:[0-9]+]] b: ptr<const void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(deref(pointer_cast<ptr<const i32>, reason=explicit>(read<ptr<const void>>(%[[VALUE_a]])))), read<i32>(deref(pointer_cast<ptr<const i32>, reason=explicit>(read<ptr<const void>>(%[[VALUE_b]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_cmp_item:[0-9]+]] @cmp_item(%[[VALUE_a_2:[0-9]+]] a: ptr<const void>, %[[VALUE_b_2:[0-9]+]] b: ptr<const void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ia:[0-9]+]] ia: ptr<const @type[[TYPE_Item]]> [storage=automatic] = pointer_cast<ptr<const @type[[TYPE_Item]]>, reason=explicit>(read<ptr<const void>>(%[[VALUE_a_2]]));
// DEFAULT-NEXT:         let %[[VALUE_ib:[0-9]+]] ib: ptr<const @type[[TYPE_Item]]> [storage=automatic] = pointer_cast<ptr<const @type[[TYPE_Item]]>, reason=explicit>(read<ptr<const void>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(field0(deref(read<ptr<const @type[[TYPE_Item]]>>(%[[VALUE_ia]])))), read<i32>(field0(deref(read<ptr<const @type[[TYPE_Item]]>>(%[[VALUE_ib]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_nums:[0-9]+]] nums: array<i32, 5> [storage=automatic] [align=16] = aggregate<array<i32, 5>, zero_fill=false>(index0 = const<i32>(4), index1 = const<i32>(1), index2 = const<i32>(5), index3 = const<i32>(3), index4 = const<i32>(2));
// DEFAULT-NEXT:         let %[[VALUE_key:[0-9]+]] key: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64, u64, ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> void>(%[[VALUE_qsort]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i32>, length=Some(5)>(%[[VALUE_nums]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), const<u64>(4), function_decay<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%[[VALUE_cmp_int]]));
// DEFAULT-NEXT:         let %[[VALUE_hit:[0-9]+]] hit: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, ptr<const void>, u64, u64, ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> ptr<void>>(%[[VALUE_bsearch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_key]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i32>, length=Some(5)>(%[[VALUE_nums]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))), const<u64>(4), function_decay<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%[[VALUE_cmp_int]])));
// DEFAULT-NEXT:         let %[[VALUE_items:[0-9]+]] items: array<@type[[TYPE_Item]], 4> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_Item]], 4>, zero_fill=false>(index0 = aggregate<@type[[TYPE_Item]], zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(30)), index1 = aggregate<@type[[TYPE_Item]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(10)), index2 = aggregate<@type[[TYPE_Item]], zero_fill=false>(field0 = const<i32>(4), field1 = const<i32>(40)), index3 = aggregate<@type[[TYPE_Item]], zero_fill=false>(field0 = const<i32>(2), field1 = const<i32>(20)));
// DEFAULT-NEXT:         let %[[VALUE_needle:[0-9]+]] needle: @type[[TYPE_Item]] [storage=automatic] = aggregate<@type[[TYPE_Item]], zero_fill=false>(field0 = const<i32>(4), field1 = const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64, u64, ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> void>(%[[VALUE_qsort]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<@type[[TYPE_Item]]>, length=Some(4)>(%[[VALUE_items]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), const<u64>(8), function_decay<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%[[VALUE_cmp_item]]));
// DEFAULT-NEXT:         let %[[VALUE_found:[0-9]+]] found: ptr<@type[[TYPE_Item]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_Item]]>, reason=assign>(call<ptr<void>, signature=fn(ptr<const void>, ptr<const void>, u64, u64, ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> ptr<void>>(%[[VALUE_bsearch]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_Item]]>>(%[[VALUE_needle]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<@type[[TYPE_Item]]>, length=Some(4)>(%[[VALUE_items]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))), const<u64>(8), function_decay<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%[[VALUE_cmp_item]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%[[VALUE_nums]]), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(5)>(%[[VALUE_nums]]), const<i32>(4)))), conditional<i32>(ne<ptr<i32>>(read<ptr<i32>>(%[[VALUE_hit]]), null<ptr<i32>>), read<i32>(deref(read<ptr<i32>>(%[[VALUE_hit]]))), neg<i32, overflow=ub>(const<i32>(1))), conditional<i32>(ne<ptr<@type[[TYPE_Item]]>>(read<ptr<@type[[TYPE_Item]]>>(%[[VALUE_found]]), null<ptr<@type[[TYPE_Item]]>>), read<i32>(field1(deref(read<ptr<@type[[TYPE_Item]]>>(%[[VALUE_found]])))), neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
