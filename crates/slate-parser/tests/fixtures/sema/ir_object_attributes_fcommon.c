// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -fcommon

int tentative;
int aligned_tentative __attribute__((aligned(16)));
int nocommon_requested __attribute__((nocommon));
int initialized = 1;
extern int declared;
static int internal;
_Thread_local int thread;
int weak __attribute__((weak));
int sectioned __attribute__((section("data")));
int completed; extern int completed;

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %0 tentative: i32 [storage=static] [linkage=external] [common];
// IR-NEXT:     global %1 aligned_tentative: i32 [storage=static] [linkage=external] [align=16] [common];
// IR-NEXT:     global %2 nocommon_requested: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %3 initialized: i32 [storage=static] = const<i32>(1) [linkage=external];
// IR-NEXT:     extern %4 declared: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %5 internal: i32 [storage=static] [linkage=internal];
// IR-NEXT:     global %6 thread: i32 [storage=thread] [linkage=external];
// IR-NEXT:     global %7 weak: i32 [storage=static] [linkage=external] [weak];
// IR-NEXT:     global %8 sectioned: i32 [storage=static] [linkage=external] [section="data"];
// IR-NEXT:     global %9 completed: i32 [storage=static] [linkage=external] [common];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
