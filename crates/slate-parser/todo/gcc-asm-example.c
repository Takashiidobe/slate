#include <stdint.h>

typedef float v4sf __attribute__((vector_size(16)));


/*
 * File-scope/basic asm.
 */
__asm__(
    ".globl slate_global_asm_marker\n"
    "slate_global_asm_marker:\n"
    ".byte 0\n"
);

void asm_basic(void)
{
    __asm__("nop");
}


/*
 * Read/write operand.
 *
 * Tests:
 *   + constraint
 *   register allocation
 *   volatile
 *   cc clobber
 */
int asm_read_write(int x)
{
    __asm__ volatile (
        "addl $7, %0"
        : "+r"(x)
        :
        : "cc"
    );

    return x;
}


/*
 * Explicit tied operand.
 *
 * Output %0 and input %1 must occupy the same location.
 */
int asm_tied(int x, int y)
{
    int out;

    __asm__ (
        "addl %2, %0"
        : "=r"(out)
        : "0"(x), "r"(y)
        : "cc"
    );

    return out;
}


/*
 * Early-clobber.
 *
 * The output is written before all inputs have been consumed,
 * so it must not overlap them.
 */
int asm_earlyclobber(int a, int b)
{
    int out;

    __asm__ (
        "movl %1, %0\n\t"
        "addl %2, %0"
        : "=&r"(out)
        : "r"(a), "r"(b)
        : "cc"
    );

    return out;
}


/*
 * Symbolically named operands.
 */
int asm_symbolic(int a, int b)
{
    int out;

    __asm__ (
        "movl %[lhs], %[dst]\n\t"
        "subl %[rhs], %[dst]"
        : [dst] "=&r"(out)
        : [lhs] "r"(a),
          [rhs] "r"(b)
        : "cc"
    );

    return out;
}


/*
 * Multiple constraint alternatives.
 *
 * Alternative 0:
 *      output register, input memory
 *
 * Alternative 1:
 *      output memory, input register
 */
int asm_multi_alternative(int x)
{
    int out;

    __asm__ (
        "movl %1, %0"
        : "=r,m"(out)
        : "m,r"(x)
    );

    return out;
}


/*
 * Memory read-modify-write.
 *
 * Tests:
 *   +m
 *   register/immediate alternative "Ir"
 *   flags
 *   memory clobber
 */
unsigned asm_memory_rmw(unsigned *base, unsigned bit)
{
    unsigned old;

    __asm__ (
        "btsl %2, %1\n\t"
        "sbbl %0, %0"
        : "=r"(old),
          "+m"(*base)
        : "Ir"(bit)
        : "cc", "memory"
    );

    return old;
}


/*
 * x86 operand modifiers.
 *
 * %b0 = low 8-bit register
 * %h0 = high 8-bit register
 *
 * "+a" forces AX/EAX/RAX family, which is required because AH exists
 * only for the legacy four GPRs.
 */
uint16_t asm_high_low_byte(uint16_t x)
{
    __asm__ (
        "xchg %h0, %b0"
        : "+a"(x)
    );

    return x;
}


/*
 * Width modifier.
 *
 * Operand itself is 64-bit, but %k prints its 32-bit register name.
 */
uint64_t asm_width_modifier(uint64_t x)
{
    uint64_t out;

    __asm__ (
        "movl %k1, %k0"
        : "=r"(out)
        : "r"(x)
    );

    return out;
}


/*
 * Flag output operand.
 *
 * There is no textual %0 in the asm template.
 * The output comes from the condition codes.
 */
unsigned asm_flag_output(unsigned a, unsigned b)
{
#if defined(__GCC_ASM_FLAG_OUTPUTS__)
    unsigned below;

    __asm__ (
        "cmpl %2, %1"
        : "=@ccb"(below)
        : "r"(a), "r"(b)
    );

    return below;
#else
    return a < b;
#endif
}


/*
 * %= generates a unique number for this asm instance.
 *
 * Particularly useful if the asm is in an inline function which
 * gets instantiated multiple times.
 */
int asm_unique_label(int x)
{
    __asm__ volatile (
        "testl %[x], %[x]\n\t"
        "jz .Ldone%=\n\t"
        "incl %[x]\n"
        ".Ldone%=:\n\t"
        : [x] "+r"(x)
        :
        : "cc"
    );

    return x;
}


/*
 * Fixed-register constraints + tied fixed-register inputs.
 *
 * CPUID takes:
 *      EAX = leaf
 *      ECX = subleaf
 *
 * and produces EAX, EBX, ECX, EDX.
 */
void asm_cpuid(unsigned leaf, unsigned subleaf, unsigned out[4])
{
    unsigned a;
    unsigned b;
    unsigned c;
    unsigned d;

    __asm__ volatile (
        "cpuid"
        : "=a"(a),
          "=b"(b),
          "=c"(c),
          "=d"(d)
        : "0"(leaf),
          "2"(subleaf)
        : "cc"
    );

    out[0] = a;
    out[1] = b;
    out[2] = c;
    out[3] = d;
}


/*
 * SIMD register constraint.
 */
v4sf asm_sse(v4sf a, v4sf b)
{
    __asm__ (
        "addps %1, %0"
        : "+x"(a)
        : "x"(b)
    );

    return a;
}


/*
 * asm goto.
 *
 * %l[name] expands to the compiler-generated assembler label
 * corresponding to the C label.
 */
int asm_goto_zero(int x)
{
    __asm__ goto (
        "testl %0, %0\n\t"
        "jz %l[zero]"
        :
        : "r"(x)
        : "cc"
        : zero
    );

    return 1;

zero:
    return 0;
}


/*
 * asm goto WITH an output.
 *
 * This is particularly important for Clang compatibility.
 */
int asm_goto_output(int x)
{
    __asm__ goto (
        "addl $1, %0\n\t"
        "jo %l[overflow]"
        : "+r"(x)
        :
        : "cc"
        : overflow
    );

    return x;

overflow:
    return -1;
}


/*
 * AT&T / Intel template alternatives.
 *
 * Compile this function with both:
 *
 *     -masm=att
 *     -masm=intel
 *
 * GCC documents {att | intel} alternatives specifically for x86
 * inline asm templates.
 */
unsigned asm_dialect(unsigned base, unsigned bit)
{
    unsigned old;

    __asm__ volatile (
        "bt{l %[bit],%[base] | %[base],%[bit]}\n\t"
        "sbb{l %[old],%[old] | %[old],%[old]}"
        : [old] "=r"(old),
          [base] "+r"(base)
        : [bit] "Ir"(bit)
        : "cc"
    );

    return old;
}


/*
 * Explicit architectural register clobber.
 */
void asm_explicit_register_clobber(uint64_t x)
{
    __asm__ volatile (
        ""
        :
        : "r"(x)
        : "r10", "memory"
    );
}


/*
 * Literal '%' inside an extended asm template.
 *
 * %%eax -> %eax in the final assembler text.
 */
void asm_escaped_percent(void)
{
    __asm__ volatile (
        "xorl %%eax, %%eax"
        :
        :
        : "eax", "cc"
    );
}
