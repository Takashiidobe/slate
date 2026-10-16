/* See 'gcc.target/nvptx/__builtin_alloca_0-1-O0.c'.
   { dg-xfail-if TODO { nvptx-*-* && { ! nvptx_softstack } } { "-O0" } { "" } }
 */

extern void abort(void);

extern __SIZE_TYPE__ strlen(const char *);
void                 foo(char *str) {
  int   len2 = strlen(str);
  char *a    = (char *)__builtin_alloca(0);
  char *b    = (char *)__builtin_alloca(len2 * 3);

  if ((int)(a - b) < (len2 * 3)) {
#ifdef _WIN32
    abort();
#endif
    return;
  }
}

static char *volatile argp = "pr36321.x";

int main(int argc, char **argv) {
  foo(argp);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! See 'gcc.target/nvptx/[[BUILTIN_ALLOCA_0:__builtin_alloca_[0-9]+]]-1-O0.c'.
// CHECK-NEXT: //! { dg-xfail-if TODO { nvptx-*-* && { ! nvptx_softstack } } { "-O0" } { "" } }
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
// CHECK-NEXT:     fn strlen([[V_0:__v[0-9]+]]: *const i8) -> u64;
// CHECK-NEXT:     fn __builtin_alloca([[V_1:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut str: *mut i8) {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let mut len2: i32 = ((unsafe { strlen(str as *const i8) }) as u32) as i32;
// CHECK-NEXT:     let mut a: *mut i8 = (__slate_alloca::alloca((((0 as i32) as i64) as u64) as usize, 16usize)
// CHECK-NEXT:         as *mut ()) as *mut i8;
// CHECK-NEXT:     let mut b: *mut i8 = (__slate_alloca::alloca(
// CHECK-NEXT:         (((len2 * (3 as i32)) as i64) as u64) as usize,
// CHECK-NEXT:         16usize,
// CHECK-NEXT:     ) as *mut ()) as *mut i8;
// CHECK-NEXT:     if (((unsafe { a.offset_from(b as *mut i8) }) as i64) as i32) < len2 * (3 as i32) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut argp: *mut i8 = b"pr36321.x\0".as_ptr() as *mut i8;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     foo(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(argp)) });
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
