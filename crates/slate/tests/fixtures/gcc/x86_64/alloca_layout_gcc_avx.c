/* { dg-options "-mavx" } */
#include <alloca.h>
#include <stdint.h>
#include <stdio.h>

__attribute__((noinline)) static void probe(int n) {
  char *a = alloca(n);
  char *b = alloca(n);
  printf("%d: %d %d\n", n, (int)((uintptr_t)a % __BIGGEST_ALIGNMENT__), (int)((uintptr_t)b % __BIGGEST_ALIGNMENT__));
}

int main(void) {
  static const int sizes[] = {1, 16, 17, 33, 100};
  for (unsigned i = 0; i < sizeof sizes / sizeof sizes[0]; i++)
    probe(sizes[i]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-mavx" }
// CHECK-NEXT: mod __slate_alloca {
// CHECK-NEXT:     use std::alloc::{Layout, alloc, dealloc, handle_alloc_error};
// CHECK-NEXT:     use std::cell::UnsafeCell;
// CHECK-EMPTY:
// CHECK-NEXT:     const ALIGN: usize = 16;
// CHECK-NEXT:     const BLOCK: usize = 1 << 20;
// CHECK-EMPTY:
// CHECK-NEXT:     struct Block {
// CHECK-NEXT:         base: *mut u8,
// CHECK-NEXT:         size: usize,
// CHECK-NEXT:     }
// CHECK-EMPTY:
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
// CHECK-EMPTY:
// CHECK-NEXT:     impl Drop for Block {
// CHECK-NEXT:         fn drop(&mut self) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 dealloc(
// CHECK-NEXT:                     self.base,
// CHECK-NEXT:                     Layout::from_size_align_unchecked(self.size, ALIGN),
// CHECK-NEXT:                 )
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     struct Stack {
// CHECK-NEXT:         blocks: Vec<Block>,
// CHECK-NEXT:         block: usize,
// CHECK-NEXT:         used: usize,
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     impl Stack {
// CHECK-NEXT:         fn allocate(&mut self, size: usize, align: usize) -> *mut u8 {
// CHECK-NEXT:             if self.blocks.is_empty() {
// CHECK-NEXT:                 self.blocks.push(Block::new(BLOCK));
// CHECK-NEXT:             }
// CHECK-NEXT:             let needed = size.saturating_add(align);
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 let block = &self.blocks[self.block];
// CHECK-NEXT:                 let base = block.base as usize;
// CHECK-NEXT:                 let top = base + block.size - self.used;
// CHECK-NEXT:                 if let Some(start) = top.checked_sub(size).map(|start| start & !(align - 1))
// CHECK-NEXT:                     && start >= base
// CHECK-NEXT:                 {
// CHECK-NEXT:                     self.used = base + block.size - start;
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         let pointer = block.base.add(start - base);
// CHECK-NEXT:                         pointer.write_bytes(0, size);
// CHECK-NEXT:                         return pointer;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 self.block += 1;
// CHECK-NEXT:                 self.used = 0;
// CHECK-NEXT:                 if self.block == self.blocks.len() || self.blocks[self.block].size < needed {
// CHECK-NEXT:                     self.blocks.truncate(self.block);
// CHECK-NEXT:                     self.blocks
// CHECK-NEXT:                         .push(Block::new(needed.next_multiple_of(ALIGN).max(BLOCK)));
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     thread_local! {
// CHECK-NEXT:         static STACK: UnsafeCell<Stack> = const {
// CHECK-NEXT:             UnsafeCell::new(Stack { blocks: Vec::new(), block: 0, used: 0 })
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub struct Mark {
// CHECK-NEXT:         block: usize,
// CHECK-NEXT:         used: usize,
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     impl Mark {
// CHECK-NEXT:         #[inline]
// CHECK-NEXT:         pub fn new() -> Self {
// CHECK-NEXT:             STACK.with(|stack| {
// CHECK-NEXT:                 let stack = unsafe { &*stack.get() };
// CHECK-NEXT:                 Mark {
// CHECK-NEXT:                     block: stack.block,
// CHECK-NEXT:                     used: stack.used,
// CHECK-NEXT:                 }
// CHECK-NEXT:             })
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     impl Drop for Mark {
// CHECK-NEXT:         #[inline]
// CHECK-NEXT:         fn drop(&mut self) {
// CHECK-NEXT:             STACK.with(|stack| {
// CHECK-NEXT:                 let stack = unsafe { &mut *stack.get() };
// CHECK-NEXT:                 stack.block = self.block;
// CHECK-NEXT:                 stack.used = self.used;
// CHECK-NEXT:             })
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     #[inline]
// CHECK-NEXT:     pub fn alloca(size: usize, align: usize) -> *mut std::ffi::c_void {
// CHECK-NEXT:         STACK
// CHECK-NEXT:             .with(|stack| unsafe {
// CHECK-NEXT:                 (*stack.get()).allocate(size, align.max(1).next_power_of_two())
// CHECK-NEXT:             })
// CHECK-NEXT:             .cast()
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "alloca"]
// CHECK-NEXT:     fn __builtin_alloca([[ARG_0:arg[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn probe(n: i32) {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let a: *mut i8 =
// CHECK-NEXT:         (__slate_alloca::alloca(((n as u64) as usize) + 8usize, 32usize) as *mut ()) as *mut i8;
// CHECK-NEXT:     let b: *mut i8 =
// CHECK-NEXT:         (__slate_alloca::alloca(((n as u64) as usize) + 8usize, 32usize) as *mut ()) as *mut i8;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}: {} {}",
// CHECK-NEXT:         n,
// CHECK-NEXT:         ((a as u64) % 32) as i32,
// CHECK-NEXT:         ((b as u64) % 32) as i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64)
// CHECK-NEXT:         < (std::mem::size_of::<[i32; 5]>() as u64) / (std::mem::size_of::<i32>() as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         probe(unsafe { sizes.0[i as usize] });
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut sizes: __SlateAlign16<[i32; 5]> = __SlateAlign16([1i32, 16i32, 17i32, 33i32, 100i32]);
// CHECK-EMPTY:
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
