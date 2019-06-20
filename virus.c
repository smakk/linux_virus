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

void virus_pathadd(char *path, char *dir, char *name){
	int i=0;
	int j=0;	
	for(;i<virus_strlen(dir);i++) path[i] = dir[i];
	//path[i++] = '/';
	for(;j<virus_strlen(name);j++,i++) path[i] = name[j];
	path[i] = '\0';
}

void getfullpath(){
	
}

void infect_file(char *file){
	virus_write(1, file,virus_strlen(file));
	virus_write(1, "\n", 1);
}

//dir must end with /
void infect_dir(char* path){
	virus_write(1, path,virus_strlen(path));
	virus_write(1, "\n", 1);
	struct linux_dirent64 *d;
	char buf[1024*1024];
	int bpos;
	int dir = virus_open(path,O_RDONLY | O_DIRECTORY, 0);
	int nread = virus_getdents64(dir, (struct linux_dirent64 *)buf, 1024*1024);
	for (bpos = 0; bpos < nread; bpos++) {
		d = (struct linux_dirent64 *) (buf + bpos);
		bpos += d->d_reclen - 1;
		char fullpath[256];
		virus_pathadd(fullpath, path, d->d_name);
		//ignore all hide file .. and .
		if(d->d_name[0] == '.') continue;
		if(d->d_type == DT_REG){
			infect_file(fullpath);
		}else if(d->d_type == DT_DIR){
			int endpos = virus_strlen(fullpath);
			fullpath[endpos] = '/';
			fullpath[endpos+1] = '\0';
			infect_dir(fullpath);
		}
		/*
		virus_write(1, d->d_name,virus_strlen(d->d_name));
		virus_write(1, "\n", 1);
		*/
        }
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

	/*
	char buf[1024*1024];
	char *start[1] = {"/home/lkm/workspace/virus/test"};
	int dir = virus_open(start[0],O_RDONLY | O_DIRECTORY, 0);
	char dirtmp[20];
	//virus_write(1, virus_itoa(dir,dirtmp),13);
	if(dir<0) virus_write(1,"dir wrong\n",13);
	int nread = virus_getdents64(dir, (struct linux_dirent64 *)buf, 1024*1024);
	if(nread<0) virus_write(1,"nread wrong\n",13);
	
	struct linux_dirent64 *d;
	int bpos;
	//virus_write(1, virus_itoa(nread,tmp), 20);
	//virus_write(1, "\n", 20);
	for (bpos = 0; bpos < nread; bpos++) {
		d = (struct linux_dirent64 *) (buf + bpos);
		bpos += d->d_reclen - 1;
		if(d->d_type == DT_REG){
			char fullpath[50];
			getfullpath();
		}else if(d->d_type == DT_REG){
			
		}
		virus_write(1, d->d_name,virus_strlen(d->d_name));
		virus_write(1, "\n", 1);
        }
	*/
	char *start[1] = {"/home/lkm/workspace/virus/test/"};
	infect_dir(start[0]);
	virus_exit(0);
}
