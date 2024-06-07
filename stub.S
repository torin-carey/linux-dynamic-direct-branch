#include <linux/linkage.h>

SYM_CODE_START(my_printk)
	// Enough room for a jmp rel32 instruction
	nop; nop; nop; nop; nop
SYM_CODE_END(my_printk)
