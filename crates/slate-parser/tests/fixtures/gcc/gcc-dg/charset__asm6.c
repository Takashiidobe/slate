/* Test for string translation.  */
/* { dg-do compile }
   { dg-require-iconv "IBM1047" } 
   { dg-final { scan-assembler "foo" } } */
int main()
{
  unsigned long int *ptr;
  ptr = ((unsigned long int *)
         ( { void *stack_ptr;
           __asm__ __volatile__ ( "foo %0" : "=r" (stack_ptr) );
           (stack_ptr); } ) );
  return 0;
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
// DEFAULT-NEXT:     fn %0 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %1 ptr: ptr<u64> [storage=automatic];
// DEFAULT-NEXT:         let %3: ptr<void> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %2 stack_ptr: ptr<void> [storage=automatic];
// DEFAULT-NEXT:             asm volatile "foo %0" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:                 template: "foo " %0;
// DEFAULT-NEXT:                 lateout 0 "r" [reg] width 64 place<ptr<void>>(%2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<ptr<void>>(%3, read<ptr<void>>(%2));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<u64>>(%1, pointer_cast<ptr<u64>, reason=explicit>(read<ptr<void>>(%3)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
