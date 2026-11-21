#include <stdio.h>
#include <string.h>

static const char *pick(int which, const char *left, const char *right) {
    return which ? left : right;
}

static int score(const int *left, const int *right, int which) {
    const int *chosen = which ? left : right;
    return *chosen;
}

int main(void) {
    char a[] = "alpha", b[] = "beta";
    int x = 3, y = 4;
    printf("%zu %d\n", strlen(pick(1, a, b)), score(&x, &y, 0));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::strlen;
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn pick(which: i32, left: *const i8, right: *const i8) -> *const i8 {
// CHECK-NEXT:     if which != 0 { left } else { right }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn score(left: &i32, right: &i32, which: i32) -> i32 {
// CHECK-NEXT:     let chosen: &i32 = if which != 0 { left } else { right };
// CHECK-NEXT:     *chosen
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: [i8; 6] = [97, 108, 112, 104, 97, 0];
// CHECK-NEXT:     let mut b: [i8; 5] = [98, 101, 116, 97, 0];
// CHECK-NEXT:     let x: i32 = 3;
// CHECK-NEXT:     let y: i32 = 4;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%zu %d\n".as_ptr(),
// CHECK-NEXT:             strlen(
// CHECK-NEXT:                 pick(1i32, a.as_mut_ptr() as *const i8, b.as_mut_ptr() as *const i8)
// CHECK-NEXT:                     as *const core::ffi::c_char,
// CHECK-NEXT:             ) as u64,
// CHECK-NEXT:             score(&x, &y, 0i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
