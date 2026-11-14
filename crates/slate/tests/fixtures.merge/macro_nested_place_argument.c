#include <stdio.h>

struct view {
    char *buf;
    long *suboffsets;
    long *strides;
};

#define HAVE_PTR(suboffsets, dim) (suboffsets && suboffsets[dim] >= 0)
#define ADJUST_PTR(ptr, suboffsets, dim) \
    (HAVE_PTR(suboffsets, dim) ? *((char**)ptr) + suboffsets[dim] : ptr)

static char *lookup(const struct view *view, long index, int dim) {
    char *ptr = view->buf;
    ptr += view->strides[dim] * index;
    ptr = ADJUST_PTR(ptr, view->suboffsets, dim);
    return ptr;
}

int main(void) {
    char data[4] = "abc";
    long strides[1] = {1};
    struct view plain = {data, NULL, strides};
    char *rows[2] = {data, data + 1};
    long subs[1] = {1};
    long one[1] = {sizeof(char *)};
    struct view indirect = {(char *)rows, subs, one};
    printf("%c %c\n", *lookup(&plain, 2, 0), *lookup(&indirect, 1, 0));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 1 compiles
// CHECK-NEXT:   macro_nested_place_argument.c: [] 1 compiled
// CHECK-NEXT: target windows-x86_64: 1 compiles
// CHECK-NEXT:   macro_nested_place_argument.c: [] 1 compiled
// CHECK-NEXT: total: 2 compiles across 2 targets
// CHECK-NEXT: unit macro_nested_place_argument: 12 items, 1 gated, 0 duplicated functions
// CHECK-NEXT: macros: 2 function-like, 2 lifted at least once (expansions counted per distinct compile)
// CHECK-NEXT:   macro ADJUST_PTR: 2 lifted, inline:
// CHECK-NEXT:   macro HAVE_PTR: 2 lifted, inline:
// CHECK-NEXT: projection: 12 checked, 0 not translatable, 0 mismatches
// CHECK-NEXT: signatures: 0 shared symbols, 0 mismatches
// CHECK-NEXT: exit: 0
// CHECK-NEXT: file main.rs
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(not(any(
// CHECK-NEXT:     all(target_arch = "x86_64", target_os = "linux", target_env = "gnu"),
// CHECK-NEXT:     all(target_arch = "x86_64", windows, target_env = "msvc")
// CHECK-NEXT: )))]
// CHECK-NEXT: compile_error!("this crate was not translated for this target");
// CHECK-EMPTY:
// CHECK-NEXT: #[cfg(windows)]
// CHECK-NEXT: mod __slate_oldnames {
// CHECK-NEXT:     #[link(name = "oldnames")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct view {
// CHECK-NEXT:     buf: *mut c_char,
// CHECK-NEXT:     suboffsets: *mut c_long,
// CHECK-NEXT:     strides: *mut c_long,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlignLargeArray<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut data: [c_char; 4] = [97, 98, 99, 0];
// CHECK-NEXT:     let mut strides: [c_long; 1] = [1];
// CHECK-NEXT:     let mut plain: view = view {
// CHECK-NEXT:         buf: data.as_mut_ptr() as *mut c_char,
// CHECK-NEXT:         suboffsets: std::ptr::null_mut::<c_long>(),
// CHECK-NEXT:         strides: strides.as_mut_ptr() as *mut c_long,
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut rows: __SlateAlignLargeArray<[*mut c_char; 2]> =
// CHECK-NEXT:         __SlateAlignLargeArray([data.as_mut_ptr() as *mut c_char, unsafe {
// CHECK-NEXT:             (data.as_mut_ptr() as *mut c_char).add(1)
// CHECK-NEXT:         }]);
// CHECK-NEXT:     let mut subs: [c_long; 1] = [1];
// CHECK-NEXT:     let mut one: [c_long; 1] = [std::mem::size_of::<*mut c_char>() as c_long];
// CHECK-NEXT:     let mut indirect: view = view {
// CHECK-NEXT:         buf: rows.0.as_mut_ptr() as *mut c_char,
// CHECK-NEXT:         suboffsets: subs.as_mut_ptr() as *mut c_long,
// CHECK-NEXT:         strides: one.as_mut_ptr() as *mut c_long,
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%c %c\n".as_ptr(),
// CHECK-NEXT:             *lookup(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(plain) as *const view,
// CHECK-NEXT:                 (2 as c_long),
// CHECK-NEXT:                 0i32,
// CHECK-NEXT:             ) as i32,
// CHECK-NEXT:             *lookup(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(indirect) as *const view,
// CHECK-NEXT:                 (1 as c_long),
// CHECK-NEXT:                 0i32,
// CHECK-NEXT:             ) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn lookup(view: *const view, index: c_long, dim: i32) -> *mut c_char {
// CHECK-NEXT:     let mut ptr: *mut c_char = unsafe { (*view).buf };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: *mut c_char = ptr;
// CHECK-NEXT:     ptr =
// CHECK-NEXT:         unsafe { [[V_0]].offset((*unsafe { (*view).strides }.offset(dim as isize) * index) as isize) };
// CHECK-NEXT:     ptr = ADJUST_PTR!(ptr, (*view).suboffsets, dim);
// CHECK-NEXT:     ptr
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! ADJUST_PTR {
// CHECK-NEXT:     ($ptr:expr, $suboffsets:expr, $dim:expr) => {
// CHECK-NEXT:         if HAVE_PTR!($suboffsets, $dim) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 unsafe { *(($ptr) as *mut *mut c_char) }
// CHECK-NEXT:                     .offset(*unsafe { $suboffsets }.offset(($dim) as isize) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             $ptr
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! HAVE_PTR {
// CHECK-NEXT:     ($suboffsets:expr, $dim:expr) => {
// CHECK-NEXT:         !unsafe { $suboffsets }.is_null()
// CHECK-NEXT:             && (unsafe { *unsafe { $suboffsets }.offset(($dim) as isize) }) >= 0
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use core::ffi::c_char;
// CHECK-NEXT: use core::ffi::c_long;
// CHECK-NEXT: use slate_support::size_t;
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
