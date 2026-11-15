	.globl seven
seven:
	movl $7, %eax
	ret

	.globl __x86_indirect_thunk_r11
__x86_indirect_thunk_r11:
	jmp *%r11

	.globl __x86_return_thunk
__x86_return_thunk:
	ret
