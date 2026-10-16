
typedef __SIZE_TYPE__ size_t;
typedef unsigned int  index_ty;
typedef index_ty     *index_list_ty;

struct mult_index {
  index_ty     index;
  unsigned int count;
};

struct mult_index_list {
  struct mult_index *item;
  size_t             nitems;
  size_t             nitems_max;

  struct mult_index *item2;
  size_t             nitems2_max;
};

int __attribute__((noinline)) hash_find_entry(size_t *result) {
  *result = 2;
  return 0;
}

extern void                                  abort(void);
struct mult_index *__attribute__((noinline)) foo(size_t n) {
  static int count = 0;
  if (count++ > 0)
    abort();
  return 0;
}

int main(void) {
  size_t nitems = 0;

  for (;;) {
    size_t list;

    hash_find_entry(&list);
    {
      size_t             len2 = list;
      struct mult_index *destptr;
      struct mult_index *dest;
      size_t             new_max = nitems + len2;

      if (new_max != len2)
        break;
      dest = foo(new_max);

      destptr = dest;
      while (len2--)
        destptr++;

      nitems = destptr - dest;
    }
  }

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mult_index {
// CHECK-NEXT:     index: u32,
// CHECK-NEXT:     count: u32,
// CHECK-NEXT: }
// CHECK-NEXT: fn hash_find_entry(mut result: &mut u64) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(result as *mut u64) = ((2 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut n: u64) -> *mut mult_index {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { count };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_0]] > (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     std::ptr::null_mut::<mult_index>()
// CHECK-NEXT: }
// CHECK-NEXT: static mut count: i32 = 0 as i32;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut nitems: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut list: u64 = 0 as u64;
// CHECK-NEXT:         hash_find_entry(unsafe { &mut (*std::ptr::addr_of_mut!(list)) });
// CHECK-NEXT:         let mut len2: u64 = list;
// CHECK-NEXT:         let mut destptr: *mut mult_index = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut dest: *mut mult_index = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut new_max: u64 = nitems.wrapping_add(len2);
// CHECK-NEXT:         if new_max != len2 {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         dest = foo(new_max);
// CHECK-NEXT:         destptr = dest;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: u64 = len2;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: u64 = [[V_2]].wrapping_sub(((1 as i32) as i64) as u64);
// CHECK-NEXT:             len2 = [[V_3]];
// CHECK-NEXT:             if !([[V_2]] != (0 as u64)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: *mut mult_index = destptr;
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: *mut mult_index = unsafe {
// CHECK-NEXT:                 [[V_4]].offset((1 as i32) as isize)
// CHECK-NEXT:             };
// CHECK-NEXT:             destptr = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:         nitems = ((unsafe { destptr.offset_from(dest as *mut mult_index) }) as i64)
// CHECK-NEXT:             as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
