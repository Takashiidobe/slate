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
