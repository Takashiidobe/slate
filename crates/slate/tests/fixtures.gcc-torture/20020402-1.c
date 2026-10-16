/* derived from PR c/2100 */

extern void abort();
extern void exit(int);

#define SMALL_N  2
#define NUM_ELEM 4

int main(void) {
  int listElem[NUM_ELEM] = {30, 2, 10, 5};
  int listSmall[SMALL_N];
  int i, j;
  int posGreatest = -1, greatest = -1;

  for (i = 0; i < SMALL_N; i++) {
    listSmall[i] = listElem[i];
    if (listElem[i] > greatest) {
      posGreatest = i;
      greatest    = listElem[i];
    }
  }

  for (i = SMALL_N; i < NUM_ELEM; i++) {
    if (listElem[i] < greatest) {
      listSmall[posGreatest] = listElem[i];
      posGreatest            = 0;
      greatest               = listSmall[0];
      for (j = 1; j < SMALL_N; j++)
        if (listSmall[j] > greatest) {
          posGreatest = j;
          greatest    = listSmall[j];
        }
    }
  }

  if (listSmall[0] != 5 || listSmall[1] != 2)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! derived from PR c/2100
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut listElem: __SlateAlign16<[i32; 4]> =
// CHECK-NEXT:         __SlateAlign16([30 as i32, 2 as i32, 10 as i32, 5 as i32]);
// CHECK-NEXT:     let mut listSmall: [i32; 2] = [0 as i32; 2];
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut posGreatest: i32 = -(1 as i32);
// CHECK-NEXT:     let mut greatest: i32 = -(1 as i32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (2 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (listSmall.as_mut_ptr() as *mut i32).offset(i as isize) } =
// CHECK-NEXT:                 unsafe { *unsafe { (listElem.0.as_mut_ptr() as *mut i32).offset(i as isize) } };
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { *unsafe { (listElem.0.as_mut_ptr() as *mut i32).offset(i as isize) } })
// CHECK-NEXT:             > greatest
// CHECK-NEXT:         {
// CHECK-NEXT:             posGreatest = i;
// CHECK-NEXT:             greatest =
// CHECK-NEXT:                 unsafe { *unsafe { (listElem.0.as_mut_ptr() as *mut i32).offset(i as isize) } };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 2 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (4 as i32) {
// CHECK-NEXT:         if (unsafe { *unsafe { (listElem.0.as_mut_ptr() as *mut i32).offset(i as isize) } })
// CHECK-NEXT:             < greatest
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (listSmall.as_mut_ptr() as *mut i32).offset(posGreatest as isize) } =
// CHECK-NEXT:                     unsafe { *unsafe { (listElem.0.as_mut_ptr() as *mut i32).offset(i as isize) } };
// CHECK-NEXT:             }
// CHECK-NEXT:             posGreatest = 0 as i32;
// CHECK-NEXT:             greatest = unsafe {
// CHECK-NEXT:                 *unsafe { (listSmall.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) }
// CHECK-NEXT:             };
// CHECK-NEXT:             j = 1 as i32;
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while j < (2 as i32) {
// CHECK-NEXT:                 if (unsafe { *unsafe { (listSmall.as_mut_ptr() as *mut i32).offset(j as isize) } })
// CHECK-NEXT:                     > greatest
// CHECK-NEXT:                 {
// CHECK-NEXT:                     posGreatest = j;
// CHECK-NEXT:                     greatest = unsafe {
// CHECK-NEXT:                         *unsafe { (listSmall.as_mut_ptr() as *mut i32).offset(j as isize) }
// CHECK-NEXT:                     };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let mut [[V_3:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:                 let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:                 j = [[V_4]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { (listSmall.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } })
// CHECK-NEXT:         != (5 as i32)
// CHECK-NEXT:         || (unsafe { *unsafe { (listSmall.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } })
// CHECK-NEXT:             != (2 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
