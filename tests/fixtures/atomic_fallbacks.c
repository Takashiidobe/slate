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
// LOWERING-NEXT:     *mut i32,
// LOWERING-NEXT:     *mut i32,
// LOWERING-NEXT:     aligned::Aligned<aligned::A16, [i32; 4]>,
// LOWERING-NEXT:     f32,
// LOWERING-NEXT: );
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
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(
// LOWERING-NEXT:         std::ptr::null_mut(),
// LOWERING-NEXT:         std::ptr::null_mut(),
// LOWERING-NEXT:         aligned::Aligned([0; 4]),
// LOWERING-NEXT:         0.0,
// LOWERING-NEXT:     );
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI64::from_ptr({{_v[0-9]+}})
// LOWERING-NEXT:             .fetch_add({{_v[0-9]+}}, std::sync::atomic::Ordering::AcqRel)
// LOWERING-NEXT:     };
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
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
// REWRITES-NEXT:     *mut i32,
// REWRITES-NEXT:     *mut i32,
// REWRITES-NEXT:     aligned::Aligned<aligned::A16, [i32; 4]>,
// REWRITES-NEXT:     f32,
// REWRITES-NEXT: );
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
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(
// REWRITES-NEXT:         std::ptr::null_mut(),
// REWRITES-NEXT:         std::ptr::null_mut(),
// REWRITES-NEXT:         aligned::Aligned([0; 4]),
// REWRITES-NEXT:         0.0,
// REWRITES-NEXT:     );
// REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = 1.5;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = 2.25;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.3) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.3) = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: f32 = {{__slate_alloca_frame[0-9]+}}.3;
// REWRITES-NEXT:     *{{__slate_alloca_frame[0-9]+}}.2 = [10, 20, 30, 40];
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.1 = {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 4;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = 2 * {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i64 = std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.1) as *mut i64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI64::from_ptr({{_v[0-9]+}})
// REWRITES-NEXT:             .fetch_add({{_v[0-9]+}}, std::sync::atomic::Ordering::AcqRel)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i64 = std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.0) as *mut i64;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.1;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.1) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.1) = {{_v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.1;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"%.2f %.2f %td %td %d %td %td\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 },
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
