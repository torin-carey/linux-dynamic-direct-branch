#define pr_fmt(fmt) "ddb: " fmt

#include <linux/module.h>

MODULE_LICENSE("GPL");

#if !IS_ENABLED(CONFIG_X86_64)
#error "This example only supports x86-64"
#endif

static int write_call(void *func, const void *target)
{
	long rel = (unsigned long)(
		(char *)target - (char *)func - 5
	);
	int rel32 = (int)rel;
	unsigned long flags;
	unsigned long cr0;
	char *base = (char *)func;
	pr_info("local_func : %px\n", func);
	pr_info("target     : %px\n", target);
	pr_info("rel        : %016lx\n", rel);
	if (rel > (long)INT_MAX || rel < (long)INT_MIN) {
		// Can't do a jmp rel32
		pr_err("relative offset outside 32 bits\n");
		return -EINVAL;
	}
	pr_info(":: e9 %02hhx %02hhx %02hhx %02hhx\n",
		(char)rel32,
		(char)(rel32>>8),
		(char)(rel32>>16),
		(char)(rel32>>24));

	local_irq_save(flags);
	asm volatile("mov %%cr0, %0" : "=r" (cr0));
	cr0 &= ~X86_CR0_WP;
	asm volatile("mov %0, %%cr0" : "+r" (cr0) : : "memory");
	*base++ = 0xe9;
	*base++ = (char)rel32;
	*base++ = (char)(rel32>>8);
	*base++ = (char)(rel32>>16);
	*base++ = (char)(rel32>>24);
	cr0 |= X86_CR0_WP;
	asm volatile("mov %0, %%cr0" : "+r" (cr0) : : "memory");
	local_irq_restore(flags);

	return 0;
}

int my_printk(const char *fmt, ...);

int mod_init(void)
{
	write_call(&my_printk, &_printk);
	my_printk(KERN_INFO "Hello, World!\n");
	return 0;
}
module_init(mod_init);

void mod_exit(void)
{
	pr_info("removing module\n");
}
module_exit(mod_exit);
