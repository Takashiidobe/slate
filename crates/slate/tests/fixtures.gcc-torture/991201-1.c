void abort(void);
void exit(int);

struct vc_data {
  unsigned long space;
  unsigned char vc_palette[16 * 3];
};

struct vc {
  struct vc_data *d;
};

struct vc_data a_con;
struct vc      vc_cons[63] = {&a_con};
int            default_red[16];
int            default_grn[16];
int            default_blu[16];

extern void bar(int);

void reset_palette(int currcons) {
  int j, k;
  for (j = k = 0; j < 16; j++) {
    (vc_cons[currcons].d->vc_palette)[k++] = default_red[j];
    (vc_cons[currcons].d->vc_palette)[k++] = default_grn[j];
    (vc_cons[currcons].d->vc_palette)[k++] = default_blu[j];
  }
  bar(k);
}

void bar(int k) {
  if (k != 16 * 3)
    abort();
}

int main() {
  reset_palette(0);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct vc_data {
// CHECK-NEXT:     space: u64,
// CHECK-NEXT:     vc_palette: [u8; 48],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct vc {
// CHECK-NEXT:     d: *mut vc_data,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut a_con: vc_data = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut vc_cons: __SlateAlign16<[vc; 63]> = __SlateAlign16({
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: [vc; 63] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]][0] = vc {
// CHECK-NEXT:         d: unsafe { std::ptr::addr_of_mut!(a_con) },
// CHECK-NEXT:     };
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: });
// CHECK-NEXT: static mut default_red: __SlateAlign16<[i32; 16]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut default_grn: __SlateAlign16<[i32; 16]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut default_blu: __SlateAlign16<[i32; 16]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn reset_palette(mut currcons: i32) {
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut k: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while j < (16 as i32) {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         k = [[V_2]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(vc_cons.0) as *mut vc }
// CHECK-NEXT:                                 .offset(currcons as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                             .d
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .vc_palette
// CHECK-NEXT:                         .as_mut_ptr() as *mut u8
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset([[V_1]] as isize)
// CHECK-NEXT:             } = ((unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(default_red.0) as *mut i32 }
// CHECK-NEXT:                         .offset(j as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }) as i8) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         k = [[V_4]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(vc_cons.0) as *mut vc }
// CHECK-NEXT:                                 .offset(currcons as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                             .d
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .vc_palette
// CHECK-NEXT:                         .as_mut_ptr() as *mut u8
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset([[V_3]] as isize)
// CHECK-NEXT:             } = ((unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(default_grn.0) as *mut i32 }
// CHECK-NEXT:                         .offset(j as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }) as i8) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         k = [[V_6]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         (*unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(vc_cons.0) as *mut vc }
// CHECK-NEXT:                                 .offset(currcons as isize)
// CHECK-NEXT:                         })
// CHECK-NEXT:                             .d
// CHECK-NEXT:                     })
// CHECK-NEXT:                         .vc_palette
// CHECK-NEXT:                         .as_mut_ptr() as *mut u8
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset([[V_5]] as isize)
// CHECK-NEXT:             } = ((unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(default_blu.0) as *mut i32 }
// CHECK-NEXT:                         .offset(j as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }) as i8) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:         j = [[V_8]];
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(k);
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut k: i32) {
// CHECK-NEXT:     if k != (16 as i32) * (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     reset_palette(0 as i32);
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
