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

// REWRITES-NOT: alpha.len() as u64

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT:     fn strncmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char, _2: usize) -> i32;
// LOWERING-NEXT:     fn memcmp(_0: *const core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> i32;
// LOWERING-NEXT:     fn strchr(_0: *const core::ffi::c_char, _1: i32) -> *mut core::ffi::c_char;
// LOWERING-NEXT:     fn strrchr(_0: *const core::ffi::c_char, _1: i32) -> *mut core::ffi::c_char;
// LOWERING-NEXT:     fn strstr(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char)
// LOWERING-NEXT:     -> *mut core::ffi::c_char;
// LOWERING-NEXT:     fn strpbrk(
// LOWERING-NEXT:         _0: *const core::ffi::c_char,
// LOWERING-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-NEXT:     ) -> *mut core::ffi::c_char;
// LOWERING-NEXT:     fn strspn(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> usize;
// LOWERING-NEXT:     fn strcspn(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> usize;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-X86_64-GNU-NEXT:     let mut alpha: [i8; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     let mut beta: [i8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut alpha: [u8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut beta: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut bytes_a: [u8; 3] = [0; 3];
// LOWERING-NEXT:     let mut bytes_b: [u8; 3] = [0; 3];
// LOWERING-X86_64-GNU-NEXT:     let mut hay: [i8; 7] = [0; 7];
// LOWERING-X86_64-GNU-NEXT:     let mut sub: [i8; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     let mut empty: [i8; 1] = [0; 1];
// LOWERING-X86_64-GNU-NEXT:     let mut set: [i8; 3] = [0; 3];
// LOWERING-X86_64-GNU-NEXT:     let mut prefix: [i8; 3] = [0; 3];
// LOWERING-X86_64-GNU-NEXT:     let mut reject: [i8; 3] = [0; 3];
// LOWERING-X86_64-GNU-NEXT:     let mut utf8: [i8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut hay: [u8; 7] = [0; 7];
// LOWERING-AARCH64-GNU-NEXT:     let mut sub: [u8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut empty: [u8; 1] = [0; 1];
// LOWERING-AARCH64-GNU-NEXT:     let mut set: [u8; 3] = [0; 3];
// LOWERING-AARCH64-GNU-NEXT:     let mut prefix: [u8; 3] = [0; 3];
// LOWERING-AARCH64-GNU-NEXT:     let mut reject: [u8; 3] = [0; 3];
// LOWERING-AARCH64-GNU-NEXT:     let mut utf8: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [97, 98, 99, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [97, 98, 99, 0];
// LOWERING-NEXT:     alpha = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [97, 98, 100, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [97, 98, 100, 0];
// LOWERING-NEXT:     beta = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: [u8; 3] = [255, 1, 0];
// LOWERING-NEXT:     bytes_a = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: [u8; 3] = [255, 2, 0];
// LOWERING-NEXT:     bytes_b = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 7] = [97, 98, 97, 99, 97, 100, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 7] = [97, 98, 97, 99, 97, 100, 0];
// LOWERING-NEXT:     hay = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [97, 99, 97, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [97, 99, 97, 0];
// LOWERING-NEXT:     sub = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 1] = [0; 1];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 1] = [0; 1];
// LOWERING-NEXT:     empty = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 3] = [99, 120, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 3] = [99, 120, 0];
// LOWERING-NEXT:     set = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 3] = [97, 98, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 3] = [97, 98, 0];
// LOWERING-NEXT:     prefix = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 3] = [99, 100, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 3] = [99, 100, 0];
// LOWERING-NEXT:     reject = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [104, -61, -87, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [104, 195, 169, 0];
// LOWERING-NEXT:     utf8 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 169;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = alpha.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = alpha.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = alpha.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = alpha.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = alpha.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = alpha.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = alpha.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = beta.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = alpha.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = beta.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = alpha.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = beta.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = alpha.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = beta.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strncmp(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = bytes_a.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = bytes_b.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         memcmp(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 97;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// LOWERING-AARCH64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 97;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:         (unsafe { strrchr({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%c %c %d %d %d %d %zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// LOWERING-AARCH64-GNU-NEXT:         (unsafe { strrchr({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%c %c %d %d %d %d %zu %zu\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(4) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(4) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = sub.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = sub.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// LOWERING-NEXT:         strstr(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-X86_64-GNU-NEXT:     }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = empty.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = empty.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// LOWERING-NEXT:         strstr(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-X86_64-GNU-NEXT:     }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = set.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = set.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// LOWERING-NEXT:         strpbrk(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-X86_64-GNU-NEXT:     }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = prefix.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = prefix.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// LOWERING-NEXT:         strspn(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as u64;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = reject.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = reject.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// LOWERING-NEXT:         strcspn(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// LOWERING-AARCH64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 122;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// LOWERING-AARCH64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = utf8.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = utf8.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// LOWERING-AARCH64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// REWRITES-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT:     fn strncmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char, _2: usize) -> i32;
// REWRITES-NEXT:     fn memcmp(_0: *const core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> i32;
// REWRITES-NEXT:     fn strchr(_0: *const core::ffi::c_char, _1: i32) -> *mut core::ffi::c_char;
// REWRITES-NEXT:     fn strrchr(_0: *const core::ffi::c_char, _1: i32) -> *mut core::ffi::c_char;
// REWRITES-NEXT:     fn strstr(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char)
// REWRITES-NEXT:     -> *mut core::ffi::c_char;
// REWRITES-NEXT:     fn strpbrk(
// REWRITES-NEXT:         _0: *const core::ffi::c_char,
// REWRITES-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-NEXT:     ) -> *mut core::ffi::c_char;
// REWRITES-NEXT:     fn strspn(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> usize;
// REWRITES-NEXT:     fn strcspn(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> usize;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-X86_64-GNU-NEXT:     let mut alpha: [i8; 4] = [0; 4];
// REWRITES-X86_64-GNU-NEXT:     let mut beta: [i8; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let mut alpha: [u8; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let mut beta: [u8; 4] = [0; 4];
// REWRITES-NEXT:     let mut bytes_a: [u8; 3] = [0; 3];
// REWRITES-NEXT:     let mut bytes_b: [u8; 3] = [0; 3];
// REWRITES-X86_64-GNU-NEXT:     let mut hay: [i8; 7] = [0; 7];
// REWRITES-X86_64-GNU-NEXT:     let mut sub: [i8; 4] = [0; 4];
// REWRITES-X86_64-GNU-NEXT:     let mut empty: [i8; 1] = [0; 1];
// REWRITES-X86_64-GNU-NEXT:     let mut set: [i8; 3] = [0; 3];
// REWRITES-X86_64-GNU-NEXT:     let mut prefix: [i8; 3] = [0; 3];
// REWRITES-X86_64-GNU-NEXT:     let mut reject: [i8; 3] = [0; 3];
// REWRITES-X86_64-GNU-NEXT:     let mut utf8: [i8; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let mut hay: [u8; 7] = [0; 7];
// REWRITES-AARCH64-GNU-NEXT:     let mut sub: [u8; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let mut empty: [u8; 1] = [0; 1];
// REWRITES-AARCH64-GNU-NEXT:     let mut set: [u8; 3] = [0; 3];
// REWRITES-AARCH64-GNU-NEXT:     let mut prefix: [u8; 3] = [0; 3];
// REWRITES-AARCH64-GNU-NEXT:     let mut reject: [u8; 3] = [0; 3];
// REWRITES-AARCH64-GNU-NEXT:     let mut utf8: [u8; 4] = [0; 4];
// REWRITES-NEXT:     alpha = [97, 98, 99, 0];
// REWRITES-NEXT:     beta = [97, 98, 100, 0];
// REWRITES-NEXT:     bytes_a = [255, 1, 0];
// REWRITES-NEXT:     bytes_b = [255, 2, 0];
// REWRITES-NEXT:     hay = [97, 98, 97, 99, 97, 100, 0];
// REWRITES-NEXT:     sub = [97, 99, 97, 0];
// REWRITES-NEXT:     empty = [0; 1];
// REWRITES-NEXT:     set = [99, 120, 0];
// REWRITES-NEXT:     prefix = [97, 98, 0];
// REWRITES-NEXT:     reject = [99, 100, 0];
// REWRITES-X86_64-GNU-NEXT:     utf8 = [104, -61, -87, 0];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%zu %d %d %d %d\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = alpha.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     utf8 = [104, 195, 169, 0];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%zu %d %d %d %d\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = alpha.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = alpha.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = alpha.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = alpha.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = alpha.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         strcmp(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == 0) as i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = alpha.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = beta.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = alpha.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = beta.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         strcmp(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} < 0) as i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = alpha.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = beta.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = alpha.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = beta.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         strncmp(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             (2 as u64) as usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == 0) as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = bytes_a.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = bytes_b.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         memcmp(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-NEXT:             (1 as u64) as usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             ({{__v[0-9]+}} == 0) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-X86_64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, 97 as i32) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-X86_64-GNU-NEXT:         (unsafe { strrchr({{__v[0-9]+}} as *const core::ffi::c_char, 97 as i32) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%c %c %d %d %d %d %zu %zu\n".as_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// REWRITES-AARCH64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, 97 as i32) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// REWRITES-AARCH64-GNU-NEXT:         (unsafe { strrchr({{__v[0-9]+}} as *const core::ffi::c_char, 97 as i32) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%c %c %d %d %d %d %zu %zu\n".as_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { *{{__v[0-9]+}} }) as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { *{{__v[0-9]+}} }) as i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add(4) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(4) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == {{__v[0-9]+}}) as i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = sub.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = sub.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// REWRITES-NEXT:         strstr(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     }) as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     }) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} != std::ptr::null_mut()) as i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = empty.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = empty.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// REWRITES-NEXT:         strstr(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     }) as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     }) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} != std::ptr::null_mut()) as i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = set.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = set.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// REWRITES-NEXT:         strpbrk(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     }) as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     }) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == std::ptr::null_mut()) as i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = prefix.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = prefix.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// REWRITES-NEXT:         strspn(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as u64;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = reject.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = reject.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             (unsafe {
// REWRITES-NEXT:                 strcspn(
// REWRITES-NEXT:                     {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:                     {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:                 )
// REWRITES-NEXT:             }) as u64,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-X86_64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, 0 as i32) }) as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d %d\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// REWRITES-AARCH64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, 0 as i32) }) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} != std::ptr::null_mut()) as i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = hay.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-X86_64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, 122 as i32) }) as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = hay.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// REWRITES-AARCH64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, 122 as i32) }) as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             ({{__v[0-9]+}} == std::ptr::null_mut()) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = utf8.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-X86_64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, 169 as i32) }) as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = utf8.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// REWRITES-AARCH64-GNU-NEXT:         (unsafe { strchr({{__v[0-9]+}} as *const core::ffi::c_char, 169 as i32) }) as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             ({{__v[0-9]+}} != std::ptr::null_mut()) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
