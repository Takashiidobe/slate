#include <stdlib.h>

struct {
  long sqlcode;
} sqlca;

struct data_record {
  int dummy;
  int a[100];
} *data_ptr, data_tmp;

int num_records() { return 1; }

void fetch() {
  static int fetch_count;

  __builtin_memset(&data_tmp, 0x55, sizeof(data_tmp));
  sqlca.sqlcode = (++fetch_count > 1 ? 100 : 0);
}

void load_data() {
  struct data_record *p;
  int                 num = num_records();

  data_ptr = malloc(num * sizeof(struct data_record));
  __builtin_memset(data_ptr, 0xaa, num * sizeof(struct data_record));

  fetch();
  p = data_ptr;
  while (sqlca.sqlcode == 0) {
    *p++ = data_tmp;
    fetch();
  }
}

int main(void) {
  load_data();
  if (sizeof(int) == 2 && data_ptr[0].dummy != 0x5555)
    abort();
  else if (sizeof(int) > 2 && data_ptr[0].dummy != 0x55555555)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     sqlcode: i64,
// CHECK-NEXT: }
// CHECK-NEXT: static mut sqlca: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct data_record {
// CHECK-NEXT:     dummy: i32,
// CHECK-NEXT:     a: [i32; 100],
// CHECK-NEXT: }
// CHECK-NEXT: static mut data_ptr: *mut data_record = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut data_tmp: data_record = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn num_records() -> i32 {
// CHECK-NEXT:     1 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn fetch() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memset(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(data_tmp) }) as *mut (),
// CHECK-NEXT:             85 as i32,
// CHECK-NEXT:             404 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = unsafe { fetch_count };
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         fetch_count = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         sqlca.sqlcode = (if [[V_4]] > (1 as i32) { 100 as i32 } else { 0 as i32 }) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut fetch_count: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn load_data() {
// CHECK-NEXT:     let mut p: *mut data_record = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut num: i32 = num_records();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         data_ptr = (unsafe { malloc(((num as i64) as u64).wrapping_mul(404 as u64)) })
// CHECK-NEXT:             as *mut data_record;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memset(
// CHECK-NEXT:             (unsafe { data_ptr }) as *mut (),
// CHECK-NEXT:             170 as i32,
// CHECK-NEXT:             ((num as i64) as u64).wrapping_mul(404 as u64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     fetch();
// CHECK-NEXT:     p = unsafe { data_ptr };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { sqlca.sqlcode }) == ((0 as i32) as i64) {
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: *mut data_record = p;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: *mut data_record = unsafe { [[V_5]].offset((1 as i32) as isize) };
// CHECK-NEXT:         p = [[V_6]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_5]] = unsafe { data_tmp };
// CHECK-NEXT:         }
// CHECK-NEXT:         fetch();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     load_data();
// CHECK-NEXT:     if (4 as u64) == (((2 as i32) as i64) as u64)
// CHECK-NEXT:         && (unsafe {
// CHECK-NEXT:             (*unsafe { unsafe { data_ptr }.offset((0 as i32) as isize) }).dummy
// CHECK-NEXT:         }) != (21845 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if (4 as u64) > (((2 as i32) as i64) as u64)
// CHECK-NEXT:             && (unsafe {
// CHECK-NEXT:                 (*unsafe { unsafe { data_ptr }.offset((0 as i32) as isize) }).dummy
// CHECK-NEXT:             }) != (1431655765 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
