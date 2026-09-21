// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

int above __attribute__((aligned(16)));
int below __attribute__((aligned(1)));
int natural __attribute__((aligned(4)));
_Alignas(32) int alignas_initialized = 1;
int redeclared;
int redeclared __attribute__((aligned(64)));
int incomplete[] __attribute__((aligned(32)));

int common_requested __attribute__((common));
int nocommon_requested __attribute__((nocommon));
int common_wins __attribute__((nocommon));
int common_wins __attribute__((common));
int common_initialized __attribute__((common)) = 3;
static int common_internal __attribute__((common));

extern int target;
static int reference __attribute__((weakref("target")));
int *use(void) { return &reference; }
static void function_reference(void) __attribute__((weakref("use")));
void call(void) { function_reference(); }

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
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %0 above: i32 [storage=static] [align=16] [linkage=external];
// IR-NEXT:     global %1 below: i32 [storage=static] [align=1] [linkage=external];
// IR-NEXT:     global %2 natural: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %3 alignas_initialized: i32 [storage=static] [align=32] = const<i32>(1) [linkage=external];
// IR-NEXT:     global %4 redeclared: i32 [storage=static] [align=64] [linkage=external];
// IR-NEXT:     global %5 incomplete: array<i32, 1> [storage=static] [align=32] [linkage=external];
// IR-NEXT:     global %6 common_requested: i32 [storage=static] [linkage=external] [common];
// IR-NEXT:     global %7 nocommon_requested: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %8 common_wins: i32 [storage=static] [linkage=external] [common];
// IR-NEXT:     global %9 common_initialized: i32 [storage=static] = const<i32>(3) [linkage=external];
// IR-NEXT:     global %10 common_internal: i32 [storage=static] [linkage=internal];
// IR-NEXT:     extern %11 target: i32 [storage=static] [linkage=external];
// IR-NEXT:     extern %12 reference: i32 [storage=static] [linkage=internal] [weakref="target"];
// IR-NEXT:     fn %13 @use() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return addr_of<ptr<i32>>(%12);
// IR-NEXT:     }
// IR-NEXT:     fn %14 @function_reference() -> void [linkage=internal] [weakref="use"];
// IR-NEXT:     fn %15 @call() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         call<void, signature=fn() -> void>(%14);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
