void abort(void);
void exit(int);

int array[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

int main(void) {
  int  i, j;
  int *p;

  for (i = 0; i < 10; i++)
    for (p = &array[0]; p != &array[9]; p++)
      if (*p == i)
        goto label;

label:
  if (i != 1)
    abort();
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
// CHECK-NEXT: static mut array: __SlateAlign16<[i32; 10]> = __SlateAlign16([
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT:     1 as i32,
// CHECK-NEXT: ]);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]])
// CHECK-NEXT:         as *mut *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]])
// CHECK-NEXT:         as *mut *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]])
// CHECK-NEXT:         as *mut *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_5:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_5:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_5]]) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[SLATE_SLOT_5]] = 0 as i32;
// CHECK-NEXT:         '__loop_3: loop {
// CHECK-NEXT:             if *[[SLATE_SLOT_5]] < (10 as i32) {
// CHECK-NEXT:                 *[[SLATE_SLOT_4]] = unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(array.0) as *mut i32 }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 };
// CHECK-NEXT:                 loop {
// CHECK-NEXT:                     if *[[SLATE_SLOT_4]]
// CHECK-NEXT:                         != unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(array.0) as *mut i32 }
// CHECK-NEXT:                                 .offset((9 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     {
// CHECK-NEXT:                         if (unsafe { *(*[[SLATE_SLOT_4]]) }) == *[[SLATE_SLOT_5]] {
// CHECK-NEXT:                             break '__loop_3;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             std::ptr::write([[SLATE_SLOT_3]], *[[SLATE_SLOT_4]]);
// CHECK-NEXT:                             std::ptr::write(
// CHECK-NEXT:                                 [[SLATE_SLOT_2]],
// CHECK-NEXT:                                 unsafe { (*[[SLATE_SLOT_3]]).offset((1 as i32) as isize) },
// CHECK-NEXT:                             );
// CHECK-NEXT:                             *[[SLATE_SLOT_4]] = *[[SLATE_SLOT_2]];
// CHECK-NEXT:                         }
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_1]], *[[SLATE_SLOT_5]]);
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_0]], *[[SLATE_SLOT_1]] + (1 as i32));
// CHECK-NEXT:                 *[[SLATE_SLOT_5]] = *[[SLATE_SLOT_0]];
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if *[[SLATE_SLOT_5]] != (1 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
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
