/* Test for warnings for qualified function return types.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=gnu99 -Wreturn-type -Wignored-qualifiers" } */

/* Qualifying a function return type makes no sense.  */

const int int_fn (void); /* { dg-warning "qualifiers" "int decl" } */
const int (*int_ptr) (void); /* { dg-warning "qualifiers" "int ptr" } */
const int int_fn2 (void) { return 0; } /* { dg-warning "qualifiers" "int defn" } */

const void void_fn (void); /* { dg-warning "qualifiers" "void decl" } */
const void (*void_ptr) (void); /* { dg-warning "qualifiers" "void ptr" } */
const void void_fn2 (void) { } /* { dg-warning "qualified" "void defn" } */

volatile void vvoid_fn (void); /* { dg-warning "qualifiers" "void decl" } */
volatile void (*vvoid_ptr) (void); /* { dg-warning "qualifiers" "void ptr" } */
volatile void vvoid_fn2 (void) { } /* { dg-warning "qualified" "void defn" } */

int *restrict ip_fn (void); /* { dg-warning "qualifiers" "restrict decl" } */
int *restrict (*ip_ptr) (void); /* { dg-warning "qualifiers" "restrict ptr" } */
int *restrict ip_fn2 (void) { return (int *)0; }; /* { dg-warning "qualifiers" "restrict defn" } */

// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     global %1 int_ptr: ptr<fn() -> i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 void_ptr: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 vvoid_ptr: ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 ip_ptr: ptr<fn() -> ptr<i32>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @int_fn() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @int_fn2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @void_fn() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @void_fn2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @vvoid_fn() -> void [linkage=external];
// DEFAULT-NEXT:     fn %8 @vvoid_fn2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @ip_fn() -> ptr<i32> [linkage=external];
// DEFAULT-NEXT:     fn %11 @ip_fn2() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<i32>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
