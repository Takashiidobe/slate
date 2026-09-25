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


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 a1: i64;
// DEFAULT-NEXT:         field1 n1: u64;
// DEFAULT-NEXT:         field2 local1: u64;
// DEFAULT-NEXT:         field3 local2: u64;
// DEFAULT-NEXT:         field4 local3: u64;
// DEFAULT-NEXT:         field5 rdbf1: u64;
// DEFAULT-NEXT:         field6 rdbf2: u64;
// DEFAULT-NEXT:         field7 milli: u64;
// DEFAULT-NEXT:         field8 frames1: u64;
// DEFAULT-NEXT:         field9 frames2: u64;
// DEFAULT-NEXT:         field10 nonShared: u64;
// DEFAULT-NEXT:         field11 newPrivate: i64;
// DEFAULT-NEXT:         field12 freeLimit: i64;
// DEFAULT-NEXT:         field13 cache1: u64;
// DEFAULT-NEXT:         field14 cache2: u64;
// DEFAULT-NEXT:         field15 cache3: u64;
// DEFAULT-NEXT:         field16 cache4: u64;
// DEFAULT-NEXT:         field17 cache5: u64;
// DEFAULT-NEXT:         field18 time6: u64;
// DEFAULT-NEXT:         field19 frames7: u64;
// DEFAULT-NEXT:         field20 page8: u64;
// DEFAULT-NEXT:         field21 ot9: u64;
// DEFAULT-NEXT:         field22 data10: u64;
// DEFAULT-NEXT:         field23 bm11: u64;
// DEFAULT-NEXT:         field24 misc12: u64;
// DEFAULT-NEXT:     } [size=200, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128, 136, 144, 152, 160, 168, 176, 184, 192]];
// DEFAULT-NEXT:     type @type1 ShrPcCommonStatSType = @type0;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 sharedAttached: u64;
// DEFAULT-NEXT:         field1 totalAttached: u64;
// DEFAULT-NEXT:         field2 avgPercentShared: i64;
// DEFAULT-NEXT:         field3 numberOfFreeFrames: u64;
// DEFAULT-NEXT:         field4 localDirtyPageCount: u64;
// DEFAULT-NEXT:         field5 globalDirtyPageCount: u64;
// DEFAULT-NEXT:         field6 wakeupInterval: i64;
// DEFAULT-NEXT:         field7 numActiveProcesses: u64;
// DEFAULT-NEXT:         field8 numRecentActiveProcesses: u64;
// DEFAULT-NEXT:         field9 gemDirtyPageKinds: array<u64, 10>;
// DEFAULT-NEXT:         field10 stoneDirtyPageKinds: array<u64, 10>;
// DEFAULT-NEXT:         field11 gemsInCacheCount: u64;
// DEFAULT-NEXT:         field12 targetFreeFrameCount: i64;
// DEFAULT-NEXT:     } [size=248, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 152, 232, 240]];
// DEFAULT-NEXT:     type @type3 ShrPcMonStatSType = @type2;
// DEFAULT-NEXT:     type @type4 = struct {
// DEFAULT-NEXT:         field0 c1: u64;
// DEFAULT-NEXT:         field1 c2: u64;
// DEFAULT-NEXT:         field2 c3: u64;
// DEFAULT-NEXT:         field3 c4: u64;
// DEFAULT-NEXT:         field4 c5: u64;
// DEFAULT-NEXT:         field5 c6: u64;
// DEFAULT-NEXT:         field6 c7: u64;
// DEFAULT-NEXT:         field7 c8: u64;
// DEFAULT-NEXT:         field8 c9: u64;
// DEFAULT-NEXT:         field9 c10: u64;
// DEFAULT-NEXT:         field10 c11: u64;
// DEFAULT-NEXT:         field11 c12: u64;
// DEFAULT-NEXT:         field12 a1: u64;
// DEFAULT-NEXT:         field13 a2: u64;
// DEFAULT-NEXT:         field14 a3: u64;
// DEFAULT-NEXT:         field15 a4: u64;
// DEFAULT-NEXT:         field16 a5: u64;
// DEFAULT-NEXT:         field17 a6: u64;
// DEFAULT-NEXT:         field18 a7: u64;
// DEFAULT-NEXT:         field19 a8: u64;
// DEFAULT-NEXT:         field20 a9: u64;
// DEFAULT-NEXT:         field21 a10: u64;
// DEFAULT-NEXT:         field22 a11: u64;
// DEFAULT-NEXT:         field23 a12: u64;
// DEFAULT-NEXT:         field24 a13: u64;
// DEFAULT-NEXT:         field25 a14: u64;
// DEFAULT-NEXT:         field26 a15: u64;
// DEFAULT-NEXT:         field27 a16: u64;
// DEFAULT-NEXT:         field28 a17: u64;
// DEFAULT-NEXT:         field29 a18: u64;
// DEFAULT-NEXT:         field30 a19: u64;
// DEFAULT-NEXT:         field31 sessionStats: array<u64, 40>;
// DEFAULT-NEXT:     } [size=568, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224, 232, 240, 248]];
// DEFAULT-NEXT:     type @type5 ShrPcGemStatSType = @type4;
// DEFAULT-NEXT:     type @type6 ShrPcStatUnion = union {
// DEFAULT-NEXT:         field0 monitor: @type2;
// DEFAULT-NEXT:         field1 gem: @type4;
// DEFAULT-NEXT:     } [size=568, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type7 = struct {
// DEFAULT-NEXT:         field0 processId: i32;
// DEFAULT-NEXT:         field1 sessionId: i32;
// DEFAULT-NEXT:         field2 cmn: @type0;
// DEFAULT-NEXT:         field3 u: @type6;
// DEFAULT-NEXT:     } [size=776, align=8, offsets=[0, 4, 8, 208]];
// DEFAULT-NEXT:     type @type8 ShrPcStatsSType = @type7;
// DEFAULT-NEXT:     type @type9 = struct {
// DEFAULT-NEXT:         field0 p1: ptr<u64>;
// DEFAULT-NEXT:         field1 p2: ptr<u64>;
// DEFAULT-NEXT:         field2 p3: ptr<u64>;
// DEFAULT-NEXT:         field3 p4: ptr<u64>;
// DEFAULT-NEXT:         field4 p5: ptr<u64>;
// DEFAULT-NEXT:         field5 p6: ptr<u64>;
// DEFAULT-NEXT:         field6 p7: ptr<u64>;
// DEFAULT-NEXT:         field7 p8: ptr<u64>;
// DEFAULT-NEXT:         field8 p9: ptr<u64>;
// DEFAULT-NEXT:         field9 p10: ptr<u64>;
// DEFAULT-NEXT:         field10 p11: ptr<u64>;
// DEFAULT-NEXT:     } [size=88, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80]];
// DEFAULT-NEXT:     type @type10 WorkEntrySType = @type9;
// DEFAULT-NEXT:     type @type11 = struct {
// DEFAULT-NEXT:         field0 stats: @type7;
// DEFAULT-NEXT:     } [size=776, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type12 ShrPcPteSType = @type11;
// DEFAULT-NEXT:     global %0 Local1: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 Local2: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 Local3: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 RDbf1: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 RDbf2: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 RDbf3: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 IntVc1: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 IntVc2: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 IntCode3: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 IntCode4: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 IntCode5: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 IntCode6: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 Lom1: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 Lom2: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %14 Lom3: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 Lom4: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 Lom5: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 Lom6: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %18 Lom7: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %19 Lom8: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %20 Lom9: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %21 Lom10: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %22 RDbf11: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %23 RDbf12: ptr<u64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %35 Workspace: @type9 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %42 MyPte: @type11 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %36 @setStatPointers(%37 statsPtr: ptr<@type7>, %38 sessionId: i64) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type7>>(%37))), truncate<i32, reason=assign, fits=unknown>(read<i64>(%38)));
// DEFAULT-NEXT:         write<i64>(field0(field2(deref(read<ptr<@type7>>(%37)))), widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<u64>(field1(field2(deref(read<ptr<@type7>>(%37)))), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(5))));
// DEFAULT-NEXT:         write<ptr<u64>>(%0, addr_of<ptr<u64>>(field2(field2(deref(read<ptr<@type7>>(%37))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%1, addr_of<ptr<u64>>(field3(field2(deref(read<ptr<@type7>>(%37))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%2, addr_of<ptr<u64>>(field4(field2(deref(read<ptr<@type7>>(%37))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%3, addr_of<ptr<u64>>(field5(field2(deref(read<ptr<@type7>>(%37))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%4, addr_of<ptr<u64>>(field6(field2(deref(read<ptr<@type7>>(%37))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%5, addr_of<ptr<u64>>(field7(field2(deref(read<ptr<@type7>>(%37))))));
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%5)), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1))));
// DEFAULT-NEXT:         write<ptr<u64>>(%6, addr_of<ptr<u64>>(field12(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%7, addr_of<ptr<u64>>(field13(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%8, addr_of<ptr<u64>>(field14(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%9, addr_of<ptr<u64>>(field15(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%10, addr_of<ptr<u64>>(field16(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%11, addr_of<ptr<u64>>(field17(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %39 workSpPtr: ptr<@type9> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<@type9>>(%39, addr_of<ptr<@type9>>(%35));
// DEFAULT-NEXT:             write<ptr<u64>>(field0(deref(read<ptr<@type9>>(%39))), addr_of<ptr<u64>>(field18(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:             write<ptr<u64>>(field1(deref(read<ptr<@type9>>(%39))), addr_of<ptr<u64>>(field19(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:             write<ptr<u64>>(field2(deref(read<ptr<@type9>>(%39))), addr_of<ptr<u64>>(field20(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:             write<ptr<u64>>(field3(deref(read<ptr<@type9>>(%39))), addr_of<ptr<u64>>(field21(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:             write<ptr<u64>>(field4(deref(read<ptr<@type9>>(%39))), addr_of<ptr<u64>>(field22(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:             write<ptr<u64>>(field5(deref(read<ptr<@type9>>(%39))), addr_of<ptr<u64>>(field23(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:             write<ptr<u64>>(field6(deref(read<ptr<@type9>>(%39))), addr_of<ptr<u64>>(field24(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:             write<ptr<u64>>(field7(deref(read<ptr<@type9>>(%39))), addr_of<ptr<u64>>(field25(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:             write<ptr<u64>>(field8(deref(read<ptr<@type9>>(%39))), addr_of<ptr<u64>>(field26(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:             write<ptr<u64>>(field9(deref(read<ptr<@type9>>(%39))), addr_of<ptr<u64>>(field27(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:             write<ptr<u64>>(field10(deref(read<ptr<@type9>>(%39))), addr_of<ptr<u64>>(field28(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<u64>>(%12, addr_of<ptr<u64>>(field0(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%13, addr_of<ptr<u64>>(field1(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%14, addr_of<ptr<u64>>(field2(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%15, addr_of<ptr<u64>>(field3(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%16, addr_of<ptr<u64>>(field4(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%17, addr_of<ptr<u64>>(field5(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%18, addr_of<ptr<u64>>(field6(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%19, addr_of<ptr<u64>>(field7(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%20, addr_of<ptr<u64>>(field8(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%21, addr_of<ptr<u64>>(field9(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%22, addr_of<ptr<u64>>(field10(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:         write<ptr<u64>>(%23, addr_of<ptr<u64>>(field11(field1(field3(deref(read<ptr<@type7>>(%37)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @initPte(%44 shrpcPtr: ptr<void>, %45 sessionId: i64) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %46 ptePtr: ptr<@type11> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type11>>(%46, addr_of<ptr<@type11>>(%42));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type7>, i64) -> void>(%36, addr_of<ptr<@type7>>(field0(deref(read<ptr<@type11>>(%46)))), read<i64>(%45));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @InitCache(%48 sessionId: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, i64) -> void>(%43, null<ptr<void>>, widen<i64, reason=arg>(read<i32>(%48)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @main(%50 argc: i32, %51 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%47, const<i32>(5));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
