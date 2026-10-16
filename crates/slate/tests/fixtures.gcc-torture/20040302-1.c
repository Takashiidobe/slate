/* { dg-require-effective-target label_values } */
int code[] = {0, 0, 0, 0, 1};

void foo(int x) {
  volatile int b;
  b = 0xffffffff;
}

void bar(int *pc) {
  static const void *l[] = {&&lab0, &&end};

  foo(0);
  goto *l[*pc];
lab0:
  foo(0);
  pc++;
  goto *l[*pc];
end:
  return;
}

int main() {
  bar(code);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: /// { dg-require-effective-target label_values }
// CHECK-NEXT: static mut code: __SlateAlign16<[i32; 5]> =
// CHECK-NEXT:     __SlateAlign16([0 as i32, 0 as i32, 0 as i32, 0 as i32, 1 as i32]);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32) {
// CHECK-NEXT:     let mut b: i32 = 0 as i32;
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(b), (4294967295 as u32) as i32) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut pc: *mut i32) {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut *mut i32 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut *mut i32 =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_1]]) as *mut *mut i32;
// CHECK-NEXT:     let mut __slate_state: usize = 2usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '__slate_dispatch: loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     foo(0 as i32);
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_1]], pc);
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_0]], unsafe {
// CHECK-NEXT:                         (*[[SLATE_SLOT_1]]).offset((1 as i32) as isize)
// CHECK-NEXT:                     });
// CHECK-NEXT:                     pc = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                     match (unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(l.0) as *mut *const () }
// CHECK-NEXT:                                 .offset((unsafe { *pc }) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as usize
// CHECK-NEXT:                     {
// CHECK-NEXT:                         6 => {
// CHECK-NEXT:                             __slate_state = 1usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         7 => {
// CHECK-NEXT:                             __slate_state = 0usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         _ => {
// CHECK-NEXT:                             unreachable!();
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     foo(0 as i32);
// CHECK-NEXT:                     match (unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(l.0) as *mut *const () }
// CHECK-NEXT:                                 .offset((unsafe { *pc }) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as usize
// CHECK-NEXT:                     {
// CHECK-NEXT:                         6 => {
// CHECK-NEXT:                             __slate_state = 1usize;
// CHECK-NEXT:                             continue '__slate_dispatch;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         7 => {
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
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut l: __SlateAlign16<[*const (); 2]> = __SlateAlign16([
// CHECK-NEXT:     (6usize as *mut ()) as *const (),
// CHECK-NEXT:     (7usize as *mut ()) as *const (),
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     bar(unsafe { std::ptr::addr_of_mut!(code.0) as *mut i32 });
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
