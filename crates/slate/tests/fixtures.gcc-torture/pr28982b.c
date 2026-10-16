/* { dg-require-stack-size "0x80100" } */

/* Like pr28982a.c, but with the spill slots outside the range of
   a single sp-based load on ARM.  This test tests for cases where
   the addresses in the base and index reloads require further reloads.  */
#define NITER 4
#define NVARS 20
#define MULTI(X)                                                               \
  X(0), X(1), X(2), X(3), X(4), X(5), X(6), X(7), X(8), X(9), X(10), X(11),    \
      X(12), X(13), X(14), X(15), X(16), X(17), X(18), X(19)

#define DECLAREI(INDEX) inc##INDEX = incs[INDEX]
#define DECLAREF(INDEX) *ptr##INDEX = ptrs[INDEX], result##INDEX = 0
#define LOOP(INDEX)     result##INDEX += *ptr##INDEX, ptr##INDEX += inc##INDEX
#define COPYOUT(INDEX)  results[INDEX] = result##INDEX

float *ptrs[NVARS];
float  results[NVARS];
int    incs[NVARS];

struct big {
  int i[0x10000];
};
void __attribute__((noinline)) bar(struct big b) { incs[0] += b.i[0]; }

void __attribute__((noinline)) foo(int n) {
  struct big b = {};
  int        MULTI(DECLAREI);
  float      MULTI(DECLAREF);
  while (n--)
    MULTI(LOOP);
  MULTI(COPYOUT);
  bar(b);
}

float input[NITER * NVARS];

int main(void) {
  int i;

  for (i = 0; i < NVARS; i++)
    ptrs[i] = input + i, incs[i] = i;
  for (i = 0; i < NITER * NVARS; i++)
    input[i] = i;
  foo(NITER);
  for (i = 0; i < NVARS; i++)
    if (results[i] != i * NITER * (NITER + 1) / 2)
      return 1;
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-stack-size "0x80100" }
// CHECK-NEXT: // Like pr28982a.c, but with the spill slots outside the range of
// CHECK-NEXT: // a single sp-based load on ARM.  This test tests for cases where
// CHECK-NEXT: // the addresses in the base and index reloads require further reloads.
// CHECK-NEXT: static mut ptrs: __SlateAlign16<[*mut f32; 20]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut results: __SlateAlign16<[f32; 20]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut incs: __SlateAlign16<[i32; 20]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct big {
// CHECK-NEXT:     i: [i32; 65536],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut b: big) {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: *mut i32 = unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = unsafe { *[[V_0]] };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 =
// CHECK-NEXT:         [[V_1]] + unsafe { *unsafe { (b.i.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0]] = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut n: i32) {
// CHECK-NEXT:     let mut b: big = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: big = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc0: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc1: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc2: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((2 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc3: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((3 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc4: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((4 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc5: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((5 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc6: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((6 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc7: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((7 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc8: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((8 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc9: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((9 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc10: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((10 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc11: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((11 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc12: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((12 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc13: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((13 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc14: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((14 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc15: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((15 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc16: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((16 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc17: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((17 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc18: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((18 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut inc19: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset((19 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut ptr0: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result0: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr1: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result1: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr2: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((2 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result2: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr3: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((3 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result3: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr4: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((4 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result4: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr5: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((5 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result5: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr6: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((6 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result6: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr7: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((7 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result7: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr8: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((8 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result8: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr9: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((9 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result9: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr10: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((10 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result10: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr11: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((11 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result11: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr12: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((12 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result12: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr13: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((13 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result13: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr14: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((14 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result14: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr15: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((15 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result15: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr16: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((16 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result16: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr17: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((17 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result17: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr18: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((18 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result18: f32 = (0 as i32) as f32;
// CHECK-NEXT:     let mut ptr19: *mut f32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset((19 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut result19: f32 = (0 as i32) as f32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = n;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] - (1 as i32);
// CHECK-NEXT:         n = [[V_4]];
// CHECK-NEXT:         if !([[V_3]] != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: f32 = result0;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: f32 = [[V_5]] + unsafe { *ptr0 };
// CHECK-NEXT:         result0 = [[V_6]];
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: *mut f32 = ptr0;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: *mut f32 = unsafe { [[V_7]].offset(inc0 as isize) };
// CHECK-NEXT:         ptr0 = [[V_8]];
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: f32 = result1;
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: f32 = [[V_9]] + unsafe { *ptr1 };
// CHECK-NEXT:         result1 = [[V_10]];
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: *mut f32 = ptr1;
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: *mut f32 = unsafe { [[V_11]].offset(inc1 as isize) };
// CHECK-NEXT:         ptr1 = [[V_12]];
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: f32 = result2;
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: f32 = [[V_13]] + unsafe { *ptr2 };
// CHECK-NEXT:         result2 = [[V_14]];
// CHECK-NEXT:         let mut [[V_15:__v[0-9]+]]: *mut f32 = ptr2;
// CHECK-NEXT:         let mut [[V_16:__v[0-9]+]]: *mut f32 = unsafe { [[V_15]].offset(inc2 as isize) };
// CHECK-NEXT:         ptr2 = [[V_16]];
// CHECK-NEXT:         let mut [[V_17:__v[0-9]+]]: f32 = result3;
// CHECK-NEXT:         let mut [[V_18:__v[0-9]+]]: f32 = [[V_17]] + unsafe { *ptr3 };
// CHECK-NEXT:         result3 = [[V_18]];
// CHECK-NEXT:         let mut [[V_19:__v[0-9]+]]: *mut f32 = ptr3;
// CHECK-NEXT:         let mut [[V_20:__v[0-9]+]]: *mut f32 = unsafe { [[V_19]].offset(inc3 as isize) };
// CHECK-NEXT:         ptr3 = [[V_20]];
// CHECK-NEXT:         let mut [[V_21:__v[0-9]+]]: f32 = result4;
// CHECK-NEXT:         let mut [[V_22:__v[0-9]+]]: f32 = [[V_21]] + unsafe { *ptr4 };
// CHECK-NEXT:         result4 = [[V_22]];
// CHECK-NEXT:         let mut [[V_23:__v[0-9]+]]: *mut f32 = ptr4;
// CHECK-NEXT:         let mut [[V_24:__v[0-9]+]]: *mut f32 = unsafe { [[V_23]].offset(inc4 as isize) };
// CHECK-NEXT:         ptr4 = [[V_24]];
// CHECK-NEXT:         let mut [[V_25:__v[0-9]+]]: f32 = result5;
// CHECK-NEXT:         let mut [[V_26:__v[0-9]+]]: f32 = [[V_25]] + unsafe { *ptr5 };
// CHECK-NEXT:         result5 = [[V_26]];
// CHECK-NEXT:         let mut [[V_27:__v[0-9]+]]: *mut f32 = ptr5;
// CHECK-NEXT:         let mut [[V_28:__v[0-9]+]]: *mut f32 = unsafe { [[V_27]].offset(inc5 as isize) };
// CHECK-NEXT:         ptr5 = [[V_28]];
// CHECK-NEXT:         let mut [[V_29:__v[0-9]+]]: f32 = result6;
// CHECK-NEXT:         let mut [[V_30:__v[0-9]+]]: f32 = [[V_29]] + unsafe { *ptr6 };
// CHECK-NEXT:         result6 = [[V_30]];
// CHECK-NEXT:         let mut [[V_31:__v[0-9]+]]: *mut f32 = ptr6;
// CHECK-NEXT:         let mut [[V_32:__v[0-9]+]]: *mut f32 = unsafe { [[V_31]].offset(inc6 as isize) };
// CHECK-NEXT:         ptr6 = [[V_32]];
// CHECK-NEXT:         let mut [[V_33:__v[0-9]+]]: f32 = result7;
// CHECK-NEXT:         let mut [[V_34:__v[0-9]+]]: f32 = [[V_33]] + unsafe { *ptr7 };
// CHECK-NEXT:         result7 = [[V_34]];
// CHECK-NEXT:         let mut [[V_35:__v[0-9]+]]: *mut f32 = ptr7;
// CHECK-NEXT:         let mut [[V_36:__v[0-9]+]]: *mut f32 = unsafe { [[V_35]].offset(inc7 as isize) };
// CHECK-NEXT:         ptr7 = [[V_36]];
// CHECK-NEXT:         let mut [[V_37:__v[0-9]+]]: f32 = result8;
// CHECK-NEXT:         let mut [[V_38:__v[0-9]+]]: f32 = [[V_37]] + unsafe { *ptr8 };
// CHECK-NEXT:         result8 = [[V_38]];
// CHECK-NEXT:         let mut [[V_39:__v[0-9]+]]: *mut f32 = ptr8;
// CHECK-NEXT:         let mut [[V_40:__v[0-9]+]]: *mut f32 = unsafe { [[V_39]].offset(inc8 as isize) };
// CHECK-NEXT:         ptr8 = [[V_40]];
// CHECK-NEXT:         let mut [[V_41:__v[0-9]+]]: f32 = result9;
// CHECK-NEXT:         let mut [[V_42:__v[0-9]+]]: f32 = [[V_41]] + unsafe { *ptr9 };
// CHECK-NEXT:         result9 = [[V_42]];
// CHECK-NEXT:         let mut [[V_43:__v[0-9]+]]: *mut f32 = ptr9;
// CHECK-NEXT:         let mut [[V_44:__v[0-9]+]]: *mut f32 = unsafe { [[V_43]].offset(inc9 as isize) };
// CHECK-NEXT:         ptr9 = [[V_44]];
// CHECK-NEXT:         let mut [[V_45:__v[0-9]+]]: f32 = result10;
// CHECK-NEXT:         let mut [[V_46:__v[0-9]+]]: f32 = [[V_45]] + unsafe { *ptr10 };
// CHECK-NEXT:         result10 = [[V_46]];
// CHECK-NEXT:         let mut [[V_47:__v[0-9]+]]: *mut f32 = ptr10;
// CHECK-NEXT:         let mut [[V_48:__v[0-9]+]]: *mut f32 = unsafe { [[V_47]].offset(inc10 as isize) };
// CHECK-NEXT:         ptr10 = [[V_48]];
// CHECK-NEXT:         let mut [[V_49:__v[0-9]+]]: f32 = result11;
// CHECK-NEXT:         let mut [[V_50:__v[0-9]+]]: f32 = [[V_49]] + unsafe { *ptr11 };
// CHECK-NEXT:         result11 = [[V_50]];
// CHECK-NEXT:         let mut [[V_51:__v[0-9]+]]: *mut f32 = ptr11;
// CHECK-NEXT:         let mut [[V_52:__v[0-9]+]]: *mut f32 = unsafe { [[V_51]].offset(inc11 as isize) };
// CHECK-NEXT:         ptr11 = [[V_52]];
// CHECK-NEXT:         let mut [[V_53:__v[0-9]+]]: f32 = result12;
// CHECK-NEXT:         let mut [[V_54:__v[0-9]+]]: f32 = [[V_53]] + unsafe { *ptr12 };
// CHECK-NEXT:         result12 = [[V_54]];
// CHECK-NEXT:         let mut [[V_55:__v[0-9]+]]: *mut f32 = ptr12;
// CHECK-NEXT:         let mut [[V_56:__v[0-9]+]]: *mut f32 = unsafe { [[V_55]].offset(inc12 as isize) };
// CHECK-NEXT:         ptr12 = [[V_56]];
// CHECK-NEXT:         let mut [[V_57:__v[0-9]+]]: f32 = result13;
// CHECK-NEXT:         let mut [[V_58:__v[0-9]+]]: f32 = [[V_57]] + unsafe { *ptr13 };
// CHECK-NEXT:         result13 = [[V_58]];
// CHECK-NEXT:         let mut [[V_59:__v[0-9]+]]: *mut f32 = ptr13;
// CHECK-NEXT:         let mut [[V_60:__v[0-9]+]]: *mut f32 = unsafe { [[V_59]].offset(inc13 as isize) };
// CHECK-NEXT:         ptr13 = [[V_60]];
// CHECK-NEXT:         let mut [[V_61:__v[0-9]+]]: f32 = result14;
// CHECK-NEXT:         let mut [[V_62:__v[0-9]+]]: f32 = [[V_61]] + unsafe { *ptr14 };
// CHECK-NEXT:         result14 = [[V_62]];
// CHECK-NEXT:         let mut [[V_63:__v[0-9]+]]: *mut f32 = ptr14;
// CHECK-NEXT:         let mut [[V_64:__v[0-9]+]]: *mut f32 = unsafe { [[V_63]].offset(inc14 as isize) };
// CHECK-NEXT:         ptr14 = [[V_64]];
// CHECK-NEXT:         let mut [[V_65:__v[0-9]+]]: f32 = result15;
// CHECK-NEXT:         let mut [[V_66:__v[0-9]+]]: f32 = [[V_65]] + unsafe { *ptr15 };
// CHECK-NEXT:         result15 = [[V_66]];
// CHECK-NEXT:         let mut [[V_67:__v[0-9]+]]: *mut f32 = ptr15;
// CHECK-NEXT:         let mut [[V_68:__v[0-9]+]]: *mut f32 = unsafe { [[V_67]].offset(inc15 as isize) };
// CHECK-NEXT:         ptr15 = [[V_68]];
// CHECK-NEXT:         let mut [[V_69:__v[0-9]+]]: f32 = result16;
// CHECK-NEXT:         let mut [[V_70:__v[0-9]+]]: f32 = [[V_69]] + unsafe { *ptr16 };
// CHECK-NEXT:         result16 = [[V_70]];
// CHECK-NEXT:         let mut [[V_71:__v[0-9]+]]: *mut f32 = ptr16;
// CHECK-NEXT:         let mut [[V_72:__v[0-9]+]]: *mut f32 = unsafe { [[V_71]].offset(inc16 as isize) };
// CHECK-NEXT:         ptr16 = [[V_72]];
// CHECK-NEXT:         let mut [[V_73:__v[0-9]+]]: f32 = result17;
// CHECK-NEXT:         let mut [[V_74:__v[0-9]+]]: f32 = [[V_73]] + unsafe { *ptr17 };
// CHECK-NEXT:         result17 = [[V_74]];
// CHECK-NEXT:         let mut [[V_75:__v[0-9]+]]: *mut f32 = ptr17;
// CHECK-NEXT:         let mut [[V_76:__v[0-9]+]]: *mut f32 = unsafe { [[V_75]].offset(inc17 as isize) };
// CHECK-NEXT:         ptr17 = [[V_76]];
// CHECK-NEXT:         let mut [[V_77:__v[0-9]+]]: f32 = result18;
// CHECK-NEXT:         let mut [[V_78:__v[0-9]+]]: f32 = [[V_77]] + unsafe { *ptr18 };
// CHECK-NEXT:         result18 = [[V_78]];
// CHECK-NEXT:         let mut [[V_79:__v[0-9]+]]: *mut f32 = ptr18;
// CHECK-NEXT:         let mut [[V_80:__v[0-9]+]]: *mut f32 = unsafe { [[V_79]].offset(inc18 as isize) };
// CHECK-NEXT:         ptr18 = [[V_80]];
// CHECK-NEXT:         let mut [[V_81:__v[0-9]+]]: f32 = result19;
// CHECK-NEXT:         let mut [[V_82:__v[0-9]+]]: f32 = [[V_81]] + unsafe { *ptr19 };
// CHECK-NEXT:         result19 = [[V_82]];
// CHECK-NEXT:         let mut [[V_83:__v[0-9]+]]: *mut f32 = ptr19;
// CHECK-NEXT:         let mut [[V_84:__v[0-9]+]]: *mut f32 = unsafe { [[V_83]].offset(inc19 as isize) };
// CHECK-NEXT:         ptr19 = [[V_84]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((0 as i32) as isize)
// CHECK-NEXT:         } = result0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_85:__v[0-9]+]]: f32 = result1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         } = [[V_85]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_86:__v[0-9]+]]: f32 = result2;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((2 as i32) as isize)
// CHECK-NEXT:         } = [[V_86]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_87:__v[0-9]+]]: f32 = result3;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((3 as i32) as isize)
// CHECK-NEXT:         } = [[V_87]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_88:__v[0-9]+]]: f32 = result4;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((4 as i32) as isize)
// CHECK-NEXT:         } = [[V_88]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_89:__v[0-9]+]]: f32 = result5;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((5 as i32) as isize)
// CHECK-NEXT:         } = [[V_89]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_90:__v[0-9]+]]: f32 = result6;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((6 as i32) as isize)
// CHECK-NEXT:         } = [[V_90]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_91:__v[0-9]+]]: f32 = result7;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((7 as i32) as isize)
// CHECK-NEXT:         } = [[V_91]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_92:__v[0-9]+]]: f32 = result8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((8 as i32) as isize)
// CHECK-NEXT:         } = [[V_92]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_93:__v[0-9]+]]: f32 = result9;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((9 as i32) as isize)
// CHECK-NEXT:         } = [[V_93]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_94:__v[0-9]+]]: f32 = result10;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((10 as i32) as isize)
// CHECK-NEXT:         } = [[V_94]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_95:__v[0-9]+]]: f32 = result11;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((11 as i32) as isize)
// CHECK-NEXT:         } = [[V_95]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_96:__v[0-9]+]]: f32 = result12;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((12 as i32) as isize)
// CHECK-NEXT:         } = [[V_96]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_97:__v[0-9]+]]: f32 = result13;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((13 as i32) as isize)
// CHECK-NEXT:         } = [[V_97]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_98:__v[0-9]+]]: f32 = result14;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((14 as i32) as isize)
// CHECK-NEXT:         } = [[V_98]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_99:__v[0-9]+]]: f32 = result15;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((15 as i32) as isize)
// CHECK-NEXT:         } = [[V_99]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_100:__v[0-9]+]]: f32 = result16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((16 as i32) as isize)
// CHECK-NEXT:         } = [[V_100]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_101:__v[0-9]+]]: f32 = result17;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((17 as i32) as isize)
// CHECK-NEXT:         } = [[V_101]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_102:__v[0-9]+]]: f32 = result18;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((18 as i32) as isize)
// CHECK-NEXT:         } = [[V_102]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_103:__v[0-9]+]]: f32 = result19;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset((19 as i32) as isize)
// CHECK-NEXT:         } = [[V_103]];
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(b);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut input: __SlateAlign16<[f32; 80]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (20 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(ptrs.0) as *mut *mut f32 }.offset(i as isize)
// CHECK-NEXT:             } = unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(input.0) as *mut f32 }.offset(i as isize)
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_104:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(incs.0) as *mut i32 }.offset(i as isize) } =
// CHECK-NEXT:                 [[V_104]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_105:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_106:__v[0-9]+]]: i32 = [[V_105]] + (1 as i32);
// CHECK-NEXT:         i = [[V_106]];
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (4 as i32) * (20 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(input.0) as *mut f32 }.offset(i as isize) } =
// CHECK-NEXT:                 i as f32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_107:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_108:__v[0-9]+]]: i32 = [[V_107]] + (1 as i32);
// CHECK-NEXT:         i = [[V_108]];
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(4 as i32);
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while i < (20 as i32) {
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(results.0) as *mut f32 }.offset(i as isize) }
// CHECK-NEXT:         }) != ((i * (4 as i32) * ((4 as i32) + (1 as i32)) / (2 as i32)) as f32)
// CHECK-NEXT:         {
// CHECK-NEXT:             return 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_109:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_110:__v[0-9]+]]: i32 = [[V_109]] + (1 as i32);
// CHECK-NEXT:         i = [[V_110]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
