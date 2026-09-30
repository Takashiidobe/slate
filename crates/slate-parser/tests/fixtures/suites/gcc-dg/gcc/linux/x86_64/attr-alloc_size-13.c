/* PR c/78666 - conflicting attribute alloc_size accepted
   { dg-do compile }
   { dg-options "-Wall" } */

#define A(...) __attribute__ ((alloc_size (__VA_ARGS__)))

A (1) char* f2_1 (int, int);
A (1) A (1) char* f2_1 (int, int);

A (1) char* f2_1 (int, int);            // { dg-message "previous declaration here" }

A (2) char* f2_1 (int, int);            // { dg-warning "ignoring attribute 'alloc_size \\\(2\\\)' because it conflicts with previous 'alloc_size \\\(1\\\)'" }


A (2) char* f2_2 (int, int);
A (2) char* f2_2 (int, int);            // { dg-message "previous declaration here" }

A (1) char* f2_2 (int, int);            // { dg-warning "ignoring attribute 'alloc_size \\\(1\\\)' because it conflicts with previous 'alloc_size \\\(2\\\)'" }


A (1) char* f3_1 (int, int, int);
A (1) char* f3_1 (int, int, int);       // { dg-message "previous declaration here" }

A (2) char* f3_1 (int, int, int);       // { dg-warning "ignoring attribute 'alloc_size \\\(2\\\)' because it conflicts with previous 'alloc_size \\\(1\\\)'" }
A (3) char* f3_1 (int, int, int);       // { dg-warning "ignoring attribute 'alloc_size \\\(3\\\)' because it conflicts with previous 'alloc_size \\\(1\\\)'" }
A (1, 2) char* f3_1 (int, int, int);    // { dg-warning "ignoring attribute 'alloc_size \\\(1, 2\\\)' because it conflicts with previous 'alloc_size \\\(1\\\)'" }
A (1, 3) char* f3_1 (int, int, int);    // { dg-warning "ignoring attribute 'alloc_size \\\(1, 3\\\)' because it conflicts with previous 'alloc_size \\\(1\\\)'" }


typedef A (2, 3) char* F3_2_3 (int, int, int);
typedef A (2, 3) char* F3_2_3 (int, int, int);
typedef A (2, 3) A (2, 3) char* F3_2_3 (int, int, int);

typedef A (1) char* F3_2_3 (int, int, int);   // { dg-warning "ignoring attribute 'alloc_size \\\(1\\\)' because it conflicts with previous 'alloc_size \\\(2, 3\\\)'" }

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     type @type[[TYPE_F3_2_3:[0-9]+]] F3_2_3 = fn(i32, i32, i32) -> ptr<i8>;
// DEFAULT-NEXT:     type @type[[TYPE_F3_2_3_2:[0-9]+]] F3_2_3 = fn(i32, i32, i32) -> ptr<i8>;
// DEFAULT-NEXT:     type @type[[TYPE_F3_2_3_3:[0-9]+]] F3_2_3 = fn(i32, i32, i32) -> ptr<i8>;
// DEFAULT-NEXT:     type @type[[TYPE_F3_2_3_4:[0-9]+]] F3_2_3 = fn(i32, i32, i32) -> ptr<i8>;
// DEFAULT-NEXT:     fn %[[VALUE_f2_1:[0-9]+]] @f2_1(%[[VALUE0:[0-9]+]] <unnamed>: i32, %[[VALUE1:[0-9]+]] <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f2_2:[0-9]+]] @f2_2(%[[VALUE2:[0-9]+]] <unnamed>: i32, %[[VALUE3:[0-9]+]] <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f3_1:[0-9]+]] @f3_1(%[[VALUE4:[0-9]+]] <unnamed>: i32, %[[VALUE5:[0-9]+]] <unnamed>: i32, %[[VALUE6:[0-9]+]] <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
