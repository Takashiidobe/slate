#include <string.h>

void abort(void);

void foo(int *BM_tab, int j) {
  int *BM_tab_base;

  BM_tab_base  = BM_tab;
  BM_tab      += 0400;
  while (BM_tab_base != BM_tab) {
    *--BM_tab = j;
    *--BM_tab = j;
    *--BM_tab = j;
    *--BM_tab = j;
  }
}

int main() {
  int BM_tab[0400];
  memset(BM_tab, 0, sizeof(BM_tab));
  foo(BM_tab, 6);
  if (BM_tab[0] != 6)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn foo(mut BM_tab: *mut i32, mut j: i32) {
// CHECK-NEXT:     let mut BM_tab_base: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     BM_tab_base = BM_tab;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: *mut i32 = BM_tab;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: *mut i32 = unsafe { [[V_0]].offset((256 as i32) as isize) };
// CHECK-NEXT:     BM_tab = [[V_1]];
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while BM_tab_base != BM_tab {
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: *mut i32 = BM_tab;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: *mut i32 = unsafe { [[V_2]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:         BM_tab = [[V_3]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_3]] = j;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: *mut i32 = BM_tab;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: *mut i32 = unsafe { [[V_4]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:         BM_tab = [[V_5]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_5]] = j;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: *mut i32 = BM_tab;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: *mut i32 = unsafe { [[V_6]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:         BM_tab = [[V_7]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_7]] = j;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: *mut i32 = BM_tab;
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: *mut i32 = unsafe { [[V_8]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:         BM_tab = [[V_9]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_9]] = j;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut BM_tab: __SlateAlign16<[i32; 256]> = __SlateAlign16([0 as i32; 256]);
// CHECK-NEXT:     unsafe { memset(BM_tab.0.as_mut_ptr() as *mut (), 0 as i32, 1024 as u64) };
// CHECK-NEXT:     foo(BM_tab.0.as_mut_ptr() as *mut i32, 6 as i32);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe { (BM_tab.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) }
// CHECK-NEXT:     }) != (6 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
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
