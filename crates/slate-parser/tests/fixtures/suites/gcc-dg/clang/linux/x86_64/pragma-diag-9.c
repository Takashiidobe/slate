/* Verify that #pragma GCC diagnostic down the inlining stack suppresses
   a warning that would otherwise be issued for inlined calls higher up
   the inlining stack.
   { dg-do compile }
   { dg-options "-O2 -Wall -Wno-array-bounds" } */

extern void* memset (void*, int, __SIZE_TYPE__);

static void warn0 (int *p)
{
  memset (p, __LINE__, 3);    // { dg-warning "\\\[-Wstringop-overflow" }
}

static void warn1 (int *p)
{
  warn0 (p + 1);
}

static void warn2 (int *p)
{
  warn1 (p + 1);
}

int a2[2];                    // { dg-message "at offset 12 into destination object 'a2' of size 8" }

void warn3 (void)
{
  warn2 (a2 + 1);
}


// Verify suppression at the innermost frame of the inlining stack.

static void ignore0 (int *p)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
  memset (p, __LINE__, 3);
#pragma GCC diagnostic pop
}

static void nowarn1_ignore0 (int *p)
{
  ignore0 (p + 1);
}

static void nowarn2_ignore0 (int *p)
{
  nowarn1_ignore0 (p + 1);
}

int b2[2];

void nowarn3_ignore0 (void)
{
  nowarn2_ignore0 (b2 + 1);
}


// Verify suppression at the second innermost frame of the inlining stack.

static void nowarn0_ignore1 (int *p)
{
  memset (p, __LINE__, 3);
}

static void ignore1 (int *p)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
  nowarn0_ignore1 (p + 1);
#pragma GCC diagnostic pop
}

void nowarn2_ignore1 (int *p)
{
  ignore1 (p + 1);
}

int c2[2];

void nowarn3_ignore1 (void)
{
  nowarn2_ignore1 (c2 + 1);
}


// Verify suppression at the third innermost frame of the inlining stack.

static void nowarn0_ignore2 (int *p)
{
  memset (p, __LINE__, 3);
}

static void nowarn1_ignore2 (int *p)
{
  nowarn0_ignore2 (p + 1);
}

static void ignore2 (int *p)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
  nowarn1_ignore2 (p + 1);
#pragma GCC diagnostic pop
}

int d2[2];

void nowarn3_ignore2 (void)
{
  ignore2 (c2 + 1);
}


// Verify suppression at the outermost frame of the inlining stack.

static void nowarn0_ignore3 (int *p)
{
  memset (p, __LINE__, 3);
}

static void nowarn1_ignore3 (int *p)
{
  nowarn0_ignore3 (p + 1);
}

static void nowarn2_ignore3 (int *p)
{
  nowarn1_ignore3 (p + 1);
}

int e2[2];

void ignore3 (void)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
  nowarn2_ignore3 (e2 + 1);
#pragma GCC diagnostic pop
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %[[VALUE_a2:[0-9]+]] a2: array<i32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b2:[0-9]+]] b2: array<i32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c2:[0-9]+]] c2: array<i32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d2:[0-9]+]] d2: array<i32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e2:[0-9]+]] e2: array<i32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_warn0:[0-9]+]] @warn0(%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_p]])), const<i32>(11), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn1:[0-9]+]] @warn1(%[[VALUE_p_2:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_warn0]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_2]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn2:[0-9]+]] @warn2(%[[VALUE_p_3:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_warn1]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_3]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_warn3:[0-9]+]] @warn3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_warn2]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_a2]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ignore0:[0-9]+]] @ignore0(%[[VALUE_p_4:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_p_4]])), const<i32>(38), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn1_ignore0:[0-9]+]] @nowarn1_ignore0(%[[VALUE_p_5:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_ignore0]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_5]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn2_ignore0:[0-9]+]] @nowarn2_ignore0(%[[VALUE_p_6:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_nowarn1_ignore0]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_6]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn3_ignore0:[0-9]+]] @nowarn3_ignore0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_nowarn2_ignore0]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_b2]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn0_ignore1:[0-9]+]] @nowarn0_ignore1(%[[VALUE_p_7:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_p_7]])), const<i32>(64), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ignore1:[0-9]+]] @ignore1(%[[VALUE_p_8:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_nowarn0_ignore1]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_8]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn2_ignore1:[0-9]+]] @nowarn2_ignore1(%[[VALUE_p_9:[0-9]+]] p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_ignore1]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_9]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn3_ignore1:[0-9]+]] @nowarn3_ignore1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_nowarn2_ignore1]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_c2]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn0_ignore2:[0-9]+]] @nowarn0_ignore2(%[[VALUE_p_10:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_p_10]])), const<i32>(92), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn1_ignore2:[0-9]+]] @nowarn1_ignore2(%[[VALUE_p_11:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_nowarn0_ignore2]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_11]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ignore2:[0-9]+]] @ignore2(%[[VALUE_p_12:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_nowarn1_ignore2]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_12]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn3_ignore2:[0-9]+]] @nowarn3_ignore2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_ignore2]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_c2]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn0_ignore3:[0-9]+]] @nowarn0_ignore3(%[[VALUE_p_13:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_p_13]])), const<i32>(120), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn1_ignore3:[0-9]+]] @nowarn1_ignore3(%[[VALUE_p_14:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_nowarn0_ignore3]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_14]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nowarn2_ignore3:[0-9]+]] @nowarn2_ignore3(%[[VALUE_p_15:[0-9]+]] p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_nowarn1_ignore3]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_15]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ignore3:[0-9]+]] @ignore3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_nowarn2_ignore3]], ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%[[VALUE_e2]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
