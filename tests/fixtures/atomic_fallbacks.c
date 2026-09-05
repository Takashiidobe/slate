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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[allow(non_camel_case_types)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-LOWERING-NEXT: enum memory_order {
// COMMON-LOWERING-NEXT:     memory_order_relaxed = 0,
// COMMON-LOWERING-NEXT:     memory_order_consume = 1,
// COMMON-LOWERING-NEXT:     memory_order_acquire = 2,
// COMMON-LOWERING-NEXT:     memory_order_release = 3,
// COMMON-LOWERING-NEXT:     memory_order_acq_rel = 4,
// COMMON-LOWERING-NEXT:     memory_order_seq_cst = 5,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 1.5;
// COMMON-LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 2.25;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.3) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.3) = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = {{__slate_alloca_frame[0-9]+}}.3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [10, 20, 30, 40];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     {{__slate_alloca_frame[0-9]+}}.1 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i64 = std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.1) as *mut i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI64::from_ptr({{__v[0-9]+}})
// COMMON-LOWERING-NEXT:             .fetch_add({{__v[0-9]+}}, std::sync::atomic::Ordering::AcqRel)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i64 = std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.0) as *mut i64;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.1) };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.1) = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: struct __SlateAllocaFrame0(
// LOWERING-X86_64-GNU-NEXT:     *mut i32,
// LOWERING-X86_64-GNU-NEXT:     *mut i32,
// LOWERING-X86_64-GNU-NEXT:     aligned::Aligned<aligned::A16, [i32; 4]>,
// LOWERING-X86_64-GNU-NEXT:     f32,
// LOWERING-X86_64-GNU-NEXT: );
// LOWERING-X86_64-GNU-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(
// LOWERING-X86_64-GNU-NEXT:         std::ptr::null_mut(),
// LOWERING-X86_64-GNU-NEXT:         std::ptr::null_mut(),
// LOWERING-X86_64-GNU-NEXT:         aligned::Aligned([0; 4]),
// LOWERING-X86_64-GNU-NEXT:         0.0,
// LOWERING-X86_64-GNU-NEXT:     );
// LOWERING-X86_64-GNU-NEXT:     *{{__slate_alloca_frame[0-9]+}}.2 = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%.2f %.2f %td %td %d %td %td\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT: struct __SlateAllocaFrame0(*mut i32, *mut i32, [i32; 4], f32);
// LOWERING-AARCH64-GNU-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 =
// LOWERING-AARCH64-GNU-NEXT:         __SlateAllocaFrame0(std::ptr::null_mut(), std::ptr::null_mut(), [0; 4], 0.0);
// LOWERING-AARCH64-GNU-NEXT:     {{__slate_alloca_frame[0-9]+}}.2 = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%.2f %.2f %td %td %d %td %td\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[allow(non_camel_case_types)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-REWRITES-NEXT: enum memory_order {
// COMMON-REWRITES-NEXT:     memory_order_relaxed = 0,
// COMMON-REWRITES-NEXT:     memory_order_consume = 1,
// COMMON-REWRITES-NEXT:     memory_order_acquire = 2,
// COMMON-REWRITES-NEXT:     memory_order_release = 3,
// COMMON-REWRITES-NEXT:     memory_order_acq_rel = 4,
// COMMON-REWRITES-NEXT:     memory_order_seq_cst = 5,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.3 = 1.5;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = 2.25;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.3) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.3) = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f32 = {{__slate_alloca_frame[0-9]+}}.3;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     {{__slate_alloca_frame[0-9]+}}.1 = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 4;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 2 * {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i64 = std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.1) as *mut i64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI64::from_ptr({{__v[0-9]+}})
// COMMON-REWRITES-NEXT:             .fetch_add({{__v[0-9]+}}, std::sync::atomic::Ordering::AcqRel)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i64 = std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.0) as *mut i64;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.1) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *std::ptr::addr_of_mut!({{__slate_alloca_frame[0-9]+}}.1) = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} as f64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{__slate_alloca_frame[0-9]+}}.2.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 },
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: struct __SlateAllocaFrame0(
// REWRITES-X86_64-GNU-NEXT:     *mut i32,
// REWRITES-X86_64-GNU-NEXT:     *mut i32,
// REWRITES-X86_64-GNU-NEXT:     aligned::Aligned<aligned::A16, [i32; 4]>,
// REWRITES-X86_64-GNU-NEXT:     f32,
// REWRITES-X86_64-GNU-NEXT: );
// REWRITES-X86_64-GNU-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 = __SlateAllocaFrame0(
// REWRITES-X86_64-GNU-NEXT:         std::ptr::null_mut(),
// REWRITES-X86_64-GNU-NEXT:         std::ptr::null_mut(),
// REWRITES-X86_64-GNU-NEXT:         aligned::Aligned([0; 4]),
// REWRITES-X86_64-GNU-NEXT:         0.0,
// REWRITES-X86_64-GNU-NEXT:     );
// REWRITES-X86_64-GNU-NEXT:     *{{__slate_alloca_frame[0-9]+}}.2 = [10, 20, 30, 40];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%.2f %.2f %td %td %d %td %td\n".as_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: struct __SlateAllocaFrame0(*mut i32, *mut i32, [i32; 4], f32);
// REWRITES-AARCH64-GNU-NEXT:     let mut {{__slate_alloca_frame[0-9]+}}: __SlateAllocaFrame0 =
// REWRITES-AARCH64-GNU-NEXT:         __SlateAllocaFrame0(std::ptr::null_mut(), std::ptr::null_mut(), [0; 4], 0.0);
// REWRITES-AARCH64-GNU-NEXT:     {{__slate_alloca_frame[0-9]+}}.2 = [10, 20, 30, 40];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%.2f %.2f %td %td %d %td %td\n".as_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
