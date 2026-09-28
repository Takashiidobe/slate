/* PR target/42564 */
/* This used to ICE on the SPARC because of an unrecognized TLS pattern.  */

/* { dg-do compile } */
/* { dg-options "-O -fPIC" } */
/* { dg-require-effective-target tls } */
/* { dg-require-effective-target fpic } */

extern void *memset(void *s, int c, __SIZE_TYPE__ n);

struct S1 { int i; };

struct S2
{
  int ver;
  struct S1 s;
};

static __thread struct S2 m;

void init(void)
{
  memset(&m.s, 0, sizeof(m.s));
}

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
// DEFAULT-NEXT:     type @type0 S1 = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 S2 = struct {
// DEFAULT-NEXT:         field0 ver: i32;
// DEFAULT-NEXT:         field1 s: @type0;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %6 m: @type1 [storage=thread] [linkage=internal];
// DEFAULT-NEXT:     fn %3 @memset(%8 s: ptr<void>, %9 c: i32, %10 n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @init() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%3, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(field1(%6))), const<i32>(0), const<u64>(4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
