#include <stdio.h>

static unsigned long swap_through_rbx(unsigned long in, unsigned long *out_rcx) {
  unsigned long rbx, rcx;
  __asm__("xchgq %%rbx, %%rcx"
          : "=b"(rbx), "=c"(rcx)
          : "b"(in), "c"(in + 1)
          : "cc");
  *out_rcx = rcx;
  return rbx;
}

static unsigned try_store(unsigned *lock, unsigned tid, unsigned long *pop) {
  __asm__ volatile("lock cmpxchgl %k[zero], %a[lock]\n\t"
                   "jnz 1f\n\t"
                   "movq %[zero], %a[pop]\n"
                   "1:"
                   : [tid] "+&a"(tid)
                   : [lock] "D"(lock), [pop] "d"(pop), [zero] "r"(0UL)
                   : "memory");
  return tid;
}

int abs(int x) { return x < 0 ? -x : x; }

__asm__(".section .data.slate_exports,\"aw\"\n"
        ".balign 8\n"
        "slate_abs_ref:\n"
        ".quad abs\n"
        ".previous\n");

extern int (*const slate_abs_ref)(int);

int main(void) {
  unsigned long rcx;
  unsigned long rbx = swap_through_rbx(5, &rcx);
  unsigned lock = 7;
  unsigned long pop = 99;
  unsigned seen = try_store(&lock, 7, &pop);
  unsigned missed = try_store(&lock, 3, &pop);
  printf("%lu %lu %u %u %u %lu %d\n", rbx, rcx, seen, missed, lock, pop,
         slate_abs_ref(-12));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut slate_abs_ref: Option<unsafe extern "C-unwind" fn(i32) -> i32>;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: core::arch::global_asm!(
// CHECK-NEXT:     ".section .data.slate_exports,\"aw\"\n.balign 8\nslate_abs_ref:\n.quad {0}\n.previous\n",
// CHECK-NEXT:     sym abs, options(att_syntax)
// CHECK-NEXT: );
// CHECK-NEXT: fn swap_through_rbx(mut r#in: u64, out_rcx: &mut u64) -> u64 {
// CHECK-NEXT:     let mut rbx: u64 = 0;
// CHECK-NEXT:     let mut rcx: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "xchg %rbx, {0:r}\nxchgq %rbx, %rcx\nxchg %rbx, {0:r}", inout(reg) r#in as
// CHECK-NEXT:             u64 => [[T_0]], lateout("cx") [[T_1]], in ("cx") r#in.wrapping_add((1 as i32) as
// CHECK-NEXT:             u64) as u64, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         rbx = unsafe { core::mem::transmute::<u64, u64>([[T_0]] as u64) };
// CHECK-NEXT:         rcx = unsafe { core::mem::transmute::<u64, u64>([[T_1]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(out_rcx as *mut u64) = rcx;
// CHECK-NEXT:     }
// CHECK-NEXT:     rbx
// CHECK-NEXT: }
// CHECK-NEXT: fn try_store(mut lock: &mut u32, mut tid: u32, mut pop: &mut u64) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "lock cmpxchgl {0:e}, (%rdi)\n\tjnz 1f\n\tmovq {0:r}, (%rdx)\n1:", in (reg) 0
// CHECK-NEXT:             as u64, inout("ax") tid as u32 => [[T_0]], in ("di") (lock as * mut u32) as u64,
// CHECK-NEXT:             in ("dx") (pop as * mut u64) as u64, options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         tid = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     tid
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn abs(x: i32) -> i32 {
// CHECK-NEXT:     if x < 0 { -x } else { x }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut rcx: u64 = 0;
// CHECK-NEXT:     let rbx: u64 = swap_through_rbx(
// CHECK-NEXT:         5u64,
// CHECK-NEXT:         unsafe { &mut (*core::ptr::addr_of_mut!(rcx)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut lock: u32 = 7;
// CHECK-NEXT:     let mut pop: u64 = 99;
// CHECK-NEXT:     let seen: u32 = try_store(
// CHECK-NEXT:         unsafe { &mut (*core::ptr::addr_of_mut!(lock)) },
// CHECK-NEXT:         7u32,
// CHECK-NEXT:         unsafe { &mut (*core::ptr::addr_of_mut!(pop)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     let missed: u32 = try_store(
// CHECK-NEXT:         unsafe { &mut (*core::ptr::addr_of_mut!(lock)) },
// CHECK-NEXT:         3u32,
// CHECK-NEXT:         unsafe { &mut (*core::ptr::addr_of_mut!(pop)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lu %lu %u %u %u %lu %d\n".as_ptr(),
// CHECK-NEXT:             rbx,
// CHECK-NEXT:             rcx,
// CHECK-NEXT:             seen,
// CHECK-NEXT:             missed,
// CHECK-NEXT:             lock,
// CHECK-NEXT:             pop,
// CHECK-NEXT:             unsafe { slate_abs_ref }.unwrap()(-12i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
