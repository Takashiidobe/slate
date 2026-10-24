#include <alloca.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct frame {
  int *args;
  int count;
};

static int const_alloca(void) {
  unsigned char *buf = __builtin_alloca(16);
  memset(buf, 3, 16);
  return buf[0] + buf[15];
}

static int dyn_alloca(int n) {
  unsigned char *buf = __builtin_alloca(n);
  memset(buf, 7, n);
  return buf[0] + buf[n - 1];
}

static int uninitialized_alloca(int n) {
  unsigned char *buf = __builtin_alloca_uninitialized(n);
  memset(buf, 9, n);
  return buf[0] + buf[n - 1];
}

static int loop_alloca(int n) {
  int *slots[8];
  for (int i = 0; i < n; i++) {
    slots[i] = alloca(sizeof(int) * (i + 1));
    for (int j = 0; j <= i; j++)
      slots[i][j] = i * 10 + j;
  }
  int sum = 0;
  for (int i = 0; i < n; i++)
    for (int j = 0; j <= i; j++)
      sum += slots[i][j];
  return sum;
}

static int sum_frame(const struct frame *f) {
  int sum = 0;
  for (int i = 0; i < f->count; i++)
    sum += f->args[i];
  return sum;
}

static int branch_alloca(int argc, const int *argv, int min) {
  struct frame f = {(int *)argv, argc};
  if (argc < min) {
    f.args = alloca(sizeof(int) * min);
    for (int i = 0; i < argc; i++)
      f.args[i] = argv[i];
    for (int i = argc; i < min; i++)
      f.args[i] = -1;
    f.count = min;
  }
  return sum_frame(&f);
}

static int ternary_alloca(char *scratch, int n) {
  char *buf = scratch ? scratch : alloca(n);
  for (int i = 0; i < n; i++)
    buf[i] = (char)i;
  return buf[n - 1];
}

static int zero_alloca(void) {
  char *a = alloca(0);
  char *b = alloca(4);
  memcpy(b, "abc", 4);
  return a != 0 && strcmp(b, "abc") == 0;
}

static int aligned_alloca(void) {
  int ok = 1;
  for (int n = 1; n < 40; n += 3) {
    char *p = __builtin_alloca(n);
    ok &= ((uintptr_t)p & 15) == 0;
  }
  return ok;
}

static int recursive_alloca(int depth) {
  int *cell = alloca(sizeof(int));
  *cell = depth;
  if (depth == 0)
    return 0;
  int below = recursive_alloca(depth - 1);
  return *cell + below;
}

static long large_alloca(int rounds) {
  long total = 0;
  for (int r = 0; r < rounds; r++) {
    unsigned char *a = alloca(600000);
    unsigned char *b = alloca(600000);
    memset(a, 1, 600000);
    memset(b, 2, 600000);
    total += a[599999] + b[0];
  }
  return total;
}

static long repeated_large(void) {
  long total = 0;
  for (int i = 0; i < 4; i++)
    total += large_alloca(1);
  return total;
}

int main(void) {
  int argv[] = {4, 5};
  char scratch[8];
  printf("%d %d %d\n", const_alloca(), dyn_alloca(10),
         uninitialized_alloca(12));
  printf("%d\n", loop_alloca(8));
  printf("%d %d\n", branch_alloca(2, argv, 5), branch_alloca(2, argv, 1));
  printf("%d %d\n", ternary_alloca(scratch, 8), ternary_alloca(0, 20));
  printf("%d %d\n", zero_alloca(), aligned_alloca());
  printf("%d\n", recursive_alloca(100));
  printf("%ld\n", repeated_large());
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
// CHECK-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT:     fn strcmp(__s1: *const i8, __s2: *const i8) -> i32;
// CHECK-NEXT:     #[link_name = "alloca"]
// CHECK-NEXT:     fn __builtin_alloca([[V_0:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn __builtin_alloca_uninitialized([[V_1:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct frame {
// CHECK-NEXT:     args: *mut i32,
// CHECK-NEXT:     count: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn const_alloca() -> i32 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let buf: *mut u8 = (__slate_alloca::alloca(16 as usize, 16usize) as *mut ())
// CHECK-NEXT:         as *mut u8;
// CHECK-NEXT:     unsafe { std::ptr::write_bytes(buf as *mut u8, 3u8, 16 as usize) };
// CHECK-NEXT:     (((unsafe { *buf.add(0) }) as u32) as i32)
// CHECK-NEXT:         + (((unsafe { *buf.add(15) }) as u32) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn dyn_alloca(n: i32) -> i32 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let buf: *mut u8 = (__slate_alloca::alloca(((n as i64) as u64) as usize, 16usize)
// CHECK-NEXT:         as *mut ()) as *mut u8;
// CHECK-NEXT:     unsafe { std::ptr::write_bytes(buf as *mut u8, 7u8, ((n as i64) as u64) as usize) };
// CHECK-NEXT:     (((unsafe { *buf.add(0) }) as u32) as i32)
// CHECK-NEXT:         + (((unsafe { *buf.offset((n - 1) as isize) }) as u32) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn uninitialized_alloca(n: i32) -> i32 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let buf: *mut u8 = (__slate_alloca::alloca(((n as i64) as u64) as usize, 16usize)
// CHECK-NEXT:         as *mut ()) as *mut u8;
// CHECK-NEXT:     unsafe { std::ptr::write_bytes(buf as *mut u8, 9u8, ((n as i64) as u64) as usize) };
// CHECK-NEXT:     (((unsafe { *buf.add(0) }) as u32) as i32)
// CHECK-NEXT:         + (((unsafe { *buf.offset((n - 1) as isize) }) as u32) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn loop_alloca(n: i32) -> i32 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let mut slots: __SlateAlign16<[*mut i32; 8]> = __SlateAlign16([0 as *mut i32; 8]);
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         slots.0[i as usize] = (__slate_alloca::alloca(
// CHECK-NEXT:             4u64.wrapping_mul(((i + 1) as i64) as u64) as usize,
// CHECK-NEXT:             16usize,
// CHECK-NEXT:         ) as *mut ()) as *mut i32;
// CHECK-NEXT:         let mut j: i32 = 0;
// CHECK-NEXT:         while j <= i {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *slots.0[i as usize].offset(j as isize) = i * 10 + j;
// CHECK-NEXT:             }
// CHECK-NEXT:             j += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         let mut j: i32 = 0;
// CHECK-NEXT:         while j <= i {
// CHECK-NEXT:             sum += unsafe { *slots.0[i as usize].offset(j as isize) };
// CHECK-NEXT:             j += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_frame(f: &frame) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     while i < unsafe { (*(f as *const frame)).count } {
// CHECK-NEXT:         sum += unsafe { *unsafe { (*(f as *const frame)).args }.offset(i as isize) };
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-NEXT: fn branch_alloca(argc: i32, argv: *const i32, min: i32) -> i32 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let mut f: frame = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: frame = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].args = argv as *mut i32;
// CHECK-NEXT:         [[T_0]].count = argc;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     if argc < min {
// CHECK-NEXT:         f.args = (__slate_alloca::alloca(
// CHECK-NEXT:             4u64.wrapping_mul((min as i64) as u64) as usize,
// CHECK-NEXT:             16usize,
// CHECK-NEXT:         ) as *mut ()) as *mut i32;
// CHECK-NEXT:         for i in 0..argc {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *f.args.offset(i as isize) = *argv.offset(i as isize);
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         for i in argc..min {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *f.args.offset(i as isize) = -1;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         f.count = min;
// CHECK-NEXT:     }
// CHECK-NEXT:     sum_frame(unsafe { &(*(std::ptr::addr_of_mut!(f) as *const frame)) })
// CHECK-NEXT: }
// CHECK-NEXT: fn ternary_alloca(scratch: *mut i8, n: i32) -> i32 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: *mut () = if !scratch.is_null() {
// CHECK-NEXT:         scratch as *mut ()
// CHECK-NEXT:     } else {
// CHECK-NEXT:         __slate_alloca::alloca(((n as i64) as u64) as usize, 16usize) as *mut ()
// CHECK-NEXT:     };
// CHECK-NEXT:     let buf: *mut i8 = [[V_2]] as *mut i8;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *buf.offset(i as isize) = i as i8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { *buf.offset((n - 1) as isize) }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn zero_alloca() -> i32 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let a: *mut i8 = (__slate_alloca::alloca(0 as usize, 16usize) as *mut ()) as *mut i8;
// CHECK-NEXT:     let b: *mut i8 = (__slate_alloca::alloca(4 as usize, 16usize) as *mut ()) as *mut i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             c"abc".as_ptr() as *const u8,
// CHECK-NEXT:             b as *mut u8,
// CHECK-NEXT:             4 as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     (!a.is_null() && (unsafe { strcmp(b as *const i8, c"abc".as_ptr()) }) == 0) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn aligned_alloca() -> i32 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let mut ok: i32 = 1;
// CHECK-NEXT:     let mut n: i32 = 1;
// CHECK-NEXT:     while n < 40 {
// CHECK-NEXT:         let p: *mut i8 = (__slate_alloca::alloca(((n as i64) as u64) as usize, 16usize)
// CHECK-NEXT:             as *mut ()) as *mut i8;
// CHECK-NEXT:         ok &= ((p as u64) & 15 == 0) as i32;
// CHECK-NEXT:         n += 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     ok
// CHECK-NEXT: }
// CHECK-NEXT: fn recursive_alloca(depth: i32) -> i32 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let cell: *mut i32 = (__slate_alloca::alloca(4 as usize, 16usize) as *mut ())
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *cell = depth;
// CHECK-NEXT:     }
// CHECK-NEXT:     if depth == 0 {
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let below: i32 = recursive_alloca(depth - 1);
// CHECK-NEXT:     (unsafe { *cell }) + below
// CHECK-NEXT: }
// CHECK-NEXT: fn large_alloca(rounds: i32) -> i64 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     for r in 0..rounds {
// CHECK-NEXT:         let a: *mut u8 = (__slate_alloca::alloca(600000 as usize, 16usize) as *mut ())
// CHECK-NEXT:             as *mut u8;
// CHECK-NEXT:         let b: *mut u8 = (__slate_alloca::alloca(600000 as usize, 16usize) as *mut ())
// CHECK-NEXT:             as *mut u8;
// CHECK-NEXT:         unsafe { std::ptr::write_bytes(a as *mut u8, 1u8, 600000 as usize) };
// CHECK-NEXT:         unsafe { std::ptr::write_bytes(b as *mut u8, 2u8, 600000 as usize) };
// CHECK-NEXT:         total
// CHECK-NEXT:             += ((((unsafe { *a.add(599999) }) as u32) as i32)
// CHECK-NEXT:                 + (((unsafe { *b.add(0) }) as u32) as i32)) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn repeated_large() -> i64 {
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         let [[V_3:__v[0-9]+]]: i64 = large_alloca(1i32);
// CHECK-NEXT:         total += [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut argv: [i32; 2] = [4, 5];
// CHECK-NEXT:     let mut scratch: [i8; 8] = [0i8; 8];
// CHECK-NEXT:     println!("{} {} {}", const_alloca(), dyn_alloca(10i32), uninitialized_alloca(12i32));
// CHECK-NEXT:     println!("{}", loop_alloca(8i32));
// CHECK-NEXT:     let __slate_printf_arg0 = branch_alloca(2i32, argv.as_mut_ptr() as *const i32, 5i32);
// CHECK-NEXT:     let __slate_printf_arg1 = branch_alloca(2i32, argv.as_mut_ptr() as *const i32, 1i32);
// CHECK-NEXT:     println!("{} {}", __slate_printf_arg0, __slate_printf_arg1);
// CHECK-NEXT:     let __slate_printf_arg0 = ternary_alloca(scratch.as_mut_ptr() as *mut i8, 8i32);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", __slate_printf_arg0, ternary_alloca(std::ptr::null_mut::< i8 > (),
// CHECK-NEXT:         20i32)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {}", zero_alloca(), aligned_alloca());
// CHECK-NEXT:     println!("{}", recursive_alloca(100i32));
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), repeated_large()) };
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
