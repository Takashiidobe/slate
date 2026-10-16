void abort(void);
void exit(int);

char a[] = "12345";
char b[] = "12345";

void loop(char *pz, char *pzDta) {
  for (;;) {
    switch (*(pz++) = *(pzDta++)) {
    case 0:
      goto loopDone2;

    case '"':
    case '\\':
      pz[-1]  = '\\';
      *(pz++) = pzDta[-1];
    }
  }
loopDone2:;

  if (a - pz != b - pzDta)
    abort();
}

int main(void) {
  loop(a, b);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: [i8; 6] = [49 as i8, 50 as i8, 51 as i8, 52 as i8, 53 as i8, 0 as i8];
// CHECK-NEXT: static mut b: [i8; 6] = [49 as i8, 50 as i8, 51 as i8, 52 as i8, 53 as i8, 0 as i8];
// CHECK-NEXT: fn r#loop(mut pz: *mut i8, mut pzDta: *mut i8) {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]])
// CHECK-NEXT:         as *mut *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]])
// CHECK-NEXT:         as *mut *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]])
// CHECK-NEXT:         as *mut *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]])
// CHECK-NEXT:         as *mut *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_5:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_5:__slate_slot_[0-9]+]]: *mut *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_5]])
// CHECK-NEXT:         as *mut *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_6:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_6:__slate_slot_[0-9]+]]: *mut *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_6]])
// CHECK-NEXT:         as *mut *mut i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '__loop_4: loop {
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_6]], pzDta);
// CHECK-NEXT:             std::ptr::write(
// CHECK-NEXT:                 [[SLATE_SLOT_5]],
// CHECK-NEXT:                 unsafe { (*[[SLATE_SLOT_6]]).offset((1 as i32) as isize) },
// CHECK-NEXT:             );
// CHECK-NEXT:             pzDta = *[[SLATE_SLOT_5]];
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_4]], pz);
// CHECK-NEXT:             std::ptr::write(
// CHECK-NEXT:                 [[SLATE_SLOT_3]],
// CHECK-NEXT:                 unsafe { (*[[SLATE_SLOT_4]]).offset((1 as i32) as isize) },
// CHECK-NEXT:             );
// CHECK-NEXT:             pz = *[[SLATE_SLOT_3]];
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_2]], unsafe { *(*[[SLATE_SLOT_6]]) });
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *(*[[SLATE_SLOT_4]]) = *[[SLATE_SLOT_2]];
// CHECK-NEXT:             }
// CHECK-NEXT:             let [[T_0:__t[0-9]+]]: i32 = *[[SLATE_SLOT_2]] as i32;
// CHECK-NEXT:             if [[T_0]] == (0 as i32) {
// CHECK-NEXT:                 break '__loop_4;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if [[T_0]] == (34 as i32) {} else {
// CHECK-NEXT:                     if [[T_0]] == (92 as i32) {} else {
// CHECK-NEXT:                         continue '__loop_4;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { pz.offset(-(1 as i32) as isize) } = (92 as i32) as i8;
// CHECK-NEXT:             }
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_1]], pz);
// CHECK-NEXT:             std::ptr::write(
// CHECK-NEXT:                 [[SLATE_SLOT_0]],
// CHECK-NEXT:                 unsafe { (*[[SLATE_SLOT_1]]).offset((1 as i32) as isize) },
// CHECK-NEXT:             );
// CHECK-NEXT:             pz = *[[SLATE_SLOT_0]];
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *(*[[SLATE_SLOT_1]]) = unsafe {
// CHECK-NEXT:                     *unsafe { pzDta.offset(-(1 as i32) as isize) }
// CHECK-NEXT:                 };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         {}
// CHECK-NEXT:         if ((unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(a) as *mut i8 }.offset_from(pz as *mut i8)
// CHECK-NEXT:         }) as i64)
// CHECK-NEXT:             != ((unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(b) as *mut i8 }
// CHECK-NEXT:                     .offset_from(pzDta as *mut i8)
// CHECK-NEXT:             }) as i64)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     r#loop(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(a) as *mut i8 },
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(b) as *mut i8 },
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
