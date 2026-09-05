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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn mempcpy(
// COMMON-LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _2: usize,
// COMMON-LOWERING-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn strsep(
// COMMON-LOWERING-NEXT:         _0: *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:     ) -> *mut core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// COMMON-LOWERING-NEXT:     fn memfrob(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-LOWERING-NEXT:     fn memrchr(_0: *const core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn rawmemchr(_0: *const core::ffi::c_void, _1: i32) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn strchrnul(_0: *const core::ffi::c_char, _1: i32) -> *mut core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn strcasestr(
// COMMON-LOWERING-NEXT:         _0: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:     ) -> *mut core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn strverscmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-LOWERING-NEXT:     fn strerrorname_np(_0: i32) -> *const core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn strerrordesc_np(_0: i32) -> *const core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn argz_create_sep(
// COMMON-LOWERING-NEXT:         _0: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: i32,
// COMMON-LOWERING-NEXT:         _2: *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _3: *mut usize,
// COMMON-LOWERING-NEXT:     ) -> i32;
// COMMON-LOWERING-NEXT:     fn argz_count(_0: *const core::ffi::c_char, _1: usize) -> usize;
// COMMON-LOWERING-NEXT:     fn argz_extract(_0: *const core::ffi::c_char, _1: usize, _2: *mut *mut core::ffi::c_char);
// COMMON-LOWERING-NEXT:     fn argz_add(
// COMMON-LOWERING-NEXT:         _0: *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: *mut usize,
// COMMON-LOWERING-NEXT:         _2: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:     ) -> i32;
// COMMON-LOWERING-NEXT:     fn argz_replace(
// COMMON-LOWERING-NEXT:         _0: *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: *mut usize,
// COMMON-LOWERING-NEXT:         _2: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _3: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _4: *mut u32,
// COMMON-LOWERING-NEXT:     ) -> i32;
// COMMON-LOWERING-NEXT:     fn argz_stringify(_0: *mut core::ffi::c_char, _1: usize, _2: i32);
// COMMON-LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT:     fn envz_add(
// COMMON-LOWERING-NEXT:         _0: *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: *mut usize,
// COMMON-LOWERING-NEXT:         _2: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _3: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:     ) -> i32;
// COMMON-LOWERING-NEXT:     fn envz_get(
// COMMON-LOWERING-NEXT:         _0: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: usize,
// COMMON-LOWERING-NEXT:         _2: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:     ) -> *mut core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn envz_entry(
// COMMON-LOWERING-NEXT:         _0: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: usize,
// COMMON-LOWERING-NEXT:         _2: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:     ) -> *mut core::ffi::c_char;
// COMMON-LOWERING-NEXT:     fn envz_remove(_0: *mut *mut core::ffi::c_char, _1: *mut usize, _2: *const core::ffi::c_char);
// COMMON-LOWERING-NEXT:     fn envz_strip(_0: *mut *mut core::ffi::c_char, _1: *mut usize);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_string_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_argz_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_envz_extensions();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_string_extensions() -> i32 {
// COMMON-LOWERING-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(
// COMMON-LOWERING-NEXT:         0,
// COMMON-LOWERING-NEXT:         std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         [0; 6],
// COMMON-LOWERING-NEXT:         [0; 4],
// COMMON-LOWERING-NEXT:         [0; 5],
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.5 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.4 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 6;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-NEXT:         mempcpy(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.0 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:                 strsep(
// COMMON-LOWERING-NEXT:                     std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.2) as *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:                     {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:                 )
// COMMON-LOWERING-NEXT:             {{__slate_alloca_frame[0-9]+}}.1 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.0;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 {{__slate_alloca_frame[0-9]+}}.0 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         unsafe { memfrob({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         unsafe { memfrob({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 97;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-NEXT:         memrchr(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 99;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:         unsafe { rawmemchr({{__v[0-9]+}} as *const core::ffi::c_void, {{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 122;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         strcasestr(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strverscmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 22;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 22;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_argz_extensions() -> i32 {
// COMMON-LOWERING-NEXT:     let mut length: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut replacements: u32 = 0;
// COMMON-LOWERING-NEXT:     argz = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     length = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     replacements = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 58;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         argz_create_sep(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(argz) as *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = length;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 =
// COMMON-LOWERING-NEXT:         (unsafe { argz_count({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as usize) }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = length;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         argz_extract(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         argz_add(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(argz) as *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         argz_replace(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(argz) as *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(replacements) as *mut u32,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = replacements;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = length;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 =
// COMMON-LOWERING-NEXT:         (unsafe { argz_count({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as usize) }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = length;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 44;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         argz_stringify(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_envz_extensions() -> i32 {
// COMMON-LOWERING-NEXT:     let mut length: u64 = 0;
// COMMON-LOWERING-NEXT:     envz = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     length = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         envz_add(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         envz_add(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = length;
// COMMON-LOWERING-NEXT:         envz_get(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = length;
// COMMON-LOWERING-NEXT:         envz_entry(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         envz_remove(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = length;
// COMMON-LOWERING-NEXT:         envz_get(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         envz_strip(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = length;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: struct __SlateAllocaFrame0(
// LOWERING-X86_64-GNU-NEXT:     i32,
// LOWERING-X86_64-GNU-NEXT:     *mut i8,
// LOWERING-X86_64-GNU-NEXT:     *mut i8,
// LOWERING-X86_64-GNU-NEXT:     [i8; 6],
// LOWERING-X86_64-GNU-NEXT:     [i8; 4],
// LOWERING-X86_64-GNU-NEXT:     [i8; 5],
// LOWERING-X86_64-GNU-NEXT:     aligned::Aligned<aligned::A16, [i8; 16]>,
// LOWERING-X86_64-GNU-NEXT: );
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         aligned::Aligned([0; 16]),
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = [0; 16];
// LOWERING-X86_64-GNU-NEXT:     *{{__slate_alloca_frame[0-9]+}}.6 = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 5] = [97, 98, 99, 97, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [103, 110, 117, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 6] = [97, 58, 58, 98, 99, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b":\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-X86_64-GNU-NEXT:             }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:         (unsafe { strchrnul({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"GNU Library\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"library\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-X86_64-GNU-NEXT:     }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"release-2\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"release-10\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { strerrorname_np({{__v[0-9]+}} as i32) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"EINVAL\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { strerrordesc_np({{__v[0-9]+}} as i32) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut argz: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 6]> =
// LOWERING-X86_64-GNU-NEXT:         aligned::Aligned([std::ptr::null_mut(); 6]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"one:two:three\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = argz;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = argz;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = arguments[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"two\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"four\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"three\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"THREE\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = argz;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = argz;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = argz;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"one,two,THREE,four\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = argz;
// LOWERING-X86_64-GNU-NEXT:     let mut envz: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"one\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"BETA\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = envz;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-X86_64-GNU-NEXT:     }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"one\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = envz;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"BETA\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-X86_64-GNU-NEXT:     }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = envz;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-X86_64-GNU-NEXT:     }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = envz;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT: struct __SlateAllocaFrame0(i32, *mut u8, *mut u8, [u8; 6], [u8; 4], [u8; 5], [u8; 16]);
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         [0; 16],
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 16] = [0; 16];
// LOWERING-AARCH64-GNU-NEXT:     {{__slate_alloca_frame[0-9]+}}.6 = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 5] = [97, 98, 99, 97, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [103, 110, 117, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 6] = [97, 58, 58, 98, 99, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b":\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = (unsafe {
// LOWERING-AARCH64-GNU-NEXT:             }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"abc\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// LOWERING-AARCH64-GNU-NEXT:         (unsafe { strchrnul({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}} as i32) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"abc\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"GNU Library\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"library\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// LOWERING-AARCH64-GNU-NEXT:     }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"release-2\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"release-10\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"gnu\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { strerrorname_np({{__v[0-9]+}} as i32) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"EINVAL\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { strerrordesc_np({{__v[0-9]+}} as i32) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut argz: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut arguments: [*mut u8; 6] = [std::ptr::null_mut(); 6];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"one:two:three\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = argz;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = argz;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut *mut u8 = arguments.as_mut_ptr() as *mut *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = arguments[({{__v[0-9]+}} as usize)];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"two\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"four\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"three\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"THREE\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = argz;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = argz;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = argz;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"one,two,THREE,four\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = argz;
// LOWERING-AARCH64-GNU-NEXT:     let mut envz: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"ALPHA\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"one\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"BETA\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = envz;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"ALPHA\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// LOWERING-AARCH64-GNU-NEXT:     }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"one\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = envz;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"BETA\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// LOWERING-AARCH64-GNU-NEXT:     }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"ALPHA\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = envz;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"ALPHA\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// LOWERING-AARCH64-GNU-NEXT:     }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = envz;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn mempcpy(
// COMMON-REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _2: usize,
// COMMON-REWRITES-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn strsep(
// COMMON-REWRITES-NEXT:         _0: *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:     ) -> *mut core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// COMMON-REWRITES-NEXT:     fn memfrob(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-REWRITES-NEXT:     fn memrchr(_0: *const core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn rawmemchr(_0: *const core::ffi::c_void, _1: i32) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn strchrnul(_0: *const core::ffi::c_char, _1: i32) -> *mut core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn strcasestr(
// COMMON-REWRITES-NEXT:         _0: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:     ) -> *mut core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn strverscmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-REWRITES-NEXT:     fn strerrorname_np(_0: i32) -> *const core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn strerrordesc_np(_0: i32) -> *const core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn argz_create_sep(
// COMMON-REWRITES-NEXT:         _0: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: i32,
// COMMON-REWRITES-NEXT:         _2: *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _3: *mut usize,
// COMMON-REWRITES-NEXT:     ) -> i32;
// COMMON-REWRITES-NEXT:     fn argz_count(_0: *const core::ffi::c_char, _1: usize) -> usize;
// COMMON-REWRITES-NEXT:     fn argz_extract(_0: *const core::ffi::c_char, _1: usize, _2: *mut *mut core::ffi::c_char);
// COMMON-REWRITES-NEXT:     fn argz_add(
// COMMON-REWRITES-NEXT:         _0: *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: *mut usize,
// COMMON-REWRITES-NEXT:         _2: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:     ) -> i32;
// COMMON-REWRITES-NEXT:     fn argz_replace(
// COMMON-REWRITES-NEXT:         _0: *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: *mut usize,
// COMMON-REWRITES-NEXT:         _2: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _3: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _4: *mut u32,
// COMMON-REWRITES-NEXT:     ) -> i32;
// COMMON-REWRITES-NEXT:     fn argz_stringify(_0: *mut core::ffi::c_char, _1: usize, _2: i32);
// COMMON-REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:     fn envz_add(
// COMMON-REWRITES-NEXT:         _0: *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: *mut usize,
// COMMON-REWRITES-NEXT:         _2: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _3: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:     ) -> i32;
// COMMON-REWRITES-NEXT:     fn envz_get(
// COMMON-REWRITES-NEXT:         _0: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: usize,
// COMMON-REWRITES-NEXT:         _2: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:     ) -> *mut core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn envz_entry(
// COMMON-REWRITES-NEXT:         _0: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: usize,
// COMMON-REWRITES-NEXT:         _2: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:     ) -> *mut core::ffi::c_char;
// COMMON-REWRITES-NEXT:     fn envz_remove(_0: *mut *mut core::ffi::c_char, _1: *mut usize, _2: *const core::ffi::c_char);
// COMMON-REWRITES-NEXT:     fn envz_strip(_0: *mut *mut core::ffi::c_char, _1: *mut usize);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             gnu_string_extensions(),
// COMMON-REWRITES-NEXT:             gnu_argz_extensions(),
// COMMON-REWRITES-NEXT:             gnu_envz_extensions(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_string_extensions() -> i32 {
// COMMON-REWRITES-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(
// COMMON-REWRITES-NEXT:         0,
// COMMON-REWRITES-NEXT:         std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         [0; 6],
// COMMON-REWRITES-NEXT:         [0; 4],
// COMMON-REWRITES-NEXT:         [0; 5],
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.5 = [97, 98, 99, 97, 0];
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.4 = [103, 110, 117, 0];
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = [97, 58, 58, 98, 99, 0];
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.2 = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-REWRITES-NEXT:         mempcpy(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             b"slate\0".as_ptr() as *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:             (6 as u64) as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.0 = 0;
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:             strsep(
// COMMON-REWRITES-NEXT:                 std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.2) as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:                 c":".as_ptr(),
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         {{__slate_alloca_frame[0-9]+}}.1 = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != std::ptr::null_mut()) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.0 * 10;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// COMMON-REWRITES-NEXT:         {{__slate_alloca_frame[0-9]+}}.0 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         memfrob(
// COMMON-REWRITES-NEXT:             {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             (3 as u64) as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         memfrob(
// COMMON-REWRITES-NEXT:             {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             (3 as u64) as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"slate".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = ({{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32)) as i64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-REWRITES-NEXT:         memrchr(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:             97 as i32,
// COMMON-REWRITES-NEXT:             (4 as u64) as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-REWRITES-NEXT:         unsafe { rawmemchr({{__v[0-9]+}} as *const core::ffi::c_void, 99 as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + ((({{__v[0-9]+}} != std::ptr::null_mut()) as i32) as i64);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strverscmp(c"release-2".as_ptr(), c"release-10".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + ((({{__v[0-9]+}} < 0) as i32) as i64);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"gnu".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + ((({{__v[0-9]+}} == 0) as i32) as i64) + ({{__slate_alloca_frame[0-9]+}}.0 as i64);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"EINVAL".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} + ((({{__v[0-9]+}} == 0) as i32) as i64);
// COMMON-REWRITES-NEXT:     ({{__v[0-9]+}} + ((({{__v[0-9]+}} != std::ptr::null_mut()) as i32) as i64)) as i32
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_argz_extensions() -> i32 {
// COMMON-REWRITES-NEXT:     let mut length: u64 = 0;
// COMMON-REWRITES-NEXT:     let mut replacements: u32 = 0;
// COMMON-REWRITES-NEXT:     replacements = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         argz_create_sep(
// COMMON-REWRITES-NEXT:             c"one:two:three".as_ptr(),
// COMMON-REWRITES-NEXT:             58 as i32,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(argz) as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 =
// COMMON-REWRITES-NEXT:         (unsafe { argz_count(argz as *const core::ffi::c_char, length as usize) }) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 3) as i32);
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         argz_extract(
// COMMON-REWRITES-NEXT:             argz as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             length as usize,
// COMMON-REWRITES-NEXT:             arguments.as_mut_ptr() as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp(arguments[1] as *const core::ffi::c_char, c"two".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         argz_add(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(argz) as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// COMMON-REWRITES-NEXT:             c"four".as_ptr(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         argz_replace(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(argz) as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// COMMON-REWRITES-NEXT:             c"three".as_ptr(),
// COMMON-REWRITES-NEXT:             c"THREE".as_ptr(),
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(replacements) as *mut u32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32) + ((replacements == 1) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 =
// COMMON-REWRITES-NEXT:         (unsafe { argz_count(argz as *const core::ffi::c_char, length as usize) }) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 4) as i32);
// COMMON-REWRITES-NEXT:     unsafe { argz_stringify(argz as *mut core::ffi::c_char, length as usize, 44 as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         strcmp(
// COMMON-REWRITES-NEXT:             argz as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             c"one,two,THREE,four".as_ptr(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     unsafe { free(argz as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_envz_extensions() -> i32 {
// COMMON-REWRITES-NEXT:     let mut length: u64 = 0;
// COMMON-REWRITES-NEXT:     envz = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     length = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         envz_add(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// COMMON-REWRITES-NEXT:             c"ALPHA".as_ptr(),
// COMMON-REWRITES-NEXT:             c"one".as_ptr(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         envz_add(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// COMMON-REWRITES-NEXT:             c"BETA".as_ptr(),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:         envz_get(
// COMMON-REWRITES-NEXT:             envz as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             length as usize,
// COMMON-REWRITES-NEXT:             c"ALPHA".as_ptr(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp({{__v[0-9]+}} as *const core::ffi::c_char, c"one".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:         envz_entry(
// COMMON-REWRITES-NEXT:             envz as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             length as usize,
// COMMON-REWRITES-NEXT:             c"BETA".as_ptr(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32);
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         envz_remove(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// COMMON-REWRITES-NEXT:             c"ALPHA".as_ptr(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:         envz_get(
// COMMON-REWRITES-NEXT:             envz as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             length as usize,
// COMMON-REWRITES-NEXT:             c"ALPHA".as_ptr(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == std::ptr::null_mut()) as i32);
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         envz_strip(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ((length == 0) as i32);
// COMMON-REWRITES-NEXT:     unsafe { free(envz as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: struct __SlateAllocaFrame0(
// REWRITES-X86_64-GNU-NEXT:     i32,
// REWRITES-X86_64-GNU-NEXT:     *mut i8,
// REWRITES-X86_64-GNU-NEXT:     *mut i8,
// REWRITES-X86_64-GNU-NEXT:     [i8; 6],
// REWRITES-X86_64-GNU-NEXT:     [i8; 4],
// REWRITES-X86_64-GNU-NEXT:     [i8; 5],
// REWRITES-X86_64-GNU-NEXT:     aligned::Aligned<aligned::A16, [i8; 16]>,
// REWRITES-X86_64-GNU-NEXT: );
// REWRITES-X86_64-GNU-NEXT:         aligned::Aligned([0; 16]),
// REWRITES-X86_64-GNU-NEXT:     *{{__slate_alloca_frame[0-9]+}}.6 = [0; 16];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = (unsafe {
// REWRITES-X86_64-GNU-NEXT:         }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { strchrnul(c"abc".as_ptr(), 122 as i32) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from(c"abc".as_ptr() as *mut i8) as i64 };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-X86_64-GNU-NEXT:         (unsafe { strcasestr(c"GNU Library".as_ptr(), c"library".as_ptr()) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { strerrorname_np(22 as i32) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { strerrordesc_np(22 as i32) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let mut argz: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 6]> =
// REWRITES-X86_64-GNU-NEXT:         aligned::Aligned([std::ptr::null_mut(); 6]);
// REWRITES-X86_64-GNU-NEXT:     let mut envz: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// REWRITES-X86_64-GNU-NEXT:     }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// REWRITES-X86_64-GNU-NEXT:     }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe {
// REWRITES-X86_64-GNU-NEXT:     }) as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: struct __SlateAllocaFrame0(i32, *mut u8, *mut u8, [u8; 6], [u8; 4], [u8; 5], [u8; 16]);
// REWRITES-AARCH64-GNU-NEXT:         [0; 16],
// REWRITES-AARCH64-GNU-NEXT:     {{__slate_alloca_frame[0-9]+}}.6 = [0; 16];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = (unsafe {
// REWRITES-AARCH64-GNU-NEXT:         }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.1;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { strchrnul(c"abc".as_ptr(), 122 as i32) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from(c"abc".as_ptr() as *mut u8) as i64 };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 =
// REWRITES-AARCH64-GNU-NEXT:         (unsafe { strcasestr(c"GNU Library".as_ptr(), c"library".as_ptr()) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { strerrorname_np(22 as i32) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { strerrordesc_np(22 as i32) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let mut argz: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let mut arguments: [*mut u8; 6] = [std::ptr::null_mut(); 6];
// REWRITES-AARCH64-GNU-NEXT:     let mut envz: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// REWRITES-AARCH64-GNU-NEXT:     }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// REWRITES-AARCH64-GNU-NEXT:     }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe {
// REWRITES-AARCH64-GNU-NEXT:     }) as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
