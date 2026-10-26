#include <stdint.h>
#include <stdio.h>

static int gnu_builtin_stack(void) {
  unsigned char *first   = __builtin_alloca(16);
  unsigned char *aligned = __builtin_alloca_with_align(16, 128);
  __builtin_memset(first, 3, 16);
  __builtin_memset(aligned, 5, 16);
  return first[0] + first[15] + aligned[0] + aligned[15] +
         ((uintptr_t)aligned % 16 == 0);
}

static int gnu_builtin_memory(void) {
  unsigned char source[16] = "abcdefghijklmno";
  unsigned char first[32]  = {};
  unsigned char second[32] = {};
  unsigned char third[32]  = {};
  void         *end        = __builtin_mempcpy(first, source, 8);
  __builtin_memcpy_inline(second, source, 8);
  __builtin_memcpy(third, source, 16);
  __builtin_memmove(third + 2, third, 8);
  __builtin_bzero(second + 8, 8);
  __builtin___memcpy_chk(first + 8, source + 8, 8, sizeof(first) - 8);
  __builtin___memmove_chk(second + 1, second, 7, sizeof(second) - 1);
  __builtin___memset_chk(second + 16, 9, 4, sizeof(second) - 16);
  return (end == first + 8) + (__builtin_memcmp(first, source, 16) == 0) +
         (__builtin_bcmp(first, source, 16) == 0) +
         (__builtin_memchr(first, 'h', 16) == first + 7) + (third[2] == 'a') +
         (third[9] == 'h') + second[0] + second[1] + second[16] + second[19];
}

static int gnu_builtin_strings(void) {
  char  first[64]  = {};
  char  second[64] = {};
  char  third[64]  = {};
  char *end;
  __builtin_strcpy(first, "gnu");
  end = __builtin_stpcpy(first + 3, "builtins");
  __builtin_strncpy(second, first, 6);
  __builtin_strcpy(third, "memory");
  __builtin_strcat(third, "-ok");
  __builtin___strcpy_chk(second, "clang", sizeof(second));
  __builtin___stpcpy_chk(second + 5, "-head", sizeof(second) - 5);
  __builtin___strcat_chk(second, "-gnu", sizeof(second));
  return (int)__builtin_strlen(first) + (end == first + 11) +
         (__builtin_strcmp(third, "memory-ok") == 0) +
         (__builtin_strncmp(second, "clang-head-gnu", 14) == 0) +
         (__builtin_strchr(second, '-') == second + 5) +
         (__builtin_strrchr(second, '-') == second + 10) +
         (__builtin_strstr(second, "head") == second + 6) +
         (int)__builtin_strspn(second, "clang") +
         (int)__builtin_strcspn(second, "-") +
         (__builtin_strpbrk(second, "-g") == second + 4);
}

int main(void) {
  printf("%d %d %d\n", gnu_builtin_stack(), gnu_builtin_memory(),
         gnu_builtin_strings());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![allow(arithmetic_overflow, unconditional_panic)]
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
// CHECK-NEXT:     #[link_name = "alloca"]
// CHECK-NEXT:     fn __builtin_alloca([[V_0:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn __builtin_alloca_with_align([[V_1:__v[0-9]+]]: u64, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_3:__v[0-9]+]]: *mut (), [[V_4:__v[0-9]+]]: i32, [[V_5:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "mempcpy"]
// CHECK-NEXT:     fn __builtin_mempcpy([[V_6:__v[0-9]+]]: *mut (), [[V_7:__v[0-9]+]]: *const (), [[V_8:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn __builtin_memcpy_inline([[V_9:__v[0-9]+]]: *mut (), [[V_10:__v[0-9]+]]: *const (), [[V_11:__v[0-9]+]]: u64);
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_12:__v[0-9]+]]: *mut (), [[V_13:__v[0-9]+]]: *const (), [[V_14:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "memmove"]
// CHECK-NEXT:     fn __builtin_memmove([[V_15:__v[0-9]+]]: *mut (), [[V_16:__v[0-9]+]]: *const (), [[V_17:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "bzero"]
// CHECK-NEXT:     fn __builtin_bzero([[V_18:__v[0-9]+]]: *mut (), [[V_19:__v[0-9]+]]: u64);
// CHECK-NEXT:     #[link_name = "__memcpy_chk"]
// CHECK-NEXT:     fn __builtin___memcpy_chk(
// CHECK-NEXT:         [[V_20:__v[0-9]+]]: *mut (),
// CHECK-NEXT:         [[V_21:__v[0-9]+]]: *const (),
// CHECK-NEXT:         [[V_22:__v[0-9]+]]: u64,
// CHECK-NEXT:         [[V_23:__v[0-9]+]]: u64,
// CHECK-NEXT:     ) -> *mut ();
// CHECK-NEXT:     #[link_name = "__memmove_chk"]
// CHECK-NEXT:     fn __builtin___memmove_chk(
// CHECK-NEXT:         [[V_24:__v[0-9]+]]: *mut (),
// CHECK-NEXT:         [[V_25:__v[0-9]+]]: *const (),
// CHECK-NEXT:         [[V_26:__v[0-9]+]]: u64,
// CHECK-NEXT:         [[V_27:__v[0-9]+]]: u64,
// CHECK-NEXT:     ) -> *mut ();
// CHECK-NEXT:     #[link_name = "__memset_chk"]
// CHECK-NEXT:     fn __builtin___memset_chk(
// CHECK-NEXT:         [[V_28:__v[0-9]+]]: *mut (),
// CHECK-NEXT:         [[V_29:__v[0-9]+]]: i32,
// CHECK-NEXT:         [[V_30:__v[0-9]+]]: u64,
// CHECK-NEXT:         [[V_31:__v[0-9]+]]: u64,
// CHECK-NEXT:     ) -> *mut ();
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_32:__v[0-9]+]]: *const (), [[V_33:__v[0-9]+]]: *const (), [[V_34:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     #[link_name = "bcmp"]
// CHECK-NEXT:     fn __builtin_bcmp([[V_35:__v[0-9]+]]: *const (), [[V_36:__v[0-9]+]]: *const (), [[V_37:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     #[link_name = "memchr"]
// CHECK-NEXT:     fn __builtin_memchr([[V_38:__v[0-9]+]]: *const (), [[V_39:__v[0-9]+]]: i32, [[V_40:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "strcpy"]
// CHECK-NEXT:     fn __builtin_strcpy([[V_41:__v[0-9]+]]: *mut i8, [[V_42:__v[0-9]+]]: *const i8) -> *mut i8;
// CHECK-NEXT:     #[link_name = "stpcpy"]
// CHECK-NEXT:     fn __builtin_stpcpy([[V_43:__v[0-9]+]]: *mut i8, [[V_44:__v[0-9]+]]: *const i8) -> *mut i8;
// CHECK-NEXT:     #[link_name = "strncpy"]
// CHECK-NEXT:     fn __builtin_strncpy([[V_45:__v[0-9]+]]: *mut i8, [[V_46:__v[0-9]+]]: *const i8, [[V_47:__v[0-9]+]]: u64) -> *mut i8;
// CHECK-NEXT:     #[link_name = "strcat"]
// CHECK-NEXT:     fn __builtin_strcat([[V_48:__v[0-9]+]]: *mut i8, [[V_49:__v[0-9]+]]: *const i8) -> *mut i8;
// CHECK-NEXT:     #[link_name = "__strcpy_chk"]
// CHECK-NEXT:     fn __builtin___strcpy_chk([[V_50:__v[0-9]+]]: *mut i8, [[V_51:__v[0-9]+]]: *const i8, [[V_52:__v[0-9]+]]: u64) -> *mut i8;
// CHECK-NEXT:     #[link_name = "__stpcpy_chk"]
// CHECK-NEXT:     fn __builtin___stpcpy_chk([[V_53:__v[0-9]+]]: *mut i8, [[V_54:__v[0-9]+]]: *const i8, [[V_55:__v[0-9]+]]: u64) -> *mut i8;
// CHECK-NEXT:     #[link_name = "__strcat_chk"]
// CHECK-NEXT:     fn __builtin___strcat_chk(
// CHECK-NEXT:         [[V_56:__v[0-9]+]]: *mut i8,
// CHECK-NEXT:         [[V_57:__v[0-9]+]]: *const i8,
// CHECK-NEXT:         [[V_58:__v[0-9]+]]: u64,
// CHECK-NEXT:     ) -> *mut i8;
// CHECK-NEXT:     #[link_name = "strlen"]
// CHECK-NEXT:     fn __builtin_strlen([[V_59:__v[0-9]+]]: *const i8) -> u64;
// CHECK-NEXT:     #[link_name = "strcmp"]
// CHECK-NEXT:     fn __builtin_strcmp([[V_60:__v[0-9]+]]: *const i8, [[V_61:__v[0-9]+]]: *const i8) -> i32;
// CHECK-NEXT:     #[link_name = "strncmp"]
// CHECK-NEXT:     fn __builtin_strncmp([[V_62:__v[0-9]+]]: *const i8, [[V_63:__v[0-9]+]]: *const i8, [[V_64:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     #[link_name = "strchr"]
// CHECK-NEXT:     fn __builtin_strchr([[V_65:__v[0-9]+]]: *const i8, [[V_66:__v[0-9]+]]: i32) -> *mut i8;
// CHECK-NEXT:     #[link_name = "strrchr"]
// CHECK-NEXT:     fn __builtin_strrchr([[V_67:__v[0-9]+]]: *const i8, [[V_68:__v[0-9]+]]: i32) -> *mut i8;
// CHECK-NEXT:     #[link_name = "strstr"]
// CHECK-NEXT:     fn __builtin_strstr([[V_69:__v[0-9]+]]: *const i8, [[V_70:__v[0-9]+]]: *const i8) -> *mut i8;
// CHECK-NEXT:     #[link_name = "strspn"]
// CHECK-NEXT:     fn __builtin_strspn([[V_71:__v[0-9]+]]: *const i8, [[V_72:__v[0-9]+]]: *const i8) -> u64;
// CHECK-NEXT:     #[link_name = "strcspn"]
// CHECK-NEXT:     fn __builtin_strcspn([[V_73:__v[0-9]+]]: *const i8, [[V_74:__v[0-9]+]]: *const i8) -> u64;
// CHECK-NEXT:     #[link_name = "strpbrk"]
// CHECK-NEXT:     fn __builtin_strpbrk([[V_75:__v[0-9]+]]: *const i8, [[V_76:__v[0-9]+]]: *const i8) -> *mut i8;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn gnu_builtin_stack() -> i32 {
// CHECK-NEXT:     let __slate_alloca_mark = __slate_alloca::Mark::new();
// CHECK-NEXT:     let first: *mut u8 = (__slate_alloca::alloca(16 as usize, 16usize) as *mut ())
// CHECK-NEXT:         as *mut u8;
// CHECK-NEXT:     let aligned: *mut u8 = (__slate_alloca::alloca(16 as usize, (128 as usize) / 8usize)
// CHECK-NEXT:         as *mut ()) as *mut u8;
// CHECK-NEXT:     unsafe { __builtin_memset(first as *mut (), 3i32, 16u64) };
// CHECK-NEXT:     unsafe { __builtin_memset(aligned as *mut (), 5i32, 16u64) };
// CHECK-NEXT:     (((unsafe { *first.add(0) }) as u32) as i32)
// CHECK-NEXT:         + (((unsafe { *first.add(15) }) as u32) as i32)
// CHECK-NEXT:         + (((unsafe { *aligned.add(0) }) as u32) as i32)
// CHECK-NEXT:         + (((unsafe { *aligned.add(15) }) as u32) as i32)
// CHECK-NEXT:         + (((aligned as u64) % 16 == 0) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_builtin_memory() -> i32 {
// CHECK-NEXT:     let mut source: __SlateAlign16<[u8; 16]> = __SlateAlign16([
// CHECK-NEXT:         97u8, 98u8, 99u8, 100u8, 101u8, 102u8, 103u8, 104u8, 105u8, 106u8, 107u8, 108u8,
// CHECK-NEXT:         109u8, 110u8, 111u8, 0u8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut first: __SlateAlign16<[u8; 32]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [u8; 32] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut second: __SlateAlign16<[u8; 32]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: [u8; 32] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut third: __SlateAlign16<[u8; 32]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: [u8; 32] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let end: *mut () = unsafe {
// CHECK-NEXT:         __builtin_mempcpy(
// CHECK-NEXT:             first.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             source.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             8u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             source.0.as_mut_ptr() as *const u8,
// CHECK-NEXT:             second.0.as_mut_ptr() as *mut u8,
// CHECK-NEXT:             8 as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             third.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             source.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memmove(
// CHECK-NEXT:             (third.0.as_mut_ptr() as *mut u8).add(2) as *mut (),
// CHECK-NEXT:             third.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             8u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_bzero((second.0.as_mut_ptr() as *mut u8).add(8) as *mut (), 8u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin___memcpy_chk(
// CHECK-NEXT:             (first.0.as_mut_ptr() as *mut u8).add(8) as *mut (),
// CHECK-NEXT:             (source.0.as_mut_ptr() as *mut u8).add(8) as *const (),
// CHECK-NEXT:             8u64,
// CHECK-NEXT:             24u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin___memmove_chk(
// CHECK-NEXT:             (second.0.as_mut_ptr() as *mut u8).add(1) as *mut (),
// CHECK-NEXT:             second.0.as_mut_ptr() as *const (),
// CHECK-NEXT:             7u64,
// CHECK-NEXT:             31u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin___memset_chk(
// CHECK-NEXT:             (second.0.as_mut_ptr() as *mut u8).add(16) as *mut (),
// CHECK-NEXT:             9i32,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     ((end == ((unsafe { (first.0.as_mut_ptr() as *mut u8).add(8) }) as *mut ())) as i32)
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             __builtin_memcmp(
// CHECK-NEXT:                 first.0.as_mut_ptr() as *const (),
// CHECK-NEXT:                 source.0.as_mut_ptr() as *const (),
// CHECK-NEXT:                 16u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == 0) as i32)
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             __builtin_bcmp(
// CHECK-NEXT:                 first.0.as_mut_ptr() as *const (),
// CHECK-NEXT:                 source.0.as_mut_ptr() as *const (),
// CHECK-NEXT:                 16u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == 0) as i32)
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             __builtin_memchr(first.0.as_mut_ptr() as *const (), 104i32, 16u64)
// CHECK-NEXT:         }) == ((unsafe { (first.0.as_mut_ptr() as *mut u8).add(7) }) as *mut ())) as i32)
// CHECK-NEXT:         + ((((third.0[2] as u32) as i32) == 97) as i32)
// CHECK-NEXT:         + ((((third.0[9] as u32) as i32) == 104) as i32) + ((second.0[0] as u32) as i32)
// CHECK-NEXT:         + ((second.0[1] as u32) as i32) + ((second.0[16] as u32) as i32)
// CHECK-NEXT:         + ((second.0[19] as u32) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_builtin_strings() -> i32 {
// CHECK-NEXT:     let mut first: __SlateAlign16<[i8; 64]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0]]: [i8; 64] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut second: __SlateAlign16<[i8; 64]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_1]]: [i8; 64] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut third: __SlateAlign16<[i8; 64]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_2]]: [i8; 64] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe { __builtin_strcpy(first.0.as_mut_ptr() as *mut i8, c"gnu".as_ptr()) };
// CHECK-NEXT:     let end: *mut i8 = unsafe {
// CHECK-NEXT:         __builtin_stpcpy((first.0.as_mut_ptr() as *mut i8).add(3), c"builtins".as_ptr())
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_strncpy(
// CHECK-NEXT:             second.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             first.0.as_mut_ptr() as *const i8,
// CHECK-NEXT:             6u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { __builtin_strcpy(third.0.as_mut_ptr() as *mut i8, c"memory".as_ptr()) };
// CHECK-NEXT:     unsafe { __builtin_strcat(third.0.as_mut_ptr() as *mut i8, c"-ok".as_ptr()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin___strcpy_chk(
// CHECK-NEXT:             second.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             c"clang".as_ptr(),
// CHECK-NEXT:             64u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin___stpcpy_chk(
// CHECK-NEXT:             (second.0.as_mut_ptr() as *mut i8).add(5),
// CHECK-NEXT:             c"-head".as_ptr(),
// CHECK-NEXT:             59u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin___strcat_chk(second.0.as_mut_ptr() as *mut i8, c"-gnu".as_ptr(), 64u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     (((unsafe { __builtin_strlen(first.0.as_mut_ptr() as *const i8) }) as u32) as i32)
// CHECK-NEXT:         + ((end == unsafe { (first.0.as_mut_ptr() as *mut i8).add(11) }) as i32)
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             __builtin_strcmp(third.0.as_mut_ptr() as *const i8, c"memory-ok".as_ptr())
// CHECK-NEXT:         }) == 0) as i32)
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             __builtin_strncmp(
// CHECK-NEXT:                 second.0.as_mut_ptr() as *const i8,
// CHECK-NEXT:                 c"clang-head-gnu".as_ptr(),
// CHECK-NEXT:                 14u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == 0) as i32)
// CHECK-NEXT:         + (((unsafe { __builtin_strchr(second.0.as_mut_ptr() as *const i8, 45i32) })
// CHECK-NEXT:             == unsafe { (second.0.as_mut_ptr() as *mut i8).add(5) }) as i32)
// CHECK-NEXT:         + (((unsafe { __builtin_strrchr(second.0.as_mut_ptr() as *const i8, 45i32) })
// CHECK-NEXT:             == unsafe { (second.0.as_mut_ptr() as *mut i8).add(10) }) as i32)
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             __builtin_strstr(second.0.as_mut_ptr() as *const i8, c"head".as_ptr())
// CHECK-NEXT:         }) == unsafe { (second.0.as_mut_ptr() as *mut i8).add(6) }) as i32)
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             __builtin_strspn(second.0.as_mut_ptr() as *const i8, c"clang".as_ptr())
// CHECK-NEXT:         }) as u32) as i32)
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             __builtin_strcspn(second.0.as_mut_ptr() as *const i8, c"-".as_ptr())
// CHECK-NEXT:         }) as u32) as i32)
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             __builtin_strpbrk(second.0.as_mut_ptr() as *const i8, c"-g".as_ptr())
// CHECK-NEXT:         }) == unsafe { (second.0.as_mut_ptr() as *mut i8).add(4) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", gnu_builtin_stack(), gnu_builtin_memory(), gnu_builtin_strings()
// CHECK-NEXT:     );
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
