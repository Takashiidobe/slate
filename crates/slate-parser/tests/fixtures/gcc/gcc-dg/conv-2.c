/* { dg-do compile } */
/* { dg-options "-Wpointer-sign" } */

void f1(long *); /* { dg-message "note: expected '\[^\n'\]*' but argument is of type '\[^\n'\]*'" "note: expected" } */
void f2(unsigned long *); /* { dg-message "note: expected '\[^\n'\]*' but argument is of type '\[^\n'\]*'" "note: expected" } */

int main()
{
  long *lp;
  unsigned long *ulp;
  char *cp;
  unsigned char *ucp;
  signed char *scp;

  ulp = lp;	/* { dg-warning " pointer targets in assignment from 'long int \\*' to 'long unsigned int \\*' differ in signedness" } */
  lp = ulp;	/* { dg-warning " pointer targets in assignment from 'long unsigned int \\*' to 'long int \\*' differ in signedness" } */
  f1(ulp);	/* { dg-warning " differ in signedness" } */
  f2(lp);	/* { dg-warning " differ in signedness" } */

  cp = ucp;	/* { dg-warning " pointer targets in assignment from 'unsigned char \\*' to 'char \\*' differ in signedness" } */
  cp = scp;	/* { dg-warning " pointer targets in assignment from 'signed char \\*' to 'char \\*' differ in signedness" } */
  ucp = scp;	/* { dg-warning " pointer targets in assignment from 'signed char \\*' to 'unsigned char \\*' differ in signedness" } */
  ucp = cp;	/* { dg-warning " pointer targets in assignment from 'char \\*' to 'unsigned char \\*' differ in signedness" } */
  scp = ucp;	/* { dg-warning " pointer targets in assignment from 'unsigned char \\*' to 'signed char \\*' differ in signedness" } */
  scp = cp;	/* { dg-warning " pointer targets in assignment from 'char \\*' to 'signed char \\*' differ in signedness" } */
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
// DEFAULT-NEXT:     fn %0 @f1(%8 <unnamed>: ptr<i64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @f2(%9 <unnamed>: ptr<u64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 lp: ptr<i64> [storage=automatic];
// DEFAULT-NEXT:         let %4 ulp: ptr<u64> [storage=automatic];
// DEFAULT-NEXT:         let %5 cp: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %6 ucp: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %7 scp: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<u64>>(%4, pointer_cast<ptr<u64>, reason=assign>(read<ptr<i64>>(%3)));
// DEFAULT-NEXT:         write<ptr<i64>>(%3, pointer_cast<ptr<i64>, reason=assign>(read<ptr<u64>>(%4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i64>) -> void>(%0, pointer_cast<ptr<i64>, reason=arg>(read<ptr<u64>>(%4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u64>) -> void>(%1, pointer_cast<ptr<u64>, reason=arg>(read<ptr<i64>>(%3)));
// DEFAULT-NEXT:         write<ptr<i8>>(%5, pointer_cast<ptr<i8>, reason=assign>(read<ptr<u8>>(%6)));
// DEFAULT-NEXT:         write<ptr<i8>>(%5, pointer_cast<ptr<i8>, reason=assign>(read<ptr<i8>>(%7)));
// DEFAULT-NEXT:         write<ptr<u8>>(%6, pointer_cast<ptr<u8>, reason=assign>(read<ptr<i8>>(%7)));
// DEFAULT-NEXT:         write<ptr<u8>>(%6, pointer_cast<ptr<u8>, reason=assign>(read<ptr<i8>>(%5)));
// DEFAULT-NEXT:         write<ptr<i8>>(%7, pointer_cast<ptr<i8>, reason=assign>(read<ptr<u8>>(%6)));
// DEFAULT-NEXT:         write<ptr<i8>>(%7, pointer_cast<ptr<i8>, reason=assign>(read<ptr<i8>>(%5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
