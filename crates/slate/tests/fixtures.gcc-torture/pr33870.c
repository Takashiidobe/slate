extern void abort(void);

typedef struct PgHdr  PgHdr;
typedef unsigned char u8;
struct PgHdr {
  unsigned int pgno;
  PgHdr       *pNextHash, *pPrevHash;
  PgHdr       *pNextFree, *pPrevFree;
  PgHdr       *pNextAll;
  u8           inJournal;
  short int    nRef;
  PgHdr       *pDirty, *pPrevDirty;
  unsigned int notUsed;
};

static inline PgHdr *merge_pagelist(PgHdr *pA, PgHdr *pB) {
  PgHdr  result;
  PgHdr *pTail;
  pTail = &result;
  while (pA && pB) {
    if (pA->pgno < pB->pgno) {
      pTail->pDirty = pA;
      pTail         = pA;
      pA            = pA->pDirty;
    } else {
      pTail->pDirty = pB;
      pTail         = pB;
      pB            = pB->pDirty;
    }
  }
  if (pA) {
    pTail->pDirty = pA;
  } else if (pB) {
    pTail->pDirty = pB;
  } else {
    pTail->pDirty = 0;
  }
  return result.pDirty;
}

PgHdr *__attribute__((noinline)) sort_pagelist(PgHdr *pIn) {
  PgHdr *a[25], *p;
  int    i;
  __builtin_memset(a, 0, sizeof(a));
  while (pIn) {
    p         = pIn;
    pIn       = p->pDirty;
    p->pDirty = 0;
    for (i = 0; i < 25 - 1; i++) {
      if (a[i] == 0) {
        a[i] = p;
        break;
      } else {
        p    = merge_pagelist(a[i], p);
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
  a[0].pgno   = 5;
  a[0].pDirty = &a[1];
  a[1].pgno   = 4;
  a[1].pDirty = &a[2];
  a[2].pgno   = 1;
  a[2].pDirty = &a[3];
  a[3].pgno   = 3;
  a[3].pDirty = 0;
  p           = sort_pagelist(&a[0]);
  if (p->pDirty == p)
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
// CHECK-NEXT: fn merge_pagelist(mut pA: *mut PgHdr, mut pB: *mut PgHdr) -> *mut PgHdr {
// CHECK-NEXT:     let mut result: PgHdr = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut pTail: *mut PgHdr = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     pTail = std::ptr::addr_of_mut!(result);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while pA != std::ptr::null_mut::<PgHdr>()
// CHECK-NEXT:         && pB != std::ptr::null_mut::<PgHdr>()
// CHECK-NEXT:     {
// CHECK-NEXT:         if (unsafe { (*pA).pgno }) < unsafe { (*pB).pgno } {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*pTail).pDirty = pA;
// CHECK-NEXT:             }
// CHECK-NEXT:             pTail = pA;
// CHECK-NEXT:             pA = unsafe { (*pA).pDirty };
// CHECK-NEXT:         } else {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*pTail).pDirty = pB;
// CHECK-NEXT:             }
// CHECK-NEXT:             pTail = pB;
// CHECK-NEXT:             pB = unsafe { (*pB).pDirty };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if pA != std::ptr::null_mut::<PgHdr>() {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*pTail).pDirty = pA;
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if pB != std::ptr::null_mut::<PgHdr>() {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*pTail).pDirty = pB;
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*pTail).pDirty = std::ptr::null_mut::<PgHdr>();
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     result.pDirty
// CHECK-NEXT: }
// CHECK-NEXT: fn sort_pagelist(mut pIn: *mut PgHdr) -> *mut PgHdr {
// CHECK-NEXT:     let mut a: __SlateAlign16<[*mut PgHdr; 25]> = __SlateAlign16([0 as *mut PgHdr; 25]);
// CHECK-NEXT:     let mut p: *mut PgHdr = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     unsafe { __builtin_memset(a.0.as_mut_ptr() as *mut (), 0 as i32, 200 as u64) };
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while pIn != std::ptr::null_mut::<PgHdr>() {
// CHECK-NEXT:         p = pIn;
// CHECK-NEXT:         pIn = unsafe { (*p).pDirty };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*p).pDirty = std::ptr::null_mut::<PgHdr>();
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
// CHECK-NEXT:             .pgno = (5 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (a.0.as_mut_ptr() as *mut PgHdr).offset((0 as i32) as isize) })
// CHECK-NEXT:             .pDirty = unsafe {
// CHECK-NEXT:             (a.0.as_mut_ptr() as *mut PgHdr).offset((1 as i32) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (a.0.as_mut_ptr() as *mut PgHdr).offset((1 as i32) as isize) })
// CHECK-NEXT:             .pgno = (4 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (a.0.as_mut_ptr() as *mut PgHdr).offset((1 as i32) as isize) })
// CHECK-NEXT:             .pDirty = unsafe {
// CHECK-NEXT:             (a.0.as_mut_ptr() as *mut PgHdr).offset((2 as i32) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (a.0.as_mut_ptr() as *mut PgHdr).offset((2 as i32) as isize) })
// CHECK-NEXT:             .pgno = (1 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (a.0.as_mut_ptr() as *mut PgHdr).offset((2 as i32) as isize) })
// CHECK-NEXT:             .pDirty = unsafe {
// CHECK-NEXT:             (a.0.as_mut_ptr() as *mut PgHdr).offset((3 as i32) as isize)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (a.0.as_mut_ptr() as *mut PgHdr).offset((3 as i32) as isize) })
// CHECK-NEXT:             .pgno = (3 as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe { (a.0.as_mut_ptr() as *mut PgHdr).offset((3 as i32) as isize) })
// CHECK-NEXT:             .pDirty = std::ptr::null_mut::<PgHdr>();
// CHECK-NEXT:     }
// CHECK-NEXT:     p = sort_pagelist(unsafe {
// CHECK-NEXT:         (a.0.as_mut_ptr() as *mut PgHdr).offset((0 as i32) as isize)
// CHECK-NEXT:     });
// CHECK-NEXT:     if (unsafe { (*p).pDirty }) == p {
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
