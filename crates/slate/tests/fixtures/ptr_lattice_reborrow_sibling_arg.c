#include <stdio.h>

static int count(const char *format, char end) {
    int n = 0;
    while (*format && *format != end) {
        format++;
        n++;
    }
    return n;
}

static int group(const char **p_format, char end, int n) {
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += **p_format;
        (*p_format)++;
    }
    if (**p_format == end)
        (*p_format)++;
    return total;
}

static int value(const char **p_format) {
    const char c = *(*p_format)++;
    if (c == '(')
        return group(p_format, ')', count(*p_format, ')'));
    return c;
}

int main(void) {
    const char *format = "(abc)d";
    const char *p = format;
    int first = value(&p);
    int second = value(&p);
    printf("%d %d %s\n", first, second, p);
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn count(mut format: *const i8, end: i8) -> i32 {
// CHECK-NEXT:     let mut n: i32 = 0;
// CHECK-NEXT:     while (unsafe { *format }) != 0 && ((unsafe { *format }) as i32) != (end as i32) {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: *const i8 = format;
// CHECK-NEXT:         format = unsafe { [[V_0]].add(1) };
// CHECK-NEXT:         n += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     n
// CHECK-NEXT: }
// CHECK-NEXT: fn group(p_format: &mut *const i8, end: i8, n: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         total += (unsafe { *(*(p_format as *mut *const i8)) }) as i32;
// CHECK-NEXT:         let [[V_1:__v[0-9]+]]: *const i8 = unsafe { *p_format };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *p_format = [[V_1]].add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { *(*(p_format as *mut *const i8)) }) as i32) == (end as i32) {
// CHECK-NEXT:         let [[V_2:__v[0-9]+]]: *const i8 = unsafe { *p_format };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *p_format = [[V_2]].add(1);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn value(p_format: &mut *const i8) -> i32 {
// CHECK-NEXT:     let mut c: i8 = 0;
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: *const i8 = unsafe { *p_format };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p_format = [[V_3]].add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     c = unsafe { *[[V_3]] };
// CHECK-NEXT:     if (c as i32) == 40 {
// CHECK-NEXT:         return {
// CHECK-NEXT:             let __arg2 = count(unsafe { *(p_format as *mut *const i8) }, 41i8);
// CHECK-NEXT:             group(p_format, 41i8, __arg2)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     c as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let format: *const i8 = c"(abc)d".as_ptr();
// CHECK-NEXT:     let mut p: *const i8 = format;
// CHECK-NEXT:     let first: i32 = value(unsafe { &mut (*std::ptr::addr_of_mut!(p)) });
// CHECK-NEXT:     let second: i32 = value(unsafe { &mut (*std::ptr::addr_of_mut!(p)) });
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%d %d %s\n".as_ptr(), first, second, p) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
