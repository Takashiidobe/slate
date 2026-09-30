// builtin macros are defined per flavor; msvc lacks the gnu-only ones and ignores #undef/#define of its own
#ifdef __LINE__
int defined_line = 1;
#else
int defined_line = 0;
#endif
#ifdef __FILE__
int defined_file = 1;
#else
int defined_file = 0;
#endif
#ifdef __FILE_NAME__
int defined_filename = 1;
#else
int defined_filename = 0;
#endif
#ifdef __BASE_FILE__
int defined_basefile = 1;
#else
int defined_basefile = 0;
#endif
#ifdef __INCLUDE_LEVEL__
int defined_includelevel = 1;
#else
int defined_includelevel = 0;
#endif
#ifdef __COUNTER__
int defined_counter = 1;
#else
int defined_counter = 0;
#endif
#ifdef __DATE__
int defined_date = 1;
#else
int defined_date = 0;
#endif
#ifdef __TIME__
int defined_time = 1;
#else
int defined_time = 0;
#endif
#ifdef __TIMESTAMP__
int defined_timestamp = 1;
#else
int defined_timestamp = 0;
#endif
#undef __COUNTER__
#ifdef __COUNTER__
int counter_after_undef = 1;
#else
int counter_after_undef = 0;
#endif
#define __DATE__ 7
int date_redefined = sizeof(__DATE__) != 12;

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
// DEFAULT-NEXT:     global %[[VALUE_defined_line:[0-9]+]] defined_line: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defined_file:[0-9]+]] defined_file: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defined_filename:[0-9]+]] defined_filename: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defined_basefile:[0-9]+]] defined_basefile: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defined_includelevel:[0-9]+]] defined_includelevel: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defined_counter:[0-9]+]] defined_counter: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defined_date:[0-9]+]] defined_date: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defined_time:[0-9]+]] defined_time: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_defined_timestamp:[0-9]+]] defined_timestamp: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_counter_after_undef:[0-9]+]] counter_after_undef: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_date_redefined:[0-9]+]] date_redefined: i32 [storage=static] = from_bool<i32, reason=assign>(ne<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(12))))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
