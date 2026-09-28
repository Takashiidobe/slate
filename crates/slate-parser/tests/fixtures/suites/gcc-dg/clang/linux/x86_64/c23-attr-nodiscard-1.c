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
// DEFAULT-NEXT:     type @type0 s1 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 s2 = struct {
// DEFAULT-NEXT:         field0 b: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type2 s2t = @type1;
// DEFAULT-NEXT:     type @type3 u1 = union {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type4 u2 = union {
// DEFAULT-NEXT:         field0 c: i16;
// DEFAULT-NEXT:         field1 d: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type5 e1 = enum : u32 {
// DEFAULT-NEXT:         %0 E1 = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type6 e2 = enum : u32 {
// DEFAULT-NEXT:         %0 E2 = const<i32>(0);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %19 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @c1() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @c2() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @cs1() -> @type0 [linkage=external] [abi=sysv64() -> coerce<i32>];
// DEFAULT-NEXT:     fn %5 @cs2() -> @type1 [linkage=external] [abi=sysv64() -> coerce<i64>];
// DEFAULT-NEXT:     fn %7 @cs3() -> @type1 [linkage=external] [abi=sysv64() -> coerce<i64>];
// DEFAULT-NEXT:     fn %10 @cu1() -> @type3 [linkage=external] [abi=sysv64() -> coerce<i64>];
// DEFAULT-NEXT:     fn %11 @cu2() -> @type4 [linkage=external] [abi=sysv64() -> coerce<i32>];
// DEFAULT-NEXT:     fn %16 @ce1() -> @type5 [linkage=external];
// DEFAULT-NEXT:     fn %17 @ce2() -> @type6 [linkage=external];
// DEFAULT-NEXT:     fn %18 @ce1a() -> @type5 [linkage=external];
// DEFAULT-NEXT:     fn %20 @v() -> void [linkage=external];
// DEFAULT-NEXT:     fn %21 @ok() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %22 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%1);
// DEFAULT-NEXT:         call<@type0, signature=fn() -> @type0, abi=sysv64() -> coerce<i32>>(%4);
// DEFAULT-NEXT:         call<@type1, signature=fn() -> @type1, abi=sysv64() -> coerce<i64>>(%5);
// DEFAULT-NEXT:         call<@type1, signature=fn() -> @type1, abi=sysv64() -> coerce<i64>>(%7);
// DEFAULT-NEXT:         call<@type3, signature=fn() -> @type3, abi=sysv64() -> coerce<i64>>(%10);
// DEFAULT-NEXT:         call<@type4, signature=fn() -> @type4, abi=sysv64() -> coerce<i32>>(%11);
// DEFAULT-NEXT:         call<@type5, signature=fn() -> @type5>(%16);
// DEFAULT-NEXT:         call<@type6, signature=fn() -> @type6>(%17);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%21);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%21);
// DEFAULT-NEXT:         call<@type0, signature=fn() -> @type0, abi=sysv64() -> coerce<i32>>(%4);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%21);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%21);
// DEFAULT-NEXT:         call<@type3, signature=fn() -> @type3, abi=sysv64() -> coerce<i64>>(%10);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%21);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%21);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%21);
// DEFAULT-NEXT:         call<@type6, signature=fn() -> @type6>(%17);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%21);
// DEFAULT-NEXT:         call<@type3, signature=fn() -> @type3, abi=sysv64() -> coerce<i64>>(%10);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%21);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%20);
// DEFAULT-NEXT:         call<@type5, signature=fn() -> @type5>(conditional<ptr<fn() -> @type5>>(ne<i32>(read<i32>(%19), const<i32>(0)), function_decay<ptr<fn() -> @type5>>(%16), function_decay<ptr<fn() -> @type5>>(%18)));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%0);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%1);
// DEFAULT-NEXT:         call<@type0, signature=fn() -> @type0, abi=sysv64() -> coerce<i32>>(%4);
// DEFAULT-NEXT:         call<@type1, signature=fn() -> @type1, abi=sysv64() -> coerce<i64>>(%5);
// DEFAULT-NEXT:         call<@type1, signature=fn() -> @type1, abi=sysv64() -> coerce<i64>>(%7);
// DEFAULT-NEXT:         call<@type3, signature=fn() -> @type3, abi=sysv64() -> coerce<i64>>(%10);
// DEFAULT-NEXT:         call<@type4, signature=fn() -> @type4, abi=sysv64() -> coerce<i32>>(%11);
// DEFAULT-NEXT:         call<@type5, signature=fn() -> @type5>(%16);
// DEFAULT-NEXT:         call<@type6, signature=fn() -> @type6>(%17);
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%21);
// DEFAULT-NEXT:         call<@type3, signature=fn() -> @type3, abi=sysv64() -> coerce<i64>>(%10);
// DEFAULT-NEXT:         call<@type5, signature=fn() -> @type5>(conditional<ptr<fn() -> @type5>>(ne<i32>(read<i32>(%19), const<i32>(0)), function_decay<ptr<fn() -> @type5>>(%16), function_decay<ptr<fn() -> @type5>>(%18)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
