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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut f: f32 = 0.0;
// LOWERING-NEXT:     let mut old_f: f32 = 0.0;
// LOWERING-NEXT:     let mut now_f: f32 = 0.0;
// LOWERING-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut p: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut old_p: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut atomic_temp: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut now_p: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut old_x: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut now_x: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 1.5;
// LOWERING-NEXT:     f = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 2.25;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { *std::ptr::addr_of_mut!(f) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *std::ptr::addr_of_mut!(f) = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     old_f = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = f;
// LOWERING-NEXT:     now_f = {{_v[0-9]+}};
// LOWERING-NEXT:     *values = [10, 20, 30, 40];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     p = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i64 = std::ptr::addr_of_mut!(p) as *mut i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { std::sync::atomic::AtomicI64::from_ptr({{_v[0-9]+}}).fetch_add({{_v[0-9]+}}, std::sync::atomic::Ordering::AcqRel) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i64 = std::ptr::addr_of_mut!(atomic_temp) as *mut i64;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = atomic_temp;
// LOWERING-NEXT:     old_p = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = p;
// LOWERING-NEXT:     now_p = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { *std::ptr::addr_of_mut!(p) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *std::ptr::addr_of_mut!(p) = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     old_x = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = p;
// LOWERING-NEXT:     now_x = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%.2f %.2f %td %td %d %td %td\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = old_f;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = now_f;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = old_p;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = now_p;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = now_p;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = old_x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = now_x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-DAG: *std::ptr::addr_of_mut!(f) =
// REWRITES-DAG: *std::ptr::addr_of_mut!(p) =
// REWRITES-NOT: AtomicF32
