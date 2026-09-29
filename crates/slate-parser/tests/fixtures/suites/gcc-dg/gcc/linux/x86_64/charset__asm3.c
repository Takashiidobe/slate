/* Test for complex asm statements. Make sure it compiles
   then test for some of the asm statements not being translated.  */
/* { dg-do compile { target { { i?86-*-* x86_64-*-* } && ilp32 } } }
   { dg-require-iconv "IBM1047" }
   { dg-final { scan-assembler "std" } }
   { dg-final { scan-assembler "cld" } }
   { dg-final { scan-assembler "rep" } }
   { dg-final { scan-assembler "movsb" } } */
#define size_t int
void *
memmove (void *__dest, __const void *__src, size_t __n)
{
  register unsigned long int __d0, __d1, __d2;
  if (__dest < __src)
    __asm__ __volatile__
      ("cld\n\t"
       "rep\n\t"
       "movsb"
       : "=&c" (__d0), "=&S" (__d1), "=&D" (__d2)
       : "0" (__n), "1" (__src), "2" (__dest)
       : "memory");
  else
    __asm__ __volatile__
      ("std\n\t"
       "rep\n\t"
       "movsb\n\t"
       "cld"
       : "=&c" (__d0), "=&S" (__d1), "=&D" (__d2)
       : "0" (__n), "1" (__n - 1 + (const char *) __src),
         "2" (__n - 1 + (char *) __dest)
       : "memory");
  return __dest;
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
// DEFAULT-NEXT:     fn %[[VALUE_memmove:[0-9]+]] @memmove(%[[VALUE___dest:[0-9]+]] __dest: ptr<void>, %[[VALUE___src:[0-9]+]] __src: ptr<const void>, %[[VALUE___n:[0-9]+]] __n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE___d0:[0-9]+]] __d0: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE___d1:[0-9]+]] __d1: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE___d2:[0-9]+]] __d2: u64 [storage=automatic];
// DEFAULT-NEXT:         if lt<ptr<void>>(read<ptr<void>>(%[[VALUE___dest]]), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<const void>>(%[[VALUE___src]])))
// DEFAULT-NEXT:             asm volatile "cld\\n\\trep\\n\\tmovsb" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:                 template: "cld\\n\\trep\\n\\tmovsb";
// DEFAULT-NEXT:                 inout 0 "c" [{cx}] width 64 place<u64>(%[[VALUE___d0]]) from read<i32>(%[[VALUE___n]]);
// DEFAULT-NEXT:                 inout 1 "S" [{si}] width 64 place<u64>(%[[VALUE___d1]]) from read<ptr<const void>>(%[[VALUE___src]]);
// DEFAULT-NEXT:                 inout 2 "D" [{di}] width 64 place<u64>(%[[VALUE___d2]]) from read<ptr<void>>(%[[VALUE___dest]]);
// DEFAULT-NEXT:                 clobbers: memory;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             asm volatile "std\\n\\trep\\n\\tmovsb\\n\\tcld" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:                 template: "std\\n\\trep\\n\\tmovsb\\n\\tcld";
// DEFAULT-NEXT:                 inout 0 "c" [{cx}] width 64 place<u64>(%[[VALUE___d0]]) from read<i32>(%[[VALUE___n]]);
// DEFAULT-NEXT:                 inout 1 "S" [{si}] width 64 place<u64>(%[[VALUE___d1]]) from ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<const i8>, reason=explicit>(read<ptr<const void>>(%[[VALUE___src]])), sub<i32, overflow=ub>(read<i32>(%[[VALUE___n]]), const<i32>(1)));
// DEFAULT-NEXT:                 inout 2 "D" [{di}] width 64 place<u64>(%[[VALUE___d2]]) from ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<void>>(%[[VALUE___dest]])), sub<i32, overflow=ub>(read<i32>(%[[VALUE___n]]), const<i32>(1)));
// DEFAULT-NEXT:                 clobbers: memory;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<ptr<void>>(%[[VALUE___dest]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
