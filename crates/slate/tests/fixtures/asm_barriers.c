#include <stdint.h>
#include <stdio.h>

static int typecheck_probe(void) { __asm__(""); return 1; }

static void empty_volatile(void) { __asm__ __volatile__(""); }

static int memory_barrier(int *p) {
  *p = 5;
  __asm__ __volatile__("" ::: "memory");
  return *p;
}

static uint32_t value_barrier(uint32_t x) {
  __asm__ __volatile__("" : "+r"(x));
  return x * 3;
}

static uint64_t value_barrier_memory(uint64_t x) {
  __asm__ __volatile__("" : "+r"(x) : : "memory");
  return x + 1;
}

static int input_barrier(const unsigned char *p, int n) {
  int sum = 0;
  for (int i = 0; i < n; i++)
    sum += p[i];
  __asm__ __volatile__("" : : "r"(p) : "memory");
  return sum;
}

#define CURLWARNING(id, message)                                               \
  static void __attribute__((__warning__(message)))                            \
  __attribute__((__unused__)) __attribute__((__noinline__))                    \
  id(void) { __asm__(""); }

CURLWARNING(curl_wrong_type, "wrong type")

static int verify_volatile(const unsigned char *x, const unsigned char *y, int n) {
  volatile uint16_t d = 0U;
  for (int i = 0; i < n; i++)
    d |= x[i] ^ y[i];
  __asm__ __volatile__("" : "+r"(d) :);
  return (1 & ((d - 1) >> 8)) - 1;
}

static int nonvolatile_guard(const unsigned char *x, int n) {
  unsigned char d = 0;
  uint16_t e;
  for (int i = 0; i < n; i++)
    d |= x[i];
  e = d;
  __asm__("" : "+r"(e) :);
  return (1 & ((e - 1) >> 8)) - 1;
}

static int no_unroll(const float *p, int n) {
  float sum = 0;
  for (int i = 0; i < n; i++) {
    __asm__(""::"r"(p));
    sum += p[i];
  }
  return (int)sum;
}

static void spin_pause(void) { __asm__ __volatile__("pause"); }

static void spin_pause_memory(void) { __asm__ __volatile__("pause" ::: "memory"); }

static void spin_rep_nop(void) { __asm__ __volatile__("rep; nop" ::: "memory"); }

int main(void) {
  int cell = 0;
  unsigned char bytes[4] = {1, 2, 3, 4};
  empty_volatile();
  spin_pause();
  spin_rep_nop();
  printf("%d %d\n", typecheck_probe(), memory_barrier(&cell));
  printf("%u %llu\n", value_barrier(7), (unsigned long long)value_barrier_memory(41));
  printf("%d\n", input_barrier(bytes, 4));
  unsigned char zeros[4] = {0};
  float floats[3] = {1.5f, 2.5f, 3.0f};
  spin_pause_memory();
  printf("%d %d\n", verify_volatile(bytes, bytes, 4), verify_volatile(bytes, zeros, 4));
  printf("%d %d\n", nonvolatile_guard(zeros, 4), nonvolatile_guard(bytes, 4));
  printf("%d\n", no_unroll(floats, 3));
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
// CHECK-NEXT: fn typecheck_probe() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("", options(att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-NEXT: fn empty_volatile() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("", options(att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn memory_barrier(p: &mut i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(p as *mut i32) = 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("", options(nostack, att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *(p as *mut i32) }
// CHECK-NEXT: }
// CHECK-NEXT: fn value_barrier(mut x: u32) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "\n# {0:e}", inlateout(reg) x as u32 => [[T_0]], options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x.wrapping_mul(3)
// CHECK-NEXT: }
// CHECK-NEXT: fn value_barrier_memory(mut x: u64) -> u64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "\n# {0:r}", inlateout(reg) x as u64 => [[T_0]], options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u64, u64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x.wrapping_add(1)
// CHECK-NEXT: }
// CHECK-NEXT: fn input_barrier(mut p: *const u8, n: i32) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         sum += ((unsafe { *unsafe { p.offset(i as isize) } }) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("\n# {0:r}", in (reg) p as u64, options(nostack, att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-NEXT: fn curl_wrong_type() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("", options(att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn verify_volatile(x: *const u8, y: *const u8, n: i32) -> i32 {
// CHECK-NEXT:     let mut d: u16 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: u16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(d),
// CHECK-NEXT:                 (((([[V_0]] as u32) as i32)
// CHECK-NEXT:                     | (((unsafe { *unsafe { x.offset(i as isize) } }) as u32) as i32)
// CHECK-NEXT:                         ^ (((unsafe { *unsafe { y.offset(i as isize) } }) as u32)
// CHECK-NEXT:                             as i32)) as i16) as u16,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u16;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "\n# {0:x}", inlateout(reg) (unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(d)) }) as u16 => [[T_0]],
// CHECK-NEXT:             options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(d),
// CHECK-NEXT:                 unsafe { std::mem::transmute::<u16, u16>([[T_0]] as u16) },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     (1
// CHECK-NEXT:         & (((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) }) as u32) as i32)
// CHECK-NEXT:             - 1 >> (8 as i32)) - 1
// CHECK-NEXT: }
// CHECK-NEXT: fn nonvolatile_guard(x: *const u8, n: i32) -> i32 {
// CHECK-NEXT:     let mut d: u8 = 0;
// CHECK-NEXT:     let mut e: u16 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         d = ((((d as u32) as i32)
// CHECK-NEXT:             | (((unsafe { *unsafe { x.offset(i as isize) } }) as u32) as i32)) as i8)
// CHECK-NEXT:             as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     e = d as u16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u16;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "\n# {0:x}", inlateout(reg) e as u16 => [[T_0]], options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         e = unsafe { std::mem::transmute::<u16, u16>([[T_0]] as u16) };
// CHECK-NEXT:     }
// CHECK-NEXT:     (1 & ((e as u32) as i32) - 1 >> (8 as i32)) - 1
// CHECK-NEXT: }
// CHECK-NEXT: fn no_unroll(mut p: *const f32, n: i32) -> i32 {
// CHECK-NEXT:     let mut sum: f32 = (0 as i32) as f32;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::arch::asm!(
// CHECK-NEXT:                 "\n# {0:r}", in (reg) p as u64, options(nostack, att_syntax)
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:         sum += unsafe { *unsafe { p.offset(i as isize) } };
// CHECK-NEXT:     }
// CHECK-NEXT:     sum as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn spin_pause() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("pause", options(att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn spin_pause_memory() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("pause", options(nostack, att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn spin_rep_nop() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("rep; nop", options(nostack, att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut cell: i32 = 0;
// CHECK-NEXT:     let mut bytes: [u8; 4] = [1, 2, 3, 4];
// CHECK-NEXT:     empty_volatile();
// CHECK-NEXT:     spin_pause();
// CHECK-NEXT:     spin_rep_nop();
// CHECK-NEXT:     let __slate_printf_arg1 = memory_barrier(unsafe {
// CHECK-NEXT:         &mut (*std::ptr::addr_of_mut!(cell))
// CHECK-NEXT:     });
// CHECK-NEXT:     println!("{} {}", typecheck_probe(), __slate_printf_arg1);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %llu\n".as_ptr(),
// CHECK-NEXT:             value_barrier(7 as u32),
// CHECK-NEXT:             value_barrier_memory(41 as u64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let __slate_printf_arg0 = input_barrier(bytes.as_mut_ptr() as *const u8, 4 as i32);
// CHECK-NEXT:     println!("{}", __slate_printf_arg0);
// CHECK-NEXT:     let mut zeros: [u8; 4] = {
// CHECK-NEXT:         let mut [[T_0]]: [u8; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0 as u8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut floats: [f32; 3] = [1.5f32, 2.5f32, 3.0f32];
// CHECK-NEXT:     spin_pause_memory();
// CHECK-NEXT:     let __slate_printf_arg0 = verify_volatile(
// CHECK-NEXT:         bytes.as_mut_ptr() as *const u8,
// CHECK-NEXT:         bytes.as_mut_ptr() as *const u8,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let __slate_printf_arg1 = verify_volatile(
// CHECK-NEXT:         bytes.as_mut_ptr() as *const u8,
// CHECK-NEXT:         zeros.as_mut_ptr() as *const u8,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {}", __slate_printf_arg0, __slate_printf_arg1);
// CHECK-NEXT:     let __slate_printf_arg0 = nonvolatile_guard(
// CHECK-NEXT:         zeros.as_mut_ptr() as *const u8,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let __slate_printf_arg1 = nonvolatile_guard(
// CHECK-NEXT:         bytes.as_mut_ptr() as *const u8,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {}", __slate_printf_arg0, __slate_printf_arg1);
// CHECK-NEXT:     let __slate_printf_arg0 = no_unroll(floats.as_mut_ptr() as *const f32, 3 as i32);
// CHECK-NEXT:     println!("{}", __slate_printf_arg0);
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
