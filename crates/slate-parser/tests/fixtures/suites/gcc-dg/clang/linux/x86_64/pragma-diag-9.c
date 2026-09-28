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
// DEFAULT-NEXT:     global %7 a2: array<i32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 b2: array<i32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %23 c2: array<i32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %31 d2: array<i32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %39 e2: array<i32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @memset(%41 <unnamed>: ptr<void>, %42 <unnamed>: i32, %43 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %1 @warn0(%2 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%0, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%2)), const<i32>(11), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @warn1(%4 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%1, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @warn2(%6 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%3, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%6), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @warn3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%5, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%7), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @ignore0(%10 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%0, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%10)), const<i32>(38), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @nowarn1_ignore0(%12 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%9, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%12), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @nowarn2_ignore0(%14 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%11, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%14), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @nowarn3_ignore0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%13, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%15), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @nowarn0_ignore1(%18 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%0, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%18)), const<i32>(64), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @ignore1(%20 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%17, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%20), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @nowarn2_ignore1(%22 p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%19, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%22), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @nowarn3_ignore1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%21, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%23), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @nowarn0_ignore2(%26 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%0, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%26)), const<i32>(92), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @nowarn1_ignore2(%28 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%25, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%28), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @ignore2(%30 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%27, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%30), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @nowarn3_ignore2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%29, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%23), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @nowarn0_ignore3(%34 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%0, pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%34)), const<i32>(120), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @nowarn1_ignore3(%36 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%33, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%36), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @nowarn2_ignore3(%38 p: ptr<i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%35, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%38), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %40 @ignore3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%37, ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(%39), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
