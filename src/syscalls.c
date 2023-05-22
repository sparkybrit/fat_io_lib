#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

#undef errno
extern int errno;
extern char _end;
extern int uart_putc(char ch);
extern int uart_getc(void);
extern volatile clock_t milliseconds;

register char * stack_ptr asm("%sp");

char *__env[1] = { 0 };
char **environ = __env;


int getpid(void)
{
	return 1;
}

__attribute__((weak)) 
int read(int file, char *ptr, int len)
{
	for (int i = 0; i < len; i++)
		*ptr++ = uart_getc();

    return len;
}

__attribute__((weak)) 
int write(int file, char *ptr, int len)
{
	for (int i = 0; i < len; i++)
		uart_putc(*ptr++);

	return len;
}

int close(int file)
{
	return -1;
}


int fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int isatty(int file)
{
	return 1;
}

int lseek(int file, int ptr, int dir)
{
	return 0;
}

int open(char *path, int flags, ...)
{
	/* Pretend like we always fail */
	return -1;
}

clock_t times(struct tms *buf)
{
	buf->tms_stime = milliseconds;
	buf->tms_utime = 0;
	buf->tms_cutime = 0;
	buf->tms_cstime = 0;
	return milliseconds;
}

int stat(const char *file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

/**
 _sbrk
 Increase program data space. Malloc and related functions depend on this
**/
caddr_t sbrk(int incr)
{
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0)
		heap_end = &_end;

	prev_heap_end = heap_end;

	if (heap_end + incr > stack_ptr)
	{
		errno = ENOMEM;
		return (caddr_t) -1;
	}

	heap_end += incr;

	return (caddr_t) prev_heap_end;
}
