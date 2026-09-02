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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: struct __SlateAllocaFrame0(
// LOWERING-NEXT:     i32,
// LOWERING-NEXT:     *mut i8,
// LOWERING-NEXT:     *mut i8,
// LOWERING-NEXT:     [i8; 6],
// LOWERING-NEXT:     [i8; 4],
// LOWERING-NEXT:     [i8; 5],
// LOWERING-NEXT:     aligned::Aligned<aligned::A16, [i8; 16]>,
// LOWERING-NEXT: );
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn mempcpy(
// LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// LOWERING-NEXT:         _1: *const core::ffi::c_void,
// LOWERING-NEXT:         _2: usize,
// LOWERING-NEXT:     ) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn strsep(
// LOWERING-NEXT:         _0: *mut *mut core::ffi::c_char,
// LOWERING-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-NEXT:     ) -> *mut core::ffi::c_char;
// LOWERING-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// LOWERING-NEXT:     fn memfrob(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT:     fn memrchr(_0: *const core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn rawmemchr(_0: *const core::ffi::c_void, _1: i32) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn strchrnul(_0: *const core::ffi::c_char, _1: i32) -> *mut core::ffi::c_char;
// LOWERING-NEXT:     fn strcasestr(
// LOWERING-NEXT:         _0: *const core::ffi::c_char,
// LOWERING-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-NEXT:     ) -> *mut core::ffi::c_char;
// LOWERING-NEXT:     fn strverscmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT:     fn strerrorname_np(_0: i32) -> *const core::ffi::c_char;
// LOWERING-NEXT:     fn strerrordesc_np(_0: i32) -> *const core::ffi::c_char;
// LOWERING-NEXT:     fn argz_create_sep(
// LOWERING-NEXT:         _0: *const core::ffi::c_char,
// LOWERING-NEXT:         _1: i32,
// LOWERING-NEXT:         _2: *mut *mut core::ffi::c_char,
// LOWERING-NEXT:         _3: *mut usize,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT:     fn argz_count(_0: *const core::ffi::c_char, _1: usize) -> usize;
// LOWERING-NEXT:     fn argz_extract(_0: *const core::ffi::c_char, _1: usize, _2: *mut *mut core::ffi::c_char);
// LOWERING-NEXT:     fn argz_add(
// LOWERING-NEXT:         _0: *mut *mut core::ffi::c_char,
// LOWERING-NEXT:         _1: *mut usize,
// LOWERING-NEXT:         _2: *const core::ffi::c_char,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT:     fn argz_replace(
// LOWERING-NEXT:         _0: *mut *mut core::ffi::c_char,
// LOWERING-NEXT:         _1: *mut usize,
// LOWERING-NEXT:         _2: *const core::ffi::c_char,
// LOWERING-NEXT:         _3: *const core::ffi::c_char,
// LOWERING-NEXT:         _4: *mut u32,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT:     fn argz_stringify(_0: *mut core::ffi::c_char, _1: usize, _2: i32);
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn envz_add(
// LOWERING-NEXT:         _0: *mut *mut core::ffi::c_char,
// LOWERING-NEXT:         _1: *mut usize,
// LOWERING-NEXT:         _2: *const core::ffi::c_char,
// LOWERING-NEXT:         _3: *const core::ffi::c_char,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT:     fn envz_get(
// LOWERING-NEXT:         _0: *const core::ffi::c_char,
// LOWERING-NEXT:         _1: usize,
// LOWERING-NEXT:         _2: *const core::ffi::c_char,
// LOWERING-NEXT:     ) -> *mut core::ffi::c_char;
// LOWERING-NEXT:     fn envz_entry(
// LOWERING-NEXT:         _0: *const core::ffi::c_char,
// LOWERING-NEXT:         _1: usize,
// LOWERING-NEXT:         _2: *const core::ffi::c_char,
// LOWERING-NEXT:     ) -> *mut core::ffi::c_char;
// LOWERING-NEXT:     fn envz_remove(_0: *mut *mut core::ffi::c_char, _1: *mut usize, _2: *const core::ffi::c_char);
// LOWERING-NEXT:     fn envz_strip(_0: *mut *mut core::ffi::c_char, _1: *mut usize);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_string_extensions() -> i32 {
// LOWERING-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(
// LOWERING-NEXT:         0,
// LOWERING-NEXT:         std::ptr::null_mut(),
// LOWERING-NEXT:         std::ptr::null_mut(),
// LOWERING-NEXT:         [0; 6],
// LOWERING-NEXT:         [0; 4],
// LOWERING-NEXT:         [0; 5],
// LOWERING-NEXT:         aligned::Aligned([0; 16]),
// LOWERING-NEXT:     );
// LOWERING-NEXT:     *{{__slate_alloca_frame[0-9]+}}.6 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.5 = [97, 98, 99, 97, 0];
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.4 = [103, 110, 117, 0];
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = [97, 58, 58, 98, 99, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.2 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-NEXT:         mempcpy(
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.0 = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b":\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-NEXT:                 strsep(
// LOWERING-NEXT:                     std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.2) as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:                     {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:                 )
// LOWERING-NEXT:             }) as *mut i8;
// LOWERING-NEXT:             {{__slate_alloca_frame[0-9]+}}.1 = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.0;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = (unsafe { strlen({{_v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__slate_alloca_frame[0-9]+}}.0 = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         unsafe { memfrob({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         unsafe { memfrob({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 97;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         unsafe { memrchr({{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 99;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:         unsafe { rawmemchr({{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 122;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 =
// LOWERING-NEXT:         (unsafe { strchrnul({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}} as i32) }) as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"abc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"GNU Library\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"library\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-NEXT:         strcasestr(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"release-2\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"release-10\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strverscmp(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
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
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_argz_extensions() -> i32 {
// LOWERING-NEXT:     let mut argz: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut length: u64 = 0;
// LOWERING-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 6]> =
// LOWERING-NEXT:         aligned::Aligned([std::ptr::null_mut(); 6]);
// LOWERING-NEXT:     let mut replacements: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     argz = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     length = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     replacements = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"one:two:three\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 58;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         argz_create_sep(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as i32,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(argz) as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = argz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { argz_count({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}} as usize) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = argz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut *mut i8 = arguments.as_mut_ptr() as *mut *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         argz_extract(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = arguments[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"two\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"four\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         argz_add(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(argz) as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"three\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"THREE\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         argz_replace(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(argz) as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(replacements) as *mut u32,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = replacements;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = argz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { argz_count({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}} as usize) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = argz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 44;
// LOWERING-NEXT:     unsafe { argz_stringify({{_v[0-9]+}} as *mut core::ffi::c_char, {{_v[0-9]+}} as usize, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = argz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"one,two,THREE,four\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = argz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_envz_extensions() -> i32 {
// LOWERING-NEXT:     let mut envz: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut length: u64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     envz = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     length = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"one\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         envz_add(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"BETA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         envz_add(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = envz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-NEXT:         envz_get(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"one\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = envz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"BETA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-NEXT:         envz_entry(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         envz_remove(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = envz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ALPHA\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe {
// LOWERING-NEXT:         envz_get(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         envz_strip(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = length;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = envz;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_string_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_argz_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_envz_extensions();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: struct __SlateAllocaFrame0(
// REWRITES-NEXT:     i32,
// REWRITES-NEXT:     *mut i8,
// REWRITES-NEXT:     *mut i8,
// REWRITES-NEXT:     [i8; 6],
// REWRITES-NEXT:     [i8; 4],
// REWRITES-NEXT:     [i8; 5],
// REWRITES-NEXT:     aligned::Aligned<aligned::A16, [i8; 16]>,
// REWRITES-NEXT: );
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn mempcpy(
// REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// REWRITES-NEXT:         _1: *const core::ffi::c_void,
// REWRITES-NEXT:         _2: usize,
// REWRITES-NEXT:     ) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn strsep(
// REWRITES-NEXT:         _0: *mut *mut core::ffi::c_char,
// REWRITES-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-NEXT:     ) -> *mut core::ffi::c_char;
// REWRITES-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// REWRITES-NEXT:     fn memfrob(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT:     fn memrchr(_0: *const core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn rawmemchr(_0: *const core::ffi::c_void, _1: i32) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn strchrnul(_0: *const core::ffi::c_char, _1: i32) -> *mut core::ffi::c_char;
// REWRITES-NEXT:     fn strcasestr(
// REWRITES-NEXT:         _0: *const core::ffi::c_char,
// REWRITES-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-NEXT:     ) -> *mut core::ffi::c_char;
// REWRITES-NEXT:     fn strverscmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT:     fn strerrorname_np(_0: i32) -> *const core::ffi::c_char;
// REWRITES-NEXT:     fn strerrordesc_np(_0: i32) -> *const core::ffi::c_char;
// REWRITES-NEXT:     fn argz_create_sep(
// REWRITES-NEXT:         _0: *const core::ffi::c_char,
// REWRITES-NEXT:         _1: i32,
// REWRITES-NEXT:         _2: *mut *mut core::ffi::c_char,
// REWRITES-NEXT:         _3: *mut usize,
// REWRITES-NEXT:     ) -> i32;
// REWRITES-NEXT:     fn argz_count(_0: *const core::ffi::c_char, _1: usize) -> usize;
// REWRITES-NEXT:     fn argz_extract(_0: *const core::ffi::c_char, _1: usize, _2: *mut *mut core::ffi::c_char);
// REWRITES-NEXT:     fn argz_add(
// REWRITES-NEXT:         _0: *mut *mut core::ffi::c_char,
// REWRITES-NEXT:         _1: *mut usize,
// REWRITES-NEXT:         _2: *const core::ffi::c_char,
// REWRITES-NEXT:     ) -> i32;
// REWRITES-NEXT:     fn argz_replace(
// REWRITES-NEXT:         _0: *mut *mut core::ffi::c_char,
// REWRITES-NEXT:         _1: *mut usize,
// REWRITES-NEXT:         _2: *const core::ffi::c_char,
// REWRITES-NEXT:         _3: *const core::ffi::c_char,
// REWRITES-NEXT:         _4: *mut u32,
// REWRITES-NEXT:     ) -> i32;
// REWRITES-NEXT:     fn argz_stringify(_0: *mut core::ffi::c_char, _1: usize, _2: i32);
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn envz_add(
// REWRITES-NEXT:         _0: *mut *mut core::ffi::c_char,
// REWRITES-NEXT:         _1: *mut usize,
// REWRITES-NEXT:         _2: *const core::ffi::c_char,
// REWRITES-NEXT:         _3: *const core::ffi::c_char,
// REWRITES-NEXT:     ) -> i32;
// REWRITES-NEXT:     fn envz_get(
// REWRITES-NEXT:         _0: *const core::ffi::c_char,
// REWRITES-NEXT:         _1: usize,
// REWRITES-NEXT:         _2: *const core::ffi::c_char,
// REWRITES-NEXT:     ) -> *mut core::ffi::c_char;
// REWRITES-NEXT:     fn envz_entry(
// REWRITES-NEXT:         _0: *const core::ffi::c_char,
// REWRITES-NEXT:         _1: usize,
// REWRITES-NEXT:         _2: *const core::ffi::c_char,
// REWRITES-NEXT:     ) -> *mut core::ffi::c_char;
// REWRITES-NEXT:     fn envz_remove(_0: *mut *mut core::ffi::c_char, _1: *mut usize, _2: *const core::ffi::c_char);
// REWRITES-NEXT:     fn envz_strip(_0: *mut *mut core::ffi::c_char, _1: *mut usize);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_string_extensions() -> i32 {
// REWRITES-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(
// REWRITES-NEXT:         0,
// REWRITES-NEXT:         std::ptr::null_mut(),
// REWRITES-NEXT:         std::ptr::null_mut(),
// REWRITES-NEXT:         [0; 6],
// REWRITES-NEXT:         [0; 4],
// REWRITES-NEXT:         [0; 5],
// REWRITES-NEXT:         aligned::Aligned([0; 16]),
// REWRITES-NEXT:     );
// REWRITES-NEXT:     *{{__slate_alloca_frame[0-9]+}}.6 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.5 = [97, 98, 99, 97, 0];
// REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.4 = [103, 110, 117, 0];
// REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = [97, 58, 58, 98, 99, 0];
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.3.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.2 = {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// REWRITES-NEXT:         mempcpy(
// REWRITES-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:             b"slate\0".as_ptr() as *const core::ffi::c_void,
// REWRITES-NEXT:             (6 as u64) as usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.0 = 0;
// REWRITES-NEXT:     loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = (unsafe {
// REWRITES-NEXT:             strsep(
// REWRITES-NEXT:                 std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.2) as *mut *mut core::ffi::c_char,
// REWRITES-NEXT:                 c":".as_ptr(),
// REWRITES-NEXT:             )
// REWRITES-NEXT:         }) as *mut i8;
// REWRITES-NEXT:         {{__slate_alloca_frame[0-9]+}}.1 = {{_v[0-9]+}};
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// REWRITES-NEXT:         if !{{_v[0-9]+}} {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.0 * 10;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.1;
// REWRITES-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { strlen({{_v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// REWRITES-NEXT:         {{__slate_alloca_frame[0-9]+}}.0 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         memfrob(
// REWRITES-NEXT:             {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut core::ffi::c_void,
// REWRITES-NEXT:             (3 as u64) as usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         memfrob(
// REWRITES-NEXT:             {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut core::ffi::c_void,
// REWRITES-NEXT:             (3 as u64) as usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.6.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const core::ffi::c_char, c"slate".as_ptr()) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = ({{_v[0-9]+}} + ({{_v[0-9]+}} as i32)) as i64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// REWRITES-NEXT:         memrchr(
// REWRITES-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-NEXT:             97 as i32,
// REWRITES-NEXT:             (4 as u64) as usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-NEXT:         unsafe { rawmemchr({{_v[0-9]+}} as *const core::ffi::c_void, 99 as i32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.5.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe { strchrnul(c"abc".as_ptr(), 122 as i32) }) as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from(c"abc".as_ptr() as *mut i8) as i64 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 =
// REWRITES-NEXT:         (unsafe { strcasestr(c"GNU Library".as_ptr(), c"library".as_ptr()) }) as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + (({{_v[0-9]+}} as i32) as i64);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strverscmp(c"release-2".as_ptr(), c"release-10".as_ptr()) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + (({{_v[0-9]+}} as i32) as i64);
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{__slate_alloca_frame[0-9]+}}.4.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const core::ffi::c_char, c"gnu".as_ptr()) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + (({{_v[0-9]+}} as i32) as i64) + ({{__slate_alloca_frame[0-9]+}}.0 as i64);
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe { strerrorname_np(22 as i32) }) as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const core::ffi::c_char, c"EINVAL".as_ptr()) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} + (({{_v[0-9]+}} as i32) as i64);
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe { strerrordesc_np(22 as i32) }) as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// REWRITES-NEXT:     ({{_v[0-9]+}} + (({{_v[0-9]+}} as i32) as i64)) as i32
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_argz_extensions() -> i32 {
// REWRITES-NEXT:     let mut argz: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:     let mut length: u64 = 0;
// REWRITES-NEXT:     let mut arguments: aligned::Aligned<aligned::A16, [*mut i8; 6]> =
// REWRITES-NEXT:         aligned::Aligned([std::ptr::null_mut(); 6]);
// REWRITES-NEXT:     let mut replacements: u32 = 0;
// REWRITES-NEXT:     replacements = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         argz_create_sep(
// REWRITES-NEXT:             c"one:two:three".as_ptr(),
// REWRITES-NEXT:             58 as i32,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(argz) as *mut *mut core::ffi::c_char,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 =
// REWRITES-NEXT:         (unsafe { argz_count(argz as *const core::ffi::c_char, length as usize) }) as u64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         argz_extract(
// REWRITES-NEXT:             argz as *const core::ffi::c_char,
// REWRITES-NEXT:             length as usize,
// REWRITES-NEXT:             arguments.as_mut_ptr() as *mut *mut core::ffi::c_char,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         strcmp(
// REWRITES-NEXT:             arguments[((1 as i64) as usize)] as *const core::ffi::c_char,
// REWRITES-NEXT:             c"two".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         argz_add(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(argz) as *mut *mut core::ffi::c_char,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// REWRITES-NEXT:             c"four".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         argz_replace(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(argz) as *mut *mut core::ffi::c_char,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// REWRITES-NEXT:             c"three".as_ptr(),
// REWRITES-NEXT:             c"THREE".as_ptr(),
// REWRITES-NEXT:             std::ptr::addr_of_mut!(replacements) as *mut u32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = replacements == 1;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32) + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 =
// REWRITES-NEXT:         (unsafe { argz_count(argz as *const core::ffi::c_char, length as usize) }) as u64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 4;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     unsafe { argz_stringify(argz as *mut core::ffi::c_char, length as usize, 44 as i32) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         strcmp(
// REWRITES-NEXT:             argz as *const core::ffi::c_char,
// REWRITES-NEXT:             c"one,two,THREE,four".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     unsafe { free(argz as *mut core::ffi::c_void) };
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_envz_extensions() -> i32 {
// REWRITES-NEXT:     let mut envz: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:     let mut length: u64 = 0;
// REWRITES-NEXT:     envz = std::ptr::null_mut();
// REWRITES-NEXT:     length = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         envz_add(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// REWRITES-NEXT:             c"ALPHA".as_ptr(),
// REWRITES-NEXT:             c"one".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         envz_add(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// REWRITES-NEXT:             c"BETA".as_ptr(),
// REWRITES-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe {
// REWRITES-NEXT:         envz_get(
// REWRITES-NEXT:             envz as *const core::ffi::c_char,
// REWRITES-NEXT:             length as usize,
// REWRITES-NEXT:             c"ALPHA".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { strcmp({{_v[0-9]+}} as *const core::ffi::c_char, c"one".as_ptr()) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe {
// REWRITES-NEXT:         envz_entry(
// REWRITES-NEXT:             envz as *const core::ffi::c_char,
// REWRITES-NEXT:             length as usize,
// REWRITES-NEXT:             c"BETA".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         envz_remove(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// REWRITES-NEXT:             c"ALPHA".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = (unsafe {
// REWRITES-NEXT:         envz_get(
// REWRITES-NEXT:             envz as *const core::ffi::c_char,
// REWRITES-NEXT:             length as usize,
// REWRITES-NEXT:             c"ALPHA".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == std::ptr::null_mut();
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         envz_strip(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(envz) as *mut *mut core::ffi::c_char,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(length) as *mut usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = length == 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + ({{_v[0-9]+}} as i32);
// REWRITES-NEXT:     unsafe { free(envz as *mut core::ffi::c_void) };
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// REWRITES-NEXT:             gnu_string_extensions(),
// REWRITES-NEXT:             gnu_argz_extensions(),
// REWRITES-NEXT:             gnu_envz_extensions(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
