#include <stdio.h>

static char greeting[] = "hello";
char padded[8] = "ab";
const char label[] = "label";
static char grid[2][4] = {"abc", "xyz"};
static const char *words[] = {"zero", "one", "two"};
static char high[] = "\x80\xff\x7f";

static int total(const char *s) {
  int sum = 0;
  while (*s) {
    sum += *s;
    s++;
  }
  return sum;
}

int main(void) {
  char local[] = "local";
  greeting[0] = 'j';
  padded[2] = 'c';
  grid[1][0] = 'w';
  local[0] = 'L';
  printf("%s %s %s %s %s %s\n", greeting, padded, label, grid[0], grid[1], local);
  printf("%d %d %d %d %d %d\n", padded[5], total(words[1]), total(words[2]), high[0], high[1], high[2]);
  for (int i = 0; i < 3; i++)
    printf("%s\n", words[i]);
  return total(greeting) % 256;
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
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut greeting: [i8; 6] = [
// CHECK-NEXT:     104 as i8,
// CHECK-NEXT:     101 as i8,
// CHECK-NEXT:     108 as i8,
// CHECK-NEXT:     108 as i8,
// CHECK-NEXT:     111 as i8,
// CHECK-NEXT:     0 as i8,
// CHECK-NEXT: ];
// CHECK-NEXT: static mut padded: [i8; 8] = [
// CHECK-NEXT:     97 as i8,
// CHECK-NEXT:     98 as i8,
// CHECK-NEXT:     0 as i8,
// CHECK-NEXT:     0 as i8,
// CHECK-NEXT:     0 as i8,
// CHECK-NEXT:     0 as i8,
// CHECK-NEXT:     0 as i8,
// CHECK-NEXT:     0 as i8,
// CHECK-NEXT: ];
// CHECK-NEXT: static mut label: [i8; 6] = [
// CHECK-NEXT:     108 as i8,
// CHECK-NEXT:     97 as i8,
// CHECK-NEXT:     98 as i8,
// CHECK-NEXT:     101 as i8,
// CHECK-NEXT:     108 as i8,
// CHECK-NEXT:     0 as i8,
// CHECK-NEXT: ];
// CHECK-NEXT: static mut grid: {{\[\[}}i8; 4]; 2] = [
// CHECK-NEXT:     [97 as i8, 98 as i8, 99 as i8, 0 as i8],
// CHECK-NEXT:     [120 as i8, 121 as i8, 122 as i8, 0 as i8],
// CHECK-NEXT: ];
// CHECK-NEXT: static mut words: __SlateAlign16<[*const i8; 3]> = __SlateAlign16([
// CHECK-NEXT:     (b"zero\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:     (b"one\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT:     (b"two\0".as_ptr() as *mut i8) as *const i8,
// CHECK-NEXT: ]);
// CHECK-NEXT: static mut high: [i8; 4] = [-128 as i8, -1 as i8, 127 as i8, 0 as i8];
// CHECK-NEXT: fn total(mut s: *const i8) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     while (unsafe { *s }) != 0 {
// CHECK-NEXT:         sum += (unsafe { *s }) as i32;
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: *const i8 = s;
// CHECK-NEXT:         s = unsafe { [[V_0]].offset(1 as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut local: [i8; 6] = [108, 111, 99, 97, 108, 0];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         greeting[0] = 106;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         padded[2] = 99;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         grid[1][0] = 119;
// CHECK-NEXT:     }
// CHECK-NEXT:     local[0] = 76;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %s %s %s %s %s\n".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(greeting) as *mut i8,
// CHECK-NEXT:             std::ptr::addr_of_mut!(padded) as *mut i8,
// CHECK-NEXT:             std::ptr::addr_of!(label) as *const i8,
// CHECK-NEXT:             std::ptr::addr_of_mut!(grid[0]) as *mut i8,
// CHECK-NEXT:             std::ptr::addr_of_mut!(grid[1]) as *mut i8,
// CHECK-NEXT:             local.as_mut_ptr() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {}", (unsafe { padded[5] }) as i32, total(unsafe { words.0[1] }),
// CHECK-NEXT:         total(unsafe { words.0[2] }), (unsafe { high[0] }) as i32, (unsafe { high[1] })
// CHECK-NEXT:         as i32, (unsafe { high[2] }) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     for i in 0 as i32..3 as i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%s\n".as_ptr(), words.0[i as usize]) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     total((unsafe { std::ptr::addr_of_mut!(greeting) as *mut i8 }) as *const i8) % 256
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
