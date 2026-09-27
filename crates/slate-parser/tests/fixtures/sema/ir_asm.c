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

void tied_order(int a) {
    int x, y;
    asm("%0 %1 %2 %3 %4" : "=r"(x), "=r"(y) : "r"(a++), "1"(a *= 10), "0"(a -= 3), "r"(a <<= 1));
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
// IR-NEXT:         asm "nop" [dialect=att] [options=nostack];
// IR-NEXT:         asm volatile "mfence" [dialect=att] [options=nostack];
// IR-NEXT:     }
// IR-NEXT:     fn %2 @extended(%3 x: i32, %4 y: i32, %5 p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm volatile inline "mov %[in], %0 %% %= %{att%|intel%} %a1 %cc2" [dialect=att] [options=nostack,may_unwind] [alternative=0] {
// IR-NEXT:             template: "mov " %1 ", " %0 " " %% " " %= " {att|intel} " %a1 " " %c0;
// IR-NEXT:             inout 0 [out] "r,m" [reg, mem] width 32 place<i32>(%3) from read<i32>(deref(read<ptr<i32>>(%5)));
// IR-NEXT:             in 1 [in] "%rm,r" [reg | mem, reg] -> reg width 32 read<i32>(%4);
// IR-NEXT:             clobbers: memory, cc, unwind, "%rdx" as dx, "not_a_register";
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %6 @hoisted(%7 p: ptr<i32>, %8 n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %30: i32 [synthetic] = read<i32>(%8);
// IR-NEXT:         let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// IR-NEXT:         write<i32>(%8, read<i32>(%31));
// IR-NEXT:         let %32: i32 [synthetic] = read<i32>(%8);
// IR-NEXT:         let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// IR-NEXT:         write<i32>(%8, read<i32>(%33));
// IR-NEXT:         asm "op %0, %1" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "op " %0 ", " %1;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%7), read<i32>(%30))));
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%32);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %9 @tied_order(%10 a: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %11 x: i32 [storage=automatic];
// IR-NEXT:         let %12 y: i32 [storage=automatic];
// IR-NEXT:         let %34: i32 [synthetic] = read<i32>(%10);
// IR-NEXT:         let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// IR-NEXT:         write<i32>(%10, read<i32>(%35));
// IR-NEXT:         let %36: i32 [synthetic] = read<i32>(%10);
// IR-NEXT:         let %37: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%36), const<i32>(10));
// IR-NEXT:         write<i32>(%10, read<i32>(%37));
// IR-NEXT:         let %38: i32 [synthetic] = read<i32>(%10);
// IR-NEXT:         let %39: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%38), const<i32>(3));
// IR-NEXT:         write<i32>(%10, read<i32>(%39));
// IR-NEXT:         let %40: i32 [synthetic] = read<i32>(%10);
// IR-NEXT:         let %41: i32 [synthetic] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%40), const<i32>(1));
// IR-NEXT:         write<i32>(%10, read<i32>(%41));
// IR-NEXT:         asm "%0 %1 %2 %3 %4" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: %0 " " %1 " " %2 " " %1 " " %0;
// IR-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%11) from read<i32>(%39);
// IR-NEXT:             inlateout 1 "r" [reg] width 32 place<i32>(%12) from read<i32>(%37);
// IR-NEXT:             in 2 "r" [reg] width 32 read<i32>(%34);
// IR-NEXT:             in 3 "r" [reg] width 32 read<i32>(%41);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %13 @jumps(%16 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm goto "jmp %l[done] %l1 %2" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "jmp " %l0 " " %l0 " " %l1;
// IR-NEXT:             in 0 "r" [reg] width 32 read<i32>(%16);
// IR-NEXT:             labels: %14, %15;
// IR-NEXT:         }
// IR-NEXT:         write<i32>(%16, const<i32>(1));
// IR-NEXT:         label %14 done:
// IR-NEXT:             return;
// IR-NEXT:         label %15 other:
// IR-NEXT:             write<i32>(%16, const<i32>(0));
// IR-NEXT:     }
// IR-NEXT:     fn %17 @dialects(%18 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "mov{l|} {%0, %%eax|eax, %0}" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "movl " %0 ", " %% "eax";
// IR-NEXT:             in 0 "r" [reg] width 32 read<i32>(%18);
// IR-NEXT:         }
// IR-NEXT:         asm "a{b|c|d} {e} f|g} {h|i" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "ab e f|g} h";
// IR-NEXT:             in 0 "r" [reg] width 32 read<i32>(%18);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %19 @directions(%20 a: i32, %21 b: i32, %22 c: i32, %23 d: i32, %24 e: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "%0 %1 %2 %3" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: %0 " " %1 " " %2 " " %3;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%20);
// IR-NEXT:             out 1 "r" [reg] width 32 place<i32>(%21);
// IR-NEXT:             inlateout 2 "r" [reg] width 32 place<i32>(%22);
// IR-NEXT:             inout 3 "r" [reg] width 32 place<i32>(%23);
// IR-NEXT:         }
// IR-NEXT:         asm "%0 %1 %2 %3" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: %0 " " %1 " " %2 " " %1;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%20);
// IR-NEXT:             inout 1 "r" [reg] width 32 place<i32>(%21) from read<i32>(%23);
// IR-NEXT:             in 2 "r" [reg] width 32 read<i32>(%22);
// IR-NEXT:         }
// IR-NEXT:         asm "%0 %1 %2 %3" [dialect=att] [options=pure,nomem,nostack] [alternative=0] {
// IR-NEXT:             template: %0 " " %1 " " %0 " " %1;
// IR-NEXT:             inlateout 0 "r,m" [reg, mem] width 32 place<i32>(%20) from read<i32>(%21);
// IR-NEXT:             inlateout 1 "r,m" [reg, mem] width 32 place<i32>(%24) from read<i32>(%22);
// IR-NEXT:         }
// IR-NEXT:         asm "%[x] %[y]" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: %0 " " %0;
// IR-NEXT:             inlateout 0 [x] "r" [reg] width 32 place<i32>(%20) from read<i32>(%21);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %25 @memory(%26 x: i32, %27 p: ptr<i32>, %28 s: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %29 r: i32 [storage=automatic] = read<i32>(%26);
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             lateout 0 "m" [mem] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%27), const<i32>(1))));
// IR-NEXT:             in 1 "m" [mem] width 32 place<i32>(deref(read<ptr<i32>>(%27)));
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2;
// IR-NEXT:             inlateout 0 "m" [mem] width 32 place<i32>(%26);
// IR-NEXT:             in 1 "rm" [reg | mem] -> reg width 32 add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// IR-NEXT:             in 2 "g" [reg | mem | imm] -> reg width 32 read<i32>(field1(deref(read<ptr<@type0>>(%28))));
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %0;
// IR-NEXT:             inlateout 0 "m" [mem] width 32 place<i32>(%26) from read<i32>(deref(read<ptr<i32>>(%27)));
// IR-NEXT:             in 1 "m" [mem] width 32 read<i32>(%29);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
