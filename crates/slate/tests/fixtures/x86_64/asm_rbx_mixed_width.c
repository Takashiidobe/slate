#include <stdio.h>

static unsigned long narrow_output(unsigned long in, unsigned *out) {
  unsigned long a;
  __asm__("movq %%rbx, %%rax\n\taddl $1, %%ebx" : "=a"(a), "=b"(*out) : "b"(in));
  return a;
}

static unsigned long wide_output(unsigned in) {
  unsigned long out;
  __asm__("movl %%ebx, %%ebx\n\taddq $1, %%rbx" : "=b"(out) : "b"(in));
  return out;
}

int main(void) {
  unsigned low;
  unsigned long a = narrow_output(0x100000005ul, &low);
  printf("%lx %u %lx\n", a, low, wide_output(0xffffffffu));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn narrow_output(mut r#in: u64, out: &mut u32) -> u64 {
// CHECK-NEXT:     let mut a: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "xchg {0:r}, %rbx\nmovq %rbx, %rax\n\taddl $1, %ebx\nxchg {0:r}, %rbx",
// CHECK-NEXT:             inout(reg) r#in as u64 => [[T_1]], lateout("ax") [[T_0]], options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         a = unsafe { std::mem::transmute::<u64, u64>([[T_0]] as u64) };
// CHECK-NEXT:         *(out as *mut u32) = unsafe { std::mem::transmute::<u32, u32>([[T_1]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-NEXT: fn wide_output(mut r#in: u32) -> u64 {
// CHECK-NEXT:     let mut out: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "xchg {0:r}, %rbx\nmovl %ebx, %ebx\n\taddq $1, %rbx\nxchg {0:r}, %rbx",
// CHECK-NEXT:             inout(reg) (r#in as u32) as u64 => [[T_0]], options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         out = unsafe { std::mem::transmute::<u64, u64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut low: u32 = 0;
// CHECK-NEXT:     let a: u64 = narrow_output(
// CHECK-NEXT:         4294967301u64,
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(low)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lx %u %lx\n".as_ptr(),
// CHECK-NEXT:             a,
// CHECK-NEXT:             low,
// CHECK-NEXT:             wide_output(4294967295u32),
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
