#include <stdio.h>

int main(void) {
  int v;

  v = 5;
  printf("%d\n", __sync_fetch_and_add(&v, 3));
  printf("%d\n", v);

  v = 5;
  printf("%d\n", __sync_fetch_and_sub(&v, 3));
  printf("%d\n", v);

  v = 0x0F;
  printf("%d\n", __sync_fetch_and_or(&v, 0xF0));
  printf("%d\n", v);

  v = 0xFF;
  printf("%d\n", __sync_fetch_and_and(&v, 0x0F));
  printf("%d\n", v);

  v = 0x0F;
  printf("%d\n", __sync_fetch_and_xor(&v, 0xFF));
  printf("%d\n", v);

  v = 5;
  printf("%d\n", __sync_fetch_and_nand(&v, 3));
  printf("%d\n", v);

  v = 5;
  printf("%d\n", __sync_sub_and_fetch(&v, 3));
  printf("%d\n", v);

  v = 0x0F;
  printf("%d\n", __sync_or_and_fetch(&v, 0xF0));
  printf("%d\n", v);

  v = 0xFF;
  printf("%d\n", __sync_and_and_fetch(&v, 0x0F));
  printf("%d\n", v);

  v = 0x0F;
  printf("%d\n", __sync_xor_and_fetch(&v, 0xFF));
  printf("%d\n", v);

  v = 5;
  printf("%d\n", __sync_nand_and_fetch(&v, 3));
  printf("%d\n", v);

  __sync_synchronize();
  printf("ok\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut v: i32 = 5;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(v) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_add(3i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 5;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(v) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_sub(3i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 15;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(v) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_or(240i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 255;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(v) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_and(15i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 15;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(v) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_xor(255i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 5;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(v) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_nand(3i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 5;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(v) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_sub(3i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:                 .wrapping_sub(3)
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 15;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(v) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_or(240i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }) | 240
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 255;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(v) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_and(15i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }) & 15
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 15;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(v) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_xor(255i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }) ^ 255
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 5;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         !((unsafe {
// CHECK-NEXT:             core::sync::atomic::AtomicI32::from_ptr(
// CHECK-NEXT:                     core::ptr::addr_of_mut!(v) as *mut i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .fetch_nand(3i32, core::sync::atomic::Ordering::SeqCst)
// CHECK-NEXT:         }) & 3)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     core::sync::atomic::fence(core::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:     println!("ok");
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
