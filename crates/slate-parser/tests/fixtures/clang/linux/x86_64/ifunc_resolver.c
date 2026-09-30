static int implementation(void) { return 42; }

static int (*resolver(void))(void) { return implementation; }

int magic(void) __attribute__((ifunc("resolver")));
static int hidden(void) __attribute__((ifunc("resolver"), visibility("hidden")));
int ignored __attribute__((ifunc("resolver")));

int use(void) { return magic() + hidden(); }

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
// DEFAULT-NEXT:     global %[[VALUE_ignored:[0-9]+]] ignored: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_implementation:[0-9]+]] @implementation() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(42);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_resolver:[0-9]+]] @resolver() -> ptr<fn() -> i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return function_decay<ptr<fn() -> i32>>(%[[VALUE_implementation]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_magic:[0-9]+]] @magic() -> i32 [linkage=external] [ifunc="resolver"];
// DEFAULT-NEXT:     fn %[[VALUE_hidden:[0-9]+]] @hidden() -> i32 [linkage=internal] [visibility=hidden] [ifunc="resolver"];
// DEFAULT-NEXT:     fn %[[VALUE_use:[0-9]+]] @use() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[VALUE_magic]]), call<i32, signature=fn() -> i32>(%[[VALUE_hidden]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
