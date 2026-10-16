/* { dg-require-effective-target label_values } */

void abort(void);
void exit(int);

int x(int i) {
  void *j[] = {&&x, &&y, &&z};
  goto *j[i];
x:
  return 2;
y:
  return 3;
z:
  return 5;
}
int main(void) {
  if (x(0) != 2 || x(1) != 3 || x(2) != 5)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target label_values }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn x(mut i: i32) -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<__SlateAlign16<[*mut (); 3]>> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut [*mut (); 3] =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut [*mut (); 3];
// CHECK-NEXT:     let mut __slate_state: usize = 3usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '__slate_dispatch: loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 5 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return 3 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     return 2 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     std::ptr::write(
// CHECK-NEXT:                         [[SLATE_SLOT_0]],
// CHECK-NEXT:                         [4usize as *mut (), 5usize as *mut (), 6usize as *mut ()],
// CHECK-NEXT:                     );
// CHECK-NEXT:                     match (unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             ((*[[SLATE_SLOT_0]]).as_mut_ptr() as *mut *mut ()).offset(i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as usize
// CHECK-NEXT:                     {
// CHECK-NEXT:                         4 => {
// CHECK-NEXT:                             __slate_state = 2usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         5 => {
// CHECK-NEXT:                             __slate_state = 1usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         6 => {
// CHECK-NEXT:                             __slate_state = 0usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         _ => {
// CHECK-NEXT:                             unreachable!();
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if x(0 as i32) != (2 as i32) {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = x(1 as i32) != (3 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         [[V_2]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = x(2 as i32) != (5 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_2]] {
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
