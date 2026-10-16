extern void abort(void);

typedef struct PgHdr  PgHdr;
typedef unsigned char u8;
struct PgHdr {
  int y;
  struct {
    unsigned int pgno;
    PgHdr       *pNextHash, *pPrevHash;
    PgHdr       *pNextFree, *pPrevFree;
    PgHdr       *pNextAll;
    u8           inJournal;
    short int    nRef;
    PgHdr       *pDirty, *pPrevDirty;
    unsigned int notUsed;
  } x;
};
PgHdr              **xx;
volatile int         vx;
static inline PgHdr *merge_pagelist(PgHdr *pA, PgHdr *pB) {
  PgHdr  result;
  PgHdr *pTail;
  xx    = &result.x.pDirty;
  pTail = &result;
  while (pA && pB) {
    if (pA->x.pgno < pB->x.pgno) {
      pTail->x.pDirty = pA;
      pTail           = pA;
      pA              = pA->x.pDirty;
    } else {
      pTail->x.pDirty = pB;
      pTail           = pB;
      pB              = pB->x.pDirty;
    }
    vx = (*xx)->y;
  }
  if (pA) {
    pTail->x.pDirty = pA;
  } else if (pB) {
    pTail->x.pDirty = pB;
  } else {
    pTail->x.pDirty = 0;
  }
  return result.x.pDirty;
}

PgHdr *__attribute__((noinline)) sort_pagelist(PgHdr *pIn) {
  PgHdr *a[25], *p;
  int    i;
  __builtin_memset(a, 0, sizeof(a));
  while (pIn) {
    p           = pIn;
    pIn         = p->x.pDirty;
    p->x.pDirty = 0;
    for (i = 0; i < 25 - 1; i++) {
      if (a[i] == 0) {
        a[i] = p;
        break;
      } else {
        p    = merge_pagelist(a[i], p);
        a[i] = 0;
        a[i] = 0;
      }
    }
    if (i == 25 - 1) {
      a[i] = merge_pagelist(a[i], p);
    }
  }
  p = a[0];
  for (i = 1; i < 25; i++) {
    p = merge_pagelist(p, a[i]);
  }
  return p;
}

int main() {
  PgHdr  a[5];
  PgHdr *p;
  a[0].x.pgno   = 5;
  a[0].x.pDirty = &a[1];
  a[1].x.pgno   = 4;
  a[1].x.pDirty = &a[2];
  a[2].x.pgno   = 1;
  a[2].x.pDirty = &a[3];
  a[3].x.pgno   = 3;
  a[3].x.pDirty = 0;
  p             = sort_pagelist(&a[0]);
  if (p->x.pDirty == p)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: i32, [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct PgHdr {
// CHECK-NEXT:     y: i32,
// CHECK-NEXT:     x: [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     pgno: u32,
// CHECK-NEXT:     pNextHash: *mut PgHdr,
// CHECK-NEXT:     pPrevHash: *mut PgHdr,
// CHECK-NEXT:     pNextFree: *mut PgHdr,
// CHECK-NEXT:     pPrevFree: *mut PgHdr,
// CHECK-NEXT:     pNextAll: *mut PgHdr,
// CHECK-NEXT:     inJournal: u8,
// CHECK-NEXT:     nRef: i16,
// CHECK-NEXT:     pDirty: *mut PgHdr,
// CHECK-NEXT:     pPrevDirty: *mut PgHdr,
// CHECK-NEXT:     notUsed: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut xx: *mut *mut PgHdr = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut vx: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn merge_pagelist(mut pA: *mut PgHdr, mut pB: *mut PgHdr) -> *mut PgHdr {
// CHECK-NEXT:     let mut result: PgHdr = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut pTail: *mut PgHdr = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         xx = std::ptr::addr_of_mut!(result.x.pDirty);
// CHECK-NEXT:     }
// CHECK-NEXT:     pTail = std::ptr::addr_of_mut!(result);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while pA != std::ptr::null_mut::<PgHdr>()
// CHECK-NEXT:         && pB != std::ptr::null_mut::<PgHdr>()
// CHECK-NEXT:     {
// CHECK-NEXT:         if (unsafe { (*pA).x.pgno }) < unsafe { (*pB).x.pgno } {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*pTail).x.pDirty = pA;
// CHECK-NEXT:             }
// CHECK-NEXT:             pTail = pA;
// CHECK-NEXT:             pA = unsafe { (*pA).x.pDirty };
// CHECK-NEXT:         } else {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*pTail).x.pDirty = pB;
// CHECK-NEXT:             }
// CHECK-NEXT:             pTail = pB;
// CHECK-NEXT:             pB = unsafe { (*pB).x.pDirty };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::ptr::write_volatile(
// CHECK-NEXT:                 std::ptr::addr_of_mut!(vx),
// CHECK-NEXT:                 unsafe { (*unsafe { *unsafe { xx } }).y },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     if pA != std::ptr::null_mut::<PgHdr>() {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*pTail).x.pDirty = pA;
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if pB != std::ptr::null_mut::<PgHdr>() {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*pTail).x.pDirty = pB;
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*pTail).x.pDirty = std::ptr::null_mut::<PgHdr>();
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     result.x.pDirty
// CHECK-NEXT: }
// CHECK-NEXT: fn sort_pagelist(mut pIn: *mut PgHdr) -> *mut PgHdr {
// CHECK-NEXT:     let mut a: __SlateAlign16<[*mut PgHdr; 25]> = __SlateAlign16([0 as *mut PgHdr; 25]);
// CHECK-NEXT:     let mut p: *mut PgHdr = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     unsafe { __builtin_memset(a.0.as_mut_ptr() as *mut (), 0 as i32, 200 as u64) };
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while pIn != std::ptr::null_mut::<PgHdr>() {
// CHECK-NEXT:         p = pIn;
// CHECK-NEXT:         pIn = unsafe { (*p).x.pDirty };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*p).x.pDirty = std::ptr::null_mut::<PgHdr>();
// CHECK-NEXT:         }
// CHECK-NEXT:         i = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (25 as i32) - (1 as i32) {
// CHECK-NEXT:             if (unsafe {
// CHECK-NEXT:                 *unsafe { (a.0.as_mut_ptr() as *mut *mut PgHdr).offset(i as isize) }
// CHECK-NEXT:             }) == std::ptr::null_mut::<PgHdr>()
// CHECK-NEXT:             {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (a.0.as_mut_ptr() as *mut *mut PgHdr).offset(i as isize)
// CHECK-NEXT:                     } = p;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 break '[[SLATE_BREAK_2]];
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 p = merge_pagelist(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (a.0.as_mut_ptr() as *mut *mut PgHdr).offset(i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     },
// CHECK-NEXT:                     p,
// CHECK-NEXT:                 );
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (a.0.as_mut_ptr() as *mut *mut PgHdr).offset(i as isize)
// CHECK-NEXT:                     } = std::ptr::null_mut::<PgHdr>();
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (a.0.as_mut_ptr() as *mut *mut PgHdr).offset(i as isize)
// CHECK-NEXT:                     } = std::ptr::null_mut::<PgHdr>();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:             i = [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if i == (25 as i32) - (1 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (a.0.as_mut_ptr() as *mut *mut PgHdr).offset(i as isize) } = merge_pagelist(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             (a.0.as_mut_ptr() as *mut *mut PgHdr).offset(i as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     },
// CHECK-NEXT:                     p,
// CHECK-NEXT:                 );
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     p = unsafe {
// CHECK-NEXT:         *unsafe { (a.0.as_mut_ptr() as *mut *mut PgHdr).offset((0 as i32) as isize) }
// CHECK-NEXT:     };
// CHECK-NEXT:     i = 1 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while i < (25 as i32) {
// CHECK-NEXT:         p = merge_pagelist(
// CHECK-NEXT:             p,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (a.0.as_mut_ptr() as *mut *mut PgHdr).offset(i as isize) }
// CHECK-NEXT:             },
// CHECK-NEXT:         );
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT:     p
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[PgHdr; 5]> = __SlateAlign16(unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut p: *mut PgHdr = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (a.0.as_mut_ptr() as *mut PgHdr).offset((0 as i32) as isize) })
// CHECK-NEXT:             .x
// CHECK-NEXT:             .pgno = (5 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (a.0.as_mut_ptr() as *mut PgHdr).offset((0 as i32) as isize) })
// CHECK-NEXT:             .x
// CHECK-NEXT:             .pDirty = unsafe {
// CHECK-NEXT:             (a.0.as_mut_ptr() as *mut PgHdr).offset((1 as i32) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (a.0.as_mut_ptr() as *mut PgHdr).offset((1 as i32) as isize) })
// CHECK-NEXT:             .x
// CHECK-NEXT:             .pgno = (4 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (a.0.as_mut_ptr() as *mut PgHdr).offset((1 as i32) as isize) })
// CHECK-NEXT:             .x
// CHECK-NEXT:             .pDirty = unsafe {
// CHECK-NEXT:             (a.0.as_mut_ptr() as *mut PgHdr).offset((2 as i32) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (a.0.as_mut_ptr() as *mut PgHdr).offset((2 as i32) as isize) })
// CHECK-NEXT:             .x
// CHECK-NEXT:             .pgno = (1 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (a.0.as_mut_ptr() as *mut PgHdr).offset((2 as i32) as isize) })
// CHECK-NEXT:             .x
// CHECK-NEXT:             .pDirty = unsafe {
// CHECK-NEXT:             (a.0.as_mut_ptr() as *mut PgHdr).offset((3 as i32) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (a.0.as_mut_ptr() as *mut PgHdr).offset((3 as i32) as isize) })
// CHECK-NEXT:             .x
// CHECK-NEXT:             .pgno = (3 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (a.0.as_mut_ptr() as *mut PgHdr).offset((3 as i32) as isize) })
// CHECK-NEXT:             .x
// CHECK-NEXT:             .pDirty = std::ptr::null_mut::<PgHdr>();
// CHECK-NEXT:     }
// CHECK-NEXT:     p = sort_pagelist(unsafe {
// CHECK-NEXT:         (a.0.as_mut_ptr() as *mut PgHdr).offset((0 as i32) as isize)
// CHECK-NEXT:     });
// CHECK-NEXT:     if (unsafe { (*p).x.pDirty }) == p {
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
