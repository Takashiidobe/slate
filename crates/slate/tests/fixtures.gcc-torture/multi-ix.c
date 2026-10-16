/* { dg-add-options stack_size } */

/* Test for a reload bug:
   if you have a memory reference using the indexed addressing
   mode, and the base address is a pseudo containing an address in the frame
   and this pseudo fails to get a hard register, we end up with a double PLUS,
   so the frame address gets reloaded.  Now, when the index got a hard register,
   and it dies in this insn, push_reload will consider that hard register as
   a reload register, and disregrad overlaps with rld[n_reloads].in .  That is
   fine as long as the add can be done with a single insn, but when the
   constant is so large that it has to be reloaded into a register first,
   that clobbers the index.  */

#include <stdarg.h>

void abort(void);
void exit(int);

#ifdef STACK_SIZE
/* We need to be careful that we don't blow our stack.  Function f, in the
   worst case, needs to fit on the stack:

   * 40 int[CHUNK] arrays;
   * ~40 ints;
   * ~40 pointers for stdarg passing.

   Subtract the last two off STACK_SIZE and figure out what the maximum
   chunk size can be.  We make the last bit conservative to account for
   register saves and other processor-dependent saving.  Limit the
   chunk size to some sane values.  */

#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

#define CHUNK                                                                  \
  MIN(500,                                                                     \
      (MAX(1, (signed)(STACK_SIZE - 40 * sizeof(int) - 256 * sizeof(void *)) / \
                  (signed)(40 * sizeof(int)))))
#else
#define CHUNK 500
#endif

void s(int, ...);
void z(int, ...);
void c(int, ...);

typedef int l[CHUNK];

void f(int n) {
  int i;
  l   a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;
  l   a10, a11, a12, a13, a14, a15, a16, a17, a18, a19;
  l   a20, a21, a22, a23, a24, a25, a26, a27, a28, a29;
  l   a30, a31, a32, a33, a34, a35, a36, a37, a38, a39;
  int i0, i1, i2, i3, i4, i5, i6, i7, i8, i9;
  int i10, i11, i12, i13, i14, i15, i16, i17, i18, i19;
  int i20, i21, i22, i23, i24, i25, i26, i27, i28, i29;
  int i30, i31, i32, i33, i34, i35, i36, i37, i38, i39;

  for (i = 0; i < n; i++) {
    s(40, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15,
      a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30,
      a31, a32, a33, a34, a35, a36, a37, a38, a39);
    i0  = a0[0];
    i1  = a1[0];
    i2  = a2[0];
    i3  = a3[0];
    i4  = a4[0];
    i5  = a5[0];
    i6  = a6[0];
    i7  = a7[0];
    i8  = a8[0];
    i9  = a9[0];
    i10 = a10[0];
    i11 = a11[0];
    i12 = a12[0];
    i13 = a13[0];
    i14 = a14[0];
    i15 = a15[0];
    i16 = a16[0];
    i17 = a17[0];
    i18 = a18[0];
    i19 = a19[0];
    i20 = a20[0];
    i21 = a21[0];
    i22 = a22[0];
    i23 = a23[0];
    i24 = a24[0];
    i25 = a25[0];
    i26 = a26[0];
    i27 = a27[0];
    i28 = a28[0];
    i29 = a29[0];
    i30 = a30[0];
    i31 = a31[0];
    i32 = a32[0];
    i33 = a33[0];
    i34 = a34[0];
    i35 = a35[0];
    i36 = a36[0];
    i37 = a37[0];
    i38 = a38[0];
    i39 = a39[0];
    z(40, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15,
      a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30,
      a31, a32, a33, a34, a35, a36, a37, a38, a39);
    a0[i0]   = i0;
    a1[i1]   = i1;
    a2[i2]   = i2;
    a3[i3]   = i3;
    a4[i4]   = i4;
    a5[i5]   = i5;
    a6[i6]   = i6;
    a7[i7]   = i7;
    a8[i8]   = i8;
    a9[i9]   = i9;
    a10[i10] = i10;
    a11[i11] = i11;
    a12[i12] = i12;
    a13[i13] = i13;
    a14[i14] = i14;
    a15[i15] = i15;
    a16[i16] = i16;
    a17[i17] = i17;
    a18[i18] = i18;
    a19[i19] = i19;
    a20[i20] = i20;
    a21[i21] = i21;
    a22[i22] = i22;
    a23[i23] = i23;
    a24[i24] = i24;
    a25[i25] = i25;
    a26[i26] = i26;
    a27[i27] = i27;
    a28[i28] = i28;
    a29[i29] = i29;
    a30[i30] = i30;
    a31[i31] = i31;
    a32[i32] = i32;
    a33[i33] = i33;
    a34[i34] = i34;
    a35[i35] = i35;
    a36[i36] = i36;
    a37[i37] = i37;
    a38[i38] = i38;
    a39[i39] = i39;
    c(40, a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15,
      a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30,
      a31, a32, a33, a34, a35, a36, a37, a38, a39);
  }
}

int main() {
  /* CHUNK needs to be at least 40 to avoid stack corruption,
     since index variable i0 in "a[i0] = i0" equals 39.  */
  if (CHUNK < 40)
    exit(0);

  f(1);
  exit(0);
}

void s(int n, ...) {
  va_list list;

  va_start(list, n);
  while (n--) {
    int *a = va_arg(list, int *);
    a[0]   = n;
  }
  va_end(list);
}

void z(int n, ...) {
  va_list list;

  va_start(list, n);
  while (n--) {
    int *a = va_arg(list, int *);
    __builtin_memset(a, 0, sizeof(l));
  }
  va_end(list);
}

void c(int n, ...) {
  va_list list;

  va_start(list, n);
  while (n--) {
    int *a = va_arg(list, int *);
    if (a[n] != n)
      abort();
  }
  va_end(list);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-add-options stack_size }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_1:__v[0-9]+]]: *mut (), [[V_2:__v[0-9]+]]: i32, [[V_3:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: // Test for a reload bug:
// CHECK-NEXT: // if you have a memory reference using the indexed addressing
// CHECK-NEXT: // mode, and the base address is a pseudo containing an address in the frame
// CHECK-NEXT: // and this pseudo fails to get a hard register, we end up with a double PLUS,
// CHECK-NEXT: // so the frame address gets reloaded.  Now, when the index got a hard register,
// CHECK-NEXT: // and it dies in this insn, push_reload will consider that hard register as
// CHECK-NEXT: // a reload register, and disregrad overlaps with rld[n_reloads].in .  That is
// CHECK-NEXT: // fine as long as the add can be done with a single insn, but when the
// CHECK-NEXT: // constant is so large that it has to be reloaded into a register first,
// CHECK-NEXT: // that clobbers the index.
// CHECK-NEXT: unsafe extern "C-unwind" fn z(mut n: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut list: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     list = __va_args.clone();
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] - (1 as i32);
// CHECK-NEXT:         n = [[V_5]];
// CHECK-NEXT:         if !([[V_4]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut a: *mut i32 = unsafe { list.next_arg::<*mut i32>() };
// CHECK-NEXT:         unsafe { __builtin_memset(a as *mut (), 0 as i32, 2000 as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut n: i32) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut a0: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a1: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a2: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a3: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a4: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a5: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a6: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a7: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a8: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a9: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a10: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a11: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a12: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a13: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a14: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a15: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a16: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a17: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a18: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a19: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a20: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a21: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a22: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a23: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a24: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a25: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a26: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a27: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a28: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a29: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a30: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a31: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a32: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a33: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a34: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a35: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a36: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a37: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a38: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut a39: __SlateAlign16<[i32; 500]> = __SlateAlign16([0 as i32; 500]);
// CHECK-NEXT:     let mut i0: i32 = 0 as i32;
// CHECK-NEXT:     let mut i1: i32 = 0 as i32;
// CHECK-NEXT:     let mut i2: i32 = 0 as i32;
// CHECK-NEXT:     let mut i3: i32 = 0 as i32;
// CHECK-NEXT:     let mut i4: i32 = 0 as i32;
// CHECK-NEXT:     let mut i5: i32 = 0 as i32;
// CHECK-NEXT:     let mut i6: i32 = 0 as i32;
// CHECK-NEXT:     let mut i7: i32 = 0 as i32;
// CHECK-NEXT:     let mut i8: i32 = 0 as i32;
// CHECK-NEXT:     let mut i9: i32 = 0 as i32;
// CHECK-NEXT:     let mut i10: i32 = 0 as i32;
// CHECK-NEXT:     let mut i11: i32 = 0 as i32;
// CHECK-NEXT:     let mut i12: i32 = 0 as i32;
// CHECK-NEXT:     let mut i13: i32 = 0 as i32;
// CHECK-NEXT:     let mut i14: i32 = 0 as i32;
// CHECK-NEXT:     let mut i15: i32 = 0 as i32;
// CHECK-NEXT:     let mut i16: i32 = 0 as i32;
// CHECK-NEXT:     let mut i17: i32 = 0 as i32;
// CHECK-NEXT:     let mut i18: i32 = 0 as i32;
// CHECK-NEXT:     let mut i19: i32 = 0 as i32;
// CHECK-NEXT:     let mut i20: i32 = 0 as i32;
// CHECK-NEXT:     let mut i21: i32 = 0 as i32;
// CHECK-NEXT:     let mut i22: i32 = 0 as i32;
// CHECK-NEXT:     let mut i23: i32 = 0 as i32;
// CHECK-NEXT:     let mut i24: i32 = 0 as i32;
// CHECK-NEXT:     let mut i25: i32 = 0 as i32;
// CHECK-NEXT:     let mut i26: i32 = 0 as i32;
// CHECK-NEXT:     let mut i27: i32 = 0 as i32;
// CHECK-NEXT:     let mut i28: i32 = 0 as i32;
// CHECK-NEXT:     let mut i29: i32 = 0 as i32;
// CHECK-NEXT:     let mut i30: i32 = 0 as i32;
// CHECK-NEXT:     let mut i31: i32 = 0 as i32;
// CHECK-NEXT:     let mut i32: i32 = 0 as i32;
// CHECK-NEXT:     let mut i33: i32 = 0 as i32;
// CHECK-NEXT:     let mut i34: i32 = 0 as i32;
// CHECK-NEXT:     let mut i35: i32 = 0 as i32;
// CHECK-NEXT:     let mut i36: i32 = 0 as i32;
// CHECK-NEXT:     let mut i37: i32 = 0 as i32;
// CHECK-NEXT:     let mut i38: i32 = 0 as i32;
// CHECK-NEXT:     let mut i39: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < n {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             s(
// CHECK-NEXT:                 40 as i32,
// CHECK-NEXT:                 a0.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a1.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a2.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a3.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a4.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a5.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a6.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a7.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a8.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a9.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a10.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a11.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a12.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a13.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a14.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a15.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a16.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a17.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a18.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a19.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a20.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a21.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a22.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a23.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a24.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a25.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a26.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a27.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a28.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a29.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a30.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a31.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a32.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a33.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a34.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a35.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a36.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a37.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a38.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a39.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         i0 = unsafe { *unsafe { (a0.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i1 = unsafe { *unsafe { (a1.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i2 = unsafe { *unsafe { (a2.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i3 = unsafe { *unsafe { (a3.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i4 = unsafe { *unsafe { (a4.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i5 = unsafe { *unsafe { (a5.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i6 = unsafe { *unsafe { (a6.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i7 = unsafe { *unsafe { (a7.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i8 = unsafe { *unsafe { (a8.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i9 = unsafe { *unsafe { (a9.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i10 = unsafe { *unsafe { (a10.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i11 = unsafe { *unsafe { (a11.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i12 = unsafe { *unsafe { (a12.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i13 = unsafe { *unsafe { (a13.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i14 = unsafe { *unsafe { (a14.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i15 = unsafe { *unsafe { (a15.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i16 = unsafe { *unsafe { (a16.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i17 = unsafe { *unsafe { (a17.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i18 = unsafe { *unsafe { (a18.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i19 = unsafe { *unsafe { (a19.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i20 = unsafe { *unsafe { (a20.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i21 = unsafe { *unsafe { (a21.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i22 = unsafe { *unsafe { (a22.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i23 = unsafe { *unsafe { (a23.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i24 = unsafe { *unsafe { (a24.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i25 = unsafe { *unsafe { (a25.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i26 = unsafe { *unsafe { (a26.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i27 = unsafe { *unsafe { (a27.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i28 = unsafe { *unsafe { (a28.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i29 = unsafe { *unsafe { (a29.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i30 = unsafe { *unsafe { (a30.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i31 = unsafe { *unsafe { (a31.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i32 = unsafe { *unsafe { (a32.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i33 = unsafe { *unsafe { (a33.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i34 = unsafe { *unsafe { (a34.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i35 = unsafe { *unsafe { (a35.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i36 = unsafe { *unsafe { (a36.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i37 = unsafe { *unsafe { (a37.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i38 = unsafe { *unsafe { (a38.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         i39 = unsafe { *unsafe { (a39.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             z(
// CHECK-NEXT:                 40 as i32,
// CHECK-NEXT:                 a0.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a1.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a2.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a3.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a4.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a5.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a6.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a7.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a8.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a9.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a10.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a11.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a12.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a13.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a14.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a15.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a16.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a17.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a18.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a19.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a20.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a21.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a22.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a23.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a24.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a25.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a26.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a27.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a28.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a29.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a30.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a31.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a32.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a33.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a34.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a35.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a36.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a37.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a38.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a39.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a0.0.as_mut_ptr() as *mut i32).offset(i0 as isize) } = i0;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a1.0.as_mut_ptr() as *mut i32).offset(i1 as isize) } = i1;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a2.0.as_mut_ptr() as *mut i32).offset(i2 as isize) } = i2;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a3.0.as_mut_ptr() as *mut i32).offset(i3 as isize) } = i3;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a4.0.as_mut_ptr() as *mut i32).offset(i4 as isize) } = i4;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a5.0.as_mut_ptr() as *mut i32).offset(i5 as isize) } = i5;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a6.0.as_mut_ptr() as *mut i32).offset(i6 as isize) } = i6;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a7.0.as_mut_ptr() as *mut i32).offset(i7 as isize) } = i7;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a8.0.as_mut_ptr() as *mut i32).offset(i8 as isize) } = i8;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a9.0.as_mut_ptr() as *mut i32).offset(i9 as isize) } = i9;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a10.0.as_mut_ptr() as *mut i32).offset(i10 as isize) } = i10;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a11.0.as_mut_ptr() as *mut i32).offset(i11 as isize) } = i11;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a12.0.as_mut_ptr() as *mut i32).offset(i12 as isize) } = i12;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a13.0.as_mut_ptr() as *mut i32).offset(i13 as isize) } = i13;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a14.0.as_mut_ptr() as *mut i32).offset(i14 as isize) } = i14;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a15.0.as_mut_ptr() as *mut i32).offset(i15 as isize) } = i15;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a16.0.as_mut_ptr() as *mut i32).offset(i16 as isize) } = i16;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a17.0.as_mut_ptr() as *mut i32).offset(i17 as isize) } = i17;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a18.0.as_mut_ptr() as *mut i32).offset(i18 as isize) } = i18;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a19.0.as_mut_ptr() as *mut i32).offset(i19 as isize) } = i19;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a20.0.as_mut_ptr() as *mut i32).offset(i20 as isize) } = i20;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a21.0.as_mut_ptr() as *mut i32).offset(i21 as isize) } = i21;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a22.0.as_mut_ptr() as *mut i32).offset(i22 as isize) } = i22;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a23.0.as_mut_ptr() as *mut i32).offset(i23 as isize) } = i23;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a24.0.as_mut_ptr() as *mut i32).offset(i24 as isize) } = i24;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a25.0.as_mut_ptr() as *mut i32).offset(i25 as isize) } = i25;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a26.0.as_mut_ptr() as *mut i32).offset(i26 as isize) } = i26;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a27.0.as_mut_ptr() as *mut i32).offset(i27 as isize) } = i27;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a28.0.as_mut_ptr() as *mut i32).offset(i28 as isize) } = i28;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a29.0.as_mut_ptr() as *mut i32).offset(i29 as isize) } = i29;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a30.0.as_mut_ptr() as *mut i32).offset(i30 as isize) } = i30;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a31.0.as_mut_ptr() as *mut i32).offset(i31 as isize) } = i31;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a32.0.as_mut_ptr() as *mut i32).offset(i32 as isize) } = i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a33.0.as_mut_ptr() as *mut i32).offset(i33 as isize) } = i33;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a34.0.as_mut_ptr() as *mut i32).offset(i34 as isize) } = i34;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a35.0.as_mut_ptr() as *mut i32).offset(i35 as isize) } = i35;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a36.0.as_mut_ptr() as *mut i32).offset(i36 as isize) } = i36;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a37.0.as_mut_ptr() as *mut i32).offset(i37 as isize) } = i37;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a38.0.as_mut_ptr() as *mut i32).offset(i38 as isize) } = i38;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (a39.0.as_mut_ptr() as *mut i32).offset(i39 as isize) } = i39;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c(
// CHECK-NEXT:                 40 as i32,
// CHECK-NEXT:                 a0.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a1.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a2.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a3.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a4.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a5.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a6.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a7.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a8.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a9.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a10.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a11.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a12.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a13.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a14.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a15.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a16.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a17.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a18.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a19.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a20.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a21.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a22.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a23.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a24.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a25.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a26.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a27.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a28.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a29.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a30.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a31.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a32.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a33.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a34.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a35.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a36.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a37.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a38.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 a39.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:         i = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     // CHUNK needs to be at least 40 to avoid stack corruption,
// CHECK-NEXT:     // since index variable i0 in "a[i0] = i0" equals 39.
// CHECK-NEXT:     if (500 as i32) < (40 as i32) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     f(1 as i32);
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn s(mut n: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut list: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     list = __va_args.clone();
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] - (1 as i32);
// CHECK-NEXT:         n = [[V_9]];
// CHECK-NEXT:         if !([[V_8]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut a: *mut i32 = unsafe { list.next_arg::<*mut i32>() };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { a.offset((0 as i32) as isize) } = n;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn c(mut n: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut list: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     list = __va_args.clone();
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: i32 = [[V_10]] - (1 as i32);
// CHECK-NEXT:         n = [[V_11]];
// CHECK-NEXT:         if !([[V_10]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut a: *mut i32 = unsafe { list.next_arg::<*mut i32>() };
// CHECK-NEXT:         if (unsafe { *unsafe { a.offset(n as isize) } }) != n {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
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
