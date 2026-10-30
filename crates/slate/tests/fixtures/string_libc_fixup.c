#include <stdio.h>
#include <string.h>

int main(void) {
  char          alpha[]     = "abc";
  char          beta[]      = "abd";
  unsigned char bytes_a[]   = "\xff\x01";
  unsigned char bytes_b[]   = "\xff\x02";
  char          hay[]       = "abacad";
  char          sub[]       = "aca";
  char          empty[]     = "";
  char          set[]       = "cx";
  char          prefix[]    = "ab";
  char          reject[]    = "cd";
  char          utf8[]      = "hé";
  int           second_byte = 0xa9;

  printf("%zu %d %d %d %d\n", strlen(alpha), strcmp(alpha, alpha) == 0,
         strcmp(alpha, beta) < 0, strncmp(alpha, beta, 2) == 0,
         memcmp(bytes_a, bytes_b, 1) == 0);
  char *first = strchr(hay, 'a');
  char *last  = strrchr(hay, 'a');
  printf("%c %c %d %d %d %d %zu %zu\n", *first, *last, last == first + 4,
         strstr(hay, sub) != 0, strstr(hay, empty) != 0, strpbrk(hay, set) == 0,
         strspn(hay, prefix), strcspn(hay, reject));
  printf("%d %d\n", strchr(hay, 0) != 0, strchr(hay, 'z') == 0);
  printf("%d\n", strchr(utf8, second_byte) != 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn memcmp([[ARG_0]]: *const (), [[ARG_1:arg[0-9]+]]: *const (), [[ARG_2:arg[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     fn strcmp([[ARG_0]]: *const i8, [[ARG_1]]: *const i8) -> i32;
// CHECK-NEXT:     fn strncmp([[ARG_0]]: *const i8, [[ARG_1]]: *const i8, [[ARG_2]]: u64) -> i32;
// CHECK-NEXT:     fn strchr([[ARG_0]]: *const i8, [[ARG_1]]: i32) -> *mut i8;
// CHECK-NEXT:     fn strrchr([[ARG_0]]: *const i8, [[ARG_1]]: i32) -> *mut i8;
// CHECK-NEXT:     fn strcspn([[ARG_0]]: *const i8, [[ARG_1]]: *const i8) -> u64;
// CHECK-NEXT:     fn strspn([[ARG_0]]: *const i8, [[ARG_1]]: *const i8) -> u64;
// CHECK-NEXT:     fn strpbrk([[ARG_0]]: *const i8, [[ARG_1]]: *const i8) -> *mut i8;
// CHECK-NEXT:     fn strstr([[ARG_0]]: *const i8, [[ARG_1]]: *const i8) -> *mut i8;
// CHECK-NEXT:     fn strlen([[ARG_0]]: *const i8) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut alpha: [i8; 4] = [97, 98, 99, 0];
// CHECK-NEXT:     let mut beta: [i8; 4] = [97, 98, 100, 0];
// CHECK-NEXT:     let mut bytes_a: [u8; 3] = [255, 1, 0];
// CHECK-NEXT:     let mut bytes_b: [u8; 3] = [255, 2, 0];
// CHECK-NEXT:     let mut hay: [i8; 7] = [97, 98, 97, 99, 97, 100, 0];
// CHECK-NEXT:     let mut sub: [i8; 4] = [97, 99, 97, 0];
// CHECK-NEXT:     let mut empty: [i8; 1] = [0];
// CHECK-NEXT:     let mut set: [i8; 3] = [99, 120, 0];
// CHECK-NEXT:     let mut prefix: [i8; 3] = [97, 98, 0];
// CHECK-NEXT:     let mut reject: [i8; 3] = [99, 100, 0];
// CHECK-NEXT:     let mut utf8: [i8; 4] = [104, -61, -87, 0];
// CHECK-NEXT:     let second_byte: i32 = 169;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%zu %d %d %d %d\n".as_ptr(),
// CHECK-NEXT:             strlen(alpha.as_mut_ptr() as *const i8),
// CHECK-NEXT:             (strcmp(alpha.as_mut_ptr() as *const i8, alpha.as_mut_ptr() as *const i8)
// CHECK-NEXT:                 == 0) as i32,
// CHECK-NEXT:             (strcmp(alpha.as_mut_ptr() as *const i8, beta.as_mut_ptr() as *const i8) < 0)
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:             (strncmp(
// CHECK-NEXT:                 alpha.as_mut_ptr() as *const i8,
// CHECK-NEXT:                 beta.as_mut_ptr() as *const i8,
// CHECK-NEXT:                 2u64,
// CHECK-NEXT:             ) == 0) as i32,
// CHECK-NEXT:             (memcmp(
// CHECK-NEXT:                 bytes_a.as_mut_ptr() as *const (),
// CHECK-NEXT:                 bytes_b.as_mut_ptr() as *const (),
// CHECK-NEXT:                 1u64,
// CHECK-NEXT:             ) == 0) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let first: *mut i8 = unsafe { strchr(hay.as_mut_ptr() as *const i8, 97i32) };
// CHECK-NEXT:     let last: *mut i8 = unsafe { strrchr(hay.as_mut_ptr() as *const i8, 97i32) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%c %c %d %d %d %d %zu %zu\n".as_ptr(),
// CHECK-NEXT:             *first as i32,
// CHECK-NEXT:             *last as i32,
// CHECK-NEXT:             (last == first.add(4)) as i32,
// CHECK-NEXT:             !strstr(hay.as_mut_ptr() as *const i8, sub.as_mut_ptr() as *const i8)
// CHECK-NEXT:                 .is_null() as i32,
// CHECK-NEXT:             !strstr(hay.as_mut_ptr() as *const i8, empty.as_mut_ptr() as *const i8)
// CHECK-NEXT:                 .is_null() as i32,
// CHECK-NEXT:             strpbrk(hay.as_mut_ptr() as *const i8, set.as_mut_ptr() as *const i8)
// CHECK-NEXT:                 .is_null() as i32,
// CHECK-NEXT:             strspn(hay.as_mut_ptr() as *const i8, prefix.as_mut_ptr() as *const i8),
// CHECK-NEXT:             strcspn(hay.as_mut_ptr() as *const i8, reject.as_mut_ptr() as *const i8),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let __slate_printf_arg0 = !unsafe { strchr(hay.as_mut_ptr() as *const i8, 0i32) }
// CHECK-NEXT:         .is_null() as i32;
// CHECK-NEXT:     let __slate_printf_arg1 = unsafe { strchr(hay.as_mut_ptr() as *const i8, 122i32) }
// CHECK-NEXT:         .is_null() as i32;
// CHECK-NEXT:     println!("{} {}", __slate_printf_arg0, __slate_printf_arg1);
// CHECK-NEXT:     let __slate_printf_arg0 = !unsafe {
// CHECK-NEXT:         strchr(utf8.as_mut_ptr() as *const i8, second_byte)
// CHECK-NEXT:     }
// CHECK-NEXT:         .is_null() as i32;
// CHECK-NEXT:     println!("{}", __slate_printf_arg0);
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
