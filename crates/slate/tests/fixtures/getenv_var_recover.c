#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// @lowering-fn-begin
// @rewrite-fn-begin
static int present_check(void) {
  setenv("SLATE_GETENV_FIXTURE_PRESENT", "1", 1);
  char *value = getenv("SLATE_GETENV_FIXTURE_PRESENT");
  if (value != NULL) {
    return 1;
  } else {
    return 0;
  }
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
static int absent_check(void) {
  unsetenv("SLATE_GETENV_FIXTURE_ABSENT");
  char *value = getenv("SLATE_GETENV_FIXTURE_ABSENT");
  if (value != NULL) {
    return 1;
  } else {
    return 0;
  }
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
static int rejected_check(void) {
  setenv("SLATE_GETENV_FIXTURE_REJECTED", "ready", 1);
  char *value = getenv("SLATE_GETENV_FIXTURE_REJECTED");
  int   found;
  if (value != NULL) {
    found = strcmp(value, "ready") == 0;
  } else {
    found = 0;
  }
  return found;
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  printf("%d\n", present_check());
  printf("%d\n", absent_check());
  printf("%d\n", rejected_check());
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn present_check() -> i32 {
// COMMON-LOWERING-DAG:     let mut __retval: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:         setenv(
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-DAG:         )
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     value = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:             return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         } else {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:             return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn absent_check() -> i32 {
// COMMON-LOWERING-DAG:     let mut __retval: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { unsetenv({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-DAG:     value = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:             return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         } else {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:             return {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn rejected_check() -> i32 {
// COMMON-LOWERING-DAG:     let mut found: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:         setenv(
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-DAG:         )
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     value = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:                 strcmp(
// COMMON-LOWERING-DAG:                     {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-DAG:                     {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-DAG:                 )
// COMMON-LOWERING-DAG:             };
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:             found = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         } else {
// COMMON-LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:             found = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = found;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG:     let mut value: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"SLATE_GETENV_FIXTURE_PRESENT\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"1\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"SLATE_GETENV_FIXTURE_PRESENT\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = (unsafe { getenv({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut i8;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = value;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let mut value: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"SLATE_GETENV_FIXTURE_ABSENT\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"SLATE_GETENV_FIXTURE_ABSENT\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = (unsafe { getenv({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut i8;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = value;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let mut value: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"SLATE_GETENV_FIXTURE_REJECTED\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"ready\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"SLATE_GETENV_FIXTURE_REJECTED\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = (unsafe { getenv({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut i8;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = value;
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = value;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = b"ready\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG:     let mut value: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"SLATE_GETENV_FIXTURE_PRESENT\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"1\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"SLATE_GETENV_FIXTURE_PRESENT\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = (unsafe { getenv({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut u8;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = value;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let mut value: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"SLATE_GETENV_FIXTURE_ABSENT\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"SLATE_GETENV_FIXTURE_ABSENT\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = (unsafe { getenv({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut u8;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = value;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let mut value: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"SLATE_GETENV_FIXTURE_REJECTED\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"ready\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"SLATE_GETENV_FIXTURE_REJECTED\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = (unsafe { getenv({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut u8;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = value;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = value;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = b"ready\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn present_check() -> i32 {
// COMMON-REWRITES-DAG:     let mut __retval: i32 = 0;
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         setenv(
// COMMON-REWRITES-DAG:             c"SLATE_GETENV_FIXTURE_PRESENT".as_ptr(),
// COMMON-REWRITES-DAG:             c"1".as_ptr(),
// COMMON-REWRITES-DAG:             1 as i32,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     match std::env::var("SLATE_GETENV_FIXTURE_PRESENT") {
// COMMON-REWRITES-DAG:         Ok(value) => {
// COMMON-REWRITES-DAG:             return 1;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:         Err(_) => {
// COMMON-REWRITES-DAG:             return 0;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     __retval
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn absent_check() -> i32 {
// COMMON-REWRITES-DAG:     let mut __retval: i32 = 0;
// COMMON-REWRITES-DAG:     unsafe { unsetenv(c"SLATE_GETENV_FIXTURE_ABSENT".as_ptr()) };
// COMMON-REWRITES-DAG:     match std::env::var("SLATE_GETENV_FIXTURE_ABSENT") {
// COMMON-REWRITES-DAG:         Ok(value) => {
// COMMON-REWRITES-DAG:             return 1;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:         Err(_) => {
// COMMON-REWRITES-DAG:             return 0;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     __retval
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn rejected_check() -> i32 {
// COMMON-REWRITES-DAG:     let mut found: i32 = 0;
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         setenv(
// COMMON-REWRITES-DAG:             c"SLATE_GETENV_FIXTURE_REJECTED".as_ptr(),
// COMMON-REWRITES-DAG:             c"ready".as_ptr(),
// COMMON-REWRITES-DAG:             1 as i32,
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-DAG:     if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { strcmp(value as *const core::ffi::c_char, c"ready".as_ptr()) };
// COMMON-REWRITES-DAG:         found = ({{__v[0-9]+}} == 0) as i32;
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         found = 0;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     found
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG:     let mut value: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-DAG:     value = (unsafe { getenv(c"SLATE_GETENV_FIXTURE_REJECTED".as_ptr()) }) as *mut i8;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = value != ({{__v[0-9]+}} as *mut i8);
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG:     let mut value: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-DAG:     value = (unsafe { getenv(c"SLATE_GETENV_FIXTURE_REJECTED".as_ptr()) }) as *mut u8;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool = value != ({{__v[0-9]+}} as *mut u8);
// SLATE-FILECHECK-END rewrites-aarch64-gnu
