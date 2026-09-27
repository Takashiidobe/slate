void f(int x, int y, int z, int w) {
  asm("%0 %1 %2" : "=r,m"(x) : "0,m"(y), "m,0"(z));
  asm("%0 %1 %2 %3" : "=r,r"(x), "=r,r"(w) : "0,1"(y), "1,0"(z));
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %0 @f(%1 x: i32, %2 y: i32, %3 z: i32, %4 w: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm "%0 %1 %2" [dialect=att] {
// DEFAULT-NEXT:             template: %0 " " %1 " " %2;
// DEFAULT-NEXT:             lateout 0 "r,m" [reg, mem] place<i32>(%1);
// DEFAULT-NEXT:             in 1 "0,m" [0, mem] read<i32>(%2);
// DEFAULT-NEXT:             in 2 "m,0" [mem, 0] read<i32>(%3);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm "%0 %1 %2 %3" [dialect=att] {
// DEFAULT-NEXT:             template: %0 " " %1 " " %2 " " %3;
// DEFAULT-NEXT:             lateout 0 "r,r" [reg, reg] place<i32>(%1);
// DEFAULT-NEXT:             lateout 1 "r,r" [reg, reg] place<i32>(%4);
// DEFAULT-NEXT:             in 2 "0,1" [0, 1] read<i32>(%2);
// DEFAULT-NEXT:             in 3 "1,0" [1, 0] read<i32>(%3);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
