#include <stdio.h>

static int open_regions;

static void region_begin(void) { open_regions++; }

static void region_end(void) { open_regions--; }

static unsigned long fill_until_fault(char *buf, unsigned long size,
                                      unsigned long fault_at) {
  unsigned long i;

  if (size == 0)
    return 0;
  region_begin();
  for (i = 0; i < size; i++)
    __asm__ goto("cmpq %[i], %[fault]\n\tje %l[out]\n\tmovb $1, (%[p])"
                 :
                 : [i] "r"(i), [fault] "r"(fault_at), [p] "r"(buf + i)
                 : "cc", "memory"
                 : out);
out:
  region_end();
  return size - i;
}

int main(void) {
  char buf[8] = {0};
  unsigned long left = fill_until_fault(buf, 8, 5);
  unsigned long none = fill_until_fault(buf, 0, 0);
  unsigned long all = fill_until_fault(buf, 3, 99);
  printf("%lu %lu %lu %d %d%d%d%d%d%d%d%d\n", left, none, all, open_regions,
         buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
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
// CHECK-NEXT: static mut open_regions: i32 = unsafe { core::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn region_begin() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         open_regions = open_regions + 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn region_end() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         open_regions = open_regions - 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fill_until_fault(mut buf: *mut i8, size: u64, mut fault_at: u64) -> u64 {
// CHECK-NEXT:     let mut i: core::mem::MaybeUninit<u64> = core::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut u64 = core::ptr::addr_of_mut!(i) as *mut u64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         if size == 0 {
// CHECK-NEXT:             return 0;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             region_begin();
// CHECK-NEXT:             *[[SLATE_SLOT_0]] = 0;
// CHECK-NEXT:             while *[[SLATE_SLOT_0]] < size {
// CHECK-NEXT:                 let mut __slate_asm_target: bool = false;
// CHECK-NEXT:                 core::arch::asm!(
// CHECK-NEXT:                     "cmpq {0:r}, {1:r}\n\tje {3}\n\tmovb $1, ({2:r})",
// CHECK-NEXT:                     in(reg) *[[SLATE_SLOT_0]] as u64,
// CHECK-NEXT:                     in(reg) fault_at as u64,
// CHECK-NEXT:                     in(reg) buf.offset(*[[SLATE_SLOT_0]] as isize) as u64,
// CHECK-NEXT:                     label {
// CHECK-NEXT:                         __slate_asm_target = true;
// CHECK-NEXT:                     },
// CHECK-NEXT:                     options(nostack, att_syntax),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 if __slate_asm_target {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     let [[V_0:__v[0-9]+]]: u64 = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                     *[[SLATE_SLOT_0]] = [[V_0]].wrapping_add(1);
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             region_end();
// CHECK-NEXT:             return size.wrapping_sub(*[[SLATE_SLOT_0]]);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: [i8; 8] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i8; 8] = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let left: u64 = fill_until_fault(buf.as_mut_ptr() as *mut i8, 8u64, 5u64);
// CHECK-NEXT:     let none: u64 = fill_until_fault(buf.as_mut_ptr() as *mut i8, 0u64, 0u64);
// CHECK-NEXT:     let all: u64 = fill_until_fault(buf.as_mut_ptr() as *mut i8, 3u64, 99u64);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lu %lu %lu %d %d%d%d%d%d%d%d%d\n".as_ptr(),
// CHECK-NEXT:             left,
// CHECK-NEXT:             none,
// CHECK-NEXT:             all,
// CHECK-NEXT:             open_regions,
// CHECK-NEXT:             buf[0] as i32,
// CHECK-NEXT:             buf[1] as i32,
// CHECK-NEXT:             buf[2] as i32,
// CHECK-NEXT:             buf[3] as i32,
// CHECK-NEXT:             buf[4] as i32,
// CHECK-NEXT:             buf[5] as i32,
// CHECK-NEXT:             buf[6] as i32,
// CHECK-NEXT:             buf[7] as i32,
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
