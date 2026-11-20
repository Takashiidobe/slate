#include <stdio.h>

int ds = 7;
static int counter;

void movsb(void *dst, const void *src, unsigned long n) {
  __asm__ volatile("rep movsb" : "+D"(dst), "+S"(src), "+c"(n) : : "memory");
}

void incl(int *p) { __asm__ volatile("lock incl %0" : "+m"(*p)); }

void bump(void) { __asm__ volatile("lock; incl counter(%%rip)" : : : "memory", "cc"); }

unsigned short read_ds(void) {
  unsigned short v;
  __asm__("movw %%ds,%0" : "=r"(v));
  return v;
}

int main(void) {
  char src[] = "hello";
  char dst[sizeof src] = {0};
  movsb(dst, src, sizeof src);
  int x = 41;
  incl(&x);
  bump();
  bump();
  printf("%s %d %d %d %u\n", dst, x, counter, ds, (unsigned)read_ds());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut ds: i32 = 7;
// CHECK-EMPTY:
// CHECK-NEXT: static mut counter: i32 = unsafe { core::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn movsb(mut dst: *mut (), mut src: *const (), mut n: u64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u64;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "rep movsb",
// CHECK-NEXT:             inlateout("di") dst as u64 => [[T_0]],
// CHECK-NEXT:             inlateout("si") src as u64 => [[T_1]],
// CHECK-NEXT:             inlateout("cx") n as u64 => [[T_2]],
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         dst = unsafe { core::mem::transmute::<u64, *mut ()>([[T_0]] as u64) };
// CHECK-NEXT:         src = unsafe { core::mem::transmute::<u64, *const ()>([[T_1]] as u64) };
// CHECK-NEXT:         n = unsafe { core::mem::transmute::<u64, u64>([[T_2]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn incl(mut p: &mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "lock incl ({0:r})",
// CHECK-NEXT:             in(reg) p as *mut i32,
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bump() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "lock; incl {0}(%rip)",
// CHECK-NEXT:             sym counter,
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn read_ds() -> u16 {
// CHECK-NEXT:     let mut v: u16 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u16;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movw %ds,{0:x}",
// CHECK-NEXT:             lateout(reg) [[T_0]],
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         v = unsafe { core::mem::transmute::<u16, u16>([[T_0]] as u16) };
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut src: [i8; 6] = [104, 101, 108, 108, 111, 0];
// CHECK-NEXT:     let mut dst: [i8; 6] = {
// CHECK-NEXT:         let mut [[T_0]]: [i8; 6] = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     movsb(
// CHECK-NEXT:         dst.as_mut_ptr() as *mut (),
// CHECK-NEXT:         src.as_mut_ptr() as *const (),
// CHECK-NEXT:         core::mem::size_of::<[i8; 6]>() as u64,
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut x: i32 = 41;
// CHECK-NEXT:     incl(unsafe { &mut (*core::ptr::addr_of_mut!(x)) });
// CHECK-NEXT:     bump();
// CHECK-NEXT:     bump();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%s %d %d %d %u\n".as_ptr(),
// CHECK-NEXT:             dst.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             x,
// CHECK-NEXT:             counter,
// CHECK-NEXT:             ds,
// CHECK-NEXT:             read_ds() as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
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
