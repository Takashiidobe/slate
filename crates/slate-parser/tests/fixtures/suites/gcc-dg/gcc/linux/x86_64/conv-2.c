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
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE1:[0-9]+]] <unnamed>: ptr<u64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_lp:[0-9]+]] lp: ptr<i64> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ulp:[0-9]+]] ulp: ptr<u64> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_cp:[0-9]+]] cp: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ucp:[0-9]+]] ucp: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_scp:[0-9]+]] scp: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<u64>>(%[[VALUE_ulp]], pointer_cast<ptr<u64>, reason=assign>(read<ptr<i64>>(%[[VALUE_lp]])));
// DEFAULT-NEXT:         write<ptr<i64>>(%[[VALUE_lp]], pointer_cast<ptr<i64>, reason=assign>(read<ptr<u64>>(%[[VALUE_ulp]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i64>) -> void>(%[[VALUE_f1]], pointer_cast<ptr<i64>, reason=arg>(read<ptr<u64>>(%[[VALUE_ulp]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u64>) -> void>(%[[VALUE_f2]], pointer_cast<ptr<u64>, reason=arg>(read<ptr<i64>>(%[[VALUE_lp]])));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_cp]], pointer_cast<ptr<i8>, reason=assign>(read<ptr<u8>>(%[[VALUE_ucp]])));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_cp]], pointer_cast<ptr<i8>, reason=assign>(read<ptr<i8>>(%[[VALUE_scp]])));
// DEFAULT-NEXT:         write<ptr<u8>>(%[[VALUE_ucp]], pointer_cast<ptr<u8>, reason=assign>(read<ptr<i8>>(%[[VALUE_scp]])));
// DEFAULT-NEXT:         write<ptr<u8>>(%[[VALUE_ucp]], pointer_cast<ptr<u8>, reason=assign>(read<ptr<i8>>(%[[VALUE_cp]])));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_scp]], pointer_cast<ptr<i8>, reason=assign>(read<ptr<u8>>(%[[VALUE_ucp]])));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_scp]], pointer_cast<ptr<i8>, reason=assign>(read<ptr<i8>>(%[[VALUE_cp]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
