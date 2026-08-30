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
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Item {
// LOWERING-NEXT:     key: i32,
// LOWERING-NEXT:     value: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn qsort(_0: *mut core::ffi::c_void, _1: usize, _2: usize, _3: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32>);
// LOWERING-NEXT:     fn bsearch(_0: *const core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize, _3: usize, _4: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32>) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn cmp_int({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let mut a: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut b: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     a = {{arg[0-9]+}};
// LOWERING-NEXT:     b = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn cmp_item({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let mut a: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut b: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut ia: *mut Item = std::ptr::null_mut();
// LOWERING-NEXT:     let mut ib: *mut Item = std::ptr::null_mut();
// LOWERING-NEXT:     a = {{arg[0-9]+}};
// LOWERING-NEXT:     b = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Item = {{_v[0-9]+}} as *mut Item;
// LOWERING-NEXT:     ia = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Item = {{_v[0-9]+}} as *mut Item;
// LOWERING-NEXT:     ib = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Item = ia;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}}).key };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Item = ib;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}}).key };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut nums: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// LOWERING-NEXT:     let mut key: i32 = 0;
// LOWERING-NEXT:     let mut hit: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut items: aligned::Aligned<aligned::A16, [Item; 4]> = aligned::Aligned([Item { key: 0, value: 0 }; 4]);
// LOWERING-NEXT:     let mut needle: Item = Item { key: 0, value: 0 };
// LOWERING-NEXT:     let mut found: *mut Item = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     *nums = [4, 1, 5, 3, 2];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     key = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = nums.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     unsafe { qsort({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize, Some(cmp_int)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(key) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = nums.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { bsearch({{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize, Some(cmp_int)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-NEXT:     hit = {{_v[0-9]+}};
// LOWERING-NEXT:     *items = [Item { key: 3, value: 30 }, Item { key: 1, value: 10 }, Item { key: 4, value: 40 }, Item { key: 2, value: 20 }];
// LOWERING-NEXT:     needle = Item { key: 4, value: 0 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Item = items.as_mut_ptr() as *mut Item;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::size_of::<Item>() as u64;
// LOWERING-NEXT:     unsafe { qsort({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize, Some(cmp_item)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(needle) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Item = items.as_mut_ptr() as *mut Item;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::size_of::<Item>() as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { bsearch({{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize, Some(cmp_item)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Item = {{_v[0-9]+}} as *mut Item;
// LOWERING-NEXT:     found = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = nums[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = nums[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = hit;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i32 = hit;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Item = found;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut Item = found;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { (*{{_v[0-9]+}}).value };
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Item {
// REWRITES-NEXT:     key: i32,
// REWRITES-NEXT:     value: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn qsort(_0: *mut core::ffi::c_void, _1: usize, _2: usize, _3: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32>);
// REWRITES-NEXT:     fn bsearch(_0: *const core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize, _3: usize, _4: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32>) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn cmp_int({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// REWRITES-NEXT: let mut a: *mut core::ffi::c_void = {{arg[0-9]+}};
// REWRITES-NEXT: let mut b: *mut core::ffi::c_void = {{arg[0-9]+}};
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = (unsafe { *(a as *mut i32) }) - unsafe { *(b as *mut i32) };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn cmp_item({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// REWRITES-NEXT: let mut a: *mut core::ffi::c_void = {{arg[0-9]+}};
// REWRITES-NEXT: let mut b: *mut core::ffi::c_void = {{arg[0-9]+}};
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut ia: *mut Item = std::ptr::null_mut();
// REWRITES-NEXT: let mut ib: *mut Item = std::ptr::null_mut();
// REWRITES-NEXT: ia = a as *mut Item;
// REWRITES-NEXT: ib = b as *mut Item;
// REWRITES-NEXT: __retval = (unsafe { (*ia).key }) - unsafe { (*ib).key };
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut nums: aligned::Aligned<aligned::A16, [i32; 5]> = aligned::Aligned([0; 5]);
// REWRITES-NEXT: let mut key: i32 = 0;
// REWRITES-NEXT: let mut hit: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let mut items: aligned::Aligned<aligned::A16, [Item; 4]> = aligned::Aligned([Item { key: 0, value: 0 }; 4]);
// REWRITES-NEXT: let mut needle: Item = Item { key: 0, value: 0 };
// REWRITES-NEXT: let mut found: *mut Item = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: *nums = [4, 1, 5, 3, 2];
// REWRITES-NEXT: key = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = nums.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT: unsafe { qsort(({{_v[0-9]+}} as *mut core::ffi::c_void) as *mut core::ffi::c_void, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize, Some(cmp_int)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(key) as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = nums.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { bsearch({{_v[0-9]+}} as *const core::ffi::c_void, ({{_v[0-9]+}} as *mut core::ffi::c_void) as *const core::ffi::c_void, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize, Some(cmp_int)) };
// REWRITES-NEXT: hit = {{_v[0-9]+}} as *mut i32;
// REWRITES-NEXT: *items = [Item { key: 3, value: 30 }, Item { key: 1, value: 10 }, Item { key: 4, value: 40 }, Item { key: 2, value: 20 }];
// REWRITES-NEXT: needle = Item { key: 4, value: 0 };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut Item = items.as_mut_ptr() as *mut Item;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = std::mem::size_of::<Item>() as u64;
// REWRITES-NEXT: unsafe { qsort({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize, Some(cmp_item)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(needle) as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut Item = items.as_mut_ptr() as *mut Item;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = std::mem::size_of::<Item>() as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { bsearch({{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize, Some(cmp_item)) };
// REWRITES-NEXT: found = {{_v[0-9]+}} as *mut Item;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = nums[({{_v[0-9]+}} as usize)];
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = nums[({{_v[0-9]+}} as usize)];
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = hit != std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { *hit };
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = -1;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = found != std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { (*found).value };
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = -1;
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
