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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn bsearch(
// CHECK-NEXT:         [[ARG_0]]: *const (),
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: *const (),
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: u64,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: u64,
// CHECK-NEXT:         [[ARG_4:arg[0-9]+]]: Option<unsafe extern "C-unwind" fn(*const (), *const ()) -> i32>,
// CHECK-NEXT:     ) -> *mut ();
// CHECK-NEXT:     fn qsort(
// CHECK-NEXT:         [[ARG_0]]: *mut (),
// CHECK-NEXT:         [[ARG_1]]: u64,
// CHECK-NEXT:         [[ARG_2]]: u64,
// CHECK-NEXT:         [[ARG_3]]: Option<unsafe extern "C-unwind" fn(*const (), *const ()) -> i32>,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Item {
// CHECK-NEXT:     key: i32,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.cmp_int")]
// CHECK-NEXT: extern "C-unwind" fn cmp_int(a: *const (), b: *const ()) -> i32 {
// CHECK-NEXT:     (unsafe { *(a as *const i32) }) - unsafe { *(b as *const i32) }
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.cmp_item")]
// CHECK-NEXT: extern "C-unwind" fn cmp_item(a: *const (), b: *const ()) -> i32 {
// CHECK-NEXT:     (unsafe { (*(a as *const Item)).key }) - unsafe { (*(b as *const Item)).key }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut nums: __SlateAlign16<[i32; 5]> = __SlateAlign16([
// CHECK-NEXT:         4i32, 1i32, 5i32, 3i32, 2i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut key: i32 = 3;
// CHECK-NEXT:     unsafe { qsort(nums.0.as_mut_ptr() as *mut (), 5u64, 4u64, Some(cmp_int)) };
// CHECK-NEXT:     let hit: *mut i32 = (unsafe {
// CHECK-NEXT:         bsearch(
// CHECK-NEXT:             std::ptr::addr_of_mut!(key) as *const (),
// CHECK-NEXT:             nums.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             5u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             Some(cmp_int),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) as *mut i32;
// CHECK-NEXT:     let mut items: __SlateAlign16<[Item; 4]> = __SlateAlign16([
// CHECK-NEXT:         Item { key: 3, value: 30 },
// CHECK-NEXT:         Item { key: 1, value: 10 },
// CHECK-NEXT:         Item { key: 4, value: 40 },
// CHECK-NEXT:         Item { key: 2, value: 20 },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut needle: Item = Item { key: 4, value: 0 };
// CHECK-NEXT:     unsafe { qsort(items.0.as_mut_ptr() as *mut (), 4u64, 8u64, Some(cmp_item)) };
// CHECK-NEXT:     let found: *mut Item = (unsafe {
// CHECK-NEXT:         bsearch(
// CHECK-NEXT:             std::ptr::addr_of_mut!(needle) as *const (),
// CHECK-NEXT:             items.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             8u64,
// CHECK-NEXT:             Some(cmp_item),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) as *mut Item;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", nums.0[0], nums.0[4], if ! hit.is_null() { unsafe { * hit } } else
// CHECK-NEXT:         { - 1i32 }, if ! found.is_null() { unsafe { (* found).value } } else { - 1i32 }
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
