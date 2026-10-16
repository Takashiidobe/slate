extern void abort(void);

typedef unsigned long HARD_REG_SET[2];
HARD_REG_SET          reg_class_contents[2];

struct du_chain {
  struct du_chain *next_use;
  int              cl;
};

void __attribute__((noinline)) merge_overlapping_regs(HARD_REG_SET *p) {
  if ((*p)[0] != -1 || (*p)[1] != -1)
    abort();
}

void __attribute__((noinline)) regrename_optimize(struct du_chain *this) {
  HARD_REG_SET     this_unavailable;
  unsigned long   *scan_fp_;
  int              n_uses;
  struct du_chain *last;

  this_unavailable[0] = 0;
  this_unavailable[1] = 0;

  n_uses = 0;
  for (last = this; last->next_use; last = last->next_use) {
    scan_fp_ = reg_class_contents[last->cl];
    n_uses++;
    this_unavailable[0] |= ~scan_fp_[0];
    this_unavailable[1] |= ~scan_fp_[1];
  }
  if (n_uses < 1)
    return;

  scan_fp_             = reg_class_contents[last->cl];
  this_unavailable[0] |= ~scan_fp_[0];
  this_unavailable[1] |= ~scan_fp_[1];

  merge_overlapping_regs(&this_unavailable);
}

int main() {
  struct du_chain du1      = {0, 0};
  struct du_chain du0      = {&du1, 1};
  reg_class_contents[0][0] = -1;
  reg_class_contents[0][1] = -1;
  reg_class_contents[1][0] = 0;
  reg_class_contents[1][1] = 0;
  regrename_optimize(&du0);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut reg_class_contents: __SlateAlign16<{{\[\[}}u64; 2]; 2]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct du_chain {
// CHECK-NEXT:     next_use: *mut du_chain,
// CHECK-NEXT:     cl: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn merge_overlapping_regs(mut p: &mut [u64; 2]) {
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe { (*(p as *mut [u64; 2])).as_mut_ptr() as *mut u64 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     }) != ((-(1 as i32) as i64) as u64)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { (*(p as *mut [u64; 2])).as_mut_ptr() as *mut u64 }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != ((-(1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn regrename_optimize(mut this: &du_chain) {
// CHECK-NEXT:     let mut this_unavailable: __SlateAlign16<[u64; 2]> = __SlateAlign16([0 as u64; 2]);
// CHECK-NEXT:     let mut scan_fp_: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut n_uses: i32 = 0 as i32;
// CHECK-NEXT:     let mut last: *mut du_chain = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             (this_unavailable.0.as_mut_ptr() as *mut u64).offset((0 as i32) as isize)
// CHECK-NEXT:         } = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             (this_unavailable.0.as_mut_ptr() as *mut u64).offset((1 as i32) as isize)
// CHECK-NEXT:         } = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     last = (this as *const du_chain) as *mut du_chain;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { (*last).next_use })
// CHECK-NEXT:         != std::ptr::null_mut::<du_chain>()
// CHECK-NEXT:     {
// CHECK-NEXT:         scan_fp_ = unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(reg_class_contents.0) as *mut [u64; 2] }
// CHECK-NEXT:                     .offset((unsafe { (*last).cl }) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:                 .as_mut_ptr() as *mut u64
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = n_uses;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         n_uses = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: *mut u64 = unsafe {
// CHECK-NEXT:             (this_unavailable.0.as_mut_ptr() as *mut u64).offset((0 as i32) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u64 = unsafe { *[[V_2]] };
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: u64 = [[V_3]]
// CHECK-NEXT:             | !unsafe { *unsafe { scan_fp_.offset((0 as i32) as isize) } };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_2]] = [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: *mut u64 = unsafe {
// CHECK-NEXT:             (this_unavailable.0.as_mut_ptr() as *mut u64).offset((1 as i32) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: u64 = unsafe { *[[V_5]] };
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: u64 = [[V_6]]
// CHECK-NEXT:             | !unsafe { *unsafe { scan_fp_.offset((1 as i32) as isize) } };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_5]] = [[V_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         last = unsafe { (*last).next_use };
// CHECK-NEXT:     }
// CHECK-NEXT:     if n_uses < (1 as i32) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     scan_fp_ = unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(reg_class_contents.0) as *mut [u64; 2] }
// CHECK-NEXT:                 .offset((unsafe { (*last).cl }) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:             .as_mut_ptr() as *mut u64
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: *mut u64 = unsafe {
// CHECK-NEXT:         (this_unavailable.0.as_mut_ptr() as *mut u64).offset((0 as i32) as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: u64 = unsafe { *[[V_8]] };
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: u64 = [[V_9]]
// CHECK-NEXT:         | !unsafe { *unsafe { scan_fp_.offset((0 as i32) as isize) } };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_8]] = [[V_10]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: *mut u64 = unsafe {
// CHECK-NEXT:         (this_unavailable.0.as_mut_ptr() as *mut u64).offset((1 as i32) as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: u64 = unsafe { *[[V_11]] };
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: u64 = [[V_12]]
// CHECK-NEXT:         | !unsafe { *unsafe { scan_fp_.offset((1 as i32) as isize) } };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_11]] = [[V_13]];
// CHECK-NEXT:     }
// CHECK-NEXT:     merge_overlapping_regs(unsafe {
// CHECK-NEXT:         &mut (*std::ptr::addr_of_mut!(this_unavailable.0))
// CHECK-NEXT:     });
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut du1: du_chain = du_chain {
// CHECK-NEXT:         next_use: std::ptr::null_mut::<du_chain>(),
// CHECK-NEXT:         cl: 0 as i32,
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut du0: du_chain = du_chain {
// CHECK-NEXT:         next_use: std::ptr::addr_of_mut!(du1),
// CHECK-NEXT:         cl: 1 as i32,
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::ptr::addr_of_mut!(reg_class_contents.0) as *mut [u64; 2]
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .as_mut_ptr() as *mut u64
// CHECK-NEXT:             }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         } = (-(1 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::ptr::addr_of_mut!(reg_class_contents.0) as *mut [u64; 2]
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .as_mut_ptr() as *mut u64
// CHECK-NEXT:             }
// CHECK-NEXT:                 .offset((1 as i32) as isize)
// CHECK-NEXT:         } = (-(1 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::ptr::addr_of_mut!(reg_class_contents.0) as *mut [u64; 2]
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .as_mut_ptr() as *mut u64
// CHECK-NEXT:             }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         } = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::ptr::addr_of_mut!(reg_class_contents.0) as *mut [u64; 2]
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .offset((1 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .as_mut_ptr() as *mut u64
// CHECK-NEXT:             }
// CHECK-NEXT:                 .offset((1 as i32) as isize)
// CHECK-NEXT:         } = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     regrename_optimize(unsafe { &(*std::ptr::addr_of_mut!(du0)) });
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
