/* PR lto/48437 */
/* { dg-do run } */
/* { dg-options "-g" } */

#if defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#define NOP "nop 0"
#elif defined(__MMIX__)
#define NOP "swym 0"
#elif defined(__or1k__)
#define NOP "l.nop"
#else
#define NOP "nop"
#endif

int i __attribute__((used));
int main() {
  volatile int i;
  for (i = 3; i < 7; ++i) {
    extern int i;
    asm volatile(NOP : : : "memory"); /* { dg-final { gdb-test . "i" "0" } } */
  }
  return 0;
}



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
// DEFAULT-NEXT:     global %0 i: i32 [storage=static] [linkage=external] [used];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 i: volatile i32 [storage=automatic];
// DEFAULT-NEXT:         for %3
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32, volatile>(%2, const<i32>(3));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32, volatile>(%2), const<i32>(7))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %4: i32 [synthetic] = read<i32, volatile>(%2);
// DEFAULT-NEXT:                 let %5: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%4), const<i32>(1));
// DEFAULT-NEXT:                 write<i32, volatile>(%2, read<i32>(%5));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     asm volatile "nop" {
// DEFAULT-NEXT:                         template: "nop";
// DEFAULT-NEXT:                         clobbers: memory;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
