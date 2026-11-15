	.globl seven
seven:
	movl $7, %eax
	ret

	.globl __x86_indirect_thunk_r11
__x86_indirect_thunk_r11:
	jmp *%r11
