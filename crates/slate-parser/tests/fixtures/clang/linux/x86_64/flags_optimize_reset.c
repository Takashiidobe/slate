#ifdef __OPTIMIZE__
int optimize;
#endif
#ifdef __OPTIMIZE_SIZE__
int optimize_size;
#endif
#ifdef __NO_INLINE__
int no_inline;
#endif

// SLATE-FILECHECK-ARGS -O2 -O0
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: storage d128 [size=16, align=16];
// DEFAULT-NEXT: }
// DEFAULT-NEXT: global %{{[0-9]+}} no_inline: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
