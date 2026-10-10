/* { dg-do compile } */
/* { dg-options "-std=gnu99" } */

/* Check that d or D is recognized as a float constant suffix.  */

double d = 0.5d;
double D = 0.5D;

/* Check that d or D is recognized in a hexadecimal floating constant.  */

double hd1 = 0x1.8p1d;
double hd2 = 0x1.p+1D;
double hd3 = 0x0.8p-1d;

/* Check that imaginary constant suffixes are still recognized with
   only i, I, j, or J.  */

double i = 0.5i;
double I = 0.5I;
double j = 0.5j;
double J = 0.5J;

/* Check that imaginary constant suffixes are allowed with d or D.  */

double di = 0.5di;
double dI = 0.5dI;
double Di = 0.5Di;
double DI = 0.5DI;
double dj = 0.5dj;
double dJ = 0.5dJ;
double Dj = 0.5Dj;
double DJ = 0.5DJ;
double id = 0.5id;
double iD = 0.5iD;
double Id = 0.5Id;
double ID = 0.5ID;
double jd = 0.5jd;
double jD = 0.5jD;
double Jd = 0.5Jd;
double JD = 0.5JD;

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
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: f64 [storage=static] = const<f64>(0.5) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_D:[0-9]+]] D: f64 [storage=static] = const<f64>(0.5) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_hd1:[0-9]+]] hd1: f64 [storage=static] = const<f64>(3.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_hd2:[0-9]+]] hd2: f64 [storage=static] = const<f64>(2.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_hd3:[0-9]+]] hd3: f64 [storage=static] = const<f64>(0.25) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_I:[0-9]+]] I: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_J:[0-9]+]] J: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_di:[0-9]+]] di: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_dI:[0-9]+]] dI: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_Di:[0-9]+]] Di: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_DI:[0-9]+]] DI: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_dj:[0-9]+]] dj: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_dJ:[0-9]+]] dJ: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_Dj:[0-9]+]] Dj: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_DJ:[0-9]+]] DJ: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_id:[0-9]+]] id: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_iD:[0-9]+]] iD: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_Id:[0-9]+]] Id: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ID:[0-9]+]] ID: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_jd:[0-9]+]] jd: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_jD:[0-9]+]] jD: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_Jd:[0-9]+]] Jd: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_JD:[0-9]+]] JD: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.5))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
