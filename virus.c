#include <fcntl.h>

#include <dirent.h>     /* Defines DT_* constants */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/syscall.h>

struct linux_dirent64 {
	long		d_ino;
	long		d_off;
	unsigned short	d_reclen;
	unsigned char	d_type;
	char		d_name[0];
};

char * virus_itoa(long x, char *t)
{
	int i;
	int j;
	i = 0;
	do
	{
		t[i] = (x % 10) + '0';
		x /= 10;
		i++;
	} while (x!=0);
	t[i] = 0;
	for (j=0; j < i / 2; j++) {
		t[j] ^= t[i - j - 1];
		t[i - j - 1] ^= t[j];
		t[j] ^= t[i - j - 1];
	}
	return t;
}

size_t virus_strlen(char *s)
{
	size_t size;
	for (size=0;s[size];size++);
	return size;
}

int virus_getdents64(unsigned int fd, struct linux_dirent64 *dirp,unsigned int count)
{
	long ret;
	__asm__ volatile(
		"mov %0, %%rdi\n"
		"mov %1, %%rsi\n"
		"mov %2, %%rdx\n"
		"mov $217, %%rax\n"
		"syscall" :: "g"(fd), "g"(dirp), "g"(count));
	asm ("mov %%rax, %0" : "=r"(ret));
	return (int)ret;
}

long virus_open(const char *path, unsigned long flags, long mode)
{
	long ret;
	__asm__ volatile(
		"mov %0, %%rdi\n"
		"mov %1, %%rsi\n"
		"mov %2, %%rdx\n"
		"mov $2, %%rax\n"
		"syscall" : : "g"(path), "g"(flags), "g"(mode));
	asm ("mov %%rax, %0" : "=r"(ret));
	return ret;
}

int virus_read(long fd, char *buf, unsigned long len)
{
	long ret;
	__asm__ volatile(
		"mov %0, %%rdi\n"
		"mov %1, %%rsi\n"
		"mov %2, %%rdx\n"
		"mov $0, %%rax\n"
		"syscall" : : "g"(fd), "g"(buf), "g"(len));
	asm("mov %%rax, %0" : "=r"(ret));
	return (int)ret;
}

long virus_write(long fd, char *buf, unsigned long len)
{
	long ret;
	__asm__ volatile(
		"mov %0, %%rdi\n"
		"mov %1, %%rsi\n"
		"mov %2, %%rdx\n"
		"mov $1, %%rax\n"
		"syscall" : : "g"(fd), "g"(buf), "g"(len));
	asm("mov %%rax, %0" : "=r"(ret));
	return ret;
}

int virus_close(unsigned int fd)
{
	long ret;
	__asm__ volatile(
		"mov %0, %%rdi\n"
		"mov $3, %%rax\n"
		"syscall" : : "g"(fd));
	return (int)ret;
}

void virus_exit(long status)
{
	__asm__ volatile(
		"mov %0, %%rdi\n"
		"mov $60, %%rax\n"
		"syscall" : : "r"(status));
}

void _start()
{
	/*
	char c[100];
	int file = virus_open("./test/test",O_RDONLY, 0);
	virus_read(file,c,100);
	virus_write(1,c,100);
	virus_close(file);
	virus_exit(0);
	*/
	char buf[1024*1024];
	int dir = virus_open("./test",O_RDONLY | O_DIRECTORY, 0);
	char dirtmp[20];
	//virus_write(1, virus_itoa(dir,dirtmp),13);
	if(dir<0) virus_write(1,"dir wrong\n",13);
	int nread = virus_getdents64(dir, (struct linux_dirent64 *)buf, 1024*1024);
	if(nread<0) virus_write(1,"nread wrong\n",13);

	/*
	int i;
	for(i=0; i<num; i++){
		struct linux_dirent64 *d = (struct linux_dirent64 *)(c + i);
		int dlen = d->d_reclen - 1;
		virus_write(1,d->d_name,dlen);
	}
	*/
	
	struct linux_dirent64 *d;
	int bpos;
	char d_type;
	char tmp[20];
	//virus_write(1, virus_itoa(nread,tmp), 20);
	//virus_write(1, "\n", 20);
	for (bpos = 0; bpos < nread; bpos++) {
		d = (struct linux_dirent64 *) (buf + bpos);
		bpos += d->d_reclen - 1;
		//virus_write(1,"get a file\n\0", 11);
		virus_write(1, d->d_name,virus_strlen(d->d_name));
		virus_write(1, "\n", 1);
		/*
		virus_write(1, virus_itoa(d->d_ino,tmp),20);
		virus_write(1, "\n", 20);
		virus_write(1, virus_itoa(d->d_type,tmp),20);
		virus_write(1, "\n", 20);
		virus_write(1, virus_itoa(d->d_reclen,tmp),20);
		virus_write(1, "\n", 20);
		virus_write(1, virus_itoa(d->d_off,tmp),20);
		virus_write(1, "\n", 20);
		*/
		//char name[20];
		
		//virus_write(1, d->d_name, 15);
		
		//bpos += d->d_reclen;
        }
	
	virus_exit(0);
}
