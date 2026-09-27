/* Test for non-lvalue arrays: test that C90 does not allow them in
   conditional expressions, while in C99 they decay and are
   allowed.  */

/* Origin: Joseph Myers <jsm@polyomino.org.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

struct s { char c[1]; };
struct s a, b, c;
int d;
int e;

void
bar (void)
{
  /* In C90, the non-lvalue arrays do not decay to pointers, and
     6.3.15 does not permit conditional expressions between arrays.
     In C99, they decay to pointers.  */
  (e ? (d ? b : c).c : (e ? b : c).c);
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     type @type0 s = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 1>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %1 a: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %6 @bar() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         conditional<ptr<i8>>(ne<i32>(read<i32>(%5), const<i32>(0)), array_decay<ptr<i8>, length=Some(1)>(field0(temporary %7 = conditional<@type0>(ne<i32>(read<i32>(%4), const<i32>(0)), read<@type0>(%2), read<@type0>(%3)))), array_decay<ptr<i8>, length=Some(1)>(field0(temporary %8 = conditional<@type0>(ne<i32>(read<i32>(%5), const<i32>(0)), read<@type0>(%2), read<@type0>(%3)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
