/* PR middle-end/83859 - attribute to establish relation between parameters
   for buffer and its size
   Test to verify the handling of attribute read_only combining multiple
   declarations of the same function.
   { dg-do compile }
   { dg-options "-Wall -ftrack-macro-expansion=0" } */

#define RW(...)    __attribute__ ((access (read_write, __VA_ARGS__)))
#define WO(...)  __attribute__ ((access (write_only, __VA_ARGS__)))

int rdwr1_rdwr1 (void*, void*);
int RW (1) RW (1) rdwr1_rdwr1 (void*, void*);
int RW (2) RW (2) rdwr1_rdwr1 (void*, void*);
int RW (1) RW (2) rdwr1_rdwr1 (void*, void*);
int RW (2) RW (1) rdwr1_rdwr1 (void*, void*);

int frdwr1_wr1 (void*, void*);
int RW (1) WO (1) frdwr1_wr1 (void*, void*);    // { dg-warning "attribute 'access\\(write_only, 1\\)' mismatch with mode 'read_write'" }

int RW (1) grdwr1_wr1 (void*, void*);           // { dg-message "previous declaration here" }

int WO (1) grdwr1_wr1 (void*, void*);         // { dg-warning "attribute 'access\\(write_only, 1\\)' mismatch with mode 'read_write'" }


int RW (1) RW (1, 2) frdwr1_rdwr1_1 (void*, int);   // { dg-warning "attribute 'access\\(read_write, 1, 2\\)' positional argument 2 missing in previous designation" }

int RW (1, 2) RW (1) frdwr1_1_rdwr1 (void*, int);   // { dg-warning "attribute 'access\\(read_write, 1\\)' missing positional argument 2 provided in previous designation" }

int RW (1)    grdwr1_rdwr1_1 (void*, int);   // { dg-message "previous declaration here" }
int RW (1, 2) grdwr1_rdwr1_1 (void*, int);   // { dg-warning "attribute 'access\\(read_write, 1, 2\\)' positional argument 2 missing in previous designation" }


typedef int *P;

int RW(1) WO(3) RW(5) WO(7) RW(9) WO(11) RW(13) WO(15) frw1_w3_rw5_w7_rw9_wr11_rw13_w15 (P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, int);

int RW(1) WO(3) RW(5) WO(7) RW(9) WO(11) RW(13) WO(15) frw1_w3_rw5_w7_rw9_wr11_rw13_w15 (P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, int);

int WO(1) WO(3) RW(5) WO(7) RW(9) WO(11) RW(13) WO(15) frw1_w3_rw5_w7_rw9_wr11_rw13_w15 (P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, int);
// { dg-warning "attribute 'access\\(write_only, 1\\)' mismatch with mode 'read_write'" "1" { target *-*-* } .-1 }

int RW(1) RW(3) RW(5) WO(7) RW(9) WO(11) RW(13) WO(15) frw1_w3_rw5_w7_rw9_wr11_rw13_w15 (P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, int);
// { dg-warning "attribute 'access\\(read_write, 3\\)' mismatch with mode 'write_only'" "3" { target *-*-* } .-1 }

int RW(1) WO(3) WO(5) WO(7) RW(9) WO(11) RW(13) WO(15) frw1_w3_rw5_w7_rw9_wr11_rw13_w15 (P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, int);
// { dg-warning "attribute 'access\\(write_only, 5\\)' mismatch with mode 'read_write'" "5" { target *-*-* } .-1 }

int RW(1) WO(3) RW(5) RW(7) RW(9) WO(11) RW(13) WO(15) frw1_w3_rw5_w7_rw9_wr11_rw13_w15 (P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, int);
// { dg-warning "attribute 'access\\(read_write, 7\\)' mismatch with mode 'write_only'" "7" { target *-*-* } .-1 }

int RW(1) WO(3) RW(5) WO(7) WO(9) WO(11) RW(13) WO(15) frw1_w3_rw5_w7_rw9_wr11_rw13_w15 (P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, int);
// { dg-warning "attribute 'access\\(write_only, 9\\)' mismatch with mode 'read_write'" "9" { target *-*-* } .-1 }

int RW(1) WO(3) RW(5) WO(7) RW(9) RW(11) RW(13) WO(15) frw1_w3_rw5_w7_rw9_wr11_rw13_w15 (P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, int);
// { dg-warning "attribute 'access\\(read_write, 11\\)' mismatch with mode 'write_only'" "11" { target *-*-* } .-1 }

int RW(1) WO(3) RW(5) WO(7) RW(9) WO(11) WO(13) WO(15) frw1_w3_rw5_w7_rw9_wr11_rw13_w15 (P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, int);
// { dg-warning "attribute 'access\\(write_only, 13\\)' mismatch with mode 'read_write'" "13" { target *-*-* } .-1 }

int RW(1) WO(3) RW(5) WO(7) RW(9) WO(11) RW(13) RW(15) frw1_w3_rw5_w7_rw9_wr11_rw13_w15 (P, P, P, P, P, P, P, P, P, P, P, P, P, P, P, int);
// { dg-warning "attribute 'access\\(read_write, 15\\)' mismatch with mode 'write_only'" "15" { target *-*-* } .-1 }

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
// DEFAULT-NEXT:     type @type[[TYPE_P:[0-9]+]] P = ptr<i32>;
// DEFAULT-NEXT:     fn %[[VALUE_rdwr1_rdwr1:[0-9]+]] @rdwr1_rdwr1(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_frdwr1_wr1:[0-9]+]] @frdwr1_wr1(%[[VALUE2:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE3:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_grdwr1_wr1:[0-9]+]] @grdwr1_wr1(%[[VALUE4:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE5:[0-9]+]] <unnamed>: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_frdwr1_rdwr1_1:[0-9]+]] @frdwr1_rdwr1_1(%[[VALUE6:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE7:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_frdwr1_1_rdwr1:[0-9]+]] @frdwr1_1_rdwr1(%[[VALUE8:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE9:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_grdwr1_rdwr1_1:[0-9]+]] @grdwr1_rdwr1_1(%[[VALUE10:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE11:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_frw1_w3_rw5_w7_rw9_wr11_rw13_w15:[0-9]+]] @frw1_w3_rw5_w7_rw9_wr11_rw13_w15(%[[VALUE12:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE13:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE14:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE15:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE16:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE17:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE18:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE19:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE20:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE21:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE22:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE23:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE24:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE25:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE26:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE27:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
