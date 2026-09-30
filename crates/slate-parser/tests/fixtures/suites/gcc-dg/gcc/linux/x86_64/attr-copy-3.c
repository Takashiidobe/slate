/* PR middle-end/81824 - Warn for missing attributes with function aliases
   Exercise attribute copy for variables.
   { dg-do compile }
   { dg-options "-O2 -Wall" } */

#define ATTR(list)   __attribute__ (list)

/* Verify that referencing a symbol with no attributes is accepted
   with no diagnostics.  */

int ref0;

ATTR ((copy (ref0))) long
var0;

/* Verify that referencing a symbol using the address-of and dereferencing
   operators is also accepted with no diagnostics.  */

ATTR ((copy (&ref0))) void* ptr0;
ATTR ((copy (*&ref0))) int arr[1];

/* Verify that referencing a symbol of a different kind than that
   of the one the attribute is applied to is diagnosed.  */

int ref1;                     /* { dg-message "previous declaration here" } */

ATTR ((copy (ref1))) int
ref1;                         /* { dg-warning ".copy. attribute ignored on a redeclaration of the referenced symbol " } */


/* Verify that circular references of the copy variable attribute
   are handled gracefully (i.e., not by getting into an infinite
   recursion) by issuing a diagnostic.  */

char xref1;
ATTR ((copy (xref1))) char
xref1;                        /* { dg-warning ".copy. attribute ignored on a redeclaration of the referenced symbol" } */
ATTR ((copy (xref1))) char
xref1;                        /* { dg-warning ".copy. attribute ignored on a redeclaration of the referenced symbol" } */
ATTR ((copy (xref1), copy (xref1))) char
xref1;                        /* { dg-warning ".copy. attribute ignored on a redeclaration of the referenced symbol" } */


/* Use attribute unused to verify that circular references propagate
   atttibutes as expected (expect no warnings the circular reference
   or for any of the unused symbols).  Also use the address-of operator
   to make sure it doesn't change anything.  */

static ATTR ((unused))        int xref2;
static ATTR ((copy (xref2)))  int xref3;
static ATTR ((copy (&xref3))) int xref4;
static ATTR ((copy (xref4)))  int xref5;
static ATTR ((copy (&xref5))) int xref6;
static ATTR ((copy (xref6)))  int xref7;
static ATTR ((copy (&xref7))) int xref8;
static ATTR ((copy (xref8)))  int xref9;
static ATTR ((copy (&xref9))) int xref2;

/* Verify that attribute exclusions apply.  */

ATTR ((common)) int common_var;
ATTR ((nocommon)) double nocommon_var;

ATTR ((copy (common_var), copy (nocommon_var))) long
common_copy;                  /* { dg-warning "ignoring attribute .nocommon. because it conflicts with attribute .common." } */


/* Verify that attribute deprecated isn't copied.  */

ATTR ((deprecated)) char deprecated_var;

ATTR ((copy (deprecated_var))) int current_var;  /* { dg-warning "\\\[-Wdeprecated-declarations]" } */
ATTR ((copy (current_var))) int current_var_2;

int return_current_vars (void) { return current_var + current_var_2; }

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
// DEFAULT-NEXT:     global %[[VALUE_ref0:[0-9]+]] ref0: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_var0:[0-9]+]] var0: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ptr0:[0-9]+]] ptr0: ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arr:[0-9]+]] arr: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ref1:[0-9]+]] ref1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_xref1:[0-9]+]] xref1: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_xref2:[0-9]+]] xref2: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_xref3:[0-9]+]] xref3: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_xref4:[0-9]+]] xref4: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_xref5:[0-9]+]] xref5: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_xref6:[0-9]+]] xref6: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_xref7:[0-9]+]] xref7: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_xref8:[0-9]+]] xref8: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_xref9:[0-9]+]] xref9: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_common_var:[0-9]+]] common_var: i32 [storage=static] [linkage=external] [common];
// DEFAULT-NEXT:     global %[[VALUE_nocommon_var:[0-9]+]] nocommon_var: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_common_copy:[0-9]+]] common_copy: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_deprecated_var:[0-9]+]] deprecated_var: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_current_var:[0-9]+]] current_var: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_current_var_2:[0-9]+]] current_var_2: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_return_current_vars:[0-9]+]] @return_current_vars() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_current_var]]), read<i32>(%[[VALUE_current_var_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
