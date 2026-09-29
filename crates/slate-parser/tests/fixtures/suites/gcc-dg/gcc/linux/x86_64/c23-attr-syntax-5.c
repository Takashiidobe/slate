/* Test C23 attribute syntax.  Test unknown standard attributes
   diagnosed with a pedwarn.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

[[unknown_attribute]]; /* { dg-error "attribute ignored" } */

[[unknown_attribute]] extern int a; /* { dg-error "attribute ignored" } */
extern int [[unknown_attribute(123)]] a; /* { dg-error "attribute ignored" } */
extern int a [[unknown_attribute("")]]; /* { dg-error "attribute ignored" } */

int f () [[unknown_attribute]]; /* { dg-error "attribute ignored" } */
int f (void) [[unknown_attribute(1)]]; /* { dg-error "attribute ignored" } */
int g ([[unknown_attribute]] int a); /* { dg-error "attribute ignored" } */
int g (int [[unknown_attribute]] a); /* { dg-error "attribute ignored" } */
int g (int a [[unknown_attribute]]); /* { dg-error "attribute ignored" } */
int g ([[unknown_attribute]] int); /* { dg-error "attribute ignored" } */
int g (int [[unknown_attribute]]); /* { dg-error "attribute ignored" } */
int g (int) [[unknown_attribute]]; /* { dg-error "attribute ignored" } */

int *[[unknown_attribute]] p; /* { dg-error "attribute ignored" } */
int b[3] [[unknown_attribute]]; /* { dg-error "attribute ignored" } */

int h (int () [[unknown_attribute]]); /* { dg-error "attribute ignored" } */

struct [[unknown_attribute]] s; /* { dg-error "attribute ignored" } */
union [[unknown_attribute]] u; /* { dg-error "attribute ignored" } */

struct [[unknown_attribute]] s2 { int a; }; /* { dg-error "attribute ignored" } */
union [[unknown_attribute(x)]] u2 { int a; }; /* { dg-error "attribute ignored" } */

struct s3 { [[unknown_attribute]] int a; }; /* { dg-error "attribute ignored" } */
struct s4 { int [[unknown_attribute]] a; }; /* { dg-error "attribute ignored" } */
union u3 { [[unknown_attribute]] int a; }; /* { dg-error "attribute ignored" } */
union u4 { int [[unknown_attribute]] a; }; /* { dg-error "attribute ignored" } */

int z = sizeof (int [[unknown_attribute]]); /* { dg-error "attribute ignored" } */

enum [[unknown_attribute]] { E1 }; /* { dg-error "attribute ignored" } */
enum { E2 [[unknown_attribute]] }; /* { dg-error "attribute ignored" } */
enum { E3 [[unknown_attribute]] = 4 }; /* { dg-error "attribute ignored" } */

void
func (void) [[unknown_attribute]] { /* { dg-error "attribute ignored" } */
  [[unknown_attribute]] int var; /* { dg-error "attribute ignored" } */
  [[unknown_attribute]] { } /* { dg-error "attribute ignored" } */
  [[unknown_attribute(!)]]; /* { dg-error "attribute ignored" } */
  [[unknown_attribute]] var = 1; /* { dg-error "attribute ignored" } */
  [[unknown_attribute]] x: var = 2; /* { dg-error "attribute ignored" } */
  for ([[unknown_attribute]] int zz = 1; zz < 10; zz++) ; /* { dg-error "attribute ignored" } */
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_s2:[0-9]+]] s2 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_u2:[0-9]+]] u2 = union {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_s3:[0-9]+]] s3 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_s4:[0-9]+]] s4 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_u3:[0-9]+]] u3 = union {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_u4:[0-9]+]] u4 = union {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_E1:[0-9]+]] E1 = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_E1]] E2 = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_E1]] E3 = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     extern %[[VALUE_E1]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: array<i32, 3> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_z:[0-9]+]] z: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(%[[VALUE_a:[0-9]+]] a: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_h:[0-9]+]] @h(%[[VALUE0:[0-9]+]] <unnamed>: ptr<fn() -> i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_func:[0-9]+]] @func() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_var:[0-9]+]] var: i32 [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         ;
// DEFAULT-NEXT:         write<i32>(%[[VALUE_var]], const<i32>(1));
// DEFAULT-NEXT:         label %[[VALUE_x:[0-9]+]] x:
// DEFAULT-NEXT:             write<i32>(%[[VALUE_var]], const<i32>(2));
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_zz:[0-9]+]] zz: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_zz]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_zz]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_zz]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
