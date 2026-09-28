// SLATE-FILECHECK-DEFINES DEFAULT

/* PR c/7160 */
/* Verify that the register-to-stack converter properly handles
   branches without return value containing function calls.  */
   
extern int gi;

extern int foo1(int, int);
extern void foo2(int, int);
extern float foo3(int);

float bar(int i1, int i2)
{
  int i3;
    
  if (i2) {
    i3 = foo1(i1, gi);
    foo2(i1, i3);
  }
  else
    return foo3(i2);
}

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
// DEFAULT-NEXT:     extern %0 gi: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo1(%8 <unnamed>: i32, %9 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo2(%10 <unnamed>: i32, %11 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo3(%12 <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @bar(%5 i1: i32, %6 i2: i32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 i3: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%7, call<i32, signature=fn(i32, i32) -> i32>(%1, read<i32>(%5), read<i32>(%0)));
// DEFAULT-NEXT:                 call<i32, signature=fn(i32, i32) -> i32>(%1, read<i32>(%5), read<i32>(%0));
// DEFAULT-NEXT:                 call<void, signature=fn(i32, i32) -> void>(%2, read<i32>(%5), read<i32>(%7));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return call<f32, signature=fn(i32) -> f32>(%3, read<i32>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
