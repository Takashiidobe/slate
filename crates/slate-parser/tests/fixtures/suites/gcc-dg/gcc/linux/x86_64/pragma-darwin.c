/* Darwin (Mac OS X) pragma exercises.  */

/* { dg-do run { target *-*-darwin* } } */
/* { dg-options "-O -Wunused" } */

/* The mark pragma is to help decorate IDEs.  */

extern void abort(void);

#pragma mark hey hey ho

/* The options pragma used to do a lot, now it's only for emulating
   m68k alignment rules in structs.  */

#pragma options 23  /* { dg-warning "malformed '#pragma options'" } */
#pragma options align  /* { dg-warning "malformed '#pragma options'" } */
#pragma options align natural /* { dg-warning "malformed '#pragma options'" } */
#pragma options align=45 /* { dg-warning "malformed '#pragma options'" } */
#pragma options align=foo /* { dg-warning "malformed '#pragma options align" } */

#ifndef __LP64__
#pragma options align=mac68k
struct s1 { short f1; int f2; };
#endif
#pragma options align=power
struct s2 { short f1; int f2; };
#ifndef __LP64__
#pragma options align=mac68k
struct s3 { short f1; int f2; };
#endif
#pragma options align=reset
struct s4 { short f1; int f2; };

#pragma options align=natural foo /* { dg-warning "junk at end of '#pragma options'" } */
/* { dg-warning "malformed '#pragma options align={mac68k|power|reset}', ignoring" "ignoring" { target *-*-* } .-1 } */

/* Segment pragmas don't do anything anymore.  */

#pragma segment foo

int
main ()
{
  int x, z;  /* { dg-warning "unused variable 'z'" } */
  #pragma unused (x, y)

#ifndef __LP64__
  if (sizeof (struct s1) != 6)
    abort ();
#endif
  if (sizeof (struct s2) != 8)
    abort ();
#ifndef __LP64__
  if (sizeof (struct s3) != 6)
    abort ();
#endif
  if (sizeof (struct s4) != 8)
    abort ();
  return 0;
}

void
unused_err_test ()
{
  int a, b;
  /* Trying to match on '(' or ')' gives regexp headaches, use . instead.  */
#pragma unused  /* { dg-warning "missing '.' after '#pragma unused" } */
#pragma unused (a  /* { dg-warning "missing '.' after '#pragma unused" } */
#pragma unused (b) foo /* { dg-warning "junk at end of '#pragma unused'" } */
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
// DEFAULT-NEXT:     type @type[[TYPE_s2:[0-9]+]] s2 = struct {
// DEFAULT-NEXT:         field0 f1: i16;
// DEFAULT-NEXT:         field1 f2: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_s4:[0-9]+]] s4 = struct {
// DEFAULT-NEXT:         field0 f1: i16;
// DEFAULT-NEXT:         field1 f2: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_unused_err_test:[0-9]+]] @unused_err_test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
