#include <stdio.h>

typedef struct {
  int value;
  unsigned : 0;
} __attribute__((aligned(16))) AlignedTail;

typedef struct {
  char tag;
  int : 0;
  int value;
} MidBarrier;

typedef struct __attribute__((packed)) {
  char tag;
  int : 0;
  int value;
} PackedBarrier;

__attribute__((noinline)) static int take_aligned(AlignedTail value) {
  return value.value + (int)sizeof(value);
}

__attribute__((noinline)) static MidBarrier round_trip(MidBarrier value) {
  value.value += value.tag;
  return value;
}

__attribute__((noinline)) static int take_packed(PackedBarrier value) {
  return value.tag + value.value + (int)sizeof(value);
}

int main(void) {
  AlignedTail   aligned = {.value = 7};
  MidBarrier    middle  = {.tag = 3, .value = 20};
  PackedBarrier packed  = {.tag = 4, .value = 30};
  middle                = round_trip(middle);
  printf("%d %d %d %zu %zu %zu\n", take_aligned(aligned), middle.value,
         take_packed(packed), sizeof(aligned), sizeof(middle), sizeof(packed));
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
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_2:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     __slate_pad_1: [u8; 3],
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn take_aligned(value: [[SLATERECORD_0]]) -> i32 {
// CHECK-NEXT:     value.value + 16
// CHECK-NEXT: }
// CHECK-NEXT: fn round_trip(mut value: [[SLATERECORD_1]]) -> [[SLATERECORD_1]] {
// CHECK-NEXT:     value.value += value.tag as i32;
// CHECK-NEXT:     return value;
// CHECK-NEXT: }
// CHECK-NEXT: fn take_packed(value: [[SLATERECORD_2]]) -> i32 {
// CHECK-NEXT:     (value.tag as i32) + value.value + 8
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let aligned: [[SLATERECORD_0]] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].value = 7 as i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut middle: [[SLATERECORD_1]] = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]].tag = 3 as i8;
// CHECK-NEXT:         [[T_1]].value = 20 as i32;
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let packed: [[SLATERECORD_2]] = {
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: [[SLATERECORD_2]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]].tag = 4 as i8;
// CHECK-NEXT:         [[T_2]].value = 30 as i32;
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     middle = round_trip(middle);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %zu %zu %zu\n".as_ptr(),
// CHECK-NEXT:             take_aligned(aligned),
// CHECK-NEXT:             middle.value,
// CHECK-NEXT:             take_packed(packed),
// CHECK-NEXT:             16 as u64,
// CHECK-NEXT:             8 as u64,
// CHECK-NEXT:             8 as u64,
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
