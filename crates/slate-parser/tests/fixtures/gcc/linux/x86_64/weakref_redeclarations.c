extern int target(void);
extern int object_target;

static int reference_first(void) __attribute__((weakref));
extern int reference_first(void) __attribute__((alias("target")));

static int alias_first __attribute__((alias("object_target")));
static int alias_first __attribute__((weakref));

static int no_target __attribute__((weakref));
static int initialized __attribute__((weakref)) = 1;
static int __attribute__((weakref("target"))) defined(void) { return 2; }

int use(void) {
  return reference_first() + alias_first + no_target + initialized + defined();
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
// DEFAULT-NEXT:     extern %[[VALUE_object_target:[0-9]+]] object_target: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_alias_first:[0-9]+]] alias_first: i32 [storage=static] [linkage=internal] [weakref="object_target"];
// DEFAULT-NEXT:     global %[[VALUE_no_target:[0-9]+]] no_target: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_initialized:[0-9]+]] initialized: i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_target:[0-9]+]] @target() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_reference_first:[0-9]+]] @reference_first() -> i32 [linkage=internal] [weakref="target"];
// DEFAULT-NEXT:     fn %[[VALUE_defined:[0-9]+]] @defined() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use:[0-9]+]] @use() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[VALUE_reference_first]]), read<i32>(%[[VALUE_alias_first]])), read<i32>(%[[VALUE_no_target]])), read<i32>(%[[VALUE_initialized]])), call<i32, signature=fn() -> i32>(%[[VALUE_defined]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
