#include <stdio.h>

static _Bool nonzero_double = __builtin_complex(2.5, 3.5);
static _Bool zero_double = __builtin_complex(0.0, 0.0);
static _Bool imag_only_float = __builtin_complex(0.0f, 1.0f);
static int equal = __builtin_complex(1.0, 2.0) == __builtin_complex(1.0, 2.0);
static int differ = __builtin_complex(1.0, 2.0) != __builtin_complex(1.0, -2.0);

int main(void) {
  printf("%d %d %d %d %d\n", nonzero_double, zero_double, imag_only_float, equal,
         differ);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut nonzero_double: bool = num_complex::Complex {
// CHECK-NEXT:     re: 2.5f64,
// CHECK-NEXT:     im: 3.5f64,
// CHECK-NEXT: }
// CHECK-NEXT:     .re
// CHECK-NEXT:     != num_complex::Complex {
// CHECK-NEXT:         re: 0.0f64,
// CHECK-NEXT:         im: 0.0,
// CHECK-NEXT:     }
// CHECK-NEXT:         .re
// CHECK-NEXT:     || num_complex::Complex {
// CHECK-NEXT:         re: 2.5f64,
// CHECK-NEXT:         im: 3.5f64,
// CHECK-NEXT:     }
// CHECK-NEXT:         .im
// CHECK-NEXT:         != num_complex::Complex {
// CHECK-NEXT:             re: 0.0f64,
// CHECK-NEXT:             im: 0.0,
// CHECK-NEXT:         }
// CHECK-NEXT:             .im;
// CHECK-NEXT: static mut zero_double: bool = num_complex::Complex {
// CHECK-NEXT:     re: 0.0f64,
// CHECK-NEXT:     im: 0.0f64,
// CHECK-NEXT: }
// CHECK-NEXT:     .re
// CHECK-NEXT:     != num_complex::Complex {
// CHECK-NEXT:         re: 0.0f64,
// CHECK-NEXT:         im: 0.0,
// CHECK-NEXT:     }
// CHECK-NEXT:         .re
// CHECK-NEXT:     || num_complex::Complex {
// CHECK-NEXT:         re: 0.0f64,
// CHECK-NEXT:         im: 0.0f64,
// CHECK-NEXT:     }
// CHECK-NEXT:         .im
// CHECK-NEXT:         != num_complex::Complex {
// CHECK-NEXT:             re: 0.0f64,
// CHECK-NEXT:             im: 0.0,
// CHECK-NEXT:         }
// CHECK-NEXT:             .im;
// CHECK-NEXT: static mut imag_only_float: bool = num_complex::Complex {
// CHECK-NEXT:     re: 0.0f32,
// CHECK-NEXT:     im: 1.0f32,
// CHECK-NEXT: }
// CHECK-NEXT:     .re
// CHECK-NEXT:     != num_complex::Complex {
// CHECK-NEXT:         re: 0.0f32,
// CHECK-NEXT:         im: 0.0,
// CHECK-NEXT:     }
// CHECK-NEXT:         .re
// CHECK-NEXT:     || num_complex::Complex {
// CHECK-NEXT:         re: 0.0f32,
// CHECK-NEXT:         im: 1.0f32,
// CHECK-NEXT:     }
// CHECK-NEXT:         .im
// CHECK-NEXT:         != num_complex::Complex {
// CHECK-NEXT:             re: 0.0f32,
// CHECK-NEXT:             im: 0.0,
// CHECK-NEXT:         }
// CHECK-NEXT:             .im;
// CHECK-NEXT: static mut equal: i32 = (num_complex::Complex {
// CHECK-NEXT:     re: 1.0f64,
// CHECK-NEXT:     im: 2.0f64,
// CHECK-NEXT: }
// CHECK-NEXT:     .re
// CHECK-NEXT:     == num_complex::Complex {
// CHECK-NEXT:         re: 1.0f64,
// CHECK-NEXT:         im: 2.0f64,
// CHECK-NEXT:     }
// CHECK-NEXT:         .re
// CHECK-NEXT:     && num_complex::Complex {
// CHECK-NEXT:         re: 1.0f64,
// CHECK-NEXT:         im: 2.0f64,
// CHECK-NEXT:     }
// CHECK-NEXT:         .im
// CHECK-NEXT:         == num_complex::Complex {
// CHECK-NEXT:             re: 1.0f64,
// CHECK-NEXT:             im: 2.0f64,
// CHECK-NEXT:         }
// CHECK-NEXT:             .im) as i32;
// CHECK-NEXT: static mut differ: i32 = (num_complex::Complex {
// CHECK-NEXT:     re: 1.0f64,
// CHECK-NEXT:     im: 2.0f64,
// CHECK-NEXT: }
// CHECK-NEXT:     .re
// CHECK-NEXT:     != num_complex::Complex {
// CHECK-NEXT:         re: 1.0f64,
// CHECK-NEXT:         im: -2.0f64,
// CHECK-NEXT:     }
// CHECK-NEXT:         .re
// CHECK-NEXT:     || num_complex::Complex {
// CHECK-NEXT:         re: 1.0f64,
// CHECK-NEXT:         im: 2.0f64,
// CHECK-NEXT:     }
// CHECK-NEXT:         .im
// CHECK-NEXT:         != num_complex::Complex {
// CHECK-NEXT:             re: 1.0f64,
// CHECK-NEXT:             im: -2.0f64,
// CHECK-NEXT:         }
// CHECK-NEXT:             .im) as i32;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {}", (unsafe { nonzero_double }) as i32, (unsafe { zero_double }) as
// CHECK-NEXT:         i32, (unsafe { imag_only_float }) as i32, unsafe { equal }, unsafe { differ }
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
