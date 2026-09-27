void operands(int x, int y, int *p) {
  asm("basic %eax %0");
  __asm__ volatile inline("mov %[in], %0 %% %= %{att%|intel%} %a1 %cc2"
                          : [out] "=&r,m"(x)
                          : [in] "%-rm,r"(y), "[out],m"(*p)
                          : "memory", "cc", "unwind", "%rdx", "not_a_register");
}

void jumps(int x) {
  asm goto("jmp %l[done] %l1 %2" : : "r"(x) : : done, other);
  asm goto("jmp %l0" : : : : done);
  x = 1;
done:
  return;
other:
  x = 0;
}

void dialects(int x) {
  asm("mov{l|} {%0, %%eax|eax, %0} {a|b" : : "r"(x));
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
// DEFAULT-NEXT:     fn %0 @operands(%1 x: i32, %2 y: i32, %3 p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm "basic %eax %0" [dialect=att];
// DEFAULT-NEXT:         asm volatile inline "mov %[in], %0 %% %= %{att%|intel%} %a1 %cc2" [dialect=att] {
// DEFAULT-NEXT:             template: "mov " %1 ", " %0 " " %% " " %= " {att|intel} " %a1 " " %c2;
// DEFAULT-NEXT:             out 0 [out] "r,m" place<i32>(%1);
// DEFAULT-NEXT:             in 1 [in] "%-rm,r" read<i32>(%2);
// DEFAULT-NEXT:             in 2 "0,m" read<i32>(deref(read<ptr<i32>>(%3)));
// DEFAULT-NEXT:             clobbers: memory, cc, unwind, "%rdx" as dx, "not_a_register";
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @jumps(%7 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm goto "jmp %l[done] %l1 %2" [dialect=att] {
// DEFAULT-NEXT:             template: "jmp " %l0 " " %l0 " " %l1;
// DEFAULT-NEXT:             in 0 "r" read<i32>(%7);
// DEFAULT-NEXT:             labels: %5, %6;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm goto "jmp %l0" [dialect=att] {
// DEFAULT-NEXT:             template: "jmp " %l0;
// DEFAULT-NEXT:             labels: %5;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32>(%7, const<i32>(1));
// DEFAULT-NEXT:         label %5 done:
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         label %6 other:
// DEFAULT-NEXT:             write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @dialects(%9 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm "mov{l|} {%0, %%eax|eax, %0} {a|b" [dialect=att] {
// DEFAULT-NEXT:             template: "movl " %0 ", " %% "eax a";
// DEFAULT-NEXT:             in 0 "r" read<i32>(%9);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
