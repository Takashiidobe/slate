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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn present_check() -> i32 {
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     let mut value: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"SLATE_GETENV_FIXTURE_PRESENT\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"1\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG:     let mut value: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"SLATE_GETENV_FIXTURE_PRESENT\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"1\0".as_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:         setenv(
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-DAG:             {{__v[0-9]+}} as i32,
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"SLATE_GETENV_FIXTURE_PRESENT\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = (unsafe { getenv({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut i8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"SLATE_GETENV_FIXTURE_PRESENT\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = (unsafe { getenv({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut u8;
// LOWERING-DAG:     value = {{__v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = value;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = value;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             __retval = {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:             return {{__v[0-9]+}};
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:             __retval = {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:             return {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn absent_check() -> i32 {
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     let mut value: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"SLATE_GETENV_FIXTURE_ABSENT\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG:     let mut value: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"SLATE_GETENV_FIXTURE_ABSENT\0".as_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { unsetenv({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"SLATE_GETENV_FIXTURE_ABSENT\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = (unsafe { getenv({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut i8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"SLATE_GETENV_FIXTURE_ABSENT\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = (unsafe { getenv({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut u8;
// LOWERING-DAG:     value = {{__v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = value;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = value;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:             __retval = {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:             return {{__v[0-9]+}};
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:             __retval = {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:             return {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn rejected_check() -> i32 {
// LOWERING-X86_64-GNU-DAG:     let mut value: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-DAG:     let mut value: *mut u8 = std::ptr::null_mut();
// LOWERING-DAG:     let mut found: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"SLATE_GETENV_FIXTURE_REJECTED\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"ready\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"SLATE_GETENV_FIXTURE_REJECTED\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"ready\0".as_ptr() as *mut u8;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:         setenv(
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-DAG:             {{__v[0-9]+}} as i32,
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"SLATE_GETENV_FIXTURE_REJECTED\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = (unsafe { getenv({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut i8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"SLATE_GETENV_FIXTURE_REJECTED\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = (unsafe { getenv({{__v[0-9]+}} as *const core::ffi::c_char) }) as *mut u8;
// LOWERING-DAG:     value = {{__v[0-9]+}};
// LOWERING-DAG:     {
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = value;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = value;
// LOWERING-DAG:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-X86_64-GNU-DAG:         let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-AARCH64-GNU-DAG:         let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = value;
// LOWERING-X86_64-GNU-DAG:             let {{__v[0-9]+}}: *mut i8 = b"ready\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = value;
// LOWERING-AARCH64-GNU-DAG:             let {{__v[0-9]+}}: *mut u8 = b"ready\0".as_ptr() as *mut u8;
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:                 strcmp(
// LOWERING-DAG:                     {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-DAG:                     {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-DAG:                 )
// LOWERING-DAG:             };
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:             found = {{__v[0-9]+}};
// LOWERING-DAG:         } else {
// LOWERING-DAG:             let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:             found = {{__v[0-9]+}};
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = found;
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn present_check() -> i32 {
// REWRITES-DAG:     let mut __retval: i32 = 0;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         setenv(
// REWRITES-DAG:             c"SLATE_GETENV_FIXTURE_PRESENT".as_ptr(),
// REWRITES-DAG:             c"1".as_ptr(),
// REWRITES-DAG:             1 as i32,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     match std::env::var("SLATE_GETENV_FIXTURE_PRESENT") {
// REWRITES-DAG:         Ok(value) => {
// REWRITES-DAG:             return 1;
// REWRITES-DAG:         }
// REWRITES-DAG:         Err(_) => {
// REWRITES-DAG:             return 0;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     __retval
// REWRITES-DAG: }
// REWRITES-DAG: fn absent_check() -> i32 {
// REWRITES-DAG:     let mut __retval: i32 = 0;
// REWRITES-DAG:     unsafe { unsetenv(c"SLATE_GETENV_FIXTURE_ABSENT".as_ptr()) };
// REWRITES-DAG:     match std::env::var("SLATE_GETENV_FIXTURE_ABSENT") {
// REWRITES-DAG:         Ok(value) => {
// REWRITES-DAG:             return 1;
// REWRITES-DAG:         }
// REWRITES-DAG:         Err(_) => {
// REWRITES-DAG:             return 0;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     __retval
// REWRITES-DAG: }
// REWRITES-DAG: fn rejected_check() -> i32 {
// REWRITES-X86_64-GNU-DAG:     let mut value: *mut i8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-DAG:     let mut value: *mut u8 = std::ptr::null_mut();
// REWRITES-DAG:     let mut found: i32 = 0;
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         setenv(
// REWRITES-DAG:             c"SLATE_GETENV_FIXTURE_REJECTED".as_ptr(),
// REWRITES-DAG:             c"ready".as_ptr(),
// REWRITES-DAG:             1 as i32,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-X86_64-GNU-DAG:     value = (unsafe { getenv(c"SLATE_GETENV_FIXTURE_REJECTED".as_ptr()) }) as *mut i8;
// REWRITES-AARCH64-GNU-DAG:     value = (unsafe { getenv(c"SLATE_GETENV_FIXTURE_REJECTED".as_ptr()) }) as *mut u8;
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: bool = value != ({{__v[0-9]+}} as *mut i8);
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: bool = value != ({{__v[0-9]+}} as *mut u8);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         let {{__v[0-9]+}}: i32 = unsafe { strcmp(value as *const core::ffi::c_char, c"ready".as_ptr()) };
// REWRITES-DAG:         found = ({{__v[0-9]+}} == 0) as i32;
// REWRITES-DAG:     } else {
// REWRITES-DAG:         found = 0;
// REWRITES-DAG:     }
// REWRITES-DAG:     found
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
