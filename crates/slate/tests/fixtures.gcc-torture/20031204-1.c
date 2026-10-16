/* PR optimization/13260 */

#include <string.h>

void abort(void);

typedef unsigned long u32;

u32 in_aton(const char *x) { return 0x0a0b0c0d; }

u32 root_nfs_parse_addr(char *name) {
  u32   addr;
  int   octets = 0;
  char *cp, *cq;

  cp = cq = name;
  while (octets < 4) {
    while (*cp >= '0' && *cp <= '9')
      cp++;
    if (cp == cq || cp - cq > 3)
      break;
    if (*cp == '.' || octets == 3)
      octets++;
    if (octets < 4)
      cp++;
    cq = cp;
  }

  if (octets == 4 && (*cp == ':' || *cp == '\0')) {
    if (*cp == ':')
      *cp++ = '\0';
    addr = in_aton(name);
    strcpy(name, cp);
  } else
    addr = (-1);

  return addr;
}

int main() {
  static char addr[] = "10.11.12.13:/hello";
  u32         result = root_nfs_parse_addr(addr);
  if (result != 0x0a0b0c0d) {
    abort();
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR optimization/13260
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn strcpy(__dest: *mut i8, __src: *const i8) -> *mut i8;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn in_aton(mut x: *const i8) -> u64 {
// CHECK-NEXT:     ((168496141 as i32) as i64) as u64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn root_nfs_parse_addr(mut name: *mut i8) -> u64 {
// CHECK-NEXT:     let mut [[ADDR_0:addr_[0-9]+]]: u64 = 0 as u64;
// CHECK-NEXT:     let mut octets: i32 = 0 as i32;
// CHECK-NEXT:     let mut cp: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut cq: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: *mut i8 = name;
// CHECK-NEXT:     cq = [[V_0]];
// CHECK-NEXT:     cp = [[V_0]];
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while octets < (4 as i32) {
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while ((unsafe { *cp }) as i32) >= (48 as i32)
// CHECK-NEXT:             && ((unsafe { *cp }) as i32) <= (57 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: *mut i8 = cp;
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: *mut i8 = unsafe { [[V_1]].offset((1 as i32) as isize) };
// CHECK-NEXT:             cp = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if cp == cq || ((unsafe { cp.offset_from(cq as *mut i8) }) as i64) > ((3 as i32) as i64) {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if ((unsafe { *cp }) as i32) == (46 as i32) || octets == (3 as i32) {
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = octets;
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:             octets = [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if octets < (4 as i32) {
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: *mut i8 = cp;
// CHECK-NEXT:             let mut [[V_6:__v[0-9]+]]: *mut i8 = unsafe { [[V_5]].offset((1 as i32) as isize) };
// CHECK-NEXT:             cp = [[V_6]];
// CHECK-NEXT:         }
// CHECK-NEXT:         cq = cp;
// CHECK-NEXT:     }
// CHECK-NEXT:     if octets == (4 as i32)
// CHECK-NEXT:         && (((unsafe { *cp }) as i32) == (58 as i32) || ((unsafe { *cp }) as i32) == (0 as i32))
// CHECK-NEXT:     {
// CHECK-NEXT:         if ((unsafe { *cp }) as i32) == (58 as i32) {
// CHECK-NEXT:             let mut [[V_7:__v[0-9]+]]: *mut i8 = cp;
// CHECK-NEXT:             let mut [[V_8:__v[0-9]+]]: *mut i8 = unsafe { [[V_7]].offset((1 as i32) as isize) };
// CHECK-NEXT:             cp = [[V_8]];
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *[[V_7]] = (0 as i32) as i8;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         [[ADDR_0]] = in_aton(name as *const i8);
// CHECK-NEXT:         unsafe { strcpy(name, cp as *const i8) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[ADDR_0]] = (-(1 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     [[ADDR_0]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut result: u64 = root_nfs_parse_addr(unsafe { std::ptr::addr_of_mut!(addr.0) as *mut i8 });
// CHECK-NEXT:     if result != (((168496141 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut addr: __SlateAlign16<[i8; 19]> = __SlateAlign16([
// CHECK-NEXT:     49 as i8, 48 as i8, 46 as i8, 49 as i8, 49 as i8, 46 as i8, 49 as i8, 50 as i8, 46 as i8,
// CHECK-NEXT:     49 as i8, 51 as i8, 58 as i8, 47 as i8, 104 as i8, 101 as i8, 108 as i8, 108 as i8, 111 as i8,
// CHECK-NEXT:     0 as i8,
// CHECK-NEXT: ]);
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
