#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// @check-fn-begin
static int present_check(void) {
  setenv("SLATE_GETENV_FIXTURE_PRESENT", "1", 1);
  char *value = getenv("SLATE_GETENV_FIXTURE_PRESENT");
  if (value != NULL) {
    return 1;
  } else {
    return 0;
  }
}
// @check-fn-end

// @check-fn-begin
static int absent_check(void) {
  unsetenv("SLATE_GETENV_FIXTURE_ABSENT");
  char *value = getenv("SLATE_GETENV_FIXTURE_ABSENT");
  if (value != NULL) {
    return 1;
  } else {
    return 0;
  }
}
// @check-fn-end

// @check-fn-begin
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
// @check-fn-end

int main(void) {
  printf("%d\n", present_check());
  printf("%d\n", absent_check());
  printf("%d\n", rejected_check());
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn present_check() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         setenv(
// CHECK-NEXT:             c"SLATE_GETENV_FIXTURE_PRESENT".as_ptr() as *const i8,
// CHECK-NEXT:             c"1".as_ptr() as *const i8,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let value: *mut i8 = unsafe {
// CHECK-NEXT:         getenv(c"SLATE_GETENV_FIXTURE_PRESENT".as_ptr() as *const i8)
// CHECK-NEXT:     };
// CHECK-NEXT:     if value != std::ptr::null_mut::<i8>() {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn absent_check() -> i32 {
// CHECK-NEXT:     unsafe { unsetenv(c"SLATE_GETENV_FIXTURE_ABSENT".as_ptr() as *const i8) };
// CHECK-NEXT:     let value: *mut i8 = unsafe {
// CHECK-NEXT:         getenv(c"SLATE_GETENV_FIXTURE_ABSENT".as_ptr() as *const i8)
// CHECK-NEXT:     };
// CHECK-NEXT:     if value != std::ptr::null_mut::<i8>() {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn rejected_check() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         setenv(
// CHECK-NEXT:             c"SLATE_GETENV_FIXTURE_REJECTED".as_ptr() as *const i8,
// CHECK-NEXT:             c"ready".as_ptr() as *const i8,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let value: *mut i8 = unsafe {
// CHECK-NEXT:         getenv(c"SLATE_GETENV_FIXTURE_REJECTED".as_ptr() as *const i8)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut found: i32 = 0 as i32;
// CHECK-NEXT:     if value != std::ptr::null_mut::<i8>() {
// CHECK-NEXT:         found = ((unsafe { strcmp(value as *const i8, c"ready".as_ptr() as *const i8) })
// CHECK-NEXT:             == (0 as i32)) as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         found = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     found
// CHECK: {{^}}}
// SLATE-FILECHECK-END
