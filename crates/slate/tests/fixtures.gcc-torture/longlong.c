/* Source: PR 321 modified for test suite by Neil Booth 14 Jan 2001.  */

void abort(void);
void exit(int);

typedef unsigned long long uint64;
unsigned long              pars;

uint64  b[32];
uint64 *r = b;

void alpha_ep_extbl_i_eq_0() {
  unsigned int rb, ra, rc;

  rb = (((unsigned long)(pars) >> 27)) & 0x1fUL;
  ra = (((unsigned int)(pars) >> 5)) & 0x1fUL;
  rc = (((unsigned int)(pars) >> 0)) & 0x1fUL;
  {
    uint64 temp = ((r[ra] >> ((r[rb] & 0x7) << 3)) & 0x00000000000000FFLL);
    if (rc != 31)
      r[rc] = temp;
  }
}

int main(void) {
  if (sizeof(uint64) == 8) {
    b[17] = 0x0000000000303882ULL; /* rb */
    b[2]  = 0x534f4f4c494d000aULL; /* ra & rc */

    pars = 0x88000042; /* 17, 2, 2 coded */
    alpha_ep_extbl_i_eq_0();

    if (b[2] != 0x4d)
      abort();
  }

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Source: PR 321 modified for test suite by Neil Booth 14 Jan 2001.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut pars: u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: __SlateAlign16<[u64; 32]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut r: *mut u64 = unsafe { std::ptr::addr_of_mut!(b.0) as *mut u64 };
// CHECK-EMPTY:
// CHECK-NEXT: fn alpha_ep_extbl_i_eq_0() {
// CHECK-NEXT:     let mut rb: u32 = ((unsafe { pars }) >> (27 as i32) & (31 as u64)) as u32;
// CHECK-NEXT:     let mut ra: u32 = (((((unsafe { pars }) as u32) >> (5 as i32)) as u64) & (31 as u64)) as u32;
// CHECK-NEXT:     let mut rc: u32 = (((((unsafe { pars }) as u32) >> (0 as i32)) as u64) & (31 as u64)) as u32;
// CHECK-NEXT:     let mut temp: u64 = (unsafe { *unsafe { unsafe { r }.offset(ra as isize) } })
// CHECK-NEXT:         >> (((unsafe { *unsafe { unsafe { r }.offset(rb as isize) } })
// CHECK-NEXT:             & (((7 as i32) as i64) as u64))
// CHECK-NEXT:             << (3 as i32))
// CHECK-NEXT:         & ((255 as i64) as u64);
// CHECK-NEXT:     if rc != ((31 as i32) as u32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { r }.offset(rc as isize) } = temp;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (8 as u64) == (((8 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(b.0) as *mut u64 }.offset((17 as i32) as isize)
// CHECK-NEXT:             } = 3160194 as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:         // rb
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(b.0) as *mut u64 }.offset((2 as i32) as isize)
// CHECK-NEXT:             } = 6003104017374052362 as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:         // ra & rc
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             pars = (2281701442 as u32) as u64;
// CHECK-NEXT:         }
// CHECK-NEXT:         // 17, 2, 2 coded
// CHECK-NEXT:         alpha_ep_extbl_i_eq_0();
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(b.0) as *mut u64 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != (((77 as i32) as i64) as u64)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
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
