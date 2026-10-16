/* Origin: PR target/6981 from Mattias Engdegaard <mattias@virtutech.se>.  */
/* { dg-require-effective-target int32plus } */

void exit(int);
void abort(void);

unsigned long long *cp, m;

void foo(void) {}

void bar(unsigned rop, unsigned long long *r) {
  unsigned rs1, rs2, rd;

top:
  rs2 = (rop >> 23) & 0x1ff;
  rs1 = (rop >> 9) & 0x1ff;
  rd  = rop & 0x1ff;

  *cp = 1;
  m   = r[rs1] + r[rs2];
  *cp = 2;
  foo();
  if (!rd)
    goto top;
  r[rd] = 1;
}

int main(void) {
  static unsigned long long r[64];
  unsigned long long        cr;
  cp = &cr;

  r[4] = 47;
  r[8] = 11;
  bar((8 << 23) | (4 << 9) | 15, r);

  if (m != 47 + 11)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Origin: PR target/6981 from Mattias Engdegaard <mattias@virtutech.se>.
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut cp: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut m: u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {}
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut rop: u32, mut [[R_0:r_[0-9]+]]: *mut u64) {
// CHECK-NEXT:     let mut [[SLATE_STORAGE_0:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut u32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_0]]) as *mut u32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_1:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_1:__slate_slot_[0-9]+]]: *mut u32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_1]]) as *mut u32;
// CHECK-NEXT:     let mut [[SLATE_STORAGE_2:__slate_storage_[0-9]+]]: std::mem::MaybeUninit<u32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_2:__slate_slot_[0-9]+]]: *mut u32 = std::ptr::addr_of_mut!([[SLATE_STORAGE_2]]) as *mut u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             *[[SLATE_SLOT_1]] = rop >> (23 as i32) & ((511 as i32) as u32);
// CHECK-NEXT:             *[[SLATE_SLOT_2]] = rop >> (9 as i32) & ((511 as i32) as u32);
// CHECK-NEXT:             *[[SLATE_SLOT_0]] = rop & ((511 as i32) as u32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { cp } = ((1 as i32) as i64) as u64;
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 m = unsafe { *unsafe { [[R_0]].offset(*[[SLATE_SLOT_2]] as isize) } }
// CHECK-NEXT:                     .wrapping_add(unsafe { *unsafe { [[R_0]].offset(*[[SLATE_SLOT_1]] as isize) } });
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { cp } = ((2 as i32) as i64) as u64;
// CHECK-NEXT:             }
// CHECK-NEXT:             foo();
// CHECK-NEXT:             if *[[SLATE_SLOT_0]] != (0 as u32) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { [[R_0]].offset(*[[SLATE_SLOT_0]] as isize) } = ((1 as i32) as i64) as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut cr: u64 = 0 as u64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         cp = std::ptr::addr_of_mut!(cr);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(r.0) as *mut u64 }.offset((4 as i32) as isize)
// CHECK-NEXT:         } = ((47 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(r.0) as *mut u64 }.offset((8 as i32) as isize)
// CHECK-NEXT:         } = ((11 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(
// CHECK-NEXT:         ((8 as i32) << (23 as i32) | (4 as i32) << (9 as i32) | (15 as i32)) as u32,
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(r.0) as *mut u64 },
// CHECK-NEXT:     );
// CHECK-NEXT:     if (unsafe { m }) != ((((47 as i32) + (11 as i32)) as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut r: __SlateAlign16<[u64; 64]> = __SlateAlign16(unsafe { std::mem::zeroed() });
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
