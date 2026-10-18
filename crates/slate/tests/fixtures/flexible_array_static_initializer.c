#include <stdio.h>
#include <string.h>

struct Tail {
  int  count;
  char tag;
  char name[];
};

struct Tail tail = {3, 'x', "hello world"};
char       *name_pointer = tail.name;

union Bytes {
  short value;
  char  bytes[];
};

static union Bytes bytes = {.bytes = {1, 2, 3, 4, 5}};

struct Packed {
  char a;
  int  values[];
} __attribute__((packed));

struct Packed packed = {7, {10, 20, 30}};

int main(void) {
  printf("%zu %zu %zu\n", sizeof(tail), sizeof(bytes), sizeof(packed));
  printf("%d %c %s %s\n", tail.count, tail.tag, tail.name, name_pointer);
  printf("%d\n", (int)((char *)tail.name - (char *)&tail));
  struct Tail copy = tail;
  printf("%d %c\n", copy.count, copy.tag);
  tail.name[0] = 'H';
  printf("%s %zu\n", name_pointer, strlen(tail.name));
  for (int i = 0; i < 5; ++i)
    printf("%d ", bytes.bytes[i]);
  printf("%d\n", bytes.value);
  printf("%d %d %d %d\n", packed.a, packed.values[0], packed.values[1], packed.values[2]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn strlen(__s: *const i8) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Tail {
// CHECK-NEXT:     count: i32,
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     name: [i8; 0],
// CHECK-NEXT: }
// CHECK-NEXT: static mut tail: __SlateStorage_tail = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: __SlateStorage_tail = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].count = 3 as i32;
// CHECK-NEXT:     [[T_0]].tag = (120 as i32) as i8;
// CHECK-NEXT:     [[T_0]].name = [
// CHECK-NEXT:         104 as i8,
// CHECK-NEXT:         101 as i8,
// CHECK-NEXT:         108 as i8,
// CHECK-NEXT:         108 as i8,
// CHECK-NEXT:         111 as i8,
// CHECK-NEXT:         32 as i8,
// CHECK-NEXT:         119 as i8,
// CHECK-NEXT:         111 as i8,
// CHECK-NEXT:         114 as i8,
// CHECK-NEXT:         108 as i8,
// CHECK-NEXT:         100 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: static mut name_pointer: *mut i8 = unsafe {
// CHECK-NEXT:     std::ptr::addr_of_mut!((* (std::ptr::addr_of_mut!(tail) as * mut Tail)).name)
// CHECK-NEXT:         as *mut i8
// CHECK-NEXT: };
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateStorage_tail {
// CHECK-NEXT:     count: i32,
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     name: [i8; 12],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union __SlateStorage_bytes {
// CHECK-NEXT:     value: i16,
// CHECK-NEXT:     bytes: [i8; 5],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateStorage_packed {
// CHECK-NEXT:     a: i8,
// CHECK-NEXT:     values: [i32; 3],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union Bytes {
// CHECK-NEXT:     value: i16,
// CHECK-NEXT:     bytes: [i8; 0],
// CHECK-NEXT: }
// CHECK-NEXT: static mut bytes: __SlateStorage_bytes = {
// CHECK-NEXT:     let mut [[T_0]]: __SlateStorage_bytes = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].bytes = [
// CHECK-NEXT:         (1 as i32) as i8,
// CHECK-NEXT:         (2 as i32) as i8,
// CHECK-NEXT:         (3 as i32) as i8,
// CHECK-NEXT:         (4 as i32) as i8,
// CHECK-NEXT:         (5 as i32) as i8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Packed {
// CHECK-NEXT:     a: i8,
// CHECK-NEXT:     values: [i32; 0],
// CHECK-NEXT: }
// CHECK-NEXT: static mut packed: __SlateStorage_packed = {
// CHECK-NEXT:     let mut [[T_0]]: __SlateStorage_packed = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].a = (7 as i32) as i8;
// CHECK-NEXT:     [[T_0]].values = [10 as i32, 20 as i32, 30 as i32];
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%zu %zu %zu\n".as_ptr(), 8 as u64, 2 as u64, 1 as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %c %s %s\n".as_ptr(),
// CHECK-NEXT:             (*(std::ptr::addr_of_mut!(tail) as *mut Tail)).count,
// CHECK-NEXT:             (*(std::ptr::addr_of_mut!(tail) as *mut Tail)).tag as i32,
// CHECK-NEXT:             std::ptr::addr_of_mut!((* (std::ptr::addr_of_mut!(tail) as * mut Tail)).name)
// CHECK-NEXT:                 as *mut i8,
// CHECK-NEXT:             name_pointer,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", ((unsafe { (std::ptr::addr_of_mut!((* (std::ptr::addr_of_mut!(tail) as *
// CHECK-NEXT:         mut Tail)).name) as * mut i8).offset_from(std::ptr::addr_of_mut!(*
// CHECK-NEXT:         (std::ptr::addr_of_mut!(tail) as * mut Tail)) as * mut i8) }) as i64) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let copy: Tail = unsafe { *(std::ptr::addr_of_mut!(tail) as *mut Tail) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %c\n".as_ptr(), copy.count, copy.tag as i32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(std::ptr::addr_of_mut!((* (std::ptr::addr_of_mut!(tail) as * mut Tail)).name)
// CHECK-NEXT:             as *mut i8)
// CHECK-NEXT:             .offset(0 as isize) = 72;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %zu\n".as_ptr(),
// CHECK-NEXT:             name_pointer,
// CHECK-NEXT:             strlen(
// CHECK-NEXT:                 (std::ptr::addr_of_mut!(
// CHECK-NEXT:                     (* (std::ptr::addr_of_mut!(tail) as * mut Tail)).name
// CHECK-NEXT:                 ) as *mut i8) as *const i8,
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     for i in 0 as i32..5 as i32 {
// CHECK-NEXT:         print!(
// CHECK-NEXT:             "{} ", (unsafe { * (std::ptr::addr_of_mut!((* (std::ptr::addr_of_mut!(bytes)
// CHECK-NEXT:             as * mut Bytes)).bytes) as * mut i8).offset(i as isize) }) as i32
// CHECK-NEXT:         );
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", (unsafe { (* (std::ptr::addr_of_mut!(bytes) as * mut Bytes)).value }) as
// CHECK-NEXT:         i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", (unsafe { (* (std::ptr::addr_of_mut!(packed) as * mut Packed)).a
// CHECK-NEXT:         }) as i32, unsafe { (std::ptr::addr_of_mut!((* (std::ptr::addr_of_mut!(packed) as
// CHECK-NEXT:         * mut Packed)).values) as * mut i32).offset(0 as isize).read_unaligned() },
// CHECK-NEXT:         unsafe { (std::ptr::addr_of_mut!((* (std::ptr::addr_of_mut!(packed) as * mut
// CHECK-NEXT:         Packed)).values) as * mut i32).offset(1 as isize).read_unaligned() }, unsafe {
// CHECK-NEXT:         (std::ptr::addr_of_mut!((* (std::ptr::addr_of_mut!(packed) as * mut Packed))
// CHECK-NEXT:         .values) as * mut i32).offset(2 as isize).read_unaligned() }
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
