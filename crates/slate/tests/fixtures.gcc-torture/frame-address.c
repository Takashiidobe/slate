/* { dg-require-effective-target return_address } */
void abort(void);

int check_fa_work(const char *, const char *) __attribute__((noinline, noipa));
int check_fa_mid(const char *) __attribute__((noinline, noipa));
int check_fa(char *) __attribute__((noinline, noipa));
int how_much(void) __attribute__((noinline, noipa));

int check_fa_work(const char *c, const char *f) {
  const char d = 0;

  if (c >= &d)
    return c >= f && f >= &d;
  else
    return c <= f && f <= &d;
}

int check_fa_mid(const char *c) {
  const char *f = __builtin_frame_address(0);

  /* Prevent a tail call to check_fa_work, eliding the current stack frame.  */
  return check_fa_work(c, f) != 0;
}

int check_fa(char *unused) {
  const char c = 0;

  /* Prevent a tail call to check_fa_mid, eliding the current stack frame.  */
  return check_fa_mid(&c) != 0;
}

int how_much(void) { return 8; }

int main(void) {
  char *unused = __builtin_alloca(how_much());

  if (!check_fa(unused))
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
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
// CHECK-NEXT:         offset: usize,
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
// CHECK-NEXT:             UnsafeCell::new(Stack { blocks: Vec::new(), block: 0, offset: 0 })
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     pub struct Mark {
// CHECK-NEXT:         block: usize,
// CHECK-NEXT:         offset: usize,
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     impl Mark {
// CHECK-NEXT:         #[inline]
// CHECK-NEXT:         pub fn new() -> Self {
// CHECK-NEXT:             STACK.with(|stack| {
// CHECK-NEXT:                 let stack = unsafe { &*stack.get() };
// CHECK-NEXT:                 Mark {
// CHECK-NEXT:                     block: stack.block,
// CHECK-NEXT:                     offset: stack.offset,
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
// CHECK-NEXT:                 stack.offset = self.offset;
// CHECK-NEXT:             })
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     #[inline]
// CHECK-NEXT:     pub fn alloca(size: usize, align: usize) -> *mut std::ffi::c_void {
// CHECK-NEXT:         STACK
// CHECK-NEXT:             .with(|stack| unsafe { (*stack.get()).allocate(size, align.max(ALIGN)) })
// CHECK-NEXT:             .cast()
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn __builtin_alloca([[V_0:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.frameaddress.p0"]
// CHECK-NEXT:     fn __slate_intrinsic_2d7c29df8fdf6378([[ARG_0:arg[0-9]+]]: i32) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-require-effective-target return_address }
// CHECK-NEXT: fn check_fa_mid(mut c: &i8) -> i32 {
// CHECK-NEXT:     let mut f: *const i8 =
// CHECK-NEXT:         (unsafe { __slate_intrinsic_2d7c29df8fdf6378(const { ((0 as i32) as u32) as i32 }) })
// CHECK-NEXT:             as *const i8;
// CHECK-NEXT:     // Prevent a tail call to check_fa_work, eliding the current stack frame.
// CHECK-NEXT:     (check_fa_work(c, f) != (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check_fa_work(mut c: &i8, mut f: *const i8) -> i32 {
// CHECK-NEXT:     let mut d: i8 = (0 as i32) as i8;
// CHECK-NEXT:     if (c as *const i8) >= std::ptr::addr_of!(d) {
// CHECK-NEXT:         return ((c as *const i8) >= f && f >= std::ptr::addr_of!(d)) as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return ((c as *const i8) <= f && f <= std::ptr::addr_of!(d)) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check_fa(mut unused: *mut i8) -> i32 {
// CHECK-NEXT:     let mut c: i8 = (0 as i32) as i8;
// CHECK-NEXT:     // Prevent a tail call to check_fa_mid, eliding the current stack frame.
// CHECK-NEXT:     (check_fa_mid(unsafe { &(*std::ptr::addr_of!(c)) }) != (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn how_much() -> i32 {
// CHECK-NEXT:     8 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let mut unused: *mut i8 =
// CHECK-NEXT:         (__slate_alloca::alloca(((how_much() as i64) as u64) as usize, 16usize) as *mut ())
// CHECK-NEXT:             as *mut i8;
// CHECK-NEXT:     if !(check_fa(unused) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
