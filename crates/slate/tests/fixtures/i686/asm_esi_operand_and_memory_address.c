#include <stdio.h>

static int bytes_differ(const void *s1, const void *s2, unsigned len) {
  _Bool diff;
  __asm__("test %%ecx, %%ecx\n\trepe cmpsb\n\tsetnz %0"
          : "=qm"(diff), "+D"(s1), "+S"(s2), "+c"(len));
  return diff;
}

static unsigned sum_via_esi(unsigned a, unsigned b) {
  __asm__("addl %%esi, %0" : "+r"(a) : "S"(b));
  return a;
}

static unsigned clobber_esi(unsigned x) {
  __asm__("movl %0, %%esi\n\tshll $2, %%esi\n\tmovl %%esi, %0" : "+r"(x) : : "esi");
  return x;
}

static int fpu_status(void) {
  unsigned short fsw = 0xffff, fcw = 0xffff;
  __asm__ volatile("fninit ; fnstsw %0 ; fnstcw %1" : "+m"(fsw), "+m"(fcw));
  return fsw == 0 && (fcw & 0x103f) == 0x3f;
}

static void set_bit(int nr, void *addr) {
  __asm__("btsl %1,%0" : "+m"(*(unsigned *)addr) : "Ir"(nr));
}

static double root(double x) {
  __asm__("fsqrt" : "+t"(x));
  return x;
}

int main(void) {
  char a[] = "slate", b[] = "slatf";
  unsigned word = 0;
  set_bit(5, &word);
  printf("%d %d %u %u %d %u %.1f\n", bytes_differ(a, b, 5), bytes_differ(a, b, 4),
         sum_via_esi(40, 2), clobber_esi(5), fpu_status(), word, root(16.0));
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn bytes_differ(mut s1: *const (), mut s2: *const (), mut len: u32) -> i32 {
// CHECK-NEXT:     let mut diff: bool = false;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov %esi, {2:e}\nmov {1:e}, %esi\ntest %ecx, %ecx\n\trepe cmpsb\n\tsetnz {0:l}\nmov %esi, {1:e}\nmov {2:e}, %esi",
// CHECK-NEXT:             lateout(reg_abcd) [[T_0]], inout(reg) s2 as u32 => [[T_2]], out(reg) _,
// CHECK-NEXT:             inlateout("di") s1 as u32 => [[T_1]], inlateout("cx") len as u32 => [[T_3]],
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         diff = ([[T_0]] as u8) != 0;
// CHECK-NEXT:         s1 = unsafe { core::mem::transmute::<u32, *const ()>([[T_1]] as u32) };
// CHECK-NEXT:         s2 = unsafe { core::mem::transmute::<u32, *const ()>([[T_2]] as u32) };
// CHECK-NEXT:         len = unsafe { core::mem::transmute::<u32, u32>([[T_3]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     diff as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_via_esi(mut a: u32, mut b: u32) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov %esi, {2:e}\nmov {1:e}, %esi\naddl %esi, {0:e}\nmov {2:e}, %esi",
// CHECK-NEXT:             inlateout(reg) a as u32 => [[T_0]], in (reg) b as u32, out(reg) _, options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         a = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-NEXT: fn clobber_esi(mut x: u32) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov %esi, {1:e}\nmovl {0:e}, %esi\n\tshll $2, %esi\n\tmovl %esi, {0:e}\nmov {1:e}, %esi",
// CHECK-NEXT:             inlateout(reg) x as u32 => [[T_0]], out(reg) _, options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn fpu_status() -> i32 {
// CHECK-NEXT:     let mut fsw: u16 = 65535;
// CHECK-NEXT:     let mut fcw: u16 = 65535;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "fninit ; fnstsw ({0:e}) ; fnstcw ({1:e})", in (reg)
// CHECK-NEXT:             core::ptr::addr_of_mut!(fsw), in (reg) core::ptr::addr_of_mut!(fcw),
// CHECK-NEXT:             options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     ((fsw as i32) == 0 && (fcw as i32) & 4159 == 63) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn set_bit(mut nr: i32, mut addr: &mut ()) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "btsl {1:e},({0:e})", in (reg) (addr as * mut ()) as * mut u32, in (reg) nr
// CHECK-NEXT:             as u32, options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn root(mut x: f64) -> f64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: f64 = x;
// CHECK-NEXT:         let mut [[T_1]]: f64 = core::mem::zeroed();
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "fldl ({0:e})\nfsqrt\nfstpl ({1:e})", in (reg) core::ptr::addr_of!([[T_0]]), in
// CHECK-NEXT:             (reg) core::ptr::addr_of_mut!([[T_1]]), lateout("st(0)") _, lateout("st(1)") _,
// CHECK-NEXT:             lateout("st(2)") _, lateout("st(3)") _, lateout("st(4)") _, lateout("st(5)")
// CHECK-NEXT:             _, lateout("st(6)") _, lateout("st(7)") _, options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = [[T_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: [i8; 6] = [115, 108, 97, 116, 101, 0];
// CHECK-NEXT:     let mut b: [i8; 6] = [115, 108, 97, 116, 102, 0];
// CHECK-NEXT:     let mut word: u32 = 0;
// CHECK-NEXT:     set_bit(5i32, unsafe { &mut (*(core::ptr::addr_of_mut!(word) as *mut ())) });
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %d %u %u %d %u %.1f\n".as_ptr(),
// CHECK-NEXT:             bytes_differ(a.as_mut_ptr() as *const (), b.as_mut_ptr() as *const (), 5u32),
// CHECK-NEXT:             bytes_differ(a.as_mut_ptr() as *const (), b.as_mut_ptr() as *const (), 4u32),
// CHECK-NEXT:             sum_via_esi(40u32, 2u32),
// CHECK-NEXT:             clobber_esi(5u32),
// CHECK-NEXT:             fpu_status(),
// CHECK-NEXT:             word,
// CHECK-NEXT:             root(16.0f64),
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
