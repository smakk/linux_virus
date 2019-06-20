void virus_exit(long status)
{
	__asm__ volatile(
		"mov %0, %%rdi\n"
		"mov $60, %%rax\n"
		"syscall" : : "r"(status));
}

void _start()
{
	virus_exit(0);
}
