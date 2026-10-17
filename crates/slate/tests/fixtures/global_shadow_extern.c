#include <stdio.h>

extern int late;
int total = 10;
static long scale;

struct Tally {
  int  hits;
  long weight;
};

struct Tally tally = {1};

static void bump(int *slot, int by) { *slot += by; }

static int shadow_param(int total) { return total * 2; }

static void record(struct Tally *t, long weight) {
  t->hits += 1;
  t->weight += weight;
}

int main(void) {
  int before = total;
  {
    int total = 3;
    bump(&total, 4);
    printf("local %d\n", total);
  }
  bump(&total, 5);
  scale = 7;
  record(&tally, scale);
  record(&tally, scale * 2);
  printf("%d %d %d\n", before, total, shadow_param(total));
  printf("%d %ld\n", tally.hits, tally.weight);
  printf("late %d\n", late);
  return 0;
}

int late = 99;

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut total: i32 = 10 as i32;
// CHECK-NEXT: static mut scale: i64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Tally {
// CHECK-NEXT:     hits: i32,
// CHECK-NEXT:     weight: i64,
// CHECK-NEXT: }
// CHECK-NEXT: static mut tally: Tally = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: Tally = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].hits = 1 as i32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn bump([[V_0:__v[0-9]+]]: *mut i32, by: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0]] = (unsafe { *[[V_0]] }) + by;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn shadow_param([[TOTAL_0:total_[0-9]+]]: i32) -> i32 {
// CHECK-NEXT:     [[TOTAL_0]] * 2
// CHECK-NEXT: }
// CHECK-NEXT: fn record(t: *mut Tally, weight: i64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*t).hits = (unsafe { (*t).hits }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*t).weight = (unsafe { (*t).weight }) + weight;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let before: i32 = unsafe { total };
// CHECK-NEXT:     let mut [[TOTAL_1:total_[0-9]+]]: i32 = 3;
// CHECK-NEXT:     bump(std::ptr::addr_of_mut!([[TOTAL_1]]), 4 as i32);
// CHECK-NEXT:     println!("local {}", [[TOTAL_1]]);
// CHECK-NEXT:     bump(unsafe { std::ptr::addr_of_mut!(total) }, 5 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         scale = 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     record(unsafe { std::ptr::addr_of_mut!(tally) }, unsafe { scale });
// CHECK-NEXT:     record(unsafe { std::ptr::addr_of_mut!(tally) }, (unsafe { scale }) * 2);
// CHECK-NEXT:     println!("{} {} {}", before, unsafe { total }, shadow_param(unsafe { total }));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%d %ld\n".as_ptr(), unsafe { tally.hits }, unsafe { tally.weight })
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("late {}", unsafe { late });
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut late: i32 = 99 as i32;
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
