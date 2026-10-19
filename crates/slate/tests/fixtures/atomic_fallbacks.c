#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>

int main(void) {
  _Atomic(float) f = 1.5f;
  float old_f      = atomic_fetch_add_explicit(&f, 2.25f, memory_order_relaxed);
  float now_f      = atomic_load(&f);

  int            values[4] = {10, 20, 30, 40};
  _Atomic(int *) p         = values;
  int           *old_p = atomic_fetch_add_explicit(&p, 2, memory_order_acq_rel);
  int           *now_p = atomic_load(&p);
  int *old_x = atomic_exchange_explicit(&p, values + 1, memory_order_release);
  int *now_x = atomic_load(&p);

  printf("%.2f %.2f %td %td %d %td %td\n", old_f, now_f, old_p - values,
         now_p - values, *now_p, old_x - values, now_x - values);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut f: f32 = 1.5f32;
// CHECK-NEXT:     let mut old_f: f32 = 0 as f32;
// CHECK-NEXT:     old_f = unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = std::sync::atomic::AtomicU32::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(f) as *mut u32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 std::sync::atomic::Ordering::Relaxed,
// CHECK-NEXT:                 std::sync::atomic::Ordering::Relaxed,
// CHECK-NEXT:                 |[[T_1:__t[0-9]+]]| Some((f32::from_bits([[T_1]]) + 2.25f32).to_bits()),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_1]]| [[T_1]]);
// CHECK-NEXT:         f32::from_bits([[T_0]])
// CHECK-NEXT:     };
// CHECK-NEXT:     let now_f: f32 = unsafe {
// CHECK-NEXT:         f32::from_bits(
// CHECK-NEXT:             std::sync::atomic::AtomicU32::from_ptr(std::ptr::addr_of_mut!(f) as *mut u32)
// CHECK-NEXT:                 .load(std::sync::atomic::Ordering::SeqCst),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:         10i32, 20i32, 30i32, 40i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut p: *mut i32 = values.0.as_mut_ptr() as *mut i32;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: *mut i32 = unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = std::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(p) as *mut *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .fetch_update(
// CHECK-NEXT:                 std::sync::atomic::Ordering::AcqRel,
// CHECK-NEXT:                 std::sync::atomic::Ordering::Acquire,
// CHECK-NEXT:                 |[[T_3:__t[0-9]+]]| Some(([[T_3]] as *mut i32).offset(2 as isize) as *mut i32),
// CHECK-NEXT:             )
// CHECK-NEXT:             .unwrap_or_else(|[[T_3]]| [[T_3]]);
// CHECK-NEXT:         [[T_2]] as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let old_p: *mut i32 = [[V_0]];
// CHECK-NEXT:     let now_p: *mut i32 = unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(p) as *mut *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::SeqCst) as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: *mut i32 = unsafe {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = (values.0.as_mut_ptr() as *mut i32).offset(1 as isize);
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] = std::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(p) as *mut *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .swap([[T_4]] as *mut i32, std::sync::atomic::Ordering::Release);
// CHECK-NEXT:         [[T_5]] as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let old_x: *mut i32 = [[V_1]];
// CHECK-NEXT:     let now_x: *mut i32 = unsafe {
// CHECK-NEXT:         std::sync::atomic::AtomicPtr::from_ptr(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(p) as *mut *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:             .load(std::sync::atomic::Ordering::SeqCst) as *mut i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.2f %.2f %td %td %d %td %td\n".as_ptr(),
// CHECK-NEXT:             old_f as f64,
// CHECK-NEXT:             now_f as f64,
// CHECK-NEXT:             old_p.offset_from(values.0.as_mut_ptr() as *mut i32) as i64,
// CHECK-NEXT:             now_p.offset_from(values.0.as_mut_ptr() as *mut i32) as i64,
// CHECK-NEXT:             *now_p,
// CHECK-NEXT:             old_x.offset_from(values.0.as_mut_ptr() as *mut i32) as i64,
// CHECK-NEXT:             now_x.offset_from(values.0.as_mut_ptr() as *mut i32) as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
