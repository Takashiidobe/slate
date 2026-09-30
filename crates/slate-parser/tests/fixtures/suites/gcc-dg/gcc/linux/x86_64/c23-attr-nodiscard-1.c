/* Test C23 deprecated attribute: valid uses.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

[[nodiscard]] int c1 (void); /* { dg-message "declared here" } */
[[__nodiscard__ ("some reason")]] int c2 (void); /* { dg-message "declared here" } */

struct [[nodiscard ("struct reason")]] s1 { int a; };
struct [[__nodiscard__]] s2 { long b; };
struct s1 cs1 (void); /* { dg-message "declared here" } */
struct s2 cs2 (void); /* { dg-message "declared here" } */
typedef struct s2 s2t;
s2t cs3 (void); /* { dg-message "declared here" } */

union [[nodiscard]] u1 { int a; long b; };
union [[nodiscard ("union reason")]] u2 { short c; float d; };
union u1 cu1 (void); /* { dg-message "declared here" } */
union u2 cu2 (void); /* { dg-message "declared here" } */

enum [[nodiscard]] e1 { E1 };
enum [[nodiscard ("enum reason")]] e2 { E2 };
enum e1 ce1 (void); /* { dg-message "declared here" } */
enum e2 ce2 (void); /* { dg-message "declared here" } */
enum e1 ce1a (void);
int i;

[[nodiscard]] void v (void); /* { dg-warning "void return type" } */

int ok (void);

void
f (void)
{
  c1 (); /* { dg-warning "ignoring return value" } */
  c2 (); /* { dg-warning "some reason" } */
  cs1 (); /* { dg-warning "struct reason" } */
  cs2 (); /* { dg-warning "ignoring return value of type" } */
  cs3 (); /* { dg-warning "ignoring return value of type" } */
  cu1 (); /* { dg-warning "ignoring return value of type" } */
  cu2 (); /* { dg-warning "union reason" } */
  ce1 (); /* { dg-warning "ignoring return value of type" } */
  ce2 (); /* { dg-warning "enum reason" } */
  ok ();
  c1 (), ok (); /* { dg-warning "ignoring return value" } */
  cs1 (), ok (); /* { dg-warning "struct reason" } */
  ok (), cu1 (); /* { dg-warning "ignoring return value" } */
  ok (), (ok (), (ok (), ce2 ())); /* { dg-warning "enum reason" } */
  (ok (), cu1 ()), ok (); /* { dg-warning "ignoring return value" } */
  v ();
  (i ? ce1 : ce1a) (); /* { dg-warning "ignoring return value of type" } */
  (void) c1 ();
  (void) c2 ();
  (void) cs1 ();
  (void) cs2 ();
  (void) cs3 ();
  (void) cu1 ();
  (void) cu2 ();
  (void) ce1 ();
  (void) ce2 ();
  (void) (ok (), cu1 ());
  (void) (i ? ce1 : ce1a) ();
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
// DEFAULT-NEXT:     type @type[[TYPE_s1:[0-9]+]] s1 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_s2:[0-9]+]] s2 = struct {
// DEFAULT-NEXT:         field0 b: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_s2t:[0-9]+]] s2t = @type[[TYPE_s2]];
// DEFAULT-NEXT:     type @type[[TYPE_u1:[0-9]+]] u1 = union {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_u2:[0-9]+]] u2 = union {
// DEFAULT-NEXT:         field0 c: i16;
// DEFAULT-NEXT:         field1 d: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_e1:[0-9]+]] e1 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_E1:[0-9]+]] E1 = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_e2:[0-9]+]] e2 = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_E1]] E2 = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_E1]] @c1() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_c2:[0-9]+]] @c2() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cs1:[0-9]+]] @cs1() -> @type[[TYPE_s1]] [linkage=external] [abi=sysv64() -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_cs2:[0-9]+]] @cs2() -> @type[[TYPE_s2]] [linkage=external] [abi=sysv64() -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_cs3:[0-9]+]] @cs3() -> @type[[TYPE_s2]] [linkage=external] [abi=sysv64() -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_cu1:[0-9]+]] @cu1() -> @type[[TYPE_u1]] [linkage=external] [abi=sysv64() -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_cu2:[0-9]+]] @cu2() -> @type[[TYPE_u2]] [linkage=external] [abi=sysv64() -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_ce1:[0-9]+]] @ce1() -> @type[[TYPE_e1]] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ce2:[0-9]+]] @ce2() -> @type[[TYPE_e2]] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ce1a:[0-9]+]] @ce1a() -> @type[[TYPE_e1]] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_v:[0-9]+]] @v() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ok:[0-9]+]] @ok() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_E1]]);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_c2]]);
// DEFAULT-NEXT:         call<@type[[TYPE_s1]], signature=fn() -> @type[[TYPE_s1]], abi=sysv64() -> native_c>(%[[VALUE_cs1]]);
// DEFAULT-NEXT:         call<@type[[TYPE_s2]], signature=fn() -> @type[[TYPE_s2]], abi=sysv64() -> native_c>(%[[VALUE_cs2]]);
// DEFAULT-NEXT:         call<@type[[TYPE_s2]], signature=fn() -> @type[[TYPE_s2]], abi=sysv64() -> native_c>(%[[VALUE_cs3]]);
// DEFAULT-NEXT:         call<@type[[TYPE_u1]], signature=fn() -> @type[[TYPE_u1]], abi=sysv64() -> native_c>(%[[VALUE_cu1]]);
// DEFAULT-NEXT:         call<@type[[TYPE_u2]], signature=fn() -> @type[[TYPE_u2]], abi=sysv64() -> native_c>(%[[VALUE_cu2]]);
// DEFAULT-NEXT:         call<@type[[TYPE_e1]], signature=fn() -> @type[[TYPE_e1]]>(%[[VALUE_ce1]]);
// DEFAULT-NEXT:         call<@type[[TYPE_e2]], signature=fn() -> @type[[TYPE_e2]]>(%[[VALUE_ce2]]);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_ok]]);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_E1]]);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_ok]]);
// DEFAULT-NEXT:         call<@type[[TYPE_s1]], signature=fn() -> @type[[TYPE_s1]], abi=sysv64() -> native_c>(%[[VALUE_cs1]]);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_ok]]);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_ok]]);
// DEFAULT-NEXT:         call<@type[[TYPE_u1]], signature=fn() -> @type[[TYPE_u1]], abi=sysv64() -> native_c>(%[[VALUE_cu1]]);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_ok]]);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_ok]]);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_ok]]);
// DEFAULT-NEXT:         call<@type[[TYPE_e2]], signature=fn() -> @type[[TYPE_e2]]>(%[[VALUE_ce2]]);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_ok]]);
// DEFAULT-NEXT:         call<@type[[TYPE_u1]], signature=fn() -> @type[[TYPE_u1]], abi=sysv64() -> native_c>(%[[VALUE_cu1]]);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_ok]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_v]]);
// DEFAULT-NEXT:         call<@type[[TYPE_e1]], signature=fn() -> @type[[TYPE_e1]]>(conditional<ptr<fn() -> @type[[TYPE_e1]]>>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), function_decay<ptr<fn() -> @type[[TYPE_e1]]>>(%[[VALUE_ce1]]), function_decay<ptr<fn() -> @type[[TYPE_e1]]>>(%[[VALUE_ce1a]])));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_E1]]);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_c2]]);
// DEFAULT-NEXT:         call<@type[[TYPE_s1]], signature=fn() -> @type[[TYPE_s1]], abi=sysv64() -> native_c>(%[[VALUE_cs1]]);
// DEFAULT-NEXT:         call<@type[[TYPE_s2]], signature=fn() -> @type[[TYPE_s2]], abi=sysv64() -> native_c>(%[[VALUE_cs2]]);
// DEFAULT-NEXT:         call<@type[[TYPE_s2]], signature=fn() -> @type[[TYPE_s2]], abi=sysv64() -> native_c>(%[[VALUE_cs3]]);
// DEFAULT-NEXT:         call<@type[[TYPE_u1]], signature=fn() -> @type[[TYPE_u1]], abi=sysv64() -> native_c>(%[[VALUE_cu1]]);
// DEFAULT-NEXT:         call<@type[[TYPE_u2]], signature=fn() -> @type[[TYPE_u2]], abi=sysv64() -> native_c>(%[[VALUE_cu2]]);
// DEFAULT-NEXT:         call<@type[[TYPE_e1]], signature=fn() -> @type[[TYPE_e1]]>(%[[VALUE_ce1]]);
// DEFAULT-NEXT:         call<@type[[TYPE_e2]], signature=fn() -> @type[[TYPE_e2]]>(%[[VALUE_ce2]]);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%[[VALUE_ok]]);
// DEFAULT-NEXT:         call<@type[[TYPE_u1]], signature=fn() -> @type[[TYPE_u1]], abi=sysv64() -> native_c>(%[[VALUE_cu1]]);
// DEFAULT-NEXT:         call<@type[[TYPE_e1]], signature=fn() -> @type[[TYPE_e1]]>(conditional<ptr<fn() -> @type[[TYPE_e1]]>>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), function_decay<ptr<fn() -> @type[[TYPE_e1]]>>(%[[VALUE_ce1]]), function_decay<ptr<fn() -> @type[[TYPE_e1]]>>(%[[VALUE_ce1a]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
