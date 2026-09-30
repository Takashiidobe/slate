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
// IR-NEXT:     type @type[[TYPE_Pair:[0-9]+]] Pair = struct {
// IR-NEXT:         field0 b: i32 : 3;
// IR-NEXT:         field1 w: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[Some(0), None], bit_units=[(0, 1)], field_units=[Some(0), None]];
// IR-NEXT:     fn %[[VALUE_basic:[0-9]+]] @basic() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "nop" [dialect=att] [options=nostack];
// IR-NEXT:         asm volatile "mfence" [dialect=att] [options=nostack];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_extended:[0-9]+]] @extended(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32, %[[VALUE_p:[0-9]+]] p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm volatile inline "mov %[in], %0 %% %= %{att%|intel%} %a1 %cc2" [dialect=att] [options=nostack,may_unwind] [alternative=0] {
// IR-NEXT:             template: "mov " %1 ", " %0 " " %% " " %= " {att|intel} " %a1 " " %c0;
// IR-NEXT:             inout 0 [out] "r,m" [reg, mem] width 32 place<i32>(%[[VALUE_x]]) from read<i32>(deref(read<ptr<i32>>(%[[VALUE_p]])));
// IR-NEXT:             in 1 [in] "%rm,r" [reg | mem, reg] -> reg width 32 read<i32>(%[[VALUE_y]]);
// IR-NEXT:             clobbers: memory, cc, unwind, "%rdx" as dx, "not_a_register";
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_hoisted:[0-9]+]] @hoisted(%[[VALUE_p_2:[0-9]+]] p: ptr<i32>, %[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE1]]));
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE3]]));
// IR-NEXT:         asm "op %0, %1" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "op " %0 ", " %1;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_2]]), read<i32>(%[[VALUE0]]))));
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%[[VALUE2]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_tied_order:[0-9]+]] @tied_order(%[[VALUE_a:[0-9]+]] a: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE5]]));
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(10));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE7]]));
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(3));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE9]]));
// IR-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE11]]));
// IR-NEXT:         asm "%0 %1 %2 %3 %4" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: %0 " " %1 " " %2 " " %1 " " %0;
// IR-NEXT:             inlateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_x_2]]) from read<i32>(%[[VALUE9]]);
// IR-NEXT:             inlateout 1 "r" [reg] width 32 place<i32>(%[[VALUE_y_2]]) from read<i32>(%[[VALUE7]]);
// IR-NEXT:             in 2 "r" [reg] width 32 read<i32>(%[[VALUE4]]);
// IR-NEXT:             in 3 "r" [reg] width 32 read<i32>(%[[VALUE11]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_jumps:[0-9]+]] @jumps(%[[VALUE_x_3:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm goto "jmp %l[done] %l1 %2" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "jmp " %l0 " " %l0 " " %l1;
// IR-NEXT:             in 0 "r" [reg] width 32 read<i32>(%[[VALUE_x_3]]);
// IR-NEXT:             labels: %[[VALUE_done:[0-9]+]], %[[VALUE_other:[0-9]+]];
// IR-NEXT:         }
// IR-NEXT:         write<i32>(%[[VALUE_x_3]], const<i32>(1));
// IR-NEXT:         label %[[VALUE_done]] done:
// IR-NEXT:             return;
// IR-NEXT:         label %[[VALUE_other]] other:
// IR-NEXT:             write<i32>(%[[VALUE_x_3]], const<i32>(0));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_dialects:[0-9]+]] @dialects(%[[VALUE_x_4:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "mov{l|} {%0, %%eax|eax, %0}" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "movl " %0 ", " %% "eax";
// IR-NEXT:             in 0 "r" [reg] width 32 read<i32>(%[[VALUE_x_4]]);
// IR-NEXT:         }
// IR-NEXT:         asm "a{b|c|d} {e} f|g} {h|i" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "ab e f|g} h";
// IR-NEXT:             in 0 "r" [reg] width 32 read<i32>(%[[VALUE_x_4]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_directions:[0-9]+]] @directions(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32, %[[VALUE_c:[0-9]+]] c: i32, %[[VALUE_d:[0-9]+]] d: i32, %[[VALUE_e:[0-9]+]] e: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "%0 %1 %2 %3" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: %0 " " %1 " " %2 " " %3;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_a_2]]);
// IR-NEXT:             out 1 "r" [reg] width 32 place<i32>(%[[VALUE_b]]);
// IR-NEXT:             inlateout 2 "r" [reg] width 32 place<i32>(%[[VALUE_c]]);
// IR-NEXT:             inout 3 "r" [reg] width 32 place<i32>(%[[VALUE_d]]);
// IR-NEXT:         }
// IR-NEXT:         asm "%0 %1 %2 %3" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: %0 " " %1 " " %2 " " %1;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_a_2]]);
// IR-NEXT:             inout 1 "r" [reg] width 32 place<i32>(%[[VALUE_b]]) from read<i32>(%[[VALUE_d]]);
// IR-NEXT:             in 2 "r" [reg] width 32 read<i32>(%[[VALUE_c]]);
// IR-NEXT:         }
// IR-NEXT:         asm "%0 %1 %2 %3" [dialect=att] [options=pure,nomem,nostack] [alternative=0] {
// IR-NEXT:             template: %0 " " %1 " " %0 " " %1;
// IR-NEXT:             inlateout 0 "r,m" [reg, mem] width 32 place<i32>(%[[VALUE_a_2]]) from read<i32>(%[[VALUE_b]]);
// IR-NEXT:             inlateout 1 "r,m" [reg, mem] width 32 place<i32>(%[[VALUE_e]]) from read<i32>(%[[VALUE_c]]);
// IR-NEXT:         }
// IR-NEXT:         asm "%[x] %[y]" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: %0 " " %0;
// IR-NEXT:             inlateout 0 [x] "r" [reg] width 32 place<i32>(%[[VALUE_a_2]]) from read<i32>(%[[VALUE_b]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_memory:[0-9]+]] @memory(%[[VALUE_x_5:[0-9]+]] x: i32, %[[VALUE_p_3:[0-9]+]] p: ptr<i32>, %[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE_Pair]]>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic] = read<i32>(%[[VALUE_x_5]]);
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             lateout 0 "m" [mem] width 32 place<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_p_3]]), const<i32>(1))));
// IR-NEXT:             in 1 "m" [mem] width 32 place<i32>(deref(read<ptr<i32>>(%[[VALUE_p_3]])));
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2;
// IR-NEXT:             inlateout 0 "m" [mem] width 32 place<i32>(%[[VALUE_x_5]]);
// IR-NEXT:             in 1 "rm" [reg | mem] -> reg width 32 add<i32, overflow=ub>(read<i32>(%[[VALUE_x_5]]), const<i32>(1));
// IR-NEXT:             in 2 "g" [reg | mem | imm | sym] -> reg width 32 read<i32>(field1(deref(read<ptr<@type[[TYPE_Pair]]>>(%[[VALUE_s]]))));
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %0;
// IR-NEXT:             inlateout 0 "m" [mem] width 32 place<i32>(%[[VALUE_x_5]]) from read<i32>(deref(read<ptr<i32>>(%[[VALUE_p_3]])));
// IR-NEXT:             in 1 "m" [mem] width 32 read<i32>(%[[VALUE_r]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
