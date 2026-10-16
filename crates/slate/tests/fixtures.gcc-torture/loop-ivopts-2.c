/* PR rtl-optimization/20290  */

/* We used to mis-optimize the second loop in main on at least ppc and
   arm, because tree loop would change the loop to something like:

  ivtmp.65 = &l[i];
  ivtmp.16 = 113;
  goto <bb 4> (<L4>);

<L3>:;
  *(ivtmp.65 + 4294967292B) = 9;
  i = i + 1;

<L4>:;
  ivtmp.16 = ivtmp.16 - 1;
  ivtmp.65 = ivtmp.65 + 4B;
  if (ivtmp.16 != 0) goto <L3>;

  We used to consider the increment of i as executed in every
  iteration, so we'd miscompute the final value.  */

extern void abort(void);

void check(unsigned int *l) {
  int i;
  for (i = 0; i < 288; i++)
    if (l[i] != 7 + (i < 256 || i >= 280) + (i >= 144 && i < 256))
      abort();
}

int main(void) {
  int          i;
  unsigned int l[288];

  for (i = 0; i < 144; i++)
    l[i] = 8;
  for (; i < 256; i++)
    l[i] = 9;
  for (; i < 280; i++)
    l[i] = 7;
  for (; i < 288; i++)
    l[i] = 8;
  check(l);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/20290
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: // We used to mis-optimize the second loop in main on at least ppc and
// CHECK-NEXT: //    arm, because tree loop would change the loop to something like:
// CHECK-NEXT: //
// CHECK-NEXT: //   ivtmp.65 = &l[i];
// CHECK-NEXT: //   ivtmp.16 = 113;
// CHECK-NEXT: //   goto <bb 4> (<L4>);
// CHECK-NEXT: //
// CHECK-NEXT: // <L3>:;
// CHECK-NEXT: //   *(ivtmp.65 + 4294967292B) = 9;
// CHECK-NEXT: //   i = i + 1;
// CHECK-NEXT: //
// CHECK-NEXT: // <L4>:;
// CHECK-NEXT: //   ivtmp.16 = ivtmp.16 - 1;
// CHECK-NEXT: //   ivtmp.65 = ivtmp.65 + 4B;
// CHECK-NEXT: //   if (ivtmp.16 != 0) goto <L3>;
// CHECK-NEXT: //
// CHECK-NEXT: //   We used to consider the increment of i as executed in every
// CHECK-NEXT: //   iteration, so we'd miscompute the final value.
// CHECK-NEXT: fn check(mut l: *mut u32) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (288 as i32) {
// CHECK-NEXT:         if (unsafe { *unsafe { l.offset(i as isize) } })
// CHECK-NEXT:             != (((7 as i32)
// CHECK-NEXT:                 + ((i < (256 as i32) || i >= (280 as i32)) as i32)
// CHECK-NEXT:                 + ((i >= (144 as i32) && i < (256 as i32)) as i32)) as u32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut l: __SlateAlign16<[u32; 288]> = __SlateAlign16([0 as u32; 288]);
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (144 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (l.0.as_mut_ptr() as *mut u32).offset(i as isize) } = (8 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (256 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (l.0.as_mut_ptr() as *mut u32).offset(i as isize) } = (9 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while i < (280 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (l.0.as_mut_ptr() as *mut u32).offset(i as isize) } = (7 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:         i = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while i < (288 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (l.0.as_mut_ptr() as *mut u32).offset(i as isize) } = (8 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = [[V_8]] + (1 as i32);
// CHECK-NEXT:         i = [[V_9]];
// CHECK-NEXT:     }
// CHECK-NEXT:     check(l.0.as_mut_ptr() as *mut u32);
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
