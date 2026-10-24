#include <stdarg.h>
#include <stdio.h>

typedef _BitInt(100) s100;
typedef _BitInt(128) s128;
typedef _BitInt(200) s200;
typedef unsigned _BitInt(300) u300;

static long long vsum(int n, ...) {
  va_list ap;
  long long total = 0;
  va_start(ap, n);
  for (int i = 0; i < n; i++) {
    switch (i % 3) {
    case 0: total += (long long)va_arg(ap, s100); break;
    case 1: total += (long long)va_arg(ap, s128); break;
    default: total += (long long)va_arg(ap, s200); break;
    }
  }
  va_end(ap);
  return total;
}

static long long vwide(int n, ...) {
  va_list ap;
  long long total = 0;
  va_start(ap, n);
  for (int i = 0; i < n; i++) {
    u300 v = va_arg(ap, u300);
    total += (long long)(v >> 250) + (long long)(v & 0xffff);
  }
  va_end(ap);
  return total;
}

int main(void) {
  printf("%lld\n", vsum(5, (s100)123456789012345LL, (s128)-77, ((s200)1 << 130) + 9, (s100)-5, (s128)1 << 100));
  printf("%lld\n", vwide(2, ((u300)3 << 255) | 0x1234, (u300)9));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_sysv_va {
// CHECK-NEXT:     #[repr(C)]
// CHECK-NEXT:     struct Tag {
// CHECK-NEXT:         gp_offset: u32,
// CHECK-NEXT:         fp_offset: u32,
// CHECK-NEXT:         overflow: *const u8,
// CHECK-NEXT:         registers: *const u8,
// CHECK-NEXT:     }
// CHECK-NEXT:     const GP_END: u32 = 48;
// CHECK-NEXT:     const FP_END: u32 = 176;
// CHECK-NEXT:     pub unsafe fn arg<L, T>(list: *mut L, classes: u32) -> T {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let tag = &mut *(list as *mut Tag);
// CHECK-NEXT:             let size = core::mem::size_of::<T>();
// CHECK-NEXT:             let mut out = core::mem::MaybeUninit::<T>::uninit();
// CHECK-NEXT:             let dst = out.as_mut_ptr() as *mut u8;
// CHECK-NEXT:             let (mut gp, mut sse) = (0, 0);
// CHECK-NEXT:             let mut rest = classes;
// CHECK-NEXT:             while rest != 0 {
// CHECK-NEXT:                 match rest & 3 {
// CHECK-NEXT:                     1 => gp += 8,
// CHECK-NEXT:                     _ => sse += 16,
// CHECK-NEXT:                 }
// CHECK-NEXT:                 rest >>= 2;
// CHECK-NEXT:             }
// CHECK-NEXT:             if classes != 0 && tag.gp_offset + gp <= GP_END
// CHECK-NEXT:                 && tag.fp_offset + sse <= FP_END
// CHECK-NEXT:             {
// CHECK-NEXT:                 let mut copied = 0;
// CHECK-NEXT:                 let mut rest = classes;
// CHECK-NEXT:                 while rest != 0 {
// CHECK-NEXT:                     let class = rest & 3;
// CHECK-NEXT:                     let (offset, width) = if class == 1 {
// CHECK-NEXT:                         (&mut tag.gp_offset, 8)
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         (&mut tag.fp_offset, if class == 3 { 16 } else { 8 })
// CHECK-NEXT:                     };
// CHECK-NEXT:                     let step = if class == 1 { 8 } else { 16 };
// CHECK-NEXT:                     core::ptr::copy_nonoverlapping(
// CHECK-NEXT:                         tag.registers.add(*offset as usize),
// CHECK-NEXT:                         dst.add(copied),
// CHECK-NEXT:                         width.min(size - copied),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     *offset += step;
// CHECK-NEXT:                     copied += width;
// CHECK-NEXT:                     rest >>= 2;
// CHECK-NEXT:                 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 let align = core::mem::align_of::<T>().max(8);
// CHECK-NEXT:                 let slot = ((tag.overflow as usize + align - 1) & !(align - 1))
// CHECK-NEXT:                     as *const u8;
// CHECK-NEXT:                 core::ptr::copy_nonoverlapping(slot, dst, size);
// CHECK-NEXT:                 tag.overflow = slot.add((size + 7) & !7);
// CHECK-NEXT:             }
// CHECK-NEXT:             out.assume_init()
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn vsum(n: i32, mut __va_args: ...) -> i64 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         match i % 3 {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 total
// CHECK-NEXT:                     += unsafe {
// CHECK-NEXT:                         __slate_sysv_va::arg::<
// CHECK-NEXT:                             _,
// CHECK-NEXT:                             bitint::BInt<100, 2, 16, bitint::A8>,
// CHECK-NEXT:                         >(std::ptr::addr_of_mut!(ap), 5u32)
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_i128() as i64;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 total
// CHECK-NEXT:                     += unsafe {
// CHECK-NEXT:                         __slate_sysv_va::arg::<
// CHECK-NEXT:                             _,
// CHECK-NEXT:                             bitint::BInt<128, 2, 16, bitint::A8>,
// CHECK-NEXT:                         >(std::ptr::addr_of_mut!(ap), 5u32)
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_i128() as i64;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 total
// CHECK-NEXT:                     += unsafe {
// CHECK-NEXT:                         __slate_sysv_va::arg::<
// CHECK-NEXT:                             _,
// CHECK-NEXT:                             bitint::BInt<200, 4, 32, bitint::A8>,
// CHECK-NEXT:                         >(std::ptr::addr_of_mut!(ap), 0u32)
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_i128() as i64;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn vwide(n: i32, mut __va_args: ...) -> i64 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let mut v: bitint::BUint<300, 5, 40, bitint::A8> = unsafe {
// CHECK-NEXT:             __slate_sysv_va::arg::<
// CHECK-NEXT:                 _,
// CHECK-NEXT:                 bitint::BUint<300, 5, 40, bitint::A8>,
// CHECK-NEXT:             >(std::ptr::addr_of_mut!(ap), 0u32)
// CHECK-NEXT:         };
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i64 = total;
// CHECK-NEXT:         total = [[V_0]]
// CHECK-NEXT:             + (((v.shr_bits(250 as usize).to_u128() as u64) as i64)
// CHECK-NEXT:                 + ((v
// CHECK-NEXT:                     .bitand_bits(
// CHECK-NEXT:                         <bitint::BUint<
// CHECK-NEXT:                             300,
// CHECK-NEXT:                             5,
// CHECK-NEXT:                             40,
// CHECK-NEXT:                             bitint::A8,
// CHECK-NEXT:                         >>::from_bint(
// CHECK-NEXT:                             <bitint::BInt<
// CHECK-NEXT:                                 300,
// CHECK-NEXT:                                 5,
// CHECK-NEXT:                                 40,
// CHECK-NEXT:                                 bitint::A8,
// CHECK-NEXT:                             >>::from_i128(65535 as i128),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .to_u128() as u64) as i64));
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lld\n".as_ptr(),
// CHECK-NEXT:             vsum(
// CHECK-NEXT:                 5i32,
// CHECK-NEXT:                 <bitint::BInt<
// CHECK-NEXT:                     100,
// CHECK-NEXT:                     2,
// CHECK-NEXT:                     16,
// CHECK-NEXT:                     bitint::A8,
// CHECK-NEXT:                 >>::from_i128(123456789012345 as i128),
// CHECK-NEXT:                 <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(-77 as i128),
// CHECK-NEXT:                 <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(1 as i128)
// CHECK-NEXT:                     .shl_bits(130 as usize)
// CHECK-NEXT:                     .wrapping_add(
// CHECK-NEXT:                         <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(9 as i128),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 <bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(-5 as i128),
// CHECK-NEXT:                 <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(1 as i128)
// CHECK-NEXT:                     .shl_bits(100 as usize),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lld\n".as_ptr(),
// CHECK-NEXT:             vwide(
// CHECK-NEXT:                 2i32,
// CHECK-NEXT:                 <bitint::BUint<
// CHECK-NEXT:                     300,
// CHECK-NEXT:                     5,
// CHECK-NEXT:                     40,
// CHECK-NEXT:                     bitint::A8,
// CHECK-NEXT:                 >>::from_bint(
// CHECK-NEXT:                         <bitint::BInt<300, 5, 40, bitint::A8>>::from_i128(3 as i128),
// CHECK-NEXT:                     )
// CHECK-NEXT:                     .shl_bits(255 as usize)
// CHECK-NEXT:                     .bitor_bits(
// CHECK-NEXT:                         <bitint::BUint<
// CHECK-NEXT:                             300,
// CHECK-NEXT:                             5,
// CHECK-NEXT:                             40,
// CHECK-NEXT:                             bitint::A8,
// CHECK-NEXT:                         >>::from_bint(
// CHECK-NEXT:                             <bitint::BInt<
// CHECK-NEXT:                                 300,
// CHECK-NEXT:                                 5,
// CHECK-NEXT:                                 40,
// CHECK-NEXT:                                 bitint::A8,
// CHECK-NEXT:                             >>::from_i128(4660 as i128),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 <bitint::BUint<
// CHECK-NEXT:                     300,
// CHECK-NEXT:                     5,
// CHECK-NEXT:                     40,
// CHECK-NEXT:                     bitint::A8,
// CHECK-NEXT:                 >>::from_bint(
// CHECK-NEXT:                     <bitint::BInt<300, 5, 40, bitint::A8>>::from_i128(9 as i128),
// CHECK-NEXT:                 ),
// CHECK-NEXT:             ),
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
