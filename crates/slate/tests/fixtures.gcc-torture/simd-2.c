/*
   Purpose: Test generic SIMD support, V8HImode.  This test should work
   regardless of if the target has SIMD instructions.
*/

void abort(void);
void exit(int);

typedef short __attribute__((vector_size(16))) vecint;

vecint i = {150, 100, 150, 200, 0, 0, 0, 0};
vecint j = {10, 13, 20, 30, 1, 1, 1, 1};
vecint k;

union {
  vecint v;
  short  i[8];
} res;

/* This should go away once we can use == and != on vector types.  */
void verify(int a1, int a2, int a3, int a4, int b1, int b2, int b3, int b4) {
  if (a1 != b1 || a2 != b2 || a3 != b3 || a4 != b4)
    abort();
}

int main() {
  k     = i + j;
  res.v = k;

  verify(res.i[0], res.i[1], res.i[2], res.i[3], 160, 113, 170, 230);

  k     = i * j;
  res.v = k;

  verify(res.i[0], res.i[1], res.i[2], res.i[3], 1500, 1300, 3000, 6000);

  k     = i / j;
  res.v = k;

  verify(res.i[0], res.i[1], res.i[2], res.i[3], 15, 7, 7, 6);

  k     = i & j;
  res.v = k;

  verify(res.i[0], res.i[1], res.i[2], res.i[3], 2, 4, 20, 8);

  k     = i | j;
  res.v = k;

  verify(res.i[0], res.i[1], res.i[2], res.i[3], 158, 109, 150, 222);

  k     = i ^ j;
  res.v = k;

  verify(res.i[0], res.i[1], res.i[2], res.i[3], 156, 105, 130, 214);

  k     = -i;
  res.v = k;
  verify(res.i[0], res.i[1], res.i[2], res.i[3], -150, -100, -150, -200);

  k     = ~i;
  res.v = k;
  verify(res.i[0], res.i[1], res.i[2], res.i[3], -151, -101, -151, -201);

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Purpose: Test generic SIMD support, V8HImode.  This test should work
// CHECK-NEXT: //! regardless of if the target has SIMD instructions.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: std::simd::Simd<i16, 8> = std::simd::Simd::from_array([
// CHECK-NEXT:     (150 as i32) as i16,
// CHECK-NEXT:     (100 as i32) as i16,
// CHECK-NEXT:     (150 as i32) as i16,
// CHECK-NEXT:     (200 as i32) as i16,
// CHECK-NEXT:     (0 as i32) as i16,
// CHECK-NEXT:     (0 as i32) as i16,
// CHECK-NEXT:     (0 as i32) as i16,
// CHECK-NEXT:     (0 as i32) as i16,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut j: std::simd::Simd<i16, 8> = std::simd::Simd::from_array([
// CHECK-NEXT:     (10 as i32) as i16,
// CHECK-NEXT:     (13 as i32) as i16,
// CHECK-NEXT:     (20 as i32) as i16,
// CHECK-NEXT:     (30 as i32) as i16,
// CHECK-NEXT:     (1 as i32) as i16,
// CHECK-NEXT:     (1 as i32) as i16,
// CHECK-NEXT:     (1 as i32) as i16,
// CHECK-NEXT:     (1 as i32) as i16,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut k: std::simd::Simd<i16, 8> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     v: std::simd::Simd<i16, 8>,
// CHECK-NEXT:     i: [i16; 8],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut res: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: /// This should go away once we can use == and != on vector types.
// CHECK-NEXT: fn verify(
// CHECK-NEXT:     mut a1: i32,
// CHECK-NEXT:     mut a2: i32,
// CHECK-NEXT:     mut a3: i32,
// CHECK-NEXT:     mut a4: i32,
// CHECK-NEXT:     mut b1: i32,
// CHECK-NEXT:     mut b2: i32,
// CHECK-NEXT:     mut b3: i32,
// CHECK-NEXT:     mut b4: i32,
// CHECK-NEXT: ) {
// CHECK-NEXT:     if a1 != b1 || a2 != b2 || a3 != b3 || a4 != b4 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         k = (unsafe { i }) + unsafe { j };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = unsafe { k };
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((3 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         160 as i32,
// CHECK-NEXT:         113 as i32,
// CHECK-NEXT:         170 as i32,
// CHECK-NEXT:         230 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         k = (unsafe { i }) * unsafe { j };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = unsafe { k };
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((3 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         1500 as i32,
// CHECK-NEXT:         1300 as i32,
// CHECK-NEXT:         3000 as i32,
// CHECK-NEXT:         6000 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         k = (unsafe { i }) / unsafe { j };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = unsafe { k };
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((3 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         15 as i32,
// CHECK-NEXT:         7 as i32,
// CHECK-NEXT:         7 as i32,
// CHECK-NEXT:         6 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         k = (unsafe { i }) & unsafe { j };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = unsafe { k };
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((3 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         20 as i32,
// CHECK-NEXT:         8 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         k = (unsafe { i }) | unsafe { j };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = unsafe { k };
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((3 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         158 as i32,
// CHECK-NEXT:         109 as i32,
// CHECK-NEXT:         150 as i32,
// CHECK-NEXT:         222 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         k = (unsafe { i }) ^ unsafe { j };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = unsafe { k };
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((3 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         156 as i32,
// CHECK-NEXT:         105 as i32,
// CHECK-NEXT:         130 as i32,
// CHECK-NEXT:         214 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         k = -unsafe { i };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = unsafe { k };
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((3 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         -(150 as i32),
// CHECK-NEXT:         -(100 as i32),
// CHECK-NEXT:         -(150 as i32),
// CHECK-NEXT:         -(200 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         k = !unsafe { i };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = unsafe { k };
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i16 }.offset((3 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32,
// CHECK-NEXT:         -(151 as i32),
// CHECK-NEXT:         -(101 as i32),
// CHECK-NEXT:         -(151 as i32),
// CHECK-NEXT:         -(201 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
