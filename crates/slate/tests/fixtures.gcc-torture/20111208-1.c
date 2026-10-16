/* PR tree-optimization/51315 */
/* Reported by Jurij Smakov <jurij@wooyd.org> */

typedef __SIZE_TYPE__ size_t;

extern void *memcpy(void *__restrict __dest, __const void *__restrict __src,
                    size_t __n) __attribute__((__nothrow__))
__attribute__((__nonnull__(1, 2)));

extern size_t strlen(__const char *__s) __attribute__((__nothrow__))
__attribute__((__pure__)) __attribute__((__nonnull__(1)));

typedef __INT16_TYPE__ int16_t;
typedef __INT32_TYPE__ int32_t;

extern void abort(void);

int a;

static void __attribute__((noinline, noclone)) do_something(int item) {
  a = item;
}

int pack_unpack(char *s, char *p) {
  char *send, *pend;
  char  type;
  int   integer_size;

  send = s + strlen(s);
  pend = p + strlen(p);

  while (p < pend) {
    type = *p++;

    switch (type) {
    case 's':
      integer_size = 2;
      goto unpack_integer;

    case 'l':
      integer_size = 4;
      goto unpack_integer;

    unpack_integer:
      switch (integer_size) {
      case 2: {
        union {
          int16_t i;
          char    a[sizeof(int16_t)];
        } v;
        memcpy(v.a, s, sizeof(int16_t));
        s += sizeof(int16_t);
        do_something(v.i);
      } break;

      case 4: {
        union {
          int32_t i;
          char    a[sizeof(int32_t)];
        } v;
        memcpy(v.a, s, sizeof(int32_t));
        s += sizeof(int32_t);
        do_something(v.i);
      } break;
      }
      break;
    }
  }
  return (int)*s;
}

int main(void) {
  int n = pack_unpack("\200\001\377\376\035\300", "sl");
  if (n != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/51315
// CHECK-NEXT: //!
// CHECK-NEXT: //! Reported by Jurij Smakov <jurij@wooyd.org>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT:     fn strlen(__s: *const i8) -> u64;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     i: i16,
// CHECK-NEXT:     a: [i8; 2],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT:     a: [i8; 4],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn do_something(mut item: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = item;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn pack_unpack(mut s: *mut i8, mut p: *mut i8) -> i32 {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]]) as *mut *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<[[SLATERECORD_1]]> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut [[SLATERECORD_1]] =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut [[SLATERECORD_1]];
// CHECK-NEXT:     let mut [[SLATE_STORAGE_3:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_3:__slate_slot_[0-9]+]]: *mut *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_3]]) as *mut *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_4:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_4:__slate_slot_[0-9]+]]: *mut *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_4]]) as *mut *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_5:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<[[SLATERECORD_0]]> =
// CHECK-NEXT:         std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_5:__slate_slot_[0-9]+]]: *mut [[SLATERECORD_0]] =
// CHECK-NEXT:         std::ptr::addr_of_mut!([[SLATE_STORAGE_5]]) as *mut [[SLATERECORD_0]];
// CHECK-NEXT:     let mut [[SLATE_STORAGE_6:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_6:__slate_slot_[0-9]+]]: *mut *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_6]]) as *mut *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_7:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_7:__slate_slot_[0-9]+]]: *mut *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_7]]) as *mut *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_8:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_8:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_8]]) as *mut i32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_9:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_9:__slate_slot_[0-9]+]]: *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_9]]) as *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_10:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_10:__slate_slot_[0-9]+]]: *mut *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_10]]) as *mut *mut i8;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_11:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<*mut i8> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_11:__slate_slot_[0-9]+]]: *mut *mut i8 = std::ptr::addr_of_mut!([[SLATE_STORAGE_11]]) as *mut *mut i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[SLATE_SLOT_11]] = unsafe { s.offset((unsafe { strlen(s as *const i8) }) as isize) };
// CHECK-NEXT:         *[[SLATE_SLOT_10]] = unsafe { p.offset((unsafe { strlen(p as *const i8) }) as isize) };
// CHECK-NEXT:         '__loop_1: loop {
// CHECK-NEXT:             if p < *[[SLATE_SLOT_10]] {
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_7]], p);
// CHECK-NEXT:                 std::ptr::write([[SLATE_SLOT_6]], unsafe {
// CHECK-NEXT:                     (*[[SLATE_SLOT_7]]).offset((1 as i32) as isize)
// CHECK-NEXT:                 });
// CHECK-NEXT:                 p = *[[SLATE_SLOT_6]];
// CHECK-NEXT:                 *[[SLATE_SLOT_9]] = unsafe { *(*[[SLATE_SLOT_7]]) };
// CHECK-NEXT:                 let [[T_0:__t[0-9]+]]: i32 = *[[SLATE_SLOT_9]] as i32;
// CHECK-NEXT:                 if [[T_0]] == (115 as i32) {
// CHECK-NEXT:                     *[[SLATE_SLOT_8]] = 2 as i32;
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if [[T_0]] == (108 as i32) {
// CHECK-NEXT:                         *[[SLATE_SLOT_8]] = 4 as i32;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         continue '__loop_1;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let [[T_1:__t[0-9]+]]: i32 = *[[SLATE_SLOT_8]];
// CHECK-NEXT:                 if [[T_1]] == (2 as i32) {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         memcpy(
// CHECK-NEXT:                             (unsafe { (*[[SLATE_SLOT_5]]).a.as_mut_ptr() as *mut i8 }) as *mut (),
// CHECK-NEXT:                             s as *const (),
// CHECK-NEXT:                             2 as u64,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     };
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_4]], s);
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_3]], unsafe {
// CHECK-NEXT:                         (*[[SLATE_SLOT_4]]).offset((2 as u64) as isize)
// CHECK-NEXT:                     });
// CHECK-NEXT:                     s = *[[SLATE_SLOT_3]];
// CHECK-NEXT:                     do_something((unsafe { (*[[SLATE_SLOT_5]]).i }) as i32);
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if [[T_1]] == (4 as i32) {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             memcpy(
// CHECK-NEXT:                                 (unsafe { (*[[SLATE_SLOT_2]]).a.as_mut_ptr() as *mut i8 })
// CHECK-NEXT:                                     as *mut (),
// CHECK-NEXT:                                 s as *const (),
// CHECK-NEXT:                                 4 as u64,
// CHECK-NEXT:                             )
// CHECK-NEXT:                         };
// CHECK-NEXT:                         std::ptr::write([[SLATE_SLOT_1]], s);
// CHECK-NEXT:                         std::ptr::write([[SLATE_SLOT_0]], unsafe {
// CHECK-NEXT:                             (*[[SLATE_SLOT_1]]).offset((4 as u64) as isize)
// CHECK-NEXT:                         });
// CHECK-NEXT:                         s = *[[SLATE_SLOT_0]];
// CHECK-NEXT:                         do_something(unsafe { (*[[SLATE_SLOT_2]]).i });
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return (unsafe { *s }) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut n: i32 = pack_unpack(
// CHECK-NEXT:         c"\x80\x01\xff\xfe\x1d\xc0".as_ptr() as *mut i8,
// CHECK-NEXT:         c"sl".as_ptr() as *mut i8,
// CHECK-NEXT:     );
// CHECK-NEXT:     if n != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
