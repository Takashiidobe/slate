/* { dg-do compile }
   { dg-options "-Wall -Wattributes -ftrack-macro-expansion=0" } */

#define ATTR(attrlist) __attribute__ (attrlist)

/* Exercise the handling of the mutually exclusive attributes
   aligned and packed on a type definition.  */

/* Pointless but benign.  */
struct ATTR ((aligned, aligned))
AlignedAligned { int i; };

/* Aligned followed by packed on a type and vice versa has a valid use:
   to decrease the alignment of a type to the specified boundary.  */
struct ATTR ((aligned, packed))
AlignedPacked { int i; };

struct ATTR ((packed, aligned))
PackedAligned { int i; };

struct ATTR ((aligned (2)))
AlignedMemberPacked
{
  int ATTR ((packed)) i; // { dg-warning "attribute ignored" "" { target default_packed } }
};

struct ATTR ((packed))
PackedMemberAligned
{
  int ATTR ((aligned (2))) i;
};

/* Silly but benign.  */
struct ATTR ((packed, packed))
PackedPacked { int i; };


/* Exercise the handling of the mutually exclusive attributes
   aligned and packed on a function declaration.  */

void ATTR ((aligned (8), packed))
faligned8_1 (void);           /* { dg-warning "ignoring attribute .packed. because it conflicts with attribute .aligned." } */

void ATTR ((aligned (8)))
faligned8_2 (void);           /* { dg-message "previous declaration here" } */

void ATTR ((packed))
faligned8_2 (void);           /* { dg-warning "ignoring attribute .packed. because it conflicts with attribute .aligned." } */

/* Exercise the handling of the mutually exclusive attributes
   always_inline and noinline (in that order).  */

inline void ATTR ((always_inline))
falways_inline1 (void);

inline void ATTR ((__always_inline__))
falways_inline1 (void);

/* Verify that repeating attribute always_inline on the same declaration
   doesn't trigger a warning.  */
inline void ATTR ((always_inline, __always_inline__))
falways_inline1 (void);

/* Verify that repeating attribute always_inline on a distinct declaration
   doesn't trigger a warning.  */
inline void ATTR ((always_inline))
falways_inline1 (void);       /* { dg-message "previous declaration here" } */

/* Verify a warning for noinline conflict.  */
void ATTR ((noinline))
falways_inline1 (void) { }    /* { dg-warning "ignoring attribute .noinline. because it conflicts with attribute .always_inline." } */

/* And again.  */
void ATTR ((always_inline))
falways_inline1 (void);


/* Exercise the handling of the mutually exclusive attributes
   noinline and always_inline (in that order).  */

void ATTR ((noinline))
fnoinline1 (void);

void ATTR ((__noinline__))
fnoinline1 (void);

/* Verify that repeating attribute noinline on the same declaration
   doesn't trigger a warning.  */
void ATTR ((noinline, __noinline__))
fnoinline1 (void);

/* Verify that repeating attribute noinline on a distinct declaration
   doesn't trigger a warning.  */
void ATTR ((noinline))
fnoinline1 (void);       /* { dg-message "previous declaration here" } */

/* Verify a warning for always_inline conflict.  */
void ATTR ((always_inline))
fnoinline1 (void) { }    /* { dg-warning "ignoring attribute .always_inline. because it conflicts with attribute .noinline." } */
			 /* { dg-note	 "previous declaration here" "" { target *-*-* } .-1 } */
			 /* { dg-note	 "previous definition" "" { target *-*-* } .-2 } */

/* Verify a warning for gnu_inline conflict.  */
inline void ATTR ((gnu_inline))
fnoinline1 (void);      /* { dg-warning "ignoring attribute .gnu_inline. because it conflicts with attribute .noinline." } */
                        /* { dg-warning "follows declaration with attribute .noinline." "inline noinline" { target *-*-* } .-1 } */

/* And again.  */
void ATTR ((noinline))
fnoinline1 (void);      /* { dg-warning "follows inline declaration" } */


/* Exercise the handling of the mutually exclusive attributes
   noreturn and warn_unused_result.  */

int ATTR ((__noreturn__))
fnoret1 (void);

int ATTR ((noreturn))
fnoret1 (void);               /* { dg-message "previous declaration here" } */

int ATTR ((warn_unused_result))
fnoret1 (void);               /* { dg-warning "ignoring attribute .warn_unused_result. because it conflicts with attribute .noreturn." } */

/* Verify that repeating attribute noreturn doesn't trigger a warning.  */
int ATTR ((noreturn)) fnoret1 (void);

int call_noret1 (void)
{
  /* Verify that attribute warn_unused_result was, in fact, ignored
     on the second declaration of fnoret1.  */
  fnoret1 ();
}

int ATTR ((noreturn, warn_unused_result))
fnoret2 (void);               /* { dg-warning "ignoring attribute .warn_unused_result. because it conflicts with attribute .noreturn." } */

/* Verify that repeating attribute noreturn doesn't trigger a warning.  */
int ATTR ((noreturn)) fnoret2 (void);

int call_noret2 (void)
{
  /* Verify that attribute warn_unused_result was, in fact, ignored
     on the second declaration of fnoret2.  */
  fnoret2 ();
}

/* Verify that attribute noreturn isn't diagnosed on a declaration
   that was previously declared warn_unused_result and that attribute
   was dropped (because the function returs void).  */

void ATTR ((warn_unused_result))
fnorety6 (void);             /* { dg-warning ".warn_unused_result. attribute ignored" } */

void ATTR ((noreturn))
fnoret6 (void);


/* Exercise the handling of the mutually exclusive attributes
   noreturn and alloc_align.  */

void* ATTR ((noreturn))
fnoret_alloc_align1 (int);    /* { dg-message "previous declaration here" } */

void* ATTR ((alloc_align (1)))
fnoret_alloc_align1 (int);    /* { dg-warning "ignoring attribute .alloc_align. because it conflicts with attribute .noreturn." } */

void* ATTR ((noreturn, alloc_align (1)))
fnoret_alloc_align2 (int);    /* { dg-warning "ignoring attribute .alloc_align. because it conflicts with attribute .noreturn." } */


void* ATTR ((noreturn)) ATTR ((alloc_align (1)))
fnoret_alloc_align3 (int);    /* { dg-warning "ignoring attribute .alloc_align. because it conflicts with attribute .noreturn." } */


void* ATTR ((alloc_align (1)))
falloc_align_noret1 (int);    /* { dg-message "previous declaration here" } */

void* ATTR ((noreturn))
falloc_align_noret1 (int);    /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .alloc_align." } */


void* ATTR ((alloc_align (1), noreturn))
falloc_align_noret2 (int);    /* { dg-warning "ignoring attribute .(noreturn|alloc_align). because it conflicts with attribute .(alloc_align|noreturn)." } */

void* ATTR ((alloc_align (1))) ATTR ((noreturn))
falloc_align_noret3 (int);    /* { dg-warning "ignoring attribute .(noreturn|alloc_align). because it conflicts with attribute .(noreturn|alloc_align)." } */


/* Exercise the handling of the mutually exclusive attributes
   noreturn and alloc_size.  */

void* ATTR ((noreturn))
fnoret_alloc_size1 (int);     /* { dg-message "previous declaration here" } */

void* ATTR ((alloc_size (1)))
fnoret_alloc_size1 (int);     /* { dg-warning "ignoring attribute .alloc_size. because it conflicts with attribute .noreturn." } */

void* ATTR ((noreturn, alloc_size (1)))
fnoret_alloc_size2 (int);     /* { dg-warning "ignoring attribute .alloc_size. because it conflicts with attribute .noreturn." } */


void* ATTR ((noreturn)) ATTR ((alloc_size (1)))
fnoret_alloc_size3 (int);     /* { dg-warning "ignoring attribute .alloc_size. because it conflicts with attribute .noreturn." } */


void* ATTR ((alloc_size (1)))
falloc_size_noret1 (int);     /* { dg-message "previous declaration here" } */

void* ATTR ((noreturn))
falloc_size_noret1 (int);     /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .alloc_size." } */


void* ATTR ((alloc_size (1), noreturn))
falloc_size_noret2 (int);     /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .alloc_size." } */

void* ATTR ((alloc_size (1))) ATTR ((noreturn))
falloc_size_noret3 (int);     /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .alloc_size." } */


/* Exercise the handling of the mutually exclusive attributes
   noreturn and const.  */

int ATTR ((noreturn))
fnoret_const1 (int);          /* { dg-message "previous declaration here" } */

int ATTR ((const))
fnoret_const1 (int);          /* { dg-warning "ignoring attribute .const. because it conflicts with attribute .noreturn." } */

/* Unfortunately, attributes on a single declarations may not be processed
   in the same order as specified... */
int ATTR ((noreturn, const))
fnoret_const2 (int);          /* { dg-warning "ignoring attribute .const. because it conflicts with attribute .noreturn." } */


int ATTR ((noreturn)) ATTR ((const))
fnoret_const3 (int);          /* { dg-warning "ignoring attribute .const. because it conflicts with attribute .noreturn." } */


int ATTR ((const))
fconst_noret1 (int);          /* { dg-message "previous declaration here" } */

int ATTR ((noreturn))
fconst_noret1 (int);          /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .const." } */


int ATTR ((const, noreturn))
fconst_noret2 (int);          /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .const." } */

int ATTR ((const)) ATTR ((noreturn))
fconst_noret3 (int);          /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .const." } */


/* Exercise the handling of the mutually exclusive attributes
   noreturn and malloc.  */

void* ATTR ((noreturn))
fnoret_malloc1 (int);         /* { dg-message "previous declaration here" } */

void* ATTR ((malloc))
fnoret_malloc1 (int);         /* { dg-warning "ignoring attribute .malloc. because it conflicts with attribute .noreturn." } */

/* Unfortunately, attributes on a single declarations may not be processed
   in the same order as specified... */
void* ATTR ((noreturn, malloc))
fnoret_malloc2 (int);         /* { dg-warning "ignoring attribute .malloc. because it conflicts with attribute .noreturn." } */


void* ATTR ((noreturn)) ATTR ((malloc))
fnoret_malloc3 (int);         /* { dg-warning "ignoring attribute .malloc. because it conflicts with attribute .noreturn." } */


void* ATTR ((__malloc__))
fmalloc_noret1 (int);

void* ATTR ((malloc))
fmalloc_noret1 (int);         /* { dg-message "previous declaration here" } */

void* ATTR ((noreturn))
fmalloc_noret1 (int);         /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .malloc." } */


void* ATTR ((malloc, noreturn))
fmalloc_noret2 (int);         /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .malloc." } */

void* ATTR ((malloc)) ATTR ((noreturn))
fmalloc_noret3 (int);         /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .malloc." } */


/* Exercise the handling of the mutually exclusive attributes
   noreturn and pure.  */

int ATTR ((noreturn))
fnoret_pure1 (int);           /* { dg-message "previous declaration here" } */

int ATTR ((pure))
fnoret_pure1 (int);           /* { dg-warning "ignoring attribute .pure. because it conflicts with attribute .noreturn." } */

/* Unfortunately, attributes on a single declarations may not be processed
   in the same order as specified... */
int ATTR ((noreturn, pure))
fnoret_pure2 (int);           /* { dg-warning "ignoring attribute .pure. because it conflicts with attribute .noreturn." } */


int ATTR ((noreturn)) ATTR ((pure))
fnoret_pure3 (int);           /* { dg-warning "ignoring attribute .pure. because it conflicts with attribute .noreturn." } */


int ATTR ((__pure__))
fpure_noret1 (int);

int ATTR ((pure))
fpure_noret1 (int);           /* { dg-message "previous declaration here" } */

int ATTR ((noreturn))
fpure_noret1 (int);           /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .pure." } */


int ATTR ((pure, noreturn))
fpure_noret2 (int);           /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .pur." } */

int ATTR ((pure)) ATTR ((noreturn))
fpure_noret3 (int);            /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .pure." } */


/* Exercise the handling of the mutually exclusive attributes
   noreturn and returns_twice.  */

int ATTR ((noreturn))
fnoret_returns_twice1 (int);  /* { dg-message "previous declaration here" } */

int ATTR ((returns_twice))
fnoret_returns_twice1 (int);  /* { dg-warning "ignoring attribute .returns_twice. because it conflicts with attribute .noreturn." } */

/* Unfortunately, attributes on a single declarations may not be processed
   in the same order as specified... */
int ATTR ((noreturn, returns_twice))
fnoret_returns_twice2 (int);  /* { dg-warning "ignoring attribute .returns_twice. because it conflicts with attribute .noreturn." } */


int ATTR ((noreturn)) ATTR ((returns_twice))
fnoret_returns_twice3 (int);  /* { dg-warning "ignoring attribute .returns_twice. because it conflicts with attribute .noreturn." } */


int ATTR ((returns_twice))
freturns_twice_noret1 (int);  /* { dg-message "previous declaration here" } */

int ATTR ((noreturn))
freturns_twice_noret1 (int);  /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .returns_twice." } */


int ATTR ((returns_twice, noreturn))
freturns_twice_noret2 (int);  /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .returns_twice." } */

int ATTR ((returns_twice)) ATTR ((noreturn))
freturns_twice_noret3 (int);  /* { dg-warning "ignoring attribute .noreturn. because it conflicts with attribute .returns_twice." } */


/* Exercise the interaction of multiple combinations of mutually
   exclusive attributes specified on distinct declarations.  */

inline int ATTR ((always_inline))
finline_cold_noreturn (int);

inline int ATTR ((cold))
finline_cold_noreturn (int);

inline int ATTR ((noreturn))
finline_cold_noreturn (int);	/* { dg-note	"previous declaration here" } */

inline int ATTR ((noinline))
finline_cold_noreturn (int);    /* { dg-warning "ignoring attribute .noinline. because it conflicts with attribute .always_inline." } */
				/* { dg-note	"previous declaration here" "" { target *-*-* } .-1 } */

inline int ATTR ((hot))
finline_cold_noreturn (int);    /* { dg-warning "ignoring attribute .hot. because it conflicts with attribute .cold." } */
				/* { dg-note	"previous declaration here" "" { target *-*-* } .-1 } */

inline int ATTR ((warn_unused_result))
finline_cold_noreturn (int);    /* { dg-warning "ignoring attribute .warn_unused_result. because it conflicts with attribute .noreturn." } */

inline int ATTR ((always_inline))
finline_cold_noreturn (int);

/* Expect no warning for the missing return statement below because
   the function is noreturn.  */
inline int ATTR ((noreturn))
finline_cold_noreturn (int i) { (void)&i; __builtin_abort (); }


/* Exercise the interaction of multiple combinations of mutually
   exclusive attributes with some specified on the same declaration
   and some on distinct declarations.  */

inline int ATTR ((always_inline, hot))
finline_hot_noret_align (int);	/* { dg-note	"previous declaration here" } */

inline int ATTR ((noreturn, noinline))
finline_hot_noret_align (int);  /* { dg-warning "ignoring attribute .noinline. because it conflicts with attribute .always_inline." } */
				/* { dg-note	"previous declaration here" "" { target *-*-* } .-1 } */

inline int ATTR ((cold, aligned (8)))
finline_hot_noret_align (int);  /* { dg-warning "ignoring attribute .cold. because it conflicts with attribute .hot." } */
				/* { dg-note	"previous declaration here" "" { target *-*-* } .-1 } */

inline int ATTR ((warn_unused_result))
finline_hot_noret_align (int);  /* { dg-warning "ignoring attribute .warn_unused_result. because it conflicts with attribute .noreturn." } */
				/* { dg-note	"previous declaration here" "" { target *-*-* } .-1 } */

inline int ATTR ((aligned (4)))
  finline_hot_noret_align (int);  /* { dg-warning "ignoring attribute .aligned \\(4\\). because it conflicts with attribute .aligned \\(8\\)." "" } */

inline int ATTR ((aligned (8)))
finline_hot_noret_align (int);  /* { dg-note	"previous declaration here" } */

inline int ATTR ((const))
finline_hot_noret_align (int);  /* { dg-warning "ignoring attribute .const. because it conflicts with attribute .noreturn." } */

/* Expect no warning for the missing return statement below because
   the function is noreturn.  */
inline int ATTR ((noreturn))
finline_hot_noret_align (int i) { (void)&i; __builtin_abort (); }


/* Expect a warning about conflicting alignment but without
   other declarations inbetween.  */
inline int ATTR ((aligned (32)))
finline_align (int);	        /* { dg-note	"previous declaration here" } */

inline int ATTR ((aligned (4)))
finline_align (int);  /* { dg-warning "ignoring attribute .aligned \\(4\\). because it conflicts with attribute .aligned \\(32\\)." "" } */

inline int ATTR ((noreturn))
finline_align (int i) { (void)&i; __builtin_abort (); }


/* Expect no note that would refer to the same declaration.  */
inline int ATTR ((aligned (32), aligned (4)))
finline_double_align (int); /* { dg-warning "ignoring attribute .aligned \\(4\\). because it conflicts with attribute .aligned \\(32\\)." } */

inline int ATTR ((noreturn))
finline_double_align (int i) { (void)&i; __builtin_abort (); }


/* Exercise variable attributes.  */

extern int ATTR ((common))
decl_common1;                 /* { dg-message "previous declaration here" } */

extern int ATTR ((nocommon))
decl_common1;                 /* { dg-warning "ignoring attribute .nocommon. because it conflicts with attribute .common." } */


extern int ATTR ((nocommon))
decl_common2;                 /* { dg-message "previous declaration here" } */

extern int ATTR ((common))
decl_common2;                 /* { dg-warning "ignoring attribute .common. because it conflicts with attribute .nocommon." } */


extern int ATTR ((common, nocommon))
decl_common3;                 /* { dg-warning "ignoring attribute .nocommon. because it conflicts with attribute .common." } */


extern int ATTR ((common, nocommon))
decl_common4;                 /* { dg-warning "ignoring attribute .nocommon. because it conflicts with attribute .common." } */

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
// DEFAULT-NEXT:     type @type[[TYPE_AlignedAligned:[0-9]+]] AlignedAligned = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_AlignedPacked:[0-9]+]] AlignedPacked = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_PackedAligned:[0-9]+]] PackedAligned = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_AlignedMemberPacked:[0-9]+]] AlignedMemberPacked = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_PackedMemberAligned:[0-9]+]] PackedMemberAligned = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_PackedPacked:[0-9]+]] PackedPacked = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0]];
// DEFAULT-NEXT:     extern %[[VALUE_decl_common1:[0-9]+]] decl_common1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_decl_common2:[0-9]+]] decl_common2: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_decl_common3:[0-9]+]] decl_common3: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_decl_common4:[0-9]+]] decl_common4: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_faligned8_1:[0-9]+]] @faligned8_1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_faligned8_2:[0-9]+]] @faligned8_2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_falways_inline1:[0-9]+]] @falways_inline1() -> void [linkage=external] [inline=always] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fnoinline1:[0-9]+]] @fnoinline1() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fnoret1:[0-9]+]] @fnoret1() -> i32 [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_call_noret1:[0-9]+]] @call_noret1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_fnoret1]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fnoret2:[0-9]+]] @fnoret2() -> i32 [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_call_noret2:[0-9]+]] @call_noret2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_fnoret2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fnorety6:[0-9]+]] @fnorety6() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret6:[0-9]+]] @fnoret6() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_alloc_align1:[0-9]+]] @fnoret_alloc_align1(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_alloc_align2:[0-9]+]] @fnoret_alloc_align2(%[[VALUE1:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_alloc_align3:[0-9]+]] @fnoret_alloc_align3(%[[VALUE2:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_falloc_align_noret1:[0-9]+]] @falloc_align_noret1(%[[VALUE3:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_falloc_align_noret2:[0-9]+]] @falloc_align_noret2(%[[VALUE4:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_falloc_align_noret3:[0-9]+]] @falloc_align_noret3(%[[VALUE5:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_alloc_size1:[0-9]+]] @fnoret_alloc_size1(%[[VALUE6:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_alloc_size2:[0-9]+]] @fnoret_alloc_size2(%[[VALUE7:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_alloc_size3:[0-9]+]] @fnoret_alloc_size3(%[[VALUE8:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_falloc_size_noret1:[0-9]+]] @falloc_size_noret1(%[[VALUE9:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_falloc_size_noret2:[0-9]+]] @falloc_size_noret2(%[[VALUE10:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_falloc_size_noret3:[0-9]+]] @falloc_size_noret3(%[[VALUE11:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_const1:[0-9]+]] @fnoret_const1(%[[VALUE12:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_const2:[0-9]+]] @fnoret_const2(%[[VALUE13:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_const3:[0-9]+]] @fnoret_const3(%[[VALUE14:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fconst_noret1:[0-9]+]] @fconst_noret1(%[[VALUE15:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fconst_noret2:[0-9]+]] @fconst_noret2(%[[VALUE16:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fconst_noret3:[0-9]+]] @fconst_noret3(%[[VALUE17:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_malloc1:[0-9]+]] @fnoret_malloc1(%[[VALUE18:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_malloc2:[0-9]+]] @fnoret_malloc2(%[[VALUE19:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_malloc3:[0-9]+]] @fnoret_malloc3(%[[VALUE20:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fmalloc_noret1:[0-9]+]] @fmalloc_noret1(%[[VALUE21:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fmalloc_noret2:[0-9]+]] @fmalloc_noret2(%[[VALUE22:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fmalloc_noret3:[0-9]+]] @fmalloc_noret3(%[[VALUE23:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_pure1:[0-9]+]] @fnoret_pure1(%[[VALUE24:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_pure2:[0-9]+]] @fnoret_pure2(%[[VALUE25:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_pure3:[0-9]+]] @fnoret_pure3(%[[VALUE26:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_fpure_noret1:[0-9]+]] @fpure_noret1(%[[VALUE27:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_fpure_noret2:[0-9]+]] @fpure_noret2(%[[VALUE28:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_fpure_noret3:[0-9]+]] @fpure_noret3(%[[VALUE29:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_returns_twice1:[0-9]+]] @fnoret_returns_twice1(%[[VALUE30:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_returns_twice2:[0-9]+]] @fnoret_returns_twice2(%[[VALUE31:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fnoret_returns_twice3:[0-9]+]] @fnoret_returns_twice3(%[[VALUE32:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_freturns_twice_noret1:[0-9]+]] @freturns_twice_noret1(%[[VALUE33:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_freturns_twice_noret2:[0-9]+]] @freturns_twice_noret2(%[[VALUE34:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_freturns_twice_noret3:[0-9]+]] @freturns_twice_noret3(%[[VALUE35:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_finline_cold_noreturn:[0-9]+]] @finline_cold_noreturn(%[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [inline=always] [definition=inline_only] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         addr_of<ptr<i32>>(%[[VALUE_i]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort:[0-9]+]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_finline_hot_noret_align:[0-9]+]] @finline_hot_noret_align(%[[VALUE_i_2:[0-9]+]] i: i32) -> i32 [linkage=external] [inline=always] [definition=inline_only] [noreturn] [memory=none] [fallthrough=ub] {
// DEFAULT-NEXT:         addr_of<ptr<i32>>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_finline_align:[0-9]+]] @finline_align(%[[VALUE_i_3:[0-9]+]] i: i32) -> i32 [linkage=external] [inline=hint] [definition=inline_only] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         addr_of<ptr<i32>>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_finline_double_align:[0-9]+]] @finline_double_align(%[[VALUE_i_4:[0-9]+]] i: i32) -> i32 [linkage=external] [inline=hint] [definition=inline_only] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         addr_of<ptr<i32>>(%[[VALUE_i_4]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
