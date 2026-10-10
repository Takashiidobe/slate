// SLATE-FILECHECK-ARGS -std=gnu99 -Wtraditional
// SLATE-FILECHECK-DEFINES DEFAULT

#define NOT_ASSERT 1

 #define X X

#if 0
 #define SKIPPED 1
#elif 1
#endif

#define f(X) X
int f;

#if 0U
#endif

#if +1
#endif

#if 0 && +1
#endif

#define ALERT "\a"
char *x = "\x0";
char *y = ALERT;
char *z = "\u0F43";
int ok = 1;

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
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([0, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([7, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_y:[0-9]+]] y: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_2]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([224, 189, 131, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_z:[0-9]+]] z: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ok:[0-9]+]] ok: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
