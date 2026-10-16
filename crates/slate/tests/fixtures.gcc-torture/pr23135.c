/* Based on execute/simd-1.c, modified by joern.rennecke@st.com to
   trigger a reload bug.  Verified for gcc mainline from 20050722 13:00 UTC
   for sh-elf -m4 -O2.  */
/* { dg-options "-Wno-psabi -fwrapv" } */
/* { dg-add-options stack_size } */

#ifndef STACK_SIZE
#define STACK_SIZE (256 * 1024)
#endif

extern void abort(void);
extern void exit(int);

typedef struct {
  char c[STACK_SIZE / 2];
} big_t;

typedef int __attribute__((mode(SI))) __attribute__((vector_size(8))) vecint;
typedef int __attribute__((mode(SI)))                                 siint;

vecint i = {150, 100};
vecint j = {10, 13};
vecint k;

union {
  vecint v;
  siint  i[2];
} res;

void verify(siint a1, siint a2, siint b1, siint b2, big_t big) {
  if (a1 != b1 || a2 != b2)
    abort();
}

int main() {
  big_t  big;
  vecint k0, k1, k2, k3, k4, k5, k6, k7;

  k0    = i + j;
  res.v = k0;

  verify(res.i[0], res.i[1], 160, 113, big);

  k1    = i * j;
  res.v = k1;

  verify(res.i[0], res.i[1], 1500, 1300, big);

  k2 = i / j;
  /* This is the observed failure - reload 0 has the wrong type and thus the
     conflict with reload 1 is missed:

  (insn:HI 94 92 96 1 pr23135.c:46 (parallel [
              (set (subreg:SI (reg:DI 253) 0)
                  (div:SI (reg:SI 4 r4)
                      (reg:SI 5 r5)))
              (clobber (reg:SI 146 pr))
              (clobber (reg:DF 64 fr0))
              (clobber (reg:DF 66 fr2))
              (use (reg:PSI 151 ))
              (use (reg/f:SI 256))
          ]) 60 {divsi3_i4} (insn_list:REG_DEP_TRUE 90 (insn_list:REG_DEP_TRUE
  89 (insn_list:REG_DEP_TRUE 42 (insn_list:REG_DEP_TRUE 83
  (insn_list:REG_DEP_TRUE 92 (insn_list:REG_DEP_TRUE 91 (nil)))))))
      (expr_list:REG_DEAD (reg:SI 4 r4)
          (expr_list:REG_DEAD (reg:SI 5 r5)
              (expr_list:REG_UNUSED (reg:DF 66 fr2)
                  (expr_list:REG_UNUSED (reg:DF 64 fr0)
                      (expr_list:REG_UNUSED (reg:SI 146 pr)
                          (insn_list:REG_RETVAL 91 (nil))))))))

  Reloads for insn # 94
  Reload 0: reload_in (SI) = (plus:SI (reg/f:SI 14 r14)
                                                      (const_int 64 [0x40]))
          GENERAL_REGS, RELOAD_FOR_OUTADDR_ADDRESS (opnum = 0)
          reload_in_reg: (plus:SI (reg/f:SI 14 r14)
                                                      (const_int 64 [0x40]))
          reload_reg_rtx: (reg:SI 3 r3)
  Reload 1: GENERAL_REGS, RELOAD_FOR_OUTPUT_ADDRESS (opnum = 0), can't combine,
  se condary_reload_p reload_reg_rtx: (reg:SI 3 r3) Reload 2: reload_out (SI) =
  (mem:SI (plus:SI (plus:SI (reg/f:SI 14 r14) (const_int 64 [0x40])) (const_int
  28 [0x1c])) [ 16 S8 A32]) FPUL_REGS, RELOAD_FOR_OUTPUT (opnum = 0)
          reload_out_reg: (subreg:SI (reg:DI 253) 0)
          reload_reg_rtx: (reg:SI 150 fpul)
          secondary_out_reload = 1

  Reload 3: reload_in (SI) = (symbol_ref:SI ("__sdivsi3_i4") [flags 0x1])
          GENERAL_REGS, RELOAD_FOR_INPUT (opnum = 1), can't combine
          reload_in_reg: (reg/f:SI 256)
          reload_reg_rtx: (reg:SI 3 r3)
    */

  res.v = k2;

  verify(res.i[0], res.i[1], 15, 7, big);

  k3    = i & j;
  res.v = k3;

  verify(res.i[0], res.i[1], 2, 4, big);

  k4    = i | j;
  res.v = k4;

  verify(res.i[0], res.i[1], 158, 109, big);

  k5    = i ^ j;
  res.v = k5;

  verify(res.i[0], res.i[1], 156, 105, big);

  k6    = -i;
  res.v = k6;
  verify(res.i[0], res.i[1], -150, -100, big);

  k7    = ~i;
  res.v = k7;
  verify(res.i[0], res.i[1], -151, -101, big);

  k     = k0 + k1 + k3 + k4 + k5 + k6 + k7;
  res.v = k;
  verify(res.i[0], res.i[1], 1675, 1430, big);

  k     = k0 * k1 * k3 * k4 * k5 * k6 * k7;
  res.v = k;
  verify(res.i[0], res.i[1], 1456467968, -1579586240, big);

  k     = k0 / k1 / k2 / k3 / k4 / k5 / k6 / k7;
  res.v = k;
  verify(res.i[0], res.i[1], 0, 0, big);

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Based on execute/simd-1.c, modified by joern.rennecke@st.com to
// CHECK-NEXT: //! trigger a reload bug.  Verified for gcc mainline from 20050722 13:00 UTC
// CHECK-NEXT: //! for sh-elf -m4 -O2.
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-options "-Wno-psabi -fwrapv" }
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-add-options stack_size }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     c: [i8; 131072],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: std::simd::Simd<i32, 2> = std::simd::Simd::from_array([150 as i32, 100 as i32]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut j: std::simd::Simd<i32, 2> = std::simd::Simd::from_array([10 as i32, 13 as i32]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut k: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     v: std::simd::Simd<i32, 2>,
// CHECK-NEXT:     i: [i32; 2],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut res: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn verify(mut a1: i32, mut a2: i32, mut b1: i32, mut b2: i32, mut big: [[SLATERECORD_0]]) {
// CHECK-NEXT:     if a1 != b1 || a2 != b2 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut big: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut k0: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut k1: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut k2: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut k3: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut k4: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut k5: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut k6: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut k7: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     k0 = (unsafe { i }) + unsafe { j };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = k0;
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         160 as i32,
// CHECK-NEXT:         113 as i32,
// CHECK-NEXT:         big,
// CHECK-NEXT:     );
// CHECK-NEXT:     k1 = (unsafe { i }) * unsafe { j };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = k1;
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         1500 as i32,
// CHECK-NEXT:         1300 as i32,
// CHECK-NEXT:         big,
// CHECK-NEXT:     );
// CHECK-NEXT:     k2 = (unsafe { i }) / unsafe { j };
// CHECK-NEXT:     // This is the observed failure - reload 0 has the wrong type and thus the
// CHECK-NEXT:     //    conflict with reload 1 is missed:
// CHECK-NEXT:     //
// CHECK-NEXT:     // (insn:HI 94 92 96 1 pr23135.c:46 (parallel [
// CHECK-NEXT:     //             (set (subreg:SI (reg:DI 253) 0)
// CHECK-NEXT:     //                 (div:SI (reg:SI 4 r4)
// CHECK-NEXT:     //                     (reg:SI 5 r5)))
// CHECK-NEXT:     //             (clobber (reg:SI 146 pr))
// CHECK-NEXT:     //             (clobber (reg:DF 64 fr0))
// CHECK-NEXT:     //             (clobber (reg:DF 66 fr2))
// CHECK-NEXT:     //             (use (reg:PSI 151 ))
// CHECK-NEXT:     //             (use (reg/f:SI 256))
// CHECK-NEXT:     //         ]) 60 {divsi3_i4} (insn_list:REG_DEP_TRUE 90 (insn_list:REG_DEP_TRUE
// CHECK-NEXT:     // 89 (insn_list:REG_DEP_TRUE 42 (insn_list:REG_DEP_TRUE 83
// CHECK-NEXT:     // (insn_list:REG_DEP_TRUE 92 (insn_list:REG_DEP_TRUE 91 (nil)))))))
// CHECK-NEXT:     //     (expr_list:REG_DEAD (reg:SI 4 r4)
// CHECK-NEXT:     //         (expr_list:REG_DEAD (reg:SI 5 r5)
// CHECK-NEXT:     //             (expr_list:REG_UNUSED (reg:DF 66 fr2)
// CHECK-NEXT:     //                 (expr_list:REG_UNUSED (reg:DF 64 fr0)
// CHECK-NEXT:     //                     (expr_list:REG_UNUSED (reg:SI 146 pr)
// CHECK-NEXT:     //                         (insn_list:REG_RETVAL 91 (nil))))))))
// CHECK-NEXT:     //
// CHECK-NEXT:     // Reloads for insn # 94
// CHECK-NEXT:     // Reload 0: reload_in (SI) = (plus:SI (reg/f:SI 14 r14)
// CHECK-NEXT:     //                                                     (const_int 64 [0x40]))
// CHECK-NEXT:     //         GENERAL_REGS, RELOAD_FOR_OUTADDR_ADDRESS (opnum = 0)
// CHECK-NEXT:     //         reload_in_reg: (plus:SI (reg/f:SI 14 r14)
// CHECK-NEXT:     //                                                     (const_int 64 [0x40]))
// CHECK-NEXT:     //         reload_reg_rtx: (reg:SI 3 r3)
// CHECK-NEXT:     // Reload 1: GENERAL_REGS, RELOAD_FOR_OUTPUT_ADDRESS (opnum = 0), can't combine,
// CHECK-NEXT:     // se condary_reload_p reload_reg_rtx: (reg:SI 3 r3) Reload 2: reload_out (SI) =
// CHECK-NEXT:     // (mem:SI (plus:SI (plus:SI (reg/f:SI 14 r14) (const_int 64 [0x40])) (const_int
// CHECK-NEXT:     // 28 [0x1c])) [ 16 S8 A32]) FPUL_REGS, RELOAD_FOR_OUTPUT (opnum = 0)
// CHECK-NEXT:     //         reload_out_reg: (subreg:SI (reg:DI 253) 0)
// CHECK-NEXT:     //         reload_reg_rtx: (reg:SI 150 fpul)
// CHECK-NEXT:     //         secondary_out_reload = 1
// CHECK-NEXT:     //
// CHECK-NEXT:     // Reload 3: reload_in (SI) = (symbol_ref:SI ("__sdivsi3_i4") [flags 0x1])
// CHECK-NEXT:     //         GENERAL_REGS, RELOAD_FOR_INPUT (opnum = 1), can't combine
// CHECK-NEXT:     //         reload_in_reg: (reg/f:SI 256)
// CHECK-NEXT:     //         reload_reg_rtx: (reg:SI 3 r3)
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = k2;
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         15 as i32,
// CHECK-NEXT:         7 as i32,
// CHECK-NEXT:         big,
// CHECK-NEXT:     );
// CHECK-NEXT:     k3 = (unsafe { i }) & unsafe { j };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = k3;
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         big,
// CHECK-NEXT:     );
// CHECK-NEXT:     k4 = (unsafe { i }) | unsafe { j };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = k4;
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         158 as i32,
// CHECK-NEXT:         109 as i32,
// CHECK-NEXT:         big,
// CHECK-NEXT:     );
// CHECK-NEXT:     k5 = (unsafe { i }) ^ unsafe { j };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = k5;
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         156 as i32,
// CHECK-NEXT:         105 as i32,
// CHECK-NEXT:         big,
// CHECK-NEXT:     );
// CHECK-NEXT:     k6 = -unsafe { i };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = k6;
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         (150 as i32).wrapping_neg(),
// CHECK-NEXT:         (100 as i32).wrapping_neg(),
// CHECK-NEXT:         big,
// CHECK-NEXT:     );
// CHECK-NEXT:     k7 = !unsafe { i };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = k7;
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         (151 as i32).wrapping_neg(),
// CHECK-NEXT:         (101 as i32).wrapping_neg(),
// CHECK-NEXT:         big,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         k = k0 + k1 + k3 + k4 + k5 + k6 + k7;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = unsafe { k };
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         1675 as i32,
// CHECK-NEXT:         1430 as i32,
// CHECK-NEXT:         big,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         k = k0 * k1 * k3 * k4 * k5 * k6 * k7;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = unsafe { k };
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         1456467968 as i32,
// CHECK-NEXT:         (1579586240 as i32).wrapping_neg(),
// CHECK-NEXT:         big,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         k = k0 / k1 / k2 / k3 / k4 / k5 / k6 / k7;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         res.v = unsafe { k };
// CHECK-NEXT:     }
// CHECK-NEXT:     verify(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(res.i) as *mut i32 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         },
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         big,
// CHECK-NEXT:     );
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
