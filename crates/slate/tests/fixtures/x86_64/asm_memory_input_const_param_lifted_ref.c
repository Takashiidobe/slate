#include <stdio.h>

struct desc_ptr {
  unsigned short size;
  unsigned long address;
} __attribute__((packed));

static unsigned long seen;

static inline void load(const struct desc_ptr *dtr) {
  unsigned long got;
  __asm__ __volatile__("movq 2(%1), %0" : "=r"(got) : "r"(dtr), "m"(*dtr));
  seen = got;
}

int main(void) {
  struct desc_ptr desc = {15, 0x1234};
  load(&desc);
  printf("%lx %u\n", seen, desc.size);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct desc_ptr {
// CHECK-NEXT:     size: u16,
// CHECK-NEXT:     address: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut seen: u64 = unsafe { core::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn load(mut dtr: &mut desc_ptr) {
// CHECK-NEXT:     let mut got: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movq 2({1:r}), {0:r}\n# {2}",
// CHECK-NEXT:             lateout(reg) [[T_0]],
// CHECK-NEXT:             in(reg) (dtr as *mut desc_ptr) as u64,
// CHECK-NEXT:             in(reg) dtr as *mut desc_ptr,
// CHECK-NEXT:             options(nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         got = unsafe { core::mem::transmute::<u64, u64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         seen = got;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut desc: desc_ptr = desc_ptr {
// CHECK-NEXT:         size: 15,
// CHECK-NEXT:         address: 4660,
// CHECK-NEXT:     };
// CHECK-NEXT:     load(unsafe { &mut (*(core::ptr::addr_of_mut!(desc) as *mut desc_ptr)) });
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%lx %u\n".as_ptr(), seen, desc.size as i32) };
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
