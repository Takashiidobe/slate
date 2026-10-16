/* PR 3967

   local-alloc screwed up consideration of high+lo_sum and created
   reg_equivs that it shouldn't have, resulting in lo_sum with
   uninitialized data, resulting in segv.  The test has to remain
   relatively large, since register spilling is required to twig
   the bug.  */

unsigned long *Local1;
unsigned long *Local2;
unsigned long *Local3;
unsigned long *RDbf1;
unsigned long *RDbf2;
unsigned long *RDbf3;
unsigned long *IntVc1;
unsigned long *IntVc2;
unsigned long *IntCode3;
unsigned long *IntCode4;
unsigned long *IntCode5;
unsigned long *IntCode6;
unsigned long *Lom1;
unsigned long *Lom2;
unsigned long *Lom3;
unsigned long *Lom4;
unsigned long *Lom5;
unsigned long *Lom6;
unsigned long *Lom7;
unsigned long *Lom8;
unsigned long *Lom9;
unsigned long *Lom10;
unsigned long *RDbf11;
unsigned long *RDbf12;

typedef struct {
  long          a1;
  unsigned long n1;
  unsigned long local1;
  unsigned long local2;
  unsigned long local3;
  unsigned long rdbf1;
  unsigned long rdbf2;
  unsigned long milli;
  unsigned long frames1;
  unsigned long frames2;
  unsigned long nonShared;
  long          newPrivate;
  long          freeLimit;
  unsigned long cache1;
  unsigned long cache2;
  unsigned long cache3;
  unsigned long cache4;
  unsigned long cache5;
  unsigned long time6;
  unsigned long frames7;
  unsigned long page8;
  unsigned long ot9;
  unsigned long data10;
  unsigned long bm11;
  unsigned long misc12;
} ShrPcCommonStatSType;

typedef struct {
  unsigned long sharedAttached;
  unsigned long totalAttached;
  long          avgPercentShared;
  unsigned long numberOfFreeFrames;
  unsigned long localDirtyPageCount;
  unsigned long globalDirtyPageCount;
  long          wakeupInterval;
  unsigned long numActiveProcesses;
  unsigned long numRecentActiveProcesses;
  unsigned long gemDirtyPageKinds[10];
  unsigned long stoneDirtyPageKinds[10];
  unsigned long gemsInCacheCount;
  long          targetFreeFrameCount;
} ShrPcMonStatSType;

typedef struct {
  unsigned long c1;
  unsigned long c2;
  unsigned long c3;
  unsigned long c4;
  unsigned long c5;
  unsigned long c6;
  unsigned long c7;
  unsigned long c8;
  unsigned long c9;
  unsigned long c10;
  unsigned long c11;
  unsigned long c12;
  unsigned long a1;
  unsigned long a2;
  unsigned long a3;
  unsigned long a4;
  unsigned long a5;
  unsigned long a6;
  unsigned long a7;
  unsigned long a8;
  unsigned long a9;
  unsigned long a10;
  unsigned long a11;
  unsigned long a12;
  unsigned long a13;
  unsigned long a14;
  unsigned long a15;
  unsigned long a16;
  unsigned long a17;
  unsigned long a18;
  unsigned long a19;
  unsigned long sessionStats[40];
} ShrPcGemStatSType;

union ShrPcStatUnion {
  ShrPcMonStatSType monitor;
  ShrPcGemStatSType gem;
};

typedef struct {
  int                  processId;
  int                  sessionId;
  ShrPcCommonStatSType cmn;
  union ShrPcStatUnion u;
} ShrPcStatsSType;

typedef struct {
  unsigned long *p1;
  unsigned long *p2;
  unsigned long *p3;
  unsigned long *p4;
  unsigned long *p5;
  unsigned long *p6;
  unsigned long *p7;
  unsigned long *p8;
  unsigned long *p9;
  unsigned long *p10;
  unsigned long *p11;
} WorkEntrySType;

WorkEntrySType Workspace;

static void setStatPointers(ShrPcStatsSType *statsPtr, long sessionId) {
  statsPtr->sessionId = sessionId;
  statsPtr->cmn.a1    = 0;
  statsPtr->cmn.n1    = 5;

  Local1 = &statsPtr->cmn.local1;
  Local2 = &statsPtr->cmn.local2;
  Local3 = &statsPtr->cmn.local3;
  RDbf1  = &statsPtr->cmn.rdbf1;
  RDbf2  = &statsPtr->cmn.rdbf2;
  RDbf3  = &statsPtr->cmn.milli;
  *RDbf3 = 1;

  IntVc1   = &statsPtr->u.gem.a1;
  IntVc2   = &statsPtr->u.gem.a2;
  IntCode3 = &statsPtr->u.gem.a3;
  IntCode4 = &statsPtr->u.gem.a4;
  IntCode5 = &statsPtr->u.gem.a5;
  IntCode6 = &statsPtr->u.gem.a6;

  {
    WorkEntrySType *workSpPtr;
    workSpPtr      = &Workspace;
    workSpPtr->p1  = &statsPtr->u.gem.a7;
    workSpPtr->p2  = &statsPtr->u.gem.a8;
    workSpPtr->p3  = &statsPtr->u.gem.a9;
    workSpPtr->p4  = &statsPtr->u.gem.a10;
    workSpPtr->p5  = &statsPtr->u.gem.a11;
    workSpPtr->p6  = &statsPtr->u.gem.a12;
    workSpPtr->p7  = &statsPtr->u.gem.a13;
    workSpPtr->p8  = &statsPtr->u.gem.a14;
    workSpPtr->p9  = &statsPtr->u.gem.a15;
    workSpPtr->p10 = &statsPtr->u.gem.a16;
    workSpPtr->p11 = &statsPtr->u.gem.a17;
  }
  Lom1   = &statsPtr->u.gem.c1;
  Lom2   = &statsPtr->u.gem.c2;
  Lom3   = &statsPtr->u.gem.c3;
  Lom4   = &statsPtr->u.gem.c4;
  Lom5   = &statsPtr->u.gem.c5;
  Lom6   = &statsPtr->u.gem.c6;
  Lom7   = &statsPtr->u.gem.c7;
  Lom8   = &statsPtr->u.gem.c8;
  Lom9   = &statsPtr->u.gem.c9;
  Lom10  = &statsPtr->u.gem.c10;
  RDbf11 = &statsPtr->u.gem.c11;
  RDbf12 = &statsPtr->u.gem.c12;
}

typedef struct {
  ShrPcStatsSType stats;
} ShrPcPteSType;

ShrPcPteSType MyPte;

static void initPte(void *shrpcPtr, long sessionId) {
  ShrPcPteSType *ptePtr;

  ptePtr = &MyPte;
  setStatPointers(&ptePtr->stats, sessionId);
}

void InitCache(int sessionId) { initPte(0, sessionId); }

int main(int argc, char *argv[]) {
  InitCache(5);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR 3967
// CHECK-NEXT: //!
// CHECK-NEXT: //! local-alloc screwed up consideration of high+lo_sum and created
// CHECK-NEXT: //! reg_equivs that it shouldn't have, resulting in lo_sum with
// CHECK-NEXT: //! uninitialized data, resulting in segv.  The test has to remain
// CHECK-NEXT: //! relatively large, since register spilling is required to twig
// CHECK-NEXT: //! the bug.
// CHECK-NEXT: static mut Local1: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut Local2: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut Local3: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut RDbf1: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut RDbf2: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut RDbf3: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut IntVc1: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut IntVc2: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut IntCode3: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut IntCode4: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut IntCode5: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut IntCode6: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut Lom1: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut Lom2: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut Lom3: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut Lom4: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut Lom5: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut Lom6: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut Lom7: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut Lom8: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut Lom9: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut Lom10: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut RDbf11: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut RDbf12: *mut u64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a1: i64,
// CHECK-NEXT:     n1: u64,
// CHECK-NEXT:     local1: u64,
// CHECK-NEXT:     local2: u64,
// CHECK-NEXT:     local3: u64,
// CHECK-NEXT:     rdbf1: u64,
// CHECK-NEXT:     rdbf2: u64,
// CHECK-NEXT:     milli: u64,
// CHECK-NEXT:     frames1: u64,
// CHECK-NEXT:     frames2: u64,
// CHECK-NEXT:     nonShared: u64,
// CHECK-NEXT:     newPrivate: i64,
// CHECK-NEXT:     freeLimit: i64,
// CHECK-NEXT:     cache1: u64,
// CHECK-NEXT:     cache2: u64,
// CHECK-NEXT:     cache3: u64,
// CHECK-NEXT:     cache4: u64,
// CHECK-NEXT:     cache5: u64,
// CHECK-NEXT:     time6: u64,
// CHECK-NEXT:     frames7: u64,
// CHECK-NEXT:     page8: u64,
// CHECK-NEXT:     ot9: u64,
// CHECK-NEXT:     data10: u64,
// CHECK-NEXT:     bm11: u64,
// CHECK-NEXT:     misc12: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     sharedAttached: u64,
// CHECK-NEXT:     totalAttached: u64,
// CHECK-NEXT:     avgPercentShared: i64,
// CHECK-NEXT:     numberOfFreeFrames: u64,
// CHECK-NEXT:     localDirtyPageCount: u64,
// CHECK-NEXT:     globalDirtyPageCount: u64,
// CHECK-NEXT:     wakeupInterval: i64,
// CHECK-NEXT:     numActiveProcesses: u64,
// CHECK-NEXT:     numRecentActiveProcesses: u64,
// CHECK-NEXT:     gemDirtyPageKinds: [u64; 10],
// CHECK-NEXT:     stoneDirtyPageKinds: [u64; 10],
// CHECK-NEXT:     gemsInCacheCount: u64,
// CHECK-NEXT:     targetFreeFrameCount: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_2:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     c1: u64,
// CHECK-NEXT:     c2: u64,
// CHECK-NEXT:     c3: u64,
// CHECK-NEXT:     c4: u64,
// CHECK-NEXT:     c5: u64,
// CHECK-NEXT:     c6: u64,
// CHECK-NEXT:     c7: u64,
// CHECK-NEXT:     c8: u64,
// CHECK-NEXT:     c9: u64,
// CHECK-NEXT:     c10: u64,
// CHECK-NEXT:     c11: u64,
// CHECK-NEXT:     c12: u64,
// CHECK-NEXT:     a1: u64,
// CHECK-NEXT:     a2: u64,
// CHECK-NEXT:     a3: u64,
// CHECK-NEXT:     a4: u64,
// CHECK-NEXT:     a5: u64,
// CHECK-NEXT:     a6: u64,
// CHECK-NEXT:     a7: u64,
// CHECK-NEXT:     a8: u64,
// CHECK-NEXT:     a9: u64,
// CHECK-NEXT:     a10: u64,
// CHECK-NEXT:     a11: u64,
// CHECK-NEXT:     a12: u64,
// CHECK-NEXT:     a13: u64,
// CHECK-NEXT:     a14: u64,
// CHECK-NEXT:     a15: u64,
// CHECK-NEXT:     a16: u64,
// CHECK-NEXT:     a17: u64,
// CHECK-NEXT:     a18: u64,
// CHECK-NEXT:     a19: u64,
// CHECK-NEXT:     sessionStats: [u64; 40],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union ShrPcStatUnion {
// CHECK-NEXT:     monitor: [[SLATERECORD_1]],
// CHECK-NEXT:     gem: [[SLATERECORD_2]],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_3:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     processId: i32,
// CHECK-NEXT:     sessionId: i32,
// CHECK-NEXT:     cmn: [[SLATERECORD_0]],
// CHECK-NEXT:     u: ShrPcStatUnion,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_4:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     p1: *mut u64,
// CHECK-NEXT:     p2: *mut u64,
// CHECK-NEXT:     p3: *mut u64,
// CHECK-NEXT:     p4: *mut u64,
// CHECK-NEXT:     p5: *mut u64,
// CHECK-NEXT:     p6: *mut u64,
// CHECK-NEXT:     p7: *mut u64,
// CHECK-NEXT:     p8: *mut u64,
// CHECK-NEXT:     p9: *mut u64,
// CHECK-NEXT:     p10: *mut u64,
// CHECK-NEXT:     p11: *mut u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut Workspace: [[SLATERECORD_4]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn setStatPointers(mut statsPtr: *mut [[SLATERECORD_3]], mut sessionId: i64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*statsPtr).sessionId = sessionId as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*statsPtr).cmn.a1 = (0 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*statsPtr).cmn.n1 = ((5 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         Local1 = unsafe { std::ptr::addr_of_mut!((*statsPtr).cmn.local1) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         Local2 = unsafe { std::ptr::addr_of_mut!((*statsPtr).cmn.local2) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         Local3 = unsafe { std::ptr::addr_of_mut!((*statsPtr).cmn.local3) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         RDbf1 = unsafe { std::ptr::addr_of_mut!((*statsPtr).cmn.rdbf1) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         RDbf2 = unsafe { std::ptr::addr_of_mut!((*statsPtr).cmn.rdbf2) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         RDbf3 = unsafe { std::ptr::addr_of_mut!((*statsPtr).cmn.milli) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { RDbf3 } = ((1 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         IntVc1 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a1) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         IntVc2 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a2) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         IntCode3 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a3) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         IntCode4 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a4) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         IntCode5 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a5) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         IntCode6 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a6) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut workSpPtr: *mut [[SLATERECORD_4]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     workSpPtr = unsafe { std::ptr::addr_of_mut!(Workspace) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*workSpPtr).p1 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a7) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*workSpPtr).p2 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a8) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*workSpPtr).p3 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a9) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*workSpPtr).p4 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a10) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*workSpPtr).p5 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a11) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*workSpPtr).p6 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a12) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*workSpPtr).p7 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a13) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*workSpPtr).p8 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a14) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*workSpPtr).p9 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a15) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*workSpPtr).p10 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a16) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*workSpPtr).p11 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.a17) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         Lom1 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.c1) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         Lom2 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.c2) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         Lom3 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.c3) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         Lom4 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.c4) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         Lom5 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.c5) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         Lom6 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.c6) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         Lom7 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.c7) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         Lom8 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.c8) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         Lom9 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.c9) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         Lom10 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.c10) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         RDbf11 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.c11) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         RDbf12 = unsafe { std::ptr::addr_of_mut!((*statsPtr).u.gem.c12) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_5:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     stats: [[SLATERECORD_3]],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut MyPte: [[SLATERECORD_5]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn initPte(mut shrpcPtr: *mut (), mut sessionId: i64) {
// CHECK-NEXT:     let mut ptePtr: *mut [[SLATERECORD_5]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ptePtr = unsafe { std::ptr::addr_of_mut!(MyPte) };
// CHECK-NEXT:     setStatPointers(
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!((*ptePtr).stats) },
// CHECK-NEXT:         sessionId,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn InitCache(mut sessionId: i32) {
// CHECK-NEXT:     initPte(std::ptr::null_mut::<()>(), sessionId as i64);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     InitCache(5 as i32);
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
