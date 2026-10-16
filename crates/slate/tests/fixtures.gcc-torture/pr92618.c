/* PR tree-optimization/92618 */

typedef long long __m128i
    __attribute__((__vector_size__(2 * sizeof(long long)), __may_alias__));

double             a[4];
unsigned long long b[4];

__attribute__((noipa)) __m128i bar(void) {
  static int cnt;
  cnt += 2;
  return (__m128i){cnt, cnt + 1};
}

#if __SIZEOF_LONG_LONG__ == __SIZEOF_DOUBLE__
typedef double __m128d
    __attribute__((__vector_size__(2 * sizeof(double)), __may_alias__));

__attribute__((noipa)) __m128i qux(void) {
  static double cnt;
  cnt += 2.0;
  return (__m128i)(__m128d){cnt, cnt + 1.0};
}
#endif

void foo(unsigned long long *x) {
  __m128i c         = bar();
  __m128i d         = bar();
  *(__m128i *)&b[0] = c;
  *(__m128i *)&b[2] = d;
  *x                = b[0] + b[1] + b[2] + b[3];
}

void baz(double *x) {
#if __SIZEOF_LONG_LONG__ == __SIZEOF_DOUBLE__
  __m128i c         = qux();
  __m128i d         = qux();
  *(__m128i *)&a[0] = c;
  *(__m128i *)&a[2] = d;
  *x                = a[0] + a[1] + a[2] + a[3];
#endif
}

int main() {
  unsigned long long c = 0;
  foo(&c);
  if (c != 2 + 3 + 4 + 5)
    __builtin_abort();
#if __SIZEOF_LONG_LONG__ == __SIZEOF_DOUBLE__
  double d = 0.0;
  baz(&d);
  if (d != 2.0 + 3.0 + 4.0 + 5.0)
    __builtin_abort();
#endif
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/92618
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: __SlateAlign16<[f64; 4]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: __SlateAlign16<[u64; 4]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn bar() -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<i64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { [[CNT_0:cnt_[0-9]+]] };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (2 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         [[CNT_0]] = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 (unsafe { [[CNT_0]] }) as i64,
// CHECK-NEXT:                 ((unsafe { [[CNT_0]] }) + (1 as i32)) as i64,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut [[CNT_0]]: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn qux() -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: f64 = unsafe { [[CNT_1:cnt_[0-9]+]] };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: f64 = [[V_2]] + 2.0f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         [[CNT_1]] = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<f64, 2>, std::simd::Simd<i64, 2>>(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_1]] =
// CHECK-NEXT:                     std::simd::Simd::from_array([unsafe { [[CNT_1]] }, (unsafe { [[CNT_1]] }) + 1.0f64]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut [[CNT_1]]: f64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: &mut u64) {
// CHECK-NEXT:     let mut c: std::simd::Simd<i64, 2> = bar();
// CHECK-NEXT:     let mut d: std::simd::Simd<i64, 2> = bar();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *((unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(b.0) as *mut u64 }.offset((0 as i32) as isize)
// CHECK-NEXT:         }) as *mut std::simd::Simd<i64, 2>) = c;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *((unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(b.0) as *mut u64 }.offset((2 as i32) as isize)
// CHECK-NEXT:         }) as *mut std::simd::Simd<i64, 2>) = d;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(x as *mut u64) = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(b.0) as *mut u64 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         .wrapping_add(unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(b.0) as *mut u64 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:         .wrapping_add(unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(b.0) as *mut u64 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:         .wrapping_add(unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(b.0) as *mut u64 }.offset((3 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         });
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut x: &mut f64) {
// CHECK-NEXT:     let mut c: std::simd::Simd<i64, 2> = qux();
// CHECK-NEXT:     let mut d: std::simd::Simd<i64, 2> = qux();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *((unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.0) as *mut f64 }.offset((0 as i32) as isize)
// CHECK-NEXT:         }) as *mut std::simd::Simd<i64, 2>) = c;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *((unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a.0) as *mut f64 }.offset((2 as i32) as isize)
// CHECK-NEXT:         }) as *mut std::simd::Simd<i64, 2>) = d;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(x as *mut f64) = (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a.0) as *mut f64 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) + unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a.0) as *mut f64 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         } + unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a.0) as *mut f64 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         } + unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a.0) as *mut f64 }.offset((3 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut c: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(c)) });
// CHECK-NEXT:     if c != ((((2 as i32) + (3 as i32) + (4 as i32) + (5 as i32)) as i64) as u64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut d: f64 = 0.0f64;
// CHECK-NEXT:     baz(unsafe { &mut (*std::ptr::addr_of_mut!(d)) });
// CHECK-NEXT:     if d != 2.0f64 + 3.0f64 + 4.0f64 + 5.0f64 {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
