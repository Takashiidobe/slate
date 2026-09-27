/* PR middle-end/81824 - Warn for missing attributes with function aliases
   Exercise attribute copy for functions.
   { dg-do compile }
   { dg-require-alias "" }
   { dg-options "-O2 -Wall -Wno-array-bounds" } */

#define Assert(expr)   typedef char AssertExpr[2 * !!(expr) - 1]

#define ATTR(list)   __attribute__ (list)

/* Verify that referencing a symbol with no attributes is accepted
   with no diagnostics.  */

void ref0 (void);

ATTR ((copy (ref0))) void
f0 (void);

/* Verify that referencing a symbol using the address-of and dereferencing
   operators is also accepted with no diagnostics.  */

ATTR ((copy (&ref0))) void f1 (void);
ATTR ((copy (*ref0))) void f2 (void);

/* Verify that referencing a symbol of a different kind than that
   of the one the attribute is applied to is diagnosed.  */

int v0;                       /* { dg-message "symbol .v0. referenced by .f3. declared here" } */

ATTR ((copy (v0))) void
f3 (void);                    /* { dg-warning ".copy. attribute ignored on a declaration of a different kind than referenced symbol" } */

void f4 (void);              /* { dg-message "symbol .f4. referenced by .v1. declared here" } */

ATTR ((copy (f4))) int
v1;                           /* { dg-warning ".copy. attribute ignored on a declaration of a different kind than referenced symbol" } */


ATTR ((copy (v0 + 1)))
void f5 (void);               /* { dg-warning ".copy. attribute ignored on a declaration of a different kind than referenced symbol" } */

void f6 (void);

ATTR ((copy (f6 - 1)))
int v1;                       /* { dg-warning ".copy. attribute ignored on a declaration of a different kind than referenced symbol" } */



/* Verify that circular references of the copy function attribute
   are handled gracefully (i.e., not by getting into an infinite
   recursion) by issuing a diagnostic.  */

void xref1 (void);            /* { dg-message "previous declaration here" } */
ATTR ((copy (xref1))) void
xref1 (void);                 /* { dg-warning ".copy. attribute ignored on a redeclaration of the referenced symbol" } */
ATTR ((copy (xref1))) void
xref1 (void);                 /* { dg-warning ".copy. attribute ignored on a redeclaration of the referenced symbol" } */
ATTR ((copy (xref1), copy (xref1))) void
xref1 (void);                 /* { dg-warning ".copy. attribute ignored on a redeclaration of the referenced symbol" } */


/* Use attribute noreturn to verify that circular references propagate
   atttibutes as expected, and unlike in the self-referential instances
   above, without a warning.  Also use the address-of operator to make
   sure it doesn't change anything.  */

ATTR ((noreturn))      void xref2 (void);
ATTR ((copy (xref2)))  void xref3 (void);
ATTR ((copy (&xref3))) void xref4 (void);
ATTR ((copy (xref4)))  void xref5 (void);
ATTR ((copy (&xref5))) void xref6 (void);
ATTR ((copy (xref6)))  void xref7 (void);
ATTR ((copy (&xref7))) void xref8 (void);
ATTR ((copy (xref8)))  void xref9 (void);
ATTR ((copy (&xref9))) void xref2 (void);

int call_ref2 (void) { xref2 (); }
int call_ref3 (void) { xref3 (); }
int call_ref4 (void) { xref4 (); }
int call_ref5 (void) { xref5 (); }
int call_ref6 (void) { xref6 (); }
int call_ref7 (void) { xref7 (); }
int call_ref8 (void) { xref8 (); }
int call_ref9 (void) { xref9 (); }


/* Verify that copying attributes from multiple symbols into one works
   as expected.  */

ATTR ((malloc)) void*
xref10 (void);

ATTR ((alloc_size (1)))
void* xref11 (int);

ATTR ((copy (xref10), copy (xref11)))
void* xref12 (int);

void* call_xref12 (void)
{
  void *p = xref12 (3);
  __builtin___strcpy_chk (p, "123", __builtin_object_size (p, 0));   /* { dg-warning "\\\[-Warray-bounds|-Wstringop-overflow" } */
  return p;
}


/* Verify that attribute exclusions apply.  */

ATTR ((const)) int
fconst (void);

ATTR ((pure)) int
fpure (void);                 /* { dg-message "previous declaration here" } */

ATTR ((copy (fconst), copy (fpure))) int
fconst_pure (void);           /* { dg-warning "ignoring attribute .pure. because it conflicts with attribute .const." } */


/* Also verify that the note in the exclusion warning points to
   the declaration from which the conflicting attribute is copied.
   The wording in the note could be improved but it's the same as
   in ordinary exclusions so making it different between the two
   would take some API changes.  */

ATTR ((const)) int
gconst (void);                /* { dg-message "previous declaration here" } */

ATTR ((pure, copy (gconst))) int
gpure_const (void);           /* { dg-warning "ignoring attribute .const. because it conflicts with attribute .pure." } */


/* Verify that attribute deprecated isn't copied (but referencing
   the deprecated declaration still triggers a warning).  */

ATTR ((deprecated)) void
fdeprecated (void);           /* { dg-message "declared here" } */

/* Unlike in most other instance the warning below is on the line
   with the copy attribute that references the deprecated function.  */
ATTR ((copy (fdeprecated)))   /* { dg-warning "\\\[-Wdeprecated-declarations]" } */
int fcurrent (void);

ATTR ((copy (fcurrent))) int
fcurrent2 (void);

int call_fcurrent (void) { return fcurrent () + fcurrent2 (); }


/* Verify that attributes are copied on a declaration using __typeof__
   and that -Wmissing-attributes is not issued.  */

ATTR ((cold)) int
target_cold (void)
{ return 0; }

__typeof__ (target_cold) ATTR ((copy (target_cold), alias ("target_cold")))
alias_cold;                   /* { dg-bogus "\\\[-Wmissing-attributes]." } */


/* Verify that attribute alias is not copied.  This also indirectly
   verifies that attribute copy itself isn't copied.  */

ATTR ((noreturn)) void fnoret (void) { __builtin_abort (); }
ATTR ((alias ("fnoret"), copy (fnoret))) void fnoret_alias (void);
ATTR ((copy (fnoret_alias))) void fnoret2 (void) { __builtin_exit (1); }

/* Expect no warning below.  */
int call_noret (void) { fnoret2 (); }


/* Verify that attribute nonnull (which is part of a function type,
   even when it's specified on a function declaration) is copied to
   the alias from its target.  Expect no warning about the alias
   specifying less restrictive attributes than its target, but do
   verify that passing a null to the alias triggers -Wnonnull.  */

ATTR ((nonnull))
void* ftarget_nonnull (void *p) { return p; }

ATTR ((alias ("ftarget_nonnull"), copy (ftarget_nonnull)))
void* falias_nonnull (void*);

void call_falias_nonnull (void)
{
  falias_nonnull (0);         /* { dg-warning "-Wnonnull" } */
}

/* Same as above but for malloc.  Also verify that the attribute
   on the alias is used by -Wstringop-overflow.  */

ATTR ((malloc))
void* ftarget_malloc (void) { return __builtin_malloc (1); }

ATTR ((alias ("ftarget_malloc"), copy (ftarget_malloc)))
void* falias_malloc (void);

void* call_falias_malloc (void)
{
  char *p = falias_malloc ();
  __builtin___strcpy_chk (p, "123", __builtin_object_size (p, 0));   /* { dg-warning "\\\[-Warray-bounds|-Wstringop-overflow" } */
  return p;
}

/* Same as above but for nothrow.  */

ATTR ((nothrow))
void ftarget_nothrow (void) { }

ATTR ((alias ("ftarget_nothrow"), copy (ftarget_nothrow)))
void falias_nothrow (void);

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     global %4 v0: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 v1: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %63 .str63: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %73 .str73: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([49, 50, 51, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @ref0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @f0() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @f2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @f3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @f4() -> void [linkage=external];
// DEFAULT-NEXT:     fn %8 @f5() -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @f6() -> void [linkage=external];
// DEFAULT-NEXT:     fn %10 @xref1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %11 @xref2() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @xref3() -> void [linkage=external];
// DEFAULT-NEXT:     fn %13 @xref4() -> void [linkage=external];
// DEFAULT-NEXT:     fn %14 @xref5() -> void [linkage=external];
// DEFAULT-NEXT:     fn %15 @xref6() -> void [linkage=external];
// DEFAULT-NEXT:     fn %16 @xref7() -> void [linkage=external];
// DEFAULT-NEXT:     fn %17 @xref8() -> void [linkage=external];
// DEFAULT-NEXT:     fn %18 @xref9() -> void [linkage=external];
// DEFAULT-NEXT:     fn %19 @call_ref2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @call_ref3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @call_ref4() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @call_ref5() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @call_ref6() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @call_ref7() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @call_ref8() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @call_ref9() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%18);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @xref10() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %28 @xref11(%57 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %29 @xref12(%58 <unnamed>: i32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %62 @__builtin___strcpy_chk(%59 <unnamed>: ptr<i8>, %60 <unnamed>: ptr<const i8>, %61 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %66 @__builtin_object_size(%64 <unnamed>: ptr<const void>, %65 <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %30 @call_xref12() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %31 p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(i32) -> ptr<void>>(%29, const<i32>(3));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%62, pointer_cast<ptr<i8>, reason=arg>(read<ptr<void>>(%31)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%63)), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%66, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%31)), const<i32>(0)));
// DEFAULT-NEXT:         return read<ptr<void>>(%31);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @fconst() -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %33 @fpure() -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %34 @fconst_pure() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %35 @gconst() -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %36 @gpure_const() -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %37 @fdeprecated() -> void [linkage=external];
// DEFAULT-NEXT:     fn %38 @fcurrent() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %39 @fcurrent2() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %40 @call_fcurrent() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%38), call<i32, signature=fn() -> i32>(%39));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @target_cold() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @alias_cold() -> i32 [linkage=external] [alias="target_cold"];
// DEFAULT-NEXT:     fn %67 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %43 @fnoret() -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%67);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @fnoret_alias() -> void [linkage=external] [alias="fnoret"];
// DEFAULT-NEXT:     fn %69 @exit(%68 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %45 @fnoret2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%69, const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @call_noret() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%45);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @ftarget_nonnull(%48 p: ptr<void>) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<void>>(%48);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @falias_nonnull(%70 <unnamed>: ptr<void>) -> ptr<void> [linkage=external] [alias="ftarget_nonnull"];
// DEFAULT-NEXT:     fn %50 @call_falias_nonnull() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>) -> ptr<void>>(%49, null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %72 @__builtin_malloc(%71 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %51 @ftarget_malloc() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(u64) -> ptr<void>>(%72, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @falias_malloc() -> ptr<void> [linkage=external] [alias="ftarget_malloc"];
// DEFAULT-NEXT:     fn %53 @call_falias_malloc() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %54 p: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn() -> ptr<void>>(%52));
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%62, read<ptr<i8>>(%54), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%73)), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%66, pointer_cast<ptr<const void>, reason=arg>(read<ptr<i8>>(%54)), const<i32>(0)));
// DEFAULT-NEXT:         return pointer_cast<ptr<void>, reason=return>(read<ptr<i8>>(%54));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %55 @ftarget_nothrow() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @falias_nothrow() -> void [linkage=external] [alias="ftarget_nothrow"];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
