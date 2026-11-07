/* { dg-do compile } */
/* { dg-options "-Wno-old-style-definition" } */

#define CONCAT_(A,B) A ## B
#define CONCAT(A,B) CONCAT_(A,B)

#define ATTR __attribute__((AT))
#define ATSYM(suf) CONCAT (AT, CONCAT_ (_, suf))

#define AT noinline

typedef int ATSYM(type) ATTR; /* { dg-warning "attribute ignored" } */

typedef int (*ATSYM(fntype))(void) ATTR; /* { dg-warning "attribute ignored" } */

struct ATSYM(struct) {
  char dummy ATTR; /* { dg-warning "attribute ignored" } */
} ATTR; /* { dg-warning "does not apply to types" } */

int ATSYM(var) ATTR;  /* { dg-warning "attribute ignored" } */

int ATSYM(fn_knrarg) (arg)
  int arg ATTR; /* { dg-warning "attribute ignored" } */
{ return 0; }

int ATSYM(fn_isoarg) (int arg ATTR) { return 0; } /* { dg-warning "attribute ignored" } */

int ATSYM(fn_vars) (void) {
  static int svar ATTR; /* { dg-warning "attribute ignored" } */
  auto int lvar ATTR; /* { dg-warning "attribute ignored" } */
  return 0;
}


#undef AT
#define AT used

typedef int ATSYM(type) ATTR; /* used attribute is no longer
				 ignored.  */

typedef int (*ATSYM(fntype))(void) ATTR; /* used attribute is no
					    longer ignored.  */

struct ATSYM(struct) {
  char dummy ATTR; /* { dg-warning "attribute ignored" } */
} ATTR; /* { dg-warning "does not apply to types" } */

int ATSYM(var) ATTR;

int ATSYM(fn_knrarg) (arg)
  int arg ATTR; /* { dg-warning "attribute ignored" } */
{ return 0; }

int ATSYM(fn_isoarg) (int arg ATTR) { return 0; } /* { dg-warning "attribute ignored" } */

int ATSYM(fn_vars) (void) {
  static int svar ATTR;
  auto int lvar ATTR; /* { dg-warning "attribute ignored" } */
  return 0;
}


/* PR 28287 */
/* These are invalid on all targets.  Applying to PARM_ or FIELD_DECL
   also caused a tree checking ice on targets that support weak,  */
#undef AT
#define AT weak

typedef int ATSYM(type) ATTR; /* { dg-warning "attribute ignored" } */

typedef int (*ATSYM(fntype))(void) ATTR; /* { dg-warning "attribute ignored" } */

struct ATSYM(struct) {
  char dummy ATTR; /* { dg-warning "attribute ignored" } */
};

int ATSYM(fn_knrarg) (arg)
  int arg ATTR; /* { dg-warning "attribute ignored" } */
{ return 0; }

int ATSYM(fn_isoarg) (int arg ATTR) { return 0; } /* { dg-warning "attribute ignored" } */


/* PR target/28648  */
/* These are invalid on all targets.  Applying to PARM_ or FIELD_DECL
   also caused a tree checking ice on targets that support dllimport.  */
#undef AT
#define AT dllimport

typedef int ATSYM(type) ATTR;  /* { dg-warning "attribute |directive ignored" } */

typedef int (*ATSYM(fntype))(void) ATTR;   /* { dg-warning "attribute |directive ignored" } */

struct ATSYM(struct) {
  char dummy ATTR;  /* { dg-warning "attribute |directive ignored" } */
};

int ATSYM(fn_knrarg) (arg)
  int arg ATTR;  /* { dg-warning "attribute |directive ignored" } */
{ return 0; }

int ATSYM(fn_isoarg) (int arg ATTR) { return 0; }  /* { dg-warning "attribute |directive ignored" } */

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
// DEFAULT-NEXT:     type @type[[TYPE_noinline_type:[0-9]+]] noinline_type = i32;
// DEFAULT-NEXT:     type @type[[TYPE_noinline_fntype:[0-9]+]] noinline_fntype = ptr<fn() -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE_noinline_struct:[0-9]+]] noinline_struct = struct {
// DEFAULT-NEXT:         field0 dummy: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_used_type:[0-9]+]] used_type = i32;
// DEFAULT-NEXT:     type @type[[TYPE_used_fntype:[0-9]+]] used_fntype = ptr<fn() -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE_used_struct:[0-9]+]] used_struct = struct {
// DEFAULT-NEXT:         field0 dummy: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_weak_type:[0-9]+]] weak_type = i32;
// DEFAULT-NEXT:     type @type[[TYPE_weak_fntype:[0-9]+]] weak_fntype = ptr<fn() -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE_weak_struct:[0-9]+]] weak_struct = struct {
// DEFAULT-NEXT:         field0 dummy: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_dllimport_type:[0-9]+]] dllimport_type = i32;
// DEFAULT-NEXT:     type @type[[TYPE_dllimport_fntype:[0-9]+]] dllimport_fntype = ptr<fn() -> i32>;
// DEFAULT-NEXT:     type @type[[TYPE_dllimport_struct:[0-9]+]] dllimport_struct = struct {
// DEFAULT-NEXT:         field0 dummy: i8;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_noinline_var:[0-9]+]] noinline_var: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_svar:[0-9]+]] svar: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_used_var:[0-9]+]] used_var: i32 [storage=static] [linkage=external] [used];
// DEFAULT-NEXT:     global %[[VALUE_svar_2:[0-9]+]] svar: i32 [storage=static] [linkage=internal] [used];
// DEFAULT-NEXT:     fn %[[VALUE_noinline_fn_knrarg:[0-9]+]] @noinline_fn_knrarg(%[[VALUE_arg:[0-9]+]] arg: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_noinline_fn_isoarg:[0-9]+]] @noinline_fn_isoarg(%[[VALUE_arg_2:[0-9]+]] arg: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_noinline_fn_vars:[0-9]+]] @noinline_fn_vars() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_lvar:[0-9]+]] lvar: i32 [storage=automatic];
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_used_fn_knrarg:[0-9]+]] @used_fn_knrarg(%[[VALUE_arg_3:[0-9]+]] arg: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_used_fn_isoarg:[0-9]+]] @used_fn_isoarg(%[[VALUE_arg_4:[0-9]+]] arg: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_used_fn_vars:[0-9]+]] @used_fn_vars() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_lvar_2:[0-9]+]] lvar: i32 [storage=automatic];
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_weak_fn_knrarg:[0-9]+]] @weak_fn_knrarg(%[[VALUE_arg_5:[0-9]+]] arg: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_weak_fn_isoarg:[0-9]+]] @weak_fn_isoarg(%[[VALUE_arg_6:[0-9]+]] arg: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dllimport_fn_knrarg:[0-9]+]] @dllimport_fn_knrarg(%[[VALUE_arg_7:[0-9]+]] arg: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dllimport_fn_isoarg:[0-9]+]] @dllimport_fn_isoarg(%[[VALUE_arg_8:[0-9]+]] arg: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
