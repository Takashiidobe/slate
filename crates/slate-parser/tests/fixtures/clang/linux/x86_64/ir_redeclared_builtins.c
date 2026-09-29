// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c17
typedef unsigned long size_t;
size_t strlen(const char *);
void abort(void);
int abs();
int malloc(int);
double fabs(const double);
void *memcpy(void *restrict, const void *restrict, size_t);
static int labs(int x) { return x; }
int __builtin_popcount(unsigned);

size_t matching(const char *s) { return strlen(s); }
void missing_noreturn(void) { abort(); }
int unprototyped(int x) { return abs(x); }
int incompatible(void) { return malloc(4); }
double qualified_parameter(double x) { return fabs(x); }
void *restricted(void *d, const void *s, size_t n) { return memcpy(d, s, n); }
int internal_linkage(int x) { return labs(x); }
int reserved(unsigned x) { return __builtin_popcount(x); }
int block_scope(const char *a, const char *b) {
    int strcmp(const char *, const char *);
    return strcmp(a, b);
}
size_t undeclared(const char *s) { return __builtin_strlen(s); }
int exit(long);
int incompatible_noreturn(void) { return exit(2); }
static void _Exit(int code) { for (;;); }
void internal_noreturn(void) { _Exit(1); }

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
// IR-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// IR-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// IR-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// IR-NEXT:     fn %[[VALUE_abs:[0-9]+]] @abs(unprototyped) -> i32 [linkage=external] [memory=none];
// IR-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE1:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_fabs:[0-9]+]] @fabs(%[[VALUE2:[0-9]+]] <unnamed>: f64 [const]) -> f64 [linkage=external] [memory=none];
// IR-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE3:[0-9]+]] <unnamed>: ptr<void> [restrict], %[[VALUE4:[0-9]+]] <unnamed>: ptr<const void> [restrict], %[[VALUE5:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// IR-NEXT:     fn %[[VALUE_labs:[0-9]+]] @labs(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_x]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE___builtin_popcount:[0-9]+]] @__builtin_popcount(%[[VALUE6:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// IR-NEXT:     fn %[[VALUE_matching:[0-9]+]] @matching(%[[VALUE_s:[0-9]+]] s: ptr<const i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], read<ptr<const i8>>(%[[VALUE_s]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_missing_noreturn:[0-9]+]] @missing_noreturn() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_unprototyped:[0-9]+]] @unprototyped(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32) -> i32>(%[[VALUE_abs]], read<i32>(%[[VALUE_x_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_incompatible:[0-9]+]] @incompatible() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32) -> i32>(%[[VALUE_malloc]], const<i32>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_qualified_parameter:[0-9]+]] @qualified_parameter(%[[VALUE_x_3:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_x_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_restricted:[0-9]+]] @restricted(%[[VALUE_d:[0-9]+]] d: ptr<void>, %[[VALUE_s_2:[0-9]+]] s: ptr<const void>, %[[VALUE_n:[0-9]+]] n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_d]]), read<ptr<const void>>(%[[VALUE_s_2]]), read<u64>(%[[VALUE_n]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_internal_linkage:[0-9]+]] @internal_linkage(%[[VALUE_x_4:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32) -> i32>(%[[VALUE_labs]], read<i32>(%[[VALUE_x_4]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_reserved:[0-9]+]] @reserved(%[[VALUE_x_5:[0-9]+]] x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], read<u32>(%[[VALUE_x_5]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE7:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE8:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_block_scope:[0-9]+]] @block_scope(%[[VALUE_a:[0-9]+]] a: ptr<const i8>, %[[VALUE_b:[0-9]+]] b: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], read<ptr<const i8>>(%[[VALUE_a]]), read<ptr<const i8>>(%[[VALUE_b]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE___builtin_strlen:[0-9]+]] @__builtin_strlen(%[[VALUE9:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// IR-NEXT:     fn %[[VALUE_undeclared:[0-9]+]] @undeclared(%[[VALUE_s_3:[0-9]+]] s: ptr<const i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE___builtin_strlen]], read<ptr<const i8>>(%[[VALUE_s_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE10:[0-9]+]] <unnamed>: i64) -> i32 [linkage=external] [noreturn];
// IR-NEXT:     fn %[[VALUE_incompatible_noreturn:[0-9]+]] @incompatible_noreturn() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i64) -> i32>(%[[VALUE_exit]], widen<i64, reason=arg>(const<i32>(2)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE__Exit:[0-9]+]] @_Exit(%[[VALUE_code:[0-9]+]] code: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// IR-NEXT:         for %[[VALUE11:[0-9]+]]
// IR-NEXT:             init:
// IR-NEXT:             condition: omitted
// IR-NEXT:             increment: omitted
// IR-NEXT:             body:
// IR-NEXT:                 ;
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_internal_noreturn:[0-9]+]] @internal_noreturn() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE__Exit]], const<i32>(1));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
