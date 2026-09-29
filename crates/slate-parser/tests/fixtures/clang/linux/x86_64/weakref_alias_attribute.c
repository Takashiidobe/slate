extern int function_target(void);
extern int object_target;
extern int ignored_target;

static int function_reference(void) __attribute__((weakref, alias("function_target")));
static int object_reference __attribute__((weakref, alias("object_target")));
static int argument_first __attribute__((weakref("object_target"), alias("ignored_target")));

int use(void) { return function_reference() + object_reference + argument_first; }

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
// DEFAULT-NEXT:     extern %[[VALUE_object_target:[0-9]+]] object_target: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_ignored_target:[0-9]+]] ignored_target: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_object_reference:[0-9]+]] object_reference: i32 [storage=static] [linkage=internal] [weakref="object_target"];
// DEFAULT-NEXT:     extern %[[VALUE_argument_first:[0-9]+]] argument_first: i32 [storage=static] [linkage=internal] [weakref="object_target"];
// DEFAULT-NEXT:     fn %[[VALUE_function_target:[0-9]+]] @function_target() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_function_reference:[0-9]+]] @function_reference() -> i32 [linkage=internal] [weakref="function_target"];
// DEFAULT-NEXT:     fn %[[VALUE_use:[0-9]+]] @use() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[VALUE_function_reference]]), read<i32>(%[[VALUE_object_reference]])), read<i32>(%[[VALUE_argument_first]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
