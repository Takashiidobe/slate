// SLATE-FILECHECK-DEFINES DEFAULT

int counter;
double real;
unsigned char bytes[16];

void read_only(int limit) {
    __asm {
        cmp limit, 3
        test counter, 1
        bt counter, 2
        push limit
        fld real
        mul limit
    }
}

void write_only(void) {
    int local;
    short status;
    __asm {
        mov local, eax
        setz byte ptr [bytes]
        fstp real
        fnstsw status
        pop counter
    }
}

void read_write(int value) {
    __asm {
        add counter, 1
        mov eax, value
        mov value, eax
        xchg eax, counter
        xadd value, ecx
    }
}

void implicit_defs(void) {
    __asm {
        cdq
        cpuid
        rdtsc
        lahf
        xlat
        cmpxchg8b real
        popad
        leave
    }
}

void string_ops(void) {
    __asm {
        rep movsb
        repne scasb
        lodsd
        stosw
    again:
        loop again
    }
}

void explicit_registers(void) {
    __asm {
        mov al, 1
        mov bh, 2
        movzx esi, ax
        movdqu xmm3, bytes
        movd mm2, counter
        mov esp, ebp
        fstsw ax
        emms
    }
}

void calls(void) {
    __asm call read_only
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_counter:[0-9]+]] counter: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_real:[0-9]+]] real: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_bytes:[0-9]+]] bytes: array<u8, 16> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_read_only:[0-9]+]] @read_only(%[[VALUE_limit:[0-9]+]] limit: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "cmp limit, 3\ntest counter, 1\nbt counter, 2\npush limit\nfld real\nmul limit" [dialect=intel] {
// DEFAULT-NEXT:             template: "cmp " addr<dword>(%0) ", 3\ntest " addr<dword>(%1) ", 1\nbt " addr<dword>(%1) ", 2\npush " addr<dword>(%0) "\nfld " addr<qword>(%2) "\nmul " addr<dword>(%0);
// DEFAULT-NEXT:             in 0 [limit] mem<read> place<i32>(%[[VALUE_limit]]);
// DEFAULT-NEXT:             in 1 [counter] mem<read> place<i32>(%[[VALUE_counter]]);
// DEFAULT-NEXT:             in 2 [real] mem<read> place<f64>(%[[VALUE_real]]);
// DEFAULT-NEXT:             clobbers: "eax" as ax, "edx" as dx, "st" as st, "st(1)" as st(1), "st(2)" as st(2), "st(3)" as st(3), "st(4)" as st(4), "st(5)" as st(5), "st(6)" as st(6), "st(7)" as st(7);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_write_only:[0-9]+]] @write_only() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_local:[0-9]+]] local: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_status:[0-9]+]] status: i16 [storage=automatic];
// DEFAULT-NEXT:         asm volatile "mov local, eax\nsetz byte ptr [bytes]\nfstp real\nfnstsw status\npop counter" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov " addr(%0) ", eax\nsetz " addr<byte>(%1) "\nfstp " addr<qword>(%2) "\nfnstsw " addr<word>(%3) "\npop " addr<dword>(%4);
// DEFAULT-NEXT:             in 0 [local] mem<write> place<i32>(%[[VALUE_local]]);
// DEFAULT-NEXT:             in 1 [bytes] mem<write> place<array<u8, 16>>(%[[VALUE_bytes]]);
// DEFAULT-NEXT:             in 2 [real] mem<write> place<f64>(%[[VALUE_real]]);
// DEFAULT-NEXT:             in 3 [status] mem<write> place<i16>(%[[VALUE_status]]);
// DEFAULT-NEXT:             in 4 [counter] mem<write> place<i32>(%[[VALUE_counter]]);
// DEFAULT-NEXT:             clobbers: "st" as st, "st(1)" as st(1), "st(2)" as st(2), "st(3)" as st(3), "st(4)" as st(4), "st(5)" as st(5), "st(6)" as st(6), "st(7)" as st(7);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_read_write:[0-9]+]] @read_write(%[[VALUE_value:[0-9]+]] value: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "add counter, 1\nmov eax, value\nmov value, eax\nxchg eax, counter\nxadd value, ecx" [dialect=intel] {
// DEFAULT-NEXT:             template: "add " addr<dword>(%0) ", 1\nmov eax, " addr(%1) "\nmov " addr(%1) ", eax\nxchg eax, " addr(%0) "\nxadd " addr(%1) ", ecx";
// DEFAULT-NEXT:             in 0 [counter] mem<readwrite> place<i32>(%[[VALUE_counter]]);
// DEFAULT-NEXT:             in 1 [value] mem<readwrite> place<i32>(%[[VALUE_value]]);
// DEFAULT-NEXT:             clobbers: "eax" as ax, "ecx" as cx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_implicit_defs:[0-9]+]] @implicit_defs() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "cdq\ncpuid\nrdtsc\nlahf\nxlat\ncmpxchg8b real\npopad\nleave" [dialect=intel] {
// DEFAULT-NEXT:             template: "cdq\ncpuid\nrdtsc\nlahf\nxlat\ncmpxchg8b " addr<qword>(%0) "\npopad\nleave";
// DEFAULT-NEXT:             in 0 [real] mem<readwrite> place<f64>(%[[VALUE_real]]);
// DEFAULT-NEXT:             clobbers: "eax" as ax, "ebp" as bp, "ebx" as bx, "ecx" as cx, "edi" as di, "edx" as dx, "esi" as si;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_string_ops:[0-9]+]] @string_ops() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "rep movsb\nrepne scasb\nlodsd\nstosw\nagain:\nloop again" [dialect=intel] {
// DEFAULT-NEXT:             template: "rep movsb\nrepne scasb\nlodsd\nstosw\n" entry_label(%12, again) ":\nloop " label(again);
// DEFAULT-NEXT:             clobbers: "eax" as ax, "ecx" as cx, "edi" as di, "esi" as si;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_explicit_registers:[0-9]+]] @explicit_registers() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "mov al, 1\nmov bh, 2\nmovzx esi, ax\nmovdqu xmm3, bytes\nmovd mm2, counter\nmov esp, ebp\nfstsw ax\nemms" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov al, 1\nmov bh, 2\nmovzx esi, ax\nmovdqu xmm3, " addr(%0) "\nmovd mm2, " addr(%1) "\nmov esp, ebp\nfstsw ax\nemms";
// DEFAULT-NEXT:             in 0 [bytes] mem<read> place<array<u8, 16>>(%[[VALUE_bytes]]);
// DEFAULT-NEXT:             in 1 [counter] mem<read> place<i32>(%[[VALUE_counter]]);
// DEFAULT-NEXT:             clobbers: "eax" as ax, "ebx" as bx, "esi" as si, "mm0" as mm0, "mm1" as mm1, "mm2" as mm2, "mm3" as mm3, "mm4" as mm4, "mm5" as mm5, "mm6" as mm6, "mm7" as mm7, "st" as st, "st(1)" as st(1), "st(2)" as st(2), "st(3)" as st(3), "st(4)" as st(4), "st(5)" as st(5), "st(6)" as st(6), "st(7)" as st(7), "xmm3" as xmm3;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_calls:[0-9]+]] @calls() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "call read_only" [dialect=intel] {
// DEFAULT-NEXT:             template: "call " %0;
// DEFAULT-NEXT:             in 0 sym<offset=0>(%[[VALUE_read_only]]);
// DEFAULT-NEXT:             clobbers: "eax" as ax, "ecx" as cx, "edx" as dx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
