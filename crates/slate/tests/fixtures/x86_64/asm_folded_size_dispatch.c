#include <stdio.h>

extern void __bad_size(void);

#define IS_ZERO(p, label)                                                      \
  do {                                                                         \
    switch (sizeof(*(p))) {                                                    \
    case 1:                                                                    \
      __asm__ goto("cmpb $0, %0\n\tje %l1" : : "q"(*(p)) : "cc" : label);          \
      break;                                                                   \
    case 2:                                                                    \
      __asm__ goto("cmpw $0, %0\n\tje %l1" : : "r"(*(p)) : "cc" : label);          \
      break;                                                                   \
    case 4:                                                                    \
      __asm__ goto("cmpl $0, %0\n\tje %l1" : : "r"(*(p)) : "cc" : label);          \
      break;                                                                   \
    case 8:                                                                    \
      __asm__ goto("cmpq $0, %0\n\tje %l1" : : "r"(*(p)) : "cc" : label);          \
      break;                                                                   \
    default:                                                                   \
      __bad_size();                                                            \
    }                                                                          \
  } while (0)

#define LOAD(x, p)                                                             \
  do {                                                                         \
    if (sizeof(*(p)) == 4)                                                     \
      __asm__("movl %1, %0" : "=r"(x) : "m"(*(p)));                                \
    else if (sizeof(*(p)) == 8)                                                \
      __asm__("movq %1, %0" : "=r"(x) : "m"(*(p)));                                \
    else                                                                       \
      __bad_size();                                                            \
  } while (0)

static int zero_u8(unsigned char *p) {
  IS_ZERO(p, yes);
  return 0;
yes:
  return 1;
}

static int zero_u32(unsigned int *p) {
  IS_ZERO(p, yes);
  return 0;
yes:
  return 1;
}

static int zero_u64(unsigned long *p) {
  IS_ZERO(p, yes);
  return 0;
yes:
  return 1;
}

static unsigned long load_u64(unsigned long *p) {
  unsigned long x;
  LOAD(x, p);
  return x;
}

static unsigned int load_u32(unsigned int *p) {
  unsigned int x;
  LOAD(x, p);
  return x;
}

static int pick(unsigned long *p) {
  int r = 0;
  switch (sizeof(*p)) {
  case 4:
    __asm__("movl %1, %0" : "=r"(r) : "r"(*p));
    break;
  case 8:
    r = (int)*p;
    break;
  }
  return r;
}

int main(void) {
  unsigned char a = 0, b = 3;
  unsigned int c = 0, d = 5;
  unsigned long e = 0, f = 7;
  printf("%d %d %d %d %d %d\n", zero_u8(&a), zero_u8(&b), zero_u32(&c),
         zero_u32(&d), zero_u64(&e), zero_u64(&f));
  printf("%lu %u %d\n", load_u64(&f), load_u32(&d), pick(&f));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __bad_size();
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn zero_u8(mut p: &u8) -> i32 {
// CHECK-NEXT:     let mut __slate_state: usize = 3usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return 0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     if false {
// CHECK-NEXT:                         __slate_state = 3usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 1usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     __slate_state = 2usize;
// CHECK-NEXT:                     core::arch::asm!(
// CHECK-NEXT:                         "cmpb $0, {0:l}\n\tje {1}", in (reg) (* (p as * const u8) as u8)
// CHECK-NEXT:                         as u32, label { __slate_state = 0usize; }, options(nostack,
// CHECK-NEXT:                         att_syntax)
// CHECK-NEXT:                     );
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { core::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn zero_u32(mut p: &u32) -> i32 {
// CHECK-NEXT:     let mut __slate_state: usize = 3usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return 0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     if false {
// CHECK-NEXT:                         __slate_state = 3usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 1usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     __slate_state = 2usize;
// CHECK-NEXT:                     core::arch::asm!(
// CHECK-NEXT:                         "cmpl $0, {0:e}\n\tje {1}", in (reg) * (p as * const u32) as u32,
// CHECK-NEXT:                         label { __slate_state = 0usize; }, options(nostack, att_syntax)
// CHECK-NEXT:                     );
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { core::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn zero_u64(mut p: &u64) -> i32 {
// CHECK-NEXT:     let mut __slate_state: usize = 3usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return 0;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     if false {
// CHECK-NEXT:                         __slate_state = 3usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 1usize;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     __slate_state = 2usize;
// CHECK-NEXT:                     core::arch::asm!(
// CHECK-NEXT:                         "cmpq $0, {0:r}\n\tje {1}", in (reg) * (p as * const u64) as u64,
// CHECK-NEXT:                         label { __slate_state = 0usize; }, options(nostack, att_syntax)
// CHECK-NEXT:                     );
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { core::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn load_u64(mut p: &u64) -> u64 {
// CHECK-NEXT:     let mut x: u64 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         if ((const { core::mem::size_of::<u64>() }) as u64) == 4 {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:                 core::arch::asm!(
// CHECK-NEXT:                     "movl ({1:r}), {0:r}", lateout(reg) [[T_0]], in (reg) p as * const u64,
// CHECK-NEXT:                     options(pure, readonly, nostack, att_syntax)
// CHECK-NEXT:                 );
// CHECK-NEXT:                 x = unsafe { core::mem::transmute::<u64, u64>([[T_0]] as u64) };
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if ((const { core::mem::size_of::<u64>() }) as u64) == 8 {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     let [[T_1:__t[0-9]+]]: u64;
// CHECK-NEXT:                     core::arch::asm!(
// CHECK-NEXT:                         "movq ({1:r}), {0:r}", lateout(reg) [[T_1]], in (reg) p as * const
// CHECK-NEXT:                         u64, options(pure, readonly, nostack, att_syntax)
// CHECK-NEXT:                     );
// CHECK-NEXT:                     x = unsafe { core::mem::transmute::<u64, u64>([[T_1]] as u64) };
// CHECK-NEXT:                 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 unsafe { __bad_size() };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn load_u32(mut p: &u32) -> u32 {
// CHECK-NEXT:     let mut x: u32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         if ((const { core::mem::size_of::<u32>() }) as u64) == 4 {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 let [[T_0]]: u32;
// CHECK-NEXT:                 core::arch::asm!(
// CHECK-NEXT:                     "movl ({1:r}), {0:e}", lateout(reg) [[T_0]], in (reg) p as * const u32,
// CHECK-NEXT:                     options(pure, readonly, nostack, att_syntax)
// CHECK-NEXT:                 );
// CHECK-NEXT:                 x = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if ((const { core::mem::size_of::<u32>() }) as u64) == 8 {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     let [[T_1]]: u32;
// CHECK-NEXT:                     core::arch::asm!(
// CHECK-NEXT:                         "movq ({1:r}), {0:e}", lateout(reg) [[T_1]], in (reg) p as * const
// CHECK-NEXT:                         u32, options(pure, readonly, nostack, att_syntax)
// CHECK-NEXT:                     );
// CHECK-NEXT:                     x = unsafe { core::mem::transmute::<u32, u32>([[T_1]] as u32) };
// CHECK-NEXT:                 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 unsafe { __bad_size() };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn pick(mut p: &u64) -> i32 {
// CHECK-NEXT:     let mut r: i32 = 0;
// CHECK-NEXT:     match (const { core::mem::size_of::<u64>() }) as u64 {
// CHECK-NEXT:         4 => {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 let [[T_0]]: u32;
// CHECK-NEXT:                 core::arch::asm!(
// CHECK-NEXT:                     "movl {1:r}, {0:e}", lateout(reg) [[T_0]], in (reg) * (p as * const u64)
// CHECK-NEXT:                     as u64, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:                 );
// CHECK-NEXT:                 r = unsafe { core::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         8 => {
// CHECK-NEXT:             r = (unsafe { *(p as *const u64) }) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: u8 = 0;
// CHECK-NEXT:     let mut b: u8 = 3;
// CHECK-NEXT:     let mut c: u32 = 0;
// CHECK-NEXT:     let mut d: u32 = 5;
// CHECK-NEXT:     let mut e: u64 = 0;
// CHECK-NEXT:     let mut f: u64 = 7;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {}", zero_u8(unsafe { & (* core::ptr::addr_of_mut!(a)) }),
// CHECK-NEXT:         zero_u8(unsafe { & (* core::ptr::addr_of_mut!(b)) }), zero_u32(unsafe { & (*
// CHECK-NEXT:         core::ptr::addr_of_mut!(c)) }), zero_u32(unsafe { & (*
// CHECK-NEXT:         core::ptr::addr_of_mut!(d)) }), zero_u64(unsafe { & (*
// CHECK-NEXT:         core::ptr::addr_of_mut!(e)) }), zero_u64(unsafe { & (*
// CHECK-NEXT:         core::ptr::addr_of_mut!(f)) })
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lu %u %d\n".as_ptr(),
// CHECK-NEXT:             load_u64(&(*core::ptr::addr_of_mut!(f))),
// CHECK-NEXT:             load_u32(&(*core::ptr::addr_of_mut!(d))),
// CHECK-NEXT:             pick(&(*core::ptr::addr_of_mut!(f))),
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
