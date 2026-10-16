/* PR rtl-optimization/23560 */

struct rtattr {
  unsigned short rta_len;
  unsigned short rta_type;
};

__attribute__((noinline)) int inet_check_attr(void *r, struct rtattr **rta) {
  int i;

  for (i = 1; i <= 14; i++) {
    struct rtattr *attr = rta[i - 1];
    if (attr) {
      if (attr->rta_len - sizeof(struct rtattr) < 4)
        return -22;
      if (i != 9 && i != 8)
        rta[i - 1] = attr + 1;
    }
  }
  return 0;
}

extern void abort(void);

int main(void) {
  struct rtattr  rt[2];
  struct rtattr *rta[14];
  int            i;

  rt[0].rta_len  = sizeof(struct rtattr) + 8;
  rt[0].rta_type = 0;
  rt[1]          = rt[0];
  for (i = 0; i < 14; i++)
    rta[i] = &rt[0];
  if (inet_check_attr(0, rta) != 0)
    abort();
  for (i = 0; i < 14; i++)
    if (rta[i] != &rt[i != 7 && i != 8])
      abort();
  for (i = 0; i < 14; i++)
    rta[i] = &rt[0];
  rta[1]         = 0;
  rt[1].rta_len -= 8;
  rta[5]         = &rt[1];
  if (inet_check_attr(0, rta) != -22)
    abort();
  for (i = 0; i < 14; i++)
    if (i == 1 && rta[i] != 0)
      abort();
    else if (i != 1 && i <= 5 && rta[i] != &rt[1])
      abort();
    else if (i > 5 && rta[i] != &rt[0])
      abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/23560
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct rtattr {
// CHECK-NEXT:     rta_len: u16,
// CHECK-NEXT:     rta_type: u16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn inet_check_attr(mut r: *mut (), mut rta: *mut *mut rtattr) -> i32 {
// CHECK-NEXT:     let mut i: i32 = 1 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= (14 as i32) {
// CHECK-NEXT:         let mut attr: *mut rtattr = unsafe { *unsafe { rta.offset((i - (1 as i32)) as isize) } };
// CHECK-NEXT:         if attr != std::ptr::null_mut::<rtattr>() {
// CHECK-NEXT:             if (((((unsafe { (*attr).rta_len }) as u32) as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(4 as u64)
// CHECK-NEXT:                 < (((4 as i32) as i64) as u64)
// CHECK-NEXT:             {
// CHECK-NEXT:                 return -(22 as i32);
// CHECK-NEXT:             }
// CHECK-NEXT:             if i != (9 as i32) && i != (8 as i32) {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe { rta.offset((i - (1 as i32)) as isize) } =
// CHECK-NEXT:                         unsafe { attr.offset((1 as i32) as isize) };
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut rt: [rtattr; 2] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut rta: __SlateAlign16<[*mut rtattr; 14]> = __SlateAlign16([0 as *mut rtattr; 14]);
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (rt.as_mut_ptr() as *mut rtattr).offset((0 as i32) as isize) }).rta_len =
// CHECK-NEXT:             (4 as u64).wrapping_add(((8 as i32) as i64) as u64) as u16;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (rt.as_mut_ptr() as *mut rtattr).offset((0 as i32) as isize) }).rta_type =
// CHECK-NEXT:             ((0 as i32) as i16) as u16;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (rt.as_mut_ptr() as *mut rtattr).offset((1 as i32) as isize) } =
// CHECK-NEXT:             unsafe { *unsafe { (rt.as_mut_ptr() as *mut rtattr).offset((0 as i32) as isize) } };
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (14 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (rta.0.as_mut_ptr() as *mut *mut rtattr).offset(i as isize) } =
// CHECK-NEXT:                 unsafe { (rt.as_mut_ptr() as *mut rtattr).offset((0 as i32) as isize) };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if inet_check_attr(
// CHECK-NEXT:         std::ptr::null_mut::<()>(),
// CHECK-NEXT:         rta.0.as_mut_ptr() as *mut *mut rtattr,
// CHECK-NEXT:     ) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (14 as i32) {
// CHECK-NEXT:         if (unsafe { *unsafe { (rta.0.as_mut_ptr() as *mut *mut rtattr).offset(i as isize) } })
// CHECK-NEXT:             != unsafe {
// CHECK-NEXT:                 (rt.as_mut_ptr() as *mut rtattr)
// CHECK-NEXT:                     .offset(((i != (7 as i32) && i != (8 as i32)) as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while i < (14 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (rta.0.as_mut_ptr() as *mut *mut rtattr).offset(i as isize) } =
// CHECK-NEXT:                 unsafe { (rt.as_mut_ptr() as *mut rtattr).offset((0 as i32) as isize) };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:         i = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (rta.0.as_mut_ptr() as *mut *mut rtattr).offset((1 as i32) as isize) } =
// CHECK-NEXT:             std::ptr::null_mut::<rtattr>();
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: *mut rtattr =
// CHECK-NEXT:         unsafe { (rt.as_mut_ptr() as *mut rtattr).offset((1 as i32) as isize) };
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: u16 = unsafe { (*[[V_8]]).rta_len };
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: u16 = (((([[V_9]] as u32) as i32) - (8 as i32)) as i16) as u16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*[[V_8]]).rta_len = [[V_10]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (rta.0.as_mut_ptr() as *mut *mut rtattr).offset((5 as i32) as isize) } =
// CHECK-NEXT:             unsafe { (rt.as_mut_ptr() as *mut rtattr).offset((1 as i32) as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if inet_check_attr(
// CHECK-NEXT:         std::ptr::null_mut::<()>(),
// CHECK-NEXT:         rta.0.as_mut_ptr() as *mut *mut rtattr,
// CHECK-NEXT:     ) != -(22 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while i < (14 as i32) {
// CHECK-NEXT:         if i == (1 as i32)
// CHECK-NEXT:             && (unsafe { *unsafe { (rta.0.as_mut_ptr() as *mut *mut rtattr).offset(i as isize) } })
// CHECK-NEXT:                 != std::ptr::null_mut::<rtattr>()
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if i != (1 as i32)
// CHECK-NEXT:                 && i <= (5 as i32)
// CHECK-NEXT:                 && (unsafe {
// CHECK-NEXT:                     *unsafe { (rta.0.as_mut_ptr() as *mut *mut rtattr).offset(i as isize) }
// CHECK-NEXT:                 }) != unsafe { (rt.as_mut_ptr() as *mut rtattr).offset((1 as i32) as isize) }
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if i > (5 as i32)
// CHECK-NEXT:                     && (unsafe {
// CHECK-NEXT:                         *unsafe { (rta.0.as_mut_ptr() as *mut *mut rtattr).offset(i as isize) }
// CHECK-NEXT:                     }) != unsafe { (rt.as_mut_ptr() as *mut rtattr).offset((0 as i32) as isize) }
// CHECK-NEXT:                 {
// CHECK-NEXT:                     unsafe { abort() };
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: i32 = [[V_11]] + (1 as i32);
// CHECK-NEXT:         i = [[V_12]];
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
