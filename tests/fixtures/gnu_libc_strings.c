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
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: struct __SlateAllocaFrame0(i32, *mut i8, *mut i8, *mut i8, [i8; 6], [i8; 4], [i8; 5], aligned::Aligned<aligned::A16, [i8; 16]>, i32);
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn mempcpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn strsep(_0: *mut *mut i8, _1: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn strlen(_0: *const i8) -> usize;
// LOWERING-NEXT:     fn memfrob(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn strcmp(_0: *const i8, _1: *const i8) -> i32;
// LOWERING-NEXT:     fn memrchr(_0: *const core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn rawmemchr(_0: *const core::ffi::c_void, _1: i32) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn strchrnul(_0: *const i8, _1: i32) -> *mut i8;
// LOWERING-NEXT:     fn strcasestr(_0: *const i8, _1: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn strverscmp(_0: *const i8, _1: *const i8) -> i32;
// LOWERING-NEXT:     fn strerrorname_np(_0: i32) -> *const i8;
// LOWERING-NEXT:     fn strerrordesc_np(_0: i32) -> *const i8;
// LOWERING-NEXT:     fn argz_create_sep(_0: *const i8, _1: i32, _2: *mut *mut i8, _3: *mut usize) -> i32;
// LOWERING-NEXT:     fn argz_count(_0: *const i8, _1: usize) -> usize;
// LOWERING-NEXT:     fn argz_extract(_0: *const i8, _1: usize, _2: *mut *mut i8);
// LOWERING-NEXT:     fn argz_add(_0: *mut *mut i8, _1: *mut usize, _2: *const i8) -> i32;
// LOWERING-NEXT:     fn argz_replace(_0: *mut *mut i8, _1: *mut usize, _2: *const i8, _3: *const i8, _4: *mut u32) -> i32;
// LOWERING-NEXT:     fn argz_stringify(_0: *mut i8, _1: usize, _2: i32);
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn envz_add(_0: *mut *mut i8, _1: *mut usize, _2: *const i8, _3: *const i8) -> i32;
// LOWERING-NEXT:     fn envz_get(_0: *const i8, _1: usize, _2: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn envz_entry(_0: *const i8, _1: usize, _2: *const i8) -> *mut i8;
// LOWERING-NEXT:     fn envz_remove(_0: *mut *mut i8, _1: *mut usize, _2: *const i8);
// LOWERING-NEXT:     fn envz_strip(_0: *mut *mut i8, _1: *mut usize);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_string_extensions() -> i32 {
// LOWERING-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(0, std::ptr::null_mut(), std::ptr::null_mut(), std::ptr::null_mut(), [0; 6], [0; 4], [0; 5], aligned::Aligned([0; 16]), 0);
// LOWERING-NEXT:     *{{__slate_alloca_frame[0-9]+}}.7 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.6 = [97, 98, 99, 97, 0];
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.5 = [103, 110, 117, 0];
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.4 = [97, 58, 58, 98, 99, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.7.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { mempcpy({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.1 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.0 = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b":\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = unsafe { strsep(std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.3) as *mut *mut i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:             {{__slate_alloca_frame[0-9]+}}.2 = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.0;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.2;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = (unsafe { strlen({{_v[0-9]+}} as *const i8) }) as u64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__slate_alloca_frame[0-9]+}}.0 = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memfrob({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memfrob({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.7.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.7.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 97;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memrchr({{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 99;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { rawmemchr({{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 122;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { strchrnul({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"GNU Library\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"library\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { strcasestr({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"release-2\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"release-10\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strverscmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 22;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe { strerrorname_np({{_v[0-9]+}} as i32) }) as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"EINVAL\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 22;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe { strerrordesc_np({{_v[0-9]+}} as i32) }) as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.8 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.8;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_argz_extensions() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut argz: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut length: u64 = 0;
// LOWERING-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 6]> = aligned::Aligned([std::ptr::null_mut(); 6]);
// LOWERING-NEXT:     let mut replacements: u32 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     argz = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     length = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     replacements = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"one:two:three\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 58;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { argz_create_sep({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32, std::ptr::addr_of_mut!(argz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = argz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { argz_count({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = argz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-NEXT:     unsafe { argz_extract({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *mut *mut i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = arguments[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"two\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"four\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { argz_add(std::ptr::addr_of_mut!(argz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"three\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"THREE\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { argz_replace(std::ptr::addr_of_mut!(argz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8, std::ptr::addr_of_mut!(replacements) as *mut u32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = replacements;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = argz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { argz_count({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = argz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 44;
// LOWERING-NEXT:     unsafe { argz_stringify({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = argz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"one,two,THREE,four\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = argz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_envz_extensions() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut envz: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut length: u64 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     envz = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     length = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"one\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { envz_add(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"BETA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { envz_add(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = envz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { envz_get({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"one\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = envz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"BETA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { envz_entry({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe { envz_remove(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = envz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { envz_get({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe { envz_strip(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = envz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_string_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_argz_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_envz_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: struct __SlateAllocaFrame0(i32, *mut i8, *mut i8, *mut i8, [i8; 6], [i8; 4], [i8; 5], aligned::Aligned<aligned::A16, [i8; 16]>, i32);
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn mempcpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn strsep(_0: *mut *mut i8, _1: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn strlen(_0: *const i8) -> usize;
// REWRITES-NEXT:     fn memfrob(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn strcmp(_0: *const i8, _1: *const i8) -> i32;
// REWRITES-NEXT:     fn memrchr(_0: *const core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn rawmemchr(_0: *const core::ffi::c_void, _1: i32) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn strchrnul(_0: *const i8, _1: i32) -> *mut i8;
// REWRITES-NEXT:     fn strcasestr(_0: *const i8, _1: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn strverscmp(_0: *const i8, _1: *const i8) -> i32;
// REWRITES-NEXT:     fn strerrorname_np(_0: i32) -> *const i8;
// REWRITES-NEXT:     fn strerrordesc_np(_0: i32) -> *const i8;
// REWRITES-NEXT:     fn argz_create_sep(_0: *const i8, _1: i32, _2: *mut *mut i8, _3: *mut usize) -> i32;
// REWRITES-NEXT:     fn argz_count(_0: *const i8, _1: usize) -> usize;
// REWRITES-NEXT:     fn argz_extract(_0: *const i8, _1: usize, _2: *mut *mut i8);
// REWRITES-NEXT:     fn argz_add(_0: *mut *mut i8, _1: *mut usize, _2: *const i8) -> i32;
// REWRITES-NEXT:     fn argz_replace(_0: *mut *mut i8, _1: *mut usize, _2: *const i8, _3: *const i8, _4: *mut u32) -> i32;
// REWRITES-NEXT:     fn argz_stringify(_0: *mut i8, _1: usize, _2: i32);
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn envz_add(_0: *mut *mut i8, _1: *mut usize, _2: *const i8, _3: *const i8) -> i32;
// REWRITES-NEXT:     fn envz_get(_0: *const i8, _1: usize, _2: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn envz_entry(_0: *const i8, _1: usize, _2: *const i8) -> *mut i8;
// REWRITES-NEXT:     fn envz_remove(_0: *mut *mut i8, _1: *mut usize, _2: *const i8);
// REWRITES-NEXT:     fn envz_strip(_0: *mut *mut i8, _1: *mut usize);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_string_extensions() -> i32 {
// REWRITES-NEXT: let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(0, std::ptr::null_mut(), std::ptr::null_mut(), std::ptr::null_mut(), [0; 6], [0; 4], [0; 5], aligned::Aligned([0; 16]), 0);
// REWRITES-NEXT: *{{__slate_alloca_frame[0-9]+}}.7 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.6 = [97, 98, 99, 97, 0];
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.5 = [103, 110, 117, 0];
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.4 = [97, 58, 58, 98, 99, 0];
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.3 = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.7.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 6;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { mempcpy({{_v[0-9]+}} as *mut core::ffi::c_void, ({{_v[0-9]+}} as *mut core::ffi::c_void) as *const core::ffi::c_void, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.1 = {{_v[0-9]+}} as *mut i8;
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.0 = 0;
// REWRITES-NEXT: loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = b":\0".as_ptr() as *mut i8;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = unsafe { strsep(std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.3) as *mut *mut i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT:         {{__slate_alloca_frame[0-9]+}}.2 = {{_v[0-9]+}};
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:         if !({{_v[0-9]+}} != {{_v[0-9]+}}) {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = 10;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.0 * {{_v[0-9]+}};
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.2;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { strlen({{_v[0-9]+}} as *const i8) }) as u64;
// REWRITES-NEXT:                     {{__slate_alloca_frame[0-9]+}}.0 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memfrob(({{_v[0-9]+}} as *mut core::ffi::c_void) as *mut core::ffi::c_void, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memfrob(({{_v[0-9]+}} as *mut core::ffi::c_void) as *mut core::ffi::c_void, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.7.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.7.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = ({{_v[0-9]+}} + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32)) as i64;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 97;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memrchr(({{_v[0-9]+}} as *mut core::ffi::c_void) as *const core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 99;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { rawmemchr(({{_v[0-9]+}} as *mut core::ffi::c_void) as *const core::ffi::c_void, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 122;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { strchrnul({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"GNU Library\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"library\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { strcasestr({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + ((({{_v[0-9]+}} != {{_v[0-9]+}}) as i32) as i64);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"release-2\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"release-10\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strverscmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + ((({{_v[0-9]+}} < {{_v[0-9]+}}) as i32) as i64);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + ((({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) as i64) + ({{__slate_alloca_frame[0-9]+}}.0 as i64);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 22;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = (unsafe { strerrorname_np({{_v[0-9]+}} as i32) }) as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"EINVAL\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + ((({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) as i64);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 22;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = (unsafe { strerrordesc_np({{_v[0-9]+}} as i32) }) as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.8 = ({{_v[0-9]+}} + ((({{_v[0-9]+}} != {{_v[0-9]+}}) as i32) as i64)) as i32;
// REWRITES-NEXT: return {{__slate_alloca_frame[0-9]+}}.8;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_argz_extensions() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut argz: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut length: u64 = 0;
// REWRITES-NEXT: let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 6]> = aligned::Aligned([std::ptr::null_mut(); 6]);
// REWRITES-NEXT: let mut replacements: u32 = 0;
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: argz = std::ptr::null_mut();
// REWRITES-NEXT: length = 0;
// REWRITES-NEXT: replacements = 0;
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"one:two:three\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 58;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { argz_create_sep({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32, std::ptr::addr_of_mut!(argz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: total = total + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = (unsafe { argz_count(argz as *const i8, length as usize) }) as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 3;
// REWRITES-NEXT: total = total + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = argz;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = length;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// REWRITES-NEXT: unsafe { argz_extract({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *mut *mut i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = arguments[({{_v[0-9]+}} as usize)];
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"two\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: total = total + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"four\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { argz_add(std::ptr::addr_of_mut!(argz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: total = total + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"three\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"THREE\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { argz_replace(std::ptr::addr_of_mut!(argz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8, std::ptr::addr_of_mut!(replacements) as *mut u32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: total = total + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 1;
// REWRITES-NEXT: total = total + ((replacements == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = (unsafe { argz_count(argz as *const i8, length as usize) }) as u64;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT: total = total + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 44;
// REWRITES-NEXT: unsafe { argz_stringify(argz as *mut i8, length as usize, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = argz;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"one,two,THREE,four\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: total = total + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: unsafe { free((argz as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_envz_extensions() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut envz: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let mut length: u64 = 0;
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: envz = std::ptr::null_mut();
// REWRITES-NEXT: length = 0;
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"one\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { envz_add(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: total = total + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"BETA\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { envz_add(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, {{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: total = total + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = envz;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = length;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { envz_get({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"one\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: total = total + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = envz;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = length;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"BETA\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { envz_entry({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: total = total + (({{_v[0-9]+}} != {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { envz_remove(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = envz;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = length;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { envz_get({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: total = total + (({{_v[0-9]+}} == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: unsafe { envz_strip(std::ptr::addr_of_mut!(envz) as *mut *mut i8, std::ptr::addr_of_mut!(length) as *mut usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 0;
// REWRITES-NEXT: total = total + ((length == {{_v[0-9]+}}) as i32);
// REWRITES-NEXT: unsafe { free((envz as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: __retval = total;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = gnu_string_extensions();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = gnu_argz_extensions();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = gnu_envz_extensions();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
