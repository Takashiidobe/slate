



typedef void *omp_interop_t;

#ifdef WIN

void win_foov(int n, double *y, void *interop_obj);


#pragma omp declare variant (win_foov) \
  match(construct={dispatch}, device={arch(x86_64)}) \
  append_args(interop(targetsync))
void _cdecl win_foo(int n, double *y);
#endif // WIN


void c_foov(int n, double *y, void *interop_obj);


#pragma omp declare variant (c_foov) \
  match(construct={dispatch}, device={arch(x86_64)}) \
  append_args(interop(targetsync))
void c_foo(int n, double *y);

// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc
// SLATE-FILECHECK-DEFINES DEFAULT -DWIN
// SLATE-FILECHECK-STD DEFAULT c17
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT -Wno-source-uses-openmp -Wno-openmp-clauses

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 omp_interop_t = ptr<void>;
// DEFAULT-NEXT:     fn %1 @win_foov(%5 n: i32, %6 y: ptr<f64>, %7 interop_obj: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @win_foo(%8 n: i32, %9 y: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @c_foov(%10 n: i32, %11 y: ptr<f64>, %12 interop_obj: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @c_foo(%13 n: i32, %14 y: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
