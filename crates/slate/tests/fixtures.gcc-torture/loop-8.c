void abort(void);
void exit(int);

double a[3] = {0.0, 1.0, 2.0};

void bar(int x, double *y) {
  if (x || *y != 1.0)
    abort();
}

int main() {
  double c;
  int    d;
  for (d = 0; d < 3; d++) {
    c = a[d];
    if (c > 0.0)
      goto e;
  }
  bar(1, &c);
  exit(1);
e:
  bar(0, &c);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut a: __SlateAlign16<[f64; 3]> = __SlateAlign16([0.0f64, 1.0f64, 2.0f64]);
// CHECK-NEXT: fn bar(mut x: i32, mut y: &f64) {
// CHECK-NEXT:     if x != (0 as i32) || (unsafe { *(y as *const f64) }) != 1.0f64 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<f64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut f64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[SLATE_SLOT_2]] = 0 as i32;
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 if *[[SLATE_SLOT_2]] < (3 as i32) {
// CHECK-NEXT:                     *[[SLATE_SLOT_3]] = unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(a.0) as *mut f64 }
// CHECK-NEXT:                                 .offset(*[[SLATE_SLOT_2]] as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     };
// CHECK-NEXT:                     if *[[SLATE_SLOT_3]] > 0.0f64 {
// CHECK-NEXT:                         break '[[JOIN_0]];
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         std::ptr::write([[SLATE_SLOT_1]], *[[SLATE_SLOT_2]]);
// CHECK-NEXT:                         std::ptr::write([[SLATE_SLOT_0]], *[[SLATE_SLOT_1]] + (1 as i32));
// CHECK-NEXT:                         *[[SLATE_SLOT_2]] = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             bar(1 as i32, unsafe { &(*std::ptr::addr_of_mut!(* [[SLATE_SLOT_3]])) });
// CHECK-NEXT:             unsafe { exit(1 as i32) };
// CHECK-NEXT:         }
// CHECK-NEXT:         bar(0 as i32, unsafe { &(*std::ptr::addr_of_mut!(* [[SLATE_SLOT_3]])) });
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
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
// SLATE-FILECHECK-END
