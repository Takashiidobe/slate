// SLATE-FILECHECK-DEFINES DEFAULT

#if !defined(_MSC_VER)
#error This test uses Microsoft-style inline __asm syntax.
#endif

#if !defined(_M_IX86)
#error This test requires 32-bit x86 MSVC mode, since this tests __asm.
#endif

// This is an example guarded by _MSC_VER && _M_IX86, so this is for i686 msvc
// to properly compile
int round_x87(double value) {
    int result;
    __asm {
        fld value
        fistp result
    }
    return result;
}

// This one comes from SDL, it emulated cpuid to properly compile
int have_cpuid(void) {
    int result = 0;

    __asm {
        pushfd
        pop eax
        mov ecx, eax
        xor eax, 200000h
        push eax
        popfd
        pushfd
        pop eax
        xor eax, ecx
        jz done
        mov result, 1
    done:
    }

    return result;
}

// Function like macro that repeatedly uses asm and even statement style
#define READ_CPUID(func, a, b, c, d) \
    __asm {                           \
        __asm mov eax, func           \
        __asm xor ecx, ecx            \
        __asm cpuid                   \
        __asm mov a, eax              \
        __asm mov b, ebx              \
        __asm mov c, ecx              \
        __asm mov d, edx              \
    }


/*
 * Basic x87:
 *   - C variable references
 *   - fld/fistp
 *   - x87 stack
 */
int asm_round_x87(double value)
{
    int result;

    __asm {
        fld value
        fistp result
    }

    return result;
}


/*
 * C locals + local asm label:
 *   - C identifier reads/writes
 *   - test/jz
 *   - local label
 */
int asm_local_and_label(int value)
{
    int result = 0;

    __asm {
        mov eax, value
        test eax, eax
        jz zero_case

        add eax, 10
        mov result, eax
        jmp done

    zero_case:
        mov result, 1234h

    done:
    }

    return result;
}


/*
 * Implicit EDX:EAX result from MUL.
 *
 * Return convention for unsigned __int64 on 32-bit MSVC is EDX:EAX,
 * so a naked function makes this particularly convenient.
 */
__declspec(naked)
unsigned __int64 asm_mul_u32(unsigned a, unsigned b)
{
    __asm {
        mov eax, dword ptr [esp+4]
        mul dword ptr [esp+8]
        ret
    }
}


/*
 * Carry propagation:
 *   - add/adc
 *   - implicit EFLAGS dependency
 *   - EDX:EAX 64-bit return
 */
__declspec(naked)
unsigned __int64 asm_add_u64(
    unsigned a_lo,
    unsigned a_hi,
    unsigned b_lo,
    unsigned b_hi)
{
    __asm {
        mov eax, dword ptr [esp+4]
        mov edx, dword ptr [esp+8]

        add eax, dword ptr [esp+0Ch]
        adc edx, dword ptr [esp+10h]

        ret
    }
}


/*
 * Borrow propagation:
 *   - sub/sbb
 *   - EFLAGS dependency
 */
__declspec(naked)
unsigned __int64 asm_sub_u64(
    unsigned a_lo,
    unsigned a_hi,
    unsigned b_lo,
    unsigned b_hi)
{
    __asm {
        mov eax, dword ptr [esp+4]
        mov edx, dword ptr [esp+8]

        sub eax, dword ptr [esp+0Ch]
        sbb edx, dword ptr [esp+10h]

        ret
    }
}


/*
 * 64-bit negation:
 *   - neg
 *   - sbb using carry from the previous NEG
 */
__declspec(naked)
__int64 asm_neg_i64(__int64 value)
{
    __asm {
        mov eax, dword ptr [esp+4]
        mov edx, dword ptr [esp+8]

        neg eax
        adc edx, 0
        neg edx

        ret
    }
}


/*
 * Shift a 64-bit integer right by one:
 *
 *     EDX:EAX >>= 1
 *
 * Exercises SHR followed by RCR, where RCR consumes the carry
 * produced by SHR.
 */
__declspec(naked)
unsigned __int64 asm_shr_u64(unsigned __int64 value)
{
    __asm {
        mov eax, dword ptr [esp+4]
        mov edx, dword ptr [esp+8]

        shr edx, 1
        rcr eax, 1

        ret
    }
}


/*
 * Natural loop + RCR carry chain.
 *
 * Shifts EDX:EAX right until EDX becomes zero.
 * This is mainly intended as a CFG/parser torture case.
 */
__declspec(naked)
unsigned __int64 asm_normalize_u64(unsigned __int64 value)
{
    __asm {
        mov eax, dword ptr [esp+4]
        mov edx, dword ptr [esp+8]

    normalize:
        test edx, edx
        jz done

        shr edx, 1
        rcr eax, 1
        jmp normalize

    done:
        ret
    }
}


/*
 * DIV:
 *   EDX:EAX / ECX
 *
 * Restrict the numerator to 32 bits here so the quotient is
 * guaranteed to fit in EAX.
 */
__declspec(naked)
unsigned asm_div_u32(unsigned numerator, unsigned denominator)
{
    __asm {
        mov eax, dword ptr [esp+4]
        xor edx, edx
        mov ecx, dword ptr [esp+8]

        div ecx

        ret
    }
}


/*
 * Return remainder instead of quotient.
 */
__declspec(naked)
unsigned asm_rem_u32(unsigned numerator, unsigned denominator)
{
    __asm {
        mov eax, dword ptr [esp+4]
        xor edx, edx
        mov ecx, dword ptr [esp+8]

        div ecx

        mov eax, edx
        ret
    }
}


/*
 * Explicit stack frame:
 *   - push ebp
 *   - mov ebp, esp
 *   - local stack allocation
 *   - dword ptr
 *   - leave
 *
 * Since this is naked, the asm owns the entire frame.
 */
__declspec(naked)
int asm_manual_frame(int value)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 10h

        mov eax, dword ptr [ebp+8]
        add eax, 5

        mov dword ptr [ebp-4], eax
        mov eax, dword ptr [ebp-4]

        leave
        ret
    }
}


/*
 * More substantial branch graph.
 */
__declspec(naked)
int asm_branch_graph(int value)
{
    __asm {
        mov eax, dword ptr [esp+4]

        test eax, eax
        jz was_zero
        js was_negative

        cmp eax, 10
        ja greater_than_ten

        add eax, 100
        jmp done

    greater_than_ten:
        add eax, 200
        jmp done

    was_negative:
        neg eax
        add eax, 300
        jmp done

    was_zero:
        mov eax, 400

    done:
        ret
    }
}


/*
 * x87 stack manipulation with explicit ST(i).
 */
double asm_x87_add(double a, double b)
{
    double result;

    __asm {
        fld a
        fld b

        faddp st(1), st(0)

        fstp result
    }

    return result;
}


/*
 * Larger x87 example:
 *   result = (a + b) * c
 */
double asm_x87_expression(double a, double b, double c)
{
    double result;

    __asm {
        fld a
        fld b
        faddp st(1), st(0)

        fld c
        fmulp st(1), st(0)

        fstp result
    }

    return result;
}


/*
 * MASM-style literals and byte/word/dword registers.
 */
unsigned asm_register_widths(unsigned value)
{
    unsigned result;

    __asm {
        mov eax, value

        xor ah, ah
        add al, 7Fh

        movzx ecx, ax
        xor ecx, 1234h

        mov result, ecx
    }

    return result;
}


/*
 * Indirect memory operand through a C pointer.
 */
int asm_load_pointer(const int *ptr)
{
    int result;

    __asm {
        mov ecx, ptr
        mov eax, dword ptr [ecx]
        mov result, eax
    }

    return result;
}


/*
 * Read-modify-write through a C pointer.
 */
void asm_increment_pointer(int *ptr)
{
    __asm {
        mov ecx, ptr
        add dword ptr [ecx], 1
    }
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
// DEFAULT-NEXT:     fn %[[VALUE_round_x87:[0-9]+]] @round_x87(%[[VALUE_value:[0-9]+]] value: f64) -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE0:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE0]]: u32 [synthetic];
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: i32 [storage=automatic];
// DEFAULT-NEXT:         asm volatile "fld value\nfistp result" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "fld " addr<qword>(%1) "\nfistp " addr<dword>(%2);
// DEFAULT-NEXT:             lateout 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE0]]);
// DEFAULT-NEXT:             in 1 [value] mem<read> place<f64>(%[[VALUE_value]]);
// DEFAULT-NEXT:             in 2 [result] mem<write> place<i32>(%[[VALUE_result]]);
// DEFAULT-NEXT:             clobbers: "st" as st, "st(1)" as st(1), "st(2)" as st(2), "st(3)" as st(3), "st(4)" as st(4), "st(5)" as st(5), "st(6)" as st(6), "st(7)" as st(7);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_result]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_have_cpuid:[0-9]+]] @have_cpuid() -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE1:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE1]]: u32 [synthetic];
// DEFAULT-NEXT:         let %[[VALUE_result_2:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         asm volatile "pushfd\npop eax\nmov ecx, eax\nxor eax, 2097152\npush eax\npopfd\npushfd\npop eax\nxor eax, ecx\njz done\nmov result, 1\ndone:" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "pushfd\npop eax\nmov ecx, eax\nxor eax, 2097152\npush eax\npopfd\npushfd\npop eax\nxor eax, ecx\njz " label(done) "\nmov " addr<dword>(%1) ", 1\n" entry_label(%4, done) ":";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE1]]);
// DEFAULT-NEXT:             in 1 [result] mem<write> place<i32>(%[[VALUE_result_2]]);
// DEFAULT-NEXT:             clobbers: "ecx" as cx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_result_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_round_x87:[0-9]+]] @asm_round_x87(%[[VALUE_value_2:[0-9]+]] value: f64) -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE2:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE2]]: u32 [synthetic];
// DEFAULT-NEXT:         let %[[VALUE_result_3:[0-9]+]] result: i32 [storage=automatic];
// DEFAULT-NEXT:         asm volatile "fld value\nfistp result" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "fld " addr<qword>(%1) "\nfistp " addr<dword>(%2);
// DEFAULT-NEXT:             lateout 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE2]]);
// DEFAULT-NEXT:             in 1 [value] mem<read> place<f64>(%[[VALUE_value_2]]);
// DEFAULT-NEXT:             in 2 [result] mem<write> place<i32>(%[[VALUE_result_3]]);
// DEFAULT-NEXT:             clobbers: "st" as st, "st(1)" as st(1), "st(2)" as st(2), "st(3)" as st(3), "st(4)" as st(4), "st(5)" as st(5), "st(6)" as st(6), "st(7)" as st(7);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_result_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_local_and_label:[0-9]+]] @asm_local_and_label(%[[VALUE_value_3:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE3:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE3]]: u32 [synthetic];
// DEFAULT-NEXT:         let %[[VALUE_result_4:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         asm volatile "mov eax, value\ntest eax, eax\njz zero_case\nadd eax, 10\nmov result, eax\njmp done\nzero_case:\nmov result, 4660\ndone:" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, " addr(%1) "\ntest eax, eax\njz " label(zero_case) "\nadd eax, 10\nmov " addr(%2) ", eax\njmp " label(done) "\n" entry_label(%10, zero_case) ":\nmov " addr<dword>(%2) ", 4660\n" entry_label(%11, done) ":";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE3]]);
// DEFAULT-NEXT:             in 1 [value] mem<read> place<i32>(%[[VALUE_value_3]]);
// DEFAULT-NEXT:             in 2 [result] mem<write> place<i32>(%[[VALUE_result_4]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_result_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_mul_u32:[0-9]+]] @asm_mul_u32(%[[VALUE_a:[0-9]+]] a: u32, %[[VALUE_b:[0-9]+]] b: u32) -> u64 [linkage=external] [naked] [fallthrough=ub] {
// DEFAULT-NEXT:         asm volatile "mov eax, dword ptr [esp + 4]\nmul dword ptr [esp + 8]\nret" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, dword ptr [esp + 4]\nmul dword ptr [esp + 8]\nret";
// DEFAULT-NEXT:             clobbers: "eax" as ax, "edx" as dx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_add_u64:[0-9]+]] @asm_add_u64(%[[VALUE_a_lo:[0-9]+]] a_lo: u32, %[[VALUE_a_hi:[0-9]+]] a_hi: u32, %[[VALUE_b_lo:[0-9]+]] b_lo: u32, %[[VALUE_b_hi:[0-9]+]] b_hi: u32) -> u64 [linkage=external] [naked] [fallthrough=ub] {
// DEFAULT-NEXT:         asm volatile "mov eax, dword ptr [esp + 4]\nmov edx, dword ptr [esp + 8]\nadd eax, dword ptr [esp + 12]\nadc edx, dword ptr [esp + 16]\nret" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, dword ptr [esp + 4]\nmov edx, dword ptr [esp + 8]\nadd eax, dword ptr [esp + 12]\nadc edx, dword ptr [esp + 16]\nret";
// DEFAULT-NEXT:             clobbers: "eax" as ax, "edx" as dx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_sub_u64:[0-9]+]] @asm_sub_u64(%[[VALUE_a_lo_2:[0-9]+]] a_lo: u32, %[[VALUE_a_hi_2:[0-9]+]] a_hi: u32, %[[VALUE_b_lo_2:[0-9]+]] b_lo: u32, %[[VALUE_b_hi_2:[0-9]+]] b_hi: u32) -> u64 [linkage=external] [naked] [fallthrough=ub] {
// DEFAULT-NEXT:         asm volatile "mov eax, dword ptr [esp + 4]\nmov edx, dword ptr [esp + 8]\nsub eax, dword ptr [esp + 12]\nsbb edx, dword ptr [esp + 16]\nret" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, dword ptr [esp + 4]\nmov edx, dword ptr [esp + 8]\nsub eax, dword ptr [esp + 12]\nsbb edx, dword ptr [esp + 16]\nret";
// DEFAULT-NEXT:             clobbers: "eax" as ax, "edx" as dx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_neg_i64:[0-9]+]] @asm_neg_i64(%[[VALUE_value_4:[0-9]+]] value: i64) -> i64 [linkage=external] [naked] [fallthrough=ub] {
// DEFAULT-NEXT:         asm volatile "mov eax, dword ptr [esp + 4]\nmov edx, dword ptr [esp + 8]\nneg eax\nadc edx, 0\nneg edx\nret" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, dword ptr [esp + 4]\nmov edx, dword ptr [esp + 8]\nneg eax\nadc edx, 0\nneg edx\nret";
// DEFAULT-NEXT:             clobbers: "eax" as ax, "edx" as dx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_shr_u64:[0-9]+]] @asm_shr_u64(%[[VALUE_value_5:[0-9]+]] value: u64) -> u64 [linkage=external] [naked] [fallthrough=ub] {
// DEFAULT-NEXT:         asm volatile "mov eax, dword ptr [esp + 4]\nmov edx, dword ptr [esp + 8]\nshr edx, 1\nrcr eax, 1\nret" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, dword ptr [esp + 4]\nmov edx, dword ptr [esp + 8]\nshr edx, 1\nrcr eax, 1\nret";
// DEFAULT-NEXT:             clobbers: "eax" as ax, "edx" as dx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_normalize_u64:[0-9]+]] @asm_normalize_u64(%[[VALUE_value_6:[0-9]+]] value: u64) -> u64 [linkage=external] [naked] [fallthrough=ub] {
// DEFAULT-NEXT:         asm volatile "mov eax, dword ptr [esp + 4]\nmov edx, dword ptr [esp + 8]\nnormalize:\ntest edx, edx\njz done\nshr edx, 1\nrcr eax, 1\njmp normalize\ndone:\nret" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, dword ptr [esp + 4]\nmov edx, dword ptr [esp + 8]\n" entry_label(%32, normalize) ":\ntest edx, edx\njz " label(done) "\nshr edx, 1\nrcr eax, 1\njmp " label(normalize) "\n" entry_label(%33, done) ":\nret";
// DEFAULT-NEXT:             clobbers: "eax" as ax, "edx" as dx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_div_u32:[0-9]+]] @asm_div_u32(%[[VALUE_numerator:[0-9]+]] numerator: u32, %[[VALUE_denominator:[0-9]+]] denominator: u32) -> u32 [linkage=external] [naked] [fallthrough=ub] {
// DEFAULT-NEXT:         asm volatile "mov eax, dword ptr [esp + 4]\nxor edx, edx\nmov ecx, dword ptr [esp + 8]\ndiv ecx\nret" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, dword ptr [esp + 4]\nxor edx, edx\nmov ecx, dword ptr [esp + 8]\ndiv ecx\nret";
// DEFAULT-NEXT:             clobbers: "eax" as ax, "ecx" as cx, "edx" as dx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_rem_u32:[0-9]+]] @asm_rem_u32(%[[VALUE_numerator_2:[0-9]+]] numerator: u32, %[[VALUE_denominator_2:[0-9]+]] denominator: u32) -> u32 [linkage=external] [naked] [fallthrough=ub] {
// DEFAULT-NEXT:         asm volatile "mov eax, dword ptr [esp + 4]\nxor edx, edx\nmov ecx, dword ptr [esp + 8]\ndiv ecx\nmov eax, edx\nret" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, dword ptr [esp + 4]\nxor edx, edx\nmov ecx, dword ptr [esp + 8]\ndiv ecx\nmov eax, edx\nret";
// DEFAULT-NEXT:             clobbers: "eax" as ax, "ecx" as cx, "edx" as dx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_manual_frame:[0-9]+]] @asm_manual_frame(%[[VALUE_value_7:[0-9]+]] value: i32) -> i32 [linkage=external] [naked] [fallthrough=ub] {
// DEFAULT-NEXT:         asm volatile "push ebp\nmov ebp, esp\nsub esp, 16\nmov eax, dword ptr [ebp + 8]\nadd eax, 5\nmov dword ptr [ebp - 4], eax\nmov eax, dword ptr [ebp - 4]\nleave\nret" [dialect=intel] {
// DEFAULT-NEXT:             template: "push ebp\nmov ebp, esp\nsub esp, 16\nmov eax, dword ptr [ebp + 8]\nadd eax, 5\nmov dword ptr [ebp - 4], eax\nmov eax, dword ptr [ebp - 4]\nleave\nret";
// DEFAULT-NEXT:             clobbers: "eax" as ax, "ebp" as bp;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_branch_graph:[0-9]+]] @asm_branch_graph(%[[VALUE_value_8:[0-9]+]] value: i32) -> i32 [linkage=external] [naked] [fallthrough=ub] {
// DEFAULT-NEXT:         asm volatile "mov eax, dword ptr [esp + 4]\ntest eax, eax\njz was_zero\njs was_negative\ncmp eax, 10\nja greater_than_ten\nadd eax, 100\njmp done\ngreater_than_ten:\nadd eax, 200\njmp done\nwas_negative:\nneg eax\nadd eax, 300\njmp done\nwas_zero:\nmov eax, 400\ndone:\nret" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov eax, dword ptr [esp + 4]\ntest eax, eax\njz " label(was_zero) "\njs " label(was_negative) "\ncmp eax, 10\nja " label(greater_than_ten) "\nadd eax, 100\njmp " label(done) "\n" entry_label(%44, greater_than_ten) ":\nadd eax, 200\njmp " label(done) "\n" entry_label(%45, was_negative) ":\nneg eax\nadd eax, 300\njmp " label(done) "\n" entry_label(%46, was_zero) ":\nmov eax, 400\n" entry_label(%47, done) ":\nret";
// DEFAULT-NEXT:             clobbers: "eax" as ax;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_x87_add:[0-9]+]] @asm_x87_add(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_5:[0-9]+]] result: f64 [storage=automatic];
// DEFAULT-NEXT:         asm volatile "fld a\nfld b\nfaddp st(1), st(0)\nfstp result" [dialect=intel] {
// DEFAULT-NEXT:             template: "fld " addr<qword>(%0) "\nfld " addr<qword>(%1) "\nfaddp st(1), st(0)\nfstp " addr<qword>(%2);
// DEFAULT-NEXT:             in 0 [a] mem<read> place<f64>(%[[VALUE_a_2]]);
// DEFAULT-NEXT:             in 1 [b] mem<read> place<f64>(%[[VALUE_b_2]]);
// DEFAULT-NEXT:             in 2 [result] mem<write> place<f64>(%[[VALUE_result_5]]);
// DEFAULT-NEXT:             clobbers: "st" as st, "st(1)" as st(1), "st(2)" as st(2), "st(3)" as st(3), "st(4)" as st(4), "st(5)" as st(5), "st(6)" as st(6), "st(7)" as st(7);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<f64>(%[[VALUE_result_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_x87_expression:[0-9]+]] @asm_x87_expression(%[[VALUE_a_3:[0-9]+]] a: f64, %[[VALUE_b_3:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result_6:[0-9]+]] result: f64 [storage=automatic];
// DEFAULT-NEXT:         asm volatile "fld a\nfld b\nfaddp st(1), st(0)\nfld c\nfmulp st(1), st(0)\nfstp result" [dialect=intel] {
// DEFAULT-NEXT:             template: "fld " addr<qword>(%0) "\nfld " addr<qword>(%1) "\nfaddp st(1), st(0)\nfld " addr<qword>(%2) "\nfmulp st(1), st(0)\nfstp " addr<qword>(%3);
// DEFAULT-NEXT:             in 0 [a] mem<read> place<f64>(%[[VALUE_a_3]]);
// DEFAULT-NEXT:             in 1 [b] mem<read> place<f64>(%[[VALUE_b_3]]);
// DEFAULT-NEXT:             in 2 [c] mem<read> place<f64>(%[[VALUE_c]]);
// DEFAULT-NEXT:             in 3 [result] mem<write> place<f64>(%[[VALUE_result_6]]);
// DEFAULT-NEXT:             clobbers: "st" as st, "st(1)" as st(1), "st(2)" as st(2), "st(3)" as st(3), "st(4)" as st(4), "st(5)" as st(5), "st(6)" as st(6), "st(7)" as st(7);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<f64>(%[[VALUE_result_6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_register_widths:[0-9]+]] @asm_register_widths(%[[VALUE_value_9:[0-9]+]] value: u32) -> u32 [linkage=external] [fallthrough=ret(read<u32>(%[[VALUE4:[0-9]+]]))] {
// DEFAULT-NEXT:         let %[[VALUE4]]: u32 [synthetic];
// DEFAULT-NEXT:         let %[[VALUE_result_7:[0-9]+]] result: u32 [storage=automatic];
// DEFAULT-NEXT:         asm volatile "mov eax, value\nxor ah, ah\nadd al, 127\nmovzx ecx, ax\nxor ecx, 4660\nmov result, ecx" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, " addr(%1) "\nxor ah, ah\nadd al, 127\nmovzx ecx, ax\nxor ecx, 4660\nmov " addr(%2) ", ecx";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE4]]);
// DEFAULT-NEXT:             in 1 [value] mem<read> place<u32>(%[[VALUE_value_9]]);
// DEFAULT-NEXT:             in 2 [result] mem<write> place<u32>(%[[VALUE_result_7]]);
// DEFAULT-NEXT:             clobbers: "ecx" as cx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_result_7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_load_pointer:[0-9]+]] @asm_load_pointer(%[[VALUE_ptr:[0-9]+]] ptr: ptr<const i32>) -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE5:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE5]]: u32 [synthetic];
// DEFAULT-NEXT:         let %[[VALUE_result_8:[0-9]+]] result: i32 [storage=automatic];
// DEFAULT-NEXT:         asm volatile "mov ecx, ptr\nmov eax, dword ptr [ecx]\nmov result, eax" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov ecx, " addr(%1) "\nmov eax, dword ptr [ecx]\nmov " addr(%2) ", eax";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE5]]);
// DEFAULT-NEXT:             in 1 [ptr] mem<read> place<ptr<const i32>>(%[[VALUE_ptr]]);
// DEFAULT-NEXT:             in 2 [result] mem<write> place<i32>(%[[VALUE_result_8]]);
// DEFAULT-NEXT:             clobbers: "ecx" as cx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_result_8]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_asm_increment_pointer:[0-9]+]] @asm_increment_pointer(%[[VALUE_ptr_2:[0-9]+]] ptr: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "mov ecx, ptr\nadd dword ptr [ecx], 1" [dialect=intel] {
// DEFAULT-NEXT:             template: "mov ecx, " addr(%0) "\nadd dword ptr [ecx], 1";
// DEFAULT-NEXT:             in 0 [ptr] mem<read> place<ptr<i32>>(%[[VALUE_ptr_2]]);
// DEFAULT-NEXT:             clobbers: "ecx" as cx;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
