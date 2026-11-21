#include <stdio.h>

struct dev {
  unsigned flags;
  unsigned short gflags;
};

__attribute__((noinline)) int change_flags(struct dev *d, unsigned flags) {
  d->flags = flags;
  d->gflags ^= 2;
  return 0;
}

__attribute__((noinline)) void notify_flags(struct dev *d, unsigned old_flags, unsigned changes) {
  printf("old=%#x now=%#x changes=%#x\n", old_flags, d->flags, changes);
  if (changes & 1)
    printf("%s\n", (d->flags & 1) ? "up" : "down");
}

int set_flags(struct dev *dev, unsigned flags) {
  int ret;
  unsigned int changes, old_flags = dev->flags, old_gflags = dev->gflags;

  ret = change_flags(dev, flags);
  if (ret < 0)
    return ret;

  changes = (old_flags ^ dev->flags) | (old_gflags ^ dev->gflags);
  notify_flags(dev, old_flags, changes);
  return ret;
}

int main(void) {
  struct dev d = {8, 0};
  set_flags(&d, 9);
  set_flags(&d, 8);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct dev {
// CHECK-NEXT:     flags: u32,
// CHECK-NEXT:     gflags: u16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[inline(never)]
// CHECK-NEXT: fn change_flags(d: &mut dev, flags: u32) -> i32 {
// CHECK-NEXT:     (*d).flags = flags;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*d).gflags = (((*d).gflags as i32) ^ 2) as u16;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[inline(never)]
// CHECK-NEXT: fn notify_flags(d: &dev, old_flags: u32, changes: u32) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"old=%#x now=%#x changes=%#x\n".as_ptr(),
// CHECK-NEXT:             old_flags,
// CHECK-NEXT:             (*d).flags,
// CHECK-NEXT:             changes,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     if changes & 1 != 0 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(
// CHECK-NEXT:                 c"%s\n".as_ptr(),
// CHECK-NEXT:                 if (*d).flags & 1 != 0 {
// CHECK-NEXT:                     c"up".as_ptr() as *mut i8
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     c"down".as_ptr() as *mut i8
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn set_flags(dev: &mut dev, flags: u32) -> i32 {
// CHECK-NEXT:     let mut ret: i32 = 0;
// CHECK-NEXT:     let mut changes: u32 = 0;
// CHECK-NEXT:     let old_flags: u32 = (*dev).flags;
// CHECK-NEXT:     let old_gflags: u32 = (*dev).gflags as u32;
// CHECK-NEXT:     ret = change_flags(dev, flags);
// CHECK-NEXT:     if ret < 0 {
// CHECK-NEXT:         return ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     changes = old_flags ^ (*dev).flags | old_gflags ^ (((*dev).gflags as i32) as u32);
// CHECK-NEXT:     notify_flags(dev, old_flags, changes);
// CHECK-NEXT:     ret
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: dev = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: dev = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].flags = 8u32;
// CHECK-NEXT:         [[T_0]].gflags = 0u16;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     set_flags(&mut d, 9u32);
// CHECK-NEXT:     set_flags(&mut d, 8u32);
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
