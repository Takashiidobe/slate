// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata

extern int shared;
int shared;
extern int shared;
int shared;
int initialized;
int initialized = 7;
extern int initialized;
extern int declaration_only;
static int private_object;
extern int private_object;
extern int completed[];
int completed[3];
int one_element[];

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
// DEFAULT-NEXT:     global %[[VALUE_shared:[0-9]+]] shared: i32 [storage=static] [linkage=external] [c="int"];
// DEFAULT-NEXT:     global %[[VALUE_initialized:[0-9]+]] initialized: i32 [storage=static] = const<i32>(7) [linkage=external] [c="int"];
// DEFAULT-NEXT:     extern %[[VALUE_declaration_only:[0-9]+]] declaration_only: i32 [storage=static] [linkage=external] [c="int"];
// DEFAULT-NEXT:     global %[[VALUE_private_object:[0-9]+]] private_object: i32 [storage=static] [linkage=internal] [c="int"];
// DEFAULT-NEXT:     global %[[VALUE_completed:[0-9]+]] completed: array<i32, 3> [storage=static] [linkage=external] [c="int[]"];
// DEFAULT-NEXT:     global %[[VALUE_one_element:[0-9]+]] one_element: array<i32, 1> [storage=static] [linkage=external] [c="int[]"];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
