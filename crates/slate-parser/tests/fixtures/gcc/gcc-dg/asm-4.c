/* { dg-do compile } */
/* { dg-options "" } */

/* "p" modifier can't be used to generate a valid memory address with ILP32.  */
/* { dg-skip-if "" { aarch64*-*-* && ilp32 } } */
/* { dg-skip-if "'p' is not supported for GCN" { amdgcn-*-* } } */

int main()
{
  int x, y, z;

  asm volatile ("test0 X%0Y%[arg]Z" : [arg] "=g" (x));
  asm volatile ("test1 X%[out]Y%[in]Z" : [out] "=g" (y) : [in] "0"(y));
  asm volatile ("test2 X%a0Y%a[arg]Z" : : [arg] "p" (&z));
  asm volatile ("test3 %[in]" : [inout] "=g"(x) : "[inout]" (x), [in] "g" (y));
}

/* { dg-final { scan-assembler {test0 X(.*)Y\1Z} } } */
/* { dg-final { scan-assembler {test1 X(.*)Y\1Z} } } */
/* { dg-final { scan-assembler {test2 X(.*)Y\1Z} } } */

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
// DEFAULT-NEXT:         let %1 x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %2 y: i32 [storage=automatic];
// DEFAULT-NEXT:         let %3 z: i32 [storage=automatic];
// DEFAULT-NEXT:         asm volatile "test0 X%0Y%[arg]Z" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             template: "test0 X" %0 "Y" %0 "Z";
// DEFAULT-NEXT:             lateout 0 [arg] "g" [reg | mem | imm | sym] -> reg width 32 place<i32>(%1);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "test1 X%[out]Y%[in]Z" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             template: "test1 X" %0 "Y" %0 "Z";
// DEFAULT-NEXT:             inlateout 0 [out] "g" [reg | mem | imm | sym] -> reg width 32 place<i32>(%2) from read<i32>(%2);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "test2 X%a0Y%a[arg]Z" [dialect=att] [options=nomem,nostack] [alternative=none] {
// DEFAULT-NEXT:             template: "test2 X" %a0 "Y" %a0 "Z";
// DEFAULT-NEXT:             in 0 [arg] "p" [unresolved("p")] width 64 addr_of<ptr<i32>>(%3);
// DEFAULT-NEXT:             rejected: 0 (operand 0: unresolved("p"));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "test3 %[in]" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             template: "test3 " %1;
// DEFAULT-NEXT:             inlateout 0 [inout] "g" [reg | mem | imm | sym] -> reg width 32 place<i32>(%1) from read<i32>(%1);
// DEFAULT-NEXT:             in 1 [in] "g" [reg | mem | imm | sym] -> reg width 32 read<i32>(%2);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
