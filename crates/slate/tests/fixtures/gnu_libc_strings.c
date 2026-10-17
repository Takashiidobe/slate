#define _GNU_SOURCE
#include <argz.h>
#include <envz.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int gnu_string_extensions(void) {
  char       destination[16] = {};
  const char repeated[]      = "abca";
  char       obscured[]      = "gnu";
  char       tokens[]        = "a::bc";
  char      *cursor          = tokens;
  char      *token;
  char      *end         = mempcpy(destination, "slate", 6);
  int        token_score = 0;

  while ((token = strsep(&cursor, ":")) != NULL) {
    token_score = token_score * 10 + (int)strlen(token);
  }

  memfrob(obscured, 3);
  memfrob(obscured, 3);

  return (int)(end - destination) + (strcmp(destination, "slate") == 0) +
         ((const char *)memrchr(repeated, 'a', 4) - repeated) +
         ((const char *)rawmemchr(repeated, 'c') - repeated) +
         (strchrnul("abc", 'z') - "abc") +
         (strcasestr("GNU Library", "library") != NULL) +
         (strverscmp("release-2", "release-10") < 0) +
         (strcmp(obscured, "gnu") == 0) + token_score +
         (strcmp(strerrorname_np(EINVAL), "EINVAL") == 0) +
         (strerrordesc_np(EINVAL) != NULL);
}

static int gnu_argz_extensions(void) {
  char        *argz   = NULL;
  size_t       length = 0;
  char        *arguments[6];
  unsigned int replacements = 0;
  int          total        = 0;

  total += argz_create_sep("one:two:three", ':', &argz, &length) == 0;
  total += argz_count(argz, length) == 3;
  argz_extract(argz, length, arguments);
  total += strcmp(arguments[1], "two") == 0;
  total += argz_add(&argz, &length, "four") == 0;
  total += argz_replace(&argz, &length, "three", "THREE", &replacements) == 0;
  total += replacements == 1;
  total += argz_count(argz, length) == 4;
  argz_stringify(argz, length, ',');
  total += strcmp(argz, "one,two,THREE,four") == 0;
  free(argz);
  return total;
}

static int gnu_envz_extensions(void) {
  char  *envz   = NULL;
  size_t length = 0;
  int    total  = 0;

  total += envz_add(&envz, &length, "ALPHA", "one") == 0;
  total += envz_add(&envz, &length, "BETA", NULL) == 0;
  total += strcmp(envz_get(envz, length, "ALPHA"), "one") == 0;
  total += envz_entry(envz, length, "BETA") != NULL;
  envz_remove(&envz, &length, "ALPHA");
  total += envz_get(envz, length, "ALPHA") == NULL;
  envz_strip(&envz, &length);
  total += length == 0;
  free(envz);
  return total;
}

int main(void) {
  printf("%d %d %d\n", gnu_string_extensions(), gnu_argz_extensions(),
         gnu_envz_extensions());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn rawmemchr(__s: *const (), __c: i32) -> *mut ();
// CHECK-NEXT:     fn memrchr(__s: *const (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT:     fn strcmp(__s1: *const i8, __s2: *const i8) -> i32;
// CHECK-NEXT:     fn strchrnul(__s: *const i8, __c: i32) -> *mut i8;
// CHECK-NEXT:     fn strcasestr(__haystack: *const i8, __needle: *const i8) -> *mut i8;
// CHECK-NEXT:     fn mempcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT:     fn strlen(__s: *const i8) -> u64;
// CHECK-NEXT:     fn strerrordesc_np(__err: i32) -> *const i8;
// CHECK-NEXT:     fn strerrorname_np(__err: i32) -> *const i8;
// CHECK-NEXT:     fn strsep(__stringp: *mut *mut i8, __delim: *const i8) -> *mut i8;
// CHECK-NEXT:     fn strverscmp(__s1: *const i8, __s2: *const i8) -> i32;
// CHECK-NEXT:     fn memfrob(__s: *mut (), __n: u64) -> *mut ();
// CHECK-NEXT:     fn argz_create_sep(
// CHECK-NEXT:         __string: *const i8,
// CHECK-NEXT:         __sep: i32,
// CHECK-NEXT:         __argz: *mut *mut i8,
// CHECK-NEXT:         __len: *mut u64,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn argz_count(__argz: *const i8, __len: u64) -> u64;
// CHECK-NEXT:     fn argz_extract(__argz: *const i8, __len: u64, __argv: *mut *mut i8);
// CHECK-NEXT:     fn argz_stringify(__argz: *mut i8, __len: u64, __sep: i32);
// CHECK-NEXT:     fn argz_add(__argz: *mut *mut i8, __argz_len: *mut u64, __str: *const i8) -> i32;
// CHECK-NEXT:     fn argz_replace(
// CHECK-NEXT:         __argz: *mut *mut i8,
// CHECK-NEXT:         __argz_len: *mut u64,
// CHECK-NEXT:         __str: *const i8,
// CHECK-NEXT:         __with: *const i8,
// CHECK-NEXT:         __replace_count: *mut u32,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn envz_entry(__envz: *const i8, __envz_len: u64, __name: *const i8) -> *mut i8;
// CHECK-NEXT:     fn envz_get(__envz: *const i8, __envz_len: u64, __name: *const i8) -> *mut i8;
// CHECK-NEXT:     fn envz_add(
// CHECK-NEXT:         __envz: *mut *mut i8,
// CHECK-NEXT:         __envz_len: *mut u64,
// CHECK-NEXT:         __name: *const i8,
// CHECK-NEXT:         __value: *const i8,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn envz_remove(__envz: *mut *mut i8, __envz_len: *mut u64, __name: *const i8);
// CHECK-NEXT:     fn envz_strip(__envz: *mut *mut i8, __envz_len: *mut u64);
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn gnu_string_extensions() -> i32 {
// CHECK-NEXT:     let mut destination: __SlateAlign16<[i8; 16]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut repeated: [i8; 5] = [97, 98, 99, 97, 0];
// CHECK-NEXT:     let mut obscured: [i8; 4] = [103, 110, 117, 0];
// CHECK-NEXT:     let mut tokens: [i8; 6] = [97, 58, 58, 98, 99, 0];
// CHECK-NEXT:     let mut cursor: *mut i8 = tokens.as_mut_ptr() as *mut i8;
// CHECK-NEXT:     let end: *mut i8 = (unsafe {
// CHECK-NEXT:         mempcpy(
// CHECK-NEXT:             destination.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             b"slate\0".as_ptr() as *const (),
// CHECK-NEXT:             6 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) as *mut i8;
// CHECK-NEXT:     let mut token_score: i32 = 0;
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: *mut i8 = unsafe {
// CHECK-NEXT:             strsep(std::ptr::addr_of_mut!(cursor), c":".as_ptr())
// CHECK-NEXT:         };
// CHECK-NEXT:         let token: *mut i8 = [[V_0]];
// CHECK-NEXT:         if !([[V_0]] != std::ptr::null_mut::<i8>()) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         token_score = token_score * 10
// CHECK-NEXT:             + (((unsafe { strlen(token as *const i8) }) as u32) as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { memfrob(obscured.as_mut_ptr() as *mut (), 3 as u64) };
// CHECK-NEXT:     unsafe { memfrob(obscured.as_mut_ptr() as *mut (), 3 as u64) };
// CHECK-NEXT:     ((((((unsafe { end.offset_from(destination.0.as_mut_ptr() as *mut i8) }) as i64)
// CHECK-NEXT:         as i32)
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             strcmp(destination.0.as_mut_ptr() as *const i8, c"slate".as_ptr())
// CHECK-NEXT:         }) == 0) as i32)) as i64)
// CHECK-NEXT:         + ((unsafe {
// CHECK-NEXT:             ((unsafe { memrchr(repeated.as_ptr() as *const (), 97 as i32, 4 as u64) })
// CHECK-NEXT:                 as *const i8)
// CHECK-NEXT:                 .offset_from(repeated.as_ptr() as *const i8)
// CHECK-NEXT:         }) as i64)
// CHECK-NEXT:         + ((unsafe {
// CHECK-NEXT:             ((unsafe { rawmemchr(repeated.as_ptr() as *const (), 99 as i32) })
// CHECK-NEXT:                 as *const i8)
// CHECK-NEXT:                 .offset_from(repeated.as_ptr() as *const i8)
// CHECK-NEXT:         }) as i64)
// CHECK-NEXT:         + ((unsafe {
// CHECK-NEXT:             unsafe { strchrnul(c"abc".as_ptr(), 122 as i32) }
// CHECK-NEXT:                 .offset_from(c"abc".as_ptr() as *mut i8)
// CHECK-NEXT:         }) as i64)
// CHECK-NEXT:         + ((((unsafe { strcasestr(c"GNU Library".as_ptr(), c"library".as_ptr()) })
// CHECK-NEXT:             != std::ptr::null_mut::<i8>()) as i32) as i64)
// CHECK-NEXT:         + ((((unsafe { strverscmp(c"release-2".as_ptr(), c"release-10".as_ptr()) }) < 0)
// CHECK-NEXT:             as i32) as i64)
// CHECK-NEXT:         + ((((unsafe { strcmp(obscured.as_mut_ptr() as *const i8, c"gnu".as_ptr()) })
// CHECK-NEXT:             == 0) as i32) as i64) + (token_score as i64)
// CHECK-NEXT:         + ((((unsafe {
// CHECK-NEXT:             strcmp(unsafe { strerrorname_np(22 as i32) }, c"EINVAL".as_ptr())
// CHECK-NEXT:         }) == 0) as i32) as i64)
// CHECK-NEXT:         + ((((unsafe { strerrordesc_np(22 as i32) }) != std::ptr::null::<i8>()) as i32)
// CHECK-NEXT:             as i64)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_argz_extensions() -> i32 {
// CHECK-NEXT:     let mut argz: *mut i8 = std::ptr::null_mut::<i8>();
// CHECK-NEXT:     let mut length: u64 = 0;
// CHECK-NEXT:     let mut arguments: __SlateAlign16<[*mut i8; 6]> = __SlateAlign16([0 as *mut i8; 6]);
// CHECK-NEXT:     let mut replacements: u32 = 0;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_1]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             argz_create_sep(
// CHECK-NEXT:                 c"one:two:three".as_ptr(),
// CHECK-NEXT:                 58 as i32,
// CHECK-NEXT:                 std::ptr::addr_of_mut!(argz),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(length),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == 0) as i32);
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_2]] + (((unsafe { argz_count(argz as *const i8, length) }) == 3) as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         argz_extract(argz as *const i8, length, arguments.0.as_mut_ptr() as *mut *mut i8)
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_3]]
// CHECK-NEXT:         + (((unsafe { strcmp(arguments.0[1] as *const i8, c"two".as_ptr()) }) == 0)
// CHECK-NEXT:             as i32);
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_4]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             argz_add(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(argz),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(length),
// CHECK-NEXT:                 c"four".as_ptr(),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == 0) as i32);
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_5]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             argz_replace(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(argz),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(length),
// CHECK-NEXT:                 c"three".as_ptr(),
// CHECK-NEXT:                 c"THREE".as_ptr(),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(replacements),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == 0) as i32);
// CHECK-NEXT:     total += (replacements == 1) as i32;
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_6]] + (((unsafe { argz_count(argz as *const i8, length) }) == 4) as i32);
// CHECK-NEXT:     unsafe { argz_stringify(argz, length, 44 as i32) };
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_7]]
// CHECK-NEXT:         + (((unsafe { strcmp(argz as *const i8, c"one,two,THREE,four".as_ptr()) }) == 0)
// CHECK-NEXT:             as i32);
// CHECK-NEXT:     unsafe { free(argz as *mut ()) };
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_envz_extensions() -> i32 {
// CHECK-NEXT:     let mut envz: *mut i8 = std::ptr::null_mut::<i8>();
// CHECK-NEXT:     let mut length: u64 = 0;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let [[V_8:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_8]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             envz_add(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(envz),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(length),
// CHECK-NEXT:                 c"ALPHA".as_ptr(),
// CHECK-NEXT:                 c"one".as_ptr(),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == 0) as i32);
// CHECK-NEXT:     let [[V_9:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_9]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             envz_add(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(envz),
// CHECK-NEXT:                 std::ptr::addr_of_mut!(length),
// CHECK-NEXT:                 c"BETA".as_ptr(),
// CHECK-NEXT:                 std::ptr::null::<i8>(),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == 0) as i32);
// CHECK-NEXT:     let [[V_10:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_10]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 (unsafe { envz_get(envz as *const i8, length, c"ALPHA".as_ptr()) })
// CHECK-NEXT:                     as *const i8,
// CHECK-NEXT:                 c"one".as_ptr(),
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == 0) as i32);
// CHECK-NEXT:     let [[V_11:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_11]]
// CHECK-NEXT:         + (((unsafe { envz_entry(envz as *const i8, length, c"BETA".as_ptr()) })
// CHECK-NEXT:             != std::ptr::null_mut::<i8>()) as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         envz_remove(
// CHECK-NEXT:             std::ptr::addr_of_mut!(envz),
// CHECK-NEXT:             std::ptr::addr_of_mut!(length),
// CHECK-NEXT:             c"ALPHA".as_ptr(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_12:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_12]]
// CHECK-NEXT:         + (((unsafe { envz_get(envz as *const i8, length, c"ALPHA".as_ptr()) })
// CHECK-NEXT:             == std::ptr::null_mut::<i8>()) as i32);
// CHECK-NEXT:     unsafe { envz_strip(std::ptr::addr_of_mut!(envz), std::ptr::addr_of_mut!(length)) };
// CHECK-NEXT:     total += (length == 0) as i32;
// CHECK-NEXT:     unsafe { free(envz as *mut ()) };
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", gnu_string_extensions(), gnu_argz_extensions(), gnu_envz_extensions()
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
