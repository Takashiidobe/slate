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
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut v: i32 = 5;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { let [[T_0:__t[0-9]+]] = 3i32; let [[T_1:__t[0-9]+]] =
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v) as * mut i32)
// CHECK-NEXT:         .fetch_add([[T_0]], std::sync::atomic::Ordering::SeqCst); [[T_1]] }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 5;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { let [[T_2:__t[0-9]+]] = 3i32; let [[T_3:__t[0-9]+]] =
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v) as * mut i32)
// CHECK-NEXT:         .fetch_sub([[T_2]], std::sync::atomic::Ordering::SeqCst); [[T_3]] }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 15;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { let [[T_4:__t[0-9]+]] = 240i32; let [[T_5:__t[0-9]+]] =
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v) as * mut i32)
// CHECK-NEXT:         .fetch_or([[T_4]], std::sync::atomic::Ordering::SeqCst); [[T_5]] }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 255;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { let [[T_6:__t[0-9]+]] = 15i32; let [[T_7:__t[0-9]+]] =
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v) as * mut i32)
// CHECK-NEXT:         .fetch_and([[T_6]], std::sync::atomic::Ordering::SeqCst); [[T_7]] }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 15;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { let [[T_8:__t[0-9]+]] = 255i32; let [[T_9:__t[0-9]+]] =
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v) as * mut i32)
// CHECK-NEXT:         .fetch_xor([[T_8]], std::sync::atomic::Ordering::SeqCst); [[T_9]] }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 5;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { let [[T_10:__t[0-9]+]] = 3i32; let [[T_11:__t[0-9]+]] =
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v) as * mut i32)
// CHECK-NEXT:         .fetch_nand([[T_10]], std::sync::atomic::Ordering::SeqCst); [[T_11]] }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 5;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { let [[T_12:__t[0-9]+]] = 3i32; let [[T_13:__t[0-9]+]] =
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v) as * mut i32)
// CHECK-NEXT:         .fetch_sub([[T_12]], std::sync::atomic::Ordering::SeqCst); [[T_13]].wrapping_sub([[T_12]])
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 15;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { let [[T_14:__t[0-9]+]] = 240i32; let [[T_15:__t[0-9]+]] =
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v) as * mut i32)
// CHECK-NEXT:         .fetch_or([[T_14]], std::sync::atomic::Ordering::SeqCst); [[T_15]] | [[T_14]] }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 255;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { let [[T_16:__t[0-9]+]] = 15i32; let [[T_17:__t[0-9]+]] =
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v) as * mut i32)
// CHECK-NEXT:         .fetch_and([[T_16]], std::sync::atomic::Ordering::SeqCst); [[T_17]] & [[T_16]] }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 15;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { let [[T_18:__t[0-9]+]] = 255i32; let [[T_19:__t[0-9]+]] =
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v) as * mut i32)
// CHECK-NEXT:         .fetch_xor([[T_18]], std::sync::atomic::Ordering::SeqCst); [[T_19]] ^ [[T_18]] }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     v = 5;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { let [[T_20:__t[0-9]+]] = 3i32; let [[T_21:__t[0-9]+]] =
// CHECK-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v) as * mut i32)
// CHECK-NEXT:         .fetch_nand([[T_20]], std::sync::atomic::Ordering::SeqCst); ! ([[T_21]] & [[T_20]]) }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", v);
// CHECK-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::SeqCst);
// CHECK-NEXT:     println!("ok");
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
