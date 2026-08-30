#include <stdio.h>

static int read_pointer(int *pointer) { return *pointer; }

static int read_array_element_through_call(void) {
  int  values[1] = {10};
  int *pointer   = &values[0];
  return read_pointer(pointer);
}

int main(void) {
  int  values[4] = {2, 4, 6, 8};
  int *p         = &values[1];
  int *q         = &values[3];
  printf("%d %ld %d\n", *p + *q, q - p, read_array_element_through_call());
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: struct __SlateAllocaFrame0(*mut i32, *mut i32, aligned::Aligned<aligned::A16, [i32; 4]>, i32);
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn read_pointer({{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-NEXT:     let mut pointer: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     pointer = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = pointer;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn read_array_element_through_call() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut values: [i32; 1] = [0; 1];
// LOWERING-NEXT:     let mut pointer: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     values = [10];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     pointer = std::ptr::addr_of_mut!(values[({{_v[0-9]+}} as usize)]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = pointer;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = read_pointer({{_v[0-9]+}});
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(std::ptr::null_mut(), std::ptr::null_mut(), aligned::Aligned([0; 4]), 0);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = {{_v[0-9]+}};
// LOWERING-NEXT:     *{{__slate_alloca_frame[0-9]+}}.2 = [2, 4, 6, 8];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.1 = std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.2[({{_v[0-9]+}} as usize)]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.0 = std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.2[({{_v[0-9]+}} as usize)]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %ld %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = read_array_element_through_call();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{__slate_alloca_frame[0-9]+}}.3;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-NOT: unsafe { *p }
// REWRITES-NOT: unsafe { *q }
// REWRITES-NOT: unsafe { *pointer }
