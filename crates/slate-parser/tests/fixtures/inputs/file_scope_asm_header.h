int header_target(int x);
asm(".pushsection .static_call.text, \"ax\"\n"
    ".globl __SCT__header_call\n"
    "__SCT__header_call:\n"
    ".byte 0xe9; .long header_target - (. + 4)\n"
    ".popsection\n");
