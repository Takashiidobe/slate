#include <stdio.h>

int main(void) {
  float _Complex f       = __builtin_complex(1.0f, 2.0f);
  double _Complex d      = __builtin_complex(3.0, 4.0);
  long double _Complex l = __builtin_complex(5.0L, 6.0L);

  printf("%d %d\n", f == __builtin_complex(1.0f, 2.0f),
         f != __builtin_complex(1.0f, 3.0f));
  printf("%d %d\n", d == __builtin_complex(3.0, 4.0),
         d != __builtin_complex(3.0, 5.0));
  printf("%d %d\n", l == __builtin_complex(5.0L, 6.0L),
         l != __builtin_complex(5.0L, 7.0L));
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(f128)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_libgcc {
// CHECK-NEXT:     #[link(name = "gcc", kind = "static")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let f: num_complex::Complex<f32> = num_complex::Complex {
// CHECK-NEXT:         re: 1.0f32,
// CHECK-NEXT:         im: 2.0f32,
// CHECK-NEXT:     };
// CHECK-NEXT:     let d: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:         re: 3.0f64,
// CHECK-NEXT:         im: 4.0f64,
// CHECK-NEXT:     };
// CHECK-NEXT:     let l: num_complex::Complex<f128> = num_complex::Complex {
// CHECK-NEXT:         re: 5f128,
// CHECK-NEXT:         im: 6f128,
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", (f == num_complex::Complex { re : 1.0f32, im : 2.0f32 }) as i32, (f !=
// CHECK-NEXT:         num_complex::Complex { re : 1.0f32, im : 3.0f32 }) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", (d == num_complex::Complex { re : 3.0f64, im : 4.0f64 }) as i32, (d !=
// CHECK-NEXT:         num_complex::Complex { re : 3.0f64, im : 5.0f64 }) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", (l == num_complex::Complex { re : 5f128, im : 6f128 }) as i32, (l !=
// CHECK-NEXT:         num_complex::Complex { re : 5f128, im : 7f128 }) as i32
// CHECK-NEXT:     );
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
