// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
void basic(void) {
    asm("nop");
    asm volatile("mfence");
}

void extended(int x, int y, int *p) {
    __asm__ volatile inline("mov %[in], %0 %% %= %{att%|intel%} %a1 %cc2"
                            : [out] "=&r,m"(x)
                            : [in] "%rm,r"(y), "[out],m"(*p)
                            : "memory", "cc", "unwind", "%rdx", "not_a_register");
}

void hoisted(int *p, int n) {
    asm("op %0, %1" : "=r"(p[n++]) : "r"(n++));
}

void jumps(int x) {
    asm goto("jmp %l[done] %l1 %2" : : "r"(x) : : done, other);
    x = 1;
done:
    return;
other:
    x = 0;
}

void dialects(int x) {
    asm("mov{l|} {%0, %%eax|eax, %0}" : : "r"(x));
    asm("a{b|c|d} {e} f|g} {h|i" : : "r"(x));
}

void directions(int a, int b, int c, int d, int e) {
    asm("%0 %1 %2 %3" : "=r"(a), "=&r"(b), "+r"(c), "+&r"(d));
    asm("%0 %1 %2 %3" : "=r"(a), "=&r"(b) : "r"(c), "1"(d));
    asm("%0 %1 %2 %3" : "=r,m"(a), "=r,m"(e) : "0,0"(b), "1,m"(c));
    asm("%[x] %[y]" : [x] "=r"(a) : [y] "[x]"(b));
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %0 @basic() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "nop" [dialect=att];
// IR-NEXT:         asm volatile "mfence" [dialect=att];
// IR-NEXT:     }
// IR-NEXT:     fn %1 @extended(%2 x: i32, %3 y: i32, %4 p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm volatile inline "mov %[in], %0 %% %= %{att%|intel%} %a1 %cc2" [dialect=att] {
// IR-NEXT:             template: "mov " %1 ", " %0 " " %% " " %= " {att|intel} " %a1 " " %c2;
// IR-NEXT:             out 0 [out] "r,m" place<i32>(%2);
// IR-NEXT:             in 1 [in] "%rm,r" read<i32>(%3);
// IR-NEXT:             in 2 "0,m" read<i32>(deref(read<ptr<i32>>(%4)));
// IR-NEXT:             clobbers: memory, cc, unwind, "%rdx" as dx, "not_a_register";
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %5 @hoisted(%6 p: ptr<i32>, %7 n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %20: i32 [synthetic] = read<i32>(%7);
// IR-NEXT:         let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// IR-NEXT:         write<i32>(%7, read<i32>(%21));
// IR-NEXT:         let %22: i32 [synthetic] = read<i32>(%7);
// IR-NEXT:         let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// IR-NEXT:         write<i32>(%7, read<i32>(%23));
// IR-NEXT:         asm "op %0, %1" [dialect=att] {
// IR-NEXT:             template: "op " %0 ", " %1;
// IR-NEXT:             lateout 0 "r" place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%6), read<i32>(%20))));
// IR-NEXT:             in 1 "r" read<i32>(%22);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %8 @jumps(%11 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm goto "jmp %l[done] %l1 %2" [dialect=att] {
// IR-NEXT:             template: "jmp " %l0 " " %l0 " " %l1;
// IR-NEXT:             in 0 "r" read<i32>(%11);
// IR-NEXT:             labels: %9, %10;
// IR-NEXT:         }
// IR-NEXT:         write<i32>(%11, const<i32>(1));
// IR-NEXT:         label %9 done:
// IR-NEXT:             return;
// IR-NEXT:         label %10 other:
// IR-NEXT:             write<i32>(%11, const<i32>(0));
// IR-NEXT:     }
// IR-NEXT:     fn %12 @dialects(%13 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "mov{l|} {%0, %%eax|eax, %0}" [dialect=att] {
// IR-NEXT:             template: "movl " %0 ", " %% "eax";
// IR-NEXT:             in 0 "r" read<i32>(%13);
// IR-NEXT:         }
// IR-NEXT:         asm "a{b|c|d} {e} f|g} {h|i" [dialect=att] {
// IR-NEXT:             template: "ab e f|g} h";
// IR-NEXT:             in 0 "r" read<i32>(%13);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %14 @directions(%15 a: i32, %16 b: i32, %17 c: i32, %18 d: i32, %19 e: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "%0 %1 %2 %3" [dialect=att] {
// IR-NEXT:             template: %0 " " %1 " " %2 " " %3;
// IR-NEXT:             lateout 0 "r" place<i32>(%15);
// IR-NEXT:             out 1 "r" place<i32>(%16);
// IR-NEXT:             inlateout 2 "r" place<i32>(%17);
// IR-NEXT:             inout 3 "r" place<i32>(%18);
// IR-NEXT:         }
// IR-NEXT:         asm "%0 %1 %2 %3" [dialect=att] {
// IR-NEXT:             template: %0 " " %1 " " %2 " " %1;
// IR-NEXT:             lateout 0 "r" place<i32>(%15);
// IR-NEXT:             inout 1 "r" place<i32>(%16) from read<i32>(%18);
// IR-NEXT:             in 2 "r" read<i32>(%17);
// IR-NEXT:         }
// IR-NEXT:         asm "%0 %1 %2 %3" [dialect=att] {
// IR-NEXT:             template: %0 " " %1 " " %0 " " %2;
// IR-NEXT:             inlateout 0 "r,m" place<i32>(%15) from read<i32>(%16);
// IR-NEXT:             lateout 1 "r,m" place<i32>(%19);
// IR-NEXT:             in 2 "1,m" read<i32>(%17);
// IR-NEXT:         }
// IR-NEXT:         asm "%[x] %[y]" [dialect=att] {
// IR-NEXT:             template: %0 " " %0;
// IR-NEXT:             inlateout 0 [x] "r" place<i32>(%15) from read<i32>(%16);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
