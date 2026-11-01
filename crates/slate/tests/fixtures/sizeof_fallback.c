#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Pair {
  int  left;
  char right;
};

struct Packed {
  char tag;
  int  value;
} __attribute__((packed));

struct Holder {
  long double wide;
  char        tag;
};

typedef int aligned_int __attribute__((aligned(16)));

struct Only_measured {
  short a;
  short b;
};

static unsigned long table[sizeof(struct Pair)];
static const unsigned long global_size = sizeof(struct Pair) * 2;

static int f(void) { return 7; }

static int classify(unsigned long size) {
  switch (size) {
  case sizeof(char):
    return 1;
  case sizeof(short):
    return 2;
  case sizeof(struct Pair):
    return 8;
  default:
    return 0;
  }
}

static void report_folded(void) {
  printf("%d\n", (int)sizeof(void));
  printf("%d\n", (int)sizeof(f));
  printf("%d\n", (int)sizeof(long double));
  printf("%d\n", (int)sizeof(_Atomic int));
  printf("%d\n", (int)sizeof(aligned_int));
}

static void report_kept(void) {
  int   values[5];
  char *text = "abc";
  printf("%d\n", (int)sizeof(values));
  printf("%d\n", (int)(sizeof(values) / sizeof(values[0])));
  printf("%d\n", (int)sizeof(text));
  printf("%d\n", (int)sizeof(struct Packed));
  printf("%d\n", (int)sizeof(struct Holder));
  printf("%d\n", (int)sizeof(struct Only_measured));
  printf("%d\n", (int)sizeof(table));
  printf("%lu\n", global_size);
}

static int sum_loop(void) {
  int values[4] = {1, 2, 3, 4};
  int total     = 0;
  for (unsigned long i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
    total += values[i];
  }
  return total;
}

static int heap_and_memory(void) {
  int        *items = malloc(3 * sizeof(int));
  struct Pair a     = {1, 'x'};
  struct Pair b;
  if (!items) {
    return -1;
  }
  memset(items, 0, 3 * sizeof(int));
  memcpy(&b, &a, sizeof(a));
  items[2] = b.left + 1;
  int result = items[0] + items[1] + items[2];
  free(items);
  return result;
}

int main(void) {
  report_folded();
  report_kept();
  printf("%d\n", classify(sizeof(short)));
  printf("%d\n", classify(sizeof(struct Pair)));
  printf("%d\n", sum_loop());
  printf("%d\n", heap_and_memory());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn malloc([[ARG_0:arg[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn free([[ARG_0]]: *mut ());
// CHECK-NEXT:     fn memcpy([[ARG_0]]: *mut (), [[ARG_1:arg[0-9]+]]: *const (), [[ARG_2:arg[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn memset([[ARG_0]]: *mut (), [[ARG_1]]: i32, [[ARG_2]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Pair {
// CHECK-NEXT:     left: i32,
// CHECK-NEXT:     right: i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Packed {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Only_measured {
// CHECK-NEXT:     a: i16,
// CHECK-NEXT:     b: i16,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut table: __SlateAlign16<[u64; 8]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut global_size: u64 = (std::mem::size_of::<Pair>() as u64).wrapping_mul(2u64);
// CHECK-NEXT: fn f() -> i32 {
// CHECK-NEXT:     7
// CHECK-NEXT: }
// CHECK-NEXT: fn classify(size: u64) -> i32 {
// CHECK-NEXT:     match size {
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             return 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             return 2;
// CHECK-NEXT:         }
// CHECK-NEXT:         8 => {
// CHECK-NEXT:             return 8;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 0;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn report_folded() {
// CHECK-NEXT:     println!("{}", 1i32);
// CHECK-NEXT:     println!("{}", 1i32);
// CHECK-NEXT:     println!("{}", 16i32);
// CHECK-NEXT:     println!("{}", 4i32);
// CHECK-NEXT:     println!("{}", 4i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn report_kept() {
// CHECK-NEXT:     __SlateAlign16([0i32; 5]);
// CHECK-NEXT:     println!("{}", std::mem::size_of::< [i32; 5] > () as i32);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", (((std::mem::size_of::< [i32; 5] > () as u64) / (std::mem::size_of::< i32 >
// CHECK-NEXT:         () as u64)) as u32) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", std::mem::size_of::<* mut i8 > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::size_of::< Packed > () as i32);
// CHECK-NEXT:     println!("{}", 32i32);
// CHECK-NEXT:     println!("{}", std::mem::size_of::< Only_measured > () as i32);
// CHECK-NEXT:     println!("{}", std::mem::size_of::< [u64; 8] > () as i32);
// CHECK-NEXT:     unsafe { slate_support::printf(c"%lu\n".as_ptr(), global_size) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_loop() -> i32 {
// CHECK-NEXT:     let values: __SlateAlign16<[i32; 4]> = __SlateAlign16([1i32, 2i32, 3i32, 4i32]);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..((std::mem::size_of::<[i32; 4]>() as u64)
// CHECK-NEXT:         / (std::mem::size_of::<i32>() as u64))
// CHECK-NEXT:     {
// CHECK-NEXT:         total += values.0[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn heap_and_memory() -> i32 {
// CHECK-NEXT:     let items: *mut i32 = (unsafe {
// CHECK-NEXT:         malloc(3u64.wrapping_mul(std::mem::size_of::<i32>() as u64))
// CHECK-NEXT:     }) as *mut i32;
// CHECK-NEXT:     let mut a: Pair = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: Pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].left = 1i32;
// CHECK-NEXT:         [[T_0]].right = 120i8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut b: Pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if items.is_null() {
// CHECK-NEXT:         return -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             items as *mut u8,
// CHECK-NEXT:             0u8,
// CHECK-NEXT:             3u64.wrapping_mul(std::mem::size_of::<i32>() as u64) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (std::ptr::addr_of_mut!(a) as *const ()) as *const u8,
// CHECK-NEXT:             (std::ptr::addr_of_mut!(b) as *mut ()) as *mut u8,
// CHECK-NEXT:             std::mem::size_of::<Pair>(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *items.add(2) = b.left + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let result: i32 = (unsafe { *items.add(0) }) + unsafe { *items.add(1) }
// CHECK-NEXT:         + unsafe { *items.add(2) };
// CHECK-NEXT:     unsafe { free(items as *mut ()) };
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     report_folded();
// CHECK-NEXT:     report_kept();
// CHECK-NEXT:     println!("{}", classify(std::mem::size_of::< i16 > () as u64));
// CHECK-NEXT:     println!("{}", classify(std::mem::size_of::< Pair > () as u64));
// CHECK-NEXT:     println!("{}", sum_loop());
// CHECK-NEXT:     println!("{}", heap_and_memory());
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
