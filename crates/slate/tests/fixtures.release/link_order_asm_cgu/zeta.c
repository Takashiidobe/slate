extern const int zeta_table[];

__asm__(".pushsection .rodata\n"
        ".balign 4\n"
        "zeta_table:\n"
        ".long 5, 7\n"
        ".popsection");

int zeta_sum(void) { return zeta_table[0] + zeta_table[1]; }
