#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int aligned_to(void *p, uintptr_t bytes) {
  return ((uintptr_t)p & (bytes - 1)) == 0;
}

static int with_align(int n) {
  int ok = 1;
  char *small = __builtin_alloca(3);
  char *a = __builtin_alloca_with_align(n, 64);
  char *b = __builtin_alloca_with_align(n, 512);
  char *c = __builtin_alloca_with_align(n, 32768);
  ok &= aligned_to(a, 8) && aligned_to(b, 64) && aligned_to(c, 4096);
  memset(small, 1, 3);
  memset(a, 2, n);
  memset(b, 3, n);
  memset(c, 4, n);
  return ok * (small[2] + a[n - 1] + b[n - 1] + c[n - 1]);
}

static int with_align_uninitialized(int n) {
  int *p = __builtin_alloca_with_align_uninitialized(sizeof(int) * n, 1024);
  for (int i = 0; i < n; i++)
    p[i] = i * i;
  int sum = 0;
  for (int i = 0; i < n; i++)
    sum += p[i];
  return aligned_to(p, 128) * sum;
}

static long large_with_align(void) {
  long total = 0;
  for (int i = 0; i < 3; i++) {
    unsigned char *p = __builtin_alloca_with_align(1100000, 2048);
    memset(p, 5, 1100000);
    total += aligned_to(p, 256) * p[1099999];
  }
  return total;
}

int main(void) {
  printf("%d %d\n", with_align(5), with_align(33));
  printf("%d\n", with_align_uninitialized(10));
  printf("%ld\n", large_with_align());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_alloca {
// CHECK-NEXT:     use std::alloc::{Layout, alloc, dealloc, handle_alloc_error};
// CHECK-NEXT:     use std::cell::UnsafeCell;
// CHECK-NEXT:     const ALIGN: usize = 16;
// CHECK-NEXT:     const BLOCK: usize = 1 << 20;
// CHECK-NEXT:     struct Block {
// CHECK-NEXT:         base: *mut u8,
// CHECK-NEXT:         size: usize,
// CHECK-NEXT:     }
// CHECK-NEXT:     impl Block {
// CHECK-NEXT:         fn new(size: usize) -> Self {
// CHECK-NEXT:             let layout = Layout::from_size_align(size, ALIGN).unwrap();
// CHECK-NEXT:             let base = unsafe { alloc(layout) };
// CHECK-NEXT:             if base.is_null() {
// CHECK-NEXT:                 handle_alloc_error(layout);
// CHECK-NEXT:             }
// CHECK-NEXT:             Block { base, size }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     impl Drop for Block {
// CHECK-NEXT:         fn drop(&mut self) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 dealloc(self.base, Layout::from_size_align_unchecked(self.size, ALIGN))
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     struct Stack {
// CHECK-NEXT:         blocks: Vec<Block>,
// CHECK-NEXT:         block: usize,
// CHECK-NEXT:         offset: usize,
// CHECK-NEXT:     }
// CHECK-NEXT:     impl Stack {
// CHECK-NEXT:         fn allocate(&mut self, size: usize, align: usize) -> *mut u8 {
// CHECK-NEXT:             if self.blocks.is_empty() {
// CHECK-NEXT:                 self.blocks.push(Block::new(BLOCK));
// CHECK-NEXT:             }
// CHECK-NEXT:             let needed = size.saturating_add(align);
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 let block = &self.blocks[self.block];
// CHECK-NEXT:                 let base = block.base as usize;
// CHECK-NEXT:                 let start = (base + self.offset).next_multiple_of(align) - base;
// CHECK-NEXT:                 if start <= block.size && block.size - start >= size {
// CHECK-NEXT:                     self.offset = start + size;
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         let pointer = block.base.add(start);
// CHECK-NEXT:                         pointer.write_bytes(0, size);
// CHECK-NEXT:                         return pointer;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 self.block += 1;
// CHECK-NEXT:                 self.offset = 0;
// CHECK-NEXT:                 if self.block == self.blocks.len()
// CHECK-NEXT:                     || self.blocks[self.block].size < needed
// CHECK-NEXT:                 {
// CHECK-NEXT:                     self.blocks.truncate(self.block);
// CHECK-NEXT:                     self.blocks
// CHECK-NEXT:                         .push(Block::new(needed.next_multiple_of(ALIGN).max(BLOCK)));
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     thread_local! {
// CHECK-NEXT:         static STACK : UnsafeCell < Stack > = const { UnsafeCell::new(Stack { blocks :
// CHECK-NEXT:         Vec::new(), block : 0, offset : 0 }) };
// CHECK-NEXT:     }
// CHECK-NEXT:     pub struct Mark {
// CHECK-NEXT:         block: usize,
// CHECK-NEXT:         offset: usize,
// CHECK-NEXT:     }
// CHECK-NEXT:     impl Mark {
// CHECK-NEXT:         #[inline]
// CHECK-NEXT:         pub fn new() -> Self {
// CHECK-NEXT:             STACK
// CHECK-NEXT:                 .with(|stack| {
// CHECK-NEXT:                     let stack = unsafe { &*stack.get() };
// CHECK-NEXT:                     Mark {
// CHECK-NEXT:                         block: stack.block,
// CHECK-NEXT:                         offset: stack.offset,
// CHECK-NEXT:                     }
// CHECK-NEXT:                 })
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     impl Drop for Mark {
// CHECK-NEXT:         #[inline]
// CHECK-NEXT:         fn drop(&mut self) {
// CHECK-NEXT:             STACK
// CHECK-NEXT:                 .with(|stack| {
// CHECK-NEXT:                     let stack = unsafe { &mut *stack.get() };
// CHECK-NEXT:                     stack.block = self.block;
// CHECK-NEXT:                     stack.offset = self.offset;
// CHECK-NEXT:                 })
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     #[inline]
// CHECK-NEXT:     pub fn alloca(size: usize, align: usize) -> *mut std::ffi::c_void {
// CHECK-NEXT:         STACK
// CHECK-NEXT:             .with(|stack| unsafe { (*stack.get()).allocate(size, align.max(ALIGN)) })
// CHECK-NEXT:             .cast()
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "alloca"]
// CHECK-NEXT:     fn __builtin_alloca([[V_0:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn __builtin_alloca_with_align([[V_1:__v[0-9]+]]: u64, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn __builtin_alloca_with_align_uninitialized([[V_3:__v[0-9]+]]: u64, [[V_4:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn aligned_to(p: *mut (), bytes: u64) -> i32 {
// CHECK-NEXT:     ((p as u64) & bytes.wrapping_sub(1) == 0) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn with_align(n: i32) -> i32 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let mut ok: i32 = 1;
// CHECK-NEXT:     let small: *mut i8 = (__slate_alloca::alloca(3 as usize, 16usize) as *mut ())
// CHECK-NEXT:         as *mut i8;
// CHECK-NEXT:     let a: *mut i8 = (__slate_alloca::alloca(
// CHECK-NEXT:         ((n as i64) as u64) as usize,
// CHECK-NEXT:         (64 as usize) / 8usize,
// CHECK-NEXT:     ) as *mut ()) as *mut i8;
// CHECK-NEXT:     let b: *mut i8 = (__slate_alloca::alloca(
// CHECK-NEXT:         ((n as i64) as u64) as usize,
// CHECK-NEXT:         (512 as usize) / 8usize,
// CHECK-NEXT:     ) as *mut ()) as *mut i8;
// CHECK-NEXT:     let c: *mut i8 = (__slate_alloca::alloca(
// CHECK-NEXT:         ((n as i64) as u64) as usize,
// CHECK-NEXT:         (32768 as usize) / 8usize,
// CHECK-NEXT:     ) as *mut ()) as *mut i8;
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: bool;
// CHECK-NEXT:     if aligned_to(a as *mut (), 8u64) != 0 {
// CHECK-NEXT:         [[V_5]] = aligned_to(b as *mut (), 64u64) != 0;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_5]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_5]] {
// CHECK-NEXT:         [[V_6]] = aligned_to(c as *mut (), 4096u64) != 0;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_6]] = false as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: i32 = [[V_6]] as i32;
// CHECK-NEXT:     ok &= [[V_7]];
// CHECK-NEXT:     unsafe { std::ptr::write_bytes(small as *mut u8, 1u8, 3 as usize) };
// CHECK-NEXT:     unsafe { std::ptr::write_bytes(a as *mut u8, 2u8, ((n as i64) as u64) as usize) };
// CHECK-NEXT:     unsafe { std::ptr::write_bytes(b as *mut u8, 3u8, ((n as i64) as u64) as usize) };
// CHECK-NEXT:     unsafe { std::ptr::write_bytes(c as *mut u8, 4u8, ((n as i64) as u64) as usize) };
// CHECK-NEXT:     ok
// CHECK-NEXT:         * (((unsafe { *small.offset(2 as isize) }) as i32)
// CHECK-NEXT:             + ((unsafe { *a.offset((n - 1) as isize) }) as i32)
// CHECK-NEXT:             + ((unsafe { *b.offset((n - 1) as isize) }) as i32)
// CHECK-NEXT:             + ((unsafe { *c.offset((n - 1) as isize) }) as i32))
// CHECK-NEXT: }
// CHECK-NEXT: fn with_align_uninitialized(n: i32) -> i32 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let p: *mut i32 = (__slate_alloca::alloca(
// CHECK-NEXT:         4u64.wrapping_mul((n as i64) as u64) as usize,
// CHECK-NEXT:         (1024 as usize) / 8usize,
// CHECK-NEXT:     ) as *mut ()) as *mut i32;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *p.offset(i as isize) = i * i;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         sum += unsafe { *p.offset(i as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     aligned_to(p as *mut (), 128u64) * sum
// CHECK-NEXT: }
// CHECK-NEXT: fn large_with_align() -> i64 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         let p: *mut u8 = (__slate_alloca::alloca(
// CHECK-NEXT:             1100000 as usize,
// CHECK-NEXT:             (2048 as usize) / 8usize,
// CHECK-NEXT:         ) as *mut ()) as *mut u8;
// CHECK-NEXT:         unsafe { std::ptr::write_bytes(p as *mut u8, 5u8, 1100000 as usize) };
// CHECK-NEXT:         let [[V_8:__v[0-9]+]]: i32 = aligned_to(p as *mut (), 256u64)
// CHECK-NEXT:             * (((unsafe { *p.offset(1099999 as isize) }) as u32) as i32);
// CHECK-NEXT:         total += [[V_8]] as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {}", with_align(5i32), with_align(33i32));
// CHECK-NEXT:     println!("{}", with_align_uninitialized(10i32));
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), large_with_align()) };
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
