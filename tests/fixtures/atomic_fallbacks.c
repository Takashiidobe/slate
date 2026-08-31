#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>

int main(void) {
  _Atomic(float) f = 1.5f;
  float old_f      = atomic_fetch_add_explicit(&f, 2.25f, memory_order_relaxed);
  float now_f      = atomic_load(&f);

  int            values[4] = {10, 20, 30, 40};
  _Atomic(int *) p         = values;
  int           *old_p = atomic_fetch_add_explicit(&p, 2, memory_order_acq_rel);
  int           *now_p = atomic_load(&p);
  int *old_x = atomic_exchange_explicit(&p, values + 1, memory_order_release);
  int *now_x = atomic_load(&p);

  printf("%.2f %.2f %td %td %d %td %td\n", old_f, now_f, old_p - values,
         now_p - values, *now_p, old_x - values, now_x - values);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: struct __SlateAllocaFrame0(*mut i32, *mut i32, aligned::Aligned<aligned::A16, [i32; 4]>, f32);
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum memory_order {
// LOWERING-NEXT:     memory_order_relaxed = 0,
// LOWERING-NEXT:     memory_order_consume = 1,
// LOWERING-NEXT:     memory_order_acquire = 2,
// LOWERING-NEXT:     memory_order_release = 3,
// LOWERING-NEXT:     memory_order_acq_rel = 4,
// LOWERING-NEXT:     memory_order_seq_cst = 5,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(std::ptr::null_mut(), std::ptr::null_mut(), aligned::Aligned([0; 4]), 0.0);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.5;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 2.25;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.3) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.3) = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{__slate_alloca_frame[0-9]+}}.3;
// LOWERING-NEXT:     *{{__slate_alloca_frame[0-9]+}}.2 = [10, 20, 30, 40];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.1 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i64 = std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.1) as *mut i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { std::sync::atomic::AtomicI64::from_ptr({{_v[0-9]+}}).fetch_add({{_v[0-9]+}}, std::sync::atomic::Ordering::AcqRel) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i64 = std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.0) as *mut i64;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.1) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.1) = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%.2f %.2f %td %td %d %td %td\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: struct __SlateAllocaFrame0(*mut i32, *mut i32, aligned::Aligned<aligned::A16, [i32; 4]>, f32);
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum memory_order {
// REWRITES-NEXT:     memory_order_relaxed = 0,
// REWRITES-NEXT:     memory_order_consume = 1,
// REWRITES-NEXT:     memory_order_acquire = 2,
// REWRITES-NEXT:     memory_order_release = 3,
// REWRITES-NEXT:     memory_order_acq_rel = 4,
// REWRITES-NEXT:     memory_order_seq_cst = 5,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(std::ptr::null_mut(), std::ptr::null_mut(), aligned::Aligned([0; 4]), 0.0);
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.3 = 1.5;
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = 2.25;
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = unsafe { *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.3) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.3) = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = {{__slate_alloca_frame[0-9]+}}.3;
// REWRITES-NEXT: *{{__slate_alloca_frame[0-9]+}}.2 = [10, 20, 30, 40];
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: {{__slate_alloca_frame[0-9]+}}.1 = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i64 = std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.1) as *mut i64;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { std::sync::atomic::AtomicI64::from_ptr({{_v[0-9]+}}).fetch_add({{_v[0-9]+}}, std::sync::atomic::Ordering::AcqRel) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i64 = std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.0) as *mut i64;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.1;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = unsafe { *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.1) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.1) = {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.1;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%.2f %.2f %td %td %d %td %td\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// REWRITES-NEXT: let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
