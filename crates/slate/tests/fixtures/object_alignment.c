#include <stdint.h>
#include <stdio.h>

typedef unsigned long long v2uq __attribute__((vector_size(16)));

static char pad0;
static uint64_t table[64][64];
static char pad1;
static uint64_t small[2] = {3, 4};
static char pad2;
static _Alignas(32) uint32_t aligned32[3];
static char pad3;
static uint8_t bytes[16];

static int aligned(const void *pointer, uintptr_t alignment) {
  return ((uintptr_t)pointer & (alignment - 1)) == 0;
}

static uint64_t sum_vectors(const uint64_t *values, int count) {
  v2uq total = {0, 0};
  for (int i = 0; i < count; i += 2) {
    total += *(const v2uq *)(values + i);
  }
  return total[0] + total[1];
}

static int locals(int depth) {
  char tag = (char)depth;
  uint64_t row[4] = {1, 2, 3, (uint64_t)depth};
  char tag2 = tag;
  _Alignas(64) unsigned char line[8] = {0};
  short marker = 7;
  uint8_t block[32];
  __attribute__((aligned(16))) uint32_t word = 5;
  for (int i = 0; i < 32; i++) {
    block[i] = (uint8_t)(i + depth);
  }
  int ok = aligned(row, 16) && aligned(line, 64) && aligned(block, 16) && aligned(&word, 16);
  ok = ok && sum_vectors(row, 4) == 6u + (uint64_t)depth;
  if (depth > 0) {
    ok = ok && locals(depth - 1);
  }
  return ok + tag2 - tag + marker - 7 + line[0] + block[0] - depth + (int)word - 5;
}

static int dispatched(int n) {
  int count = 0;
  uint64_t values[2] = {10, 20};
again:
  if (!aligned(values, 16)) {
    return -1;
  }
  values[0] += sum_vectors(values, 2);
  if (++count < n) {
    goto again;
  }
  return (int)(values[0] % 1000);
}

int main(void) {
  for (int i = 0; i < 64; i++) {
    for (int j = 0; j < 64; j++) {
      table[i][j] = (uint64_t)(i * 64 + j);
    }
  }
  printf("table %d\n", aligned(table, 16));
  printf("small %d\n", aligned(small, 16));
  printf("aligned32 %d\n", aligned(aligned32, 32));
  printf("bytes %d\n", aligned(bytes, 16));
  printf("table sum %llu\n", (unsigned long long)sum_vectors(table[5], 64));
  printf("small sum %llu\n", (unsigned long long)sum_vectors(small, 2));
  printf("locals %d\n", locals(5));
  printf("dispatched %d\n", dispatched(4));
  return pad0 + pad1 + pad2 + pad3;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![allow(arithmetic_overflow, unconditional_panic)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[repr(C, align(32))]
// CHECK-NEXT: struct __SlateAlign32<T>(T);
// CHECK-NEXT: #[repr(C, align(64))]
// CHECK-NEXT: struct __SlateAlign64<T>(T);
// CHECK-NEXT: static mut pad0: i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut table: __SlateAlign16<{{\[\[}}u64; 64]; 64]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut pad1: i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut small: __SlateAlign16<[u64; 2]> = __SlateAlign16([
// CHECK-NEXT:     ((3 as i32) as i64) as u64,
// CHECK-NEXT:     ((4 as i32) as i64) as u64,
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut pad2: i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut aligned32: __SlateAlign32<[u32; 3]> = __SlateAlign32(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut pad3: i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut bytes: __SlateAlign16<[u8; 16]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn aligned(pointer: *const (), alignment: u64) -> i32 {
// CHECK-NEXT:     ((pointer as u64) & alignment.wrapping_sub(1) == 0) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_vectors(values: *const u64, count: i32) -> u64 {
// CHECK-NEXT:     let mut total: std::simd::Simd<u64, 2> = std::simd::Simd::from_array([
// CHECK-NEXT:         0 as u64,
// CHECK-NEXT:         0 as u64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < count {
// CHECK-NEXT:         total
// CHECK-NEXT:             += unsafe {
// CHECK-NEXT:                 *((unsafe { values.offset(i as isize) })
// CHECK-NEXT:                     as *const std::simd::Simd<u64, 2>)
// CHECK-NEXT:             };
// CHECK-NEXT:         i += 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     total[0].wrapping_add(total[1])
// CHECK-NEXT: }
// CHECK-NEXT: fn locals(depth: i32) -> i32 {
// CHECK-NEXT:     let tag: i8 = depth as i8;
// CHECK-NEXT:     let mut row: __SlateAlign16<[u64; 4]> = __SlateAlign16([
// CHECK-NEXT:         1 as u64,
// CHECK-NEXT:         2 as u64,
// CHECK-NEXT:         3 as u64,
// CHECK-NEXT:         (depth as i64) as u64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let tag2: i8 = tag;
// CHECK-NEXT:     let mut line: __SlateAlign64<[u8; 8]> = __SlateAlign64({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [u8; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0 as u8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let marker: i16 = 7;
// CHECK-NEXT:     let mut block: __SlateAlign16<[u8; 32]> = __SlateAlign16([0 as u8; 32]);
// CHECK-NEXT:     let mut word: __SlateAlign16<u32> = __SlateAlign16(5 as u32);
// CHECK-NEXT:     for i in 0 as i32..32 as i32 {
// CHECK-NEXT:         block.0[i as usize] = ((i + depth) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut ok: i32 = 0;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if aligned(row.0.as_mut_ptr() as *const (), 16 as u64) != 0 {
// CHECK-NEXT:         [[V_0]] = aligned(line.0.as_mut_ptr() as *const (), 64 as u64) != 0;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         [[V_1]] = aligned(block.0.as_mut_ptr() as *const (), 16 as u64) != 0;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         [[V_2]] = aligned(std::ptr::addr_of_mut!(word.0) as *const (), 16 as u64) != 0;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     ok = [[V_2]] as i32;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:     if ok != 0 {
// CHECK-NEXT:         [[V_3]] = sum_vectors(row.0.as_mut_ptr() as *const u64, 4 as i32)
// CHECK-NEXT:             == (6 as u64).wrapping_add((depth as i64) as u64);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_3]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     ok = [[V_3]] as i32;
// CHECK-NEXT:     if depth > 0 {
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:         if ok != 0 {
// CHECK-NEXT:             [[V_4]] = locals(depth - 1) != 0;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[V_4]] = false as bool;
// CHECK-NEXT:         }
// CHECK-NEXT:         ok = [[V_4]] as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     ok + (tag2 as i32) - (tag as i32) + (marker as i32) - 7 + ((line.0[0] as u32) as i32)
// CHECK-NEXT:         + ((block.0[0] as u32) as i32) - depth + (word.0 as i32) - 5
// CHECK-NEXT: }
// CHECK-NEXT: fn dispatched(n: i32) -> i32 {
// CHECK-NEXT:     let mut values: std::mem::MaybeUninit<__SlateAlign16<[u64; 2]>> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut [u64; 2] = std::ptr::addr_of_mut!(values) as *mut [u64; 2];
// CHECK-NEXT:     let mut count: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0;
// CHECK-NEXT:         std::ptr::write([[SLATE_SLOT_0]], [10 as u64, 20 as u64]);
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if !(aligned((*[[SLATE_SLOT_0]]).as_mut_ptr() as *const (), 16 as u64) != 0) {
// CHECK-NEXT:                 return -1;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 let [[V_5:__v[0-9]+]]: u64 = sum_vectors(
// CHECK-NEXT:                     (*[[SLATE_SLOT_0]]).as_mut_ptr() as *const u64,
// CHECK-NEXT:                     2 as i32,
// CHECK-NEXT:                 );
// CHECK-NEXT:                 let [[V_6:__v[0-9]+]]: *mut u64 = unsafe {
// CHECK-NEXT:                     ((*[[SLATE_SLOT_0]]).as_mut_ptr() as *mut u64).offset(0 as isize)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 let [[V_7:__v[0-9]+]]: u64 = unsafe { *[[V_6]] };
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *[[V_6]] = [[V_7]].wrapping_add([[V_5]]);
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let [[V_8:__v[0-9]+]]: i32 = count + 1;
// CHECK-NEXT:                 count = [[V_8]];
// CHECK-NEXT:                 if !([[V_8]] < n) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return (((*[[SLATE_SLOT_0]])[0] % 1000) as u32) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     for i in 0 as i32..64 as i32 {
// CHECK-NEXT:         for j in 0 as i32..64 as i32 {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 table.0[i as usize][j as usize] = ((i * 64 + j) as i64) as u64;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "table {}", aligned((unsafe { std::ptr::addr_of_mut!(table.0) as * mut [u64; 64]
// CHECK-NEXT:         }) as * const (), 16 as u64)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "small {}", aligned((unsafe { std::ptr::addr_of_mut!(small.0) as * mut u64 }) as
// CHECK-NEXT:         * const (), 16 as u64)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "aligned32 {}", aligned((unsafe { std::ptr::addr_of_mut!(aligned32.0) as * mut
// CHECK-NEXT:         u32 }) as * const (), 32 as u64)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "bytes {}", aligned((unsafe { std::ptr::addr_of_mut!(bytes.0) as * mut u8 }) as *
// CHECK-NEXT:         const (), 16 as u64)
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"table sum %llu\n".as_ptr(),
// CHECK-NEXT:             sum_vectors(
// CHECK-NEXT:                 (unsafe { std::ptr::addr_of_mut!(table.0[5]) as *mut u64 })
// CHECK-NEXT:                     as *const u64,
// CHECK-NEXT:                 64 as i32,
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"small sum %llu\n".as_ptr(),
// CHECK-NEXT:             sum_vectors(
// CHECK-NEXT:                 (unsafe { std::ptr::addr_of_mut!(small.0) as *mut u64 }) as *const u64,
// CHECK-NEXT:                 2 as i32,
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("locals {}", locals(5 as i32));
// CHECK-NEXT:     println!("dispatched {}", dispatched(4 as i32));
// CHECK-NEXT:     ((unsafe { pad0 }) as i32) + ((unsafe { pad1 }) as i32) + ((unsafe { pad2 }) as i32)
// CHECK-NEXT:         + ((unsafe { pad3 }) as i32)
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
