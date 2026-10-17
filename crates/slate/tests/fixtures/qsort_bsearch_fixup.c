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
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn bsearch(
// CHECK-NEXT:         __key: *const (),
// CHECK-NEXT:         __base: *const (),
// CHECK-NEXT:         __nmemb: u64,
// CHECK-NEXT:         __size: u64,
// CHECK-NEXT:         __compar: Option<unsafe extern "C-unwind" fn(*const (), *const ()) -> i32>,
// CHECK-NEXT:     ) -> *mut ();
// CHECK-NEXT:     fn qsort(
// CHECK-NEXT:         __base: *mut (),
// CHECK-NEXT:         __nmemb: u64,
// CHECK-NEXT:         __size: u64,
// CHECK-NEXT:         __compar: Option<unsafe extern "C-unwind" fn(*const (), *const ()) -> i32>,
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
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut key: i32 = 3;
// CHECK-NEXT:     unsafe { qsort(nums.0.as_mut_ptr() as *mut (), 5 as u64, 4 as u64, Some(cmp_int)) };
// CHECK-NEXT:     let hit: *mut i32 = (unsafe {
// CHECK-NEXT:         bsearch(
// CHECK-NEXT:             std::ptr::addr_of_mut!(key) as *const (),
// CHECK-NEXT:             nums.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             5 as u64,
// CHECK-NEXT:             4 as u64,
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
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         qsort(items.0.as_mut_ptr() as *mut (), 4 as u64, 8 as u64, Some(cmp_item))
// CHECK-NEXT:     };
// CHECK-NEXT:     let found: *mut Item = (unsafe {
// CHECK-NEXT:         bsearch(
// CHECK-NEXT:             std::ptr::addr_of_mut!(needle) as *const (),
// CHECK-NEXT:             items.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             4 as u64,
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:             Some(cmp_item),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) as *mut Item;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", nums.0[0], nums.0[4], if hit != std::ptr::null_mut::< i32 > () {
// CHECK-NEXT:         unsafe { * hit } } else { - (1 as i32) }, if found != std::ptr::null_mut::< Item
// CHECK-NEXT:         > () { unsafe { (* found).value } } else { - (1 as i32) }
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
