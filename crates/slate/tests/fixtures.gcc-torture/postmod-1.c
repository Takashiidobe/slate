#define DECLARE_ARRAY(A)   array##A[0x10]
#define DECLARE_COUNTER(A) counter##A = 0
#define DECLARE_POINTER(A) *pointer##A = array##A + x
/* Create a loop that allows post-modification of pointerA, followed by
   a use of the post-modified address.  */
#define BEFORE(A)          counter##A += *pointer##A, pointer##A += 3
#define AFTER(A)           counter##A += pointer##A[x]

/* Set up the arrays so that one iteration of the loop sets the counter
   to 3.0f.  */
#define INIT_ARRAY(A) array##A[1] = 1.0f, array##A[5] = 2.0f

/* Check that the loop worked correctly for all values.  */
#define CHECK_ARRAY(A) exit_code |= (counter##A != 3.0f)

/* Having 6 copies triggered the bug for ARM and Thumb.  */
#define MANY(A) A(0), A(1), A(2), A(3), A(4), A(5)

/* Each addendA should be allocated a register.  */
#define INIT_VOLATILE(A) addend##A = vol
#define ADD_VOLATILE(A)  vol += addend##A

/* Having 5 copies triggered the bug for ARM and Thumb.  */
#define MANY2(A) A(0), A(1), A(2), A(3), A(4)

float MANY(DECLARE_ARRAY);
float MANY(DECLARE_COUNTER);

volatile int stop = 1;
volatile int vol;

void __attribute__((noinline)) foo(int x) {
  float MANY(DECLARE_POINTER);
  int   i;

  do {
    MANY(BEFORE);
    MANY(AFTER);
    /* Create an inner loop that should ensure the code above
       has registers free for reload inheritance.  */
    {
      int MANY2(INIT_VOLATILE);
      for (i = 0; i < 10; i++)
        MANY2(ADD_VOLATILE);
    }
  } while (!stop);
}

int main(void) {
  int exit_code = 0;

  MANY(INIT_ARRAY);
  foo(1);
  MANY(CHECK_ARRAY);
  return exit_code;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Create a loop that allows post-modification of pointerA, followed by
// CHECK-NEXT: //! a use of the post-modified address.
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: // Set up the arrays so that one iteration of the loop sets the counter
// CHECK-NEXT: // to 3.0f.
// CHECK-NEXT: // Check that the loop worked correctly for all values.
// CHECK-NEXT: // Having 6 copies triggered the bug for ARM and Thumb.
// CHECK-NEXT: // Each addendA should be allocated a register.
// CHECK-NEXT: // Having 5 copies triggered the bug for ARM and Thumb.
// CHECK-NEXT: static mut array0: __SlateAlign16<[f32; 16]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut array1: __SlateAlign16<[f32; 16]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut array2: __SlateAlign16<[f32; 16]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut array3: __SlateAlign16<[f32; 16]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut array4: __SlateAlign16<[f32; 16]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut array5: __SlateAlign16<[f32; 16]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut counter0: f32 = (0 as i32) as f32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut counter1: f32 = (0 as i32) as f32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut counter2: f32 = (0 as i32) as f32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut counter3: f32 = (0 as i32) as f32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut counter4: f32 = (0 as i32) as f32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut counter5: f32 = (0 as i32) as f32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut stop: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut vol: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32) {
// CHECK-NEXT:     let mut pointer0: *mut f32 =
// CHECK-NEXT:         unsafe { unsafe { std::ptr::addr_of_mut!(array0.0) as *mut f32 }.offset(x as isize) };
// CHECK-NEXT:     let mut pointer1: *mut f32 =
// CHECK-NEXT:         unsafe { unsafe { std::ptr::addr_of_mut!(array1.0) as *mut f32 }.offset(x as isize) };
// CHECK-NEXT:     let mut pointer2: *mut f32 =
// CHECK-NEXT:         unsafe { unsafe { std::ptr::addr_of_mut!(array2.0) as *mut f32 }.offset(x as isize) };
// CHECK-NEXT:     let mut pointer3: *mut f32 =
// CHECK-NEXT:         unsafe { unsafe { std::ptr::addr_of_mut!(array3.0) as *mut f32 }.offset(x as isize) };
// CHECK-NEXT:     let mut pointer4: *mut f32 =
// CHECK-NEXT:         unsafe { unsafe { std::ptr::addr_of_mut!(array4.0) as *mut f32 }.offset(x as isize) };
// CHECK-NEXT:     let mut pointer5: *mut f32 =
// CHECK-NEXT:         unsafe { unsafe { std::ptr::addr_of_mut!(array5.0) as *mut f32 }.offset(x as isize) };
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: f32 = unsafe { counter0 };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: f32 = [[V_0]] + unsafe { *pointer0 };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             counter0 = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: *mut f32 = pointer0;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: *mut f32 = unsafe { [[V_2]].offset((3 as i32) as isize) };
// CHECK-NEXT:         pointer0 = [[V_3]];
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: f32 = unsafe { counter1 };
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: f32 = [[V_4]] + unsafe { *pointer1 };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             counter1 = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: *mut f32 = pointer1;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: *mut f32 = unsafe { [[V_6]].offset((3 as i32) as isize) };
// CHECK-NEXT:         pointer1 = [[V_7]];
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: f32 = unsafe { counter2 };
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: f32 = [[V_8]] + unsafe { *pointer2 };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             counter2 = [[V_9]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: *mut f32 = pointer2;
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: *mut f32 = unsafe { [[V_10]].offset((3 as i32) as isize) };
// CHECK-NEXT:         pointer2 = [[V_11]];
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: f32 = unsafe { counter3 };
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: f32 = [[V_12]] + unsafe { *pointer3 };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             counter3 = [[V_13]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: *mut f32 = pointer3;
// CHECK-NEXT:         let mut [[V_15:__v[0-9]+]]: *mut f32 = unsafe { [[V_14]].offset((3 as i32) as isize) };
// CHECK-NEXT:         pointer3 = [[V_15]];
// CHECK-NEXT:         let mut [[V_16:__v[0-9]+]]: f32 = unsafe { counter4 };
// CHECK-NEXT:         let mut [[V_17:__v[0-9]+]]: f32 = [[V_16]] + unsafe { *pointer4 };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             counter4 = [[V_17]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_18:__v[0-9]+]]: *mut f32 = pointer4;
// CHECK-NEXT:         let mut [[V_19:__v[0-9]+]]: *mut f32 = unsafe { [[V_18]].offset((3 as i32) as isize) };
// CHECK-NEXT:         pointer4 = [[V_19]];
// CHECK-NEXT:         let mut [[V_20:__v[0-9]+]]: f32 = unsafe { counter5 };
// CHECK-NEXT:         let mut [[V_21:__v[0-9]+]]: f32 = [[V_20]] + unsafe { *pointer5 };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             counter5 = [[V_21]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_22:__v[0-9]+]]: *mut f32 = pointer5;
// CHECK-NEXT:         let mut [[V_23:__v[0-9]+]]: *mut f32 = unsafe { [[V_22]].offset((3 as i32) as isize) };
// CHECK-NEXT:         pointer5 = [[V_23]];
// CHECK-NEXT:         let mut [[V_24:__v[0-9]+]]: f32 = unsafe { counter0 };
// CHECK-NEXT:         let mut [[V_25:__v[0-9]+]]: f32 = [[V_24]] + unsafe { *unsafe { pointer0.offset(x as isize) } };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             counter0 = [[V_25]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_26:__v[0-9]+]]: f32 = unsafe { counter1 };
// CHECK-NEXT:         let mut [[V_27:__v[0-9]+]]: f32 = [[V_26]] + unsafe { *unsafe { pointer1.offset(x as isize) } };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             counter1 = [[V_27]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_28:__v[0-9]+]]: f32 = unsafe { counter2 };
// CHECK-NEXT:         let mut [[V_29:__v[0-9]+]]: f32 = [[V_28]] + unsafe { *unsafe { pointer2.offset(x as isize) } };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             counter2 = [[V_29]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_30:__v[0-9]+]]: f32 = unsafe { counter3 };
// CHECK-NEXT:         let mut [[V_31:__v[0-9]+]]: f32 = [[V_30]] + unsafe { *unsafe { pointer3.offset(x as isize) } };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             counter3 = [[V_31]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_32:__v[0-9]+]]: f32 = unsafe { counter4 };
// CHECK-NEXT:         let mut [[V_33:__v[0-9]+]]: f32 = [[V_32]] + unsafe { *unsafe { pointer4.offset(x as isize) } };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             counter4 = [[V_33]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_34:__v[0-9]+]]: f32 = unsafe { counter5 };
// CHECK-NEXT:         let mut [[V_35:__v[0-9]+]]: f32 = [[V_34]] + unsafe { *unsafe { pointer5.offset(x as isize) } };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             counter5 = [[V_35]];
// CHECK-NEXT:         }
// CHECK-NEXT:         // Create an inner loop that should ensure the code above
// CHECK-NEXT:         // has registers free for reload inheritance.
// CHECK-NEXT:         let mut addend0: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vol)) };
// CHECK-NEXT:         let mut addend1: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vol)) };
// CHECK-NEXT:         let mut addend2: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vol)) };
// CHECK-NEXT:         let mut addend3: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vol)) };
// CHECK-NEXT:         let mut addend4: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vol)) };
// CHECK-NEXT:         i = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (10 as i32) {
// CHECK-NEXT:             let mut [[V_36:__v[0-9]+]]: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vol)) };
// CHECK-NEXT:             let mut [[V_37:__v[0-9]+]]: i32 = [[V_36]] + addend0;
// CHECK-NEXT:             unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vol), [[V_37]]) };
// CHECK-NEXT:             let mut [[V_38:__v[0-9]+]]: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vol)) };
// CHECK-NEXT:             let mut [[V_39:__v[0-9]+]]: i32 = [[V_38]] + addend1;
// CHECK-NEXT:             unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vol), [[V_39]]) };
// CHECK-NEXT:             let mut [[V_40:__v[0-9]+]]: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vol)) };
// CHECK-NEXT:             let mut [[V_41:__v[0-9]+]]: i32 = [[V_40]] + addend2;
// CHECK-NEXT:             unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vol), [[V_41]]) };
// CHECK-NEXT:             let mut [[V_42:__v[0-9]+]]: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vol)) };
// CHECK-NEXT:             let mut [[V_43:__v[0-9]+]]: i32 = [[V_42]] + addend3;
// CHECK-NEXT:             unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vol), [[V_43]]) };
// CHECK-NEXT:             let mut [[V_44:__v[0-9]+]]: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vol)) };
// CHECK-NEXT:             let mut [[V_45:__v[0-9]+]]: i32 = [[V_44]] + addend4;
// CHECK-NEXT:             unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vol), [[V_45]]) };
// CHECK-NEXT:             let mut [[V_46:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_47:__v[0-9]+]]: i32 = [[V_46]] + (1 as i32);
// CHECK-NEXT:             i = [[V_47]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !(!((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(stop)) }) != (0 as i32))) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut exit_code: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(array0.0) as *mut f32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         } = 1.0f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(array0.0) as *mut f32 }.offset((5 as i32) as isize)
// CHECK-NEXT:         } = 2.0f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(array1.0) as *mut f32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         } = 1.0f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(array1.0) as *mut f32 }.offset((5 as i32) as isize)
// CHECK-NEXT:         } = 2.0f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(array2.0) as *mut f32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         } = 1.0f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(array2.0) as *mut f32 }.offset((5 as i32) as isize)
// CHECK-NEXT:         } = 2.0f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(array3.0) as *mut f32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         } = 1.0f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(array3.0) as *mut f32 }.offset((5 as i32) as isize)
// CHECK-NEXT:         } = 2.0f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(array4.0) as *mut f32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         } = 1.0f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(array4.0) as *mut f32 }.offset((5 as i32) as isize)
// CHECK-NEXT:         } = 2.0f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(array5.0) as *mut f32 }.offset((1 as i32) as isize)
// CHECK-NEXT:         } = 1.0f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(array5.0) as *mut f32 }.offset((5 as i32) as isize)
// CHECK-NEXT:         } = 2.0f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(1 as i32);
// CHECK-NEXT:     let mut [[V_48:__v[0-9]+]]: i32 = exit_code;
// CHECK-NEXT:     let mut [[V_49:__v[0-9]+]]: i32 = [[V_48]] | (((unsafe { counter0 }) != 3.0f32) as i32);
// CHECK-NEXT:     exit_code = [[V_49]];
// CHECK-NEXT:     let mut [[V_50:__v[0-9]+]]: i32 = exit_code;
// CHECK-NEXT:     let mut [[V_51:__v[0-9]+]]: i32 = [[V_50]] | (((unsafe { counter1 }) != 3.0f32) as i32);
// CHECK-NEXT:     exit_code = [[V_51]];
// CHECK-NEXT:     let mut [[V_52:__v[0-9]+]]: i32 = exit_code;
// CHECK-NEXT:     let mut [[V_53:__v[0-9]+]]: i32 = [[V_52]] | (((unsafe { counter2 }) != 3.0f32) as i32);
// CHECK-NEXT:     exit_code = [[V_53]];
// CHECK-NEXT:     let mut [[V_54:__v[0-9]+]]: i32 = exit_code;
// CHECK-NEXT:     let mut [[V_55:__v[0-9]+]]: i32 = [[V_54]] | (((unsafe { counter3 }) != 3.0f32) as i32);
// CHECK-NEXT:     exit_code = [[V_55]];
// CHECK-NEXT:     let mut [[V_56:__v[0-9]+]]: i32 = exit_code;
// CHECK-NEXT:     let mut [[V_57:__v[0-9]+]]: i32 = [[V_56]] | (((unsafe { counter4 }) != 3.0f32) as i32);
// CHECK-NEXT:     exit_code = [[V_57]];
// CHECK-NEXT:     let mut [[V_58:__v[0-9]+]]: i32 = exit_code;
// CHECK-NEXT:     let mut [[V_59:__v[0-9]+]]: i32 = [[V_58]] | (((unsafe { counter5 }) != 3.0f32) as i32);
// CHECK-NEXT:     exit_code = [[V_59]];
// CHECK-NEXT:     exit_code
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
