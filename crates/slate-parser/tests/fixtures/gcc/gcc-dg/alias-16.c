/* { dg-do compile } */
/* { dg-options "-Wstrict-aliasing=1 -fstrict-aliasing" } */

struct incomplete;
struct s1 { int i; };
struct s2 { double d; };

void
f (int *i, double *d, struct s1 *s1, struct s2 *s2, char *c)
{
  (char *) i;
  (char *) d;
  (char *) s1;
  (char *) s2;
  (char *) c;

  (int *) i;
  (int *) d; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (int *) s1; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (int *) s2; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (int *) c;

  (double *) i; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (double *) d;
  (double *) s1; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (double *) s2; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (double *) c;

  (struct incomplete *) i; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (struct incomplete *) d; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (struct incomplete *) s1; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (struct incomplete *) s2; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (struct incomplete *) c; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */

  (struct s1 *) i; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (struct s1 *) d; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (struct s1 *) s1;
  (struct s1 *) s2; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (struct s1 *) c;

  (struct s2 *) i; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (struct s2 *) d; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (struct s2 *) s1; /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  (struct s2 *) s2;
  (struct s2 *) c;
}

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
// DEFAULT-NEXT:     type @type0 incomplete = struct incomplete;
// DEFAULT-NEXT:     type @type1 s1 = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type2 s2 = struct {
// DEFAULT-NEXT:         field0 d: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %3 @f(%4 i: ptr<i32>, %5 d: ptr<f64>, %6 s1: ptr<@type1>, %7 s2: ptr<@type2>, %8 c: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         pointer_cast<ptr<i8>, reason=explicit>(read<ptr<i32>>(%4));
// DEFAULT-NEXT:         pointer_cast<ptr<i8>, reason=explicit>(read<ptr<f64>>(%5));
// DEFAULT-NEXT:         pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type1>>(%6));
// DEFAULT-NEXT:         pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type2>>(%7));
// DEFAULT-NEXT:         read<ptr<i8>>(%8);
// DEFAULT-NEXT:         read<ptr<i32>>(%4);
// DEFAULT-NEXT:         pointer_cast<ptr<i32>, reason=explicit>(read<ptr<f64>>(%5));
// DEFAULT-NEXT:         pointer_cast<ptr<i32>, reason=explicit>(read<ptr<@type1>>(%6));
// DEFAULT-NEXT:         pointer_cast<ptr<i32>, reason=explicit>(read<ptr<@type2>>(%7));
// DEFAULT-NEXT:         pointer_cast<ptr<i32>, reason=explicit>(read<ptr<i8>>(%8));
// DEFAULT-NEXT:         pointer_cast<ptr<f64>, reason=explicit>(read<ptr<i32>>(%4));
// DEFAULT-NEXT:         read<ptr<f64>>(%5);
// DEFAULT-NEXT:         pointer_cast<ptr<f64>, reason=explicit>(read<ptr<@type1>>(%6));
// DEFAULT-NEXT:         pointer_cast<ptr<f64>, reason=explicit>(read<ptr<@type2>>(%7));
// DEFAULT-NEXT:         pointer_cast<ptr<f64>, reason=explicit>(read<ptr<i8>>(%8));
// DEFAULT-NEXT:         pointer_cast<ptr<@type0>, reason=explicit>(read<ptr<i32>>(%4));
// DEFAULT-NEXT:         pointer_cast<ptr<@type0>, reason=explicit>(read<ptr<f64>>(%5));
// DEFAULT-NEXT:         pointer_cast<ptr<@type0>, reason=explicit>(read<ptr<@type1>>(%6));
// DEFAULT-NEXT:         pointer_cast<ptr<@type0>, reason=explicit>(read<ptr<@type2>>(%7));
// DEFAULT-NEXT:         pointer_cast<ptr<@type0>, reason=explicit>(read<ptr<i8>>(%8));
// DEFAULT-NEXT:         pointer_cast<ptr<@type1>, reason=explicit>(read<ptr<i32>>(%4));
// DEFAULT-NEXT:         pointer_cast<ptr<@type1>, reason=explicit>(read<ptr<f64>>(%5));
// DEFAULT-NEXT:         read<ptr<@type1>>(%6);
// DEFAULT-NEXT:         pointer_cast<ptr<@type1>, reason=explicit>(read<ptr<@type2>>(%7));
// DEFAULT-NEXT:         pointer_cast<ptr<@type1>, reason=explicit>(read<ptr<i8>>(%8));
// DEFAULT-NEXT:         pointer_cast<ptr<@type2>, reason=explicit>(read<ptr<i32>>(%4));
// DEFAULT-NEXT:         pointer_cast<ptr<@type2>, reason=explicit>(read<ptr<f64>>(%5));
// DEFAULT-NEXT:         pointer_cast<ptr<@type2>, reason=explicit>(read<ptr<@type1>>(%6));
// DEFAULT-NEXT:         read<ptr<@type2>>(%7);
// DEFAULT-NEXT:         pointer_cast<ptr<@type2>, reason=explicit>(read<ptr<i8>>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
