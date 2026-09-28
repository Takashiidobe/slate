_Thread_local int thread_local_value;
__thread int gnu_thread_value;
int *__restrict restricted_value;
int *__restrict__ restricted_alias_value;

_Noreturn void noreturn_function(void);
_Atomic(int) atomic_value;
_Atomic(unsigned long) atomic_unsigned_long_value;

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
// DEFAULT-NEXT:     global %0 thread_local_value: i32 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     global %1 gnu_thread_value: i32 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     global %2 restricted_value: ptr<i32> [storage=static] [restrict] [linkage=external];
// DEFAULT-NEXT:     global %3 restricted_alias_value: ptr<i32> [storage=static] [restrict] [linkage=external];
// DEFAULT-NEXT:     global %5 atomic_value: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 atomic_unsigned_long_value: atomic u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @noreturn_function() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
