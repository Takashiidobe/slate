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
// IR-NEXT:     type @type0 size_t = u64;
// IR-NEXT:     fn %1 @strlen(%32 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// IR-NEXT:     fn %2 @abort() -> void [linkage=external];
// IR-NEXT:     fn %3 @abs(unprototyped) -> i32 [linkage=external];
// IR-NEXT:     fn %4 @malloc(%33 <unnamed>: i32) -> i32 [linkage=external];
// IR-NEXT:     fn %5 @fabs(%34 <unnamed>: f64 [const]) -> f64 [linkage=external];
// IR-NEXT:     fn %6 @memcpy(%35 <unnamed>: ptr<void> [restrict], %36 <unnamed>: ptr<const void> [restrict], %37 <unnamed>: u64) -> ptr<void> [linkage=external];
// IR-NEXT:     fn %7 @labs(%8 x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%8);
// IR-NEXT:     }
// IR-NEXT:     fn %9 @__builtin_popcount(%38 <unnamed>: u32) -> i32 [linkage=external];
// IR-NEXT:     fn %10 @matching(%11 s: ptr<const i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<u64, signature=fn(ptr<const i8>) -> u64>(strlen, read<ptr<const i8>>(%11));
// IR-NEXT:     }
// IR-NEXT:     fn %12 @missing_noreturn() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         call<void, signature=fn() -> void>(abort);
// IR-NEXT:     }
// IR-NEXT:     fn %13 @unprototyped(%14 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32) -> i32>(abs, read<i32>(%14));
// IR-NEXT:     }
// IR-NEXT:     fn %15 @incompatible() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32) -> i32>(%4, const<i32>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %16 @qualified_parameter(%17 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<f64, signature=fn(f64) -> f64>(fabs, read<f64>(%17));
// IR-NEXT:     }
// IR-NEXT:     fn %18 @restricted(%19 d: ptr<void>, %20 s: ptr<const void>, %21 n: u64) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(memcpy, read<ptr<void>>(%19), read<ptr<const void>>(%20), read<u64>(%21));
// IR-NEXT:     }
// IR-NEXT:     fn %22 @internal_linkage(%23 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32) -> i32>(%7, read<i32>(%23));
// IR-NEXT:     }
// IR-NEXT:     fn %24 @reserved(%25 x: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(u32) -> i32>(__builtin_popcount, read<u32>(%25));
// IR-NEXT:     }
// IR-NEXT:     fn %29 @strcmp(%39 <unnamed>: ptr<const i8>, %40 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// IR-NEXT:     fn %26 @block_scope(%27 a: ptr<const i8>, %28 b: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(strcmp, read<ptr<const i8>>(%27), read<ptr<const i8>>(%28));
// IR-NEXT:     }
// IR-NEXT:     fn %30 @undeclared(%31 s: ptr<const i8>) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<u64, signature=fn(ptr<const i8>) -> u64>(__builtin_strlen, read<ptr<const i8>>(%31));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
