extern void abort(void);

typedef float v4flt __attribute__((vector_size(16)));

void __attribute__((noinline)) foo(float *dst, float **src, int a, int n) {
  int      i, j;
  int      z = sizeof(v4flt) / sizeof(float);
  unsigned m = sizeof(v4flt) - 1;

  for (j = 0; j < n && (((unsigned long)dst + j) & m); ++j) {
    float t = src[0][j];
    for (i = 1; i < a; ++i)
      t += src[i][j];
    dst[j] = t;
  }

  for (; j < (n - (4 * z - 1)); j += 4 * z) {
    v4flt t0 = *(v4flt *)(src[0] + j + 0 * z);
    v4flt t1 = *(v4flt *)(src[0] + j + 1 * z);
    v4flt t2 = *(v4flt *)(src[0] + j + 2 * z);
    v4flt t3 = *(v4flt *)(src[0] + j + 3 * z);
    for (i = 1; i < a; ++i) {
      t0 += *(v4flt *)(src[i] + j + 0 * z);
      t1 += *(v4flt *)(src[i] + j + 1 * z);
      t2 += *(v4flt *)(src[i] + j + 2 * z);
      t3 += *(v4flt *)(src[i] + j + 3 * z);
    }
    *(v4flt *)(dst + j + 0 * z) = t0;
    *(v4flt *)(dst + j + 1 * z) = t1;
    *(v4flt *)(dst + j + 2 * z) = t2;
    *(v4flt *)(dst + j + 3 * z) = t3;
  }
  for (; j < n; ++j) {
    float t = src[0][j];
    for (i = 1; i < a; ++i)
      t += src[i][j];
    dst[j] = t;
  }
}

float buffer[64];

int main(void) {
  int    i;
  float *dst, *src[2];
  char  *cptr;

  cptr    = (char *)buffer;
  cptr   += (-(long int)buffer & (16 * sizeof(float) - 1));
  dst     = (float *)cptr;
  src[0]  = dst + 16;
  src[1]  = dst + 32;
  for (i = 0; i < 16; ++i) {
    src[0][i] = (float)i + 11 * (float)i;
    src[1][i] = (float)i + 12 * (float)i;
  }
  foo(dst, src, 2, 16);
  for (i = 0; i < 16; ++i) {
    float e = (float)i + 11 * (float)i + (float)i + 12 * (float)i;
    if (dst[i] != e)
      abort();
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn foo(mut dst: *mut f32, mut src: *mut *mut f32, mut a: i32, mut n: i32) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut z: i32 = (((16 as u64) / (4 as u64)) as u32) as i32;
// CHECK-NEXT:     let mut m: u32 = (16 as u64).wrapping_sub(((1 as i32) as i64) as u64) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while j < n
// CHECK-NEXT:         && (dst as u64).wrapping_add((j as i64) as u64) & (m as u64) != (0 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut t: f32 = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { *unsafe { src.offset((0 as i32) as isize) } }.offset(j as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:         i = 1 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < a {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: f32 = t;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: f32 = [[V_0]]
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { *unsafe { src.offset(i as isize) } }.offset(j as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 };
// CHECK-NEXT:             t = [[V_1]];
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             i = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { dst.offset(j as isize) } = t;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         j = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while j < n - ((4 as i32) * z - (1 as i32)) {
// CHECK-NEXT:         let mut t0: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:             *((unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { *unsafe { src.offset((0 as i32) as isize) } }
// CHECK-NEXT:                         .offset(j as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset(((0 as i32) * z) as isize)
// CHECK-NEXT:             }) as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut t1: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:             *((unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { *unsafe { src.offset((0 as i32) as isize) } }
// CHECK-NEXT:                         .offset(j as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset(((1 as i32) * z) as isize)
// CHECK-NEXT:             }) as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut t2: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:             *((unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { *unsafe { src.offset((0 as i32) as isize) } }
// CHECK-NEXT:                         .offset(j as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset(((2 as i32) * z) as isize)
// CHECK-NEXT:             }) as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut t3: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:             *((unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     unsafe { *unsafe { src.offset((0 as i32) as isize) } }
// CHECK-NEXT:                         .offset(j as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset(((3 as i32) * z) as isize)
// CHECK-NEXT:             }) as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:         };
// CHECK-NEXT:         i = 1 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while i < a {
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: std::simd::Simd<f32, 4> = t0;
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: std::simd::Simd<f32, 4> = [[V_6]]
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     *((unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe { *unsafe { src.offset(i as isize) } }
// CHECK-NEXT:                                 .offset(j as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                             .offset(((0 as i32) * z) as isize)
// CHECK-NEXT:                     }) as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                 };
// CHECK-NEXT:             t0 = [[V_7]];
// CHECK-NEXT:             let mut [[V_8:__v[0-9]+]]: std::simd::Simd<f32, 4> = t1;
// CHECK-NEXT:             let mut [[V_9:__v[0-9]+]]: std::simd::Simd<f32, 4> = [[V_8]]
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     *((unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe { *unsafe { src.offset(i as isize) } }
// CHECK-NEXT:                                 .offset(j as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                             .offset(((1 as i32) * z) as isize)
// CHECK-NEXT:                     }) as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                 };
// CHECK-NEXT:             t1 = [[V_9]];
// CHECK-NEXT:             let mut [[V_10:__v[0-9]+]]: std::simd::Simd<f32, 4> = t2;
// CHECK-NEXT:             let mut [[V_11:__v[0-9]+]]: std::simd::Simd<f32, 4> = [[V_10]]
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     *((unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe { *unsafe { src.offset(i as isize) } }
// CHECK-NEXT:                                 .offset(j as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                             .offset(((2 as i32) * z) as isize)
// CHECK-NEXT:                     }) as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                 };
// CHECK-NEXT:             t2 = [[V_11]];
// CHECK-NEXT:             let mut [[V_12:__v[0-9]+]]: std::simd::Simd<f32, 4> = t3;
// CHECK-NEXT:             let mut [[V_13:__v[0-9]+]]: std::simd::Simd<f32, 4> = [[V_12]]
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     *((unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             unsafe { *unsafe { src.offset(i as isize) } }
// CHECK-NEXT:                                 .offset(j as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                             .offset(((3 as i32) * z) as isize)
// CHECK-NEXT:                     }) as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                 };
// CHECK-NEXT:             t3 = [[V_13]];
// CHECK-NEXT:             let mut [[V_14:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_15:__v[0-9]+]]: i32 = [[V_14]] + (1 as i32);
// CHECK-NEXT:             i = [[V_15]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *((unsafe {
// CHECK-NEXT:                 unsafe { dst.offset(j as isize) }.offset(((0 as i32) * z) as isize)
// CHECK-NEXT:             }) as *mut std::simd::Simd<f32, 4>) = t0;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *((unsafe {
// CHECK-NEXT:                 unsafe { dst.offset(j as isize) }.offset(((1 as i32) * z) as isize)
// CHECK-NEXT:             }) as *mut std::simd::Simd<f32, 4>) = t1;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *((unsafe {
// CHECK-NEXT:                 unsafe { dst.offset(j as isize) }.offset(((2 as i32) * z) as isize)
// CHECK-NEXT:             }) as *mut std::simd::Simd<f32, 4>) = t2;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *((unsafe {
// CHECK-NEXT:                 unsafe { dst.offset(j as isize) }.offset(((3 as i32) * z) as isize)
// CHECK-NEXT:             }) as *mut std::simd::Simd<f32, 4>) = t3;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_16:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_17:__v[0-9]+]]: i32 = [[V_16]] + (4 as i32) * z;
// CHECK-NEXT:         j = [[V_17]];
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while j < n {
// CHECK-NEXT:         let mut t: f32 = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { *unsafe { src.offset((0 as i32) as isize) } }.offset(j as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:         i = 1 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while i < a {
// CHECK-NEXT:             let mut [[V_18:__v[0-9]+]]: f32 = t;
// CHECK-NEXT:             let mut [[V_19:__v[0-9]+]]: f32 = [[V_18]]
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { *unsafe { src.offset(i as isize) } }.offset(j as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 };
// CHECK-NEXT:             t = [[V_19]];
// CHECK-NEXT:             let mut [[V_20:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_21:__v[0-9]+]]: i32 = [[V_20]] + (1 as i32);
// CHECK-NEXT:             i = [[V_21]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { dst.offset(j as isize) } = t;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_22:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_23:__v[0-9]+]]: i32 = [[V_22]] + (1 as i32);
// CHECK-NEXT:         j = [[V_23]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut buffer: __SlateAlign16<[f32; 64]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut dst: *mut f32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut src: __SlateAlign16<[*mut f32; 2]> = __SlateAlign16([0 as *mut f32; 2]);
// CHECK-NEXT:     let mut cptr: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     cptr = (unsafe { std::ptr::addr_of_mut!(buffer.0) as *mut f32 }) as *mut i8;
// CHECK-NEXT:     let mut [[V_24:__v[0-9]+]]: *mut i8 = cptr;
// CHECK-NEXT:     let mut [[V_25:__v[0-9]+]]: *mut i8 = unsafe {
// CHECK-NEXT:         [[V_24]]
// CHECK-NEXT:             .offset(
// CHECK-NEXT:                 ((-((unsafe { std::ptr::addr_of_mut!(buffer.0) as *mut f32 }) as i64)
// CHECK-NEXT:                     as u64)
// CHECK-NEXT:                     & (((16 as i32) as i64) as u64)
// CHECK-NEXT:                         .wrapping_mul(4 as u64)
// CHECK-NEXT:                         .wrapping_sub(((1 as i32) as i64) as u64)) as isize,
// CHECK-NEXT:             )
// CHECK-NEXT:     };
// CHECK-NEXT:     cptr = [[V_25]];
// CHECK-NEXT:     dst = cptr as *mut f32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (src.0.as_mut_ptr() as *mut *mut f32).offset((0 as i32) as isize) } = unsafe {
// CHECK-NEXT:             dst.offset((16 as i32) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (src.0.as_mut_ptr() as *mut *mut f32).offset((1 as i32) as isize) } = unsafe {
// CHECK-NEXT:             dst.offset((32 as i32) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: while i < (16 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (src.0.as_mut_ptr() as *mut *mut f32).offset((0 as i32) as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset(i as isize)
// CHECK-NEXT:             } = (i as f32) + ((11 as i32) as f32) * (i as f32);
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (src.0.as_mut_ptr() as *mut *mut f32).offset((1 as i32) as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset(i as isize)
// CHECK-NEXT:             } = (i as f32) + ((12 as i32) as f32) * (i as f32);
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_26:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_27:__v[0-9]+]]: i32 = [[V_26]] + (1 as i32);
// CHECK-NEXT:         i = [[V_27]];
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(dst, src.0.as_mut_ptr() as *mut *mut f32, 2 as i32, 16 as i32);
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: while i < (16 as i32) {
// CHECK-NEXT:         let mut e: f32 = (i as f32) + ((11 as i32) as f32) * (i as f32) + (i as f32)
// CHECK-NEXT:             + ((12 as i32) as f32) * (i as f32);
// CHECK-NEXT:         if (unsafe { *unsafe { dst.offset(i as isize) } }) != e {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_28:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_29:__v[0-9]+]]: i32 = [[V_28]] + (1 as i32);
// CHECK-NEXT:         i = [[V_29]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
