#ifdef __OPTIMIZE__
int optimize;
#endif
#ifdef __OPTIMIZE_SIZE__
int optimize_size;
#endif
#ifdef __NO_INLINE__
int no_inline;
#endif

// SLATE-FILECHECK-ARGS -O2 -Os
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: global %{{[0-9]+}} optimize: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: global %{{[0-9]+}} optimize_size: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
