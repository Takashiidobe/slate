void add_unwind_adjustsp(long);
void abort(void);

unsigned char bytes[5];

int flag;

void add_unwind_adjustsp(long offset) {
  int           n;
  unsigned long o;

  o = (long)((offset - 0x204) >> 2);

  n = 0;
  do {
  a:
    bytes[n]   = o & 0x7f;
    o        >>= 7;
    if (o) {
      bytes[n] |= 0x80;
      if (flag)
        goto a;
    }
    n++;
  } while (o);
}

int main(void) {
  add_unwind_adjustsp(4132);
  if (bytes[0] != 0x88 || bytes[1] != 0x07)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut bytes: [u8; 5] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut flag: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn add_unwind_adjustsp(mut offset: i64) {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]])
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut u8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut u8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut u8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut u8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut u8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut *mut u8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]])
// CHECK-NEXT:         as *mut *mut u8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_5:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_5:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_5]])
// CHECK-NEXT:         as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_6:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_6:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_6]])
// CHECK-NEXT:         as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_7:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u64> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_7:__slate_slot_[0-9]+]]: *mut u64 = std::ptr::addr_of_mut!([[SLATE_STORAGE_7]]) as *mut u64;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_8:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_8:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_8]]) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[SLATE_SLOT_7]] = (offset - ((516 as i32) as i64) >> (2 as i32)) as u64;
// CHECK-NEXT:         *[[SLATE_SLOT_8]] = 0 as i32;
// CHECK-NEXT:         '__loop_3: loop {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(bytes) as *mut u8 }
// CHECK-NEXT:                         .offset(*[[SLATE_SLOT_8]] as isize)
// CHECK-NEXT:                 } = (*[[SLATE_SLOT_7]] & (((127 as i32) as i64) as u64)) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_6]], *[[SLATE_SLOT_7]]);
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_5]], *[[SLATE_SLOT_6]] >> (7 as i32));
// CHECK-NEXT:             *[[SLATE_SLOT_7]] = *[[SLATE_SLOT_5]];
// CHECK-NEXT:             if *[[SLATE_SLOT_7]] != (0 as u64) {
// CHECK-NEXT:                 std::ptr::write(
// CHECK-NEXT:                     [[SLATE_SLOT_4]],
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(bytes) as *mut u8 }
// CHECK-NEXT:                             .offset(*[[SLATE_SLOT_8]] as isize)
// CHECK-NEXT:                     },
// CHECK-NEXT:                 );
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_3]], unsafe { *(*[[SLATE_SLOT_4]]) });
// CHECK-NEXT:                 std::ptr::write(
// CHECK-NEXT:                     [[SLATE_SLOT_2]],
// CHECK-NEXT:                     ((((*[[SLATE_SLOT_3]] as u32) as i32) | (128 as i32)) as i8) as u8,
// CHECK-NEXT:                 );
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *(*[[SLATE_SLOT_4]]) = *[[SLATE_SLOT_2]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 if (unsafe { flag }) != (0 as i32) {
// CHECK-NEXT:                     continue '__loop_3;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_1]], *[[SLATE_SLOT_8]]);
// CHECK-NEXT:             std::ptr::write([[SLATE_SLOT_0]], *[[SLATE_SLOT_1]] + (1 as i32));
// CHECK-NEXT:             *[[SLATE_SLOT_8]] = *[[SLATE_SLOT_0]];
// CHECK-NEXT:             if !(*[[SLATE_SLOT_7]] != (0 as u64)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     add_unwind_adjustsp((4132 as i32) as i64);
// CHECK-NEXT:     if (((unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(bytes) as *mut u8 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) as u32) as i32) != (136 as i32)
// CHECK-NEXT:         || (((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(bytes) as *mut u8 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as u32) as i32) != (7 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
