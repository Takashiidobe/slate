// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
struct Pair { int b : 3; int w; };

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

void memory(int x, int *p, struct Pair *s) {
    register int r = x;
    asm("# %0 %1" : "=m"(p[1]) : "m"(*p));
    asm("# %0 %1 %2" : "+m"(x) : "rm"(x + 1), "g"(s->w));
    asm("# %0 %1" : "=m"(x) : "0"(*p), "m"(r));
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
// IR-NEXT:     type @type0 Pair = struct {
// IR-NEXT:         field0 b: i32 : 3;
// IR-NEXT:         field1 w: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// IR-NEXT:     fn %1 @basic() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "nop" [dialect=att];
// IR-NEXT:         asm volatile "mfence" [dialect=att];
// IR-NEXT:     }
// IR-NEXT:     fn %2 @extended(%3 x: i32, %4 y: i32, %5 p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm volatile inline "mov %[in], %0 %% %= %{att%|intel%} %a1 %cc2" [dialect=att] [alternative=0] {
// IR-NEXT:             template: "mov " %1 ", " %0 " " %% " " %= " {att|intel} " %a1 " " %c0;
// IR-NEXT:             inout 0 [out] "r,m" [reg, mem] width 32 place<i32>(%3) from read<i32>(deref(read<ptr<i32>>(%5)));
// IR-NEXT:             in 1 [in] "%rm,r" [reg | mem, reg] -> reg width 32 read<i32>(%4);
// IR-NEXT:             clobbers: memory, cc, unwind, "%rdx" as dx, "not_a_register";
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %6 @hoisted(%7 p: ptr<i32>, %8 n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %26: i32 [synthetic] = read<i32>(%8);
// IR-NEXT:         let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// IR-NEXT:         write<i32>(%8, read<i32>(%27));
// IR-NEXT:         let %28: i32 [synthetic] = read<i32>(%8);
// IR-NEXT:         let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// IR-NEXT:         write<i32>(%8, read<i32>(%29));
// IR-NEXT:         asm "op %0, %1" [dialect=att] {
// IR-NEXT:             template: "op " %0 ", " %1;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%7), read<i32>(%26))));
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%28);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %9 @jumps(%12 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm goto "jmp %l[done] %l1 %2" [dialect=att] {
// IR-NEXT:             template: "jmp " %l0 " " %l0 " " %l1;
// IR-NEXT:             in 0 "r" [reg] width 32 read<i32>(%12);
// IR-NEXT:             labels: %10, %11;
// IR-NEXT:         }
// IR-NEXT:         write<i32>(%12, const<i32>(1));
// IR-NEXT:         label %10 done:
// IR-NEXT:             return;
// IR-NEXT:         label %11 other:
// IR-NEXT:             write<i32>(%12, const<i32>(0));
// IR-NEXT:     }
// IR-NEXT:     fn %13 @dialects(%14 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "mov{l|} {%0, %%eax|eax, %0}" [dialect=att] {
// IR-NEXT:             template: "movl " %0 ", " %% "eax";
// IR-NEXT:             in 0 "r" [reg] width 32 read<i32>(%14);
// IR-NEXT:         }
// IR-NEXT:         asm "a{b|c|d} {e} f|g} {h|i" [dialect=att] {
// IR-NEXT:             template: "ab e f|g} h";
// IR-NEXT:             in 0 "r" [reg] width 32 read<i32>(%14);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %15 @directions(%16 a: i32, %17 b: i32, %18 c: i32, %19 d: i32, %20 e: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "%0 %1 %2 %3" [dialect=att] {
// IR-NEXT:             template: %0 " " %1 " " %2 " " %3;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%16);
// IR-NEXT:             out 1 "r" [reg] width 32 place<i32>(%17);
// IR-NEXT:             inlateout 2 "r" [reg] width 32 place<i32>(%18);
// IR-NEXT:             inout 3 "r" [reg] width 32 place<i32>(%19);
// IR-NEXT:         }
// IR-NEXT:         asm "%0 %1 %2 %3" [dialect=att] {
// IR-NEXT:             template: %0 " " %1 " " %2 " " %1;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%16);
// IR-NEXT:             inout 1 "r" [reg] width 32 place<i32>(%17) from read<i32>(%19);
// IR-NEXT:             in 2 "r" [reg] width 32 read<i32>(%18);
// IR-NEXT:         }
// IR-NEXT:         asm "%0 %1 %2 %3" [dialect=att] [alternative=0] {
// IR-NEXT:             template: %0 " " %1 " " %0 " " %1;
// IR-NEXT:             inlateout 0 "r,m" [reg, mem] width 32 place<i32>(%16) from read<i32>(%17);
// IR-NEXT:             inlateout 1 "r,m" [reg, mem] width 32 place<i32>(%20) from read<i32>(%18);
// IR-NEXT:         }
// IR-NEXT:         asm "%[x] %[y]" [dialect=att] {
// IR-NEXT:             template: %0 " " %0;
// IR-NEXT:             inlateout 0 [x] "r" [reg] width 32 place<i32>(%16) from read<i32>(%17);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %21 @memory(%22 x: i32, %23 p: ptr<i32>, %24 s: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %25 r: i32 [storage=automatic] = read<i32>(%22);
// IR-NEXT:         asm "# %0 %1" [dialect=att] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             lateout 0 "m" [mem] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%23), const<i32>(1))));
// IR-NEXT:             in 1 "m" [mem] width 32 place<i32>(deref(read<ptr<i32>>(%23)));
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2" [dialect=att] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2;
// IR-NEXT:             inlateout 0 "m" [mem] width 32 place<i32>(%22);
// IR-NEXT:             in 1 "rm" [reg | mem] -> reg width 32 add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// IR-NEXT:             in 2 "g" [reg | mem | imm] -> reg width 32 read<i32>(field1(deref(read<ptr<@type0>>(%24))));
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] {
// IR-NEXT:             template: "# " %0 " " %0;
// IR-NEXT:             inlateout 0 "m" [mem] width 32 place<i32>(%22) from read<i32>(deref(read<ptr<i32>>(%23)));
// IR-NEXT:             in 1 "m" [mem] width 32 read<i32>(%25);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
